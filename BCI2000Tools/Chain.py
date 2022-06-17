# -*- coding: utf-8 -*-
# 
#   $Id: Chain.py 5501 2016-08-09 14:27:52Z jhill $
#   
#   This file is part of the BCPy2000 framework, a Python framework for
#   implementing modules that run on top of the BCI2000 <http://bci2000.org/>
#   platform, for the purpose of realtime biosignal processing.
# 
#   Copyright (C) 2007-12  Jeremy Hill, Thomas Schreiner,
#                          Christian Puzicha, Jason Farquhar
#   
#   bcpy2000@bci2000.org
#   
#   The BCPy2000 framework is free software: you can redistribute it
#   and/or modify it under the terms of the GNU General Public License
#   as published by the Free Software Foundation, either version 3 of
#   the License, or (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
__all__ = [
	'bci2000chain', 'bci2000root', 'bci2000path',
]

import os,sys,tempfile,time
import numpy
from .Parameters import make_bciprm, read_bcidate

try: import scipy, SigTools # used for sstruct (could be replaced by dict),  ChannelSet (could be omitted)  and loadmat (required - introduces a scipy dependency)
except ImportError:
	try: import scipy, BCPy2000.SigTools as SigTools
	except ImportError: SigTools = None

if SigTools:
	CONTAINER = SigTools.sstruct
	ReadMatFile = SigTools.loadmat
else:
	from .LoadStream2Mat import ReadMatFile, Bunch
	CONTAINER = Bunch


dat2stream_has_p_flag = 1  # new-style bci_dat2stream has a -p flag allowing it to read a .prm file to override parameter values that came from the .dat file. Without this, some source parameters like SampleBlockSize and SourceChGain cannot be overridden.
stream2mat_saves_parms = 1 # new-style bci_stream2mat saves a string representation of the collated output parameters in the mat file, so we don't have to rely on a separate parameter file.

BCI2000_ROOT_DIR = None

def bci2000root( set=None ):
	global BCI2000_ROOT_DIR
	if set != None: BCI2000_ROOT_DIR = os.path.realpath( set )
	return BCI2000_ROOT_DIR

def bci2000path( filename ):
	if isinstance( filename, str ) and isinstance( BCI2000_ROOT_DIR, str ) and not os.path.exists( filename ) and os.path.exists( os.path.join( BCI2000_ROOT_DIR, filename ) ):
		filename = os.path.realpath( os.path.join( BCI2000_ROOT_DIR, filename ) )
	return filename

def bci2000chain(datfile, chain='SpectralSignalProcessing', parms=(), dims='auto', start=None, duration=None, verbose=False, keep=False, binpath=None, **kwargs):
	"""
	
	This is a provisional Python port of the Matlab-based tools/matlab/bci2000chain.m
	
	Excuse the relative paths - this example works if you're currently working in the root
	directory of the BCI2000 distro:
	
	    bci2000chain( datfile='data/samplefiles/eeg3_2.dat',
	                  chain='TransmissionFilter|SpatialFilter|ARFilter',
	                  binpath='tools/cmdline',
	                  parms=[ 'tools/matlab/ExampleParameters.prm' ], SpatialFilterType=3 )
	
	Or for more portable operation, you can do this kind of thing:
	
	    bci2000root( '/PATH/TO/BCI2000' )
	    
	    bci2000chain(datfile=bci2000path( 'data/samplefiles/eeg3_2.dat' ),
	                 chain='TransmissionFilter|SpatialFilter|ARFilter',
	                 parms=[ bci2000path( 'tools/matlab/ExampleParameters.prm' ) ], SpatialFilterType=3 )
	
	
	For clues, see http://www.bci2000.org/wiki/index.php/User_Reference:Matlab_Tools
	and matlab documentation in bci2000chain.m
	
	Most arguments are like the flags in the Matlab equivalent. mutatis to a large extent mutandis.
	Note that for now there is no global way of managing the system path. Either add the
	tools/cmdline directory to your $PATH variable before starting Python, or supply its location
	every time while calling, in the <binpath> argument.

	"""###
	
	if verbose: verbosityFlag = '-v'
	else: verbosityFlag = '-q'
	
	if isinstance(chain, str):
		if chain.lower() == 'SpectralSignalProcessing'.lower():
			chain = 'TransmissionFilter|SpatialFilter|SpectralEstimator|LinearClassifier|LPFilter|ExpressionFilter|Normalizer'
		elif chain.lower() == 'ARSignalProcessing'.lower():
			chain = 'TransmissionFilter|SpatialFilter|ARFilter|LinearClassifier|LPFilter|ExpressionFilter|Normalizer'
		elif chain.lower() == 'P3SignalProcessing'.lower():
			chain = 'TransmissionFilter|SpatialFilter|P3TemporalFilter|LinearClassifier'
		chain = chain.split('|')
	chain = [c.strip() for c in chain if len(c.strip())]
	
	if len(chain) == 0: print('WARNING: chain is empty')
	
	if start != None and len(str(start).strip()): start = ' --start=' + str(start).replace(' ', '')
	else: start = ''
	if duration != None and len(str(duration).strip()): duration = ' --duration=' + str(duration).replace(' ', '')
	else: duration = ''
	
	if dims == None or str(dims).lower() == 'auto': dims = 0
	if dims not in (0, 2, 3): raise ValueError("dims must be 2, 3 or 'auto'")
	
	out = CONTAINER()
	err = ''
	
	cmd = ''
	binaries = []
	tmpdir = tempfile.mkdtemp(prefix='bci2000chain_')
	
	tmpdatfile  = os.path.join(tmpdir, 'in.dat')
	prmfile_in  = os.path.join(tmpdir, 'in.prm')
	prmfile_out = os.path.join(tmpdir, 'out.prm')
	matfile     = os.path.join(tmpdir, 'out.mat')
	bcifile     = os.path.join(tmpdir, 'out.bci')
	shfile      = os.path.join(tmpdir, 'go.bat')
	logfile     = os.path.join(tmpdir, 'log.txt')
	
	if not isinstance(datfile, str):
		raise ValueError('datfile must be a filename') # TODO: if datfile contains the appropriate info, use some create_bcidat equivalent and do datfile = tmpdatfile

	if not os.path.isfile(datfile): raise IOError('file not found: %s' % datfile)
	
	mappings = {
		'$DATFILE':     datfile,
		'$PRMFILE_IN':  prmfile_in,
		'$PRMFILE_OUT': prmfile_out,
		'$MATFILE':     matfile,
		'$BCIFILE':     bcifile,
		'$SHFILE':      shfile,
		'$LOGFILE':     logfile,
	}
	
	if binpath == None and BCI2000_ROOT_DIR != None: binpath = 'tools/cmdline'
	binpath = bci2000path( binpath )
	def exe(name):
		if binpath: return '"' + os.path.join(binpath, name) + '"'
		else: return name
	
	if parms == None: parms = []
	if isinstance(parms, tuple): parms = list(parms)
	if not isinstance(parms, list): parms = [parms]
	else: parms = list(parms)
	if len(kwargs): parms.append(kwargs)
	
	if parms == None or len(parms) == 0:
		cmd += exe('bci_dat2stream') + start + duration + ' < "$DATFILE"'
		binaries.append('bci_dat2stream')
	else:
		if verbose: print('# writing custom parameter file %s' % prmfile_in)
		parms = make_bciprm(verbosityFlag, datfile, parms, '>', prmfile_in)
		
		if dat2stream_has_p_flag:
			cmd += exe('bci_dat2stream') + ' "-p$PRMFILE_IN"' + start + duration + ' < "$DATFILE"' # new-style bci_dat2stream with -p option
			binaries.append('bci_dat2stream')
		else:
			if len(start) or len(duration):
				raise ValueError('old versionsof bci_dat2stream have no --start or --duration option')
			cmd += '('   # old-style bci_dat2stream with no -p option
			cmd +=         exe('bci_prm2stream') + ' < "$PRMFILE_IN"'
			cmd += '&& ' + exe('bci_dat2stream') + ' --transmit-sd < "$DATFILE"'
			cmd += ')'
			binaries.append('bci_dat2stream')
			binaries.append('bci_prm2stream')

	for c in chain:
		cmd += ' | ' + exe(c)
		binaries.append(c)

	if stream2mat_saves_parms:
		cmd += ' | ' + exe('bci_stream2mat') + ' > "$MATFILE"' # new-style bci_stream2mat with Parms output
		binaries.append('bci_stream2mat')
	else:
		cmd += ' > "$BCIFILE"'
		cmd += ' && ' + exe('bci_stream2mat') + ' < "$BCIFILE" > "$MATFILE"'
		cmd += ' && ' + exe('bci_stream2prm') + ' < "$BCIFILE" > "$PRMFILE_OUT"' # old-style bci_stream2mat without Parms output
		binaries.append('bci_stream2mat')
		binaries.append('bci_stream2prm')
	
	for k,v in list(mappings.items()): cmd = cmd.replace(k, v)
	
	win = sys.platform.lower().startswith('win')
	if win: shebang = '@'
	else: shebang = '#!/bin/sh\n\n'
	open(shfile, 'wt').write(shebang+cmd+'\n')
	if not win: os.chmod(shfile, 484) # rwxr--r--
	
	def tidytext(x):
		return x.strip().replace('\r\n', '\n').replace('\r', '\n')
	def getstatusoutput(cmd):
		pipe = os.popen(cmd + ' 2>&1', 'r') # TODO: does this work on Windows? could always make use of logfile here if not
		output = pipe.read()
		status = pipe.close()
		if status == None: status = 0
		return status,tidytext(output)
	def getoutput(cmd):
		return getstatusoutput(cmd)[1]

	if verbose: print('# querying version information')	
	binaries = CONTAINER([(bin, getoutput(exe(bin) + ' --version').replace('\n', '  ') ) for bin in binaries])

	if verbose: print(cmd)
	t0 = time.time()	
	failed,output = getstatusoutput(shfile)
	chaintime = time.time() - t0
	
	failsig = 'Configuration Error: '
	if failsig in output: failed = 1
	printable_output = output
	printable_lines = output.split('\n')
	maxlines = 10
	if len(printable_lines) > maxlines:
		printable_output = '\n'.join(printable_lines[:maxlines] + ['[%d more lines omitted]' % (len(printable_lines) - maxlines)])
	if failed:
		if verbose: err = 'system call failed:\n' + printable_output # cmd has already been printed, so don't clutter things further
		else: err = 'system call failed:\n\n%s\n\n%s' % (cmd, printable_output)
		
	if err == '':
		if verbose: print('# loading %s' % matfile)
		try:
			mat = ReadMatFile(matfile)
		except:
			err = "The chain must have failed: could not load %s\nShell output was as follows:\n%s" % (matfile, printable_output)
		else:
			if 'Data' not in mat:  err = "The chain must have failed: no 'Data' variable found in %s\nShell output was as follows:\n%s" % (matfile, printable_output)
			if 'Index' not in mat: err = "The chain must have failed: no 'Index' variable found in %s\nShell output was as follows:\n%s" % (matfile, printable_output)

	if err == '':
		out.FileName = datfile
		if stream2mat_saves_parms:
			if verbose: print('# decoding parameters loaded from the mat-file')
			parms = make_bciprm(verbosityFlag, mat['Parms'][0])
		else:
			if verbose: print('# reading output parameter file' + prmfile_out)
			parms = ParmList(prmfile_out) # if you get an error that prmfile_out does not exist, recompile your bci_dat2stream and bci_stream2mat binaries from up-to-date sources, and ensure that dat2stream_has_p_flag and stream2mat_saves_parms, at the top of this file, are both set to 1
		
		out.DateStr = read_bcidate(parms, 'ISO')
		out.DateNum = read_bcidate(parms)
		out.FilterChain = chain
		out.ToolVersions = binaries
		out.ShellInput = cmd
		out.ShellOutput = output
		out.ChainTime = chaintime
		out.ChainSpeedFactor = None
		out.Megabytes = None
		out.Parms = parms
		out.Parms.sort()
		
		mat['Index'] = mat['Index'][0,0]
		sigind = mat['Index'].Signal - 1 # indices vary across channels fastest, then elements
		nChannels,nElements = sigind.shape
		nBlocks = mat['Data'].shape[1]
		out.Blocks = nBlocks
		out.BlocksPerSecond = float(parms.SamplingRate.ScaledValue) / float(parms.SampleBlockSize.ScaledValue)
		out.SecondsPerBlock = float(parms.SampleBlockSize.ScaledValue)  / float(parms.SamplingRate.ScaledValue)
		out.ChainSpeedFactor = float(out.Blocks * out.SecondsPerBlock) / float(out.ChainTime)
		
		def unnumpify(x):
			while isinstance(x, numpy.ndarray) and x.size == 1: x = x[0]
			if isinstance(x, (numpy.ndarray,tuple,list)): x = [unnumpify(xi) for xi in x]
			return x
		
		out.Channels = nChannels
		out.ChannelLabels = unnumpify(mat.get('ChannelLabels', []))
		try: out.ChannelSet = SigTools.ChannelSet(out.ChannelLabels)
		except: out.ChannelSet = None
		
		out.Elements = nElements
		out.ElementLabels = unnumpify(mat.get('ElementLabels', []))
		out.ElementValues = numpy.ravel(mat.get('ElementValues', []))
		out.ElementUnit = unnumpify(mat.get('ElementUnit', None))
		out.ElementRate = out.BlocksPerSecond * out.Elements
		
		out.Time = out.SecondsPerBlock * numpy.arange(0, nBlocks)
		out.FullTime = out.Time
		out.FullElementValues = out.ElementValues


		# Why does sigind have to be transposed before vectorizing to achieve the same result as sigind(:) WITHOUT a transpose in Matlab?
		# This will probably forever be one of the many deep mysteries of numpy dimensionality handling
		out.Signal = mat['Data'][sigind.T.ravel(), :]  # nChannels*nElements - by - nBlocks
		out.Signal = out.Signal + 0.0 # make a contiguous copy
		
		def seconds(s):  # -1 means "no information", 0 means "not units of time",  >0 means the scaling factor
			if getattr(s, 'ElementUnit', None) in ('',None,()) or s.ElementUnit == []: return -1
			s = s.ElementUnit
			if   s.endswith('seconds'): s = s[:-6]
			elif s.endswith('second'):  s = s[:-5]
			elif s.endswith('sec'):     s = s[:-2]		
			if s.endswith('s'): return { 'ps': 1e-12, 'ns': 1e-9, 'us': 1e-6, 'mus': 1e-6, 'ms': 1e-3, 's' : 1e+0, 'ks': 1e+3, 'Ms': 1e+6, 'Gs': 1e+9, 'Ts': 1e+12,}.get(s, 0)
			else: return 0
	
		if dims == 0: # dimensionality has not been specified explicitly: so guess, based on ElementUnit and/or filter name
			# 3-dimensional output makes more sense than continuous 2-D whenever "elements" can't just be concatenated into an unbroken time-stream
			if len(chain): lastfilter = chain[-1].lower()
			else: lastfilter = ''
			if lastfilter == 'p3temporalfilter':
				dims = 3
			else:
				factor = seconds(out)
				if factor > 0:  # units of time.  TODO: could detect whether the out.ElementValues*factor are (close enough to) contiguous from block to block; then p3temporalfilter wouldn't have to be a special case above 
					dims = 2 
				elif factor == 0: # not units of time: use 3D by default
					dims = 3
				elif lastfilter in ['p3temporalfilter', 'arfilter', 'fftfilter', 'coherencefilter', 'coherencefftfilter']:  # no ElementUnit info? guess based on filter name
					dims = 3
				else:
					dims = 2
		
		if dims == 3:
			out.Signal = numpy.reshape(out.Signal, (nChannels, nElements, nBlocks), order='F') # nChannels - by - nElements - by - nBlocks
			out.Signal = numpy.transpose(out.Signal, (2,0,1))                                  # nBlocks - by - nChannels - by - nElements
			out.Signal = out.Signal + 0.0 # make a contiguous copy
		elif dims == 2:
			out.FullTime = numpy.repeat(out.Time, nElements)
			factor = seconds(out)
			if len(out.ElementValues):
				out.FullElementValues = numpy.tile(out.ElementValues, nBlocks)
				if factor > 0: out.FullTime = out.FullTime + out.FullElementValues * factor
			
			out.Signal = numpy.reshape(out.Signal, (nChannels, nElements*nBlocks), order='F')  # nChannels - by - nSamples
			out.Signal = numpy.transpose(out.Signal, (1,0))                                    # nSamples - by - nChannels
			out.Signal = out.Signal + 0.0 # make a contiguous copy
		else:
			raise RuntimeError('internal error')
		
		out.States = CONTAINER()
		try: fieldnames = mat['Index']._fieldnames
		except AttributeError: items = list(mat['Index'].items())
		else: items = [ ( k, getattr(mat['Index'], k) ) for k in fieldnames ]
		states = [(k,int(v)-1) for k, v in items if k != 'Signal']
		for k,v in states: setattr(out.States, k, mat['Data'][v,:])
		# TODO: how do the command-line tools handle event states? this seems to be set up to deliver one value per block whatever kind of state we're dealing with
		
		out.Megabytes = megs(out)
	else:
		out = err
		keep = True
		print(err)
		print()
	
		
	if os.path.isdir(tmpdir):
		files = sorted([os.path.join(tmpdir, file) for file in os.listdir(tmpdir) if file not in ['.','..']])
		if keep: print('The following commands should be executed to clean up the temporary files:')
		elif verbose: print('# removing temp files and directory ' + tmpdir)
		
		for file in files:
			if keep: print("os.remove(r'%s')" % file)
			else:
				try: os.remove(file)
				except Exception as err: sys.stderr.write( "failed to remove %s:\n    %s\n" % ( file, str( err ) ) )
			
		if keep: print("os.rmdir(r'%s')" % tmpdir)
		else:
			try: os.rmdir(tmpdir)
			except Exception as err: sys.stderr.write( "failed to remove %s:\n    %s" % ( tmpdir, str( err ) ) )
		if keep: print("")
		
	return out

def megs(x, rec=0):
	"""
	Very rough calculation of the number of megabytes occupied by an object and its children.
	Note that some of the children (and hence the memory) may be shared by other objects.
	Note also that mutual references by objects to each other will screw things up.
	"""###
	rec += 1
	if rec > 100: print('megs failed'); return 0.0 # too much recursion
	denom = 1024.0 * 1024.0
	def siz(x, rec):
		if hasattr(sys, 'getsizeof'): m = sys.getsizeof(x) / denom # only the container overhead
		else: # Python versions <2.6 do not have sys.getsizeof. Container overhead will be underestimated.
			try: m = numpy.dtype( x.__class__ ).itemsize
			except: m = 0
		if hasattr(x, '__dict__'): m += megs(x.__dict__, rec)  # works for sstruct and Param objects very nicely; should work for most old- and new-type objects
		return m
	if isinstance(x, dict):                   return siz(x, rec) + megs(list(x.items()), rec)
	if isinstance(x, (tuple,list)):           return siz(x, rec) + sum([megs(xi, rec) for xi in x])
	if isinstance(x, numpy.ndarray):          return siz(x, rec) + x.nbytes / denom
	return siz(x, rec) # other esoteric types may not be covered - numpy.ndarray, for example, has no __dict__ and requires special handling, and perhaps there are others like it out there
	
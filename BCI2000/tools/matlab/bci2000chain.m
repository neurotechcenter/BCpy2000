function [out, err] = bci2000chain(datfile, chain, varargin)
% BCI2000CHAIN    Run a BCI2000 data file through a chain of command-line filters
% 
% S = BCI2000CHAIN(DATFILE, CHAIN, ...)
% 
% DATFILE is the name of, or path to, a BCI2000 data file.
% 
% CHAIN   is a specification of the chain of command-line filters to run
%         through. It can be a cell array of strings, e.g.
%                       {'TransmissionFilter', 'SpatialFilter', 'ARFilter'}
%         or a pipe-delimited string, e.g.
%                               'TransmissionFilter|SpatialFilter|ARFilter'
%         The strings 'ARSignalProcessing' or 'P3SignalProcessing' may be
%         used as a shorthand for the complete pipe for those particular
%         signal processing modules. 
% 
% By default, the chain uses the parameters that were stored in the data
% file. However, additional optional arguments (...) are passed to
% MAKE_BCIPRM, thereby allowing you the option of changing parameter
% values.
% 
% Example:
%     fn = bci2000path('data', 'samplefiles', 'eeg3_2.dat');
%     s = bci2000chain(fn, 'TransmissionFilter|SpatialFilter|ARFilter', ...
%                          'ExampleParameters.prm', 'SpatialFilterType', 3)
% 
% This example reads and replays the sample data file through the specified
% filter chain, after substituting in the parameters specified in
% ExampleParameters.prm, and switching the spatial filter to CAR mode.
% 
% Additional string arguments beginning with '-' may be inserted between CHAIN
% and the remaining parameter arguments. These are flags, and they change the
% behaviour as follows:
% 
%    '-v' or '--verbose'           : be verbose
%    '-k' or '--keep'              : do not delete temporary files at the end
%    '-3' or '--three-dimensional' : return signal as a three-dimensional array
%    '-2' or '--two-dimensional'   : return signal as a two-dimensional array
% 
% A three-dimensional signal is SampleBlocks x Channels x Elements, whereas
% a two-dimensional signal is Elements x Channels (the Elements of each
% SampleBlock are concatenated along the first dimension). In the absence of a
% flag explicitly specifying either two- or three-dimensional output,
% BCI2000CHAIN makes its best guess as to what is appropriate given the output
% ElementUnit, and/or the name, of the last filter in the chain.
% 
% Finally, two optional time parameters can be passed through to bci_dat2stream,
% specifying the offset within the file at which processing should start, and
% the duration of the signal that should be processed.
% 
%    '-sTTT' or '--start=TTT'      : specify time offset TTT within file
%    '-dTTT' or '--duration=TTT'   : specify maximum duration TTT
% 
% These should be expressed like any other BCI2000 time parameter: as a whole
% number of SampleBlocks (e.g. --duration=10)  or as a number of seconds
% corresponding to a whole number of SampleBlocks, with the unit explicitly
% expressed (e.g. --duration=10s).
% 
% BCI2000CHAIN has the following dependencies:
% 
% M-files:   make_bciprm, read_bcidate, read_bciprm
% Mex-files: load_bcidat, convert_bciprm  (used by the m-files above)
% Binaries:  bci_dat2stream, bci_stream2mat
%            Additional filter binaries (e.g. TransmissionFilter, etc.)
%            The location of these binaries must be added to the PATH
%            environment variable. The optional utility bci2000path.m is
%            useful for this.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% $Id: bci2000chain.m 4671 2013-12-27 23:03:54Z jhill $ 
%% Author: Jeremy Hill <jezhill@gmail.com>
%%
%% $BEGIN_BCI2000_LICENSE$
%% 
%% This file is part of BCI2000, a platform for real-time bio-signal research.
%% [ Copyright (C) 2000-2012: BCI2000 team and many external contributors ]
%% 
%% BCI2000 is free software: you can redistribute it and/or modify it under the
%% terms of the GNU General Public License as published by the Free Software
%% Foundation, either version 3 of the License, or (at your option) any later
%% version.
%% 
%% BCI2000 is distributed in the hope that it will be useful, but
%%                         WITHOUT ANY WARRANTY
%% - without even the implied warranty of MERCHANTABILITY or FITNESS FOR
%% A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
%% 
%% You should have received a copy of the GNU General Public License along with
%% this program.  If not, see <http://www.gnu.org/licenses/>.
%% 
%% $END_BCI2000_LICENSE$
%% http://www.bci2000.org 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


% Setting the following two flags to 1 indicates that we are running new-style "single-chain"
% versions of bci_dat2stream and bci_stream2mat, i.e. ones that were compiled from source files
% whose version is >= r3203 (2011-04-13).  With older versions, we can still run, but the
% corresponding flags need to be set to 0 and we will also need to build binaries for bci_prm2stream
% and bci_stream2prm (furthermore, in the old-style bci_dat2stream, certain parameters cannot be
% altered from the Matlab command-line at all).

dat2stream_has_p_flag = 1;  % new-style bci_dat2stream has a -p flag allowing it to read a .prm file to override parameter values that came from the .dat file. Without this, some source parameters like SampleBlockSize and SourceChGain cannot be overridden.
stream2mat_saves_parms = 1; % new-style bci_stream2mat saves a string representation of the collated output parameters in the mat file, so we don't have to rely on a separate parameter file.


if nargin < 2, chain = []; end
if isnumeric(chain) & isempty(chain), chain = 'SpectralSignalProcessing'; end
if isequal(chain, 'SpectralSignalProcessing')
	chain = 'TransmissionFilter|SpatialFilter|SpectralEstimator|LinearClassifier|LPFilter|ExpressionFilter|Normalizer';
elseif isequal(chain, 'ARSignalProcessing')
	chain = 'TransmissionFilter|SpatialFilter|ARFilter|LinearClassifier|LPFilter|ExpressionFilter|Normalizer';
elseif isequal(chain, 'P3SignalProcessing')
	chain = 'TransmissionFilter|SpatialFilter|P3TemporalFilter|LinearClassifier';
end
if ischar(chain)
	cc = {}; while ~isempty(chain), [cc{end+1,1}, chain] = strtok(chain, '|'); end
	chain = cc;
end
rm = logical(zeros(size(chain)));
for i = 1:numel(chain)
	chain{i} = deblank(fliplr(deblank(fliplr(chain{i}))));
	rm(i) = isempty(chain{i});
end
chain(rm) = [];
if isempty(char(chain)), warning('chain is empty'), end

opts = {};
prm = {};
for i = 1:numel(varargin)
	if isempty(prm) & ischar(varargin{i})
		if strncmp(varargin{i}, '-', 1)
			opts{end+1} = varargin{i};
			continue
		end
	end
	prm{end+1} = varargin{i};
end
[twodee,   opts, ind2]  = getopt(opts, '-2', '--two-dimensional');
[threedee, opts, ind3]  = getopt(opts, '-3', '--three-dimensional');
[preserve_tmpdir, opts] = getopt(opts, '-k', '--keep');
[verbose, opts]         = getopt(opts, '-v', '--verbose');
[pretty,  opts]         = getopt(opts, '-p', '--pretty');

[use_offset,   opts, ind, offset  ] = getopt(opts, '-s', '--start');
[use_duration, opts, ind, duration] = getopt(opts, '-d', '--duration');

if use_offset
	if ~dat2stream_has_p_flag, error('older builds of bci_dat2stream have no -s or --start flag'), end % update your bci_dat2stream, and set the dat2stream_has_p_flag variable back to 1, above
	if isempty(offset), error('the -s or --start option must be used with an argument (e.g. --start=10s)'), end
	offset = [' --start=' offset];
end
if use_duration
	if ~dat2stream_has_p_flag, error('older builds of bci_dat2stream have no -d or --duration flag'), end % update your bci_dat2stream, and set the dat2stream_has_p_flag variable back to 1, above
	if isempty(duration), error('the -d or --duration option must be used with an argument (e.g. --duration=10s)'), end
	duration = [' --duration=' duration];
end

if ~isempty(opts)
	opts(2, :) = {' '};
	error(['unrecognized option(s): ' opts{1:end-1}])
end

if pretty, pretty = {'-p'}; else pretty = {}; end
if twodee & threedee, if ind3 >= ind2, twodee = 0; else threedee = 0; end, end
% if both --two-dimensional and --three-dimensional are given, obey whichever one is last-mentioned later
if twodee, dimensionality = 2; elseif threedee, dimensionality = 3; else dimensionality = 0; end
% dimensionality = 0 means neither --two-dimensional nor --threedimensional was given explicitly ('auto' mode)

%out = struct('use_offset', use_offset, 'offset', offset, 'use_duration', use_duration, 'duration', duration, 'pretty', pretty, 'dimensionality', dimensionality); return


cmd = {};
binaries = {};
tmpdir = tempname;
if verbose, fprintf('# creating directory %s\n', tmpdir); end
[success, msg] = mkdir(tmpdir);
if ~success, error(msg), end

tmpdatfile  = fullfile(tmpdir, 'in.dat');
prmfile_in  = fullfile(tmpdir, 'in.prm');
prmfile_out = fullfile(tmpdir, 'out.prm');
matfile     = fullfile(tmpdir, 'out.mat');
bcifile     = fullfile(tmpdir, 'out.bci');

if isunix
	DYLD_LIBRARY_PATH = getenv('DYLD_LIBRARY_PATH');
	DYLD_FRAMEWORK_PATH = getenv('DYLD_FRAMEWORK_PATH');
	setenv('DYLD_LIBRARY_PATH', '');  setenv('DYLD_FRAMEWORK_PATH', '')
	% this seems to be required if any of the command-line filters have been built with a QT dependency
	% causing them to be dependent on the Qt framework dylibs, which for some reason cannot be accessed
	% because of Matlab's setting of these two environment variables.  They are set back the way they
	% were below.
end

if isstruct(datfile), datfile = {datfile}; end
if iscell(datfile)
	createArgs = datfile;
	datfile = tmpdatfile;
	if verbose, fprintf('# creating temporary data file %s\n', datfile); end
	create_bcidat(datfile, createArgs{:});
end

if ~exist(datfile, 'file') | isdir(datfile), error(sprintf('file %s not found', datfile)), end
if ~isempty(which(datfile)), datfile = which(datfile); end
datfile = resolve(datfile);

mappings = {
	'$DATFILE'     datfile
	'$PRMFILE_IN'  prmfile_in
	'$PRMFILE_OUT' prmfile_out
	'$MATFILE'     matfile
	'$BCIFILE'     bcifile
};

if isempty(prm)
	cmd{end+1} = ['bci_dat2stream' offset duration ' < "$DATFILE"'];
else
	if ~iscell(prm), prm = {prm}; end
	if verbose, fprintf('# writing custom parameter file %s\n', prmfile_in); end
	make_bciprm(datfile, prm{:}, pretty{:}, '>', prmfile_in);
	
	if dat2stream_has_p_flag
		cmd{end+1} = ['bci_dat2stream -p$PRMFILE_IN' offset duration ' < "$DATFILE"']; % new-style bci_dat2stream with -p option
		binaries{end+1} = 'bci_dat2stream';
	else
		cmd{end+1} = '(';   % old-style bci_dat2stream with no -p option
		cmd{end+1} = '   bci_prm2stream < $PRMFILE_IN';
		cmd{end+1} = '&& bci_dat2stream --transmit-sd < $DATFILE';
		cmd{end+1} = ')';
		binaries{end+1} = 'bci_dat2stream';
		binaries{end+1} = 'bci_prm2stream';
	end
end
for i = 1:numel(chain)
	cmd{end+1} = sprintf('| %s', chain{i});
	binaries{end+1} = chain{i};
end
if stream2mat_saves_parms
	cmd{end+1} = '| bci_stream2mat > $MATFILE'; % new-style bci_stream2mat with Parms output
	binaries{end+1} = 'bci_stream2mat';
else
	cmd{end+1} = ' > $BCIFILE && bci_stream2mat < $BCIFILE > $MATFILE && bci_stream2prm < $BCIFILE > $PRMFILE_OUT';  % old-style bci_stream2mat without Parms output
	binaries{end+1} = 'bci_stream2mat';
	binaries{end+1} = 'bci_stream2prm';
end

cmd = cmd(:)'; cmd(2, :) = {' '}; cmd = [cmd{1:end-1}];

for i = 1:size(mappings, 1), cmd = strrep(cmd, mappings{i, :}); end
if verbose, fprintf('%s\n', cmd); end

t = clock;
[failed output] = system(cmd);
chaintime = etime(clock, t);
output = tidytext(output);
failsig = 'Configuration Error: ';
failmatch = findstr([failsig output], failsig);
if length(failmatch) > 1, failed = 1; end % TODO: really SYSTEM should have caught this. Is this Windoze-specific?
global BCI2000CHAIN_LASTOUTPUT
BCI2000CHAIN_LASTOUTPUT = output;
printable_output = output;
maxlines = 10;
newlines = find(printable_output==char(10));
if numel(newlines) > maxlines-1, printable_output = sprintf('%s[%d more lines omitted]', printable_output(1:newlines(maxlines-1)), numel(newlines)-maxlines+2); end
if verbose & (nargout>=2 | ~failed) & ~isempty(output), fprintf('%s\n', printable_output); end
if failed
	if verbose
		err = sprintf('system call failed:\n%s', printable_output); % cmd has already been printed, so don't clutter things further
	else
		err = sprintf('system call failed:\n%s\n%s', cmd, printable_output);
	end
else
	err = '';
end

if verbose, fprintf('# querying version information\n'); end
binver = {};
for i = 1:numel(binaries), [failed binver{i}] = system([binaries{i} ' --version']); binver{i} = strrep(tidytext(binver{i}), char(10), ' '); end
binaries = [binaries(:)'; binver(:)']; binaries = struct(binaries{:});
binaries.load_bcidat = strrep(tidytext(evalc('load_bcidat --version')), char(10), ' ');
binaries.convert_bciprm = strrep(tidytext(evalc('convert_bciprm --version')), char(10), ' ');

if isunix
	setenv('DYLD_LIBRARY_PATH', DYLD_LIBRARY_PATH) % see above
	setenv('DYLD_FRAMEWORK_PATH', DYLD_FRAMEWORK_PATH)
end

if isempty(err)
	if verbose, fprintf('# loading %s\n', matfile); end
	mat = load(matfile);
	if ~isfield(mat, 'Data'), err = sprintf('chain must have failed: no ''Data'' variable found in %s\nShell output was as follows:\n%s', matfile, printable_output); end
	if ~isfield(mat, 'Index'), err = sprintf('chain must have failed: no ''Index'' variable found in %s\nShell output was as follows:\n%s', matfile, printable_output); end
end

if isempty(err)
	out.FileName = datfile;
	if stream2mat_saves_parms
		if verbose, fprintf('# decoding parameters loaded from the mat-file\n'); end
		parms = read_bciprm(mat.Parms);
	else
		if verbose, fprintf('# reading output parameter file %s\n', prmfile_out); end
		parms = read_bciprm(prmfile_out); % if you get an error that prmfile_out does not exist, recompile your bci_dat2stream and bci_stream2mat binaries from up-to-date sources, and ensure that dat2stream_has_p_flag and stream2mat_saves_parms, at the top of this file, are both set to 1
	end
	[out.DateStr, dateErr] = read_bcidate(parms, 'ISO');
	[out.DateNum, dateErr] = read_bcidate(parms);
	out.FilterChain = chain(:)';	
	out.ToolVersions = binaries;
	out.ShellInput = cmd;
	out.ShellOutput = output;
	out.ChainTime = chaintime;
	out.ChainSpeedFactor = nan;
	out.Megabytes = nan;
	out.Parms = parms;
	sigind = mat.Index.Signal;  % indices vary across channels fastest, then elements
	[nChannels nElements] = size(sigind);
	nBlocks = size(mat.Data, 2);
	out.Blocks = nBlocks;
	out.BlocksPerSecond = parms.SamplingRate.NumericValue  / parms.SampleBlockSize.NumericValue;
	out.SecondsPerBlock = parms.SampleBlockSize.NumericValue  / parms.SamplingRate.NumericValue;
	out.ChainSpeedFactor = out.Blocks * out.SecondsPerBlock / out.ChainTime;
	out.Channels = nChannels;
	if isfield(mat, 'ChannelLabels'), out.ChannelLabels = mat.ChannelLabels(:)'; end
	out.Elements = nElements;
	if isfield(mat, 'ElementLabels'), out.ElementLabels = mat.ElementLabels(:)'; end
	if isfield(mat, 'ElementValues'), out.ElementValues = mat.ElementValues(:)'; end
	if isfield(mat, 'ElementUnit'), out.ElementUnit = mat.ElementUnit; end
	out.ElementRate = out.BlocksPerSecond * out.Elements;
	
	out.Time = out.SecondsPerBlock * single(0:nBlocks-1)';
	out.FullTime = out.Time;
	if isfield(out, 'ElementValues'), out.FullElementValues = out.ElementValues; end
	out.Signal = mat.Data(sigind(:), :);  % nChannels*nElements - by - nBlocks

	if dimensionality == 0 % dimensionality has not been specified explicitly: so guess, based on ElementUnit and/or filter name
		% 3-dimensional output makes more sense than continuous 2-D whenever "elements" can't just be concatenated into an unbroken time-stream
		if isempty(chain), lastfilter = ''; else lastfilter = lower(chain{end}); end
		if strcmp(lastfilter, 'p3temporalfilter')
			dimensionality = 3;
		else
			factor = seconds(out);
			if factor > 0  % units of time.  TODO: could detect whether the out.ElementValues*factor are (close enough to) contiguous from block to block; then p3temporalfilter wouldn't have to be a special case above 
				dimensionality = 2; 
			elseif factor == 0 % not units of time: use 3D by default
				dimensionality = 3;
			elseif ismember(lastfilter, { 'p3temporalfilter', 'arfilter', 'fftfilter', 'coherencefilter', 'coherencefftfilter' })  % no ElementUnit info? guess based on filter name
				dimensionality = 3;
			else
				dimensionality = 2;
			end
		end
	end
	if dimensionality == 3
		out.Signal = reshape(out.Signal, [nChannels nElements nBlocks]); % nChannels - by - nElements - by - nBlocks
		out.Signal = permute(out.Signal, [3 1 2]);                       % nBlocks - by - nChannels - by - nElements
	elseif dimensionality == 2
		t = repmat(out.Time', nElements, 1);
		out.FullTime = t(:);
		factor = seconds(out);
		if isfield(out, 'ElementValues')
			out.FullElementValues = repmat(out.ElementValues(:), nBlocks, 1);
			if factor > 0, out.FullTime = out.FullTime + out.FullElementValues * factor; end
		end
		out.Signal = reshape(out.Signal, [nChannels nElements*nBlocks]); % nChannels - by - nSamples
		out.Signal = permute(out.Signal, [2 1]);                         % nSamples - by - nChannels
	else
		error('internal error in bci2000chain.m')
	end
	out.States = rmfield(mat.Index, 'Signal');  % TODO: how do the command-line tools handle event states? this seems to be set up to deliver one value per block whatever kind of state we're dealing with
	statenames = fieldnames(out.States);
	for i = 1:numel(statenames)
		out.States.(statenames{i}) = mat.Data(out.States.(statenames{i}), :)';
	end
	out.Megabytes = getfield(whos('out'), 'bytes') / 1024^2;
else
	preserve_tmpdir = 1;
	out = [];
end
	
if exist(tmpdir, 'dir')
	a = dir(tmpdir);
	a = sort({a(~[a.isdir]).name});
	if preserve_tmpdir
		fprintf('The following commands should be executed to clean up the temporary files:\n');
	elseif verbose
		fprintf('# removing temp files and directory %s\n', tmpdir);
	end
	for i = 1:numel(a)
		filepath = fullfile(tmpdir, a{i});
		if preserve_tmpdir
			fprintf('    delete(''%s'')\n', filepath);
		else
			delete(filepath)
		end
	end
	if preserve_tmpdir
		fprintf('    rmdir(''%s'')\n', tmpdir);
	else
		rmdir(tmpdir);
	end
end

if nargout < 2, error(err), end


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function [ispresent,opts,lastind,arg] = getopt(opts, shortform, longform)
arg = '';
arg_expected = (nargout >= 4);
matched = logical(zeros(size(opts)));
for i = 1:numel(opts)
	opt = opts{i};
	if arg_expected
		prefixes = {[longform '='],  longform,  [shortform '='], shortform};
		for j = 1:numel(prefixes)
			if strncmp(opt, prefixes{j}, length(prefixes{j}))
				arg = opt(length(prefixes{j})+1:end);
				matched(i) = 1;
				break;
			end
		end
	else
		matched(i) = strcmp(opt, shortform) | strcmp(opt, longform);
	end
end
ispresent = any(matched);
opts(matched) = [];
lastind = max(find(matched));
if isempty(lastind), lastind = 0; end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function p = resolve(p)

if isdir(p)
	olddir = pwd; cd(p); p = pwd; cd(olddir)
else
	[pdir stem xtn] = fileparts(p);
	if ~isempty(pdir)
		if isdir(pdir)
			pdir = resolve(pdir);
		end
		p = fullfile(pdir, [stem xtn]);
	end
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function x = tidytext(x)
x = deblank(x);
x = fliplr(deblank(fliplr(x)));
x = strrep(x, char([13 10]), char(10));
x = strrep(x, char([13]), char(10));


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function factor = seconds(s)  % -1 means "no information", 0 means "not units of time",  >0 means the scaling factor
factor = -1;
if ~isfield(s, 'ElementUnit'), return; end
factor = 0;
s = s.ElementUnit;
if     strncmp(lower(fliplr(s)), fliplr('seconds'), 7), s(end-5:end) = [];
elseif strncmp(lower(fliplr(s)), fliplr('second' ), 6), s(end-4:end) = [];
elseif strncmp(lower(fliplr(s)), fliplr('sec'    ), 3), s(end-1:end) = [];
end
if strncmp(lower(fliplr(s)), 's', 1)
	factors.ps = 1e-12;
	factors.ns = 1e-9;
	factors.us = 1e-6;  factors.mus = 1e-6;
	factors.ms = 1e-3;
	factors.s  = 1e+0;
	factors.ks = 1e+3;
	factors.Ms = 1e+6;
	factors.Gs = 1e+9;
	factors.Ts = 1e+12;
	if isfield(factors, s), factor = getfield(factors, s); end
end

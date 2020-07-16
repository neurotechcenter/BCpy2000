# -*- coding: utf-8 -*-
# 
#   $Id: klr.py 3445 2011-08-08 15:46:38Z jhill $
#   
#   This file is part of the BCPy2000 framework, a Python framework for
#   implementing modules that run on top of the BCI2000 <http://bci2000.org/>
#   platform, for the purpose of realtime biosignal processing.
# 
#   Copyright (C) 2007-11  Jeremy Hill, Thomas Schreiner,
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
import numpy

def klr_cg(K, Y, C=0, **kwargs):
	"""
	Regularised Kernel Logistic Regression Classifier
	
	[alphab,f,J,obj]=klr_cg(K,Y,C,varargin)
	Regularised Kernel Logistic Regression Classifier using a pre-conditioned
	conjugate gradient solver on the primal objective function
	
	J = C w' K w + sum_i log( (1 + exp( - y_i ( w'*K_i + b ) ) )^-1 ) 
	
	Inputs:
	    K       - [NxN] kernel matrix
	    Y       - [Nx1] matrix of -1/0/+1 labels, (0 label pts are implicitly ignored)
	    C       - the regularisation parameter, roughly max allowed length of the weight vector
	              good default is: .1*var(data) = .1*(mean(diag(K))-mean(K(:))))
	    
	Outputs:
	    alphab  - [(N+1)x1] matrix of the kernel weights and the bias [alpha;b]
	    f       - [Nx1] vector of decision values
	    J       - the final objective value
	    obj     - [J Ed Ew]
	    
	Options:
	    alphab  - [(N+1)x1] initial guess at the kernel parameters, [alpha;b] ([])
	    ridge   - [float] ridge to add to the kernel to improve convergence.  
	               ridge<0 -- absolute ridge value
	               ridge>0 -- size relative to the mean kernel eigenvalue
	    maxEval - [int] max number for function evaluations                    (N*5)
	    maxIter - [int] max number of CG steps to do                           (inf)
	    maxLineSrch - [int] max number of line search iterations to perform    (50)
	    objTol0 - [float] relative objective gradient tolerance                (1e-8)
	    objTol  - [float] absolute objective gradient tolerance                (0)
	    tol0    - [float] relative error tolerance, w.r.t. initial gradient    (0)
	    tol     - [float] absolute error tolerance                             (0)
	    verb    - [int] verbosity                                              (0)
	    step    - initial step size guess                                      (1)
	    wght    - point weights [Nx1] vector of label accuracy probabilities   ([])
	              [2x1] for per class weightings
	    nobias  - [bool] flag we don't want the bias computed                  (false)
	"""###
	
	# Copyright 2006-     by Jason D.R. Farquhar (jdrf@zepler.org)
	# Permission is granted for anyone to copy, use, or modify this
	# software and accompanying documents for any uncommercial
	# purposes, provided this copyright notice is retained, and note is
	# made of any changes that have been made. This software and
	# documents are distributed without any warranty, express or
	# implied
	# 
	# Adapted from: Matlab 6.x and up
	#           to: Python 2.5.4 + Numpy 1.3.0
	#           by: Jeremy Hill
	#           on: 2010-01-18
	#     based on: klr_cg.m  r12513 in MPI svn
	
	class optstruct:
		def __init__(self, d): self.__dict__.update(d)
	opts = optstruct({
		'alphab': None,
		'nobias': False,
		'maxIter': numpy.inf,
		'maxEval': None,
		'tol': 0.0,
		'tol0': 0.0,
		'objTol': 0.0,
		'objTol0': 1e-8,
		'verb': -1,
		'step': 0.0,
		'wght': None,  # sequence of N point-weightings or 2 class-weightings
		'ridge': 1e-9,
		'maxLineSrch':50,
		'maxStep': 3.0,
		'minStep': 0.05,
		'marate': 0.95,
		'bPC': None,
		'incThresh': 0.75,
	})
	unrec = set(kwargs.keys()) - set(opts.__dict__.keys())
	if len(unrec):
		raise TypeError('unrecognized option(s): %s' % ', '.join(['"' + a + '"' for a in unrec]))
	opts.__dict__.update(kwargs)

	if opts.verb >= 0:
		import sys
		out = sys.stdout

	
	K = numpy.asmatrix(K)   # no copy
	dtype = K.dtype
	if dtype==numpy.float32: eps=1e-7
	elif dtype==numpy.float64: eps=1e-16
	else: raise ValueError("unrecognized numeric type")
	N = K.shape[1]
	Y = numpy.matrix(Y, dtype=dtype)  # makes copy
	Y.shape = (N,1)
	C = float(C)
	
	if opts.alphab == None:
		wb = numpy.asmatrix(numpy.zeros((N+1,1), dtype=dtype))
	else:
		wb = numpy.matrix(opts.alphab, dtype=dtype) # makes a copy
		wb.shape = (N+1,1)
		if opts.nobias: wb[-1,0] = 0.0
	w = wb[:-1]               # view into wb: w only  ( matrix shape = (N,1) )
	b = wb[ -1]               # view into wb: b only  ( matrix shape = (1,)  )
	
	if opts.maxEval == None: opts.maxEval = (Y.flat != 0).sum() * 5

	meanabsdiagK = numpy.mean(numpy.abs(numpy.diag(K)))
	opts.ridge = numpy.matrix(opts.ridge, dtype=dtype)  # makes a copy
	opts.ridge.shape = (opts.ridge.size,1)
	if opts.ridge.size == 1:
		if opts.ridge > 0.0: opts.ridge *= meanabsdiagK   # make the ridge relative to the max eigen-value
		else:                opts.ridge *= -1.0           # negative value means absolute ridge
		

	
	# N.B. this form of loss weighting has no true probabilistic interpretation!
	wghtY = Y.copy()
	if opts.wght != None: # point weighting -- only needed in wghtY
		opts.wght = numpy.matrix(opts.wght, dtype=dtype) # makes a copy
		opts.wght.shape = (opts.wght.size,1)
		if opts.wght.size == 2: # per-class weights
			wghtY[Y<0] = -1.0 * opts.wght.flat[0]
			wghtY[Y>0] = +1.0 * opts.wght.flat[1]
		elif opts.wght.size == N:
			wghtY = numpy.multiply(Y, opts.wght)
		else:
			raise ValueError('wght must be 2 or N elements long')
	
	def vcat(a,b): return numpy.r_[numpy.asmatrix(a), numpy.asmatrix(b)]
	def scalarproduct(a,b): return numpy.inner(a.flat, b.flat)
	
	# check if it's more efficient to sub-set the kernel, because of lots of ignored points
	incIdx = (wghtY != 0.0)
	if incIdx.mean() < opts.incThresh: # if enough ignored to be worth it
		oK = K
		oY = Y
		K = K[incIdx, incIdx]
		Y = Y[incIdx, :]
		wghtY = wghtY[incIdx, :]
		wb = wb[vcat(incIdx, True), :];
		w = wb[:-1]               # view into wb: w only  ( matrix shape = (N,1) )
		b = wb[ -1]               # view into wb: b only  ( matrix shape = (1,)  )
	else:
		incIdx.flat = True
	
	# set the bias (i.e. b) pre-conditioner
	bPC = opts.bPC;
	if bPC == None: # bias pre-condn with the diagonal of the hessian
		bPC  = N / (meanabsdiagK + 2.0 * C)
	
	wK   = K * w + numpy.multiply(opts.ridge, w)
	dv   = wK + b
	g    = 1.0 / (1.0 + numpy.exp(-numpy.multiply(Y, dv)))  # =Pr(x|y)
	g[g<=0.0] = eps                                        # to stop log 0
	Yerr = numpy.multiply(wghtY, 1 - g)

	# precond'd gradient:
	#  [K  0  ]^-1 [(lambda*wK-K((1-g).Y))] = [lambda w - (1-g).Y]
	#  [0  bPC]    [ -1'*((1-g).Y)        ]   [ -1'*(1-g).Y./bPC  ] 
	
	MdJ_w = 2.0 * C * w - Yerr
	if opts.nobias: MdJ_b = 0
	else: MdJ_b = -Yerr.sum()/bPC
	MdJ   = vcat(MdJ_w,  MdJ_b)
	dJ    = vcat(K * MdJ_w + numpy.multiply(opts.ridge, MdJ_w),  MdJ_b * bPC)

	Mr   = -MdJ
	d    =  Mr
	dtdJ = -scalarproduct(d, dJ)
	r2   =  dtdJ
	r02  =  r2
	
	Ed   = -scalarproduct(numpy.log(g), numpy.multiply(Y, wghtY))  # -ln P(D|w,b,fp)
	Ew   = scalarproduct(wK, w)               # -ln P(w,b|R);
	J    = Ed + C * Ew            # J=neg log posterior
	
	# Set the initial line-search step size
	step = opts.step
	if step <= 0:
		step = min(1,  abs(J/max(dtdJ,eps)) ** 0.5)  # init step assuming opt is at 0
	step = abs(step)
	tstep = step
	
	neval = 1
	if opts.verb > 0:   # debug code      
		if opts.verb > 1: lend='\n'
		else: out.write('\n'); lend='\r'
		out.write('%3d) %3d x=[%8f,%8f,.] J=%5f (%8f+%8f) |dJ|=%8g\n' % (0,neval,wb.flat[0],wb.flat[1],J,Ew,Ed,r2))
	
	# pre-cond non-lin CG iteration
	J0 = J; madJ = abs(J); # init-grad est is init val

	iter = 0
	while iter < min(opts.maxIter,2e6):
		iter += 1
		
		oJ,oMr,or2 = J,Mr,r2 # record info about prev result we need
		if iter == 1 and (r2 <= opts.tol or r2 <= eps): break 
	
		#---------------------------------------------------------------------
		# Secant method for the root search.
		if opts.verb > 1:
			out.write('.%d %g=%g @ %g\n' % (0,0,dtdJ,J))
			# if opts.verb > 2: hold off;plot(0,dtdJ,'r*');hold on;text(0,double(dtdJ),num2str(0)); grid on;
		ostep=numpy.inf;step=tstep; # max(tstep,abs(1e-6/dtdJ)); # prev step size is first guess!
		odtdJ=dtdJ; # one step before is same as current
		wb.flat = (wb + step * d).flat
		a = K * d[:-1]
		Kd = vcat(K * d[:-1] + numpy.multiply(opts.ridge, d[:-1]).sum(),   bPC*d[-1])   # cache, so don't comp dJ

		j = 0
		while j < opts.maxLineSrch:
			j += 1

			neval=neval+1;
			oodtdJ=odtdJ; odtdJ=dtdJ; # prev and 1 before grad values
			
			# Eval the gradient at this point.  N.B. only gradient needed for secant
			wK   = K * w + numpy.multiply(opts.ridge, w) # include ridge
			dv   = wK + b
			g    = 1.0 / (1.0 + numpy.exp(-numpy.multiply(Y, dv)))  # =Pr(x|y)
			g[g<=0.0] = eps                                        # to stop log 0
			Yerr = numpy.multiply(wghtY, 1 - g)

			MdJ_w = 2.0 * C * w - Yerr
			if opts.nobias: MdJ_b = 0
			else: MdJ_b = -Yerr.sum()/bPC
			MdJ   = vcat(MdJ_w,  MdJ_b)
			dtdJ  = -scalarproduct(Kd, MdJ)  # gradient along the line @ new position
	
			# convergence test, and numerical res test
			if ( abs(dtdJ)<opts.tol or abs(dtdJ*step)<eps or abs(odtdJ-dtdJ)<eps ) and (iter>1 or j>=3): break  # Ensure we do decent line search for 1st step size!
			
			if opts.verb > 1:
				Ed   = -scalarproduct(numpy.log(g), numpy.multiply(Y, wghtY))         # P(D|w,b,fp)
				Ew   = scalarproduct(wK, w)             # P(w,b|R);
				J    = Ed + C * Ew;               # J=neg log posterior         
				out.write('#%d %g=%g @ %g\n' % (j,tstep,dtdJ,J))
				# if opts.verb > 2: plot(tstep,dtdJ,'*'); text(double(tstep),double(dtdJ),num2str(j));

			# now compute the new step size
			# backeting check, so it always decreases
			if oodtdJ*odtdJ < 0 < odtdJ*dtdJ and abs(step*odtdJ/(odtdJ-dtdJ)) > abs(ostep):      # oodtdJ still brackets, and would jump outside
				step = ostep + step; # make as if we jumped here directly.
				odtdJ = oodtdJ;
			ostep = step;
			# *RELATIVE* secant step size
			nstep = dtdJ/(odtdJ-dtdJ);
			nstep = numpy.sign(nstep) * max(opts.minStep, min(abs(nstep), opts.maxStep))
			step  = step * nstep ;
			tstep = tstep + step;            # total step size
			
			# move to the new point
			wb.flat = (wb + step*d).flat ;

		if opts.verb > 1: out.write('\n')

		# compute the other bits needed for CG iteration
		dJ   = vcat(K * MdJ_w + numpy.multiply(opts.ridge , MdJ_w),  bPC*MdJ_b)
		if opts.nobias: dJ.flat[-1] = 0
		Mr = -MdJ;
		r2 = abs(scalarproduct(Mr, dJ))
	
		# compute the function evaluation
		Ed   = -scalarproduct(numpy.log(g), numpy.multiply(Y, wghtY))         # P(D|w,b,fp)
		Ew   = scalarproduct(wK, w)             # P(w,b|R);
		J    = Ed + C * Ew;               # J=neg log posterior         

		if opts.verb>0:   # debug code      
			out.write(('%3d) %3d x=[%8f,%8f,.] J=%5f (%8f+%8f) |dJ|=%8g' + lend) % (iter,neval,wb.flat[0],wb.flat[1],J,Ew,Ed,r2))
	
		#------------------------------------------------
		# convergence test
		#if ( iter==1 )     madJ=J;      J0=J;     dJ0=1;
		if iter==1:     madJ=(oJ-J); dJ0=max(abs(madJ),eps); 
		else:           madJ=madJ*(1 - opts.marate) + (oJ-J)*(opts.marate); # move-ave objective grad est

		if r2 <= max(opts.tol, eps, r02*opts.tol0) \
		   or neval > opts.maxEval \
		   or madJ < opts.objTol \
		   or madJ / dJ0 < opts.objTol0: break    # term tests
	
		#------------------------------------------------
		# conjugate direction selection
		delta = max(0, scalarproduct(Mr-oMr, -dJ/or2)); # Polak-Ribier
		#delta = max(0, r2/or2)        # Fletcher-Reeves
		d      = Mr + delta * d        # conj grad direction
		dtdJ   =-scalarproduct(d,dJ);  # new search dir grad.
		if dtdJ <= 0:                  # non-descent dir switch to steepest
			if opts.verb >= 2: out.write('non-descent dir\n')
			d = Mr
			dtdJ = -scalarproduct(d, dJ) 
		
	if opts.verb >= 0:
		out.write('%3d) %3d x=[%8f,%8f,.] J=%5f (%5f+%5f) |dJ|=%8g\n' % (iter,neval,wb.flat[0],wb.flat[1],J,Ew,Ed,r2));
		if J0 < J: out.write('\nWARNING: Non-reduction\n')
	
	# compute final decision values.
	if not incIdx.all(): # map back to the full kernel space, if needed
		K=oK; Y=oY;
		nwb = Y*0; nwb[incIdx]=w; nwb.flat[-1]=b; wb.flat=nwb.flat
	
	f = K * w + b
	p = 1.0 / (1.0 + numpy.exp(-f)) # Pr(y==1|x,w,b)

	obj = [J, Ew, Ed];

	return [wb,f,J,obj]

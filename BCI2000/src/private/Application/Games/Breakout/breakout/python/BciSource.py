

import re
import md5
import numpy
import sys
class BciSource (BciGenericSource):

	def Construct(self):
		self.define_param('Controller list  ControllerExpressions= 1 ((MousePosX-32767)/320)-1 % % % // ')
		self.define_param('Controller int   DiffController= 1 1 0 1 // (boolean)')
		self.define_param('Controller int   SplitController= 1 1 0 1 // (boolean)')
		self.define_param('Controller float ControllerGain= 3 3 0 1 // ')
		self.define_param('Controller float ControllerNoise= 0 0 0 % // ')
		self.define_param('Controller float MaxRobotOutput= 0.25 0.25 0 1 // ')
		self.define_param('Controller int   RobotIntelligence= 1 1 0 % // ')
	
	def Preflight(self, inprops):
		self.missed = False
		self.RobotOffset = None
		self.MaxRobotOutput = float(self.params['MaxRobotOutput'])
		self.ControllerGain = float(self.params['ControllerGain'])
		self.ControllerNoise = float(self.params['ControllerNoise'])
		self.RobotIntelligence = float(self.params['RobotIntelligence'])

		self.expressions = [self.PythonizeExpression(xi) for xi in self.params.ControllerExpressions]
		self.split = bool(self.params.SplitController)
		
		if self.split and len(self.expressions) != 1:
			raise EndUserError('ControllerExpressions should only have one element if SplitController is checked')
		if not self.split and len(self.expressions) != 2:
			raise EndUserError('ControllerExpressions should have two elements if SplitController is unchecked')
		
		self.prev = {}
		if int(self.params.DiffController):
			self.expressions = ['self.Diff(%s,"%s")'%(x,md5.md5(x).hexdigest()) for x in self.expressions]
		
		for i,x in enumerate(self.expressions):
			try:
				eval(x)
			except:
				etype,eobj,etrace = sys.exc_info()
				raise EndUserError('%s in ControllerExpressions #%d: %s' % (etype.__name__, i+1, eobj))
		
		if self.split:
			self.splitexpr, = self.expressions
		else:
			self.leftexpr,self.rightexp = self.expressions
		
			
	def Process(self, sig):
		if self.states.JustMissed: self.missed = True
		if self.states.JustHit: self.missed = False
		if self.RobotOffset == None or self.states.JustHit or self.states.JustMissed:
			self.RobotOffset = 2*(numpy.random.rand()-0.5) * 1e-4 * self.states.PaddleWidthTimes10000 / 3.0
		
		if self.split:
			x = eval(self.splitexpr)
			left = min(x,0)
			right = max(0,x)
		else:
			left = eval(self.leftexpr)
			right = eval(self.rightexpr)
			
		sig = numpy.random.randn(*sig.shape)
		sig[0,:] *= left
		sig[1,:] *= right
		
		sig *= self.ControllerGain
		sig += numpy.random.randn(*sig.shape) * self.ControllerNoise
		
		return sig
	
	
	def Robot(self):	

		r = 1e-4 * self.states.RightMotionRequiredTimes10000
		l = 1e-4 * self.states.LeftMotionRequiredTimes10000
		optimal = r - l
		if self.RobotOffset != None: optimal += self.RobotOffset
		follow = 1e-4 * (self.states.BallXPosTimes10000 - self.states.PaddleXPosTimes10000)

		theta = self.states.BallAngle
		
		if self.missed:
			dx = optimal
		elif self.RobotIntelligence == 0:
			dx = follow
		else:
			use_optimal = self.states.FutureBounces <= self.RobotIntelligence
			if self.RobotIntelligence == 1: use_optimal &= self.states.BallAngle>180
			if use_optimal: dx = optimal
			else: dx = follow / 5
				
		dx = min(self.MaxRobotOutput, dx)
		return dx
		
	
	
	def Signal(self, ch, el):
		return self.in_signal[ch-1,el-1]
			
	def Diff(self, val, key):
		prev = self.prev.get(key, val)
		self.prev[key] = val
		return (val - prev) * self.nominal.PacketsPerSecond
		
	def PythonizeExpressionSubstring(self, x):
		if x in self.states: return 'float(self.states.%s)' % x
		if hasattr(self, x): return 'self.' + x
		if hasattr(numpy, x): return 'numpy.' + x
		raise EndUserError("failed to interpret '%s' in expression" % x)
		
	def PythonizeExpression(self, x):
		x = x.replace('^', '**')
		x = re.sub('[A-Za-z_][0-9A-Za-z_]*', lambda m:self.PythonizeExpressionSubstring(m.group()), x)
		return x
	

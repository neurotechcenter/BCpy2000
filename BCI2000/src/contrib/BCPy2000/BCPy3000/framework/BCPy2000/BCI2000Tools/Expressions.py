__all__ = [
	'Expression'
]

import re, numpy

class Expression(object):
	def __init__(self, string, states=None, channels=None, elements=None, bci=None):
		self.__pystring = '0'
		self.__string = '0'
		self.__channels = []
		self.__elements = []
		self.__signal = numpy.zeros((0,0))
		self.states = {}
		if bci != None:
			if states == None: states = bci.states
			if channels == None: channels = bci.in_signal_props['ChannelLabels']
			if elements == None: elements = bci.in_signal_props['ElementLabels']
		if string != None: self.string = string
		if states != None: self.states = states
		if channels != None: self.channels = channels
		if elements != None: self.elements = elements
	
	def Signal(self, ch, el):
		if isinstance(ch, str):
			if ch.lower() not in self.__channels: raise ValueError('error in expression "%s": unrecognized channel label "%s"' % (self.string, ch))
			ch = self.__channels.index(ch.lower()) + 1
		elif not 1 <= ch <= self.__signal.shape[0]:
			raise ValueError('error in expression "%s": only channel indices from 1 to %d are valid' % (self.string,self.__signal.shape[0]))
		if isinstance(el, str):
			if el.lower() not in self.__elements: raise ValueError('error in expression "%s": unrecognized element label "%s"' % (self.string, el))
			el = self.__elements.index(el.lower()) + 1
		elif not 1 <= el <= self.__signal.shape[1]:
			raise ValueError('error in expression "%s": only element indices from 1 to %d are valid' % (self.string,self.__signal.shape[1]))
			
		return float(self.__signal[ch-1, el-1])
	
	def Evaluate(self, input=None):
		if input == None:
			input = numpy.zeros((len(self.channels),len(self.elements)))
		self.__signal = numpy.asarray(input)
		while len(self.__signal.shape) < 2: self.__signal.shape = tuple(self.__signal.shape) + (1,)
		return eval(self.__pystring)
		
	def _PythonizeExpression(self, x):
		x = x.replace('^', '**')
		x = re.sub('[A-Za-z_][0-9A-Za-z_]*', lambda m:self._PythonizeExpressionSubstring(m.group()), x)
		return x
		
	def _PythonizeExpressionSubstring(self, x):
		if x in self.states: return 'float(self.states["%s"])' % x
		if hasattr(self, x): return 'self.' + x
		if hasattr(numpy, x): return 'numpy.' + x
		raise ValueError("failed to interpret '%s' in expression" % x)
		
	@apply
	def string():
		doc = "source string for the Expression"
		def fget(self): return self.__string
		def fset(self, val): self.__pystring = self._PythonizeExpression(val) ; self.__string = val
		return property(fget, fset, doc=doc)	
	@apply
	def channels():
		doc = "sequence of channel labels"
		def fget(self): return tuple(self.__channels)
		def fset(self, val): self.__channels = [str(x).lower() for x in val]
		return property(fget, fset, doc=doc)
	@apply
	def elements():
		doc = "sequence of element labels"
		def fget(self): return tuple(self.__elements)
		def fset(self, val): self.__elements = [str(x).lower() for x in val]
		return property(fget, fset, doc=doc)

try: from BCI2000PythonCore import BciCore,EndUserError
except ImportError: from BCPy2000.Generic import BciCore,EndUserError
def make_expression(self, string):
	try:
		expr = Expression(string, bci=self)
		expr.Evaluate()
	except e:
		raise EndUserError(e) 
	return expr
BciCore.Expression = make_expression

class BciSignalProcessing (BciGenericSignalProcessing):
	
	def StartRun(self):
		self.remember('printed')
		
	def Process(self, sig):
		if self.since('printed')['msec'] > 1000:
			for x,y in sorted(self.states.items()):
				print x.rjust(20)+':',y
			print
			self.remember('printed')
					
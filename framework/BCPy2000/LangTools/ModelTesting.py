# -*- coding: utf-8 -*-
# 
#   $Id: ModelTesting.py 4262 2012-08-17 00:28:35Z jhill $
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
__all__ = [
	'test',
]
import os, sys

import numpy
from . import TextPrediction

try: import SigTools
except ImportError: import BCPy2000.SigTools as SigTools
from SigTools.NumTools import sstruct


def test(codebook, model=None, alphabet=None, txt=None, balanced_acc=0.75, default_context='. ',
         threshold=1.0, min_epochs=1, max_epochs=72,
         minprob=None, exponent=1.0, w=80):

	modelin = model
	if model == None: model = TextPrediction.LanguageModel(alphabet=alphabet)
	if alphabet == None: alphabet = TextPrediction.default_alphabet36
	if txt==None: txt = TextPrediction.default_test_text

	alphabet = model.match_case(alphabet)
	if len(set(alphabet) - set(model.alphabet)) > 0:
		raise ValueError("some characters in the alphabet are not in the model")

	if len(txt) < 1000 and os.path.isfile(TextPrediction.FindFile(txt)):
		txt = TextPrediction.ReadUTF8(TextPrediction.FindFile(txt))
	# Things will fail if some characters in the test text are not in alphabet. So...
	if 'this is a massive hack':
		oldalphabet = model.alphabet
		model.alphabet = set(oldalphabet).intersection(alphabet)
		txt = model.clean(txt)
		model.alphabet = oldalphabet
	
	if isinstance(codebook, dict) and 'Matrix' in codebook: codebook = codebook['Matrix']
	
	if isinstance(codebook, str): codebook = [[int(i) for i in row.split()] for row in codebook.strip().replace('.','0').replace('-','0').split('\n')]
	codebook = numpy.asarray(codebook)
	
	N,L = codebook.shape
	if N != len(alphabet): raise ValueError("number of rows in the codebook should be equal to the number of symbols in the alphabet")
	
	out = txt[:0]
	nchars = len(txt)
	correct = numpy.zeros((nchars,), dtype=numpy.bool)
	nepochs = numpy.zeros((nchars,), dtype=numpy.int16)
	z = SigTools.invcg(balanced_acc)
	
	confusion = numpy.zeros((2,2),dtype=numpy.float64)
	correct_running = SigTools.running_mean()
	nepochs_running = SigTools.running_mean()
	try:
		for i in range(nchars):
			correct_symbol = txt[i]
			correct_index = alphabet.index(correct_symbol) # will throw an error if not found, and rightly so
			context = model.prepend_context(txt[:i], default_context)
			d = TextPrediction.Decoder(choices=alphabet,
			                           context=context, model=model, verbose=False, 
			                           threshold=threshold, min_epochs=min_epochs, max_epochs=max_epochs,
			                           minprob=minprob, exponent=exponent)
			result = None
			while result == None: 
				#__IPYTHON__.dbstop()
				col = codebook[:, d.L % L]
				Cij = int(col[correct_index])
				d.new_column(col)
				mean = z * {0:-1, 1:1}[Cij]
				x = numpy.random.randn() + mean
				p = SigTools.logistic(2.0 * z * x)
				confusion[Cij, int(round(p))] += 1
				result = d.new_transmission(p)
			nepochs[i] = d.L
			correct[i] = (result == correct_symbol)
			out = out + result
			nepochs_running += d.L
			correct_running += (result == correct_symbol)
			if w:
				sys.stdout.write(result); sys.stdout.flush()
				if (i+1) % w == 0: sys.stdout.write('   %3d\n' % round(100.0 * float(i+1)/nchars))
				elif (i+1) == nchars: sys.stdout.write('\n')
	except KeyboardInterrupt:
		if w: sys.stdout.write('\n')
		pass
	
	ndone = len(out)

	s = sstruct()
	s.alphabet = alphabet
	s.codebook = codebook
	s.model = modelin
	s.input = txt[:ndone]
	s.output = out
	s.conditions = sstruct()
	s.conditions.threshold = threshold
	s.conditions.min_epochs = min_epochs
	s.conditions.max_epochs = max_epochs
	s.conditions.minprob = minprob
	s.conditions.exponent = exponent
	s.epoch_acc = sstruct()
	s.epoch_acc.desired = balanced_acc
	s.epoch_acc.empirical_nontarget, s.epoch_acc.empirical_target = (confusion.diagonal() / confusion.sum(axis=1)).flat
	s.epoch_acc.confusion = confusion
	s.nepochs = sstruct()
	s.nepochs.each  = nepochs[:ndone]
	s.nepochs.mean  = nepochs_running.m
	s.nepochs.std   = nepochs_running.v_unbiased ** 0.5
	s.nepochs.ste   = s.nepochs.std / nepochs_running.n ** 0.5
	s.letter_acc = sstruct()
	s.letter_acc.each = correct[:ndone]
	s.letter_acc.mean = correct_running.m
	s.letter_acc.std  = correct_running.v_unbiased ** 0.5
	s.letter_acc.ste  = s.letter_acc.std / correct_running.n ** 0.5
	return s

# -*- coding: utf-8 -*-
# 
#   $Id: TextPrediction.py 4633 2013-10-30 20:57:39Z jhill $
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
# -*- coding: utf-8 -*-
__all__ = [
	'FixupDistribution',
	'Decoder',
	'LoadLanguageModel', 'LanguageModel',
	'PPM', 'NGram', 'MKN',
	'pdict'
]

import numpy
import os,sys
import codecs
from . import TreeStructure
from .ProgressMonitoring import progress
import pickle, gzip, time
import md5

##############################################################################################

def FixupDistribution(p, minprob=None, exponent=1.0):
	cl = p.__class__
	isdict = isinstance(p, dict)
	if isdict: k,p = list(zip(*list(p.items())))

	n = float(len(p))
	if minprob == None: minprob = 0.5 / n
	p = numpy.asarray(p, dtype=numpy.float64)
	if p.sum(): p = p / p.sum()
	p = numpy.clip(p, minprob, 1.0) - minprob
	div = p.sum()
	if div == 0.0: div = 1.0
	p = minprob + p / div * (1.0 - n * minprob)
	p = p / p.sum()
	
	p = p ** exponent
	p = p / p.sum()

	if isdict: p = cl(dict(list(zip(k,p))))
	return p
	
##############################################################################################

def stringify(k):
	if isinstance(k, (str, int, float, bool)): return str(k)
	if isinstance(k, list): return '[' + ','.join([stringify(x) for x in k]) + ']' 
	if isinstance(k, tuple): return '(' + ','.join([stringify(x) for x in k]) + {1:','}.get(len(k), '') + ')' 
	return "<%s.%s instance at 0x%08X>" % (k.__class__.__module__,k.__class__.__name__,id(k))

##############################################################################################

class pdict(dict):
	def __repr__(self):
		w = 50
		s = ''
		kv = sorted([(stringify(k),v) for k,v in list(self.items())])
		sn = max([len(k) for k,v in kv] + [5])
		vals = [v for k,v in kv]
		vmin, vmax, vsum = float(min(vals)),float(max(vals)),float(sum(vals))
		for k,v in kv:
			if vmax == 0: n = 0
			else: n = int(round(w * v / vmax))
			s += '%s : %s \n' % (k.rjust(sn), '*' * n)
		s += '%s = %g \n' % ('min'.rjust(sn), vmin)
		s += '%s = %g \n' % ('max'.rjust(sn), vmax)
		s += '%s = %g \n' % ('total'.rjust(sn), vsum)
		return s

##############################################################################################
##############################################################################################

class Decoder(object):

	##########################################################################################

	def __init__(self, choices,    mapping=None, labels=None,
	                   context='', model=None, verbose=None,
	                   threshold=1.0, min_epochs=1, max_epochs=None, 
	                   minprob=None, exponent=1.0, cromwell=0.01):
		"""
		<choices> is a list of N possible choices. The element values are
		arbitrary here, having meaning only to the interface which calls this.
		
		<mapping> is a list of N items corresponding to the items of <choices>.
		Each item is a sequence (a string, list or tuple) of the symbols of the
		alphabet that the corresponding choice might lead to: that is, the user
		should choose option choices[i] if he wants to transmit any of the
		symbols in mapping[i].
		"""###
		self.choices = choices
		if mapping == None: mapping = [(c,) for c in choices]
		if labels == None: labels = [' '.join(map(str,sorted(x))) for x in mapping]
		self.mapping = mapping
		self.labels = labels
		self.threshold = threshold
		self.min_epochs = min_epochs
		self.max_epochs = max_epochs
		self.context = context
		self.model = model # will contain the alphabet
		self.N = len(self.choices)
		self.L = 0
		self.minprob = minprob
		self.exponent = exponent
		self.cromwell = cromwell
		self.verbose = verbose
		if self.verbose == None: self.verbose = getattr(self.model, 'verbose', 0)

		self.loglikelihood = numpy.zeros((self.N,), dtype=numpy.float64)
		self.logprior = numpy.log(numpy.ones((self.N,), dtype=numpy.float64) / self.N)

		if self.model != None:
			dist = self.model.predict_letter(context) # over symbols of the alphabet
			prior = numpy.zeros((self.N,), dtype=numpy.float64)
			for choice_index in range(self.N):
				symbols = mapping[choice_index]
				for s in symbols: prior[choice_index] += dist.get(s, 0.0)
			prior = FixupDistribution(prior, minprob=self.minprob, exponent=self.exponent)			
			self.logprior = numpy.log(prior)
		self.cols = []
		self.probs = []
		self.done = False
		
		if self.verbose:
			context = getattr(self.model, 'last_used_context', context)
			print('\n\n\n"' + context + '"\n')
			print(self.prior())

	##########################################################################################
	
	def prior(self):
		return pdict(list(zip(self.labels, numpy.exp(self.logprior).flat)))
	
	##########################################################################################
	
	def likelihood(self):
		return pdict(list(zip(self.labels, numpy.exp(self.loglikelihood).flat)))
	
	##########################################################################################
	
	def posterior(self):
		posterior = numpy.exp(self.loglikelihood + self.logprior)
		posterior /= posterior.sum()
		return pdict(list(zip(self.labels, numpy.exp(posterior).flat)))

	##########################################################################################

	def new_column(self, col):
		if col.size != self.N: raise RuntimeError('wrong-sized codebook passed to Decoder (expected %d, got %d)' % (self.N, col.size))
		self.cols.append(col)
			
	##########################################################################################

	def new_transmission(self, p, force_answer=False):
		p = max(self.cromwell, min(1.0 - self.cromwell, p))
		self.probs.append(p)
		while len(self.cols) and len(self.probs) and (self.max_epochs == None or self.L < self.max_epochs):
			pj = self.probs.pop(0)
			C_j = self.cols.pop(0)
			self.L += 1
			loglike = numpy.log([1-pj, pj])[[int(c) for c in C_j.flat]]
			self.loglikelihood += loglike
			#self.eachcol = getattr(self, 'eachcol', []); self.eachloglike = getattr(self, 'eachloglike', []) # &&&
			#self.eachcol.append(C_j); self.eachloglike.append(loglike) # &&&
		if self.verbose:
			print('got probability', p)
			print('likelihood:')
			print(self.likelihood())
			print('posterior:')
			print(self.posterior())
		posterior = numpy.exp(self.loglikelihood + self.logprior)
		posterior /= posterior.sum()
		if self.done: return
		if self.max_epochs != None and self.L >= self.max_epochs: force_answer = True
		if self.min_epochs != None and self.L < self.min_epochs and not force_answer: return
		if force_answer or max(posterior) > self.threshold:
			atMAP = (posterior == max(posterior))
			if sum(atMAP) == 1: answer = numpy.argmax(posterior)     # if there's a single maximum-a-posteriori candidate, use it...
			else:
				posterior = self.loglikelihood * atMAP               # ...otherwise, judge among the MAP candidates according to their likelihood (uninfluenced by the prior)
				atMAP = (posterior==max(posterior))
				if sum(atMAP) == 1: answer = numpy.argmax(posterior) # ...and if there's one winner of *that*, choose it...
				else:
					posterior = self.logprior * atMAP                # ...otherwise judge among the winners according to their prior probabilities
					answer = numpy.argmax(posterior)                 # ...and if there's one winner of *that*, choose it, otherwise choose an arbitrary one of the winners
			self.done = True
			if self.verbose:
				print("chosen", stringify(self.choices[answer]))
			return self.choices[answer]

	##########################################################################################

	def evolution(self, n=None, flash=False, log=False, **kwargs): # &&&
		e = numpy.array(self.eachloglike).T
		if n == None: n = e.shape[1]
		c = numpy.array(self.eachcol).T
		c = numpy.c_[c[:,1]*0, c[:, :n]]
		ll = numpy.concatenate((numpy.expand_dims(self.logprior,1)*0+1, e[:, :n]), axis=1)
		lp = numpy.concatenate((numpy.expand_dims(self.logprior,1)*1+0, e[:, :n]), axis=1)
		ll = numpy.exp(numpy.cumsum(ll, axis=1)); ll = ll / numpy.expand_dims(ll.sum(axis=0),0)
		lp = numpy.exp(numpy.cumsum(lp, axis=1)); lp = lp / numpy.expand_dims(lp.sum(axis=0),0)
		lab = self.labels		
		if log:
			lp,ll = numpy.log(lp),numpy.log(ll)
			vmin,vmax = numpy.log(0.01), 0.0
		else:
			vmin,vmax = 0.0,1.0
			
		import SigTools.Plotting
		pylab = SigTools.Plotting.load_pylab()
		pylab.clf();
		sp = {True:3, False:2}[flash]
		if 1:     ax = pylab.subplot(1,sp,1); pylab.imshow(ll, vmin=vmin, vmax=vmax, interpolation='nearest', aspect='auto'); ax.set_yticks(list(range(self.N))); ax.set_yticklabels(lab) ; ax.set_title('%s likelihood' % kwargs.get('title', ''))
		if 1:     ax = pylab.subplot(1,sp,2); pylab.imshow(lp, vmin=vmin, vmax=vmax, interpolation='nearest', aspect='auto'); ax.set_yticks(list(range(self.N))); ax.set_yticklabels(lab) ; ax.set_title('%s posterior' % kwargs.get('title', ''))
		if flash: ax = pylab.subplot(1,sp,3); pylab.imshow(c,  vmin=0,    vmax=1,    interpolation='nearest', aspect='auto'); ax.set_yticks(list(range(self.N))); ax.set_yticklabels(lab) ; ax.set_title('%s flash' % kwargs.get('title', ''))
		return ll,lp
		
##############################################################################################
##############################################################################################

rsrcpath = [
	os.path.join(os.path.dirname(__file__), 'rsrc'), 
	#os.path.join(os.path.dirname(__file__), '..', '..', '..', '..', '..', '..', '..', '..', '..', 'coding', 'rsrc'),
]
rsrcpath = [os.path.realpath(d) for d in rsrcpath if os.path.isdir(d)]

def FindFile(filename):
	if filename == None: return None
	f = os.path.abspath(filename)
	if os.path.isfile(f): return f
	p = ['.'] + rsrcpath + sys.path
	for d in p:
		f = os.path.realpath(os.path.join(d,filename))
		if os.path.isfile(f): return f
	return filename

def ReadUTF8(filename):
	fh = open(filename)
	txt = fh.read()
	fh.close()
	txt = txt.decode('utf-8').lstrip(str(codecs.BOM_UTF8, 'utf-8'))
	txt = txt.replace('\r\n', '\n').replace('\r', '\n')
	return txt

def LoadLanguageModel(filename, **kwargs):
	if filename.lower().endswith('.gz'): openfn = gzip.open
	else: openfn = open
	filename = FindFile(filename)
	print('loading language model from %s ...' % filename)
	obj = pickle.load(openfn(filename, 'rb'))
	for k,v in list(kwargs.items()): setattr(obj, k, v)
	print('done')
	return obj

##############################################################################################

default_alphabet36 = 'abcdefghijklmnopqrstuvwxyz -.,;:"\'?!'
default_alphabet   = ' -.,;:"\'?!()0123456789abcdefghijklmnopqrstuvwxyz'
german_alphabet    = ' -.,;:"\'?!()0123456789abcdefghijklmnopqrstuvwxyzäöüß'

default_corpus = 'dasher-english-corpus.txt'
default_translations = 'translation-table-utf8.txt'
default_test_text = 'obama.txt'

##############################################################################################
##############################################################################################

class LanguageModel(object):

	##########################################################################################

	def __init__(self, filename=None, alphabet=None, translation_file=None, space_delimited=False, max_chars=16, max_words=3, trim=True, verbose=False):
		if alphabet == None: alphabet = default_alphabet
		self.alphabet = alphabet
		self.space_delimited = space_delimited
		self.translation_file = FindFile(translation_file)
		self.max_chars = max_chars
		self.max_words = max_words
		self.trim = trim
		self.verbose = verbose
		self.corpus = None
		self.timestamp = None

		self.prefixtree = TreeStructure.trie()
		if filename != None: self.loadtrie(filename)	
		self.translations = self.read_translations(self.translation_file)
		self.hashattr = ['space_delimited', 'max_words', 'max_chars', 'trim', 'translations']
		
	##########################################################################################

	def clean(self, text):
		text = self.match_case(text)
		if '\n' not in self.alphabet: text = text.replace('\n', ' ')
		text = ['. '] + list(text) + [' ']
		pr = progress(len(text), 'cleaning')
		for i in range(len(text)):
			if i > 0: prevx = text[i-1]
			else: prevx = ''
			if i+1 < len(text): nextx = text[i+1]
			else: nextx = ''
			
			x = text[i]
			if not x in self.alphabet: x = self.translations.get(x,'')			
			if self.space_delimited:
				if x in '.,?!;:' and nextx == ' ': x = ' ' + x
				elif x in '"()[]{}' and prevx == ' ': x = x + ' '
				# In space_delimited mode, space now unambiguously denotes the end of a word. Ending the sentence
				# is a separate issue (only likely to happen after the end of a word). So effectively, each punctuation
				# concept ("sentence ends now", "sentence ends now as a question", etc...) has been made a "word". For
				# now, the default is *not* to do things in space_delimited mode, but in future it would be desirable.
				# However, TODO: how to undo this at the prediction stage??
	
			if prevx.endswith(' ') or prevx.endswith('\n'): x = x.lstrip(' ') # collapse multiple spaces
			text[i] = x
			if i%5000 == 0: pr.update(i)
		pr.done()
		text = ''.join(text)
		return text
	
	##########################################################################################

	def gethash(self, length=16):
		attr = tuple(sorted([
			(p,getattr(self,p)) for p in self.hashattr
		]))
		m = md5.md5()
		m.update(pickle.dumps(attr, 0))
		return m.hexdigest()[:length]

	##########################################################################################

	def pre_save(self):
		return self.__class__.__name__

	##########################################################################################

	def save(self, filestem='', compress=True):
		if self.timestamp == None: self.timestamp = time.localtime()[:6]
		dsaved = dict(self.__dict__)
		desc = self.pre_save()
		if self.prefixtree.isondisk():
			self.prefixtree = self.prefixtree.read()
		self.pack()
		filename = '-'.join([x for x in [
			filestem,
			desc,
			'%s' % self.gethash(8), 
			'%s' % self.prefixtree.gethash(8), 
			# '%04d%02d%02d%02d%02d%02d' % time.localtime()[:6],
		] if len(x)])
		filename += '.pk'

		if compress:
			openfn = gzip.open
			filename += '.gz'
		else:
			openfn = open
		print("saving language model as %s ..." % filename)
		pickle.dump(self, openfn(filename, 'wb'), -1)
		self.__dict__.update(dsaved)
		print('done')
		
	def loadtrie(self, filename):
		filename = FindFile(filename) # TODO: @@@ more graceful behaviour if not found?
		self.prefixtree = TreeStructure.trie(filename)
		self.alphabet = sorted(self.prefixtree.dist('').keys())
		
	##########################################################################################
	
	def cases(self):
		"""
		Return (loweronly, upperonly, both) which are counts of the number of elements in
		self.alphabet for which different .upper() and .lower() case versions theoretically
		exist, and...
		
		loweronly:  only the lower-case version is present
		upperonly:  only the upper-case version is present
		both:       both versions are present
		"""###
		
		lowers = set()
		uppers = set()
		for c in self.alphabet:
			if not hasattr(c, 'lower') or not hasattr(c, 'upper'): continue
			up,lo = c.upper(),c.lower()
			if up == lo: continue
			if c == up: uppers.add(lo)
			if c == lo: lowers.add(lo)
		
		loweronly = len(lowers - uppers)
		upperonly = len(uppers - lowers)
		both      = len(lowers.intersection(uppers))
		return loweronly,upperonly,both
	
	##########################################################################################

	def read_corpus(self, filename=None):
		"""
		Read a given text file, "clean" it according to the specified alphabet, and
		return the resulting content.  The text file is assumed to be either ASCII
		or UTF-8 encoded.
		"""###
		if filename == None: filename = default_corpus
		filename = FindFile(filename) # TODO: @@@ more graceful behaviour if not found?
		t = ReadUTF8(filename)
		return self.clean(t)
		
	##########################################################################################

	def read_translations(self, filename=None):
		if filename == None:
			filename = FindFile(default_translations)
			if not os.path.isfile(filename): return {}
			# TODO: @@@ is this really satisfactory? fail silently if using default, and default not found
		else:
			filename = FindFile(filename)
		t = ReadUTF8(filename)
		t = t.split('\n') 
		d = {}
		for line in t:
			w = line.split(' ')
			if len(w) == 0: continue
			src,cand = w[0], w[2:]
			if src in self.alphabet: continue
			cand = [c for c in cand if len(c) and False not in [ci in self.alphabet for ci in c]]
			if len(cand): d[src] = cand[0]
		return d
	
	##########################################################################################

	def build(self, text=None):
		"""
		Take text (for example as returned by corpus() ) and build a trie. The
		maximum n-gram length is self.max_chars characters or self.max_words
		words, whichever is shorter.
		"""###
		if text == None: text = self.read_corpus()
		t = TreeStructure.trie()
		istop = len(text)
		pr = progress(istop, 'building language model')
		for i in range(istop):
			substr = ''; j = i; nwords = 0; lastchar = ' '
			while j<istop:
				char = text[j]
				substr += char
				endword = (char == ' ' and lastchar != ' ')
				lastchar = char
				j += 1
				if endword:
					nwords += 1
					if len(substr) >= self.max_chars or nwords >= self.max_words: break
			#print '"'+substr+'"'
			t.add(substr)
			if i%5000 == 0: pr.update(i, '(%d nodes)'%len(t.nodes))
		self.prefixtree = t
		self.corpus = text
		self.timestamp = time.localtime()[:6]
		return self
		
	##########################################################################################
	
	def pack(self):
		if not self.prefixtree.ispacked(): self.prefixtree = self.prefixtree.pack()
		return self
	
	##########################################################################################

	def match_case(self, txt):
		loweronly,upperonly,both = self.cases()
		if hasattr(txt, 'lower') and loweronly > 0 and upperonly == 0 and both == 0: txt = txt.lower()
		if hasattr(txt, 'upper') and upperonly > 0 and loweronly == 0 and both == 0: txt = txt.upper()
		return txt
		
	##########################################################################################

	def trim_until_found(self, txt):
		"""
		Trim the context from the left until it *is* actually found in the trie.
		(actually, grow it from the right and stop at the longest string that is found).
		without this, the algorithm seems to return a flat distribution whenever a context
		is used that does not appear as an exact substring in the corpus.  Clearly this is
		crude: more sophisticated language models will not require this.
		"""###
		
		found = ''
		for i in range(1,len(txt)+1):
			query = txt[-i:]
			if self.prefixtree.retrieve(query) == None: break
			else: found = query
		txt = found
		
		return txt
		
	##########################################################################################

	def prepend_context(self, txt, prefix='. '):

		baseline = self.prefixtree.dist('')
		cc, prefix = prefix[::-1],''
		for c in cc:
			if c not in baseline: break
			prefix = c + prefix
		if len(prefix) == 0: return txt
		rm = 0
		for i in range(1, len(prefix)+1):
			if txt.startswith(prefix[-i:]): rm = i
		if rm: prefix = prefix[:-rm]
		if len(prefix): txt = prefix + txt

		return txt
		
	##########################################################################################
	
	def prepare_context(self, context):
		# do not exceed the character limit used while building the trie (including the next char)
		context = context[-self.max_chars+1:]

		# on the assumption that the next character is a non-space, do not exceed the word limit
		# used while building the trie
		backwards, context = context[::-1], ''
		nwords, wasspace = 0, False
		for c in backwards:
			isspace = (c == ' ')
			if isspace and not wasspace: nwords += 1
			if not isspace and wasspace and nwords >= self.max_words: break
			context = c + context
			wasspace = isspace

		# model option:  trim until found
		if self.trim: context = self.trim_until_found(context)
		
		# store
		self.last_used_context = context
		return context

	##########################################################################################

	def predict_letter(self, context):
		context = self.prepare_context(context)
		return pdict(self.prefixtree.dist(context))

	##########################################################################################
		
	def predict_word_completion(self, wordstem, prevwords=''):
		
		startword = ' ' # may only be one
		endword = '.,?!;:\'"[](){}\n\t '		
		if startword not in self.prefixtree.dist(''):
			raise RuntimeError('the word-delimiter is not in the language-model\'s alphabet')
		
		words,stemcount = [],0

		prevwords = startword + prevwords.lstrip(endword)
		if prevwords[-1] not in endword: prevwords = prevwords + startword
		
		wordstem = wordstem.split(' ')[-1]
		nodenum,substr = self.prefixtree.walk(prevwords + wordstem)
		if substr == prevwords + wordstem:
			stemcount = self.prefixtree.nodes[nodenum].count
			t = [(nodenum,wordstem)]
			while len(t):
				nodenum,substr = t.pop()
				d = self.prefixtree.nodes[nodenum].children
				wordcount = 0
				for k,v in list(d.items()):
					if k in endword: wordcount += self.prefixtree.nodes[v].count
					else: t.append((v,substr+k))
				wordcount = float(wordcount)/float(stemcount)
				if wordcount: words.append((wordcount, substr))
		return sorted(words), stemcount
			
##############################################################################################
##############################################################################################

class PPM(LanguageModel):
	"""
	Provides the classical PPM (Prediction by partial matching) distributions behavior
	by combining the n-grams with the method C. Returns the distribution for the next
	letter, based on the given context.
	"""###
	
	##########################################################################################
	
	def __init__(self, *pargs, **kwargs):
		#super(PPM, self).__init__(*pargs, **kwargs)
		LanguageModel.__init__(self, *pargs, **kwargs)
	
	##########################################################################################

	def predict_letter(self, context):
		
		baseline = self.prefixtree.dist('')
		context = self.prepare_context(context)
		if len(context)==0: return baseline
		counts = dict()
		preFactor = 1.0
		#combine n-grams
		for i in range( len(context)):
			newCounts = self.prefixtree.dist(context[i:])
			if len(newCounts) == 0: break
			normFactor = float(len(newCounts) + sum(newCounts.values()))			
			#update distribution
			for k in list(newCounts.keys()):
				counts[k] = counts.get(k, 0.0) + preFactor * newCounts[k] / normFactor #normalize by Method C
			preFactor *= len(newCounts) / normFactor
		#uniform distribution over alphabet
		alphabet = list(baseline.keys())
		for k in alphabet:
			counts[k] = counts.get(k, 0.0) + preFactor / len(alphabet)
		return pdict(counts)

##############################################################################################
##############################################################################################

class NGram(LanguageModel):
	def __init__(self, *pargs, **kwargs):
		"""
		e.g. to construct a trigram model:    NGram(3, ...)   or   NGram(..., N=3)
		where ... are the other input arguments to LanguageModel.
		"""###
		pargs = list(pargs)
		if len(pargs) and isinstance(pargs[0], int) and not 'N' in kwargs: self.N = pargs.pop(0)
		else: self.N = kwargs.pop('N', None)
		if self.N == None: raise TypeError('argument N was not supplied')
		LanguageModel.__init__(self, *pargs, **kwargs)
		self.hashattr += ['N']

	def pre_save(self):
		return '%s%d' % (self.__class__.__name__, self.N )

	def predict_letter(self, context):
		context = context[-self.N:]
		context = self.prepare_context(context)
		return pdict(self.prefixtree.dist(context))
		
##############################################################################################
##############################################################################################

class MKN(LanguageModel):
	def __init__(self, *pargs, **kwargs):
		"""
		A language model which is
		
		- Interpolated:  meaning all context lengths are combined for all
		  predictions, irrespective of whether the predicted string appears
		  in the corpus (in contrast to a backoff model---see [1], pp15--16).
		
		- Kneser-Ney-based: so that absolute discounting of n-gram frequency
		  estimates is used, and lower-than-maximum-order models use a
		  strange way of counting (extended context counts as opposed to
		  n-gram occurrence counts). This is what was used in [1] and
		  corresponds to the variant called MODKN-EXTEND in [2] (pp 3--4).
		  
		- Modified: meaning that the modification proposed in [1] is used,
		  namely that the discounting value has a dependency on the value of
		  the count it is discounting.
		
		[1] Chen, S. F. and Goodman, J. (1998): An Empirical Study of Smoothing
		Techniques for Language Modeling. Technical Report TR-10-98, Center
		for Research in Computing Technology (Harvard University), August 1998.
		
		[2] James, F. (2000): Modified Kneser-Ney Smoothing of n-gram Models.
		Technical Report 00.07, Research Institute for Advanced Computer
		Science (USRA/NASA), October 2000.
		"""###
		pargs = list(pargs)
		if len(pargs) and isinstance(pargs[0], int) and not 'N' in kwargs: self.maxn = pargs.pop(0)
		else: self.maxn = kwargs.pop('N', None)
		if self.maxn == None: raise TypeError('argument N was not supplied')
		self.maxc = kwargs.pop('maxc', 3)
		self.discounts = None
		self.suffixtree = None
		self.slow = False
		LanguageModel.__init__(self, *pargs, **kwargs)
		self.hashattr += ['maxn', 'maxc']

	def pre_save(self):
		if getattr(self, 'suffixtree', None) != None:
			if self.suffixtree.isondisk(): self.suffixtree = self.suffixtree.read()
		return '%s%d' % (self.__class__.__name__, self.maxn )
		
	def CountNGrams(self):
		Q = dict([(
			n,
			dict([  (i,0) for i in list(range(1, self.maxc+2))+['more']  ])
		) for n in range(1, self.maxn+1)])
		
		t = self.prefixtree.nodes
		stack = [list(t[0].children.values())]
		
		while len(stack):
			nodes = stack[-1]
			if len(nodes) == 0: stack.pop(); continue
			node = t[nodes.pop()]
			n = len(stack)
			c = node.count
			if c > self.maxc+1: c = 'more'
			Q[n][c] += 1
			if n < self.maxn: stack.append(list(node.children.values()))
		return Q
		
	def SetupDiscounts(self, maxc=None, maxn=None):
		"""
			Y[n] = Q[n][1] / (Q[n][1] + 2.0 * Q[n][2])
			D[n][1] = 1.0 - 2.0 * Y[n] * Q[2] / Q[1]
			D[n][2] = 2.0 - 3.0 * Y[n] * Q[3] / Q[2]
			D[n][3] = 3.0 - 4.0 * Y[n] * Q[4] / Q[3]
			
			where Q[n][c] is the number of strings of length n
			that appear exactly c times.
			
		"""###
		if maxn != None: self.maxn = maxn
		if maxc != None: self.maxc = maxc
		
		self.discounts = [None]
		print("counting n-grams up to length %d..." % self.maxn)
		Q = self.counts = self.CountNGrams()
		for n in range(1, self.maxn+1):
			Y = Q[n][1] / max(1.0,  Q[n][1] + 2.0 * Q[n][2])
			disc = [0]
			for c in range(1, self.maxc+1):
				D = c - (c + 1.0) * Y * float(Q[n][c+1]) / max(1.0, float(Q[n][c]))
				disc.append(D)
			self.discounts.append(disc)
		return self
		
	def N(self, d, criterion, exact=False):
		"""
			N_criterion  ( string . )  if exact==True
			N_criterion+ ( string . )  if exact==False
			
			where string is the context that led us to d, an
			unnormalized dictionary of children's counts.
		"""###
		if exact: return sum([c == criterion for wi,c in list(d.items())])
		else:     return sum([c >= criterion for wi,c in list(d.items())])

	
	def D(self, c, n):
		"""
			Discount as a function of count (adjusted so as never to go negative).
		"""###
		if n < len(self.discounts): disc = self.discounts[n]
		else: disc = self.discounts[-1]
		ind = min(len(disc)-1, c)
		d = disc[ind]
		d = min(c, d)
		return d
			
	def predict_letter(self, context):
		#import SigTools; return SigTools.unpickle('firstprior') # &&&
		if self.discounts == None: self.SetupDiscounts()
		context = context[-(self.maxn-1):]
		context = self.prepare_context(context)
		p = {}
		fac = 1.0
		nz = False
		nstart = n = len(context) + 1
		highest = True
		while True:
			ttt = time.time()
			if highest:
				counts = self.prefixtree.dist(context)
				denom = float(sum(counts.values()))
				denom = max(1.0, denom)
				gamma = 0.0				
				if n < len(self.discounts): disc = self.discounts[n]
				else: disc = self.discounts[-1]				
				for i in range(1, len(disc)):
					exact = i < len(disc) - 1
					gamma += disc[i] * self.N(counts,i,exact)
				gamma /= denom
			else:
				counts = self.kndist(context)
				v = list(counts.values())
				denom = max(1.0, float(sum(v)))
				gamma = sum([self.D(c, n) for c in v]) / denom
			for k,v in list(counts.items()):
				p[k] = p.get(k, 0.0) + fac * max(0, v - self.D(v, n)) / denom
				if p[k]: nz = True
			fac *= gamma
			ttt = time.time() - ttt
			#if self.verbose: print "n=%d:  %g sec" % (n, ttt)
			if len(context) == 0: break
			context = context[1:]
			n -= 1
			highest = False
			
		return pdict(p)

	def pack(self, *pargs, **kwargs):
		LanguageModel.pack(self, *pargs, **kwargs)
		if getattr(self, 'suffixtree', None) != None:
			if not self.suffixtree.ispacked(): self.suffixtree = self.suffixtree.pack()
		if self.discounts == None or len(self.discounts) <= self.maxn: self.SetupDiscounts()
		return self

	def kndist(self, string):
		if self.slow or self.suffixtree == None:
			return self.prefixtree.kndist(string) # slow
		
		# equivalent but much faster algorithm
		d = self.prefixtree.retrieve(string)
		if d == None: successors = []
		else: successors = list(d.children.keys())
		gnirts = string[::-1]
		p = {}
		for successor in successors:
			d = self.suffixtree.dist(successor+gnirts)
			p[successor] = self.N(d, 1, exact=False)
		return p

	def build_suffixtree(self, text=None):
		if text == None: text = self.corpus
		if text == None: text = self.read_corpus()
		prev = self.prefixtree, self.corpus, self.timestamp
		self.build(text[::-1])
		self.suffixtree = self.prefixtree
		self.prefixtree, self.corpus, self.timestamp = prev
		return self


##############################################################################################
##############################################################################################

# TODO:
#   nasty bug:  when a language model is used which takes up a lot of memory,
#               pyspeller likelihoods somehow get corrupted....  is not a function of the prior itself
#               but somehow a side-effect of using the trees...  parts marked &&& here and in BciApplication.py are for debugging this
#   flat prior results from MKN with trim=False when string gets too long.  Can that be right?
#
# 	factor in word-completions and backspace to predictive distribution
# 	implement word-completions
# 	model the backspace key

##############################################################################################
##############################################################################################

function out = CompareFilteringStrategies(fn, firLength_seconds, passband_hz, varargin)
% CompareFilteringStrategies(FILENAME, FIRLENGTH_SECONDS,  PASSBAND_HZ,  ....)
% 
% Compare the delays and phase effects of IIR, FIR and hybrid (IIR high-pass, FIR low-pass)
% filtering prior to Hilbert phase or envelope estimation.
% 
% Calls testHilbertChain,  which in turn is a BCI2000CHAIN wrapper.  Type HELP BCI2000CHAIN
% and/or go to http://www.bci2000.org/wiki/index.php/User_Reference:Matlab_Tools to understand
% the necessary dependencies.
% 
% Use additional arguments to play with the other parameters, e.g.
%  CompareFilteringStrategies( ...,   'HighPassOrder', 4, 'OutputSignal', 2, 'Delay', '0.2s', ...)
% 
if nargin < 1, fn = ''; end
if isempty(fn), fn = bci2000path('data/samplefiles/eeg3_2.dat'); end

if nargin < 2, firLength_seconds = []; end
if isempty(firLength_seconds), firLength_seconds = 0.5; end

if nargin < 3, passband_hz = []; end
if isempty(passband_hz), passband_hz = [6 12]; end
	
parms = read_bciprm(fn);
fs = parms.SamplingRate.NumericValue;
nyq = fs/2;

b = fir1(round(firLength_seconds*fs), passband_hz/nyq, 'bandpass' );

figure(1), clf, [out.envelopeFIR, out.filteredFIR] = testHilbertChain(fn, 'FIRCoefficients', b, 'OutputSignal', 1, 'Delay', '0.1s', varargin{:}); title FIR
figure(2), clf, [out.envelopeIIR, out.filteredIIR] = testHilbertChain(fn, 'HighPassCorner', passband_hz(1), 'LowPassCorner', passband_hz(2), 'OutputSignal', 1, 'Delay', '0.1s', varargin{:}); title IIR
figure(3), clf, [out.envelopeHybrid, out.filteredHybrid] = testHilbertChain(fn, 'FIRCoefficients', b, 'HighPassCorner', passband_hz(1), 'OutputSignal', 1, 'Delay', '0.1s', varargin{:}); title Hybrid

if exist('plotsig')
	channel = 1;
	signals = {
		out.filteredFIR.Signal(:, channel)
		out.filteredIIR.Signal(:, channel)
		out.filteredHybrid.Signal(:, channel)
	}';
	signals(2, :) = {fs};
	
	figure(4), clf, plotsig(signals{:})
end
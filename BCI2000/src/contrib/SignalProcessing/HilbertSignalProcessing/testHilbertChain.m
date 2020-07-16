function [output, filtered, raw] = testHilbertChain(fn, varargin)

% example
%
%     fn = bci2000path('data/samplefiles/eeg3_2.dat');
%     parms = read_bciprm(fn);
%     fs = parms.SamplingRate.NumericValue;
%     nyq = fs/2;
%     firLength_seconds = 0.5;
%     passband_hz = [6 12];
%     b = fir1(round(firLength_seconds*fs), passband_hz/nyq, 'bandpass' );
% 
%     [envelope, filtered] = testHilbertChain(fn, 'FIRCoefficients', b, 'OutputSignal', 1, 'Delay', '0.1s');
% 
% Black: raw signal
% Blue:  filtered signal (by FIR or IIR)
% Cyan:  filtered signal (delayed by the same amount as the delay HilbertFilter will introduce)
% Red:   HilbertFilter output signal
% 
% Note that in the realtime system, the FIR will introduce a further delay in
% addition to the delay entailed by the Hilbert transformer (and to be effective,
% the FIR will probably need to be much longer than the Hilbert delay). IIR
% filtering is available as an alternative, entailing shorter effective delays,
% but beware non-linear phase response in the pass-band, leading to potentially
% inaccurate estimates of Hilbert phase.
% 
% NB: requires BCI2000CHAIN, and all its dependencies.  Read the help there, and/or visit
% http://www.bci2000.org/wiki/index.php/User_Reference:Matlab_Tools
% 

if nargin < 1, fn = ''; end
if isempty(fn), fn = bci2000path('data/samplefiles/eeg3_2.dat'); end

pdef = {  % definition/defaults, in case these parameters are not defined in the data file
	'Filtering:Spatial%20Filter  int       SpatialFilterType=  0      0    0 3 // '
	'Filtering:IIR%20Filter      float     HighPassCorner=     0      0    0 % // '
	'Filtering:IIR%20Filter      int       HighPassOrder=      2      2    0 % // '
	'Filtering:IIR%20Filter      float     LowPassCorner=      0      0    0 % // '
	'Filtering:IIR%20Filter      int       LowPassOrder=       4      4    0 % // '
	'Filtering:IIR%20Filter      float     NotchCenter=        0     60Hz  0 % // '
	'Filtering:IIR%20Filter      int       NotchOrder=         4      4    0 % // '
	'Filtering:IIR%20Filter      float     FilterGain=         1.0    1.0  0 % // '
	'Filtering:FIR%20Filtering   floatlist FIRCoefficients= 0         1    % % // FIR Filter Coefficients'
	'Filtering:Analytic%20Signal int       OutputSignal=       1      1    0 4 // Signal provided to the next filter: 0: Input Signal, 1: Hilbert envelope 2: Hilbert phase 3: Real part - delayed original signal 4: Imaginary part - Hilbert transform // (enumeration)'
	'Filtering:Analytic%20Signal float     Delay=              0.1s   0.1s 0 % // Delay for convolution with FIR Hilbert transformer'
	'Filtering:Derivative        int       DiffOrder=          0      0    0 % // Order of derivative (0 disables)'
	'Filtering:Derivative        float     UnwrapDiff=         0      0    0 % // Unwrapping period (NB: 2*pi=6.283185307179586232;  0 disables)'
	'Filtering:Derivative        int       DivideByDeltaTime=  1      1    0 1 // 0:No - return raw numeric diffs, 1:Yes - return derivative  (enumeration)'
};

chain = {'TransmissionFilter', 'SpatialFilter', 'CustomIIRFilter', 'CustomFIRFilter', 'HilbertFilter', 'DiffFilter'};

output = bci2000chain(fn, chain, pdef, fn, varargin{:});

raw = bci2000chain(fn, chain(1:2), pdef, fn, varargin{:});
filtered = bci2000chain(fn, chain(1:find(strcmp(chain, 'HilbertFilter'))-1), pdef, fn, varargin{:});
delay = decode_bcitime(output.Parms.Delay, '-RoundToSamples', output);
channel = 1;
plot(raw.FullTime, raw.Signal(:, channel)-mean(raw.Signal(:, channel)), 'kx-', filtered.FullTime, filtered.Signal(:, channel), 'bx-', filtered.FullTime + delay, filtered.Signal(:, channel), 'cx-', output.FullTime, output.Signal(:, channel), 'rx-')
grid on, zoom xon, figure(gcf), drawnow

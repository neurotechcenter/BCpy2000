function [seconds, err] = decode_bcitime( paramVal, varargin )
% DECODE_BCITIME    Decode as seconds any BCI2000 parameter value that is supposed to indicate time
% 
% SECONDS = DECODE_BCITIME(P  [, ... ])
% 
% P might be:
%      a numeric value (or array of numeric values)
%      a string (or cell array of strings)
%      a scalar structure with a cell array of strings in its .Value field 
%        (i.e. a substruct of the structure output of convert_bciprm / read_bciprm / make_bciprm)
% 
% P (or each element of P) should be a string with a BCI2000 PhysicalUnit string appended to it
% ending in 's', hence indicating some number of seconds, milliseconds, microseconds, etc.
% Numerical values of P are returned, definitively in seconds.
% 
% Alternatively, each element of P may be (possibly a string representations of) a bare number
% without any PhysicalUnit string. In this case it indicates a number of SampleBlocks, and
% additional arguments are required in order to specify the SamplingRate and SampleBlockSize
% for conversion of the values of P to seconds.  The additional arguments may be any sequence
% of arguments that can be interpreted and collated by MAKE_BCIPRM to provide the necessary
% information.
% 
% The following examples all return 0.2 :
% 
%      decode_bcitime( '200ms' )
%      decode_bcitime( '0.2s' )
%      decode_bcitime( '00:00.2' )
%      decode_bcitime( 5, 'SampleBlockSize', 40, 'SamplingRate', 1000 )
%      decode_bcitime( '5', 'SampleBlockSize', 20, 'SamplingRate', 500 )
% 
% Optionally, DECODE_BCITIME can also automatically round its output (still expressed seconds)
% such that it corresponds to a whole number of samples, or to a whole number of SampleBlocks:
% 
%      SECONDS = DECODE_BCITIME( P, '-RoundToSamples', ... ) 
%      SECONDS = DECODE_BCITIME( P, '-RoundToBlocks', ... ) 
% 
% [SECONDS, ERR] = DECODE_BCITIME( ... ) catches any interpretation error instead of crashing.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% $Id: decode_bcitime.m 3899 2012-03-28 16:00:20Z jhill $ 
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

err = {};

if isstruct(paramVal), paramVal = paramVal.Value; end
if ~iscell(paramVal), paramVal = {paramVal}; end

rounding = 'none';
if numel(varargin)
	if ischar(varargin{1}) & size(varargin{1}, 1) == 1
		switch lower(varargin{1})
			case {'-roundtosamples'}
				rounding = 'samples';
				varargin(1) = [];
			case {'-roundtoblocks', '-roundtosampleblocks'}
				rounding = 'blocks';
				varargin(1) = [];
		end
	end
end

SamplesPerSecond = 0; SamplesPerBlock = 0; 
SecondsPerBlock = 0; BlocksPerSecond = 0;
if numel(varargin)
	template = {
		'Blah float SamplingRate= 0 0 % % // '
		'Blah float SampleBlockSize= 0 0 % % // '
	};
	[pstr parms] = make_bciprm(template, varargin{:});
	SamplesPerSecond = parms.SamplingRate.NumericValue;
	SamplesPerBlock = parms.SampleBlockSize.NumericValue;
	if SamplesPerSecond & SamplesPerBlock
		BlocksPerSecond = SamplesPerSecond / SamplesPerBlock;
		SecondsPerBlock = SamplesPerBlock / SamplesPerSecond;
	end
end


seconds = nan + zeros(size(paramVal));

factors.ps = 1e-12;
factors.ns = 1e-9;
factors.us = 1e-6;  factors.mus = 1e-6;
factors.ms = 1e-3;
factors.s  = 1e+0;
factors.ks = 1e+3;
factors.Ms = 1e+6;
factors.Gs = 1e+9;
factors.Ts = 1e+12;


for i = 1:numel(paramVal)
	t = paramVal{i};
	if isnumeric(t), t = num2str(t); end
	
	factor = 1;
	unit = '';
	while numel(t) > 0
		if any(t(end)=='0123456789.'), break, end
		unit = [t(end) unit];
		t(end) = [];
	end
	if endswith(unit, 'sec'), unit = unit(1:end-2); end
	
	if any(t == ':')                                 % Sexagesimal:  HH:MM:SS[.SSS] or MM:SS[.SSS]
		if isequal(unit, 's') | isequal(unit, '')
			tt = [];
			while ~isempty(t)
				[tti t] = strtok(t, ':');
				[tt(1, end+1) err] = evaluate(tti, err);
			end
			t = sum(tt .* 60.^(length(tt)-1:-1:0));
		else
			% should result in an error
			[t err] = evaluate([t unit], err);
		end
		factor = 1;
	elseif isfield(factors, unit)                     % A recognized PhysicalUnit ending in 's'
		[t, err] = evaluate(t, err);
		factor = getfield(factors, unit);
	elseif ~isempty(unit)                             % any other PhysicalUnit
		% should result in an error
		[t, err] = evaluate([t unit], err);
		factor = 1;
	else                                              % raw value: must convert from SampleBlocks
		if SecondsPerBlock == 0, error('need information about SamplingRate and SampleBlockSize in order to convert unitless parameter values'), end
		[t, err] = evaluate(t, err);
		factor = SecondsPerBlock;
	end
	seconds(i) = t * factor;
	switch rounding
		case 'samples'
			if SamplesPerSecond == 0, error('need information about SamplingRate in order to round values to an integer number of samples'), end
			seconds(i) = round(seconds(i) * SamplesPerSecond) / SamplesPerSecond;
		case 'blocks'
			if BlocksPerSecond == 0, error('need information about SamplingRate and SampleBlockSize in order to round values to an integer number of blocks'), end
			seconds(i) = round(seconds(i) * BlocksPerSecond) / BlocksPerSecond;
	end
end
if ~isempty(err), err = [{'failed to decode BCI2000 parameter value:'} err]; end
err(2, :) = {sprintf('\n   ')};
err = [err{1:end-1}];
if nargout < 2, error(err), end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function [t, err] = evaluate(t, err)
try
	t = eval(t);
	ok = 1;
catch
	err{1, end+1} = sprintf('failed to evaluate ''%s'' as a time', t);
	t = nan;
	ok = 0;
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function out = endswith(s, suffix)
out = strncmp(fliplr(s), fliplr(suffix), length(suffix));

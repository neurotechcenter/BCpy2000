function create_bcidat(datfilename, signal, varargin)
% CREATE_BCIDAT    Create a "toy" BCI2000 .dat file based on a Matlab variable.
% 
% CREATE_BCIDAT wraps the mex-file SAVE_BCIDAT in order to save a BCI2000
% .dat file whose content can be based on a very minimal set of
% specifications: a numeric signal matrix, and a scalar sampling rate.
% 
% Currently there is no way to save state variables in the file.
% Note that if you have a consistent set of [signal, states, parameters]
% from (for example) LOAD_BCIDAT, then you can simply call SAVE_BCIDAT
% directly instead of CREATE_BCIDAT.  CREATE_BCIDAT is intended more for
% the case where you want to create "toy" signals from scratch using
% Matlab, for example as a prelude to testing filters using BCI2000CHAIN.
% 
% CREATE_BCIDAT(FILENAME, SIGNAL, SAMPLINGRATE)
%   where SIGNAL is the numeric signal content (samples-by-channels)
%         SAMPLINGRATE is the numeric sampling rate in Hz.
% 
% CREATE_BCIDAT(FILENAME, SIGNAL, ...)
%   allows additional parameter filenames (or direct name,value pairs) to
%   override the defaults in the same way as MAKE_BCIPRM.  These settings
%   should include the SamplingRate parameter.
% 
% CREATE_BCIDAT(FILENAME, S)
%   where S is a struct
%         S.Signal contains the numeric signal (samples-by-channels)
%         S.Parms may optionally be a struct containing parameter definitions
% 
% CREATE_BCIDAT(FILENAME, S, ...)
%   allows additional parameter filenames (or direct name,value pairs) to
%   override the defaults (and those given in S.Parms, if any) in the same
%   way as MAKE_BCIPRM
% 
% In all cases the signal is assumed to be pre-calibrated, and the numeric
% is converted to float32 before saving. Therefore SourceChOffset is always
% saved as a list of 0s, and SourceChGain as a list of 1s.
% 
% BCI2000CHAIN allows you to use CREATE_BCIDAT transparently, creating and
% deleting the appropriate temporary file for you (not necessarily a time-
% efficient thing to do, of course). This is done by passing either a cell
% array or struct in place of the filename:
%     BCI2000CHAIN(  {SIGNAL, SAMPLINGRATE, ...}, CHAINSTR,  ....)
%     BCI2000CHAIN(  S, CHAINSTR,  ....)
%     BCI2000CHAIN(  {S, ...} , CHAINSTR,  ....)
% 
% 
% CREATE_BCIDAT has the following dependencies:
% 
%  M-files:   make_bciprm
%             read_bciprm (used by make_bciprm)
%  Mex-files: save_bcidat
%             convert_bciprm (used by make_bciprm and read_bciprm)
%             load_bcidat (used by read_bciprm in some cases)

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% $Id: create_bcidat.m 4263 2012-08-18 00:25:47Z jhill $ 
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

states = struct;
parms = varargin;
if ~isempty(parms)
	if isnumeric(parms{1})
		parms = [{'SamplingRate'} parms];
	end
end
if isempty(parms)
	if isstruct(signal) & numel(signal) == 1
		s = signal;
		signal = s.Signal;
		s = rmfield(s, 'Signal');
		if isfield(s, 'States');
			states = s.States; % TODO: currently unused---will be overwritten later
			% TODO: states are currently unused---note that this section is written to handle BCI2000CHAIN
			% output structs, and these usually have a reduced amount of state information in them (one
			% value per block, not one value per sample as SAVE_BCIDAT would require).  We could repmat up
			% each state time-series, but users might too-easily overlook the fact that some information
			% (any changes in Event states at sub-block precision) has ended up different in the new file
			% from how it was in the original file. Better to leave the state info out.
			s = rmfield(s, 'States');
		end
		if isfield(s, 'Parms'), s = s.Parms; end
		fields = fieldnames(s);
		vals = struct2cell(s);
		structFormattedParms = struct;
		for i = 1:numel(fields)
			field = fields{i};
			val = vals{i};
			if isstruct(val)
				structFormattedParms.(field) = val;
			else
				parms = [parms {field val}];
			end
		end
		parms = [{structFormattedParms} parms];
	end
end

if ~isnumeric(signal) | isempty(signal) | ndims(signal) ~= 2, error('unrecognized signal format'), end
[nSamples nChannels] = size(signal);
if nChannels > nSamples, warning(sprintf('signal interpreted as %d SAMPLES by %d CHANNELS: is that really intended?', nSamples, nChannels)), end
signal = single(signal);

parms_0 = { % these are defaults, in case nothing is specified.  Note that this results in an illegal SamplingRate that will result in an error: users must specify a SamplingRate, because it cannot be guessed
	'Source int       SamplingRate=       0                 256Hz 1 % // sample rate'
	'Source int       SampleBlockSize=    8                 8 1 % // number of samples transmitted at a time'
	'Source list      ChannelNames=     0                   % % % // list of channel names'
	'Source list      TransmitChList=   8 1 2 3 4 5 6 7 8   % % % // list of transmitted channels'
	'Source int       SourceCh=           8                 8 1 % // number of digitized and stored channels'
	'Source floatlist SourceChOffset=   8 0 0 0 0 0 0 0 0   0 % % // Offset for channels in A/D units'
	'Source floatlist SourceChGain=     8 1 1 1 1 1 1 1 1   1 % % // gain for each channel (A/D units -> muV)'
};

% collate the defaults, the optional inputs, and the mandatory overrides
[parmStr parms] = make_bciprm( ...
	parms_0, ...
	'TransmitChList', 1:nChannels, ...
	parms{:}, ...
	'SourceCh', nChannels, ...
	'SourceChOffset', zeros(nChannels,1), ...
	'SourceChGain', ones(nChannels, 1) ...
);
% the user can override TransmitChList if he really wants, but the last
% three override everything:  SourceCh must be right, and the data are
% assumed to be calibrated already, so offsets 0 and gains 1 are ensured

%disp(char(parmStr))

% sanity-check the collated parameters
if parms.SamplingRate.NumericValue == 0, error('SamplingRate was not specified'), end
if numel(parms.SourceChOffset.Value) ~= nChannels, error(sprintf('SourceChOffset parameter must have %d elements to match the number of channels', nChannels)), end
if numel(parms.SourceChGain.Value) ~= nChannels, error(sprintf('SourceChGain parameter must have %d elements to match the number of channels', nChannels)), end
if numel(parms.ChannelNames.Value) ~= nChannels & numel(parms.ChannelNames.Value) ~= 0, error(sprintf('ChannelNames parameter must have %d elements to match the number of channels', nChannels)), end
if max(parms.TransmitChList.NumericValue) > nChannels, error(sprintf('TransmitChList contains values that exceed the number of channels (%d)', nChannels)), end

% Do it!  Note that no states are saved at the moment - except the Running state which is essential for healthy FilterTool operation
states = struct('Running', uint8(ones(size(signal,1), 1)));
save_bcidat(datfilename, signal, states, parms);

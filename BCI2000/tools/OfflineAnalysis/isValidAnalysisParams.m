%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% $Id: UGenericSignal.m 2007-111-26 12:31:37EST fialkoff $ 
%% 
%% File: UGenericSignal.m 
%% 
%% Author: Joshua Fialkoff <fialkj@rpi.edu>, Gerwin Schalk <schalk@wadsworth.org>
%%
%% Description: This function inspects the structure and contents of an
%% analysis parameters structure in order to verify its integrity.
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
%% http:%%www.bci2000.org 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function errorCode = isValidAnalysisParams(params)
%isValidAnalysisParams     checks to make sure that structure of params is
%valid for usage with runBasicAnalysis.m


INVALID_PARAMS = 1;
INVALID_DATA_FILE = 2;
INVALID_TARGET_CONDITIONS = 4;
INVALID_CONDITION_LABELS = 8;
UNEQUAL_CONDITION_SIZES = 16;
INVALID_SPECTRUM_CHANS = 32;
INVALID_TOPO_FREQS = 64;
INVALID_MONTAGE_FILE = 128;
INVALID_ACQ_TYPE = 256;
INVALID_TRIAL_CHANGE_COND = 512;
NO_DATA_FILE_SPECIFIED = 1024;
INVALID_DOMAIN = 2048;


errorCode = 0;

if ~isa(params, 'struct')
  errorCode = INVALID_PARAMS;
else
  if ~(isfield(params, 'dataFiles') && ...
      isfield(params, 'targetConditions') && ...
      isfield(params, 'conditionLabels') && ...
      isfield(params, 'channels') && ...
      isfield(params, 'topoParams') && ...
      isfield(params, 'spatialFilt') && ...
      isfield(params, 'montageFile') && ...
      isfield(params, 'domain') && ...
      isfield(params, 'acqType'))
    errorCode = INVALID_PARAMS;
  else
    % check that dataFile, montageFile acqMode and domain are strings
    if ~isfield(params, 'signal')
      if isempty(params.dataFiles)
        errorCode = errorCode + NO_DATA_FILE_SPECIFIED;
      else
        for idxFile = 1:length(params.dataFiles)
          if ~isa(params.dataFiles{idxFile}, 'char')
            errorCode = errorCode + INVALID_DATA_FILE;
            break;
          end
        end
      end
    end
    if ~isa(params.montageFile, 'char') && ~isa(params.montageFile, 'struct')
      errorCode = errorCode + INVALID_MONTAGE_FILE;
    end
    if ~isa(params.acqType, 'char') || ...
        ~ismember(params.acqType, {'eeg', 'ecog'})
      errorCode = errorCode + INVALID_ACQ_TYPE;
    end
    if ~isa(params.domain, 'char') || ...
        ~ismember(params.domain, {'time', 'freq'})
      errorCode = errorCode + INVALID_DOMAIN;
    end    
    if ~isa(params.trialChangeCondition, 'char')
      errorCode = errorCode + INVALID_TRIAL_CHANGE_COND;
    end
    
    % check that targetCondions and conditionLabels are cell vectors
    if ~isa(params.targetConditions, 'cell') || ...
        max(size(params.targetConditions)) > 2 || ...
        min(size(params.targetConditions)) ~= 1
      
      errorCode = errorCode + INVALID_TARGET_CONDITIONS;
    end
    if ~isa(params.conditionLabels, 'cell') || ...
        ~all(size(params.targetConditions) == size(params.conditionLabels))
      errorCode = errorCode + INVALID_CONDITION_LABELS;
    end

    % check that targetConditions and conditionLabels are the same size
    if bitand(errorCode, ...
        bitor(INVALID_TARGET_CONDITIONS, ...
          INVALID_CONDITION_LABELS)) == 0 && ...
        all(size(params.targetConditions) ~= size(params.conditionLabels))

      errorCode = errorCode + UNEQUAL_CONDITION_SIZES;
    else
      % check that each element is a string
      for idx = 1:length(params.targetConditions)
        if bitand(errorCode, INVALID_TARGET_CONDITIONS) == 0 && ...
            ~isa(params.targetConditions{idx}, 'char')
          errorCode = errorCode + INVALID_TARGET_CONDITIONS
        end
        if bitand(errorCode, INVALID_CONDITION_LABELS) == 0 && ...
            ~isa(params.conditionLabels{idx}, 'char')
          errorCode = errorCode + INVALID_CONDITION_LABELS;
        end
        if bitand(errorCode, INVALID_TARGET_CONDITIONS) && ...
            bitand(errorCode, INVALID_CONDITION_LABELS)
          break;
        end
      end
    end

    % check that spectrumChans is a numeric vector
    if ~isempty(params.channels) && ...
        (~isnumeric(params.channels) || ...
        length(size(params.channels)) ~= 2)
      errorCode = errorCode + INVALID_SPECTRUM_CHANS;
    end

    % check that topoFreqs is a numeric vector
    if ~isempty(params.topoParams) && ... 
        (~isnumeric(params.topoParams) || ...
        length(size(params.topoParams)) ~= 2)
      errorCode = errorCode + INVALID_TOPO_FREQS;
    end
    
  end
end

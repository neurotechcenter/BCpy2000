%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% $Id: timeAnalysis.m 2007-111-26 12:31:37EST fialkoff $ 
%% 
%% File: timeAnalysis.m 
%% 
%% Author: Joshua Fialkoff <fialkj@rpi.edu>, Gerwin Schalk <schalk@wadsworth.org>
%%
%% Description: This function performs a time-domain analysis of the input
%% data
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
function [res1, res2, ressq, timeBinsMs] = timeAnalysis(...
  analysisParams, signal, states, trialnr, sigParams, verbose, ...
  errorOverride, settings)

funcName = 'p300Analysis';
num_channels=size(signal, 2);

if isfield(sigParams.SamplingRate, 'NumericValue')
  samplefreq=sigParams.SamplingRate.NumericValue;
else
  samplefreq = str2double(sigParams.SamplingRate.Value);
end
trialLength = analysisParams.dataSegLength;
condition1idxstr=sprintf(...
  'condition1idx=find((trialnr == cur_trial) & %s, 1);', ...
  analysisParams.targetConditions{1});
  
cond1 = eval(analysisParams.targetConditions{1});

if length(analysisParams.targetConditions) == 2
  condition2idxstr=sprintf(...
    'condition2idx=find((trialnr == cur_trial) & %s, 1);', ...
    analysisParams.targetConditions{2});
  cond2 = eval(analysisParams.targetConditions{2});
end

triallength=ceil(analysisParams.dataSegLength*samplefreq/1000);                     % convert from ms into samples

timeBinsMs = (.5:triallength-.5)/samplefreq * 1000;

if verbose
  fprintf(1, 'BCI2000 P3 Analysis Routine V1.20\n');
  fprintf(1, '(C) 2002-03 Gerwin Schalk\n');
  fprintf(1, '=================================\n');
end

trials=unique(trialnr);
trials = trials( trials > 0 );

cond1Cnt = 0;
cond2Cnt = 0;


bInc1 = 0;
bInc2 = 0;
lastNum = trialnr(1);

for idxSamp = 1:length(trialnr)
  trialNum = trialnr(idxSamp);
  
  if trialNum ~= lastNum || idxSamp == length(trialnr)
    if bInc1
      cond1Cnt = cond1Cnt + 1;
      bInc1 = 0;
    end
    if bInc2
      cond2Cnt = cond2Cnt + 1;
      bInc2 = 0;
    end
    lastNum = trialNum;
  end
  
  if ~bInc1 && cond1(idxSamp)
    bInc1 = 1;
  end

  if length(analysisParams.targetConditions) == 2
    if ~bInc2 && cond2(idxSamp)
      bInc2 = 1;
    end
  end
end

%preallocate memory - lots to store
% for cur_trial=min(trials):max(trials)
%   % condition 1
%   eval(condition1idxstr);
%   if (isempty(condition1idx) == 0) && ...
%       length(signal) >= condition1idx + triallength
%     cond1Cnt = cond1Cnt + 1;
%   end
%   
%   % condition 2
%   eval(condition2idxstr);
%   if (isempty(condition2idx) == 0) && ...
%       length(signal) >= condition2idx + triallength
%     cond2Cnt = cond2Cnt + 1;
%   end
% end

avgdata1 = zeros(triallength, num_channels, cond1Cnt);
avgdata2 = zeros(triallength, num_channels, cond2Cnt);


if verbose
  fprintf(1, '\r');
  fprintf(1, 'Processing all trials (i.e., stimuli)\n');
end

cond1Cnt = 1;
cond2Cnt = 1;
for cur_trial=min(trials):max(trials)

  if verbose && (mod(cur_trial+1, 50) == 0)
    fprintf(1, '%03d ', cur_trial+1);
    if (mod(cur_trial+1, 150) == 0)
       fprintf(1, '* /%d\r', max(trials));
    end
  end

  % condition 1
  eval(condition1idxstr);
  if (isempty(condition1idx) == 0) && ...
      length(signal) >= condition1idx + triallength

    if size(avgdata1, 3) < cond1Cnt
      avgdata1(:,:,end+1:end+1000) = ...
        zeros(triallength, num_channels, 1000);
    end
    avgdata1(:,:,cond1Cnt) = signal(...
      condition1idx:condition1idx+triallength-1, :);
    cond1Cnt = cond1Cnt + 1;
  end
  
  if length(analysisParams.targetConditions) == 2
    % condition 2
    eval(condition2idxstr);
    if (isempty(condition2idx) == 0) && ...
        length(signal) >= condition2idx + triallength
    
      if size(avgdata2, 3) < cond2Cnt
        avgdata2(:,:,end+1:end+1000) = ...
          zeros(triallength, num_channels, 1000);
      end
      avgdata2(:,:,cond2Cnt)=signal(...
        condition2idx:condition2idx+triallength-1, :);
      cond2Cnt = cond2Cnt + 1;
    end
  end
end

%in preallocation, it was possible we allocated a bit too much, truncate
%unused space
if size(avgdata1, 3) >= cond1Cnt
  avgdata1(:,:,cond1Cnt:end) = [];
end
if length(analysisParams.targetConditions) == 2
  if size(avgdata2, 3) >= cond2Cnt
    avgdata2(:,:,cond2Cnt:end) = [];
  end
end

%ensure we have enough data
if cond1Cnt < settings.minTrialsPerCondErr
  error([funcName ':minTrialsCond1NotMet'], sprintf('Evaluation of target condition 1 in conjunction with the trial change condition resulted in fewer than %d useable trials.  Please relax your conditions.', settings.minTrialsPerCondErr));
elseif ~errorOverride && cond1Cnt < settings.minTrialsPerCondWarn
  error([funcName ':sugTrialsCond1NotMet'], sprintf('Evaluation of target condition 1 in conjunction with the trial change condition resulted in fewer than %d useable trials.  You may want to relax your conditions.', settings.minTrialsPerCondWarn));
end
if length(analysisParams.targetConditions) == 2
  if cond2Cnt < settings.minTrialsPerCondErr
    error([funcName ':minTrialsCond2NotMet'], sprintf('Evaluation of target condition 2 in conjunction with the trial change condition resulted in fewer than %d useable trials.  Please relax your conditions.', settings.minTrialsPerCondErr));
  elseif ~errorOverride && cond2Cnt < settings.minTrialsPerCondWarn
    error([funcName ':sugTrialsCond2NotMet'], sprintf('Evaluation of target condition 2 in conjunction with the trial change condition resulted in fewer than %d useable trials.  You may want to relax your conditions.', settings.minTrialsPerCondWarn));
  end
end

if verbose
  fprintf(1, '\r');
  fprintf(1, 'Calculating statistics\n');
end

% calculate average trials for each condition and each channel
res1=mean(avgdata1, 3);

if length(analysisParams.targetConditions) == 2
  res2=mean(avgdata2, 3);

  % calculate rsqu for each channel and each sample between up and down target
  ressq = calc_rsqu(double(avgdata1), double(avgdata2), 1);
else
  res2 = [];
  ressq = [];
end


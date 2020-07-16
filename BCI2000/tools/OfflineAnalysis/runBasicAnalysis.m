%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% $Id: runBasicAnalysis.m 2007-111-26 12:31:37EST fialkoff $ 
%% 
%% File: runBasicAnalysis.m 
%% 
%% Author: Joshua Fialkoff <fialkj@rpi.edu>, Gerwin Schalk <schalk@wadsworth.org>
%%
%% Description: This function performs a time or frequency-based analysis
%% of multiple BCI2000 data files and then generates a number of plots
%% useful in determining the best feature set for the data.
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
function [handles] = runBasicAnalysis(params, settings, plots, errorOverride, verbose, handles)
  %%% plots input parameter (bit-wise combo. of following)
  % settings.pltSelFeatures - features
  % settings.pltSelSpectra - spectra
  % settings.pltSelTopos - topoos


  if strcmp(params.acqType, 'eeg')
    lp_cutoff = settings.maxFreqEEG;
  else
    lp_cutoff = settings.maxFreqECoG;
  end

  error(nargchk(2,6,nargin));
  if nargin < 5
    verbose = 0;
  end
  if nargin < 4
    errorOverride = 0;
  end

  funcName = 'runBasicAnalysis';

  %determine topo grid size if a topo was requested
  if bitand(plots, settings.pltSelTopos)
    switch length(params.topoParams)
      case 1
        topogrid = [1 1];
      case 2
        topogrid = [1 2];
      case {3 4}
        topogrid = [2 2];
      case {5 6}
        topogrid = [2 3];
      case {7 8 9}
        topogrid = [3 3];
      otherwise
        error([funcName ':maxTopoFreqsExceeded'], 'Maximum number of topographic plots exceeded');
    end
  end

  % Check structure of params
  if isValidAnalysisParams(params) ~= 0
    error([funcName ':invalidParams'], 'Invalid format of input parameter params');
  end

  % Make sure data files exist
  if ~isfield(params, 'signal')
    if isempty(params.dataFiles)
      error([funcName ':noDataFileSpecified'], 'params.dataFiles must contain at least one valid data file');
    else
      for idxFile = 1:length(params.dataFiles)
        if ~exist(params.dataFiles{idxFile}, 'file')
          error([funcName ':invalidDataFile'], ...
            sprintf('Specified data file %s does not exist', ...
            params.dataFiles(idxFile)));
        end
      end
    end
  end

  % Check that params.spatialFilt is one of the accepted values
  if ~strcmpi(params.spatialFilt, 'raw') && ...
      ~strcmpi(params.spatialFilt, 'car')
    error([funcName ':invalidSpatialFilt'], 'Specified spatial filter is not supported');
  end

  if bitand(plots, settings.pltSelTopos) 
    if isa(params.montageFile, 'char')
      %Montage file is specified as a file rather than an electrode struct
      % Make sure montage file exists
      if exist(params.montageFile, 'file')
        eloc_file = params.montageFile;
      else
        error([funcName ':invalidMontageFile'], 'params.montageFile can''t be found');
      end

      %Check that montage file is valid
      [isValid numChans] = isValidMontageFile(params.montageFile, settings, ...
        params.acqType);
      if ~isValid
        if strcmp(params.acqType, 'eeg')
          mode = 'EEG';
        else
          mode = 'ECoG';
        end
        error([funcName ':invalidMontageFile'], sprintf('Specifited montage file is not in the accepted format for %s processing', mode));
      end
    else
      %Montage is specified as an electrode struct
      numChans = length(params.montageFile);  
      eloc_file = params.montageFile;
    end

  end

  %Load data files
  if isfield(params, 'signal')
    signal = params.signal;
    states = params.states;
    bciParams = params.bciParams;
  else
    if verbose
      fprintf(1, 'Loading data file ...\n');
    end
    files = struct();
    [files(1:length(params.dataFiles)).name] = deal(params.dataFiles{:});
    try
      [signal, states, bciParams]=load_bcidat(files.name, '-calibrated');
      
      %load separate parameters
  %    bciParams = cell(length(files), 1);
  %     for idxFile = 1:length(files)
  %       [ignore1, ignore2, paramsTemp] = load_bcidat(files(idxFile).name, [0 0]);
  %       bciParams{idxFile} = paramsTemp;
  %     end
    catch
      rethrow(lasterror);
    end
  end

  %make sure that data has same number of channels as the montage file
  if bitand(plots, settings.pltSelTopos) && numChans ~= size(signal, 2)
    error([funcName ':invalidMontageFile'], ...
      'Specifited montage file must have the same number of channels as the data');
  end

  % isValidAnalysisParams confirms that params.channels and
  % params.topoParams are numeric vectors and have at least one entry.  
  % Are the channels specified within range?
  if bitand(plots, bitor(settings.pltSelSpectra, settings.pltSelTimeSeq))
    if any(params.channels < 1) || ...
        any(params.channels > size(signal, 2))
      error([funcName ':invalidSpecChans'], 'One or more of the specified channels are outside the range of available channels');
    else
      channels=params.channels;
    end
  end

  if isfield(bciParams.SamplingRate, 'NumericValue')
    samplefreq=bciParams.SamplingRate.NumericValue;
  else
    samplefreq = str2double(bciParams.SamplingRate.Value);
  end

  if lp_cutoff > samplefreq/2
    lp_cutoff = samplefreq/2;
    %if the last bin has less samples, truncate it
    lp_cutoff = lp_cutoff - mod(lp_cutoff-settings.hpCutoff, settings.freqBinWidth);
  end
  if bitand(plots, settings.pltSelTopos)
    if strcmp(params.domain, 'freq')
      if any(params.topoParams < settings.hpCutoff) || ...
          any(params.topoParams > lp_cutoff) 
        error([funcName ':invalidSpecChans'], sprintf('One or more of the specified topographic frequencies are outside the range of available frequencies.  Please choose frequencies between %d and %d.', settings.hpCutoff, lp_cutoff));
      else
        %translate frequencies into bins - handle issue of user requesting
        %the right edge of the final bin
        topoParams = params.topoParams;
        %deal with right edge frequncies
        topoParams(topoParams == lp_cutoff) = lp_cutoff-settings.freqBinWidth;
        %translate into bins
        topoParams = (topoParams+settings.freqBinWidth/2)/settings.freqBinWidth;
        topofrequencybins=ceil(topoParams);
      end
    elseif strcmp(params.domain, 'time')
      if any(params.topoParams < 0) || ...
          any(params.topoParams > settings.dataSegLength)
        error([funcName ':invalidSpecChans'], sprintf('One or more of the specified topographic times are outside the accepted range.  Please choose times between 0 and %dms.', ...
          settings.dataSegLength));
      else
        topotimebins = floor(params.topoParams/1000 * samplefreq)+1;
      end
    end
  end

  % Check for valid target conditions
  try
    targetIdxs1 = eval(sprintf('find(%s)', params.targetConditions{1}));
  catch
    error([funcName ':invalidTargetCondition1'], 'Syntax error in target condition 1');
  end
  condition1txt = params.conditionLabels{1};

  if length(params.targetConditions) == 2
    try
      targetIdxs2 = eval(sprintf('find(%s)', params.targetConditions{2}));
    catch
      error([funcName ':invalidTargetCondition2'], 'Syntax error in target condition 2');
    end
    condition2txt = params.conditionLabels{2};
  end

  % determine trial numbers
  if verbose
    fprintf(1, 'Determining trial numbers ...\n');
  end
  if( strcmpi( params.trialChangeCondition, 'auto' ) )
    targetIdx = [targetIdxs1; targetIdxs2];
    trialEnds = [0; find( diff( targetIdx ) ~= 1 ); length( targetIdx )];
    trialLengths = diff( trialEnds );
    goodTrials = find( trialLengths ./ mean( trialLengths ) >= 0.7 );
    badTrials = length( trialLengths ) - length( goodTrials );
    if( badTrials > 0 )
      uiwait(msgbox2( ...
        [num2str( badTrials ) ' trials were removed because they were too short.'], ...
        'Warning', ...
        'warnRemovedTrials', ...
        'modal' ));
    end
    trialnr = zeros(size(signal,1),1);
    for( i = 1:length(goodTrials) )
      trialnr( targetIdx(trialEnds(goodTrials(i))+1):targetIdx(trialEnds(goodTrials(i)+1)) ) = i;
    end
  else
    old_iti=-1;
    cur_trial=0;
    try
      trialdef=eval(params.trialChangeCondition);
    catch
      error([funcName ':invalidTrialChangeCond'], ...
        'Invalid trial change condition specified');
    end
    trialnr=zeros(size(trialdef));
    for cur_samp=1:length(trialdef)
     if ((trialdef(cur_samp) == 1) & (trialdef(cur_samp) ~= old_iti))
        cur_trial=cur_trial+1;
     end
     trialnr(cur_samp)=cur_trial;
     old_iti=trialdef(cur_samp);
    end
    if cur_trial < settings.minTrialsPerCondErr
      error([funcName ':minTrialsCond1NotMet'], sprintf('Evaluation of the trial change condition resulted in fewer than %d trials.  Please relax your conditions.', settings.minTrialsPerCondErr));
    end
  end

  if length(unique(trialnr(targetIdxs1))) < settings.minTrialsPerCondErr
    error([funcName ':minTrialsCond1NotMet'], sprintf('Evaluation of target condition 1 in conjunction with the trial change condition resulted in fewer than %d useable trials.  Please relax your conditions.', settings.minTrialsPerCondErr));
  elseif ~errorOverride && length(unique(trialnr(targetIdxs1))) < settings.minTrialsPerCondWarn
    error([funcName ':sugTrialsCond1NotMet'], sprintf('Evaluation of target condition 1 in conjunction with the trial change condition resulted in fewer than %d useable trials.  You may want to relax your conditions.', settings.minTrialsPerCondWarn));
  end

  if length(params.targetConditions) == 2
    if length(unique(trialnr(targetIdxs2))) < settings.minTrialsPerCondErr
      error([funcName ':minTrialsCond2NotMet'], sprintf('Evaluation of target condition 2 in conjunction with the trial change condition resulted in fewer than %d useable trials.  Please relax your conditions.', settings.minTrialsPerCondErr));
    elseif ~errorOverride && length(unique(trialnr(targetIdxs2))) < settings.minTrialsPerCondWarn
      error([funcName ':sugTrialsCond2NotMet'], sprintf('Evaluation of target condition 2 in conjunction with the trial change condition resulted in fewer than %d useable trials.  You may want to relax your conditions.', settings.minTrialsPerCondWarn));
    end
  end

  %remove signal DC
  for ch=1:size(signal, 2)
   signal(:, ch)=signal(:, ch)-mean(signal(:, ch));
  end

  % Perform CAR spatial filtering if requested
  if verbose
    fprintf(1, 'Spatial filtering\n');
  end
  if strcmpi(params.spatialFilt, 'car')
    signal = carFilt(signal, 2);
  end

  if nargin < 6
    %figure handles were not specified
    handles = struct('r2', [], 'chans', [], 'topos', []);
  else
    %check to make sure that the figure references are still valid
    hFigs = findobj('type', 'figure');
    if isfield(handles, 'r2')
      if ~isempty(handles.r2) && ~ismember(handles.r2, hFigs)
        handles.r2 = [];
      end
    else
      handles.r2 = [];
    end
    
    if isfield(handles, 'chans')
      if ~isempty(handles.chans) && ~ismember(handles.chans, hFigs)
        handles.chans = [];
      end
    else
      handles.chans = [];
    end
    
    if isfield(handles, 'topos')
      if ~isempty(handles.topos) && ~ismember(handles.topos, hFigs)
        handles.topos = [];
      end
    else
      handles.topos = [];
    end
  end
 
 verNum = sscanf(version, '%f');
 verNum = verNum(1); 
  
  if verNum < 8.4
    opengl neverselect;
  end
  
%   if bitand(plots, settings.pltSelFeatures) && isempty(handles.r2)
%       handles.r2 = figure;
%   end
%   if (bitand(plots, settings.pltSelSpectra) || ...
%       bitand(plots, settings.pltSelTimeSeq)) && isempty(handles.chans)
%     handles.chans = figure;
%   end
%   if bitand(plots, settings.pltSelTopos) && isempty(handles.topos)
%     handles.topos = figure;
%   end
  
  if strcmp(params.domain, 'freq')
    modelOrd = 18+round(samplefreq/100);
    
    parms = [modelOrd, settings.hpCutoff+settings.freqBinWidth/2, ...
      lp_cutoff-settings.freqBinWidth/2, settings.freqBinWidth, ...
      round(settings.freqBinWidth/.2), settings.trend, samplefreq];
%     parms=[settings.hpCutoff, lp_cutoff+settings.freqBinWidth, 0.2, settings.freqBinWidth, settings.trend, modelOrd, ...
%       samplefreq];  % EEG

    spectralSize = round(settings.spectralSize/1000 * samplefreq);
    spectralStep = round(settings.spectralStep/1000 * samplefreq);
    [res1, res2, ressq, freq_bins] = freqAnalysis( params, signal, ...
      trialnr,  bciParams, states, parms, spectralSize, ...
      spectralStep, verbose, errorOverride, ...
      settings.minTrialsPerCondWarn, settings.minTrialsPerCondErr);    
   
      if verbose
        fprintf(1, 'Plotting results ...\n');
      end       
      if bitand(plots, settings.pltSelSpectra)
        plotChans(freq_bins, 'Spectra %s', ...
          'r^2 %s', 'Spectra');
      end
      if bitand(plots, settings.pltSelFeatures)
        if length(params.targetConditions) == 1
          tmpTitle = 'Spectra Amplitude';
        else
          tmpTitle = 'r^2 Values Between Condtions 1 and 2';
        end
        
        plotFeatures(freq_bins, tmpTitle, 'Frequency (Hz)', 'Channel');
      end
      if bitand(plots, settings.pltSelTopos)
        titleData = mat2cell(reshape(freq_bins(topofrequencybins) + settings.freqBinWidth/2, ...
          length(topofrequencybins), 1), ...
          ones(length(topofrequencybins), 1), 1);
        titleData(:, 2) = mat2cell(reshape(params.topoParams, ...
          length(topofrequencybins), 1), ...
          ones(length(topofrequencybins), 1), 1);
        plotTopos(params.acqType, topofrequencybins, ...
          '%0.2f Hz (%0.2f Hz requested)', titleData);
      end
  else
    params.dataSegLength = settings.dataSegLength;
    [res1, res2, ressq, timems] = timeAnalysis(...
      params, signal, states, trialnr, bciParams, verbose, ...
      errorOverride, settings);     
    
    if verbose
      fprintf(1, 'Plotting results ...\n');
    end       
    if bitand(plots, settings.pltSelTimeSeq)
      plotChans(timems, 'Waveform (%s)', ...
        'Waveform r^2 (%s)', 'Waveforms');
    end
    if bitand(plots, settings.pltSelFeatures)
      if length(params.targetConditions) == 1
        tmpTitle = 'Waveform Amplitude';
      else
        tmpTitle = 'r^2 Values Between Condtions 1 and 2';
      end
      plotFeatures(timems, tmpTitle, 'Time (ms)', 'Channel');
      shading interp;
    end
    if bitand(plots, settings.pltSelTopos)
      %deal with potential issue of user specifying a time is at the very
      %end of the segment
      tmpIdx = find(topotimebins > length(timems));
      topotimebins(tmpIdx) = length(timems) * ones(1, length(tmpIdx));
      
      titleData = mat2cell(reshape(timems(topotimebins), ...
        length(topotimebins), 1), ...
        ones(length(topotimebins), 1), 1);
      titleData(:, 2) = mat2cell(reshape(params.topoParams, ...
        length(topotimebins), 1), ...
        ones(length(topotimebins), 1), 1);
      plotTopos(params.acqType, topotimebins, ...
        '%0.2f ms (%0.2f ms requested)', titleData);    
    end
  end



  
  function plotChans(xData, title1, title2, figTitle)
    channels = params.channels;
    channelNames = {};
    channelLabels = {};
    if( isstruct( params.montageFile ) )
      channelLabels = { params.montageFile.label };
    end
    for( i=1:length(channels) )
      if( channels(i) <= length( channelLabels ) )
        channelNames{i} = channelLabels{channels(i)};
      else
        channelNames{i} = [ 'Channel ' num2str( channels(i) ) ];
      end
    end
      
    if isempty(handles.chans)
      handles.chans = figure;
    else
      figure(handles.chans);
    end
    clf;
    set(handles.chans, 'name', figTitle);
    
    num_plotchannels=max(size(channels));
    for cur_plotchannel=1:num_plotchannels
      if length(params.targetConditions) == 2
        subplot(num_plotchannels, 2, (cur_plotchannel-1)*2+1);
        plot(xData, res1(:, channels(cur_plotchannel)), 'r');
        hold on;
        plot(xData, res2(:, channels(cur_plotchannel)), 'b');
        titletxt=sprintf(title1, channelNames{cur_plotchannel});
        title(titletxt); 
        axis tight;
        legend(condition1txt, condition2txt);

        subplot(num_plotchannels, 2, cur_plotchannel*2);
        plot(xData, ressq(:, channels(cur_plotchannel)), 'b');
        titletxt=sprintf(title2, channelNames{cur_plotchannel});
        title(titletxt); 
        minY = min(ressq(:, channels(cur_plotchannel)));
        maxY = max(ressq(:, channels(cur_plotchannel)));
        if minY == maxY
          minY = minY - .05
          maxY == minY + .05;
        end
        axis([min(xData), max(xData), minY, maxY]);
      else
        subplot(num_plotchannels, 1, cur_plotchannel);
        plot(xData, res1(:, channels(cur_plotchannel)), 'r');
        titletxt=sprintf(title1, channelNames{cur_plotchannel});
        title(titletxt); 
        axis tight;
      end
    end
  end

    function plotFeatures(xData, pltTitle, xLabel, yLabel)
      if isempty(handles.r2)
        handles.r2 = figure;
      else
        figure(handles.r2);
      end
      clf;
      set(handles.r2, 'name', 'Features');
      
      dispmin=min(min(ressq));
      dispmax=max(max(ressq));

      num_channels=size(signal, 2);

      if length(params.targetConditions) == 2
        data2plot=ressq';
      else
        data2plot = res1';
      end
      data2plot=cat(2, data2plot, zeros(size(data2plot, 1), 1));
      data2plot=cat(1, data2plot, zeros(1, size(data2plot, 2)));

      xData(end+1) = xData(end) + diff(xData(end-1:end));
      surf(xData, [1:num_channels + 1], data2plot);
      axis tight;
  %     xData(end) = [];
  %     surf(xData, [1:num_channels], ressq');
  % 
  %     pcolor(xData, [1:num_channels], ressq');

      view(2);
      colormap jet;
      colorbar;
      
      xlabel(xLabel);
      ylabel(yLabel);
      title(pltTitle);    

      if isstruct(params.montageFile)
        labels = { params.montageFile.label };
        for( i = length( labels ) + 1 : num_channels )
          labels{i} = num2str( i );
        end
        set( gca, 'ytick', [1:num_channels] + .5 );
        set( gca, 'yticklabel', labels );
      else
        tick = get( gca, 'ytick' );
        set( gca, 'ytick', tick + .5 );
        set( gca, 'yticklabel', num2str( tick' ) );
      end
    end

    function plotTopos(acqType, topoBins, pltTitle, titleData)
      if isempty(handles.topos)
        handles.topos = figure;
      else
        figure(handles.topos);
      end
      clf;
      set(handles.topos, 'name', 'Topographies');
      
      num_topos=length(params.topoParams);
      topoHandles = zeros(num_topos, 1);
      for cur_topo=1:num_topos
        pltIdx = cur_topo;
        hPlt = subplot(topogrid(1), topogrid(2), pltIdx);
        topoHandles(cur_topo) = hPlt;
        if length(params.targetConditions) == 2
          data2plot=ressq(topoBins(cur_topo), :);
        else
          data2plot = res1(topoBins(cur_topo), :);
        end
        
        options = { 'gridscale', 200 };
        if strcmpi( acqType, 'eeg' )
          options = { options{:}, 'electrodes', 'labels' };
        end
        topoplot(data2plot, eloc_file, acqType, 'maplimits', [min(min(data2plot)), max(max(data2plot))], options{:} );
        titletxt=sprintf(pltTitle, titleData{cur_topo, :});
        title(titletxt); 
        colormap jet;
        if cur_topo == 1
          topoPos = get(hPlt, 'position');
        end
        
        if(cur_topo == num_topos)
          hCb = colorbar;
          
          %compensate for matlab deforming the graph showing the colorbar
          topoPosLast = get(hPlt, 'position');
          topoPosLast(3) = topoPos(3);
          set(gca, 'position', topoPosLast);
        end      
      end
      lastRow = 0;
      
      cbPos = get(hCb, 'position');
      bufLen = cbPos(1) - topoPosLast(1);
      
      for cur_topo=1:num_topos
        pltIdx = cur_topo;
        hPlt = topoHandles(cur_topo);
        pos = get(hPlt, 'position');
        
        if cur_topo==1
          shiftAmt = pos(1)/2;
        end
        if mod(pltIdx-1, topogrid(2)) == 0
          rowY = pos(2);
        else
          pos(2) == rowY;
        end
        
        pos(1) = pos(1)-shiftAmt;
        set(hPlt, 'position', pos);
      end
    
    end

  end

function EEG = pop_loadBCI2000(fileName, events)
% pop_loadBCI2000() - loads BCI2000 .dat files into EEGLAB
%
% Usage:
%   >> EEG = pop_loadBCI2000();
%   >> EEG = pop_loadBCI2000(fileName);
%   >> EEG = pop_loadBCI2000(fileName, events);
%
% Inputs:
%   fileName - File name(s), either a string (possibly containing wildcards) or
%              a cell array of strings containing multiple file names
%   events   - List of events to import, can either be numeric (a vector) or a
%              cell array of strings containing the names of the events to
%              import (default: all events are imported)
%
% Outputs:
%   EEG      - EEGLAB data structure
%
% Examples:
%   Open GUI dialog:
%     >> EEG = pop_loadBCI2000();
%
%   Load all .dat files in the current folder with all events:
%     >> EEG = pop_loadBCI2000('*.dat');
%
%   Load all .dat files with event numbers 2 and 5:
%     >> EEG = pop_loadBCI2000('*.dat', [2, 5]);
%
%   Load all .dat files with events 'StimulusCode' and 'Feedback':
%     >> EEG = pop_loadBCI2000('*.dat', {'StimulusCode', 'Feedback'});
%
%   Load two specific files:
%     >> EEG = pop_loadBCI2000({'set001.dat', 'set002.dat'});

% Copyright by Clemens Brunner <clbrunner@ucsd.edu>
% Revision: 0.31
% Parts based on BCI2000import.m from the BCI2000 distribution (www.bci2000.org)

% Revision history:
%   0.35  Patched around old load_bcidat mexfiles for mac. Reintroduced
%         support for pre R2009b Matlab (null output ~ -> 'unused').
%         Chadwick.Boulay@gmail.com 2014-03-09
%   0.34  For incompatible input files, fails before event selection GUI.
%   0.33  Separated fileName logic and events logic.
%         GUI mode asks to select states quickly again, as in 0.31
%         Added a few states to ignore.
%         Restored ability to pass a numeric vector as 'events'
%         Chadwick.Boulay@gmail.com 2013-01-05
%   0.32  Added compatibility for cell array of filenames
%         Removed loops and other simplifications
%         Keeps any state change to non-zero value as an event
%         Keeps event durations
%         Chadwick.Boulay@gmail.com
%   0.31  Loads calibrated data
%   0.30: Create boundary events when loading multiple files
%   0.26: Import channel labels
%   0.25: Added EEG.urevent structure
%   0.20: Adapted to EEGLAB conventions
%   0.10: Initial version

% This program is free software; you can redistribute it and/or modify it under
% the terms of the GNU General Public License as published by the Free Software
% Foundation; either version 2 of the License, or (at your option) any later
% version.
%
% This program is distributed in the hope that it will be useful, but WITHOUT
% ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
% FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
%
% You should have received a copy of the GNU General Public License along with
% this program; if not, write to the Free Software Foundation, Inc., 59 Temple
% Place - Suite 330, Boston, MA  02111-1307, USA.

%% Process fileName(s)
if nargin < 1  % No input arguments specified, show GUI
    [fileName, filePath] = uigetfile('*.dat', 'Choose BCI2000 file(s) -- pop_loadBCI2000', 'multiselect', 'on');
    if isfloat(fileName)
        files = struct('name', {});
    elseif ischar(fileName) % Only one chosen
        files = struct('name', fullfile(filePath,fileName));
    else
        files = struct('name', strcat(repmat({filePath}, size(fileName)), fileName));
    end
else %input argument may be cell array of fileName(s) or a string
    if iscell(fileName)
        files = struct('name', fileName);
    else % Just a string (possibly containing wildcards)
        files = dir(fileName); %Convert this to a cell array of fileName(s)
        filePath = strcat(fileparts(fileName),filesep);
        files = struct('name', strcat(repmat({filePath}, size(files)), {files.name}));
    end
end
    
%% Process which events to store.
if isempty(files)
  error('no files given');
end
args = {};
for i =1:length(files)
  args = { args{:}, files(i).name, [0 0] };
end
[ unused, states, unused ] = load_bcidat( args{:} );%Load state names
stateNames = fieldnames(states);
if nargin<1 %GUI
    % Build a string consisting of all events, separated by a "|" (necessary for
    % inputgui() below)
    eventString = sprintf('%s|', stateNames{:});
    eventString(end) = [];
    %Show GUI to ask to select events.
    temp = inputgui('geometry', {1, 1}, 'geomvert', [1, 10], ...
        'uilist', {{'style', 'text', 'string', 'Select events'}, ...
        {'style', 'listbox', 'string', eventString, 'min', 0, 'max', 2}});
    if isempty(temp)  % Abort if user clicked "Cancel"
        error('Loading aborted.'); end
    
    events = stateNames(temp{1});
    clear('temp');
elseif nargin==1%No GUI, no events provided.
    events = stateNames; %Select all events.
elseif nargin==2 && ~iscell(events)
    if isnumeric(events)
        events = stateNames(events);
    elseif ischar(events)
        events = {events};
    end
end

%% Load the data
if ispc
    [ signal, states, parameters, unused, file_samples ] ...
            = load_bcidat(files.name, '-calibrated');
else
    file_samples = nan(length(files), 1);
    for fildIdx=1:length(files)
        [ signal, unused, unused ] ...
                = load_bcidat(files(fildIdx).name, '-calibrated');
        file_samples(fildIdx) = size(signal,1);
    end
    [ signal, states, parameters ] ...
                = load_bcidat(files.name, '-calibrated');
end

%% Prepare output
EEG = eeg_emptyset();
EEG.setname = 'Imported BCI2000 data set';
EEG.srate = parameters.SamplingRate.NumericValue;
EEG.nbchan = size(signal, 2);
EEG.pnts = size(signal, 1);
EEG.trials = 1;
EEG.data = signal';
if isfield(parameters, 'ChannelNames')
    EEG.chanlocs = struct('labels', parameters.ChannelNames.Value);% Assign channel labels
elseif isfield(parameters, 'TransmitChList')
    EEG.chanlocs = struct('labels', parameters.TransmitChList.Value);% Assign channel labels
end
EEG = eeg_checkset(EEG);

%% Remove states that were not selected (events).
[unused, ia, ib] = setxor(events, stateNames);
if ~isempty(ia)
    warning('''%s'' is not an event in the signal. Therefore, this event is being ignored.\n',...
        events{ia});
end
states = rmfield(states, stateNames(ib));

%% Convert states to compressed EEGLAB event format
states = compressData(states);
stateNames = fieldnames(states);
for k = 1:length(stateNames)
    these_events = struct(...
        'latency', num2cell(states.(stateNames{k}).latency),...
        'position', num2cell(states.(stateNames{k}).value),...
        'duration', num2cell(states.(stateNames{k}).duration));
    [these_events.type] = deal(stateNames{k});
    EEG.event = [EEG.event these_events'];
end

%% Create boundary events at the start of new files if >1 file
if length(files)>1
    fileBorders = [1 1+cumsum(double(file_samples(1:end-1)))];
    EEG.event = eeg_insertbound(EEG.event, EEG.pnts * EEG.trials, fileBorders');
end

%% Verify output
EEG = eeg_checkset(EEG, 'eventconsistency');
EEG = eeg_checkset(EEG, 'makeur'); %EEG.urevent
EEG = eeg_checkset(EEG);


function data = compressData(data)
% Compresses event data time series by noting only changes in event values.
%
% TODO: Detailed description

if isstruct(data)
    % Time stamps are not compressable, nor are they used by EEGLAB.
    discardStates = {'SourceTime','StimulusTime'};
    to_remove = intersect(fieldnames(data), discardStates);
    data = rmfield(data, to_remove);
    
    % List of known event types to compress
    compressStates = {'TargetCode','ResultCode','IntertrialInterval','Feedback','Dwelling','StimulusCode'};
    stateNames = fieldnames(data);
    for k = 1:length(stateNames)
        if ismember(stateNames{k}, compressStates)  % If the event is in the list of known events, compress it
            if ~isstruct(data.(stateNames{k}))
                data.(stateNames{k}) = compressData(data.(stateNames{k}));
            end
        else  % If the event is unknown, try to figure out if to compress it
            value = data.(stateNames{k});
            if isstruct(value)  % I don't know when this field can be a struct; obviously, it is ignored then
                continue; end
            % Specifies the percentage of number of different event values relative to the length of the EEG signals (e.g. 0.05 is 5%)
            relativeEventLength = 0.05;
            U = unique(value);
            
            % Compress only if there are at least 2 different event values AND
            % if the number of different event values does not exceed the above set threshold
            if length(U) > 1 && length(U) < length(value)*relativeEventLength
                data.(stateNames{k}) = compressData(value);
            else
                warning('''%s'' is not a valid event for EEGLAB. This event is being discarded.', stateNames{k});
                data = rmfield(data, stateNames{k});
            end
        end
    end

else
    
    data = double(data);
    ev_ch_ix = find([false;diff(data) ~= 0]); %Sample indices for any event change.
    dur = [diff(ev_ch_ix);length(data)-ev_ch_ix(end)]; %Duration between event changes.
    pos = data(ev_ch_ix); %New value at event change.
    
    %Presumably we don't want to store events when the event changes to 0.
    keep_event = pos~=0;
    
    clear data;
    data.latency = ev_ch_ix(keep_event);
    data.value = pos(keep_event);
    data.duration = dur(keep_event);
end

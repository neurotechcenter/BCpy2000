%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% $Id: OfflineAnalysis.m 2007-111-26 12:31:37EST fialkoff $ 
%% 
%% File: OfflineAnalysis.m 
%% 
%% Author: Joshua Fialkoff <fialkj@rpi.edu>, Gerwin Schalk <schalk@wadsworth.org>
%%
%% Description: This file defines a class of functions that control the
%% operation of the OfflineAnalysis GUI and perform data validation.
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
function varargout = OfflineAnalysis(varargin)

oadir = fileparts(which(mfilename));
olddir = cd;
mexdir = [oadir,'/../mex'];

if isdir(mexdir) && ~isdeployed,
    addpath(mexdir);
end
%cd(oadir)
%cd ../mex
%mexdir = cd; 
%cd(olddir)
%addpath(mexdir)

verNum = sscanf(version, '%d.%s');
verNum = verNum(1);


% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @OfflineAnalysis_OpeningFcn, ...
                   'gui_OutputFcn',  @OfflineAnalysis_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT

function paramsOut = fillParams(handles, bThourough)
% if bThourough is set to 1, function will confirm that specified files
% exist
settings = get(handles.figTop, 'userdata');
paramsOut = [];

if get(handles.radModeEEG, 'value') == 1
  acqMode = 'eeg';
else
  acqMode = 'ecog';
end
if get(handles.radDomainTime, 'value') == 1
  domain = 'time';
else
  domain = 'freq';
end

if bThourough
  fileInfo = get(handles.lstDataFiles, 'userdata');
  filePaths = {};
  if length(fileInfo) > 0
    [filePaths{1:length(fileInfo)}] = deal(fileInfo.path);
    params.fileInfo = rmfield(fileInfo, 'path');
  end
  params = getAnalysisParams(domain, acqMode, filePaths);
else
  params = getAnalysisParams(domain, acqMode);
end

params.montageFile = strtrim(get(handles.txtMontageFile, 'string'));
params.topoParams = strtrim(get(handles.txtTopoFreqs, 'string'));
params.targetConditions{1} = strtrim(get(handles.txtTargetCond1, 'string'));
params.conditionLabels{1} = strtrim(get(handles.txtTargetLabel1, 'string'));
tmpCond = strtrim(get(handles.txtTargetCond2, 'string'));
tmpTxt = strtrim(get(handles.txtTargetLabel2, 'string'));
if ~isempty(tmpCond) || ~isempty(tmpTxt)
  params.targetConditions{2} = tmpCond;
  params.conditionLabels{2} = tmpTxt;
end
params.trialChangeCondition = strtrim(get(handles.txtTrialChangeCond, 'string'));
params.channels = strtrim(get(handles.txtSpecChans, 'string'));

tmp = get(handles.radGrpSpatialFilt, 'SelectedObject');
params.spatialFilt = get(tmp, 'UserData');
  
if strcmp(params.acqType, 'eeg')
  maxFreq = settings.maxFreqEEG;
else
  maxFreq = settings.maxFreqECoG;
end

if bThourough
  for idxFile = 1:length(params.dataFiles)
    if ~exist(params.dataFiles{idxFile}, 'file')
      errordlg(sprintf('Specified data file %s does not exist', ...
        params.dataFiles{idxFile}), 'File not found');
      return;
    end
  end
end
if isempty(params.montageFile) 
  %check to see if channel names are available
  allFilesInfo = get(handles.lblDataFiles, 'userdata');
  if ~isempty(allFilesInfo) && ~isempty(allFilesInfo.elecs) && ...
      allFilesInfo.some1020
    %channel names are available and conform to 10-20 spec
    params.montageFile = allFilesInfo.elecs;
  else
    params.topoParams = '';
  end
end
if ~isValidStateCondition(params.targetConditions{1}, 'states')
  errordlg('The specified condition for target 1 is invalid', 'Invalid condition');
  return;
end
if isempty(params.conditionLabels{1})
  errordlg('Please specify a value for target label 1', 'Target label 1 not specified');
  return;
end
if length(params.targetConditions) == 2
  if ~isValidStateCondition(params.targetConditions{2}, 'states')
    errordlg('The specified condition for target 2 is invalid', 'Invalid condition');
    return;
  end
  if isempty(params.conditionLabels{2})
    errordlg('Please specify a value for target label 2', 'Target label 2 not specified');
    return;
  end
end
if ~strcmpi( params.trialChangeCondition, 'auto' ) && ~isValidStateCondition(params.trialChangeCondition, 'states')
  errordlg('The specified trial change condition is invalid', 'Invalid trial change condition');
  return;
end
if ~isempty(params.channels)
  if( isstruct( params.montageFile ) )
    chNames = { params.montageFile.label };
    chSpec = textscan( params.channels, '%s' );
    chSpec = chSpec{1};
    for( i = 1:length(chSpec) )
      idx = find( strcmpi( chNames, chSpec{i} ) );
      if( ~isempty( idx ) )
        chSpec{i} = num2str( idx(1) );
      end
    end
    params.channels = sprintf( '%s ', chSpec{:} );
  end
  
  bValid = 1;
  try
    params.channels = eval(['[' params.channels ']']);
  catch
    bValid = 0;
  end
  
  if bValid && (...
      ~isnumeric(params.channels) || ...
      any(params.channels ~= floor(params.channels)) || ...
      any(params.channels < 1)...
      )
    bValid = 0;
  end
  if ~bValid
    if get(handles.radDomainTime, 'value')
      errordlg('The specified waveform channel values are invalid', 'Invalid waveform channels');
    else
      errordlg('The specified spectrum channel values are invalid', 'Invalid spectrum channels');
    end
    return;
  end
end

if ~isempty(params.montageFile) && ~isempty(params.topoParams)
  %user has requested generation of topographies
  if( isstruct( params.montageFile ) && allFilesInfo.some1020 && ~allFilesInfo.is1020 )
    uiwait(msgbox2( [ ...
      'Some channels labels do not correspond to names from the 10-20 standard. ' ...
      'These channels will be omitted from topographies.' ], ...
      'Warning', ...
      'warnOmittedChannels', ...
      'modal' ));
  end
  bValid = 1;
  try
    params.topoParams = eval(['[' params.topoParams ']']);
  catch
    bValid = 0;
  end
  if bValid
    if get(handles.radDomainTime, 'value') 
      bValid = isnumeric(params.topoParams) && ...
        all(params.topoParams >= 0) && ...
        all(params.topoParams <= settings.dataSegLength);       
    else
      bValid = isnumeric(params.topoParams) && ...
        all(params.topoParams >= -1) && ...
        all(params.topoParams <= maxFreq);        
    end
  end

  if bValid
    if length(params.topoParams) > 9
      if get(handles.radDomainTime, 'value')
        errordlg(sprintf('Maximum number of topo times exceeded.  Please choose up to 9 times from 0 to %d ms.', settings.dataSegLength), 'Number of plots exceeded');
      else
        errordlg(sprintf('Maximum number of topo frequencies exceeded.  Please choose up to 9 frequencies from -1 to %d Hz.', maxFreq), 'Number of plots exceeded');
      end
      return;
    end
  else
    if get(handles.radDomainTime, 'value')
      errordlg(sprintf('The specified topo time values are invalid.  Please choose up to 9 times from 0 to %d ms.', settings.dataSegLength), 'Invalid Topo Frequencies');
    else
      errordlg(sprintf('The specified topo frequency values are invalid.  Please choose up to 9 frequencies from -1 to %d Hz.', maxFreq), 'Invalid Topo Frequencies');
    end
    return;
  end
end

%fileInfo = get(handles.lstDataFiles, 'userdata');
%numSamps = {};
%[numSamps{1:length(fileInfo)}] = deal(fileInfo.numSamps);
%params.dataFileSamps = numSamps;
%
paramsOut = params;
return;


function strout = strtrim(strin)
  strout = regexprep(strin, '^\s*(.*?)\s*$', '$1');
  
% --- Executes just before OfflineAnalysis is made visible.
function OfflineAnalysis_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to OfflineAnalysis (see VARARGIN)

% Choose default command line output for OfflineAnalysis
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes OfflineAnalysis wait for user response (see UIRESUME)
% uiwait(handles.figTop);
settings = reloadSettings(handles);


bgColor = [.925 .914 .847];
setBgColors(get(hObject, 'children'), bgColor);

pnlDomain_SelectionChangeFcn(handles.pnlDomain, [], handles);



function setBgColors(objects, bgColor)

for idxObj = 1:length(objects)
  if strcmp(get(objects(idxObj), 'type'), 'uicontrol') && ...
      (strcmp(get(objects(idxObj), 'style'), 'edit') || ...
      strcmp(get(objects(idxObj), 'style'), 'listbox'))
    set(objects(idxObj), 'backgroundcolor', [1 1 1]);
  elseif ~strcmp(get(objects(idxObj), 'type'), 'uimenu') && ~strcmp(get(objects(idxObj), 'tag'), 'lstDataFiles')
    set(objects(idxObj), 'backgroundcolor', bgColor);
    setBgColors(get(objects(idxObj), 'children'), bgColor);
  end
end

% --- Outputs from this function are returned to the command line.
function varargout = OfflineAnalysis_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;



function txtDatafile_Callback(hObject, eventdata, handles)
% hObject    handle to txtDatafile (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of txtDatafile as text
%        str2double(get(hObject,'String')) returns contents of txtDatafile as a double


% --- Executes during object creation, after setting all properties.
function txtDatafile_CreateFcn(hObject, eventdata, handles)
% hObject    handle to txtDatafile (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc
    set(hObject,'BackgroundColor','white');
else
    set(hObject,'BackgroundColor',get(0,'defaultUicontrolBackgroundColor'));
end


% --- Executes on button press in btnBrowseDataFile.
function btnBrowseDataFile_Callback(hObject, eventdata, handles)
% hObject    handle to btnBrowseDataFile (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)



function txtTargetCond1_Callback(hObject, eventdata, handles)
% hObject    handle to txtTargetCond1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of txtTargetCond1 as text
%        str2double(get(hObject,'String')) returns contents of txtTargetCond1 as a double


% --- Executes during object creation, after setting all properties.
function txtTargetCond1_CreateFcn(hObject, eventdata, handles)
% hObject    handle to txtTargetCond1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc
    set(hObject,'BackgroundColor','white');
else
    set(hObject,'BackgroundColor',get(0,'defaultUicontrolBackgroundColor'));
end



function txtTargetCond2_Callback(hObject, eventdata, handles)
% hObject    handle to txtTargetCond2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of txtTargetCond2 as text
%        str2double(get(hObject,'String')) returns contents of txtTargetCond2 as a double


% --- Executes during object creation, after setting all properties.
function txtTargetCond2_CreateFcn(hObject, eventdata, handles)
% hObject    handle to txtTargetCond2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc
    set(hObject,'BackgroundColor','white');
else
    set(hObject,'BackgroundColor',get(0,'defaultUicontrolBackgroundColor'));
end



function txtTrialChangeCond_Callback(hObject, eventdata, handles)
% hObject    handle to txtTrialChangeCond (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of txtTrialChangeCond as text
%        str2double(get(hObject,'String')) returns contents of txtTrialChangeCond as a double


% --- Executes during object creation, after setting all properties.
function txtTrialChangeCond_CreateFcn(hObject, eventdata, handles)
% hObject    handle to txtTrialChangeCond (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc
    set(hObject,'BackgroundColor','white');
else
    set(hObject,'BackgroundColor',get(0,'defaultUicontrolBackgroundColor'));
end



function txtSpecChans_Callback(hObject, eventdata, handles)
% hObject    handle to txtSpecChans (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of txtSpecChans as text
%        str2double(get(hObject,'String')) returns contents of txtSpecChans as a double


% --- Executes during object creation, after setting all properties.
function txtSpecChans_CreateFcn(hObject, eventdata, handles)
% hObject    handle to txtSpecChans (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc
    set(hObject,'BackgroundColor','white');
else
    set(hObject,'BackgroundColor',get(0,'defaultUicontrolBackgroundColor'));
end



function txtTopoFreqs_Callback(hObject, eventdata, handles)
% hObject    handle to txtTopoFreqs (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of txtTopoFreqs as text
%        str2double(get(hObject,'String')) returns contents of txtTopoFreqs as a double


% --- Executes during object creation, after setting all properties.
function txtTopoFreqs_CreateFcn(hObject, eventdata, handles)
% hObject    handle to txtTopoFreqs (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc
    set(hObject,'BackgroundColor','white');
else
    set(hObject,'BackgroundColor',get(0,'defaultUicontrolBackgroundColor'));
end

% --- Executes during object creation, after setting all properties.
function txtMontageFile_CreateFcn(hObject, eventdata, handles)
% hObject    handle to txtMontageFile (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc
    set(hObject,'BackgroundColor','white');
else
    set(hObject,'BackgroundColor',get(0,'defaultUicontrolBackgroundColor'));
end


% --- Executes on button press in btnBrowseMontageFile.
function btnBrowseMontageFile_Callback(hObject, eventdata, handles)
% hObject    handle to btnBrowseMontageFile (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
settings = get(handles.figTop, 'userdata');
if isfield(settings, 'montageFileDir') && ~isempty(settings.montageFileDir)
  [fn path] = uigetfile('*.*', 'Choose a montage file', ...
    settings.montageFileDir);
else
  [fn path] = uigetfile('*.*', 'Choose a montage file');
end

if fn ~= 0
  set(handles.txtMontageFile, 'string', [path fn]);
  settings.montageFileDir = fullfile(path, '.', filesep);
  saveSettings(settings, handles);
  set(handles.lblUsingChanNames, 'visible', 'off');
end


function txtDataFile_Callback(hObject, eventdata, handles)
% hObject    handle to txtDataFile (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of txtDataFile as text
%        str2double(get(hObject,'String')) returns contents of txtDataFile as a double


% --- Executes during object creation, after setting all properties.
function txtDataFile_CreateFcn(hObject, eventdata, handles)
% hObject    handle to txtDataFile (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc
    set(hObject,'BackgroundColor','white');
else
    set(hObject,'BackgroundColor',get(0,'defaultUicontrolBackgroundColor'));
end


% --- Executes on button press in btnGenPlots.
function btnGenPlots_Callback(hObject, eventdata, handles)
% hObject    handle to btnGenPlots (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
settings = get(handles.figTop, 'userdata');

bDebug = 0;
bSuccess = 0;
plots2Gen = 0;


params = fillParams(handles, 1);


if ~isempty(params)
  if isempty(params.dataFiles)
    errordlg('Please specify at least one data file for analysis.', ...
      'No Data File Specified');
    return;
  end  
  
  plots2Gen = settings.pltSelFeatures;
  if ~isempty(params.montageFile) && ~isempty(params.topoParams)
    plots2Gen = plots2Gen + settings.pltSelTopos;
  end
  if ~isempty(params.channels)
    if strcmp(params.domain, 'time')
      plots2Gen = plots2Gen + settings.pltSelTimeSeq;
    else
      plots2Gen = plots2Gen + settings.pltSelSpectra;
    end
  end

  try

    hWait = waitdlg('Please wait for the analysis to be complete.', 'Please wait');
    %assignin('base', 'params', params);
    %assignin('base', 'settings', settings);
    %assignin('base', 'plots2Gen', plots2Gen);
    
    if get(handles.chkOverwritePlots, 'value') == 1
      figHandles = get(handles.chkOverwritePlots, 'userdata');
      figHandles = runBasicAnalysis(params, settings, plots2Gen, get(handles.chkIgnoreWarnings, 'value'), 0, figHandles);
    else
      figHandles = runBasicAnalysis(params, settings, plots2Gen, get(handles.chkIgnoreWarnings, 'value'));
    end
    bSuccess = true;

  catch
    [msg msgId] = lasterr;
    msg = regexprep(msg, '.*?\n(.*)', '$1');

    try
      %in case user closed the dialog
      delete(hWait);
    catch
    end


    switch(regexprep(msgId, '.*:(.*)$', '$1'))
      case {'sugTrialsCond1NotMet', 'sugTrialsCond2NotMet'}
        warndlg([msg '  If you wish to continue anyways, please check the box labeled ''Ignore Warnings''.'], 'Warning');
      otherwise
        errordlg(msg, 'Warning');
    end
    return;
  end
  try
    %in case user closed the dialog
    delete(hWait);
  catch
  end

  if strcmpi(get(handles.chkOverwritePlots, 'enable'), 'off')
    set(handles.chkOverwritePlots, 'enable', 'on');
    set(handles.chkOverwritePlots, 'value', 1);
  end
  set(handles.chkOverwritePlots, 'userdata', figHandles);

  topFigPos = get(handles.figTop, 'position');
end

function txtTargetLabel1_Callback(hObject, eventdata, handles)
% hObject    handle to txtTargetLabel1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of txtTargetLabel1 as text
%        str2double(get(hObject,'String')) returns contents of txtTargetLabel1 as a double


% --- Executes during object creation, after setting all properties.
function txtTargetLabel1_CreateFcn(hObject, eventdata, handles)
% hObject    handle to txtTargetLabel1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc
    set(hObject,'BackgroundColor','white');
else
    set(hObject,'BackgroundColor',get(0,'defaultUicontrolBackgroundColor'));
end



function txtTargetLabel2_Callback(hObject, eventdata, handles)
% hObject    handle to txtTargetLabel2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of txtTargetLabel2 as text
%        str2double(get(hObject,'String')) returns contents of txtTargetLabel2 as a double


% --- Executes during object creation, after setting all properties.
function txtTargetLabel2_CreateFcn(hObject, eventdata, handles)
% hObject    handle to txtTargetLabel2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc
    set(hObject,'BackgroundColor','white');
else
    set(hObject,'BackgroundColor',get(0,'defaultUicontrolBackgroundColor'));
end




% --- Executes on button press in chkIgnoreWarnings.
function chkIgnoreWarnings_Callback(hObject, eventdata, handles)
% hObject    handle to chkIgnoreWarnings (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of chkIgnoreWarnings




% --- Executes on button press in chkOverwritePLots.
function chkOverwritePLots_Callback(hObject, eventdata, handles)
% hObject    handle to chkOverwritePLots (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of chkOverwritePLots


% --- Executes on button press in chkOverwritePlots.
function chkOverwritePlots_Callback(hObject, eventdata, handles)
% hObject    handle to chkOverwritePlots (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of chkOverwritePlots




% --- Executes on button press in btnRestore.
function btnRestore_Callback(hObject, eventdata, handles)
% hObject    handle to btnRestore (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(handles.pnlRmt, 'visible', 'off');
procPos = get(handles.pnlProc, 'position');
figPos = get(handles.figTop, 'position');
screenDims = get( 0, 'ScreenSize' );
figPos(3) = procPos(3);
if procPos(1)+procPos(3) > screenDims(3)
  proPos(1) = sreenDims(3) - procPos(3);
end
  

% --- Executes on button press in togglebutton6.
function togglebutton6_Callback(hObject, eventdata, handles)
% hObject    handle to togglebutton6 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of togglebutton6




% --------------------------------------------------------------------
function btnGrpRmt_SelectionChangeFcn(hObject, eventdata, handles)
% hObject    handle to btnGrpRmt (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

tmp = get(hObject, 'SelectedObject');
figure(get(tmp, 'UserData'));


% --- Executes during object creation, after setting all properties.
function figTop_CreateFcn(hObject, eventdata, handles)
% hObject    handle to figTop (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called
% figPos = get(hObject, 'position');
% figPos(3) = 108.2;
% figPos(4) = 32.3
% set(hObject, 'position', figPos);



% --------------------------------------------------------------------
function mnuModeEEG_Callback(hObject, eventdata, handles)
% hObject    handle to mnuModeEEG (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(hObject, 'checked', 'on');
set(handles.mnuModeECoG, 'checked', 'off');

% --------------------------------------------------------------------
function mnuModeECoG_Callback(hObject, eventdata, handles)
% hObject    handle to mnuModeECoG (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(hObject, 'checked', 'on');
set(handles.mnuModeEEG, 'checked', 'off');

% --------------------------------------------------------------------
function mnuMode_Callback(hObject, eventdata, handles)
% hObject    handle to mnuMode (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --- Executes on selection change in lstDataFiles.
function lstDataFiles_Callback(hObject, eventdata, handles)
% hObject    handle to lstDataFiles (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = get(hObject,'String') returns lstDataFiles contents as cell array
%        contents{get(hObject,'Value')} returns selected item from lstDataFiles
if isempty(get(hObject, 'value'))
  set(hObject, 'value', 1);
end

% --- Executes during object creation, after setting all properties.
function lstDataFiles_CreateFcn(hObject, eventdata, handles)
% hObject    handle to lstDataFiles (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: listbox controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc
    set(hObject,'BackgroundColor','white');
else
    set(hObject,'BackgroundColor',get(0,'defaultUicontrolBackgroundColor'));
end


% --- Executes on button press in btnAdd.
function btnAdd_Callback(hObject, eventdata, handles)
% hObject    handle to btnAdd (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
settings = get(handles.figTop, 'userdata');

if isfield(settings, 'dataFileDir') && ~isempty(settings.dataFileDir)
  [fn path] = uigetfile('*.dat', 'Choose a BCI2000 data file', ...
    settings.dataFileDir, 'multiselect', 'on');
else
  [fn path] = uigetfile('*.dat', 'Choose a BCI2000 data file', ...
    'multiselect', 'on');
end

if ~isnumeric(fn)
  if ~iscell(fn)
    fn = {fn};
  end
  
  %save last path as the default
  settings.dataFileDir = fullfile(path, '.', filesep);
  saveSettings(settings, handles);
  
  curFiles = get(handles.lstDataFiles, 'string');
  curFileInfo = get(handles.lstDataFiles, 'userdata');  
  curFilePaths = cell(length(curFileInfo), 1);
	if isempty(curFileInfo)
		curFileInfo = [];
	else
		[curFilePaths{1:end}] = deal(curFileInfo.path);
	end

	issueErr = 1;

	for idxFile = 1:length(fn)
    fp = [path fn{idxFile}];
    
    %make sure this file isn't already inserted
    if isempty(find(ismember(curFilePaths, fp)==1)) %get param, state and num samples info
      [tmpSig, tmpStates, tmpParams, tmpNumSamps] = ...
        load_bcidat(fp, [1 1]);
			
			%get the channel names for the data files that have been added so far if the names
			%exist
			allFilesInfo = get(handles.lblDataFiles, 'userdata');

			[chkRes allFilesInfo] = checkFile(allFilesInfo, tmpSig, tmpStates, tmpParams, settings);

      if ~isfield(allFilesInfo, 'warningsIssued')
        allFilesInfo.warningsIssued = [];
      end

			if chkRes <= 2 
        if allFilesInfo.some1020
          switch chkRes
          case 1
            if (~isfield(settings, 'warnEmpty1020CnFollowing') || ...
                settings.warnEmpty1020CnFollowing) && ~ismember(chkRes, allFilesInfo.warningsIssued)
              allFilesInfo.warningsIssued = [allFilesInfo.warningsIssued 1 2]; 
              uiwait(msgbox2(['The file "' fn{idxFile} '", does not have embedded channel name ' ...
                'data.  Because it has the same number of channels as previously selected files, however, ' ...
                'Offline Analysis will assume that the channel names corresponding to the data in this file ' ...
                'are the same as the 10-20 compliant channel names specified in the other files. ' ...
                'To indicate that there is a discrepancy, this file, and any others without channel ' ...
                'name data that may be added later, will be marked with an asterisk.'], ...
                'Channel Names', 'warnEmpty1020CnFollowing', 'modal'));
              settings = reloadSettings(handles);
            end

            curFiles{end+1} = ['*' fn{idxFile}];
          case 2
            if (~isfield(settings, 'warnEmpty1020CnPreceeding') || ...
                settings.warnEmpty1020CnPreceeding) && ~ismember(chkRes, allFilesInfo.warningsIssued)
              allFilesInfo.warningsIssued = [allFilesInfo.warningsIssued 1 2]; 
              uiwait(msgbox2(['Offline Analysis has detected that the file "' fn{idxFile} '" ' ...
                'contains embedded channel name data that is compliant with the 10-20 standard. ' ...
                'This data can be used, in place of a montage file, to generate topographies. ' ...
                'While previously selected files do not contain any channel name data, ' ...
                'Offline Analysis will assume that the channel names for these data are the ' ...
                'same as those specified in "' fn{idxFile} '".  To indicate that there is a ' ...
                'discrepancy, any file, including files that may be added later, ' ...
                'will be marked with an asterisk.'], ...
                'Channel Names', 'warnEmpty1020CnPreceeding', 'modal'));
              settings = reloadSettings(handles);
            end

            %mark all other files with an astrix
            for idxListedFile = 1:length(curFiles)
              curFiles{idxListedFile} = ['*' curFiles{idxListedFile}];
            end
            curFiles{end+1} = fn{idxFile};
          case 0
            if (~isfield(settings, 'warnUsingChannelNames') || ...
                settings.warnUsingChannelNames) 
              numFiles = length(get(handles.lstDataFiles, 'string'));
              if numFiles == 0
                uiwait(msgbox2(['Offline Analysis has detected that the file "' fn{idxFile} '" ' ...
                  'has embedded channel name data that is compliant with the 10-20 standard. ' ...
                  'BCI2000 Offline Analysis will use these names, in place of a montage file, to ' ...
                  'generate topographies. To override, simply specify a montage file and ' ...
                  'proceed as usual.'], 'Channel Names', 'warnUsingChannelNames', 'modal'));
                settings = reloadSettings(handles);
              end
            end

            curFiles{end+1} = fn{idxFile};
          end

          %update the montage file field if user hasn't already selected a montage file
          montageFile = strtrim(get(handles.txtMontageFile, 'string'));
          if length(montageFile) == 0
            set(handles.lblUsingChanNames, 'visible', 'on');
          end

        else
          %not 1020 issue warnings only when there's a mix of files without CN data and files
          %with
          switch chkRes
          case 1
            if (~isfield(settings, 'warnEmptyCnFollowing') || ...
                settings.warnEmptyCnFollowing) && ~ismember(chkRes, allFilesInfo.warningsIssued)
              allFilesInfo.warningsIssued = [allFilesInfo.warningsIssued 1 2]; 
              uiwait(msgbox2(['BCI2000 allows for channel name data to be embedded within datafiles. ' ...
                'To maintain consistency, Offline Analysis inspects added files to make ' ...
                'sure that the channel names are the same.  While this is the case for all of the ' ...
                'currently listed files, the file "' fn{idxFile} '" differs in that it does not ' ...
                'contain any channel name data.' ...
                'Please confirm that this file is intended to be grouped with the others listed. ' ...
                'To indicate that there is a discrepancy, this file, and any others without channel ' ...
                'name data that may be added later, will be marked with an asterisk.'], ...
                'Channel Names', 'warnEmptyCnFollowing', 'modal'));
              settings = reloadSettings(handles);
            end

            curFiles{end+1} = ['*' fn{idxFile}];
          case 2
            if (~isfield(settings, 'warnEmptyCnPreceeding') || ...
                settings.warnEmptyCnPreceeding) && ~ismember(chkRes, allFilesInfo.warningsIssued)
              allFilesInfo.warningsIssued = [allFilesInfo.warningsIssued 1 2]; 
              uiwait(msgbox2(['BCI2000 allows for channel name data to be embedded within datafiles. ' ...
                'To maintain consistency, Offline Analysis inspects added files to make ' ...
                'sure that the channel names are the same.  The files added thus far do ' ...
                'not contain channel name data.  The file "' fn{idxFile} '", however, does. ' ...
                'Please confirm that this file is intended to be grouped with the others listed. ' ...
                'To indicate that there is a discrepancy, files without channel name data, including ' ...
                'those that may be added later, will be marked with an asterisk.'], ...
                'Channel Names', 'warnEmptyCnPreceeding', 'modal'));
              settings = reloadSettings(handles);
            end

            %mark all other files with an astrix
            for idxListedFile = 1:length(curFiles)
              curFiles{idxListedFile} = ['*' curFiles{idxListedFile}];
            end
            curFiles{end+1} = fn{idxFile};
          case 0
            curFiles{end+1} = fn{idxFile};
          end
				end

        %save the channel label data to the global workspace
        set(handles.lblDataFiles, 'userdata', allFilesInfo);

				%update the file list 
				set(handles.lstDataFiles, 'string', curFiles);

				curFileInfo(end+1).path = fp;
				curFileInfo(end).numSamps = tmpNumSamps;
				curFileInfo(end).states = tmpStates;
				curFileInfo(end).params = tmpParams;

				set(handles.lstDataFiles, 'userdata', curFileInfo);  

			elseif chkRes == 3 
				errText = ['Channel name data in "' fn{idxFile} '" does not agree with ' ...
					'previously added files.  Please ' ...
					'review the selected datafiles to ensure the correct files have been chosen.'];
				uiwait(warndlg(errText, 'Channel Names', 'modal'));
			elseif chkRes == 4
				errText = ['The number of channels in "' fn{idxFile} '" does not agree with ' ...
				'previously added files.  Please review the selected datafiles to ensure ' ...
				'the correct files have been chosen.'];
				uiwait(warndlg(errText, 'Number of Channels', 'modal'));
			end
    end
  end

  set(handles.lstDataFiles, 'userdata', curFileInfo);  
  
  set(handles.btnRemove, 'enable', 'on');
  
  adaptConditions( handles );
end

function adaptConditions( handles )
% Toggle between references to TargetCode and StimulusCode, depending on
% which is present in data files.
fileInfo = get(handles.lstDataFiles, 'userdata');
if ~isempty(fileInfo)
  states = fileInfo.states;
  cond = { ...
    get(handles.txtTargetCond1, 'string'), ...
    get(handles.txtTargetCond2, 'string'), ...
    get(handles.txtTrialChangeCond, 'string'), ...
  };
  if ~isfield( states, 'StimulusCode' ) && isfield( states, 'TargetCode' )
    cond = regexprep( cond, 'states\.StimulusCode', 'states.\TargetCode' );
  elseif ~isfield( states, 'TargetCode' ) && isfield( states, 'StimulusCode' )
    cond = regexprep( cond, 'states\.TargetCode', 'states.\StimulusCode' );
  end
  set(handles.txtTargetCond1, 'string', cond{1});
  set(handles.txtTargetCond2, 'string', cond{2});
  set(handles.txtTrialChangeCond, 'string', cond{3});
end

function [res, allFilesInfo, fileTypesChange] = checkFile(allFilesInfo, signal, states, params, settings)
% This function will check to see if the ChannelNames parameter is the 
% same for all files and that every channel name corresponds to a standard
% 10-20 name and that the names aren't duplicted.

res = 0;
%0 - everything okay - nothing to comment about
%1 - some files have channel name data, the new does not
%2 - previous files do not have channel name data, the new one does
%3 - some previous files have channel name data that conflicts with the new one
%4 - new file has a different number of chanels

%first make sure that the new file has the right number of channels
numElecs = size(signal, 2); 
if ~isempty(allFilesInfo) && numElecs ~= allFilesInfo.numElecs
	res = 4;
	return;
end

%Do the previously included files have channel data?
%If yes, check if the new file has the same names as existing files or doesn't have channel
%	name data at all
%If no and the file has channel names, make sure they correspond to 10-20 names

chanNamesSet = ~isempty(allFilesInfo) && ~isempty(allFilesInfo.elecs);
all1020 = 0;
some1020 = false;
elecsInFile = [];

if chanNamesSet
	if ~isempty(params.ChannelNames.Value)
		%new file has channel names too, we need to make sure they're the same
		chanNames = params.ChannelNames.Value;

		res = 0;

		if length(chanNames) ~= length(allFilesInfo.elecs)
			res = 3;
		else
			%check to make sure all the channels are labeled the same
			for idxCn = 1:length(chanNames)

				if ~strcmpi(chanNames{idxCn}, allFilesInfo.elecs(idxCn).label)
					res = 3;
					break;
				end
			end
		end
	else
		%new file does not have channel names, old ones do
		res = 1;
	end
else
	%no channel names set yet
	if ~isfield(params, 'ChannelNames') || isempty(params.ChannelNames.Value)
		%nothing to set here
		res = 0;
	else
		%set new chan names, are they 10-20?
		elecsInFile = struct('elecNum', [], 'label', '', 'coords', []);
		%initialize with proper size
		chanNames = params.ChannelNames.Value; 
		elecsInFile(length(chanNames)).label = '';

    if length(chanNames) ~= numElecs
      all1020 = 0
    else
      all1020 = 1;
    end

		for idxCn = 1:length(chanNames)
			bFoundElec = 0;
			elecsInFile(idxCn).label = strtrim(chanNames{idxCn});

      %determine if there's a 10-20 label that matches this label
      for idxElec = 1:length(settings.elecs1020)
        if strcmpi(settings.elecs1020(idxElec).label, elecsInFile(idxCn).label)
          elecsInFile(idxCn) = settings.elecs1020(idxElec);
          settings.elecs1020(idxElec) = [];
          bFoundElec = 1;
          some1020 = true;
          break;
        end
      end
			
			if all1020 && ~bFoundElec
				%we still need to create the label name template for the files
				%continue adding labels, but don't worry about checking for 10-20 match
				all1020 = 0;
			end
		end

		if isempty(allFilesInfo)
			%this is the first file being added
			res = 0;
		else
			res = 2;
		end
	end
	
	%for later reference, we'll save the label list here:
	allFilesInfo.is1020 = all1020;
  allFilesInfo.some1020 = some1020;
	allFilesInfo.elecs = elecsInFile;
	allFilesInfo.numElecs = numElecs;

end




% --- Executes on button press in btnRemove.
function btnRemove_Callback(hObject, eventdata, handles)
% hObject    handle to btnRemove (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
curFiles = get(handles.lstDataFiles, 'string');

if length(curFiles) == 1
	%this is the last file, just clear
	clearFiles(handles);
else
	curFileInfo = get(handles.lstDataFiles, 'userdata');
	selectedIdx = get(handles.lstDataFiles, 'value');

	curFiles(selectedIdx) = [];
	curFileInfo(selectedIdx) = [];

	set(handles.lstDataFiles, 'string', curFiles);
	set(handles.lstDataFiles, 'userdata', curFileInfo);

	if selectedIdx > length(curFiles) && selectedIdx > 1
		set(handles.lstDataFiles, 'value', selectedIdx - 1);
	end

	%are the channel names set?
	allFilesInfo = get(handles.lblDataFiles, 'userdata');
	chanNamesSet = ~isempty(allFilesInfo) && ~isempty(allFilesInfo.elecs);
	if chanNamesSet
		%check to see if any of the files still present have channel name data
		%and take appropriate action if not
		chanNamesSet = 0;
		for idxFile = 1:length(curFileInfo)
			if ~isempty(curFileInfo(idxFile).params.ChannelNames.Value)
				chanNamesSet = 1;
				break;
			end
		end

		if ~chanNamesSet
			allFilesInfo.elecs = [];
			allFilesInfo.is1020 = 0;
      allFilesInfo.some1020 = false;
      allFilesInfo.warningsIssued = [];
			set(handles.lblDataFiles, 'userdata', allFilesInfo);

      %also remaining files, without channel data, should have an astrix next to their name
      %get rid of it
      for idxFile = 1:length(curFiles)
        curFiles{idxFile}(1) = [];
      end
      set(handles.lstDataFiles, 'string', curFiles);
      set(handles.lblUsingChanNames, 'visible', 'off');
		end
	end
end

% --- Executes on button press in btnClear.
function btnClear_Callback(hObject, eventdata, handles)
% hObject    handle to btnClear (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
clearFiles(handles);

function clearFiles(handles)
set(handles.lstDataFiles, 'value', 1);
set(handles.lstDataFiles, 'string', {});
set(handles.lstDataFiles, 'userdata', {});
set(handles.lblDataFiles, 'userdata', {});
set(handles.btnRemove, 'enable', 'off');
set(handles.lblUsingChanNames, 'visible', 'off');


% --- Executes on button press in togglebutton7.
function togglebutton7_Callback(hObject, eventdata, handles)
% hObject    handle to togglebutton7 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of togglebutton7




% --- Executes when user attempts to close figTop.
function figTop_CloseRequestFcn(hObject, eventdata, handles)
% hObject    handle to figTop (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: delete(hObject) closes the figure
delete(hObject);




% --- Executes on button press in chkFeatures.
function chkFeatures_Callback(hObject, eventdata, handles)
% hObject    handle to chkFeatures (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of chkFeatures



% --- Executes during object creation, after setting all properties.
function btnBrowseMontageFile_CreateFcn(hObject, eventdata, handles)
% hObject    handle to btnBrowseMontageFile (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called




% --------------------------------------------------------------------
function uipanel12_SelectionChangeFcn(hObject, eventdata, handles)
% hObject    handle to uipanel12 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --------------------------------------------------------------------
function pnlDomain_SelectionChangeFcn(hObject, eventdata, handles)
% hObject    handle to pnlMode (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
settings = get(handles.figTop, 'userdata');

if get(handles.radDomainFreq, 'value')
  set(handles.lblChannels, 'string', 'Spectra Channels');
  set(handles.lblTopoParams, 'string', 'Topo Frequencies (Hz)');
  set(handles.txtTargetCond1, 'string', settings.freqCondition1);
  set(handles.txtTargetCond2, 'string', settings.freqCondition2);
  set(handles.txtTargetLabel1, 'string', settings.freqConditionLabel1);
  set(handles.txtTargetLabel2, 'string', settings.freqConditionLabel2);
  set(handles.txtTrialChangeCond, 'string', settings.freqTrialChangeCond);
  set(handles.radSpatialFiltCAR, 'value', 1);
  set(handles.radSpatialFiltNone, 'value', 0);  
else
  set(handles.lblChannels, 'string', 'Waveform Channels');
  set(handles.lblTopoParams, 'string', 'Topo Times (ms)');
  set(handles.txtTargetCond1, 'string', settings.p300Condition1);
  set(handles.txtTargetCond2, 'string', settings.p300Condition2);
  set(handles.txtTargetLabel1, 'string', settings.p300ConditionLabel1);
  set(handles.txtTargetLabel2, 'string', settings.p300ConditionLabel2);
  set(handles.txtTrialChangeCond, 'string', settings.p300TrialChangeCond);
  set(handles.radSpatialFiltCAR, 'value', 0);
  set(handles.radSpatialFiltNone, 'value', 1);  
end
adaptConditions( handles );

% --------------------------------------------------------------------
function mnuSaveSettings_Callback(hObject, eventdata, handles)
% hObject    handle to mnuSaveSettings (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
params = fillParams(handles, 0);

if ~isempty(params)

  %make sure that if there's montage file that it is a file name and not an electrode struct
  if isa(params.montageFile, 'struct')
    params.montageFile = [];
  end

  settings = get(handles.figTop, 'userdata');
  if isfield(settings, 'wsFileDir') && ~isempty(settings.wsFileDir)
    [fn path] = uiputfile('*.bws', 'Save Workspace Settings', ...
      settings.wsFileDir);
  else
    [fn path] = uiputfile('*.bws', 'Save Workspace Settings');
  end

  if fn ~= 0
    try
      save(fullfile(path, fn), 'params');
      msgbox('Current settings have been saved', 'Settings Saved', 'modal');

      %save last path as the default
      settings.wsFileDir = fullfile(path, '.', filesep);
      saveSettings(settings, handles);
    catch
      [msg msgId] = lasterr;
      msg = regexprep(msg, '.*?\n(.*)', '$1');    
      errordlg(msg, 'Unable to save', 'modal');
      return;
    end
  end
end

% --------------------------------------------------------------------
function mnuLoadSettings_Callback(hObject, eventdata, handles)
% hObject    handle to mnuLoadSettings (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
settings = get(handles.figTop, 'userdata');
if isfield(settings, 'wsFileDir') && ~isempty(settings.wsFileDir)
  [fn path] = uigetfile('*.bws', 'Load Workspace Settings', ...
    settings.wsFileDir);
else
  [fn path] = uigetfile('*.bws', 'Load Workspace Settings');
end
if fn ~= 0
  %save last path as the default
  settings.wsFileDir = fullfile(path, '.', filesep);
  saveSettings(settings, handles);

  try
    params = load('-mat', fullfile(path, fn));
  catch
    [msg msgId] = lasterr;
    msg = regexprep(msg, '.*?\n(.*)', '$1');    
    errordlg(msg, 'Invalid file', 'modal');
    return;
  end
  
  if ~isfield(params, 'params')
    errordlg('Specified file is not a valid workspace file.', 'Invalid file', 'modal');
    return;
  end
  
  params = params.params;

  flags = isValidAnalysisParams(params);
  %ignore abscence of data files
  flags = flags - bitand(flags, 1024);
  
  if flags ~= 0
    errordlg('Specified file is not a valid workspace file.', 'Invalid file', 'modal');
    return;
  end

  if strcmp(params.domain, 'time')
    set(handles.radDomainFreq, 'value', 0);
    set(handles.radDomainTime, 'value', 1);
  else
    set(handles.radDomainFreq, 'value', 1);
    set(handles.radDomainTime, 'value', 0);
  end
  pnlDomain_SelectionChangeFcn(handles.pnlDomain, [], handles);
  if strcmp(params.acqType, 'eeg')
    set(handles.radModeECoG, 'value', 0);
    set(handles.radModeEEG, 'value', 1);
  else
    set(handles.radModeECoG, 'value', 1);
    set(handles.radModeEEG, 'value', 0);
  end
  

  %dispStr = {};
  %for idxFile = 1:length(params.dataFiles)
    %[path, fn, ext, versn] = fileparts(params.dataFiles{idxFile});
    %dispStr{idxFile} = [fn ext versn];
  %end
  %set(handles.lstDataFiles, 'value', 1);
  %set(handles.lstDataFiles, 'string', dispStr);
  %set(handles.lstDataFiles, 'userdata', params.dataFiles);
  %if isempty(dispStr)
    %set(handles.btnRemove, 'enable', 'off');
  %else
    %set(handles.btnRemove, 'enable', 'on');
  %end

  set(handles.txtMontageFile, 'string', params.montageFile);

  set(handles.txtTargetCond1, 'string', params.targetConditions{1});
  set(handles.txtTargetLabel1, 'string', params.conditionLabels{1});
  set(handles.txtTargetCond2, 'string', params.targetConditions{2});
  set(handles.txtTargetLabel2, 'string', params.conditionLabels{2});

  set(handles.txtTrialChangeCond, 'string', params.trialChangeCondition);
  set(handles.txtSpecChans, 'string', int2str(params.channels));
  set(handles.txtTopoFreqs, 'string', int2str(params.topoParams));

  if strcmpi(params.spatialFilt, 'CAR')
    set(handles.radSpatialFiltCAR, 'value', 1);
    set(handles.radSpatialFiltNone, 'value', 0);
  else
    set(handles.radSpatialFiltCAR, 'value', 0);
    set(handles.radSpatialFiltNone, 'value', 1);
  end
end
    
        
    
% --------------------------------------------------------------------
function mnuFile_Callback(hObject, eventdata, handles)
% hObject    handle to mnuFile (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)





% --- Executes during object creation, after setting all properties.
function pnlProc_CreateFcn(hObject, eventdata, handles)
% hObject    handle to pnlProc (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called




% --- Executes on button press in radDomainFreq.
function radDomainFreq_Callback(hObject, eventdata, handles)
% hObject    handle to radDomainFreq (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of radDomainFreq


% --- Executes during object creation, after setting all properties.
function pnlDomain_CreateFcn(hObject, eventdata, handles)
% hObject    handle to pnlDomain (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called


% --- Executes during object creation, after setting all properties.
function pnlAcqType_CreateFcn(hObject, eventdata, handles)
% hObject    handle to pnlAcqType (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called


% --- Executes on button press in radModeEEG.
function radModeEEG_Callback(hObject, eventdata, handles)
% hObject    handle to radModeEEG (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of radModeEEG




% --- Executes on key press over txtMontageFile with no controls selected.
function txtMontageFile_KeyPressFcn(hObject, eventdata, handles)
% hObject    handle to txtMontageFile (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

%perform some sillyness to get MATLAB to behave correctly
uicontrol(handles.lblMontageFile);
uicontrol(hObject);
uicontrol(handles.lblMontageFile);
uicontrol(hObject);

if length(strtrim(get(hObject, 'string'))) == 0
	allFilesInfo = get(handles.lblDataFiles, 'userdata');

	if ~isempty(allFilesInfo) && allFilesInfo.some1020
    set(handles.lblUsingChanNames, 'visible', 'on');
	end
else
  set(handles.lblUsingChanNames, 'visible', 'off');
end








% --- If Enable == 'on', executes on mouse press in 5 pixel border.
% --- Otherwise, executes on mouse press in 5 pixel border or over txtMontageFile.
function txtMontageFile_ButtonDownFcn(hObject, eventdata, handles)
% hObject    handle to txtMontageFile (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

function settings = reloadSettings(handles)
try
  settings = loadSettings();
catch
  error('Some of the files necessary are missing or have been corrupted.  Please reinstall.');
  return;
end
set(handles.figTop, 'userdata', settings);

function saveSettings(settings, handles)
set(handles.figTop, 'userdata', settings);
storeSettings(settings);

% --------------------------------------------------------------------
function mnuExportToPDF_Callback(hObject, eventdata, handles)
% hObject    handle to mnuExportToPDF (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

figHandles = get(handles.chkOverwritePlots, 'userdata');

if isempty(figHandles)
  uiwait(msgbox('Please run an analysis before trying to save your plots', 'Exporting to PDF'));
  return;
end

settings = get(handles.figTop, 'userdata');

if (~isfield(settings, 'warnSavePDFNameConvention') || settings.warnSavePDFNameConvention) 

  uiwait(msgbox2(['In the dialog to follow, you will be asked to choose a file name ' ...
    'under which to save your exported files.  After entering the name of your choosing ' ...
    'BCI2000 Offline Analysis will save only your most recent plots ' ... 
    'according to the following convention: ''<the name you enter> - <Plot Type>.pdf''.'], ...  
    'Exporting to PDF', 'warnSavePDFNameConvention', 'modal'));
  settings = reloadSettings(handles);

end

if get(handles.radDomainTime, 'value') == 1
  domain = 'time';
else
  domain = 'freq';
end
settings = exportPlotsToPDF(settings, figHandles, domain);
saveSettings(settings, handles);



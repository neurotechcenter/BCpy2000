%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% $Id: elocHelper.m 2007-111-26 12:31:37EST fialkoff $ 
%% 
%% File: elocHelper.m 
%% 
%% Author: Joshua Fialkoff <fialkj@rpi.edu>, Gerwin Schalk <schalk@wadsworth.org>
%%
%% Description: This file defines a class of functions that control the
%% operation of the elocHelper GUI and perform data validation.
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
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%function varargout = elocHelper(varargin)
function varargout = elocHelper(varargin)
verNum = sscanf(version, '%d.%s');
verNum = verNum(1);
%if ~strncmp(computer, 'PCWIN', 5) || verNum < 7
if verNum < 7
  error('Eloc Helper currently works only on version 7.x or greater of MATLAB or the MATLAB Component Runtime.');
  return;
end

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @elocHelper_OpeningFcn, ...
                   'gui_OutputFcn',  @elocHelper_OutputFcn, ...
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


% --- Executes just before elocHelper is made visible.
function elocHelper_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to elocHelper (see VARARGIN)

% Choose default command line output for elocHelper
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes elocHelper wait for user response (see UIRESUME)
% uiwait(handles.figTop);

try
  settings = loadSettings();
catch
  error('Some of the files necessary are missing or have been corrupted.  Please reinstall.');
  return;
end
set(handles.figTop, 'userdata', settings);
axes(handles.axHead)
elocVis(settings);
axis square;


% --- Outputs from this function are returned to the command line.
function varargout = elocHelper_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


% --- Executes on selection change in lstElecs.
function lstElecs_Callback(hObject, eventdata, handles)
% hObject    handle to lstElecs (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
settings = get(handles.figTop, 'userdata');
curIdx = get(handles.lstElecs, 'value');
elecData = get(hObject, 'userdata');
lastSelectedIdx = get(handles.pnlElecProps, 'userdata');
bOverrideValidate = ~isempty(eventdata) && eventdata;
if bOverrideValidate
  %if this is true, the last selected item was just removed
  lastSelectedIdx = [];
end

if isempty(lastSelectedIdx) || curIdx ~= lastSelectedIdx
  if isempty(lastSelectedIdx)
    %this will catch the cases where the list is empty or the last item was
    %just removed
    isValid = 1;
  else
    isValid = validateElecData(elecData(lastSelectedIdx), handles);
  end
  if isValid && (bOverrideValidate || checkForChanges(handles))
    %we don't need to check for changes if the item currently being
    %displayed was just removed
    
    if ~isempty(elecData)
      elecIdx = get(hObject, 'value');

      if ~isempty(lastSelectedIdx) && ~isempty(elecData(lastSelectedIdx).markerTextHandle)
        set(elecData(lastSelectedIdx).markerTextHandle, 'backgroundcolor', settings.markerColor);
      end

      if isempty(elecIdx)
        elecIdx = 1;
        set(hObject, 'value', elecIdx)
      end
      set(handles.txtLabel, 'string', elecData(elecIdx).label);
      set(handles.txtLabel, 'userdata', elecData(elecIdx).label);
      set(handles.chkNumLock, 'value', elecData(elecIdx).numLocked);
      set(handles.chkNumLock, 'userdata', elecData(elecIdx).numLocked);
      set(handles.txtElecNum, 'string', int2str(elecData(elecIdx).elecNum));
      set(handles.txtElecNum, 'userdata', int2str(elecData(elecIdx).elecNum));
      set(handles.pnlElecProps, 'userdata', elecIdx);
      if ~isempty(elecData(elecIdx).coords)
        tmp = sprintf(settings.coordStrFormat, elecData(elecIdx).coords(1));
        set(handles.txtXCoord, 'string', tmp);
        set(handles.txtXCoord, 'userdata', tmp);

        tmp = sprintf(settings.coordStrFormat, elecData(elecIdx).coords(2));
        set(handles.txtYCoord, 'string', tmp);
        set(handles.txtYCoord, 'userdata', tmp);
      end
      if ~isempty(elecData(elecIdx).markerTextHandle)
        try
          set(elecData(elecIdx).markerTextHandle, 'backgroundcolor', settings.selectedMarkerColor);
        catch
          elecData(elecIdx).markerTextHandle = [];
          set(handles.lstElecs, 'userdata', elecData);
        end
      end
      set(handles.btnUpdate, 'enable', 'on');
    end
  elseif ~isempty(lastSelectedIdx)
    set(hObject, 'value', lastSelectedIdx);

  end
end



function clearProperties(handles)
set(handles.pnlElecProps, 'userdata', []);
set(handles.chkNumLock, 'value', 0);
set(handles.chkNumLock, 'userdata', []);
set(handles.txtElecNum,  'string', '');
set(handles.txtElecNum, 'userdata', '');
set(handles.txtLabel, 'string', '');
set(handles.txtLabel, 'userdata', '');
set(handles.txtXCoord, 'string', '');
set(handles.txtXCoord, 'userdata', '');
set(handles.txtYCoord, 'string', '');
set(handles.txtYCoord, 'userdata', '');


% Hints: contents = get(hObject,'String') returns lstElecs contents as cell array
%        contents{get(hObject,'Value')} returns selected item from lstElecs


% --- Executes during object creation, after setting all properties.
function lstElecs_CreateFcn(hObject, eventdata, handles)
% hObject    handle to lstElecs (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: listbox controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc
    set(hObject,'BackgroundColor','white');
else
    set(hObject,'BackgroundColor',get(0,'defaultUicontrolBackgroundColor'));
end


function bSuccess = importFile(handles)
settings = get(handles.figTop, 'userdata');
strVal = get(handles.lstElecs, 'string');
mode = get(handles.axHead, 'userdata');
bSuccess = 0;

bContinue = 0;
if length(strVal) > 0
  %some entries have already been added
  if strcmp(questdlg('Importing an existing file will overwrite all current entries.  Would you like to continue?', 'Overwrite current data?', 'Yes', 'No', 'No'), 'No')
    return;
  end
end

if isfield(settings, 'montageFileDir') && ~isempty(settings.montageFileDir)
  [fn path] = uigetfile('*.*', 'Choose a montage file', ...
    settings.montageFileDir);
else
  [fn path] = uigetfile('*.*', 'Choose a montage file');
end

if fn ~= 0
  settings.montageFileDir = path;
  set(handles.figTop, 'userdata', settings);
  
  if isValidMontageFile([path fn], settings,  mode)
    reset(handles);
    axes(handles.axHead);
    switch(mode)
      case 'eeg'
        elecInfo = sortElecData(elocVis(settings, [path fn]));
      case 'ecog'
        elecInfo = sortElecData(elocVisECoG(settings, [path fn]));
    end
    strVal = cell(length(elecInfo), 1);
    for idxElec = 1:length(elecInfo)
      %set a default value for the lock parameter
      elecInfo(idxElec).numLocked = 0;
      %buld the list
      strVal{idxElec} = sprintf(settings.lstElecsStrFormat, elecInfo(idxElec).elecNum, elecInfo(idxElec).label);
    end
    set(handles.lstElecs, 'string', strVal);
    set(handles.lstElecs, 'userdata', elecInfo);
    set(handles.lstElecs, 'value', 1);
    lstElecs_Callback(handles.lstElecs, [], handles);
    set(handles.btnRemove, 'enable', 'on');
    set(handles.btnCreate, 'enable', 'on');
    set(handles.btnUseGrid, 'enable', 'on');
    bSuccess = 1;
  else
    errordlg('The selected file cannot be read or is not a valid eloc file', 'Invalid file');
  end
end

function [elecDataSort, newIdx] = sortElecData(elecData, trackIdx)
%sort the data in the structure by the electrode number
elecNums = {};
[elecNums{1:length(elecData)}] = deal(elecData.elecNum);
elecNums = cell2mat(elecNums);
[tmp sortedIdxs] = sort(elecNums);

lastElecNum = 0;
add2ElecNum = 0;
for idxElec = 1:length(elecData)
  if idxElec == 1
    elecNum = elecData(sortedIdxs(idxElec)).elecNum;
  else
    elecNum = elecDataSort(idxElec-1).elecNum+1;
    if elecData(sortedIdxs(idxElec)).elecNum > elecNum
      elecNum = elecData(sortedIdxs(idxElec)).elecNum;
    end
  end
  if idxElec ~= length(elecData) && ...
      elecNum == elecData(sortedIdxs(idxElec+1)).elecNum && ...
      ((nargin >= 2 && sortedIdxs(idxElec) ~= trackIdx && ...
      sortedIdxs(idxElec+1) == trackIdx ) || ...
      elecData(sortedIdxs(idxElec+1)).numLocked)
    %the electrode at idxElec+1 is the one that was just changed and is
    %equal in elecNum to the electrode at idxElec.  Because we just
    %changed it, we'll want to make sure it retains that number.  So,
    %switch the two. ****OR**** The electrode at idxElec+1 has a locked
    %electrode number, so we'll have to give that one precedence over the
    %current index
    tmp = sortedIdxs(idxElec+1);
    sortedIdxs(idxElec+1) = sortedIdxs(idxElec);
    sortedIdxs(idxElec) = tmp;

  end
  
  elecDataSort(idxElec) = elecData(sortedIdxs(idxElec));
  elecDataSort(idxElec).elecNum = elecNum;
  
  if nargin >= 2 && sortedIdxs(idxElec) == trackIdx
    newIdx = idxElec;
  end
  
end


function reset(handles)
  mode = get(handles.axHead, 'userdata');
  settings = get(handles.figTop, 'userdata');
  axes(handles.axHead);
  cla;
  switch(mode)
    case 'eeg'
      elocVis(settings);
    case 'ecog'
      elocVisECoG(settings);
  end
  clearProperties(handles);
  set(handles.lstElecs,  'string', {});
  set(handles.lstElecs, 'userdata', []);
  set(handles.lstElecs, 'value', 1);
  
  set(handles.btnUpdate, 'enable', 'off');
  set(handles.btnUseGrid, 'enable', 'off');  


% --------------------------------------------------------------------
function fn = prepareExport(handles)
fn = '';
settings = get(handles.figTop, 'userdata');
elecData = get(handles.lstElecs, 'userdata');

if length(elecData) == 0
  errordlg('Please add at least one electrode before exporting', 'Export Error');
  return;
elseif ~checkForChanges(handles)
  return;
end

%now loop through the electrodes and make sure they're all valid
for idxElec = 1:length(elecData)
  [isValid errMsg] = validateElecData(elecData(idxElec), handles, 1);
  if ~isValid
    errordlg(sprintf('The following error was detected for electrode #%d: %s', ...
      idxElec, errMsg));
    set(handles.lstElecs, 'value', idxElec);
    lstElecs_Callback(handles.lstElecs, 1, handles);
    return;
  end
end

bRenumber = 0;
for idxElec = 1:length(elecData)
  if ~bRenumber && idxElec ~= elecData(idxElec).elecNum
    if strcmp(questdlg('In order to export electrode location data, electrodes must be numbered consecuatively from 1 to the number of electrodes.  If you would like to have your electrodes renumbered to suit this criteria, click Fix.  Click Cancel if you prefer to make the changes yourself.', ...
        'Electrode Numbering Error', 'Fix', 'Cancel', 'Cancel'), 'Fix')
      bRenumber = 1;
    else
      return;
    end
  end
  if bRenumber
    elecData(idxElec).elecNum = idxElec;
  end
end

if bRenumber
  updateList(elecData, handles);
end

%data is good, figure out where we're saving it

if isfield(settings, 'montageFileDir') && ~isempty(settings.montageFileDir)
  [fn path] = uiputfile('*.*', 'Export as...', ...
    settings.montageFileDir);
else
  [fn path] = uiputfile('*.*', 'Export as...');
end

if fn ~= 0
  fn = [path fn];
  
  settings.montageFileDir = path;
  set(handles.figTop, 'userdata', settings);  
end

  



% --------------------------------------------------------------------
function mnuFile_Callback(hObject, eventdata, handles)
% hObject    handle to mnuFile (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --- Executes during object creation, after setting all properties.
function axHead_CreateFcn(hObject, eventdata, handles)
% hObject    handle to axHead (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: place code in OpeningFcn to populate axHead





function txtLabel_Callback(hObject, eventdata, handles)
% hObject    handle to txtLabel (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of txtLabel as text
%        str2double(get(hObject,'String')) returns contents of txtLabel as a double


% --- Executes during object creation, after setting all properties.
function txtLabel_CreateFcn(hObject, eventdata, handles)
% hObject    handle to txtLabel (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc
    set(hObject,'BackgroundColor','white');
else
    set(hObject,'BackgroundColor',get(0,'defaultUicontrolBackgroundColor'));
end



function txtXCoord_Callback(hObject, eventdata, handles)
% hObject    handle to txtXCoord (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of txtXCoord as text
%        str2double(get(hObject,'String')) returns contents of txtXCoord as a double


% --- Executes during object creation, after setting all properties.
function txtXCoord_CreateFcn(hObject, eventdata, handles)
% hObject    handle to txtXCoord (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc
    set(hObject,'BackgroundColor','white');
else
    set(hObject,'BackgroundColor',get(0,'defaultUicontrolBackgroundColor'));
end



function txtYCoord_Callback(hObject, eventdata, handles)
% hObject    handle to txtYCoord (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of txtYCoord as text
%        str2double(get(hObject,'String')) returns contents of txtYCoord as a double


% --- Executes during object creation, after setting all properties.
function txtYCoord_CreateFcn(hObject, eventdata, handles)
% hObject    handle to txtYCoord (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc
    set(hObject,'BackgroundColor','white');
else
    set(hObject,'BackgroundColor',get(0,'defaultUicontrolBackgroundColor'));
end


% --- Executes on button press in btnUseGrid.
function btnUseGrid_Callback(hObject, eventdata, handles)
% hObject    handle to btnUseGrid (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
settings = get(handles.figTop, 'userdata');
set(handles.lblUseGrid, 'visible', 'on');
axes(handles.axHead);
button = [];
xCord = [];
yCord = [];

while isempty(button) || ~(button == settings.escKeyButtonCode || (button == settings.leftMouseButtonCode && ~isempty(xCord) && ~isempty(yCord)))
  [xCord yCord button] = ginput(1);
end
if button == 1
  mode = get(handles.axHead, 'userdata');

  if strcmp(mode, 'ecog') && (any(max(xCord,yCord) > settings.ecogCoordRange(2)) || ...
      any(min(xCord,yCord) < settings.ecogCoordRange(1))) 
    errordlg('Invalid ECoG electrode location');
  elseif strcmp(mode, 'eeg') && (xCord^2 + yCord^2 > settings.headRadius^2)
    errordlg('Invalid EEG electrode location');
  else
    set(handles.txtXCoord, 'string', sprintf(settings.coordStrFormat, xCord));
    set(handles.txtYCoord, 'string', sprintf(settings.coordStrFormat, yCord));
  end
end
set(handles.lblUseGrid, 'visible', 'off');  



% --- Executes on button press in btnAdd.
function btnAdd_Callback(hObject, eventdata, handles)
% hObject    handle to btnAdd (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --- Executes on button press in btnUpdate.
function btnUpdate_Callback(hObject, eventdata, handles)
% hObject    handle to btnUpdate (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
settings = get(handles.figTop, 'userdata');
formVals = validateProperties(handles);
if ~isempty(formVals)
  axes(handles.axHead);
  elecInfo = get(handles.lstElecs, 'userdata');
  curIdx = get(handles.pnlElecProps, 'userdata');

  elecInfo(curIdx).numLocked = formVals.numLocked;
  elecInfo(curIdx).elecNum = formVals.elecNum;
  elecInfo(curIdx).label = formVals.label;
  elecInfo(curIdx).coords = formVals.coordinates;
  
  %now sort the data based on the new elecNum paying attention to any
  %shifts in this electrodes index in the structure
  [elecInfo curIdx] = sortElecData(elecInfo, curIdx);
  set(handles.pnlElecProps, 'userdata', curIdx);
  
  %remove marker from topoplot.  The try/catch is there just to be careful
  try
    delete(elecInfo(curIdx).markerTextHandle);
  catch
  end
  
  %add new marker
  hold on;
  [markerTextHandle] = ...
    addTopoMarker(elecInfo(curIdx).coords(1), ...
    elecInfo(curIdx).coords(2), int2str(elecInfo(curIdx).elecNum), ...
    settings);
  hold off;
  set(markerTextHandle, 'backgroundcolor', settings.selectedMarkerColor);
  
  
  elecInfo(curIdx).markerTextHandle = markerTextHandle;
  
  %update lstElecs and marker text
  updateList(elecInfo, handles);
  %and in case the index changed as a result of sorting
  set(handles.lstElecs, 'value', curIdx);
  
  %update old values used to check for changes
  set(handles.chkNumLock, 'userdata', formVals.numLocked);
  set(handles.txtElecNum, 'userdata', int2str(formVals.elecNum));
  set(handles.txtLabel, 'userdata', formVals.label);
  set(handles.txtXCoord, 'userdata', get(handles.txtXCoord, 'string'));
  set(handles.txtYCoord, 'userdata', get(handles.txtYCoord, 'string'))
  
end
 
function updateList(elecInfo, handles)
settings = get(handles.figTop, 'userdata');
strVal = {};
for idxElec = 1:length(elecInfo)
  if elecInfo(idxElec).numLocked
    strVal{idxElec} = sprintf(settings.lstElecsStrFormatLocked, ...
      elecInfo(idxElec).elecNum, elecInfo(idxElec).label);
  else
    strVal{idxElec} = sprintf(settings.lstElecsStrFormat, ...
      elecInfo(idxElec).elecNum, elecInfo(idxElec).label);
  end
   set(elecInfo(idxElec).markerTextHandle, 'string', ...
    int2str(elecInfo(idxElec).elecNum));
end
set(handles.lstElecs, 'string', strVal);
set(handles.lstElecs, 'userdata', elecInfo);

function formVals = validateProperties(handles)
settings = get(handles.figTop, 'userdata');
formVals = [];

elecNumVal = get(handles.txtElecNum, 'string');
labVal = get(handles.txtLabel, 'string');
xCoord = get(handles.txtXCoord, 'string');
yCoord = get(handles.txtYCoord, 'string');

%trim whitespace
elecNumVal = regexprep(elecNumVal, settings.strCleanUpRegexrep, '$1');
labVal = regexprep(labVal, settings.strCleanUpRegexrep, '$1');
xCoord = regexprep(xCoord, settings.strCleanUpRegexrep, '$1');
yCoord = regexprep(yCoord, settings.strCleanUpRegexrep, '$1');

if isempty(elecNumVal)
  errordlg('Please enter a value for the electrode number', 'Invalid electrode number');
  return;
else
  elecNumVal = str2double(elecNumVal);
  if isnan(elecNumVal) || ceil(elecNumVal) ~= elecNumVal || elecNumVal < 1
    errordlg('Please enter a positive integer value for the electrode number');
    return;
  end
  
  %now make sure that the specified electrode number isn't already taken
  %(and locked in) for some other electrode
  elecData = get(handles.lstElecs, 'userdata');
  curIdx = get(handles.pnlElecProps, 'userdata');
  for idxElec = 1:length(elecData)
    if elecData(idxElec).elecNum > elecNumVal
      %elecData should be ordered with respect to elecNum, so nothing after
      %this point could be equal to the specified elecNum
      break;
    elseif idxElec ~= curIdx && elecData(idxElec).elecNum == elecNumVal ...
        && elecData(idxElec).numLocked
      if strcmp(questdlg('The specified electrode number is already in use for another electrode that maintains a lock on the number.  Do you wish to remove the lock and proceed?', 'Electrode Locked', 'Yes', 'No', 'No'), 'No')
        return;
      else
        elecData(idxElec).numLocked = 0;
        set(handles.lstElecs, 'userdata', elecData);
        break;
      end
    end
  end
      
end

if isempty(labVal)
  errordlg('Please enter a value for the electrode label', 'Invalid label');
  return;
elseif length(labVal) > settings.labelMaxLen
  errordlg(sprintf('Please select an electrode label that comprises 1 to %d characters', ...
    settings.labelMaxLen), 'Invalid Label');
  return;
end
if isempty(xCoord)
  errordlg('Please enter a value for the electrode X coordinate', 'Invalid coordinates');
  return;
else
  xCoord = str2double(xCoord);
  if isnan(xCoord) || length(xCoord) ~= 1 || ~isreal(xCoord)
    errordlg('Please enter a valid X coordinate for this electrode', 'Invalid coordinates');
    return;
  end
end
if isempty(yCoord)
  errordlg('Please enter a value for the electrode Y coordinate', 'Invalid coordinates');
  return;
else
  yCoord = str2double(yCoord);
  if isnan(yCoord) || length(yCoord) ~= 1 || ~isreal(yCoord)
    errordlg('Please enter a valid Y coordinate for this electrode', 'Invalid coordinates');
    return;
  end
end

mode = get(handles.axHead, 'userdata');
switch(mode)
  case 'eeg'
    if xCoord^2 + yCoord^2 > settings.headRadius
      errordlg(sprintf('The specified coordinates are outside the valid range (i.e., x^2 + y^2 must be less than or equal to %0.2f)', settings.headRadius), 'Invalid coordinates');
      return;
    end
  case 'ecog'
    if xCoord < settings.ecogCoordRange(1) || ...
        xCoord > settings.ecogCoordRange(2)
      errordlg(sprintf('The specified x-coordinate is outside the valid range.  Please choose a number between %d and %d.', ...
        settings.ecogCoordRange(1), settings.ecogCoordRange(2)));
      return;
    elseif yCoord < settings.ecogCoordRange(1) || ...
        yCoord > settings.ecogCoordRange(2)
      errordlg(sprintf('The specified y-coordinate is outside the valid range.  Please choose a number between %0.4f and %0.4f.', ...
        settings.ecogCoordRange(1), settings.ecogCoordRange(2)));  
      return;
    end
end

formVals = struct('numLocked', [], 'elecNum', [], 'label', '', 'coordinates', []);
formVals.elecNum = elecNumVal;
formVals.numLocked = (get(handles.chkNumLock, 'value') == 1);
formVals.label = labVal;
formVals.coordinates = [xCoord yCoord];
return;


% --- Executes on button press in btnCreate.
function btnCreate_Callback(hObject, eventdata, handles)
% hObject    handle to btnCreate (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
settings = get(handles.figTop, 'userdata');
curIdx = get(handles.pnlElecProps, 'userdata');
elecInfo = get(handles.lstElecs, 'userdata');

if isempty(curIdx)
  isValid = 1;
else
  isValid = validateElecData(elecInfo(curIdx), handles);
end
if isValid && checkForChanges(handles)
  if isempty(elecInfo)
    newElecNum = 1;
  else
    newElecNum = elecInfo(end).elecNum + 1;
  end
  newIdx = length(elecInfo)+1;
  elecInfo(newIdx).label = '';
  elecInfo(newIdx).numLocked = 0;
  elecInfo(newIdx).elecNum = newElecNum;
  elecInfo(newIdx).coords = [];
  elecInfo(newIdx).markerTextHandle = [];

  vals = get(handles.lstElecs, 'string');
  vals{end+1} = sprintf(settings.lstElecsStrFormat, newElecNum, settings.defaultElecLabel);

  set(handles.btnUpdate, 'enable', 'on');
  set(handles.btnUseGrid, 'enable', 'on');
  set(handles.btnRemove, 'enable', 'on');

  clearProperties(handles);
  set(handles.txtLabel, 'string', settings.defaultElecLabel);
  set(handles.txtElecNum, 'string', newElecNum);
  set(handles.txtElecNum, 'userdata', int2str(newElecNum));
  set(handles.pnlElecProps, 'userdata', newIdx);

  set(handles.lstElecs, 'string', vals);
  set(handles.lstElecs, 'value', newIdx);
  set(handles.lstElecs, 'userdata', elecInfo);
  
  %set the previously selected item back to the default color
  if ~isempty(curIdx)
    set(elecInfo(curIdx).markerTextHandle, 'backgroundcolor', settings.markerColor);
  end
end


function [isValid, errMsg] = validateElecData(elecData, handles, bQuiet)
if nargin < 3
  bQuiet = 0;
end
errMsg = '';
settings = get(handles.figTop, 'userdata');

isValid = 0;
if isempty(elecData.elecNum)
  errMsg = 'Please enter a valid number for the current electrode and hit Update';
  if ~bQuiet
    errordlg(errMsg, 'Data Error');
  end
  return;
end  
if isempty(elecData.label)
  errMsg = 'Please enter a valid label for the current electrode and hit Update';
  if ~bQuiet
    errordlg(errMsg, 'Data Error');
  end
  return;
elseif length(elecData.label) > settings.labelMaxLen
  errMsg = sprintf('Electrode labels must be no longer than %d characters.  Please enter a label for the current electrode that comprises 1 to %d characters and hit Update', ...
    settings.labelMaxLen, settings.labelMaxLen);
  if ~bQuiet
    errordlg(errMsg, 'Data Error');
  end
  return;  
end
if isempty(elecData.coords)
  errMsg = 'Please enter valid coordinates for the current electrode and hit Update';
  if ~bQuiet
    errordlg(errMsg, 'Data Error');
  end
  return;
else
  mode = get(handles.axHead, 'userdata');

  if strcmp(mode, 'eeg') && sum(elecData.coords.^2) > settings.headRadius^2
    errMsg = sprintf('The specified coordinates are outside the valid range (i.e., x^2 + y^2 must be less than or equal to %0.2f).  Please enter valid X and Y coordinates for the current electrode and hit Update.', settings.headRadius);
    if ~bQuiet
      errordlg(errMsg, 'Data Error');
    end
    return;
  elseif strcmp(mode, 'ecog') && (any(elecData.coords > settings.ecogCoordRange(2)) || ...
      any(elecData.coords < settings.ecogCoordRange(1))) 
    errMsg = sprintf('Please choose X and Y coordinates that are between the values of %d and %d.', ...
      settings.ecogCoordRange(1), settings.ecogCoordRange(2));
    if ~bQuiet
      errordlg(errMsg, 'Data Error');
    end
  end
end

isValid = 1;
return;


function bCont = checkForChanges(handles)
bCont = 1;
numLock = get(handles.chkNumLock, 'value');
numLockOld = get(handles.chkNumLock, 'userdata');
elecNum = get(handles.txtElecNum, 'string');
elecNumOld = get(handles.txtElecNum, 'userdata');
xCoord = get(handles.txtXCoord, 'string');
xCoordOld = get(handles.txtXCoord, 'userdata');
yCoord = get(handles.txtYCoord, 'string');
yCoordOld = get(handles.txtYCoord, 'userdata');
labVal = get(handles.txtLabel, 'string');
labValOld = get(handles.txtLabel, 'userdata');
if ~(isempty(numLockOld) && isempty(xCoordOld) && isempty(yCoordOld) && isempty(labValOld) && isempty(elecNumOld)) && ...
    ~(numLock == numLockOld && strcmp(elecNum, elecNumOld) && strcmp(xCoord, xCoordOld) && strcmp(yCoord, yCoordOld) && strcmp(labVal, labValOld))
  switch questdlg('There are unsaved changes.  Do you wish to continue and discard these changes?', 'Unsaved Changes', 'Yes', 'No', 'No')
    case 'Yes'
      return;
    case 'No'
      bCont = 0;
      return;
  end
end

return;


% --- Executes on button press in btnRemove.
function btnRemove_Callback(hObject, eventdata, handles)
% hObject    handle to btnRemove (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
settings = get(handles.figTop, 'userdata');

elecInfo = get(handles.lstElecs, 'userdata');
curIdx = get(handles.lstElecs, 'value');
strVals = get(handles.lstElecs, 'string');

%remove figure markers, try/catch for safety
try
  delete(elecInfo(curIdx).markerTextHandle);
catch
end

elecInfo(curIdx) = [];
strVals(curIdx) = [];

%now update the marker text of all remaining markers
% for idxMarker = 1:length(elecInfo)
%   set(elecInfo(idxMarker).markerTextHandle, 'string', int2str(idxMarker));
% end

% for idx=1:length(strVals)
%   strVals{idx} = sprintf(settings.lstElecsStrFormat, idx, elecInfo(idx).label);
% end

set(handles.lstElecs, 'string', strVals);
set(handles.lstElecs, 'userdata', elecInfo);

if curIdx > 1
  curIdx = curIdx - 1;
elseif curIdx == length(elecInfo)
  curIdx = 0;
end
if curIdx >= 1
  set(handles.lstElecs, 'value', curIdx);
  lstElecs_Callback(handles.lstElecs, 1, handles);
else
  %last item was deleted
  clearProperties(handles);
  set(handles.btnUseGrid, 'enable', 'off');
  set(handles.btnUpdate, 'enable', 'off');
  set(handles.btnRemove, 'enable', 'off');
end




% --- Executes on button press in btnCancel.
function btnCancel_Callback(hObject, eventdata, handles)
% hObject    handle to btnCancel (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)




% --- Executes on mouse press over axes background.
function axHead_ButtonDownFcn(hObject, eventdata, handles)
% hObject    handle to axHead (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)




% --------------------------------------------------------------------
function mnuExportBci2000_Callback(hObject, eventdata, handles)
% hObject    handle to mnuExportBci2000 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
fn = prepareExport(handles);
if fn ~= 0
  settings = get(handles.figTop, 'userdata');
  elecData = get(handles.lstElecs, 'userdata');
  try
    bSuccess = exportBCI2000ElocFile(elecData, fn, settings);
  catch
    bSuccess = 0;
  end

  if bSuccess
    msgbox('Electrode data was successfully exported!', 'Export Success');
  else
    errordlg('Unable to write to file', 'Export Error');
  end
end


% --------------------------------------------------------------------
function mnuExportOffline_Callback(hObject, eventdata, handles)
% hObject    handle to mnuExportOffline (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
fn = prepareExport(handles);
if fn ~= 0
  settings = get(handles.figTop, 'userdata');
  elecData = get(handles.lstElecs, 'userdata');
  try
    bSuccess = exportOfflineElocFile(elecData, fn, settings);
  catch
    bSuccess = 0;
  end

  if bSuccess
    msgbox('Electrode data was successfully exported!', 'Export Success');
  else
    errordlg('Unable to write to file', 'Export Error');
  end
end



% --------------------------------------------------------------------
function mnuNewEEG_Callback(hObject, eventdata, handles)
% hObject    handle to mnuNewEEG (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
if prepareNew(handles)
  set(handles.axHead, 'userdata', 'eeg');
  reset(handles);
end

% --------------------------------------------------------------------
function mnuNewECoG_Callback(hObject, eventdata, handles)
% hObject    handle to mnuNewECoG (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
if prepareNew(handles)
  set(handles.axHead, 'userdata', 'ecog');
  reset(handles);
end

function bContinue = prepareNew(handles)
strVal = get(handles.lstElecs, 'string');
bContinue = 0;
if length(strVal) > 0
  %some entries have already been added
  if strcmp(questdlg('Creating a new file will clear all of the current entries.  Do you wish to continue?', ...
      'Overwrite current data?', 'Yes', 'No', 'No'), 'No')
    return;
  end
end
bContinue = 1;

% --------------------------------------------------------------------
function mnuImport_Callback(hObject, eventdata, handles)
% hObject    handle to mnuImportEEG (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% --------------------------------------------------------------------
function mnuImportEEG_Callback(hObject, eventdata, handles)
% hObject    handle to mnuImport (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
curMode = get(handles.axHead, 'userdata');
set(handles.axHead, 'userdata', 'eeg');
if ~importFile(handles)
  set(handles.axHead, 'userdata', curMode);
end

% --------------------------------------------------------------------
function mnuImportECoG_Callback(hObject, eventdata, handles)
% hObject    handle to mnuImportECoG (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
curMode = get(handles.axHead, 'userdata');
set(handles.axHead, 'userdata', 'ecog');
if ~importFile(handles)
  set(handles.axHead, 'userdata', curMode);
end

% --------------------------------------------------------------------
function mnuNew_Callback(hObject, eventdata, handles)
% hObject    handle to mnuNew (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)




% --------------------------------------------------------------------
function mnuModeEEG_Callback(hObject, eventdata, handles)
% hObject    handle to mnuModeEEG (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(handles.mnuModeECoG, 'checked', 'off');
set(hObject, 'checked', 'on');

% --------------------------------------------------------------------
function mnuModeECoG_Callback(hObject, eventdata, handles)
% hObject    handle to mnuModeECoG (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(handles.mnuModeEEG, 'checked', 'off');
set(hObject, 'checked', 'on');

% --------------------------------------------------------------------
function mnuMode_Callback(hObject, eventdata, handles)
% hObject    handle to mnuMode (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --------------------------------------------------------------------
function mnuOptions_Callback(hObject, eventdata, handles)
% hObject    handle to mnuOptions (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)





function txtElecNum_Callback(hObject, eventdata, handles)
% hObject    handle to txtElecNum (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of txtElecNum as text
%        str2double(get(hObject,'String')) returns contents of txtElecNum as a double


% --- Executes during object creation, after setting all properties.
function txtElecNum_CreateFcn(hObject, eventdata, handles)
% hObject    handle to txtElecNum (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc
    set(hObject,'BackgroundColor','white');
else
    set(hObject,'BackgroundColor',get(0,'defaultUicontrolBackgroundColor'));
end


% --- Executes on button press in chkNumLock.
function chkNumLock_Callback(hObject, eventdata, handles)
% hObject    handle to chkNumLock (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of chkNumLock




% --- Executes when user attempts to close figTop.
function figTop_CloseRequestFcn(hObject, eventdata, handles)
% hObject    handle to figTop (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: delete(hObject) closes the figure
settings = get(handles.figTop, 'userdata');
storeSettings(settings);

delete(hObject);



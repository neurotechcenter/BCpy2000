%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% $Id: FilterGUI.m 4536 2013-08-05 14:30:13Z mellinger $ 
% Author: juergen.mellinger@uni-tuebingen.de
% Description: A simple GUI to apply HP, LP, and Notch filtering to BCI2000 data 
%   files.
%
% (C) 2000-2010, BCI2000 Project
% http:%%www.bci2000.org 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function varargout = FilterGUI(varargin)
% FILTERGUI M-file for FilterGUI.fig
%      FILTERGUI, by itself, creates a new FILTERGUI or raises the existing
%      singleton*.
%
%      H = FILTERGUI returns the handle to a new FILTERGUI or the handle to
%      the existing singleton*.
%
%      FILTERGUI('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in FILTERGUI.M with the given input arguments.
%
%      FILTERGUI('Property','Value',...) creates a new FILTERGUI or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before FilterGUI_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to FilterGUI_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help FilterGUI

% Last Modified by GUIDE v2.5 18-May-2010 17:07:43

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @FilterGUI_OpeningFcn, ...
                   'gui_OutputFcn',  @FilterGUI_OutputFcn, ...
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


% --- Executes just before FilterGUI is made visible.
function FilterGUI_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to FilterGUI (see VARARGIN)

% Choose default command line output for FilterGUI
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes FilterGUI wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = FilterGUI_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


% --- Executes on selection change in hpMenu.
function hpMenu_Callback(hObject, eventdata, handles)
% hObject    handle to hpMenu (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns hpMenu contents as cell array
%        contents{get(hObject,'Value')} returns selected item from hpMenu


% --- Executes during object creation, after setting all properties.
function hpMenu_CreateFcn(hObject, eventdata, handles)
% hObject    handle to hpMenu (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on selection change in lpMenu.
function lpMenu_Callback(hObject, eventdata, handles)
% hObject    handle to lpMenu (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns lpMenu contents as cell array
%        contents{get(hObject,'Value')} returns selected item from lpMenu


% --- Executes during object creation, after setting all properties.
function lpMenu_CreateFcn(hObject, eventdata, handles)
% hObject    handle to lpMenu (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on selection change in notchMenu.
function notchMenu_Callback(hObject, eventdata, handles)
% hObject    handle to notchMenu (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns notchMenu contents as cell array
%        contents{get(hObject,'Value')} returns selected item from notchMenu


% --- Executes during object creation, after setting all properties.
function notchMenu_CreateFcn(hObject, eventdata, handles)
% hObject    handle to notchMenu (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



% --- Executes on button press in applyButton.
function applyButton_Callback(hObject, eventdata, handles)
% hObject    handle to applyButton (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
[files, path] = uigetfile('*.dat', 'Choose a BCI2000 data file', ...
    'multiselect', 'on');
if(~isnumeric(files))
  contents = cellstr(get(handles.hpMenu, 'String'));
  hp = contents{get(handles.hpMenu, 'Value')};
  switch( hp )
      case 'Off'
          settings.hp = 0;
      case '0.1 Hz'
          settings.hp = 0.1;
      case '1 Hz'
          settings.hp = 1;
      case '5 Hz'
          settings.hp = 5;
      otherwise
          error('Unexpected value in HP menu');
  end 
  contents = cellstr(get(handles.lpMenu, 'String'));
  lp = contents{get(handles.lpMenu, 'Value')};
  switch( lp )
      case 'Off'
          settings.lp = 0;
      case '9 Hz'
          settings.lp = 9;
      case '30 Hz'
          settings.lp = 30;
      case '40 Hz'
          settings.lp = 40;
      case '70 Hz'
          settings.lp = 70;
      otherwise
          error('Unexpected value in LP menu');
  end 
  contents = cellstr(get(handles.notchMenu, 'String'));
  notch = contents{get(handles.notchMenu, 'Value')};
  switch( notch )
      case 'Off'
          settings.notch = 0;
      case '50 Hz'
          settings.notch = 50;
      case '60 Hz'
          settings.notch = 60;
      otherwise
          error('Unexpected value in Notch menu');
  end 
  if(~iscell(files))
    files = {files};
  end
  for(i=1:length(files))
    infile = [path files{i}];
    outfile = [path regexprep(files{i}, '\.dat$', '') '.filtered.dat'];
    ApplyFilter(infile, outfile, settings);
  end
end

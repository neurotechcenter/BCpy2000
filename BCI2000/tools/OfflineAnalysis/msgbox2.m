%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% $Id: msgbox2.m 2007-03-14 12:31:37EST fialkoff $ 
%% 
%% File: OfflineAnalysis.m 
%% 
%% Author: Joshua Fialkoff <fialkj@rpi.edu>, Gerwin Schalk <schalk@wadsworth.org>
%%
%% Description: This function creates a dialog box similar to msgbox that 
%% allows the user to choose whether or not the message should be displayed
%% in the future.  State is maintained via settings.mat.
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
function varargout=msgbox2(varargin)
%MSGBOX Message box.
%  msgbox(Message,Title,ParamName) creates a message box that automatically wraps
%  Message to fit an appropriately sized Figure.  Message is a string
%  vector, string matrix or cell array.  Title specified the title of the message
%  box.  The message box also contains a checkbox that will allow the user
%  to prevent a given message from showing in the future.  The settings.mat
%  parameter corresponding to the warning is specified in ParamName.
%
%  msgbox(Message, ... ,CreateMode) specifies whether the message box is modal
%  or non-modal and if it is non-modal, whether to replace another
%  message box with the same Title.  Valid values for CreateMode are
%  'modal', 'non-modal','replace'.  'non-modal' is the default.
%
%  CreateMode may also be a structure with fields WindowStyle and
%  msgbox(Message,Title,ParamName,CreateMode,Icon) specifies which Icon to display in
%  the message box.  Icon is 'none', 'error', 'help', 'warn', or
%  'custom'. The default is 'none'.
%
%  h = msgbox(...) returns the handle of the box in h.
%
%  To make msgbox block execution until the user responds, include the
%  string 'modal' in the input argument list and wrap the call to
%  msgbox with UIWAIT.
%
%  An example which blocks execution until the user responds:
%    uiwait(msgbox('String','Title','ParamName','modal'));
%

error(nargchk(3,4,nargin));
error(nargoutchk(0,1,nargout));

BodyTextString=varargin{1};
TitleString = varargin{2};
ParamName = varargin{3};

globalParams = struct();
globalParams.paramName = ParamName;



if ~iscell(BodyTextString),BodyTextString=cellstr(BodyTextString);end

%%%%%%%%%%%%%%%%%%%%
%%% Nargin Check %%%
%%%%%%%%%%%%%%%%%%%%

IconString='none';
Interpreter='none';
CreateMode = 'non-modal';

if nargin == 4
  CreateMode = varargin{4};
end  

CreateMode=lower(CreateMode);
if ~strcmp(CreateMode,'non-modal')& ~strcmp(CreateMode,'modal') & ...
   ~strcmp(CreateMode,'replace'),
    warning('Invalid string for CreateMode in MSGBOX.');
    CreateMode='non-modal';
end    

IconString=lower(IconString);
if ~(strcmp(IconString,'none')|strcmp(IconString,'help')  ...
    |strcmp(IconString,'warn')|strcmp(IconString,'error') ...
    |strcmp(IconString,'custom')),
    warning('Invalid string for Icon in MSGBOX.');
    IconString='none';
end

%%%%%%%%%%%%%%%%%%%%%
%%% General Info. %%%
%%%%%%%%%%%%%%%%%%%%%
Black      =[0       0        0      ]/255;
LightGray  =[192     192      192    ]/255;
LightGray2 =[160     160      164    ]/255;
MediumGray =[128     128      128    ]/255;
White      =[255     255      255    ]/255;

%%%%%%%%%%%%%%%%%%%%%
%%% Set Positions %%%
%%%%%%%%%%%%%%%%%%%%%
DefFigPos=get(0,'DefaultFigurePosition');

MsgOff=7;
%IconWidth=32;
IconWidth=38;

FigWidth=200;
if strcmp(IconString,'none'),
  MsgTxtWidth=FigWidth-2*MsgOff;
else,
  MsgTxtWidth=FigWidth-2*MsgOff-IconWidth;
end
FigHeight=100;
DefFigPos(3:4)=[FigWidth FigHeight];

ChkWidth=FigWidth-2*MsgOff;
ChkHeight=17;
ChkXOffset=MsgOff;
ChkYOffset=MsgOff;

OKWidth=40;
OKHeight=17;
OKXOffset=(FigWidth-OKWidth)/2;
OKYOffset=MsgOff+ChkYOffset+ChkHeight;


MsgTxtXOffset=MsgOff;
MsgTxtYOffset=MsgOff+OKYOffset+OKHeight+ChkYOffset+ChkHeight;
MsgTxtHeight=FigHeight-MsgOff-MsgTxtYOffset;
MsgTxtForeClr=Black;

%IconHeight=32;
IconHeight=38;
IconXOffset=MsgTxtXOffset;
IconYOffset=FigHeight-MsgOff-IconHeight;

%%%%%%%%%%%%%%%%%%%%%
%%% Create MsgBox %%%
%%%%%%%%%%%%%%%%%%%%%

CreateModeFlag=1;

% See if a modal or replace dialog already exists and delete all of its 
% children
MsgboxTag = ['Msgbox_', TitleString];
if ~strcmp(CreateMode,'non-modal'),
  TempHide=get(0,'ShowHiddenHandles');
  set(0,'ShowHiddenHandles','on');
  OldFig=findobj(0,'Type','figure','Tag',MsgboxTag,'Name',TitleString);
  if ~isempty(OldFig),
    CreateModeFlag=0;
    if length(OldFig)>1,
      BoxHandle=OldFig(1);
      close(OldFig(2:end));
      OldFig(2:end)=[];
    end % if length
    BoxHandle=OldFig;
    CurPos=get(BoxHandle,'Position');
    CurPos(3:4)=[FigWidth FigHeight];
    set(BoxHandle,'Position',CurPos);
    BoxChildren=get(BoxHandle,'Children');
    delete(BoxChildren);
    figure(BoxHandle);
  end
  set(0,'ShowHiddenHandles',TempHide);
end

if strcmp(CreateMode,'modal'),
  WindowStyle='modal';
else,
  WindowStyle='normal';
end

if CreateModeFlag,
  BoxHandle=dialog(                                            ...
                  'Name'            ,TitleString             , ...
                  'Pointer'         ,'arrow'                 , ...
                  'Units'           ,'points'                , ...
                  'Visible'         ,'off'                   , ...
                  'KeyPressFcn'     ,@doKeyPress             , ...
                  'WindowStyle'     ,WindowStyle             , ...
                  'HandleVisibility','callback'              , ...
                  'Toolbar'         ,'none'                  , ...
                  'Tag'             ,MsgboxTag                 ...
                  );
else,
  set(BoxHandle,   ...
     'WindowStyle'     ,WindowStyle, ...
     'HandleVisibility','on'         ...
     );
end %if strcmp


FigColor=get(BoxHandle,'Color');

MsgTxtBackClr=FigColor;

Font.FontUnits='points';
Font.FontSize=get(0,'FactoryUIControlFontSize');
Font.FontName=get(0,'FactoryUIControlFontName');

OKHandle=uicontrol(BoxHandle           , Font                             , ...
                  'Style'              ,'pushbutton'                      , ...
                  'Units'              ,'points'                          , ...
                  'Position'           ,[ OKXOffset OKYOffset OKWidth OKHeight ] , ...
                  'CallBack'           ,'mbSaveAndClose(gcbf);'                    , ...
                  'KeyPressFcn'        ,@doKeyPress                       , ...
                  'String'             ,'OK'                              , ...
                  'HorizontalAlignment','center'                          , ...
                  'Tag'                ,'OKButton'                          ...
                  );
 
chkHandle=uicontrol(BoxHandle           , Font                             , ...
                  'Style'              ,'checkbox'                      , ...
                  'Units'              ,'points'                          , ...
                  'Position'           ,[ ChkXOffset ChkYOffset ChkWidth ChkHeight ] , ...
                  'String'             ,'Don''t display this message in the future'	, ...
                  'HorizontalAlignment','left'                          , ...
                  'Tag'                ,'chkShow'                          ...
                  );

globalParams.check = chkHandle;

MsgHandle=uicontrol(BoxHandle           , Font          , ...
                   'Style'              ,'text'         , ...
                   'Units'              ,'points'       , ...
                   'Position'           ,[MsgTxtXOffset ...
                                          MsgTxtYOffset ...
                                          MsgTxtWidth   ...
                                          MsgTxtHeight  ...
                                         ]              , ...
                   'String'             ,' '            , ...
                   'Tag'                ,'MessageBox'   , ...
                   'HorizontalAlignment','left'         , ...    
                   'BackgroundColor'    ,MsgTxtBackClr  , ...
                   'ForegroundColor'    ,MsgTxtForeClr   ...
                   );

               
set(BoxHandle, 'userdata', globalParams);

[WrapString,NewMsgTxtPos]=textwrap(MsgHandle,BodyTextString,75);
NumLines=size(WrapString,1);

MsgTxtWidth=max(MsgTxtWidth,NewMsgTxtPos(3));
MsgTxtHeight=max(MsgTxtHeight,NewMsgTxtPos(4));

if ~strcmp(IconString,'none'),
  MsgTxtXOffset=IconXOffset+IconWidth+MsgOff;
  FigWidth=MsgTxtXOffset+MsgTxtWidth+MsgOff;  
  % Center Vertically around icon  
  if IconHeight>MsgTxtHeight,
    IconYOffset=OKYOffset+OKHeight+MsgOff;
    MsgTxtYOffset=IconYOffset+(IconHeight-MsgTxtHeight)/2;
    FigHeight=IconYOffset+IconHeight+MsgOff;    
  % center around text    
  else,
    MsgTxtYOffset=OKYOffset+OKHeight+MsgOff;
    IconYOffset=MsgTxtYOffset+(MsgTxtHeight-IconHeight)/2;
    FigHeight=MsgTxtYOffset+MsgTxtHeight+MsgOff;    
  end    
  
else,
  FigWidth=MsgTxtWidth+2*MsgOff;  
  MsgTxtYOffset=OKYOffset+OKHeight+MsgOff;
  FigHeight=MsgTxtYOffset+MsgTxtHeight+MsgOff;
end % if ~strcmp

OKXOffset=(FigWidth-OKWidth)/2; 
DefFigPos(3:4)=[FigWidth FigHeight];
DefFigPos = getnicedialoglocation(DefFigPos, get(BoxHandle,'Units'));

% if there is a figure out there and it's modal, we need to be modal too
if ~isempty(gcbf) && strcmp(get(gcbf,'WindowStyle'),'modal')
    set(BoxHandle,'WindowStyle','modal');
end

set(BoxHandle,'Position',DefFigPos);
set(OKHandle,'Position',[OKXOffset OKYOffset OKWidth OKHeight]);  
  
set(OKHandle,'Units','pixels');
BtnPos = get(OKHandle,'Position');
set(OKHandle,'Units','points');
h = uicontrol(BoxHandle,'BackgroundColor', 'k', ...
              'Style','frame','Position',[ BtnPos(1)-1 BtnPos(2)-1 BtnPos(3)+2 BtnPos(4)+2 ]);
uistack(h,'bottom')

delete(MsgHandle);
AxesHandle=axes('Parent',BoxHandle,'Position',[0 0 1 1],'Visible','off');

MsgHandle=text( ...
    'Parent'              ,AxesHandle                        , ...
    'Units'               ,'points'                          , ...
    'Color'               ,get(OKHandle,'ForegroundColor')   , ...
    Font                  , ...
    'HorizontalAlignment' ,'left'                            , ...
    'VerticalAlignment'   ,'bottom'                          , ...
    'Position'            ,[ MsgTxtXOffset MsgTxtYOffset+5 0], ...
    'String'              ,WrapString                        , ...
    'Interpreter'         ,Interpreter                       , ...
    'Tag'                 ,'MessageBox'                        ...
    );


if ~strcmp(IconString,'none'),
  IconAxes=axes(                                            ...
               'Parent'          ,BoxHandle               , ...
               'Units'           ,'points'                , ...
               'Position'        ,[IconXOffset IconYOffset ...
                                   IconWidth IconHeight]  , ...
               'Tag'             ,'IconAxes'                ...
               );         
 

  if ~strcmp(IconString,'custom'),IconCMap=[Black;FigColor];end
 
 load dialogicons.mat
 if strcmp('warn',IconString),
            IconData=warnIconData;
            warnIconMap(256,:)=get(BoxHandle,'color');
            IconCMap=warnIconMap;
    
  elseif strcmp('help',IconString),
            IconData=helpIconData;
            helpIconMap(256,:)=get(BoxHandle,'color');
            IconCMap=helpIconMap;

  elseif strcmp('error',IconString),
            IconData=errorIconData;
            errorIconMap(146,:)=get(BoxHandle,'color');
            IconCMap=errorIconMap;
  end
  
  Img=image('CData',IconData,'Parent',IconAxes);  
  set(BoxHandle, 'Colormap', IconCMap);
  set(IconAxes          , ...
      'XLim'            ,get(Img,'XData')+[-0.5 0.5], ...
      'YLim'            ,get(Img,'YData')+[-0.5 0.5], ...
      'Visible'         ,'off'                      , ...
      'YDir'            ,'reverse'                    ...      
      );
  
end % if ~strcmp

% make sure we are on screen
movegui(BoxHandle)

set(BoxHandle,'HandleVisibility','callback','Visible','on');

% make sure the window gets drawn even if we are in a pause
drawnow


if nargout==1,varargout{1}=BoxHandle;end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%% InternalCreateFlag %%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function [Flag,CreateMode,Interpreter]=InternalCreateFlag(String)

Flag=0;
CreateMode='non-modal';
Interpreter='none';

if iscell(String),String=String{:};end
if isstr(String),
  if strcmp(String,'non-modal') | strcmp(String,'modal') | ...
     strcmp(String,'replace'),
    Flag=1;
    CreateMode=String;
  end
elseif isstruct(String),
  Flag=1;
  CreateMode=String.WindowStyle;
  Interpreter=String.Interpreter;
end  

function doKeyPress(obj, evd)
switch(evd.Key)
 case {'return','space','escape'}
  delete(gcbf);
end

function figure_size = getnicedialoglocation(figure_size, figure_units)
% adjust the specified figure position to fig nicely over GCBF
% or into the upper 3rd of the screen

%  Copyright 1999-2002 The MathWorks, Inc.
%  $Revision: 1.1.6.1 $

parentHandle = gcbf;
propName = 'Position';
if isempty(parentHandle)
    parentHandle = 0;
    propName = 'ScreenSize';
end

old_u = get(parentHandle,'Units');
set(parentHandle,'Units',figure_units);
container_size=get(parentHandle,propName);
set(parentHandle,'Units',old_u);

figure_size(1) = container_size(1)  + 1/2*(container_size(3) - figure_size(3));
figure_size(2) = container_size(2)  + 2/3*(container_size(4) - figure_size(4));

% topoplot()   - plot a topographic map of an EEG field as a 2-D
%                circular view (looking down at the top of the head) 
%                 using cointerpolation on a fine cartesian grid.
% Usage:
%        >>  topoplot(datavector,'eloc_file','acquisition_type');
%        >>  topoplot(datavector,'eloc_file','acquisition_type', 'Param1','Value1', ...)
% Inputs:
%    datavector = vector of values at the corresponding locations.
%   'eloc_file' = name of an EEG electrode position file {0 -> 'chan_file'}
%   'acquisition_type' = 'EEG' or 'ECoG'
%
% Optional Parameters & Values (in any order):
%                  Param                         Value
%                'colormap'         -  any sized colormap
%                'interplimits'     - 'electrodes' to furthest electrode
%                                     'outline' to edge of outline
%                                        {default 'outline'}
%                'gridscale'        -  scaling grid size {default 67}
%                'maplimits'        - 'absmax' +/- the absolute-max 
%                                     'maxmin' scale to data range
%                                     [clim1,clim2] user-definined lo/hi
%                                        {default = 'absmax'}
%                'style'            - 'straight' colormap only
%                                     'contour' contour lines only
%                                     'both' both colormap and contour lines
%                                     'fill' constant color between lines
%                                     'blank' just outline and electrodes
%                                        {default = 'both'}
%                'numcontour'       - number of contour lines
%                                        {default = 6}
%                'shading'          - 'flat','interp'  {default = 'flat'}
%                'outlinecolor'        - Color of outline {default black}
%                'electrodes'       - 'on','off','labels','numbers'
%                'efontsize','electcolor','emarker','emarkersize' - details
%
% Note: topoplot() only works when map limits are >= the max and min 
%                                     interpolated data values.
% Eloc_file format:
%         chan_number degrees radius reject_level amp_gain channel_name
%        (Angle-0 = Cz-to-Fz; C3-angle =-90; Radius at edge of image = 0.5)
%
%       For a sample eloc file:     >> topoplot('example')
%
% Note: for acquisition type EEG, topoplot() will ignore any electrode with a position outside 
%       the head (radius > 0.5)

% Topoplot Version 2.1

% Begun by Andy Spydell, NHRC,  7-23-96
% 8-96 Revised by Colin Humphries, CNL / Salk Institute, La Jolla CA
%   -changed surf command to imagesc (faster)
%   -can now handle arbitrary scaling of electrode distances
%   -can now handle non integer angles in eloc_file
% 4-4-97 Revised again by Colin Humphries, reformat by SM
%   -added parameters
%   -changed eloc_file format
% 2-26-98 Revised by Colin
%   -changed image back to surface command
%   -added fill and blank styles
%   -removed extra background colormap entry (now use any colormap)
%   -added parameters for electrode colors and labels
%   -now each topoplot axes use the caxis command again.
%   -removed OUTPUT parameter
% 3-11-98 changed default emarkersize, improve help msg -sm

function handle = topoplot(Vl,loc_file,acq_type,varargin)
%do initialization
if strcmpi(acq_type, 'eeg')
  RMAX=0.5;
elseif strcmpi(acq_type, 'ecog')
  % limits for topo display
  RMAX=1;
else
  error('topoplot(); acq_type must be either ''eeg'' or ''ecog''');
end

% User Defined Defaults:
MAXCHANS = 256;
DEFAULT_ELOC = 'eloc64.txt';
INTERPLIMITS = 'outline';  % head, electrodes
MAPLIMITS = 'absmax';   % absmax, maxmin, [values]
GRID_SCALE = 67;
CONTOURNUM = 6;
STYLE = 'both';       % both,straight,fill,contour,blank
HCOLOR = [0 0 0];
ECOLOR = [0 0 0];
CONTCOLOR = [0 0 0];
ELECTROD = 'on';      % ON OFF LABEL
EMARKERSIZE = 6;
EFSIZE = get(0,'DefaultAxesFontSize');
HLINEWIDTH = 2;
EMARKER = '.';
SHADING = 'flat';     % flat or interp

%%%%%%%%%%%%%%%%%%%%%%%
nargs = nargin;
if nargs < 2
  loc_file = DEFAULT_ELOC;
end
if nargs == 1
  if isstr(Vl)
    if any(strcmp(lower(Vl),{'example','demo'}))
      fprintf(['This is an example of an electrode location file,\n',...
               'an ascii file consisting of the following four columns:\n',...
               ' channel_number degrees arc_length channel_name\n\n',...
               'Example:\n',...
               ' 1               -18    .352       Fp1.\n',...
               ' 2                18    .352       Fp2.\n',...
               ' 5               -90    .181       C3..\n',...
               ' 6                90    .181       C4..\n',...
               ' 7               -90    .500       A1..\n',...
               ' 8                90    .500       A2..\n',...
               ' 9              -142    .231       P3..\n',...
               '10               142    .231       P4..\n',...
               '11                 0    .181       Fz..\n',...
               '12                 0    0          Cz..\n',...
               '13               180    .181       Pz..\n\n',...
               'The model head sphere has a diameter of 1.\n',...
               'The vertex (Cz) has arc length 0. Channels with arc \n',...
               'lengths > 0.5 are not plotted nor used for interpolation.\n'...
               'Zero degrees is towards the nasion. Positive angles\n',...
               'point to the right hemisphere; negative to the left.\n',...
               'Channel names should each be four chars, padded with\n',...
               'periods (in place of spaces).\n'])
      return

    end
  end
end
if isempty(loc_file)
  loc_file = 0;
end
if ~ischar(loc_file) && ~isstruct(loc_file)
  loc_file = DEFAULT_ELOC;
end

if nargs > 2
  if ~(round(nargs/2) ~= nargs/2)
    error('topoplot(): Even number of inputs?')
  end
  for i = 4:2:nargs
    Param = varargin{i-3}; %eval(['p',int2str((i-3)/2 +1)]);
    Value = varargin{i-2}; %eval(['v',int2str((i-3)/2 +1)]);
    if ~isstr(Param)
      error('topoplot(): Parameter must be a string')
    end
    Param = lower(Param);
    switch lower(Param)
      case 'colormap'
        if size(Value,2)~=3
          error('topoplot(): Colormap must be a n x 3 matrix')
        end
        colormap(Value)
      case {'interplimits','headlimits'}
        if ~isstr(Value)
          error('topoplot(): interplimits value must be a string')
        end
        Value = lower(Value);
        if ~strcmp(Value,'electrodes') & ~strcmp(Value,'outline')
          error('topoplot(): Incorrect value for interplimits')
        end
        INTERPLIMITS = Value;
      case 'maplimits'
        MAPLIMITS = Value;
      case 'gridscale'
        GRID_SCALE = Value;
      case 'style'
	STYLE = lower(Value);
      case 'numcontour'
        CONTOURNUM = Value;
      case 'electrodes'
	ELECTROD = lower(Value);
      case 'emarker'
	EMARKER = Value;
      case {'outlinecolor','hcolor'}
	HCOLOR = Value;
      case {'electcolor','ecolor'}
	ECOLOR = Value;
      case {'emarkersize','emsize'}
	EMARKERSIZE = Value;
      case {'efontsize','efsize'}
	EFSIZE = Value;
      case 'shading'
	SHADING = lower(Value);
	if ~any(strcmp(SHADING,{'flat','interp'}))
	  error('Invalid Shading Parameter')
	end
      otherwise
	error('Unknown parameter.')
    end
  end
end

[r,c] = size(Vl);
if r>1 & c>1,
  error('topoplot(): data should be a single vector\n');
end

if isa(loc_file, 'struct')
  %data specified as electrode structure, translate to data structure used here
  x = zeros(length(loc_file), 1);
  y = x;
  labels = repmat(' ', length(loc_file), 4);
  for idxElec = 1:length(loc_file)
    if( isempty( loc_file(idxElec).coords ) )
      x(idxElec) = 100;
      y(idxElec) = 100;
    else
      x(idxElec) = loc_file(idxElec).coords(2);
      y(idxElec) = loc_file(idxElec).coords(1);
    end
    labels(idxElec, 1:length(loc_file(idxElec).label)) = loc_file(idxElec).label;
  end
else

  fid = fopen(loc_file);
  if fid<1,
    fprintf('topoplot(): cannot open eloc_file (%s).\n',loc_file);
    return
  end
  A = fscanf(fid,'%d %f %f %s',[7 MAXCHANS]);
  fclose(fid);

  A = A';

  if length(Vl) ~= size(A,1),
   fprintf(...
     'topoplot(): data vector must have the same rows (%d) as eloc_file (%d)\n',...
                 length(Vl),size(A,1));
   A
   error('');
  end

  labels = setstr(A(:,4:7));
  idx = find(labels == '.');                       % some labels have dots
  labels(idx) = setstr(abs(' ')*ones(size(idx)));  % replace them with spaces

  Th = pi/180*A(:,2);                              % convert degrees to radians
  Rd = A(:,3);
  if strcmpi(acq_type, 'eeg')
    ii = find(Rd <= 0.5);                     % interpolate on-head channels only
    Th = Th(ii);
    Rd = Rd(ii);
    Vl = Vl(ii);
    labels = labels(ii,:);
  end

  [x,y] = pol2cart(Th,Rd);      % transform from polar to cartesian coordinates
end

%normalize values for ecog
if strcmpi(acq_type, 'ecog')
  normVal = max(max(abs([x,y]))) * RMAX * 1.1;
  x = x /normVal;
  y = y /normVal;
end
  
ha = gca;
cla
hold on

if ~strcmp(STYLE,'blank')
  % find limits for interpolation
  if strcmp(INTERPLIMITS,'outline')
    xmin = -RMAX;
    xmax = RMAX;
    
    ymin = -RMAX;
    ymax = RMAX;
  else
    xmin = min(x); 
    xmax = max(x);

    ymin = min(y); 
    ymax = max(y);
  end
  xi = linspace(xmin,xmax,GRID_SCALE);   % x-axis description (row vector)
  yi = linspace(ymin,ymax,GRID_SCALE);   % y-axis description (row vector)
  
  [Xi,Yi,Zi] = griddata(y,x,Vl,yi',xi,'v4'); % Interpolate data
  
  % Exclude data outside the grid bounds
  if strcmpi(acq_type, 'eeg')
    mask = (sqrt(Xi.^2+Yi.^2) <= RMAX);
  else
    mask = ((Xi <= RMAX & Xi >= -RMAX) & (Yi <= RMAX & Yi >= -RMAX));
  end
  ii = find(mask == 0);
  Zi(ii) = NaN;
  
  % calculate colormap limits
  m = size(colormap,1);
  if isstr(MAPLIMITS)
    if strcmp(MAPLIMITS,'absmax')
      amin = -max(max(abs(Zi)));
      amax = max(max(abs(Zi)));
    elseif strcmp(MAPLIMITS,'maxmin')
      amin = min(min(Zi));
      amax = max(max(Zi));
    end
  else
    amin = MAPLIMITS(1);
    amax = MAPLIMITS(2);
  end
  delta = xi(2)-xi(1); % length of grid entry
  
  % Draw topoplot on head
  if strcmp(STYLE,'contour')
    contour(Xi,Yi,Zi,CONTOURNUM,'k');
  elseif strcmp(STYLE,'both')
    surface(Xi-delta/2,Yi-delta/2,zeros(size(Zi)),Zi,'EdgeColor','none',...
	'FaceColor',SHADING);
    contour(Xi,Yi,Zi,CONTOURNUM,'k');
  elseif strcmp(STYLE,'straight')
    surface(Xi-delta/2,Yi-delta/2,zeros(size(Zi)),Zi,'EdgeColor','none',...
	'FaceColor',SHADING);
  elseif strcmp(STYLE,'fill')
    contourf(Xi,Yi,Zi,CONTOURNUM,'k');
  else
    error('Invalid style')
  end
  caxis([amin amax]) % set coloraxis
end

l = 0:2*pi/100:2*pi;
if strcmpi(acq_type, 'eeg')
  %plot ears, nose and head
  basex = .18*RMAX;  
  tip = RMAX*1.15; base = RMAX-.004;

  set(ha,'Xlim',[-RMAX*1.3 RMAX*1.3],'Ylim',[-RMAX*1.3 RMAX*1.3])
  EarX = [.497 .510 .518 .5299 .5419 .54 .547 .532 .510 .489];
  EarY = [.0555 .0775 .0783 .0746 .0555 -.0055 -.0932 -.1313 -.1384 -.1199];
  
  plot(cos(l).*RMAX,sin(l).*RMAX,...
      'color',HCOLOR,'Linestyle','-','LineWidth',HLINEWIDTH);

  plot([.18*RMAX;0;-.18*RMAX],[base;tip;base],...
      'Color',HCOLOR,'LineWidth',HLINEWIDTH);

  plot(EarX,EarY,'color',HCOLOR,'LineWidth',HLINEWIDTH)
  plot(-EarX,EarY,'color',HCOLOR,'LineWidth',HLINEWIDTH)   
else
  
  %set(ha,'Xlim',[-RMAXX RMAXX],'Ylim',[-RMAXY RMAXY])
end
axis tight;
axis square;

% Plot Electrodes
if strcmp(ELECTROD,'on') 
  hp2 = plot(y,x,EMARKER,'Color',ECOLOR,'markersize',EMARKERSIZE);
elseif strcmp(ELECTROD,'labels')
  for i = 1:size(labels,1)
    text(y(i),x(i),labels(i,:),'HorizontalAlignment','center',...
	'VerticalAlignment','middle','Color',ECOLOR,...
	'FontSize',EFSIZE)
  end
elseif strcmp(ELECTROD,'numbers')
whos y x 
  for i = 1:size(labels,1)
    text(y(i),x(i),int2str(i),'HorizontalAlignment','center',...
	'VerticalAlignment','middle','Color',ECOLOR,...
	'FontSize',EFSIZE)
  end
end

   

hold off
axis off


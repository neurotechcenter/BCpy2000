%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% $Id: elocVisECoG.m 2007-111-26 12:31:37EST fialkoff $ 
%% 
%% File: elocVisECoG.m 
%% 
%% Author: Joshua Fialkoff <fialkj@rpi.edu>, Gerwin Schalk <schalk@wadsworth.org>
%%
%% Description: This function generates a visualization plot of an ECoG
%% electrode location file.
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
function [elecInfo] = elocVisECoG(params, loc_file)

error(nargchk(1,2,nargin));

% User Defined Defaults:
MAXCHANS = 256;

elecInfo = struct('elecNum', [], 'label', '', 'coords', [], 'markerTextHandle', []);

%%%%%%%%%%%%%%%%%%%%%%%
if nargin > 1
  fid = fopen(loc_file);
  if fid<1,
    error('Specified eloc file cannot be found');
    return;
  end
  A = fscanf(fid,'%d %f %f %s',[7 MAXCHANS]);
  fclose(fid);

  A = A';

  labels = setstr(A(:,4:7));
  idx = find(labels == '.');                       % some labels have dots
  labels(idx) = setstr(abs(' ')*ones(size(idx)));  % replace them with spaces

  elecNums = A(:,1);
  Th = pi/180*A(:,2);                              % convert degrees to radians
  Rd = A(:,3);

  [y,x] = pol2cart(Th,Rd);      % transform from polar to cartesian coordinates

  %normalize 
  normVal = max(max(abs([x,y])));
  x = x /normVal;
  y = y /normVal;
  
  for idx = 1:length(x)
    elecInfo(idx).elecNum = elecNums(idx);
    elecInfo(idx).coords = [x(idx) y(idx)];
    elecInfo(idx).label = strtrim(labels(idx, :));
  end
  ha = gca;
  cla
  
end

hold on

% Draw grid
gridX = [params.ecogCoordRange params.ecogCoordRange([2 1])];
gridY(1:2) = params.ecogCoordRange(2);
gridY(3:4) = params.ecogCoordRange(1);

fill(gridX, gridY, params.headFillColor);

% Draw grid outline
% plot(gridX, gridY,'color',params.headOutlineColor,'LineWidth',params.headOutlineWidth);
% plot(gridX(4:1), gridY(4:1),'color',params.headOutlineColor,'LineWidth',params.headOutlineWidth);

if nargin > 1
  % Plot Electrodes
  for idx = 1:size(labels,1)
    [markerTextHandle] = addTopoMarker(x(idx), y(idx), int2str(elecInfo(idx).elecNum), params);
    elecInfo(idx).markerTextHandle = markerTextHandle;
  end
end

hold off
axis tight
axis square
axis off


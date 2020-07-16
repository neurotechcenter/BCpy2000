%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% $Id: UGenericSignal.m 2007-111-26 12:31:37EST fialkoff $ 
%% 
%% File: UGenericSignal.m 
%% 
%% Author: Joshua Fialkoff <fialkj@rpi.edu>, Gerwin Schalk <schalk@wadsworth.org>
%%
%% Description: This function generates an electrode location file
%% compatible with topoplot and topoplotEEG.
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
function success = exportOfflineElocFile(elecData, fn, settings)
success = 0;

fid = fopen(fn, 'w+');
if fid == -1
  return;
end

for idxElec = 1:length(elecData)
  %0 degrees is towards the nose, so reverse coordinates in the function
  [theta rad] = cart2pol(elecData(idxElec).coords(2), ...
    elecData(idxElec).coords(1));
  theta = theta * 180/pi;
  %trim the label and check to make sure it has the right number of
  %characters
  label  = regexprep(elecData(idxElec).label, ...
    settings.strCleanUpRegexrep, '$1');
  if length(label) > settings.labelMaxLen
    label = label(1:settings.labelMaxLen);
  elseif length(label) < settings.labelMaxLen
    while length(label) < settings.labelMaxLen
      label(end+1) = settings.labelPadChar;
    end
  end
  fprintf(fid, '%d %.4f %.4f %s\r\n', idxElec, theta, rad, label);
end

fclose(fid);
success = 1;
return;

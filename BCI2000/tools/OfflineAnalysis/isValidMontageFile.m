%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% $Id: UGenericSignal.m 2007-111-26 12:31:37EST fialkoff $ 
%% 
%% File: UGenericSignal.m 
%% 
%% Author: Joshua Fialkoff <fialkj@rpi.edu>, Gerwin Schalk <schalk@wadsworth.org>
%%
%% Description: This file checks the format of and values specified within
%% a given montage file in order to make sure the file is in the approved
%% format.
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
function [isValid numChans] = isValidMontageFile(fn, settings, acqType)
error(nargchk(3,3,nargin));

isValid = 1;
numChans = 0;

pat = '^\s*(?<idx>\d+)\s+(?<theta>-?\d+\.?\d*)\s+(?<r>\d+\.?\d*)\s+(?<lab>\S+)$';
fid=fopen(fn);
if fid == -1
  isValid = 0;
  return;
end
while 1
  tline = fgetl(fid);
  if ~ischar(tline)
      break;
  end

  [lineData tmp errorMsg] = sscanf(tline, '%d %f %f %s', 7);

  if size(lineData, 2) > 1 || ~isempty(errorMsg)
    isValid = 0;
    break;
  elseif isempty(errorMsg) && ~isempty(lineData) %skips blank lines
    numChans = numChans + 1;

    theta = lineData(2);
    r = lineData(3);

    if theta > 360 || theta < -360 || r < 0 || ...
        (strcmp(acqType, 'eeg') && r > settings.headRadius)
      isValid = 0;
      break;
    end      

  end
end
fclose(fid);

return;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% $Id: getAnalysisParams.m 2007-111-26 12:31:37EST fialkoff $ 
%% 
%% File: getAnalysisParams.m 
%% 
%% Author: Joshua Fialkoff <fialkj@rpi.edu>, Gerwin Schalk <schalk@wadsworth.org>
%%
%% Description: This function returns an empty analysis params structure.
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
function analysisParams = getAnalysisParams(domain, acqMode, dataFiles)
% getAnalysisParams     returns an empty structure for use with
% runBasicAnalysis.  The dataFile field is set equal to the value of the
% dataFile input argument

error(nargchk(0,3,nargin));


analysisParams = struct();
switch nargin
case 3
  if iscell(dataFiles)
    analysisParams.dataFiles = dataFiles;
  else
    analysisParams.dataFiles = {dataFiles};
  end
case 2
  analysisParams.dataFiles = {};
case 1
  analysisParams.dataFiles = {};
  acqMode = '';
case 0
  analysisParams.dataFiles = {};
  acqMode = '';
  domain = '';
end

analysisParams.targetConditions = {};
analysisParams.conditionLabels = {};
analysisParams.trialChangeCondition = '';
analysisParams.spatialFilt = '';
analysisParams.montageFile = '';
analysisParams.domain = domain;
analysisParams.acqType = acqMode;
analysisParams.channels = [];
analysisParams.topoParams = [];
  

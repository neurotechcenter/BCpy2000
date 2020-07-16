function [ parameters, states ] = bci_Construct

% Filter construction demo
% 
% Perform any initialization; request BCI2000 parameters and states
% by returning parameter and state definition lines as demonstrated
% below.

% BCI2000 filter interface for Matlab
% juergen.mellinger@uni-tuebingen.de, 2005
% $BEGIN_BCI2000_LICENSE$
% 
% This file is part of BCI2000, a platform for real-time bio-signal research.
% [ Copyright (C) 2000-2012: BCI2000 team and many external contributors ]
% 
% BCI2000 is free software: you can redistribute it and/or modify it under the
% terms of the GNU General Public License as published by the Free Software
% Foundation, either version 3 of the License, or (at your option) any later
% version.
% 
% BCI2000 is distributed in the hope that it will be useful, but
%                         WITHOUT ANY WARRANTY
% - without even the implied warranty of MERCHANTABILITY or FITNESS FOR
% A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
% 
% You should have received a copy of the GNU General Public License along with
% this program.  If not, see <http://www.gnu.org/licenses/>.
% 
% $END_BCI2000_LICENSE$

parameters = { ...
  [ 'MatlabDemo list MyFilterOffsets= 3 0 0 0 0 0 0 ' ...
    '// A list of offsets added to the signal' ] ...
  [ 'MatlabDemo matrix MyFilterMatrix= 3 2 1 2 3 4 5 6 0 0 0 ' ...
    '// A demo filter matrix' ] ...
  [ 'MatlabDemo int MyRunCount= 1 0 0 0 ' ...
    '// A parameter modified by Matlab' ] ...
  [ 'MatlabDemo int MyReportBogusError= 1 % % % ' ...
    '// Report a bogus error (boolean)' ] ...
};
states = { ...
  'MyDemoState 4 0 0 0' ...
};

function out_signal = bci_Process( in_signal )

% Filter process demo
% 
% Apply a filter to in_signal, and return the result in out_signal.
% Signal dimensions are ( channels x samples ).

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

% Parameters and states are global variables.
global bci_Parameters bci_States;
% Access to multi-sample states is read-only -- use bci_States to set per-block states.
global bci_StateSamples;

% We use a global variable to store our filter's configuration.
global myFilterMatrix myFilterOffsets;

out_signal = myFilterMatrix * in_signal;
for( i = 1:size( out_signal, 2 ) )
  out_signal( :, i ) = out_signal( :, i ) + myFilterOffsets;
end

% Run the source module with --LogMouse=1 in order to see mouse movements.
global myMouseY;
if( isfield( bci_StateSamples, 'MousePosY' ) )
  myMouseY = [myMouseY bci_StateSamples.MousePosY];
  if( length( myMouseY ) > 500 )
    myMouseY = myMouseY( end-500:end );
  end;
  plot( myMouseY );
end

% For demonstration purposes, set the demo state:
bci_States.MyDemoState = randi(12);

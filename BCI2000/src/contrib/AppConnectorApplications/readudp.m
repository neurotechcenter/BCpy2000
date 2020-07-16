function readudp
% Demo showing how to access AppConnector and P3Speller information through
% a UDP socket from Matlab.
%
% This demo is part of the BCI2000 project.
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
% $Id: readudp.m 3798 2012-02-01 18:07:06Z mellinger $

ip = 'localhost';
port = 20320;
% Create and open a UDP object that connects to BCI2000.
u = udp( ip, 20319, 'LocalPort', port, 'Terminator', 'CR/LF', 'Timeout', 10 );
fopen( u ); 
% Read data until timeout occurs.
s = fgetl( u );
while( s~=-1 ) 
  fprintf( '%s', s ); 
  s = fgetl( u );
end 
% Close and delete the UDP object.
fclose( u ); 
delete( u ); 

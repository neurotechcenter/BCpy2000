function [ signal, varargout ] = load_bcimat( filename, varargin )
%LOAD_BCIMAT Load a matlab file written by bci_stream2mat.
%
%  [ signal, state1, state2, ... ] =
%     load_bcimat( 'filename', output_dimensions, 'state1', 'state2', ... )
%
%  loads signal and state data from the file given in the first argument.
%
%  In the second argument, specify the number of dimensions that you want
%  the signal output variable to have; use 2 for EEG-like data (samples
%  by channels) and 3 for spectral data (blocks by bins by channels).
%  The default is 3.
%
%  Remaining input arguments are treated as BCI2000 state names; associated 
%  state data will be copied into the variables specified as remaining 
%  output arguments.
%  State variables will always be one-dimensional, with their number of 
%  entries matching the first dimension of the signal variable.
%  
%  This file is a supplement to the bci_stream2mat command line tool which
%  is part of the BCI2000 project (http://www.bci2000.org).
%
%  $Id: load_bcimat.m 3798 2012-02-01 18:07:06Z mellinger $
%  Author: juergen.mellinger@uni-tuebingen.de
%  $BEGIN_BCI2000_LICENSE$
%  
%  This file is part of BCI2000, a platform for real-time bio-signal research.
%  [ Copyright (C) 2000-2012: BCI2000 team and many external contributors ]
%  
%  BCI2000 is free software: you can redistribute it and/or modify it under the
%  terms of the GNU General Public License as published by the Free Software
%  Foundation, either version 3 of the License, or (at your option) any later
%  version.
%  
%  BCI2000 is distributed in the hope that it will be useful, but
%                          WITHOUT ANY WARRANTY
%  - without even the implied warranty of MERCHANTABILITY or FITNESS FOR
%  A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
%  
%  You should have received a copy of the GNU General Public License along with
%  this program.  If not, see <http://www.gnu.org/licenses/>.
%  
%  $END_BCI2000_LICENSE$


if( nargin < 1 )
  error( 'No file name given.' );
  return;
else
  if( nargin < 2 )
    outdim = 3;
    num_states = 0;
  else
    outdim = varargin{ 1 };
    num_states = nargin - 2;
  end
end
if( num_states ~= nargout - 1 )
  error( 'There must be an output argument for the signal, and one for each state argument.' );
  return;
end

filevars = who( '-file', filename );
if( isempty( find( ismember( filevars, 'Data' ) == 1, 1 ) ) || isempty( find( ismember( filevars, 'Index' ) == 1, 1 ) ) )
  error( 'Input file is not a bci_stream2mat file.' );
  return;
end
load( filename );

switch outdim
  case 2
    signal = zeros( size( Index.Signal, 2 ) * size( Data, 2 ), size( Index.Signal, 1 ), 'single' );
    for( i = 1 : size( Index.Signal, 1 ) )
      signal( :, i ) = reshape( Data( Index.Signal( i, : ), : ), [], 1 );
    end  
    for( i = 1 : num_states )
      state = [];
      idx = eval( [ 'Index.' varargin{ i + 1 } ] ); 
      for( j = 1 : size( Index.Signal, 2 ) );
        state = [ state; Data( idx, : ) ];
      end
      varargout{ i } = reshape( state, [], 1 );
    end

  case 3
    signal = zeros( size( Data, 2 ), ...
      size( Index.Signal, 2 ), size( Index.Signal, 1 ), 'single' );
    for( i = 1 : size( Index.Signal, 1 ) )
      signal( :, :, i ) = reshape( Data( Index.Signal( i, : ).', : ).', ...
        [], size( Index.Signal, 2 ), 1 );
    end
    for( i = 1 : num_states )
      idx = eval( [ 'Index.' varargin{ i + 1 } ] ); 
      varargout{ i } = squeeze( Data( idx, : ).' );
    end
    
  otherwise
    error( 'Unsupported number of output dimensions.' );
end

function dll_example
% Example illustrating the usage of a BCI2000 filter dll from within
% MATLAB.
% $Id: dll_example.m 3798 2012-02-01 18:07:06Z mellinger $
% Author: juergen.mellinger@uni-tuebingen.de
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
%
% This example shows how to use a BCI2000 filter compiled as a dll when
% processing data from Matlab.
% Because Matlab is the caller of the BCI2000 filter, it must play the role
% of the BCI2000 framework in regard to the filter (or multiple filters).
% This means
% 1) providing state list information using the DLL's PutState()
%    function;
% 1) instantiating the filter by calling the DLL's Instantiate() function;
% 2) providing state vector data as read, e.g., from a BCI2000 data file 
%    using SetStatevector(), or setting states by their names using 
%    SetState();
% 3) providing parameter information from a data file or from PutParameter();
% 4) initializing the filter -- Preflight(), Initialize(), StartRun();
% 5) having the filter process data -- Process();
% 6) de-initializing the filter -- StopRun(), Dispose().
%
% The call_bcidll function below is a wrapper to Matlab's callib() function 
% that takes care of error handling. 
%
% For further information about BCI2000 filters in general and especially
% the semantics of a BCI2000 filter's member functions, see the
% filter interface discussion in section 6.6 of the Software Design 
% Document at BCI2000/doc/implementation.pdf.
% For further information about the BCI2000 dll interface, see the
% bci_dll.h header file.

filter_name = 'TransmissionFilter';
loadlibrary( filter_name, 'bci_dll', 'alias', 'lib' );
libfunctions( 'lib', '-full' );
try
  fprintf( 1, 'Filter Info:\n%s', calllib( 'lib', 'GetInfo' ) );
  % Add a state to the list of states.
  call_bcidll( 'lib', 'PutState', 'TestState 16 3 0 0' );
  % Instantiate the filter.
  call_bcidll( 'lib', 'Instantiate' );
  % Try reading and writing binary state vector information.
  % Note that binary state vectors from different DLLs are incompatible
  % unless they received identical state information with PutState()
  % before calling the DLL's Instantiate() function.
  statevectorLength = call_bcidll( 'lib', 'GetStatevectorLength', 0 );
  statevectorData = libpointer( 'uint8Ptr', uint8( zeros( statevectorLength, 1 ) ) );
  call_bcidll( 'lib', 'GetStatevector', statevectorData );
  statevector = get( statevectorData, 'Value' )
  call_bcidll( 'lib', 'SetStatevector', statevectorData );
  
  % Add some parameters to the parameter list.
% call_bcidll( 'lib', 'PutParameter', 'this is not a valid parameter line' );
  call_bcidll( 'lib', 'PutParameter', 'Source int SourceCh= 3 0 0 0 //' );
  call_bcidll( 'lib', 'PutParameter', 'Source intlist TransmitChList= 2 2 1 0 0 0 //' );

  % Call Preflight() to obtain signal output dimensions.
  inputSignal = [ 1 2 3 4; 5 6 7 8; 9 10 11 12 ]
  inputChannels = size( inputSignal, 1 );
  inputElements = size( inputSignal, 2 );
  [ outputChannels, outputElements, outputProperties ] = ...
    call_bcidll( 'lib', 'Preflight', inputChannels, inputElements, {''} );
  outputProperties{1}
  outputSignal = zeros( outputChannels, outputElements );
  % Initialize the DLL prior to calling Process().
  call_bcidll( 'lib', 'Initialize' );
  call_bcidll( 'lib', 'StartRun' );
  % Set a state value.
  call_bcidll( 'lib', 'SetStateValue', 'TestState', 65535 );
  % Create pointers to input and output data.
  input = libpointer( 'doublePtr', inputSignal );
  output = libpointer( 'doublePtr', outputSignal );
  % Call Process() and read the output signal.
  call_bcidll( 'lib', 'Process', input, output );
  outputSignal = get( output, 'Value' )
  % Read a state value.
  [ stateName, value ] = call_bcidll( 'lib', 'GetStateValue', 'TestState', 0 )
  % Finish processing.
  call_bcidll( 'lib', 'StopRun' );
  % Reset the DLL's internal state.
  call_bcidll( 'lib', 'Dispose' );
catch
  fprintf( 1, '%s\n', lasterr );
end
unloadlibrary( 'lib' );

end % function

function varargout = call_bcidll( libname, funcname, varargin )
% Call a function from a BCI2000 filter DLL, and report errors if any.
result = cell( nargout + 1, 1 ); 
[ result{:} ] = calllib( libname, funcname, varargin{:} ); 
if( ~result{1} )
  error( 'Error calling %s:\n%s', funcname, calllib( libname, 'GetError' ) );
end
for( k = 1 : nargout )
  varargout{k} = result{k + 1};
end
end % function

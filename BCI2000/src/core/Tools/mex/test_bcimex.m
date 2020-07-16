function test_bcimex
% The text_bcimex function is part of the BCI2000 project.
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

%  Matlab M-file to test BCI2000 Matlab mex files.
%  $Id$

TESTFILE = 'testdata';
tmpdatfile = [tempname '.dat'];


success = true;
try
  for n = 1:2
    if n == 2
      fprintf( 1, 'Repeating tests to detect improper initialization issues ...\n' );
    end
    fprintf( 1, 'Testing load_bcidat ... ' );
    [signal, states, parameters] = load_bcidat( [TESTFILE '.dat'] );
    ref = load( [TESTFILE '.mat'] );
    if( any( signal ~= ref.signal ) )
      error( 'Signal data mismatch' );
    end
    if( ~equal_structs( states, ref.states ) )
      error( 'State vector data mismatch' );
    end
    if( ~equal_structs( parameters, ref.parameters ) )
      error( 'Parameter mismatch' );
    end
    fprintf( 1, 'OK\n' );

    fprintf( 1, 'Testing convert_bciprm ... ' );
    if( ~equal_structs( parameters, convert_bciprm( convert_bciprm( parameters ) ) ) )
      error( 'Mismatch when converting forth and back' );
    end
    fprintf( 1, 'OK\n' );

    fprintf( 1, 'Testing save_bcidat ... ' );
    save_bcidat( tmpdatfile, signal, states, parameters );
    [signal, states, parameters] = load_bcidat( tmpdatfile );  
    if( any( signal ~= ref.signal ) )
      error( 'Signal data mismatch' );
    end
    if( ~equal_structs( states, ref.states ) )
      error( 'State vector data mismatch' );
    end
    if( ~equal_structs( parameters, ref.parameters ) )
      error( 'Parameter mismatch' );
    end  
    fprintf( 1, 'OK\n' );

    fprintf( 1, 'Testing mem ... ' );
    spectrum_ = mem( double( signal ), [16, 0, 0.4, 0.02, 15, 0] );
    if( any( spectrum_ ~= ref.spectrum_ ) )
      rel_err = sqrt( norm( spectrum_ - ref.spectrum_, 'fro' ) / norm( spectrum_, 'fro' ) );
      fprintf( 1, '\nWarning: Mismatch between computed spectra (relative error is %d)\n', rel_err );
      err_limit = 1e-6;
      if( rel_err > err_limit )
        error( 'Mismatch between computed spectra exceeds relative error limit of %f\n', err_limit );
      end
    end
    fprintf( 1, 'OK\n' );

  end
  delete( tmpdatfile );
  fprintf( 1, 'Unloading mex files to avoid stale executables in Matlab memory ... ' );
  clear load_bcidat convert_bciprm save_bcidat mem;
  fprintf( 1, 'OK\n' );
catch err
  success = false;
end
if( success )
  fprintf( 1, 'Mex files tested OK.\n' );
else
  fprintf( 1, 'Error: %s.\n', err.message );
end

% A helper function to test structs for equality.
function result = equal_structs( inStruct1, inStruct2 )
result = true;
fnames = fieldnames( inStruct1 );
if( length( fnames )~=length(fieldnames(inStruct2)) )
  result = false;
elseif( ~strcmp( fnames, fieldnames( inStruct2 ) ) )
  result = false;
else
  for i = 1:length( fnames )
    if( isstruct( inStruct1.(fnames{i}) ) )
      if( ~equal_structs( inStruct1.(fnames{i}), inStruct2.(fnames{i}) ) )
        result = false;
      end
    elseif( ischar( inStruct1.(fnames{i}) ) || iscell( inStruct1.(fnames{i}) ) )
      if( ~strcmp( inStruct1.(fnames{i}), inStruct2.(fnames{i}) ) )
        result = false;
      end
    elseif( isnumeric( inStruct1.(fnames{i}) ) )
        result = isequalwithequalnans( inStruct1.(fnames{i}), inStruct2.(fnames{i}) );
    else
      if( any( inStruct1.(fnames{i}) ~= inStruct2.(fnames{i}) ) )
        result = false;
      end
    end
  end
end

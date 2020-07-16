function missing_mex_file()
% 
%  Reports that a mex file binary is missing for the current platform.
%  Derives the mex file's name from the name of the caller.
%
%  This function is part of the BCI2000 project. 
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

%  This is a help file documenting the functionality contained in
%  load_bcimat.mex.
%  $Id: missing_mex_file.m 4536 2013-08-05 14:30:13Z mellinger $
%
stack = dbstack(1);
error( [ ...
  '\nThere is no ''%s'' mex file for your platform present in %s.\n\n' ...
  'Pre-compiled mex files for a number of platforms are available at ' ...
  'http://www.bci2000.org/svn/trunk/tools/mex.\n\n' ...
  'You may also compile mex files for your platform by downloading ' ...
  'the BCI2000 source code from http://www.bci2000.org/svn/trunk, ' ...
  'and executing src/core/Tools/mex/buildmex.m from within Matlab.\n\n' ...
  'Access to BCI2000 is free under the terms of the GNU General Public ' ...
  'License (GPL). Register at http://www.bci2000.org/GPL/ to obtain ' ...
  'a user account for downloading BCI2000.' ...
  ], ...
  stack(1).name, ...
  fileparts(which(stack(1).file)) ...
);

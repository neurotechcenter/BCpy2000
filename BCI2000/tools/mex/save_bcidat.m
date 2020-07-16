function save_bcidat( varargin )
%SAVE_BCIDAT Save Matlab workspace variables into a BCI2000 data file. 
%
%  save_bcidat( 'filename', signal, states, parameters )
%
%  Saves signal, state, and parameter data into the named file.
%  The signal, state, and parameter arguments must be Matlab structs as 
%  created by the <tt>load_bcidat</tt>, or <tt>convert_bciprm</tt> mex
%  files.
%  Signal data is always interpreted as raw data, i.e. it will be written
%  into the output file unchanged.
%
%  The output file format is deduced from the output file's extension, 
%  which may be .dat, .edf, or .gdf. When no extension is recognized, 
%  the BCI2000 dat file format is used.
%
%  The save_bcidat function is part of the BCI2000 project.

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
%  save_bcimat.mex.
%  $Id: save_bcidat.m 3798 2012-02-01 18:07:06Z mellinger $
%
missing_mex_file;

function result = convert_bciprm( varargin )
%CONVERT_BCIPRM Convert between Matlab representations of BCI2000 parameters.
%
% A Matlab (mex) subroutine that converts BCI2000 parameters
% from Matlab struct into string representation and back.
%
%  parameter_lines = convert_bciprm( parameter_struct );
%
% converts a BCI2000 parameter struct (as created by load_bcidat)
% into a cell array of strings containing valid BCI2000 parameter
% definition strings (ignoring the NumericValue field if present).
%
% When the input is a cell array rather than a Matlab struct, convert_bciprm
% will interpret the input as a list of BCI2000 parameter definition strings:
%
%  parameter_struct = convert_bciprm( parameter_lines );
%
% This file is part of the BCI2000 project.

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

%  This is a help file documenting the functionality contained in
%  convert_bciprm.mex.
%  $Id: convert_bciprm.m 3798 2012-02-01 18:07:06Z mellinger $
missing_mex_file;
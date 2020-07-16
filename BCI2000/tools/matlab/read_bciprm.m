function p = read_bciprm(fn, subfield)
% READ_BCIPRM   Read BCI2000 parameters from various different formats
% 
% P = READ_BCIPRM(X)
% 
% Read the BCI2000 parameters from X, regardless of whether X is a .dat
% filename, a .prm filename, a string containing the plaintext contents of
% a .prm file, a cell array of parameter strings (like the output of
% CONVERT_BCIPRM when used in one direction), or even if X is already in
% the output struct format.
% 
% Returns a struct in whatever format is returned by the currently-compiled
% LOAD_BCIDAT and CONVERT_BCIPRM mex-files.
% 
% P = READ_BCIPRM(X, 'Value')
% 
% Same as above, except that for every parameter, only the named subfield 
% (in this example, 'Value') is returned.  If you are using an old version
% of LOAD_BCIDAT or CONVERT_BCIPRM that does not return substructures,
% this sub-field argument is ignored. Therefore, passing 'Value'
% effectively wraps both the old and new mex files so that their behaviour
% is the same (for backward compatibility). 
% 
% dependencies:  mexfiles load_bcidat and convert_bciprm

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% $Id: read_bciprm.m 4671 2013-12-27 23:03:54Z jhill $ 
%% Author: Jeremy Hill <jezhill@gmail.com>
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
%% http://www.bci2000.org 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

p = wrangle(fn); % subfunction below

if nargin < 2
	subfield = '';
end
if ~isempty(subfield)
	ff = fieldnames(p);
	for i = 1:numel(ff)
		if isstruct(p.(ff{i}))
			p.(ff{i}) = p.(ff{i}).(subfield);
		end
	end
end



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function p = wrangle(p)

if isstruct(p)
	if numel(p) ~= 1, error('non-scalar struct'), end
	if isfield(p, 'Parms')
		if isstruct(p.Parms) & numel(p.Parms) == 1
			ppval = struct2cell(p.Parms);
			if isstruct(ppval{1}) & numel(ppval{1}) == 1
				p = p.Parms;
			end
		end
	end
	return
end

if ischar(p)
	if size(p, 1) > 1
		p = cellstr(p);
	elseif exist(p, 'file')
		if ~isempty(which(p)), p = which(p); end
		if strncmp(lower(fliplr(p)), fliplr('.dat'), 4) | strncmp(lower(fliplr(p)), fliplr('.bci2000'), 7)
			[ans ans p] = load_bcidat(p, [0 0]);
			return
		end
		if ~strncmp(lower(fliplr(p)), fliplr('.prm'), 4), error('can only deal with .dat and .prm files'), end
		fid = fopen(p, 'r');
		if fid == -1, error(sprintf('failed to open ''%s''', p)), end
		p = fread(fid, inf); p = char(p(:)');
		fclose(fid);
		if isempty(p), keyboard, end
	end
end

if iscellstr(p)
	pp = p;
else
	if ~ischar(p), error('unrecognized input type'), end
	p = fliplr(deblank(fliplr(deblank(p))));
	p = strrep(p, [char(13) char(10)], char(10));
	p = strrep(p, [char(13)], char(10));
	
	pp = {};
	while ~isempty(p)
		[ppi p] = strtok(p, char(10));
		if ~isempty(ppi), pp{end+1,1} = ppi; end
	end
end

p = convert_bciprm(pp);

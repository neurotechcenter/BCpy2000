function [srcnames, dstnames] = rename_bcidat(f, dstdir, dateformat)
% RENAME_BCIDAT  renames BCI2000 data files according to a datestamped convention.
% 
%     RENAME_BCIDAT(F)
% 
% Input argument F could be a filename (or better still, a complete file
% path) of a .dat file.  Or it could be a cell array of filenames.  Or,
% probably most usefully of all, it can be the name (or complete path) of a
% directory: in that case, the directory will be searched recursively for
% .dat files.
% 
% For each file, RENAME_BCIDAT finds the StorageTime datestamp, the
% SubjectName, the SubjectSession and the SubjectRun parameters stored
% inside the file. Then it moves/renames the file according to a certain
% scheme that starts with an ISO date stamp  yyyymmdd_HHMMSS.  This way,
% all files will have a unique filename no matter what was originally
% entered as session and run numbers, and they will appear in chronological
% order in Windows.
% 
% The files are renamed where they sit (in whatever subdirectory they are
% found). If you want to change this and consolidate things by moving them
% all into one directory then you can supply the name of that directory as
% a second argument: 
% 
%    RENAME_BCIDAT(F, D)
% 
% This is the same, except that the files are all moved so that they sit at
% the top level in directory D.  One useful value for D is '.'  which means
% the current directory that you're working in in Matlab.
% 
% So for example,
% 
%    RENAME_BCIDAT('.', '.')
% 
% would find all .dat files in the directory where you are working now, and
% all its sub-directories and sub-sub-directories, and then it would rename
% those files to have unique, date-coded names, and move them all to the
% top level (directly in the directory you are working in now).
% 
% If you just want to see what WOULD happen, without the files actually being
% renamed, then you can append a question-mark to the file or directory name.
% 
% Finally, if you want to use a different format other than yyyymmdd_HHMMSS
% (and have carefully thought through the implications of doing so) then
% this can be specified using a third input argument. For example:
% 
%     RENAME_BCIDAT([F '?'], '',  'yyyymmdd')   % preview only
%     RENAME_BCIDAT(F,       '',  'yyyymmdd')   % rename in-place
%     RENAME_BCIDAT(F,       D,   'yyyymmdd')   % rename and move to D
% 

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% $Id: rename_bcidat.m 3846 2012-03-02 02:26:48Z jhill $ 
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

if nargin < 3, dateformat = ''; end
if isempty(dateformat), dateformat = 'yyyymmdd_HHMMSS'; end

if nargin < 2, dstdir = ''; end

if nargin < 1, f = ''; end
dryrun = strncmp(fliplr(f), '?', 1);
if dryrun, f(end) = []; end
if isempty(f), f = '.'; end



if ischar(f)
	if isdir(f)
		f = jdir(f, 1);
		isdat = logical(zeros(size(f)));
		for i = 1:numel(f), isdat(i) = strncmp(fliplr(lower(f{i})), 'tad.', 4); end
		f = f(isdat);
	end
end
f = cellstr(f);

if ~isempty(dstdir)
	oldd = cd;
	cd(dstdir)
	dstdir = cd;
	cd(oldd)
end

if dryrun, verb = 'would move'; else verb = 'moving'; end
blank = repmat(' ', size(verb));
ghostfiles = {};
dstnames = cell(size(f));
for i = 1:numel(f)
	[srcdir srcname srcxtn] = fileparts(f{i});
	oldd = cd;
	if ~isempty(srcdir), cd(srcdir), end
	srcdir = cd;
	cd(oldd)
	srcname = fullfile(srcdir, [srcname srcxtn]);
	p = read_bciprm(srcname);
	d = read_bcidate(p);
	subj = p.SubjectName.Value{1};
	session = p.SubjectSession.NumericValue;
	run = p.SubjectRun.NumericValue;
	dstname = sprintf('%s_%s_S%03dR%03d.dat', datestr(d, dateformat), upper(subj), session, run);
	if isempty(dstdir)
		dstname = fullfile(fileparts(srcname), dstname);
	else
		dstname = fullfile(dstdir, dstname);
	end
	if strcmp(lower(srcname), lower(dstname)) % assume a case-insensitive filesystem, for safety's sake
		fprintf('  %s is already where it should be\n', dstname);
	else
	    fprintf('  %s from: %s\n%s     to: %s\n', verb, srcname, blank, dstname);
        if exist(dstname, 'file')
			fprintf('* NOPE, CANCELLED: destination file already exists\n');
		elseif dryrun
			if ismember(dstname, ghostfiles)
				fprintf('* NOPE, CANCELLED: destination file would already exist\n');
            else
                dstnames{i} = dstname;
				ghostfiles{end+1,1} = dstname;
			end
        else
            dstnames{i} = dstname;
			movefile(srcname, dstname);
		end
	end
	fprintf('\n');
end
if nargout, srcnames = f; end

function aa = jdir(d, recursive)

oldd = cd;
d = cellstr(d);
aa = {};
for id = 1:numel(d)
	cd(d{id})
	d{id} = cd;
	a = dir;
	cd(oldd)
	a(strcmp({a.name}, '.')) = []; % sometimes I think TheMathWorks are deliberately making my life difficult
	a(strcmp({a.name}, '..')) = []; % sometimes I think TheMathWorks are deliberately making my life difficult
	for i = 1:numel(a), a(i).name = fullfile(d{id}, a(i).name); end
	isd = [a.isdir]; ad = a(isd); a(isd) = [];
	a = sort({a.name}');
	aa = [aa;a];
	if recursive, for i = 1:numel(ad), aa = [aa;jdir(ad(i).name,1)];, end, end
end


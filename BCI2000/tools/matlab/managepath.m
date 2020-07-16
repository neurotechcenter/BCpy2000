function [out, isd, isf, err] = managepath(varargin)
% MANAGEPATH   Concatenate and resolve filesystem paths, and add or remove them from the Matlab path or the operating-system PATH
% 
% MANAGEPATH  .. foo bar/baz
%     concatenates the input arguments into ../foo/bar/baz, and then makes
%     sure that all the slashes go in the current operating system's
%     preferred direction. Then, if the resulting path is a valid file or
%     directory, resolve it into an absolute path.
% 
% [D,  ISDIR, ISFILE] = MANAGEPATH(...)
%     returns not only the resolved absolute path, but also the results of
%     the tests for whether it is a valid directory or a valid file (for
%     this purpose, a directory is NOT considered to be a type of "file").
% 
% Various flags can be passed as the first input argument, to perform
% subsequent tasks with the resultant resolved absolute path.
% 
% MANAGEPATH -ADDTOMATLABPATH ...
%     Prepend the directory to the Matlab PATH (or move it to the
%     beginning, if it is already in another position).
% 
% MANAGEPATH -REMOVEFROMMATLABPATH ...
%     Remove the directory from the Matlab PATH, if it is there.
% 
% MANAGEPATH -ADDTOSYSTEMPATH ...
%     Prepend the directory to the operating system's PATH environment
%     variable (or move it to the beginning, if it is already in another
%     position).
% 
% MANAGEPATH -REMOVEFROMSYSTEMPATH ...
%     Remove the directory from the operating system's PATH environment
%     variable, if it is there.
% 
% MANAGEPATH -REMOVEALL ...
%     Search both the Matlab PATH and the operating-system PATH for any
%     directory that is somewhere underneath the specified directory, and
%     remove them.
% 
% MANAGEPATH -ONMATLABPATH ...
% MANAGEPATH -ONSYSTEMPATH ...
%     Instead of a resolved path, return a numeric value indicating the
%     slot that the specified directory occupies on the Matlab PATH (or
%     operating-system PATH). A zero indicates that it is not there.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% $Id: managepath.m 3798 2012-02-01 18:07:06Z mellinger $ 
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


cmd = '';
err = '';

if numel(varargin)
	cmd = varargin{1};
	if ischar(cmd) & ~isempty(cmd)
		cmd = lower(cmd);
		if cmd(1) == '-'
			varargin(1) = [];
		else
			cmd = '';
		end
	end
end

switch length(varargin)
	case 0, d = '';
	case 1, d = varargin{1};
	otherwise, d = fullfile(varargin{:});
end
d = strrep(strrep(d, '\', '/'), '/', filesep);
isd = exist(d, 'dir') ~= 0;
isf = exist(d, 'file') ~= 0 & ~isd;
if isd
	d = resolve(d);
elseif isf
	[parent stem xtn] = fileparts(d);
	d = fullfile(resolve(parent), [stem xtn]);
end

if ~isempty(cmd)
	mp = split(pathsep, path);
	mpmatch = find(matches(mp, d, 'full'));
	sp = split(pathsep, getenv('PATH'));
	spmatch = find(matches(sp, d, 'full'));
end

switch cmd
	case {''}
		out = d;
		err = '';

	case {'-onmatlabpath'}
		d = [mpmatch(:);0];
		d = d(1);
		out = d;
		err = '';

	case {'-onsystempath'}
		d = [spmatch(:);0];
		d = d(1);
		out = d;
		err = '';

	case {'-addtomatlabpath', '-add', '-addpath'}
		doit = 0;
		if ~isd &  isf
			err = sprintf('cannot add to matlab path, since it is a file, not a directory: %s', d);
		elseif ~isd
			err = sprintf('cannot add to matlab path, since it does not exist: %s', d);
		elseif isempty(mpmatch)
			fprintf('adding to the Matlab path:  %s\n', d);
			doit = 1;
		elseif ~ismember(1, mpmatch)
			fprintf('moving to the top of the Matlab path: %s\n', d);
			doit = 1;
		end
		if doit
			addpath(d)
		end

	case {'-removefrommatlabpath', '-rm', '-rmpath'}
		doit = 0;
		if isempty(mpmatch)
			fprintf('cannot remove from the Matlab path, since it is not there:  %s\n', d);
		else
			fprintf('removing from the Matlab path:  %s\n', d);
			doit = 1;
		end
		if doit
			rmpath(d)
		end

	case {'-addtosystempath', '-addsys', '-addsyspath'}
		doit = 0;
		if ~isd &  isf
			err = sprintf('cannot add to operating-system PATH, since it is a file, not a directory: %s', d);
		elseif ~isd
			err = sprintf('cannot add to operating-system PATH, since it does not exist: %s', d);
		elseif isempty(spmatch)
			fprintf('adding to the operating-system PATH:  %s\n', d);
			doit = 1;
		elseif ~ismember(1, spmatch)
			fprintf('moving to the top of the operating-system PATH: %s\n', d);
			doit = 1;
		end
		if doit
			sp(spmatch) = [];
			sp = [{d};sp(:)];
			sp = join(pathsep, sp);
			setenv('PATH', sp);
		end

	case {'-removefromsystempath', '-rmsys', '-rmsyspath'}
		doit = 0;
		if isempty(spmatch)
			fprintf('cannot remove from the operating-system PATH, since it is not there:  %s\n', d);
		else
			fprintf('removing from the operating-system PATH:  %s\n', d);
			doit = 1;
		end
		if doit
			sp(spmatch) = [];
			sp = join(pathsep, sp);
			setenv('PATH', sp);
		end

	case {'-removeall', '-rmall'}
		for i = 1:numel(mp)
			if matches(d, mp{i}, 'partial')
				fprintf('removing from the Matlab path:  %s\n', mp{i});
				rmpath(mp{i})
			end
		end
		rm = logical(zeros(size(sp)));
		for i = 1:numel(sp)
			if matches(d, sp{i}, 'partial')
				fprintf('removing from the operating-system PATH:  %s\n', sp{i});
				rm(i) = 1;
			end
		end
		if any(rm)
			sp(rm) = [];
			sp = join(pathsep, sp);
			setenv('PATH', sp);
		end

	otherwise
		err = sprintf('unknown command ''%s''', cmd(:)');
end

if nargout, out = d; end
if nargout < 4, error(err), end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function d = resolve(d)
% if d exists as a directory, turn it into an absolute path
if isempty(d), d = '.'; end
oldd = pwd;
try
	cd(d)
	d = pwd;
end
cd(oldd)

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function out = matches(parent, child, mode)

if iscellstr(parent)
	out = logical(zeros(size(parent)));
	for i = 1:numel(parent), out(i) = matches(parent{i}, child, mode); end
	return
end
if ~strncmp(fliplr(parent), filesep, length(filesep)), parent = [parent filesep]; end
if ~strncmp(fliplr(child), filesep, length(filesep)), child = [child filesep]; end
if ispc | ismac
	parent = lower(parent);
	child = lower(child);
end
switch mode
	case 'full'
		out = strcmp(parent, child);
	case 'partial'
		out = strncmp(parent, child, length(parent));
	otherwise
		error('unrecognized mode')
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function parts = split(delimiter, string)
parts = {};
while ~isempty(string)
	[parts{end+1,1} string] = strtok(string, delimiter);
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function string = join(delimiter, parts)
parts = parts(:)';
parts(2, :) = {delimiter};
string = [parts{1:end-1}];

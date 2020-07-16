function [out, isd, isf] = bci2000path(varargin)
% BCI2000PATH    Determine, find and use paths in the BCI2000 distribution
% 
% BCI2000PATH -SET C:\PATH\TO\BCI2000
%      Stores the path to the root directory of the desired BCI2000
%      installation, as a global variable.
% 
% If no such root has been set explicitly, other calls to BCI2000PATH
% will try to infer the location of BCI2000 based on the location of
% the bci2000path.m file itself, and -SET that.
% 
% P = BCI2000PATH('tools/mex')
%      Returns the full path to the named subdirectory or file inside the
%      remembered BCI2000 installation.
%   
% [P, ISD, ISF] = BCI2000PATH('tools/mex/load_bcidat.m')
%      P is the resolved absolute path. ISD is 1 if the item exists as a
%      directory. ISF is 1 if it exists as a file (as should be the case
%      in this example).
% 
% Other flags supported by MANAGEPATH are also accepted, such as:
%      -AddToMatlabPath
%      -RemoveFromMatlabPath
%      -AddToSystemPath
%      -RemoveFromSystemPath
%      -RemoveAll
% 
% For example:
%      % Set up BCI2000 toolboxes:
%      BCI2000PATH -AddToMatlabPath tools/mex
%      BCI2000PATH -AddToMatlabPath tools/matlab   % spot the 
%      BCI2000PATH -AddToSystemPath tools/cmdline  %          TWO differences
% 
%      % Remove BCI2000 toolboxes:
%      BCI2000PATH -RemoveAll

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% $Id: bci2000path.m 3798 2012-02-01 18:07:06Z mellinger $ 
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

global BCI2000ROOT

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

if isequal(cmd, '-set')
	[BCI2000ROOT, isd, isf] = managepath(varargin{:});
	
	if isempty(BCI2000ROOT)
		whereAmI = fileparts(which(mfilename));
		[d, isd, isf] = managepath(whereAmI, '..', '..');
		if isd
			[whereShouldIBe isd isf] = managepath(d, 'tools', 'matlab');
			if isd, BCI2000ROOT = d; end
		end
	end
	
	if isempty(BCI2000ROOT)
		if ispc
			tries = {
				{'c:\bci2000\3.x'}
				{'c:\BCI2000'}
			};
		else
			tries = {
				{getenv('HOME'), 'bci', 'bci2000', '3.x'}
				{getenv('HOME'), 'BCI2000'}
			};
		end
		for i = 1:numel(tries)
			[d isd isf] = managepath(tries{i}{:});
			if isd, BCI2000ROOT = d; break, end
		end
		if isempty(BCI2000ROOT), error('failed to find BCI2000 root:  use BCI2000PATH -SET to set it'), end
		fprintf('\nI am guessing BCI2000 is here:  %s\n*** Please make sure this is the correct copy. ***\nIf not, call (e.g.) BCI2000PATH -SET %s\n\n', BCI2000ROOT, 'C:\CORRECT\PATH\TO\BCI2000');
	end
	
	if nargout > 0, out = BCI2000ROOT; end
	return
end
	
if isempty(BCI2000ROOT), bci2000path -set, end

[p, isd, isf, err] = managepath(cmd, BCI2000ROOT, varargin{:});
if isempty(cmd) | nargout > 0, out = p; end
error(err)

function varargout = filtermex(varargin)

[here, mexname, ext] = fileparts(which(mfilename));

oldd = cd; cd(here); cd ..; cd ..; cd ..;  root = cd; cd(oldd)

def = {}; src = {}; inc = {};

inc{end+1} = here;
src{end+1} = fullfile(here, [mexname '.cpp']);
src{end+1} = fullfile(here, 'CustomIIR.cpp');

inc{end+1} = fullfile(root, 'extlib', 'math');
src{end+1} = fullfile(root, 'extlib', 'math', 'FilterDesign.cpp');
inc{end+1} = fullfile(root, 'shared', 'utils');
% src{end+1} = fullfile(root, 'shared', 'utils', 'DisplayFilter.cpp');
inc{end+1} = fullfile(root, 'shared', 'utils', 'Expression');
src{end+1} = fullfile(root, 'shared', 'utils', 'Expression', 'ArithmeticExpression.cpp');
src{end+1} = fullfile(root, 'shared', 'utils', 'Expression', 'ExpressionParser.cpp');
inc{end+1} = fullfile(root, 'shared', 'types');
src{end+1} = fullfile(root, 'shared', 'types', 'Brackets.cpp');
src{end+1} = fullfile(root, 'shared', 'types', 'EncodedString.cpp');
src{end+1} = fullfile(root, 'shared', 'types', 'GenericSignal.cpp');
src{end+1} = fullfile(root, 'shared', 'types', 'LabelIndex.cpp');
src{end+1} = fullfile(root, 'shared', 'types', 'PhysicalUnit.cpp');
src{end+1} = fullfile(root, 'shared', 'types', 'SignalProperties.cpp');
src{end+1} = fullfile(root, 'shared', 'types', 'SignalType.cpp');
inc{end+1} = fullfile(root, 'shared', 'bcistream');
src{end+1} = fullfile(root, 'shared', 'bcistream', 'BCIError.cpp');
src{end+1} = fullfile(root, 'shared', 'bcistream', 'BCIError_mex.cpp');
inc{end+1} = fullfile(root, 'shared', 'config'); % for PCHInlcudes.h
inc{end+1} = fullfile(root, 'shared'); % for defines.h

for i = 1:numel(inc), inc{i} = ['-I' inc{i}]; end

def{end+1} = '-D__FUNC__=__FUNCTION__';
def{end+1} = '-Dstricmp=strcasecmp';
def{end+1} = '-Dstrnicmp=strncasecmp';

args = [src inc def]';
fprintf('compiling %s:\n', mexname)
disp(char(args))
mex(args{:}, '-output', mexname)
if nargout
	[varargout{:}] = feval(mexname, varargin{:});
else
	feval(mexname, varargin{:})
end

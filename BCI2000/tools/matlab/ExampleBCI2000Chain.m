

% set up BCI2000 toolbox paths(only needs to be done once per session)
bci2000path -AddToMatlabPath tools/mex     % binary mex-files (load_bcidat and friends)
bci2000path -AddToMatlabPath tools/matlab  % matlab m-files
bci2000path -AddToSystemPath tools/cmdline % binary executables (command-line utilities)

filename = bci2000path('data', 'samplefiles', 'eeg3_2.dat');
s = bci2000chain(filename, 'TransmissionFilter|SpatialFilter|ARFilter', 'ExampleParameters.prm', 'SpatialFilterType', 3)

% This example reads and replays the sample file through the specified filter chain, starting with the parameters
% originally used in the recording, but overwriting some of these with the ones specified in ExampleParameters.prm,
% and finally switching the spatial filter to CAR mode.
% 
% See HELP BCI2000CHAIN for more.



% clean up matlab path and operating-system path (if desired)
% bci2000path -RemoveAll

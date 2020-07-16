%% MATLAB compiler code to deploy OfflineAnalysis and elocHelper
clear all
close all

platform = computer;

copyfile(which('settings.mat.default'), strcat(fileparts('settings.mat.default'), 'settings.mat'));

if strcmp(platform,'PCWIN')
    fprintf(1,'Building for Windows 32-bit\n');
    mcc -v -e -a settings.mat -a load_bcidat.mexw32 -a logo.jpg -I ../mex -o OfflineAnalysisWin32 OfflineAnalysis.m
    mcc -v -e -a settings.mat                       -a logo.jpg           -o elocHelperWin32      elocHelper.m
elseif strcmp(platform,'PCWIN64')
    fprintf(1,'Building for Windows 64-bit\n');
    mcc -v -e -a settings.mat -a load_bcidat.mexw64 -a logo.jpg -I ../mex -o OfflineAnalysisWin64 OfflineAnalysis.m   
    mcc -v -e -a settings.mat                       -a logo.jpg           -o elocHelperWin64      elocHelper.m
elseif strcmp(platform,'GLNXA64')
    fprintf(1,'Building for Linux 64-bit\n');
    mcc -v -e -a settings.mat -a load_bcidat.mexglx64 -a logo.jpg -I ../mex -o OfflineAnalysisLinux64 OfflineAnalysis.m  
    mcc -v -e -a settings.mat                         -a logo.jpg           -o elocHelperLinux64      elocHelper.m
elseif strcmp(platform,'MACI64')
    fprintf(1,'Building for Mac OS X 64-bit\n');
    mcc -v -e -a settings.mat -a load_bcidat.mexmaci64 -a logo.jpg -I ../mex -o OfflineAnalysisMac64 OfflineAnalysis.m       
    mcc -v -e -a settings.mat                          -a logo.jpg           -o elocHelperMac64      elocHelper.m
end
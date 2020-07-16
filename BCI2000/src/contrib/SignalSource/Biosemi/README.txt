Alpha Version of the BIOSEMI acquisition module.
Samuel A. Inverso (samuel.inverso@gmail.com), Yang Zhen

Before running:
Copy the Labview_DLL.dll to the prog directory for the module to work.

Note:
In Source parameters you can set the module to postfix the BIOSEMI trigger
channels after the signal channels. i.e. if you acquire 64 channels of EEG, and
set postfix to true, channels 0 to 63 will be EEG and channels 64 to 79 will 
be trigger channels 0 to 15. 
Warning: If postfix trigger channels is 0, the trigger values will not be saved in the data files and are not recoverable.
 


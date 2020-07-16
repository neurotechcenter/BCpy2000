Include the following .lib files to the project:

nidaqmx.lib

This library file is already converted from OMF  to COFF using omf2coff DOS
utility contained in the Borland C Builder directory.
The original NIDAQmx library is also included with the name NIDAQmx_orig.lib


Acquisition parameters:
The maximum number of channels is fixed to 16 and can be changed
in the Preflight function.
Minimum and maximum value of the input voltage for the DAQ Card is now set
to min=-5V , max=5V.
Terminal configuration is set to Not Referenced Single Ended.
All the parameters can be modified changing the corresponding
variables in the ADConfig function.



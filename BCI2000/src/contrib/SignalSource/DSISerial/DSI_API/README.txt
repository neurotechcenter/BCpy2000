# This file is part of the Application Programmer's Interface (API) for Dry Sensor Interface
# (DSI) EEG systems by Wearable Sensing. The API consists of code, headers, dynamic libraries
# and documentation.  The API allows software developers to interface directly with DSI
# systems to control and to acquire data from them.
# 
# The API is not certified to any specific standard. It is not intended for clinical use.
# The API, and software that makes use of it, should not be used for diagnostic or other
# clinical purposes.  The API is intended for research use and is provided on an "AS IS"
# basis.  WEARABLE SENSING, INCLUDING ITS SUBSIDIARIES, DISCLAIMS ANY AND ALL WARRANTIES
# EXPRESSED, STATUTORY OR IMPLIED, INCLUDING BUT NOT LIMITED TO ANY IMPLIED WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, NON-INFRINGEMENT OR THIRD PARTY RIGHTS.
# 
# Copyright (c) 2014-2018 Wearable Sensing LLC


The Dry Sensor Interface API dynamic libraries in this directory have version 1.15.1.

To create a DSI application:

(1) Add the file DSI_API_Loader.c to your project (can be compiled as C or C++)

(2) #include "DSI.h" in your own C or C++ code

(3) Call Load_DSI_API() in your code before calling any of the other functions.

(4) Check the return value of Load_DSI_API before proceeding. It will be zero on
    success. Any non-zero value means the API failed to load and other API calls
    (function names starting with DSI_) should not be called.
    
Consult demo.c for more details, including commented example code.


Current issues with g.NEEDAccess/g.Nautilus API (as of BETA 743)
================================================================
* Uninstall of BETA 6XX didn't work -- Required manual purge of install files and all instances of g.tec from registry.  Took hours.
	* Could be related to Windows 8 -> Windows 8.1 upgrade between installation and uninstallation.

	__Remedy:__ Ensure uninstall works.

* Static link lib is ... not optimal for BCI2000.
	* Only lib released requires Visual Studio (X64)?
	* Requires X64 build tools (not available with VS Express?)
	* .lib is COFF formatted, and incompatible with cross-platform compilation.

	__Remedy:__ Strip out any extra code in lib file and put into GDSClientAPI.dll.
		Ideally, the lib is just glue between the header and the dll.

* Demo device is mislabeled... ?  
	* GDS reports the nautilus as NB-2014.01.02, but the device is labelled NA-2014.01.02...
	* The base station is labelled NB-2014.01.02. Is this expected?  Should these have the same name?

	__Remedy:__ Use sharpie marker to change Serial Number on label from __NA__-2014.01.02 to __NB__-2014.01.02?

* Client application crash can cause GDS service crash.
	* Probably not a super big issue, but manual restart is inconvenient and well beyond most users.

	__Remedy:__ Enable automatic GDS Server restart?

* Default scan count for acquisition at 250 Hz is 8 scans
	* This makes for weird sample block lengths in BCI2000, (e.g., 250 Hz @ 8 scans per block = 32 ms per block)
	* As is, this is a bit confusing.  The documentation states that the default scan rate for 500 Hz acquisition is 16 scans per block
		* But the API says it's actually 15 scans.  This makes for better block lengths

	__Remedy:__ Make default sampling rates 256/512 Hz instead of 250/500 Hz or make default scan sizes multiples of 5/10/15 

* It takes a very long time to connect to the server
	* Sometimes upward of 10 seconds
	* BCI2000 has a timeout afterwhich it assumes the source module has broken; 
		* g.NEEDaccess exceeds that timeout by a lot (default is 2 sec to start acquisition, g.NEEDaccess is 8-10 sec)
	* In order to verify configuration before starting acquisition, BCI2000 has to connect to the server twice; doubling this time!

	__Remedy:__ Optimize the connection time, or look into why it is soooo slow.

* Timing (inter-block jitter) is sub-par.
	* When acquiring default scancounts from g.NEEDaccess, timings between blocks can jitter as much as 100 ms
	* This makes real-time operation with the device untenable.
	* Timing plot exhibits heart-beat in some configurations suggesting that "optimal" scan-counts may not actually be.

	__Remedy:__ Perhaps support memory-mapped architecture instead of client/server networked (UDP) architecture for real-time use?

* At default "optimal" minimum scan counts, system does not perform in real-time and lags behind
	* At 15 scans per block @ 500 Hz, or 8 scans per block @ 250 Hz, average block timing is consistently longer than it should be.
	* At 8 scans/250 Hz, this average block timing exceeds 5% of the sampling rate.
	* At these low scan counts (sample block sizes) the AcquisitonError callbacks are called after 5-10 minutes,
		* Likely due to a buffer overflow from operating behind real-time.

	__Remedy:__ Fix real-time operation or set default minimum scan counts to 16/30

* GetData has confusing behavior
	* I asked for 32 scans and I've set up 32 channels, so I would expect a buffer size of 1024 samples.
	* Instead, I get a buffer of size 1056 samples, which seems to be formatted as 32 scans of 33 channels.
	* In general, there is not much information about the returned databuffer.  Please update documentation.
	* Specifically, I'm looking for a line that reads:
		* Data returned by GetData is in the following format:
		* One scan is returned for each set of channels, and scans are appended sequentially.
		* scanSize = bufferSize / scanCount
		* Value( ch, el ) = buffer[ ( scanSize * el ) + ch ]
		* Data returned is in unscaled units (use GDS_GetScaling)

	__Remedy:__ Update documentation to reflect format of returned DataBuffers

* Sample code hasn't been updated
	__Remedy:__ Update sample code

* Currently no API-standard way to acquire Accelerometer data, Digital Input data, Battery data, signal quality data, etc.
	__Remedy:__ Document how to acquire this data.


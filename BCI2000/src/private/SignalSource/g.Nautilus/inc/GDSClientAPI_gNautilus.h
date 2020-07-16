/* ========================================================================================
 * g.tec Device Service - Client API for g.USBamp devices
 * Copyright (c) 2013 Guger Technologies OG
 * ========================================================================================
 */

#ifndef GDSCLIENTAPI_GNAUTILUS_HPP
#define GDSCLIENTAPI_GNAUTILUS_HPP

#include "GDSClientAPI.h"

#ifdef __cplusplus
extern "C" {
#endif

// ========================================================================================
// Constants
// ========================================================================================

//! The maximum number of modules that can be mounted on a g.Nautilus device.
#define GDS_GNAUTILUS_MODULES_MAX 8

//! The maximum number of channels within a single module.
#define GDS_GNAUTILUS_CHANNELS_PER_MODULE_MAX 8

//! The maximum number of channels a g.Nautilus device provides.
#define GDS_GNAUTILUS_CHANNELS_MAX 64

//! The maximum number of additional channels
#define GDS_GNAUTILUS_ADDITIONAL_CHANNELS_MAX 8

//! The number of asynchronous digital IN channels a g.Nautilus device provides.
#define GDS_GNAUTILUS_DIGITAL_IO_CHANNELS_MAX 8

//! The number of the maximum length of an electrode name.
#define GDS_GNAUTILUS_ELECTRODE_NAME_LENGTH_MAX 4

//! The index number that indicates that no filter is chosen.
#define GDS_GNAUTILUS_NO_FILTER_IDX -1

//! The maximum length of the device information string.
#define GDS_GNAUTILUS_DEVICE_INFORMATION_LENGTH_MAX 1024


// ========================================================================================
// Enumerations
// ========================================================================================

//! The input signals provided for a channel of a g.Nautilus device.
enum GDS_GNAUTILUS_INPUT_SIGNAL
{
	//! Analog input from electrodes.
	GDS_GNAUTILUS_INPUT_SIGNAL_ELECTRODE = 0,

	//! Channels inputs shorted to GND. 
	GDS_GNAUTILUS_INPUT_SIGNAL_SHORTED = 1,

	//! Measurement of the voltage supply.
	GDS_GNAUTILUS_INPUT_SIGNAL_MVDD = 2,

	//! Noise reduction mode (driven right leg)
	GDS_GNAUTIUS_INPUT_SIGNAL_DRL = 3,

	//! Measurement of the temperature of the chip.
	GDS_GNAUTILUS_INPUT_TEMPERATURE = 4,

	//! Test Signal as input for all electrodes.
	GDS_GNAUTILUS_INPUT_SIGNAL_TEST_SIGNAL = 5,

	//! Shortcut within the chip itself (not grounded).
	GDS_GNAUTIUS_INPUT_SIGNAL_SHORTCUT_CHIP = 6,

	//! Driven right leg signal measurement
	GDS_GNAUTIUS_INPUT_SIGNAL_DRL_CHIP = 7,

	//! Chip internal test signal.
	GDS_GNAUTIUS_INPUT_SIGNAL_TEST_SIGNAL_CHIP = 8
};


// ========================================================================================
// Structures
// ========================================================================================
	
//! Describes the state of an asynchronous digital IN channel.
typedef struct _GDS_GNAUTILUS_DIGITAL_IO_CHANNEL
{
	//! The one-based channel number of the digital IN channel described by this instance.
	unsigned int ChannelNumber;

	//! Indicates if the channel described by this instance is a digital input or output channel.
	GDS_CHANNEL_DIRECTION Direction;
} GDS_GNAUTILUS_DIGITAL_IO_CHANNEL, *PGDS_GNAUTILUS_DIGITAL_IO_CHANNEL;

//! Describes the scaling values for each channel of a single device.
typedef struct _GDS_GNAUTILUS_SCALING
{
	//! The offset value for each channel in µV.
	float Offset[GDS_GNAUTILUS_CHANNELS_MAX];

	//! The scaling factor for each channel.
	float Factor[GDS_GNAUTILUS_CHANNELS_MAX];

} GDS_GNAUTILUS_SCALING, *PGDS_GNAUTILUS_SCALING;

//! Describes the configuration of a single channel of a g.Nautilus device.
typedef struct _GDS_GNAUTILUS_CHANNEL_CONFIGURATION
{
	//! Indicates whether the channel is enabled for data acquisition or not.
	BOOL Enabled;

	//! The gain/sensitivity factor of the channel.
	double Sensitivity;

	//! Indicates whether the channel is used for noise reduction computation or not.
	BOOL UsedForNoiseReduction;

	//! Indicates whether the channel is used for common average reference (CAR) calculation.
	BOOL UsedForCar;
		
	//! The zero-based index of the bandpass filter that is applied to the channel, or #GDS_GNAUTILUS_NO_FILTER_IDX to not apply a bandpass filter to the channel.
	int32_t BandpassFilterIndex;

	//! The zero-based index of the notch filter that is applied to the channel, or #GDS_GNAUTILUS_NO_FILTER_IDX to not apply a notch filter to the channel.
	int32_t NotchFilterIndex;

	//! The zero-based index of the channel that is used for bipolar derivation, or #GDS_GNAUTILUS_NO_FILTER_IDX to not use the channel for bipolar derivation.
	int32_t BipolarFilterIndex;

} GDS_GNAUTILUS_CHANNEL_CONFIGURATION, *PGDS_GNAUTILUS_CHANNEL_CONFIGURATION;

//! Describes the configuration of a g.Nautilus device.
typedef struct _GDS_GNAUTILUS_CONFIGURATION
{
	//! Indicates whether the g.Nautilus device is operated in slave mode or not.
	BOOL Slave;

	//! Specifies the input signal of all channels for the device.
	GDS_GNAUTILUS_INPUT_SIGNAL InputSignal;

	//! The sampling rate of the g.Nautilus device in Hertz.
	uint32_t SamplingRate;

	//! The number of scans to retrieve from the device at once, or zero to apply an automatically determined number of scans for the specified sampling rate.
	/*!
		Setting \ref NumberOfScans to a value other than zero is recommended for advanced users only.

		This value depends on the value of \ref SamplingRate. The following table lists the recommended values of \ref NumberOfScans for all possible sampling rates.

		\li \c 8 for a sampling rate of 250 Hz
		\li \c 16 for a sampling rate of 500 Hz
	 */
	uint32_t NumberOfScans;

	//! Indicates whether noise reduction should be enabled.
	BOOL NoiseReduction;

	//! Indicates whether common average (CAR) calculation should be enabled.
	BOOL CAR;

	//! Indicates whether acceleration data channels should be included in data acquisition.
	BOOL AccelerationData;

	//! Indicates whether the counter channel should be included in data acquisition.
	BOOL Counter;

	//! Indicates whether the link quality information channel should be included in data acquisition.
	BOOL LinkQualityInformation;

	//! Indicates whether the battery level (state of charge) channel should be included in data acquisition.
	BOOL BatteryLevel;

	//! Indicates whether the synchronously sampled digital input channels should be included in data acquisition.
	BOOL DigitalIOs;

	//! Indicates whether the validation indicator channel should be included in data acquisition.
	BOOL ValidationIndicator;

	//! The radio channel used for wireless communication between head set and base station of the device.
	uint32_t NetworkChannel;

	//! The configuration of each channel.
    /*!
        An element at zero-based index <i>i</i> holds the configuration of the channel with zero-based channel number <i>i</i>.
        Channels that the device does not provide cannot be selected for acquisition. The other settings in their configuration structure will be ignored. 
     */
	GDS_GNAUTILUS_CHANNEL_CONFIGURATION Channels[GDS_GNAUTILUS_CHANNELS_MAX];

} GDS_GNAUTILUS_CONFIGURATION, *PGDS_GNAUTILUS_CONFIGURATION;


// ========================================================================================
// Function pointer definitions
// ========================================================================================

//! The type of functions that handle callbacks of completed asynchronous impedance measurement cycles.
/*!
	\param connectionHandle	The handle associated with the session that triggered the callback.
	\param deviceName		The name of the device that triggered the callback.
	\param impedances		An allocated array that receives the impedance values in ohm for each channel. An element at zero-based index <i>i</i> receives the impedance value in ohm of zero-based channel number <i>i</i>, or \c std::numeric_limits<double>::quiet_NaN() (see extended description of \ref GDS_GNAUTILUS_GetImpedance for details).
    \param aborted			Indicates whether an occurred error is crucial for impedance measurement so that measurement had to be stopped automatically, if any.
	\param result			The result of the operation which has triggered the callback.
	\param usrData			Can be used to pass data from the user to the callback.
*/
typedef void (__stdcall *GDS_GNAUTILUS_ImpedanceMeasurementCallback)(GDS_HANDLE connectionHandle, const char (&deviceName)[DEVICE_NAME_LENGTH_MAX], double (&impedances)[GDS_GNAUTILUS_CHANNELS_MAX], BOOL aborted, GDS_RESULT result, void* usrData);


// ========================================================================================
// API Methods
// ========================================================================================

//! Retrieves the number of actually mounted modules of a device and the names of all available channels.
/*!
	The function returns the number of mounted modules within the parameter \a mountedModulesCount as well as the mapping of the channels within the array \a electrodeNames. The number of modules determines the maximum number of available channels. 
	To retrieve the number of modules and the length of the array, call the method with \a electrodeNames set to \c NULL. After allocating memory for the array, call the method again to retrieve the array holding the electrode names.

	\param connectionHandle		The connection handle whose associated data acquisition contains the device to retrieve the device information from.
	\param deviceName			The name of the device to retrieve the device information from.
	\param mountedModulesCount	A pointer to the variable that receives the number of mounted modueles.
	\param electrodeNamesCount	A pointer to the variable that specifies the number of elements in the \a electrodeNames array.
	\param electrodeNames		A pointer to an array that holds the names of the numbered channels as strings (e.g. PO4, C3,...). An element at zero-based index <i>i</i> contains the electrode name of the channel with zero-based channel number <i>i</i>.

	\returns A \ref GDS_RESULT structure containing information about the operation's success. 
*/
GDSCLIENTAPI_API GDS_RESULT __stdcall GDS_GNAUTILUS_GetDeviceInformation(GDS_HANDLE connectionHandle, const char (&deviceName)[DEVICE_NAME_LENGTH_MAX], uint32_t *mountedModulesCount, char (*electrodeNames)[GDS_GNAUTILUS_ELECTRODE_NAME_LENGTH_MAX], size_t *electrodeNamesCount);

//! Retrieves extended information about the specified device.
/*!
	\param connectionHandle		The connection handle whose associated data acquisition contains the device to retreive extended device information for.
	\param deviceName			The name of the device to retrieve extended device information for.
	\param deviceInformation	The extended device information string.

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
 */
GDSCLIENTAPI_API GDS_RESULT __stdcall GDS_GNAUTILUS_GetDeviceInformationEx(GDS_HANDLE connectionHandle, const char (&deviceName)[DEVICE_NAME_LENGTH_MAX], char (&deviceInformation)[GDS_GNAUTILUS_DEVICE_INFORMATION_LENGTH_MAX]);

//! Retrieves a boolean flag for each possible channel a device in its full configuration may provide. The flag indicates whether the corresponding channel is actually provided and available on the device.
/*!
	\param connectionHandle		The connection handle whose associated data acquisition contains the device to get the list of available channels for.
	\param deviceName			The name of the device to get the list of available channels for.
	\param availableChannels	An allocated array of length #GDS_GNAUTILUS_CHANNELS_MAX where an element at index <i>i</i> receives a boolean flag indicating whether the corresponding channel with one-based channel number <i>i + 1</i> is available on the device.

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
 */
GDSCLIENTAPI_API GDS_RESULT __stdcall GDS_GNAUTILUS_GetAvailableChannels(GDS_HANDLE connectionHandle, const char (&deviceName)[DEVICE_NAME_LENGTH_MAX], BOOL (&availableChannels)[GDS_GNAUTILUS_CHANNELS_MAX]);

//! Gets information about available digital I/O channels of the specified g.Nautilus device of the data acquisition session associated with the specified connection handle.
/*!
	\param connectionHandle				The connection handle whose associated data acquisition session contains the device to get information about available digital I/O channels for.
	\param deviceName					The name of the device to get information about available digital I/O channels for.
	\param availableDigitalIOChannels	A reference to a fixed size array of a number of exactly #GDS_GNAUTILUS_DIGITAL_IO_CHANNELS_MAX elements of type \ref GDS_GNAUTILUS_DIGITAL_IO_CHANNEL that receives information about available digital I/O channels.

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
 */ 
GDSCLIENTAPI_API GDS_RESULT __stdcall GDS_GNAUTILUS_GetAvailableDigitalIOs(GDS_HANDLE connectionHandle, const char (&deviceName)[DEVICE_NAME_LENGTH_MAX], GDS_GNAUTILUS_DIGITAL_IO_CHANNEL (&availableDigitalIOChannels)[GDS_GNAUTILUS_DIGITAL_IO_CHANNELS_MAX]);

//! Gets a list of supported sampling rates.
/*!
	\a supportedSamplingRates is a pointer to the first element of an array that must be allocated by the caller before. To determine the number of elements of the array to allocate, call \ref GDS_GNAUTILUS_GetSupportedSamplingRates with \a supportedSamplingRates set to \c NULL.
	The function then returns the number of elements of the \a supportedSamplingRates array in \a supportedSamplingRatesCount. Allocate an array of type uint32_t of that size and pass it on to another call of \ref GDS_GNAUTILUS_GetSupportedSamplingRates to retrieve the list of supported sampling rates.

	\param connectionHandle				The connection handle whose associated data acquisition contains the device to get the list of supported sampling rates for.
	\param deviceName					The name of the device to get the list of supported sampling rates for.
	\param supportedSamplingRates		A pointer to the first element of an allocated array of size \a	supported_sampling_rates_count that receives the list of supported sampling rates, or \c NULL to receive the number of supported sampling rates in \a supported_sampling_rates_count only.
	\param supportedSamplingRatesCount	A pointer to a variable that receives the number of supported sampling rates for the specified device.

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
 */
GDSCLIENTAPI_API GDS_RESULT __stdcall GDS_GNAUTILUS_GetSupportedSamplingRates(GDS_HANDLE connectionHandle, const char (&deviceName)[DEVICE_NAME_LENGTH_MAX], uint32_t *supportedSamplingRates, size_t *supportedSamplingRatesCount);

//! Retrieves the supported sensitivity values.
/*!
	In order to retrieve the supported sensitivity values, the method has to be called twice: First call it with parameter \a supportedSensitivities = NULL, then the method returns the number of available sensitivity values. Using this number, allocate an array of size \a sensitivities_count and call the method again to retrieve the supportedSensitivities themselves.

	\param connectionHandle				The connection handle whose associated data acquisition contains the device to retrieve the supported sensitivity values for.
	\param deviceName					The name of the device to retrieve the supported sensitivity values for.
	\param supportedSensitivities		The array the sensitivity values will be stored in.
	\param supportedSensitivitiesCount	A pointer to the variable that holds the number of available sensitivity values.

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
*/
GDSCLIENTAPI_API GDS_RESULT __stdcall GDS_GNAUTILUS_GetSupportedSensitivities(GDS_HANDLE connectionHandle, const char (&deviceName)[DEVICE_NAME_LENGTH_MAX], double *supportedSensitivities, size_t *supportedSensitivitiesCount);

//! Retrieves the supported network channel numbers.
/*!
	In order to retrieve the supported network channel values, the method has to be called twice: First call it with parameter \a supportedNetworkChannels = NULL, then the method returns the number of available network channels. Using this number, allocate an array of size \a nw_channel_count and call the method again to retrieve the channels themselves.

	\param connectionHandle					The connection handle whose associated data acquisition contains the device to retrieve the supported network channels for.
	\param deviceName						The name of the device to retrieve the supported network channels for.
	\param supportedNetworkChannels			The array where the network channel values will be stored in.
	\param supportedNetworkChannelsCount	A pointer to the variable that holds the number of available network channels.

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
*/
GDSCLIENTAPI_API GDS_RESULT __stdcall GDS_GNAUTILUS_GetSupportedNetworkChannels(GDS_HANDLE connectionHandle, const char (&deviceName)[DEVICE_NAME_LENGTH_MAX], uint32_t *supportedNetworkChannels, size_t *supportedNetworkChannelsCount);

//! Retrieves the supported input sources.
/*!
	In order to retrieve the supported input sources, the method has to be called twice: First call it with paramter \a supportedInputSources = NULL, then the method returns the number of available input sources. Using this number, allocate an array of size \a supportedInputSourcesCount and call the method again to retrieve the input sources themselves.

	\param connectionHandle			The connection handle whose associated data acquisition contains the device to retrieve the supported input sources for.
	\param deviceName				The name of the device to retrieve the supported input sources for.
	\param supportedInputSources	The array where the input sources will be stored in. 
	\param input_soruces_length		A pointer to the variable that holds the number of available input sources.

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
*/
GDSCLIENTAPI_API GDS_RESULT	__stdcall GDS_GNAUTILUS_GetSupportedInputSources(GDS_HANDLE connectionHandle, const char (&deviceName) [DEVICE_NAME_LENGTH_MAX], GDS_GNAUTILUS_INPUT_SIGNAL *supportedInputSources, size_t *supportedInputSourcesCount);

//! Sets the specified channel for network communication between head set and base station.
/*!
	The function sets the specified network channel for the heads as well as for the base station since both need to connect via the same channel.

	\param connectionHandle	The connection handle whose associated data acquisition contains the device to set the specified channel number.
	\param deviceName		The name of the device to retrieve for which the specified channel number shall be set.
	\param networkChannel	The network channel number to use.

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
*/
GDSCLIENTAPI_API GDS_RESULT __stdcall GDS_GNAUTILUS_SetNetworkChannel(GDS_HANDLE connectionHandle, const char (&deviceName)[DEVICE_NAME_LENGTH_MAX], uint32_t networkChannel);

//! Retrieves the current network channel number used for communication between head set and base station.
/*!
	The function retrieves the current network channel number into \a networkChannel.

	\param connectionHandle	The connection handle whose associated data acquisition contains the device to retrieve the network channel from.
	\param deviceName		The name of the device to retrieve the network channel for.
	\param networkChannel	A pointer to the variable that receives the channel number. 

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
*/
GDSCLIENTAPI_API GDS_RESULT __stdcall GDS_GNAUTILUS_GetNetworkChannel(GDS_HANDLE connectionHandle, const char (&deviceName)[DEVICE_NAME_LENGTH_MAX], uint32_t *networkChannel);

//! Gets the scaling values for the specified g.Nautilus device of the data acquisition session associated with the specified connection handle.
/*!
	\param connectionHandle	The connection handle whose associated data acquisition session contains the device to get the scaling values for.
	\param deviceName		The name of the device to get the scaling values for.
	\param scaling			A pointer to an allocated \ref GDS_GNAUTILUS_SCALING structure that receives the scaling values of the specified device.

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
 */
GDSCLIENTAPI_API GDS_RESULT __stdcall GDS_GNAUTILUS_GetScaling(GDS_HANDLE connectionHandle, const char (&deviceName)[DEVICE_NAME_LENGTH_MAX], GDS_GNAUTILUS_SCALING *scaling);

//! Sets the scaling values for the specified g.Nautilus device of the data acquisition session associated with the specified connection handle.
/*!
	\ref GDS_GNAUTILUS_Calibrate may return NaN values for channels it cannot calculate scaling values for.
	Any NaN values in \a scaling's \ref GDS_GNAUTILUS_SCALING::Factor array will be replaced by 1 before they're sent to the device.
	Any NaN values in \a scaling's \ref GDS_GNAUTILUS_SCALING::Offset array will be replaced by 0 before they're sent to the device.

	\param connectionHandle	The connection handle whose associated data acquisition session contains the device to set the scaling values for.
	\param deviceName		The name of the device to set the scaling values for.
	\param scaling			A pointer to an initialized \ref GDS_GNAUTILUS_SCALING structure that contains the scaling values to set.

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
 */ 
GDSCLIENTAPI_API GDS_RESULT __stdcall GDS_GNAUTILUS_SetScaling(GDS_HANDLE connectionHandle, const char (&deviceName)[DEVICE_NAME_LENGTH_MAX], GDS_GNAUTILUS_SCALING *scaling);

//! Resets the device's scaling to a neutral calibration (offset = 0, scaling factor = 1)
/*!
	\param connectionHandle	The connection handle whose associated data acquisition contains the specified g.Nautilus device.
	\param deviceName		The name of the device for which the calibration configuration shall be reset.

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
*/
GDSCLIENTAPI_API GDS_RESULT __stdcall GDS_GNAUTILUS_ResetScaling(GDS_HANDLE connectionHandle, const char(&deviceName)[DEVICE_NAME_LENGTH_MAX]);

//! Calculates, but does not yet apply new scaling values for each synchronously sampled analog input channel under the current conditions if the calibration set is applied.
/*!
	The function calibrates the device and stores the new calibration values in the configuration.
	The returned values will not be applied unless they're explicitly set with \ref GDS_GNAUTILUS_SetScaling.

	Calibration is required if amplitude and/or offset of the known input signal differs from the measured signal.

	The function returns quiet NaN values (\c std::numeric_limits<float>::quiet_NaN()) as scaling factor and offset for channels that failed calibration.

	\param connectionHandle	The connection handle whose associated data acquisition contains the device to calibrate.
	\param deviceName		The name of the device which is to be calibrated.
	\param scaling			A pointer to an allocated \ref GDS_GNAUTILUS_SCALING structure that receives the new scaling values calculated under the current conditions for each synchronously sampled analog input channel.

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
*/
GDSCLIENTAPI_API GDS_RESULT __stdcall GDS_GNAUTILUS_Calibrate(GDS_HANDLE connectionHandle, const char (&deviceName)[DEVICE_NAME_LENGTH_MAX], GDS_GNAUTILUS_SCALING *scaling);

//! Gets the list of available bandpass filters for the device.
/*!
	The functions retrieves the list of available bandpass filters for the specified g.Nautilus device. \a bandpassFilters is a pointer to an array that has to be allocated by the caller (of size \a bandpassFiltersCount). To determine the \a bandpassFiltersCount, call \ref GDS_GNAUTILUS_GetBandpassFilters with parameter \a bandpassFilters set to \c NULL.
	The function will return the number of available bandbass filters within the parameter \a bandpassFiltersCount. Use this value to allocate the filter array properly and call \ref GDS_GNAUTILUS_GetBandpassFitlers a second time in order to retrieve the filter list.

	\param connectionHandle		The connection handle whose associated data acquisition contains the device to get the available bandpass filters for.
	\param deviceName			The name of the device for which the bandpass filter list shall be retrieved.
	\param bandpassFilters		A pointer to an allocated array of type \ref GDS_FILTER_INFO and size \a bandpassFiltersCount. OR: \c NULL when aiming to retrieve the number of available bandpass filters only within the parameter \a bandpassFiltersCount.
	\param bandpassFiltersCount	A pointer to a variable that receives the number of available bandpass filters for the g.Nautilus device.

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
*/
GDSCLIENTAPI_API GDS_RESULT __stdcall GDS_GNAUTILUS_GetBandpassFilters(GDS_HANDLE connectionHandle, const char (&deviceName)[DEVICE_NAME_LENGTH_MAX], GDS_FILTER_INFO *bandpassFilters, size_t *bandpassFiltersCount);

//! Gets the list of available notch filters for the device.
/*!
	The function retrieves the list of available notch filters for the specified g.Nautilus device. \a notchFilters is a pointer to an array that has to be allocated by the caller (of size \a notchFiltersCount). To determine the \a notchFiltersCount, call \ref GDS_GNAUTILUS_GetNotchFilters with parameter \a notchFilters set to \c NULL. 
	The function will return the number of available notch filters within the parameter \a notchFiltersCount. Use this value to allocate the filter array properly and call \ref GDS_GNAUTILUS_GetNotchFilters a second time in order to retrieve the filter list.

	\param connectionHandle		The connection handle whose associated data acquisition contains the device to get the available notch filters for.
	\param deviceName			The name of the device for which the notch filter list shall be retrieved. 
	\param notchFilters			A pointer to an allocated array of type \ref GDS_FILTER_INFO and size \a notchFiltersCount. OR: \c NULL when aiming to retrieve the number of available notch filters only within the paramter \a notchFiltersCount.
	\param notchFiltersCount	A pointer to a variable that receives the number of available notch filters for the g.Nautilus device.

	\returns A \ref GDS_RESULT sturcture containing information about the operation's success.
*/
GDSCLIENTAPI_API GDS_RESULT __stdcall GDS_GNAUTILUS_GetNotchFilters(GDS_HANDLE connectionHandle, const char (&deviceName)[DEVICE_NAME_LENGTH_MAX], GDS_FILTER_INFO *notchFilters, size_t *notchFiltersCount); 

//! Resets the bandpass filter entry for every channel such that no bandpass filtering is performed at all.
/*!
	\param connectionHandle	The connection handle whose associated data acquisition contains the specified g.Nautilus device.
	\param deviceName		The name of the device for whose specified channel the filter shall be set.

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
*/
GDSCLIENTAPI_API GDS_RESULT __stdcall GDS_GNAUTILUS_ResetBandpassFilters(GDS_HANDLE connectionHandle, const char (&deviceName)[DEVICE_NAME_LENGTH_MAX]);

//! Resets the notch filter entry for every channel such that no notch filtering is performed at all.
/*!
	\param connectionHandle	The connection handle whose associated data acquisition contains the specified g.Nautilus device.
	\param deviceName		The name of the device for whose specified channel the filter shall be set.

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
*/
GDSCLIENTAPI_API GDS_RESULT __stdcall GDS_GNAUTILUS_ResetNotchFilters(GDS_HANDLE connectionHandle, const char (&deviceName)[DEVICE_NAME_LENGTH_MAX]);

//! Resets the bipolar filter entry for every channel such that no bipolar filtering is performed at all.
/*!
	\param connectionHandle	The connection handle whose associated data acquisition contains the specified g.Nautilus device.
	\param deviceName		The name of the device for whose specified channel the filter shall be set.

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
*/
GDSCLIENTAPI_API GDS_RESULT	__stdcall GDS_GNAUTILUS_ResetBipolarFilters(GDS_HANDLE connectionHandle, const char(&deviceName)[DEVICE_NAME_LENGTH_MAX]);

//! Measures impedance of each input channel in ohm.
/*!
	Electrode 'Cz' plays a special role in impedance measurement and must always be connected.
	The method throws an exception if an invalid reference signal is measured on electrode 'Cz' (maybe due to a not connected electrode), which does not allow calculation of impedance values of all other channels.

	All returned impedance values are rounded to the nearest integral kilo-ohm value. The method returns quiet NaN values (\c std::numeric_limits<double>::quiet_NaN()) for not available channels. The availability of a channel can be determined using \ref GDS_GNAUTILUS_GetAvailableChannels.
	All measured values are clipped within a range from 5,000 ohm to 500,000 ohm. However, if the impedance value cannot be measured exactly, the method returns a value of 4,000 ohm for any estimated impedance value below 5,000 ohm, and 501,000 for any estimated impedance value above 500,000 ohm.

	\param connectionHandle	The connection handle whose associated data acquisition session contains the device to measure impedance for.
	\param deviceName		The name of the device to measure impedance for.	
	\param impedances		An allocated array that receives the impedance values in ohm for each channel. An element at zero-based index <i>i</i> receives the impedance value in ohm of zero-based channel number <i>i</i>, or \c std::numeric_limits<double>::quiet_NaN() (see extended description of the method for details).

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
 */
GDSCLIENTAPI_API GDS_RESULT __stdcall GDS_GNAUTILUS_GetImpedance(GDS_HANDLE connectionHandle, const char (&deviceName)[DEVICE_NAME_LENGTH_MAX], double (&impedances)[GDS_GNAUTILUS_CHANNELS_MAX]);

//! Starts continuous impedance measurement asynchronously and calls the specified callback for each completed measurement cycle.
/*!
	This method returns immediately. The \a impedanceMeasurementCallback function will be called after each completed impedance measurement cycle until the \ref GDS_GNAUTILUS_StopImpedanceMeasurement method is called.
	If an error occurs during impedance measurement, it is forwarded to the callback as parameter. Errors occur in the same situations as for \ref GDS_GNAUTILUS_GetImpedance.

	If the \c result parameter in the callback handler (see \ref GDS_GNAUTILUS_ImpedanceMeasurementCallback) indicates an error and the \c aborted flag of the callback handler equals \c true as well, impedance measurement has been stopped automatically and can be started again with \ref GDS_GNAUTILUS_StartImpedanceMeasurement.
	If the \c result parameter in the callback handler (see \ref GDS_GNAUTILUS_ImpedanceMeasurementCallback) indicates an error but the \c aborted flag of the callback handler equals \c false, only the current measurement cycle is affected but impedance measurement continues.

	\param connectionHandle				The connection handle whose associated data acquisition session contains the device to start impedance measurement for.
	\param deviceName					The name of the device to start impedance measurement for.
	\param impedanceMeasurementCallback	The callback method to execute after each completed impedance measurement cycle.
	\param userData						The user data which should be passed to the callback.

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
 */
GDSCLIENTAPI_API GDS_RESULT __stdcall GDS_GNAUTILUS_StartImpedanceMeasurement(GDS_HANDLE connectionHandle, const char (&deviceName)[DEVICE_NAME_LENGTH_MAX], GDS_GNAUTILUS_ImpedanceMeasurementCallback impedanceMeasurementCallback, void *userData = NULL);

//! Stops continuous, asynchronous impedance measurement started with \ref GDS_GNAUTILUS_StartImpedanceMeasurement before.
/*!
	\param connectionHandle		The connection handle whose associated data acquisition session contains the device to stop impedance measurement for.
	\param deviceName			The name of the device to stop impedance measurement for.
	\param waitUntilCompleted	\c true to block until impedance measurement has been completed; \c false to return immediately.

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
 */
GDSCLIENTAPI_API GDS_RESULT __stdcall GDS_GNAUTILUS_StopImpedanceMeasurement(GDS_HANDLE connectionHandle, const char (&deviceName)[DEVICE_NAME_LENGTH_MAX], BOOL waitUntilCompleted);

#ifdef __cplusplus
}
#endif

#endif //GDSCLIENTAPI_GNAUTILUS_HPP


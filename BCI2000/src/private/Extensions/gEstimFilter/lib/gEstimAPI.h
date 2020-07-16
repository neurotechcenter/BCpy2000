// vi:syntax=c
// this file is generated from 'gEstimApi.h.stpl'

/// \file gEstimAPI.h
/// \brief gEstim API 
///
/// This file contains the API definitions (beyond what is in gEstimTypes.h) and the functions needed to control the g.Estim device.
/// Most of it applies to the g.EstimPRO as well es g.EstimFES device.

// C API to control g.Estim
// Roland Puntaier (puntaier@gtec.at)
// 2016-06-30
// Guger Technologies OG


#ifndef GESTIMAPI_H
#define GESTIMAPI_H

/// \cond Doxygen_Suppress
#if defined _WIN32 || defined __CYGWIN__
  #ifdef BUILDING_DLL
    #ifdef __GNUC__
      #define DLL_PUBLIC __attribute__ ((dllexport))
    #else
      #define DLL_PUBLIC __declspec(dllexport) 
    #endif
  #else
    #ifdef __GNUC__
      #define DLL_PUBLIC __attribute__ ((dllimport))
    #else
      #define DLL_PUBLIC __declspec(dllimport)
    #endif
  #endif
  #define DLL_LOCAL
#else
  #if __GNUC__ >= 4
    #define DLL_PUBLIC __attribute__ ((visibility ("default")))
    #define DLL_LOCAL  __attribute__ ((visibility ("hidden")))
  #else
    #define DLL_PUBLIC
    #define DLL_LOCAL
  #endif
#endif
/// \endcond

#include "gEstimTypes.h"
#include "gEstimDefaults.h"


#ifdef __cplusplus
extern "C" {
#endif 

#define G_ESTIM_PRO "g.EstimPRO" //!< This is the name of the device. It can be g.EstimPRO or g.EstimFES. The identifier field of \ref _GDevice holds this string.
#define API_VERSION "1.18.00.4104"    //!< This is the API version. It is the same as in the AssemblyInfo.

#define MAX_CALLBACK_FIELDSCHANGED 11 //!< Only this many callback functions can be registered with \ref register_with_polling()

#define G_ESTIM_FW {"V1.2","V1.02o",NULL} ///< FW Versions (newest first). FW_version_ must fit up to the length of this string.
#define G_ESTIM_HW {"V1.3","V1.2","V1.1","A1_V1.2","A1_V1.1",NULL} ///< HW Version (newest first). It must fit to the Hardware version up to the length of this string.
#define G_ESTIM_FW_HW_COUNT 5 /// count of combinations of FW and HW version that are recognized
#define G_ESTIM_FW_HW_IDX { {0,0},{1,1},{1,2},{1,3},{1,4} } ///< only these combinations of G_ESTIM_FW and G_ESTIM_HW are allowed

#ifdef GESTIMPRO
#define G_ESTIM_SIM "ES-SIM0.SI.M0" ///< The serial of the device, when in simulation mode
#else
#define G_ESTIM_SIM "EF-SIM0.SI.M0" ///< The serial of the device, when in simulation mode
#endif
#define G_ESTIM_HW_SIM "V1.3_HW-SIM"     ///< The value used for HW_version_, when in simulation mode


/** variables are identified by these IDs
- in \ref error_field \ref st_default \ref st_maximum \ref st_minimum \ref st_defaults
- in \ref error_fields() return value
*/
typedef enum
{
  GFIELD_SERIAL = 0,
  GFIELD_IDENTIFIER = 1,
  GFIELD_FW_VERSION = 2,
  GFIELD_HW_VERSION = 3,
  GFIELD_COEFF_CS1 = 4,
  GFIELD_COEFF_CS2 = 5,
  GFIELD_COEFF_I = 6,
  GFIELD_COEFF_V = 7,
  GFIELD_COEFF_R = 8,
  GFIELD_VOLTAGE_LIMIT = 9,
  GFIELD_ALTERNATE = 10,
  GFIELD_PHASE_CURRENT1 = 11,
  GFIELD_PHASE_CURRENT2 = 12,
  GFIELD_PHASE_DURATION1 = 13,
  GFIELD_PHASE_DURATION2 = 14,
  GFIELD_FADE_IN_TIME = 15,
  GFIELD_FADE_OUT_TIME = 16,
  GFIELD_FADE_TYPE = 17,
  GFIELD_INTERPHASE = 18,
  GFIELD_PULSE_RATE = 19,
  GFIELD_PULSES = 20,
  GFIELD_DO1_PRE_TIME = 21,
  GFIELD_DO1_POST_TIME = 22,
  GFIELD_DO2_PRE_TIME = 23,
  GFIELD_DO2_POST_TIME = 24,
  GFIELD_DO1_FUNCTION = 25,
  GFIELD_DO2_FUNCTION = 26,
  GFIELD_DIO_ENABLED = 27,
  GFIELD_HIGHVOLTAGE_LED = 28,
  GFIELD_O_FUNCTION = 29,
  GFIELD_STATE = 30,
  GFIELD_TOTAL_CHARGE = 31,
  GFIELD_EVENT = 32,
  GFIELD_ERROR = 33,
  GFIELD_STIM_EVENT = 34,
  GFIELD_BATTERY1 = 35,
  GFIELD_BATTERY2 = 36,
  GFIELD_DI_STATE = 37,
  GFIELD_FADE_PROGRESS = 38,
  GFIELD_CURRENT1_SUM = 39,
  GFIELD_CURRENT2_SUM = 40,
  GFIELD_VOLTAGE1_SUM = 41,
  GFIELD_VOLTAGE2_SUM = 42,
  GFIELD_DURATION1_SUM = 43,
  GFIELD_DURATION2_SUM = 44,
  GFIELD_MIN_CURRENT_PH1 = 45,
  GFIELD_MAX_CURRENT_PH1 = 46,
  GFIELD_MIN_CURRENT_PH2 = 47,
  GFIELD_MAX_CURRENT_PH2 = 48,
  GFIELD_APPLIED_PULSES = 49,
  GFIELD_RESISTANCE = 50,
  GFIELD_CURRENT = 51,
  GFIELD_VOLTAGE = 52,
  GFIELD_TRAIN_RATE = 53,
  GFIELD_N_TRAINS = 54,
  GFIELD_JITTER = 55,
  GFIELD_OPERATIONMODE = 56,
  GFIELD_EL_TYPE = 57,
  GFIELD_EL_CIRC_DIAMETER = 58,
  GFIELD_EL_DEPTH_DIAMETER = 59,
  GFIELD_EL_DEPTH_LENGTH = 60,
  GFIELD_EL_OTHER_SURFACE = 61,
  GFIELD_APPLIED_TRAINS = 62,
  GFIELD_REMAINING = 63,
  GFIELD_END
} GField;


/// \brief These are the offsets of the \ref _GDevice fields. One can also get them using \ref rwcode
typedef enum
{
  RW_SERIAL = 0,
  RW_IDENTIFIER = 16,
  RW_FW_VERSION = 80,
  RW_HW_VERSION = 100,
  RW_COEFF_CS1 = 120,
  RW_COEFF_CS2 = 124,
  RW_COEFF_I = 128,
  RW_COEFF_V = 132,
  RW_COEFF_R = 136,
  RW_VOLTAGE_LIMIT = 140,
  RW_ALTERNATE = 142,
  RW_PHASE_CURRENT1 = 144,
  RW_PHASE_CURRENT2 = 146,
  RW_PHASE_DURATION1 = 148,
  RW_PHASE_DURATION2 = 150,
  RW_FADE_IN_TIME = 152,
  RW_FADE_OUT_TIME = 154,
  RW_FADE_TYPE = 156,
  RW_INTERPHASE = 158,
  RW_PULSE_RATE = 160,
  RW_PULSES = 162,
  RW_DO1_PRE_TIME = 164,
  RW_DO1_POST_TIME = 166,
  RW_DO2_PRE_TIME = 168,
  RW_DO2_POST_TIME = 170,
  RW_DO1_FUNCTION = 172,
  RW_DO2_FUNCTION = 174,
  RW_DIO_ENABLED = 176,
  RW_HIGHVOLTAGE_LED = 178,
  RW_O_FUNCTION = 180,
  RW_STATE = 182,
  RW_TOTAL_CHARGE = 184,
  RW_EVENT = 188,
  RW_ERROR = 190,
  RW_STIM_EVENT = 192,
  RW_BATTERY1 = 194,
  RW_BATTERY2 = 196,
  RW_DI_STATE = 198,
  RW_FADE_PROGRESS = 200,
  RW_CURRENT1_SUM = 202,
  RW_CURRENT2_SUM = 206,
  RW_VOLTAGE1_SUM = 210,
  RW_VOLTAGE2_SUM = 214,
  RW_DURATION1_SUM = 218,
  RW_DURATION2_SUM = 220,
  RW_MIN_CURRENT_PH1 = 222,
  RW_MAX_CURRENT_PH1 = 224,
  RW_MIN_CURRENT_PH2 = 226,
  RW_MAX_CURRENT_PH2 = 228,
  RW_APPLIED_PULSES = 230,
  RW_RESISTANCE = 232,
  RW_CURRENT = 234,
  RW_VOLTAGE = 1034,
  RW_END = 1834
} RW;



/** If the socket port is set, the API gets into simulation mode.

\param port	The winsocket localhost port number

Later, when opening a new device, if it is not possible to connect to the specified port,
the device, that is being created, becomes a server and listens at the specified port.
I.e. the process first calling \ref openDevice() becomes the server, the second one becomes the client.

*/
DLL_PUBLIC void set_socket_port_for_sim(int port);


/** Make entries it the file %APPDATA%/g.EstimPRO/g.EstimFES.log or %APPDATA%/g.EstimFES/g.EstimFES.log

There is only one debug log. It is emptied at each start.

*/
DLL_PUBLIC void debugLog(
    const char *info //!< The text for the entry is appended to the file.
    );

/** Toggles the debug log and returns the new value.

If debuglog is on, the file %APPDATA%\<appname>\<appname>.log is started from scratch at every start.
*/
DLL_PUBLIC bool toggle_debuglog();


/** Check whether a dongle with the feature 106 is present.

Without this feature it is not possible to use the advanced mode (GESTIM_MODE_ADVANCED).
*/
DLL_PUBLIC bool has_dongle();


/** Creates a log file for the session in folder C:/users/user/Documents/gtec/g.EstimPRO or C:/users/user/Documents/gtec/g.EstimFES

The file name will contain the mode parameter, but in addition it will have the following format:

For PRO:

\verbatim

  gEstimPro_<mode>_<TD>.csv
      mode should be <ST>_<SessionName>
        ST = (ECS|HHD|ADV)
        SessionName is a name given by the user
      TD = day and time

Example: gEstimPRO_ADV_MySession_ES-2017.10.01_2016-06-24-10-42-25.csv

\endverbatim

For FES: gEstimFES instead of gEstimPRO.

*/
DLL_PUBLIC void newSessionLog(
        PGDevice d,   //!< The session log is per device. This is the device handle.
        char *mode    //!< The string representing the mode of the device (ECS, HHD, ADV,...)
        );


/** Get the current session log file

*/
DLL_PUBLIC const char* gt_sessionLogFile(
        PGDevice d   //!< The session log is per device. This is the device handle.
        );

/** Make a new log entry.

\verbatim

entrytype;mode;datetime;entrytext;comment

mode = ECS|HHD|ADV
       ECS = Basic Mode    
       HHD = Handheld Mode 
       ADV = Advanced Mode 

datetime = %Y-%m-%d-%H-%M-%S-%f

entrytype used currently = 
     Electrode Changed 
   | End Session       
   | State             
   | Pulse             
   | Sequence          
   | Start Session     
   | Train             
   | User Entry        
   | Warning           

\endverbatim

Example:

\verbatim
sessionLog(d,"State","Ready for stimulation, last measured values: 0,2723V 0,05mA","");
\endverbatim

results in

\verbatim
State;2016-06-24-19-54-29.179;Ready for stimulation, last measured values: 0,2723V 0,05mA;
\endverbatim


*/
DLL_PUBLIC void sessionLog(
    PGDevice d, //!< The session log is per device. This is the device handle.
    char *entrytype, //!< one of a set of predefined strings (see in text), but not mandatorily so
    char *entrytext, //!< the mein text of the log entry
    char *comment    //!< additional comment for the log entry
    );




/** Finds and opens a connected g.Estim device.

\return true for success, else the last GError can be found via gt_error() also with parameter NULL

If ``serial=NULL``, this function acts like an iterator, 
i.e. a second call will open the second device up to \ref MAX_DEVICES.
If a serial numbers is given, then exactly that device is searched and opened.
If no (more) device exits, false is returned.
If the first parameter is NULL, then the iteration is reset. 

openDevice() provides iterator functionality. This means that a subsequent call will
open the next device until the iterator state is reset.

Use \ref openDevices() to open all connected available devices.

Use \ref closeDevices() to close all open devices and reset the iteration.

Call \ref closeDevice() to close only the device d.

There are only #MAX_DEVICES simultaneously open devices possible. 

If sim_port was provided via \ref set_socket_port_for_sim(), simulation mode is used (serial == ES-SIM0.SI.M0).
The first device in the first process will be server and simulate the device. It will enter a "while(1) tfr_respond();".
The second process calling openDevice() will be the winsocket client and control the device (active part in the communication).
The communication is done over `http://localhost:<sim_port>`.

*/
DLL_PUBLIC bool openDevice(
    PGDevice *d,              //!< receives the device; reset iteration with NULL
    char serial[SERIAL_SIZE], //!< NULL -> connect to first of kind, else exactly to the device with that serial
    bool check_version        //!< If true only specific hardcoded FW/HW versions are accepted.
        );

/** Closes the g.Estim device.

@return true for success, else the last GError can be found via gt_error()

\see openDevice()

*/
DLL_PUBLIC bool closeDevice(
        PGDevice d //!< device handle
        );

/** open all connected devices

*/
DLL_PUBLIC void openDevices(
    bool check_version //!< true: a strict version compatibility check with FW is done. false: no such check can lead to troubles if versions differ
    );

/** Return all open devices

  @return pointer to array of device handles of size \ref MAX_DEVICES

*/
DLL_PUBLIC PGDevice *gt_openDevices();

/** count open devices

  @return number of open devices

*/
DLL_PUBLIC int count_openDevices();

/** close all open devices

  @return pointer to array of device handles of size \ref MAX_DEVICES

This function also resets the iterator state of openDevice().

*/
DLL_PUBLIC void closeDevices();

/** Is this device open?

*/
DLL_PUBLIC bool isOpenDevice(
        PGDevice d //!< device handle
        );


/*
[rcv_snd]

All communication to the device is done in a separate thread. 
This function only returns, whether it was possible to communicate to that thread.
It returns true, if successly handed over to the communication thread.

Use wait_for_end_of_action() to wait for the actual communication to the device to complete.

Use gt_error() to find out the error, that resulted from the communication to the device.

[rcv_snd]
*/

/** Main gEstim protocol function called by rcv_ and snd_ functions.

request() might wait for the completion in the communication thread, 
if it is a read or a write while not stimulating.
In general expect request() to return before completion of the command
and use wait_for_end_of_action(d) to wait for completion.

*/
DLL_PUBLIC bool request(
    PGDevice d,      //!< opaque API device struct pointer
    int command,     //!< command: read or write (\see GCommand)
    uint16_t offset, //!< offset of member (\see RW \see rwcode)
    uint16_t length  //!< given by sizeof that member or sum sizes for consecutive members
    );

/** Requests the full record representing the current status of the device to a local copy (without the sample, though).


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool rcv_all(
        PGDevice d //!< device handle
        );

/** Retrieves stimulation parameters from device.


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool rcv_stim(
        PGDevice d //!< device handle
        );

/** Retrieves last pulse samples as recorded by the device, i.e GDevice::current_ and GDevice::voltage_


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool rcv_samples(
        PGDevice d //!< device handle
        );



/** Checks if it is a valid state change from current to new state

Possible state changes are: GSTATE_STOP->GSTATE_ACTIVE, GSTATE_STOP->GSTATE_LOCK
From GSTATE_ACTIVE and GSTATE_STIMULATION every state change is possible.
GSTATE_RESISTANCE is only possible from GSTATE_ACTIVE. 

State transition from any state to itself, is valid, but ignored

\return \ref GError value, and specifically GERROR_SUCCESS, if the state change is possible.

*/
DLL_PUBLIC GError error_state_change(
        PGDevice d,   //!< device handle
        GState state //!< the new \ref GState value
        );

/** Checks whether the field produces any inconsistencies with the limits or other fields.

Setting a field via the st_xxx() functions does not return an error, if the value is wrong.
When calling activate() or a snd_xxx() function, the device would return an error.

This function allows to see whether the field is OK, before sending the field to the device.

Internally this function calls error_device_field() for fields that are part of the configuration.
There is also error_state_change() that is called by the device on a state change.

See error_fields() for the consistency requirements.

\return GERROR_SUCCESS or an appropriate \ref GError code.

*/
DLL_PUBLIC GError error_field(
        PGDevice d, //!< device handle
        int id     //!< the \ref GField value
        );

/** Check if any of the fields produces an inconsistency.

\return a uint64_t where \ref GField enums identify the bit that produces an error

This function allows to see whether the config is OK, before sending it to the device.

Consistency requirements:

- The fields must be within the according minimum and maximum (\ref defaults)
- In addition:

State

\snippet gEstimDefaults.c doxyreq

Other fields

\snippet gEstimDefaults.c doxyreq0
\snippet gEstimDefaults.c doxyreq1
\snippet gEstimDefaults.c doxyreq2
\snippet gEstimDefaults.c doxyreq3
\snippet gEstimDefaults.c doxyreq4
\snippet gEstimDefaults.c doxyreq5
\snippet gEstimDefaults.c doxyreq6
\snippet gEstimDefaults.c doxyreq7

*/
DLL_PUBLIC uint64_t error_fields(
        PGDevice d //!< device handle
        );

/** Set default value in accordance with the mode the device is in (\ref st_operationmode() \ref gt_operationmode())

\return true, unless it is not allowed to set this value.

*/
DLL_PUBLIC bool st_default(
        PGDevice d, //!< device handle
        int id     //!< the \ref GField value
        );

/** Set maximum value in accordance with the mode the device is in (\ref st_operationmode() \ref gt_operationmode())

\return true, unless it is not allowed to set this value.

*/
DLL_PUBLIC bool st_maximum(
        PGDevice d, //!< device handle
        int id     //!< the \ref GField value
        );

/** Set minimum value in accordance with the mode the device is in (\ref st_operationmode() \ref gt_operationmode())

\return true, unless it is not allowed to set this value.

*/
DLL_PUBLIC bool st_minimum(
        PGDevice d, //!< device handle
        int id     //!< the \ref GField value
        );

/** Set default values in accordance with the mode the device is in (\ref st_operationmode() \ref gt_operationmode())

\return true

*/
DLL_PUBLIC bool st_defaults(
        PGDevice d //!< device handle
        );





/** Return the value of this field that is not part of the device config \ref _GDevice

[1Hz]. The frequency of the train must be such as to fit in gt_train_duration().

*/
DLL_PUBLIC double gt_train_rate(volatile GDevice *d);

/** Set the value of this field that is not part of the device config \ref _GDevice

[1Hz]. The frequency of the train must be such as to fit in gt_train_duration().

*/
DLL_PUBLIC bool st_train_rate(
    PGDevice d, //!< device handle
    double v //!< new value to be set locally, i.e. this is not yet transmitted to the device
    );

/** Return the value of this field that is not part of the device config \ref _GDevice

[1]. More than one train (=sequence) are handled by the API via the parallel communication thread. wait_for_end_of_sequence() waits (and blocks) until all the trains are done, which can take hours.

*/
DLL_PUBLIC uint32_t gt_n_trains(volatile GDevice *d);

/** Set the value of this field that is not part of the device config \ref _GDevice

[1]. More than one train (=sequence) are handled by the API via the parallel communication thread. wait_for_end_of_sequence() waits (and blocks) until all the trains are done, which can take hours.

*/
DLL_PUBLIC bool st_n_trains(
    PGDevice d, //!< device handle
    uint32_t v //!< new value to be set locally, i.e. this is not yet transmitted to the device
    );

/** Return the value of this field that is not part of the device config \ref _GDevice

[1%]. A value other than 0 (0&lt;1&lt;=100) will randomly choose a distance between trains of a sequence from 0 to the value derived from \ref GExtra::train_rate_((period–train_duration)*(1+(rand(jitter_)-jitter_)/100))

*/
DLL_PUBLIC uint32_t gt_jitter(volatile GDevice *d);

/** Set the value of this field that is not part of the device config \ref _GDevice

[1%]. A value other than 0 (0&lt;1&lt;=100) will randomly choose a distance between trains of a sequence from 0 to the value derived from \ref GExtra::train_rate_((period–train_duration)*(1+(rand(jitter_)-jitter_)/100))

*/
DLL_PUBLIC bool st_jitter(
    PGDevice d, //!< device handle
    uint32_t v //!< new value to be set locally, i.e. this is not yet transmitted to the device
    );

/** Return the value of this field that is not part of the device config \ref _GDevice

[1]. \ref GESTIM_MODE_COUNT values: \ref GESTIM_MODE_DEVICE (default) \ref GESTIM_MODE_BASIC \ref GESTIM_MODE_HANDHELD \ref GESTIM_MODE_ADVANCED \ref GESTIM_MODE_FES (the last is set automatically). These are stored in the bits given by \ref GESTIM_MODE_MASK. Other internal options are stored on the other bits.

*/
DLL_PUBLIC int gt_operationmode(volatile GDevice *d);

/** Set the value of this field that is not part of the device config \ref _GDevice

[1]. \ref GESTIM_MODE_COUNT values: \ref GESTIM_MODE_DEVICE (default) \ref GESTIM_MODE_BASIC \ref GESTIM_MODE_HANDHELD \ref GESTIM_MODE_ADVANCED \ref GESTIM_MODE_FES (the last is set automatically). These are stored in the bits given by \ref GESTIM_MODE_MASK. Other internal options are stored on the other bits.

*/
DLL_PUBLIC bool st_operationmode(
    PGDevice d, //!< device handle
    int v //!< new value to be set locally, i.e. this is not yet transmitted to the device
    );

/** Return the value of this field that is not part of the device config \ref _GDevice

[1]. An electrode type: \ref ELECTRODE_CIRCULAR \ref ELECTRODE_OTHER \ref ELECTRODE_DEPTH \ref ELECTRODE_NONE

*/
DLL_PUBLIC int gt_el_type(volatile GDevice *d);

/** Set the value of this field that is not part of the device config \ref _GDevice

[1]. An electrode type: \ref ELECTRODE_CIRCULAR \ref ELECTRODE_OTHER \ref ELECTRODE_DEPTH \ref ELECTRODE_NONE

*/
DLL_PUBLIC bool st_el_type(
    PGDevice d, //!< device handle
    int v //!< new value to be set locally, i.e. this is not yet transmitted to the device
    );

/** Return the value of this field that is not part of the device config \ref _GDevice

[1um]. The diameter for \ref ELECTRODE_CIRCULAR \ref GExtra::el_type_

*/
DLL_PUBLIC uint32_t gt_el_circ_diameter(volatile GDevice *d);

/** Set the value of this field that is not part of the device config \ref _GDevice

[1um]. The diameter for \ref ELECTRODE_CIRCULAR \ref GExtra::el_type_

*/
DLL_PUBLIC bool st_el_circ_diameter(
    PGDevice d, //!< device handle
    uint32_t v //!< new value to be set locally, i.e. this is not yet transmitted to the device
    );

/** Return the value of this field that is not part of the device config \ref _GDevice

[1um]. The diameter for \ref ELECTRODE_DEPTH \ref GExtra::el_type_

*/
DLL_PUBLIC uint32_t gt_el_depth_diameter(volatile GDevice *d);

/** Set the value of this field that is not part of the device config \ref _GDevice

[1um]. The diameter for \ref ELECTRODE_DEPTH \ref GExtra::el_type_

*/
DLL_PUBLIC bool st_el_depth_diameter(
    PGDevice d, //!< device handle
    uint32_t v //!< new value to be set locally, i.e. this is not yet transmitted to the device
    );

/** Return the value of this field that is not part of the device config \ref _GDevice

[1um]. The depth for \ref ELECTRODE_DEPTH \ref GExtra::el_type_

*/
DLL_PUBLIC uint32_t gt_el_depth_length(volatile GDevice *d);

/** Set the value of this field that is not part of the device config \ref _GDevice

[1um]. The depth for \ref ELECTRODE_DEPTH \ref GExtra::el_type_

*/
DLL_PUBLIC bool st_el_depth_length(
    PGDevice d, //!< device handle
    uint32_t v //!< new value to be set locally, i.e. this is not yet transmitted to the device
    );

/** Return the value of this field that is not part of the device config \ref _GDevice

[1um^2]. The surface for \ref ELECTRODE_OTHER \ref GExtra::el_type_

*/
DLL_PUBLIC uint64_t gt_el_other_surface(volatile GDevice *d);

/** Set the value of this field that is not part of the device config \ref _GDevice

[1um^2]. The surface for \ref ELECTRODE_OTHER \ref GExtra::el_type_

*/
DLL_PUBLIC bool st_el_other_surface(
    PGDevice d, //!< device handle
    uint64_t v //!< new value to be set locally, i.e. this is not yet transmitted to the device
    );

/** Return the value of this field that is not part of the device config \ref _GDevice

[10pC]. The charge of one phase. This is a calculated value.

*/
DLL_PUBLIC int32_t gt_phase_charge(volatile GDevice *d);

/** Return the value of this field that is not part of the device config \ref _GDevice

[10pC]. The charge applied by a whole train. This is a calculated value.

*/
DLL_PUBLIC int64_t gt_train_charge(volatile GDevice *d);

/** Return the value of this field that is not part of the device config \ref _GDevice

[1]. The battery in percent. This is a calculated value.

*/
DLL_PUBLIC uint16_t gt_battery_pc(volatile GDevice *d);

/** Return the value of this field that is not part of the device config \ref _GDevice

[10us]. The duration of a pulse. This is a calculated value.

*/
DLL_PUBLIC uint32_t gt_pulse_duration(volatile GDevice *d);

/** Return the value of this field that is not part of the device config \ref _GDevice

[10us]. The time from one pulse to the next pulse in a train. This is a calculated value.

*/
DLL_PUBLIC uint32_t gt_pulse_period(volatile GDevice *d);

/** Set the value of this field that is not part of the device config \ref _GDevice

[10us]. The time from one pulse to the next pulse in a train. This is a calculated value.

*/
DLL_PUBLIC bool st_pulse_period(
    PGDevice d, //!< device handle
    uint32_t v //!< new value to be set locally, i.e. this is not yet transmitted to the device
    );

/** Return the value of this field that is not part of the device config \ref _GDevice

[10us]. Train duration. This is a calculated value.

*/
DLL_PUBLIC uint32_t gt_train_duration(volatile GDevice *d);

/** Return the value of this field that is not part of the device config \ref _GDevice

[1uA]. The average current of the first phase calculated from \ref GDevice::current1_sum_ and \ref GDevice::duration1_sum_.

*/
DLL_PUBLIC double gt_current1_avg(volatile GDevice *d);

/** Return the value of this field that is not part of the device config \ref _GDevice

[1uA]. The average current of the first phase calculated from \ref GDevice::current2_sum_ and \ref GDevice::duration2_sum_.

*/
DLL_PUBLIC double gt_current2_avg(volatile GDevice *d);

/** Return the value of this field that is not part of the device config \ref _GDevice

[1V]. The average voltage of the first phase calculated from \ref GDevice::voltage1_sum_ and \ref GDevice::duration1_sum_.

*/
DLL_PUBLIC double gt_voltage1_avg(volatile GDevice *d);

/** Return the value of this field that is not part of the device config \ref _GDevice

[1V]. The average voltage of the first phase calculated from \ref GDevice::voltage2_sum_ and \ref GDevice::duration2_sum_.

*/
DLL_PUBLIC double gt_voltage2_avg(volatile GDevice *d);

/** Return the value of this field that is not part of the device config \ref _GDevice

[1]. Number of pulses of train fade in.

*/
DLL_PUBLIC uint16_t gt_fade_in_pulses(volatile GDevice *d);

/** Return the value of this field that is not part of the device config \ref _GDevice

[1]. Number of pulses of train fade out.

*/
DLL_PUBLIC uint16_t gt_fade_out_pulses(volatile GDevice *d);

/** Return the value of this field that is not part of the device config \ref _GDevice

[1]. Ratio between current faded amplitude/phase and full amplitude/phase

*/
DLL_PUBLIC double gt_fade_factor(volatile GDevice *d);

/** Return the value of this field that is not part of the device config \ref _GDevice

[10us]. The average current of the first phase calculated from \ref GDevice::current1_sum_ and \ref GDevice::duration1_sum_.

*/
DLL_PUBLIC uint64_t gt_train_period(volatile GDevice *d);

/** Set the value of this field that is not part of the device config \ref _GDevice

[10us]. The average current of the first phase calculated from \ref GDevice::current1_sum_ and \ref GDevice::duration1_sum_.

*/
DLL_PUBLIC bool st_train_period(
    PGDevice d, //!< device handle
    uint64_t v //!< new value to be set locally, i.e. this is not yet transmitted to the device
    );

/** Return the value of this field that is not part of the device config \ref _GDevice

[10us]. The duration of the whole sequence. This is a calculated value.

*/
DLL_PUBLIC uint64_t gt_sequence_duration(volatile GDevice *d);

/** Return the value of this field that is not part of the device config \ref _GDevice

[1um^2]. The surface of the electrode. This is a calculated value.

*/
DLL_PUBLIC uint64_t gt_el_calc_surface(volatile GDevice *d);

/** Return the value of this field that is not part of the device config \ref _GDevice

[1uC/cm^2]. The charge density for a phase. This is a calculated value.

*/
DLL_PUBLIC double gt_charge_density(volatile GDevice *d);

/** Return the value of this field that is not part of the device config \ref _GDevice

[1]. The trains applied since start of sequence.

*/
DLL_PUBLIC uint32_t gt_applied_trains(volatile GDevice *d);

/** Return the value of this field that is not part of the device config \ref _GDevice

[10us]. This represents the time remaining in the sequence.

*/
DLL_PUBLIC uint64_t gt_remaining(volatile GDevice *d);



/** The periods for the train of a sequence due to \ref GExtra::jitter_ are pre-calculated. This function returns the actual periods used for the sequence.

*/
DLL_PUBLIC uint64_t *gt_periods(
        PGDevice d //!< device handle
        );

/** Sets the sequence duration by changing the \ref GExtra::train_rate_ (\ref st_sequence_duration_by_n_trains() )

*/
DLL_PUBLIC bool st_sequence_duration_by_train_rate ( 
    PGDevice d, //!< the device handle
    uint64_t v  //!< new value to be set locally, i.e. this is not yet transmitted to the device
    );

/** Sets the sequence duration by changing \ref GExtra::n_trains_ (\ref st_sequence_duration_by_train_rate() )

*/
DLL_PUBLIC bool st_sequence_duration_by_n_trains  ( 
    PGDevice d, //!< the device handle
    uint64_t v  //!< new value to be set locally, i.e. this is not yet transmitted to the device
    );

/** Sets the train duration by changing \ref GDevice::pulses_ (\ref st_train_duration_by_pulse_rate())

*/
DLL_PUBLIC bool st_train_duration_by_pulses       ( 
    PGDevice d, //!< the device handle
    uint32_t v  //!< the target value
    );

/** Sets the train duration by changing the pulse \ref GDevice::pulse_rate_ (\ref st_train_duration_by_pulse_rate())

*/
DLL_PUBLIC bool st_train_duration_by_pulse_rate    ( 
    PGDevice d, //!< the device handle
    uint32_t v  //!< the target value
    );


/** Start the self test and return.

Use \ref wait_for_end_of_action() to wait in a blocking way until the sequence is finished.

Use \ref gt_selftest_passed() to see, whether self test has passed.


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool selftest(
    PGDevice d   //!< the device handle
        );

/** Returns whether self test has passed.

*/
DLL_PUBLIC bool gt_selftest_passed(
    PGDevice d   //!< the device handle
    );


//control
/** Returns true if the thread communicating to the device does accept new tasks within a specific time (\ref DEVICE_READY_TIMEOUT).

request() works only, if deviceReady() returns true.

During do_sequence(), deviceReady() becomes `true` even if not done yet, to allow certain requests during the long sequence time.
These requests are handled in the function controlling the sequence. 

*/
DLL_PUBLIC bool deviceReady(
    PGDevice d, //!< The device handle
    bool wait   //!< true to wait DEVICE_READY_TIMEOUT or false to return immediately with the state of the communication thread
    );

/** Returns `true` if the sequence is running.

In between trains in a sequence the device is in \ref GSTATE_ACTIVE 
and one would not be able to tell otherwise, whether a sequence is running.

*/
DLL_PUBLIC bool sequence_running(
    PGDevice d   //!< the device handle
    );

/** Use this function to wait for sequence started with \ref start_sequence() to complete. It runs in a separate thread.

*/
DLL_PUBLIC void wait_for_end_of_sequence(
    PGDevice d   //!< the device handle
    );

/** wait_for_end_of_action() is a synonym for \ref wait_for_end_of_sequence(). 
 Use this function to wait for the completion of a communication action.

*/
#define wait_for_end_of_action wait_for_end_of_sequence 

/** Sets \ref GDevice::state_ to \ref GSTATE_ACTIVE and sends the whole configuration to the device.


After activate() one should call \ref wait_for_end_of_action().
activate() immediately returns, but the communication thread waits about a second for the device to charge.
During this time no communication to the device is possible.

@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool activate(
    PGDevice d   //!< the device handle
        );

/** First abort the task done by the communication thread, then set the device to \ref GSTATE_STOP.


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool stop(
    PGDevice d   //!< the device handle
        );

/** Abort the current sequence and set the device back to \ref GSTATE_ACTIVE.

abort_sequence() can also abort the self-test and in general
every longer task that is done by the communication thread.


Note for FES with fade_out_time_>0: the device will use the received GSTATE_ACTIVE
to start the fade-out but will not set its state to GSTATE_ACTIVE. 
It will become GSTATE_ACTIVE after the fade-out is over.

@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool abort_sequence(
    PGDevice d   //!< the device handle
        );

/** Start the sequence and return. 

Use \ref wait_for_end_of_sequence() to wait in a blocking way until the sequence is finished.


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool start_sequence(
    PGDevice d   //!< the device handle
        );

/** Resets the total charge counter on the device. 

\ref GDevice::total_charge_ is a field that is updated by the regular pinging of the communication thread.
When doing {st_total_charge(d,0);snd_total_charge()}, the 0 could be changed to the current value in between.
Therefore this function.


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool reset_total_charge(
        PGDevice d //!< device handle
        );

/** Returns true, if the serial number is that of a FES device, else false.

*/
DLL_PUBLIC bool is_FES_serial(
        PGDevice d //!< device handle
        );



/** Clears the last error that was different from GERROR_SUCCESS.
*/
DLL_PUBLIC uint16_t clr_last_error(
    PGDevice d   //!< the device handle
        );

/** Returns the last error that was different from GERROR_SUCCESS.
*/
DLL_PUBLIC uint16_t gt_last_error(
    PGDevice d   //!< the device handle
        );

/** Returns the error text for an error code. The error text is at most \ref MAX_ERROR_TEXT_LEN long.

*/
DLL_PUBLIC const char* gt_error_text(
    PGDevice d,   //!< the device handle
    GError error  //!< the \ref GError code
    );



/** Start the calibration of uC2 and return.

Use \ref wait_for_end_of_action() to wait in a blocking way until the task is done.

Calibrate uC2, i.e. the measurement. 

This calibration maps the device internal measurement to the setting.
uC1 is required to be calibrated first, 
i.e. to provide the currents according to the settings.

To calibrate uC1 the actual measurement of the current is needed. 
This is done using NI USB-6212 DAQ unit used for testing. 
The calibration of uC1 is part of the test script test_device.py: calibrate1().
uC1 calibration maps the setting to the actual current.

@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool calibrate2(
    PGDevice d   //!< the device handle
        );


/** Start sending \ref GDevice::phase_current1_, \ref GDevice::phase_current2_, \ref GDevice::phase_duration1_, \ref GDevice::phase_duration2_ and return.

This function can be used to change these values during the stimulation. 
The function is necessary, because the values must fit together and need to be set at the same time.
This is for FES devices.

Don't call this function in too a high rate, not more often than 10 times a second,
else the device gets problems.


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool snd_current_n_duration(
    PGDevice d   //!< the device handle
        );


/** Returns the resistance.

- If the device is not active it is set active.

- The device is put into \ref GSTATE_RESISTANCE.

- Waits 5 times \ref DEVICE_READY_TIMEOUT for the result.

This is the only function that communicates to the device,
which returns only when the result is available.

At the end of this function the state is GSTATE_ACTIVE.

*/
DLL_PUBLIC uint16_t measure_resistance(
    PGDevice d   //!< the device handle
        );

/** Return the magnitude of the device internal test resistance.

*/
DLL_PUBLIC uint16_t gt_test_resistance(
    PGDevice d   //!< the device handle
        );

/** Returns the resistance of the last resistance measurement

*/
DLL_PUBLIC uint16_t gt_last_resistance(
    PGDevice d   //!< the device handle
        );


/** The callback function registered with \ref register_with_polling() must have this type.

This function is called every \ref PING_TIME_MS to inform about changed values.

Fields can change because of the API or because of the device and it's environment.
The path to inform the user of the API is via such a callback function in both cases.

*/
typedef void (*FieldsChangedCallback)(
    PGDevice d,  //!< device handle
    uint64_t changed_flags //!< \ref GField identifies the bit that are set, if the according GDevice field has changed since the last callback
    );

/** Register a function of signature \ref FieldsChangedCallback.

Only \ref MAX_CALLBACK_FIELDSCHANGED such function can be registered.

*/
DLL_PUBLIC bool register_with_polling(
    PGDevice d,  //!< device handle
    FieldsChangedCallback fieldsChangedCallback //!< the callback function of signature \ref FieldsChangedCallback
    );

/** Unregister a registered function. (\ref register_with_polling())

*/
DLL_PUBLIC bool unregister_with_polling(
    PGDevice d,  //!< device handle
    FieldsChangedCallback fieldsChangedCallback //!< the callback function of signature \ref FieldsChangedCallback
    );


/** Return the C++ redistributable version
*/
DLL_PUBLIC int gt_CrtVersion();
		
/** Returns the used FTDI library version.
*/
DLL_PUBLIC uint32_t gt_ftdi_library_version();

/** Returns the used FTDI driver version.
*/
DLL_PUBLIC uint32_t gt_ftdi_driver_version(
    PGDevice d   //!< the device handle
    );


/** Return the default voltage_limit value.

[0.1V]. Voltage above which GEVENT_HIGHVOLTAGE will happen

*/
DLL_PUBLIC uint16_t gt_voltage_limit_default(
    PGDevice d  //!< device handle
    );

/** Return the maximum voltage_limit value.

[0.1V]. Voltage above which GEVENT_HIGHVOLTAGE will happen

*/
DLL_PUBLIC uint16_t gt_voltage_limit_max(
    PGDevice d  //!< device handle
    );

/** Return the minimum voltage_limit value.

[0.1V]. Voltage above which GEVENT_HIGHVOLTAGE will happen

*/
DLL_PUBLIC uint16_t gt_voltage_limit_min(
    PGDevice d  //!< device handle
    );


/** Return the default alternate value.

[1]. Alternating setting \see GAlternate

*/
DLL_PUBLIC GAlternate gt_alternate_default(
    PGDevice d  //!< device handle
    );

/** Return the maximum alternate value.

[1]. Alternating setting \see GAlternate

*/
DLL_PUBLIC GAlternate gt_alternate_max(
    PGDevice d  //!< device handle
    );

/** Return the minimum alternate value.

[1]. Alternating setting \see GAlternate

*/
DLL_PUBLIC GAlternate gt_alternate_min(
    PGDevice d  //!< device handle
    );


/** Return the default phase_current1 value.

[10uA]. Current phase 1 

*/
DLL_PUBLIC int16_t gt_phase_current1_default(
    PGDevice d  //!< device handle
    );

/** Return the maximum phase_current1 value.

[10uA]. Current phase 1 

*/
DLL_PUBLIC int16_t gt_phase_current1_max(
    PGDevice d  //!< device handle
    );

/** Return the minimum phase_current1 value.

[10uA]. Current phase 1 

*/
DLL_PUBLIC int16_t gt_phase_current1_min(
    PGDevice d  //!< device handle
    );


/** Return the default phase_current2 value.

[10uA]. Current phase 2 . This must be minus phase_current1_. 

*/
DLL_PUBLIC int16_t gt_phase_current2_default(
    PGDevice d  //!< device handle
    );

/** Return the maximum phase_current2 value.

[10uA]. Current phase 2 . This must be minus phase_current1_. 

*/
DLL_PUBLIC int16_t gt_phase_current2_max(
    PGDevice d  //!< device handle
    );

/** Return the minimum phase_current2 value.

[10uA]. Current phase 2 . This must be minus phase_current1_. 

*/
DLL_PUBLIC int16_t gt_phase_current2_min(
    PGDevice d  //!< device handle
    );


/** Return the default phase_duration1 value.

[10us]. duration phase 1 

*/
DLL_PUBLIC uint16_t gt_phase_duration1_default(
    PGDevice d  //!< device handle
    );

/** Return the maximum phase_duration1 value.

[10us]. duration phase 1 

*/
DLL_PUBLIC uint16_t gt_phase_duration1_max(
    PGDevice d  //!< device handle
    );

/** Return the minimum phase_duration1 value.

[10us]. duration phase 1 

*/
DLL_PUBLIC uint16_t gt_phase_duration1_min(
    PGDevice d  //!< device handle
    );


/** Return the default phase_duration2 value.

[10us]. duration phase 2 . This must be equal to phase_current1_ (or 0 = monophasic, for PRO only)

*/
DLL_PUBLIC uint16_t gt_phase_duration2_default(
    PGDevice d  //!< device handle
    );

/** Return the maximum phase_duration2 value.

[10us]. duration phase 2 . This must be equal to phase_current1_ (or 0 = monophasic, for PRO only)

*/
DLL_PUBLIC uint16_t gt_phase_duration2_max(
    PGDevice d  //!< device handle
    );

/** Return the minimum phase_duration2 value.

[10us]. duration phase 2 . This must be equal to phase_current1_ (or 0 = monophasic, for PRO only)

*/
DLL_PUBLIC uint16_t gt_phase_duration2_min(
    PGDevice d  //!< device handle
    );


/** Return the default fade_in_time value.

[1ms]. fade in time of train 

*/
DLL_PUBLIC uint16_t gt_fade_in_time_default(
    PGDevice d  //!< device handle
    );

/** Return the maximum fade_in_time value.

[1ms]. fade in time of train 

*/
DLL_PUBLIC uint16_t gt_fade_in_time_max(
    PGDevice d  //!< device handle
    );

/** Return the minimum fade_in_time value.

[1ms]. fade in time of train 

*/
DLL_PUBLIC uint16_t gt_fade_in_time_min(
    PGDevice d  //!< device handle
    );


/** Return the default fade_out_time value.

[1ms]. fade out time of train 

*/
DLL_PUBLIC uint16_t gt_fade_out_time_default(
    PGDevice d  //!< device handle
    );

/** Return the maximum fade_out_time value.

[1ms]. fade out time of train 

*/
DLL_PUBLIC uint16_t gt_fade_out_time_max(
    PGDevice d  //!< device handle
    );

/** Return the minimum fade_out_time value.

[1ms]. fade out time of train 

*/
DLL_PUBLIC uint16_t gt_fade_out_time_min(
    PGDevice d  //!< device handle
    );


/** Return the default fade_type value.

[1]. fade in/out using phase or amplitude or both \see GFade

*/
DLL_PUBLIC GFade gt_fade_type_default(
    PGDevice d  //!< device handle
    );

/** Return the maximum fade_type value.

[1]. fade in/out using phase or amplitude or both \see GFade

*/
DLL_PUBLIC GFade gt_fade_type_max(
    PGDevice d  //!< device handle
    );

/** Return the minimum fade_type value.

[1]. fade in/out using phase or amplitude or both \see GFade

*/
DLL_PUBLIC GFade gt_fade_type_min(
    PGDevice d  //!< device handle
    );


/** Return the default interphase value.

[10us]. duration between phases . This must be 0 if phase_duration2_ is 0.

*/
DLL_PUBLIC uint16_t gt_interphase_default(
    PGDevice d  //!< device handle
    );

/** Return the maximum interphase value.

[10us]. duration between phases . This must be 0 if phase_duration2_ is 0.

*/
DLL_PUBLIC uint16_t gt_interphase_max(
    PGDevice d  //!< device handle
    );

/** Return the minimum interphase value.

[10us]. duration between phases . This must be 0 if phase_duration2_ is 0.

*/
DLL_PUBLIC uint16_t gt_interphase_min(
    PGDevice d  //!< device handle
    );


/** Return the default pulse_rate value.

[0.1Hz]. stimulation pulse rate 

*/
DLL_PUBLIC uint16_t gt_pulse_rate_default(
    PGDevice d  //!< device handle
    );

/** Return the maximum pulse_rate value.

[0.1Hz]. stimulation pulse rate 

*/
DLL_PUBLIC uint16_t gt_pulse_rate_max(
    PGDevice d  //!< device handle
    );

/** Return the minimum pulse_rate value.

[0.1Hz]. stimulation pulse rate 

*/
DLL_PUBLIC uint16_t gt_pulse_rate_min(
    PGDevice d  //!< device handle
    );


/** Return the default pulses value.

[1]. number of stimulation pulses

*/
DLL_PUBLIC uint16_t gt_pulses_default(
    PGDevice d  //!< device handle
    );

/** Return the maximum pulses value.

[1]. number of stimulation pulses

*/
DLL_PUBLIC uint16_t gt_pulses_max(
    PGDevice d  //!< device handle
    );

/** Return the minimum pulses value.

[1]. number of stimulation pulses

*/
DLL_PUBLIC uint16_t gt_pulses_min(
    PGDevice d  //!< device handle
    );


/** Return the default train_rate value.

[1Hz]. The frequency of the train must be such as to fit in gt_train_duration().

*/
DLL_PUBLIC double gt_train_rate_default(
    PGDevice d  //!< device handle
    );

/** Return the maximum train_rate value.

[1Hz]. The frequency of the train must be such as to fit in gt_train_duration().

*/
DLL_PUBLIC double gt_train_rate_max(
    PGDevice d  //!< device handle
    );

/** Return the minimum train_rate value.

[1Hz]. The frequency of the train must be such as to fit in gt_train_duration().

*/
DLL_PUBLIC double gt_train_rate_min(
    PGDevice d  //!< device handle
    );


/** Return the default n_trains value.

[1]. More than one train (=sequence) are handled by the API via the parallel communication thread. wait_for_end_of_sequence() waits (and blocks) until all the trains are done, which can take hours.

*/
DLL_PUBLIC uint32_t gt_n_trains_default(
    PGDevice d  //!< device handle
    );

/** Return the maximum n_trains value.

[1]. More than one train (=sequence) are handled by the API via the parallel communication thread. wait_for_end_of_sequence() waits (and blocks) until all the trains are done, which can take hours.

*/
DLL_PUBLIC uint32_t gt_n_trains_max(
    PGDevice d  //!< device handle
    );

/** Return the minimum n_trains value.

[1]. More than one train (=sequence) are handled by the API via the parallel communication thread. wait_for_end_of_sequence() waits (and blocks) until all the trains are done, which can take hours.

*/
DLL_PUBLIC uint32_t gt_n_trains_min(
    PGDevice d  //!< device handle
    );


/** Return the default jitter value.

[1%]. A value other than 0 (0&lt;1&lt;=100) will randomly choose a distance between trains of a sequence from 0 to the value derived from \ref GExtra::train_rate_((period–train_duration)*(1+(rand(jitter_)-jitter_)/100))

*/
DLL_PUBLIC uint32_t gt_jitter_default(
    PGDevice d  //!< device handle
    );

/** Return the maximum jitter value.

[1%]. A value other than 0 (0&lt;1&lt;=100) will randomly choose a distance between trains of a sequence from 0 to the value derived from \ref GExtra::train_rate_((period–train_duration)*(1+(rand(jitter_)-jitter_)/100))

*/
DLL_PUBLIC uint32_t gt_jitter_max(
    PGDevice d  //!< device handle
    );

/** Return the minimum jitter value.

[1%]. A value other than 0 (0&lt;1&lt;=100) will randomly choose a distance between trains of a sequence from 0 to the value derived from \ref GExtra::train_rate_((period–train_duration)*(1+(rand(jitter_)-jitter_)/100))

*/
DLL_PUBLIC uint32_t gt_jitter_min(
    PGDevice d  //!< device handle
    );


/** Return the default do1_pre_time value.

[1ms]. pre-stimulation time DO1 

*/
DLL_PUBLIC uint16_t gt_do1_pre_time_default(
    PGDevice d  //!< device handle
    );

/** Return the maximum do1_pre_time value.

[1ms]. pre-stimulation time DO1 

*/
DLL_PUBLIC uint16_t gt_do1_pre_time_max(
    PGDevice d  //!< device handle
    );

/** Return the minimum do1_pre_time value.

[1ms]. pre-stimulation time DO1 

*/
DLL_PUBLIC uint16_t gt_do1_pre_time_min(
    PGDevice d  //!< device handle
    );


/** Return the default do1_post_time value.

[1ms]. post-stimulation time DO1 

*/
DLL_PUBLIC uint16_t gt_do1_post_time_default(
    PGDevice d  //!< device handle
    );

/** Return the maximum do1_post_time value.

[1ms]. post-stimulation time DO1 

*/
DLL_PUBLIC uint16_t gt_do1_post_time_max(
    PGDevice d  //!< device handle
    );

/** Return the minimum do1_post_time value.

[1ms]. post-stimulation time DO1 

*/
DLL_PUBLIC uint16_t gt_do1_post_time_min(
    PGDevice d  //!< device handle
    );


/** Return the default do2_pre_time value.

[1ms]. pre-stimulation time DO2 

*/
DLL_PUBLIC uint16_t gt_do2_pre_time_default(
    PGDevice d  //!< device handle
    );

/** Return the maximum do2_pre_time value.

[1ms]. pre-stimulation time DO2 

*/
DLL_PUBLIC uint16_t gt_do2_pre_time_max(
    PGDevice d  //!< device handle
    );

/** Return the minimum do2_pre_time value.

[1ms]. pre-stimulation time DO2 

*/
DLL_PUBLIC uint16_t gt_do2_pre_time_min(
    PGDevice d  //!< device handle
    );


/** Return the default do2_post_time value.

[1ms]. post-stimulation time DO2 

*/
DLL_PUBLIC uint16_t gt_do2_post_time_default(
    PGDevice d  //!< device handle
    );

/** Return the maximum do2_post_time value.

[1ms]. post-stimulation time DO2 

*/
DLL_PUBLIC uint16_t gt_do2_post_time_max(
    PGDevice d  //!< device handle
    );

/** Return the minimum do2_post_time value.

[1ms]. post-stimulation time DO2 

*/
DLL_PUBLIC uint16_t gt_do2_post_time_min(
    PGDevice d  //!< device handle
    );


/** Return the default do1_function value.

[1]. DO1 functionality setting \see GDO

*/
DLL_PUBLIC GDO gt_do1_function_default(
    PGDevice d  //!< device handle
    );

/** Return the maximum do1_function value.

[1]. DO1 functionality setting \see GDO

*/
DLL_PUBLIC GDO gt_do1_function_max(
    PGDevice d  //!< device handle
    );

/** Return the minimum do1_function value.

[1]. DO1 functionality setting \see GDO

*/
DLL_PUBLIC GDO gt_do1_function_min(
    PGDevice d  //!< device handle
    );


/** Return the default do2_function value.

[1]. DO2 functionality setting \see GDO

*/
DLL_PUBLIC GDO gt_do2_function_default(
    PGDevice d  //!< device handle
    );

/** Return the maximum do2_function value.

[1]. DO2 functionality setting \see GDO

*/
DLL_PUBLIC GDO gt_do2_function_max(
    PGDevice d  //!< device handle
    );

/** Return the minimum do2_function value.

[1]. DO2 functionality setting \see GDO

*/
DLL_PUBLIC GDO gt_do2_function_min(
    PGDevice d  //!< device handle
    );


/** Return the default dio_enabled value.

[1]. DIO enable settings e.g. (ioenabled&amp;(1&lt;&lt;GMASK_DI1))

*/
DLL_PUBLIC uint16_t gt_dio_enabled_default(
    PGDevice d  //!< device handle
    );

/** Return the maximum dio_enabled value.

[1]. DIO enable settings e.g. (ioenabled&amp;(1&lt;&lt;GMASK_DI1))

*/
DLL_PUBLIC uint16_t gt_dio_enabled_max(
    PGDevice d  //!< device handle
    );

/** Return the minimum dio_enabled value.

[1]. DIO enable settings e.g. (ioenabled&amp;(1&lt;&lt;GMASK_DI1))

*/
DLL_PUBLIC uint16_t gt_dio_enabled_min(
    PGDevice d  //!< device handle
    );


/** Return the default highvoltage_led value.

[1]. Actual state of high voltage LED (1 = high-voltage)

*/
DLL_PUBLIC uint16_t gt_highvoltage_led_default(
    PGDevice d  //!< device handle
    );

/** Return the maximum highvoltage_led value.

[1]. Actual state of high voltage LED (1 = high-voltage)

*/
DLL_PUBLIC uint16_t gt_highvoltage_led_max(
    PGDevice d  //!< device handle
    );

/** Return the minimum highvoltage_led value.

[1]. Actual state of high voltage LED (1 = high-voltage)

*/
DLL_PUBLIC uint16_t gt_highvoltage_led_min(
    PGDevice d  //!< device handle
    );


/** Return the default o_function value.

[1]. Output functionality setting \see GOFunction

*/
DLL_PUBLIC GOFunction gt_o_function_default(
    PGDevice d  //!< device handle
    );

/** Return the maximum o_function value.

[1]. Output functionality setting \see GOFunction

*/
DLL_PUBLIC GOFunction gt_o_function_max(
    PGDevice d  //!< device handle
    );

/** Return the minimum o_function value.

[1]. Output functionality setting \see GOFunction

*/
DLL_PUBLIC GOFunction gt_o_function_min(
    PGDevice d  //!< device handle
    );


/** Return the default operationmode value.

[1]. \ref GESTIM_MODE_COUNT values: \ref GESTIM_MODE_DEVICE (default) \ref GESTIM_MODE_BASIC \ref GESTIM_MODE_HANDHELD \ref GESTIM_MODE_ADVANCED \ref GESTIM_MODE_FES (the last is set automatically). These are stored in the bits given by \ref GESTIM_MODE_MASK. Other internal options are stored on the other bits.

*/
DLL_PUBLIC int gt_operationmode_default(
    PGDevice d  //!< device handle
    );

/** Return the maximum operationmode value.

[1]. \ref GESTIM_MODE_COUNT values: \ref GESTIM_MODE_DEVICE (default) \ref GESTIM_MODE_BASIC \ref GESTIM_MODE_HANDHELD \ref GESTIM_MODE_ADVANCED \ref GESTIM_MODE_FES (the last is set automatically). These are stored in the bits given by \ref GESTIM_MODE_MASK. Other internal options are stored on the other bits.

*/
DLL_PUBLIC int gt_operationmode_max(
    PGDevice d  //!< device handle
    );

/** Return the minimum operationmode value.

[1]. \ref GESTIM_MODE_COUNT values: \ref GESTIM_MODE_DEVICE (default) \ref GESTIM_MODE_BASIC \ref GESTIM_MODE_HANDHELD \ref GESTIM_MODE_ADVANCED \ref GESTIM_MODE_FES (the last is set automatically). These are stored in the bits given by \ref GESTIM_MODE_MASK. Other internal options are stored on the other bits.

*/
DLL_PUBLIC int gt_operationmode_min(
    PGDevice d  //!< device handle
    );


/** Return the default el_type value.

[1]. An electrode type: \ref ELECTRODE_CIRCULAR \ref ELECTRODE_OTHER \ref ELECTRODE_DEPTH \ref ELECTRODE_NONE

*/
DLL_PUBLIC int gt_el_type_default(
    PGDevice d  //!< device handle
    );

/** Return the maximum el_type value.

[1]. An electrode type: \ref ELECTRODE_CIRCULAR \ref ELECTRODE_OTHER \ref ELECTRODE_DEPTH \ref ELECTRODE_NONE

*/
DLL_PUBLIC int gt_el_type_max(
    PGDevice d  //!< device handle
    );

/** Return the minimum el_type value.

[1]. An electrode type: \ref ELECTRODE_CIRCULAR \ref ELECTRODE_OTHER \ref ELECTRODE_DEPTH \ref ELECTRODE_NONE

*/
DLL_PUBLIC int gt_el_type_min(
    PGDevice d  //!< device handle
    );


/** Return the default el_circ_diameter value.

[1um]. The diameter for \ref ELECTRODE_CIRCULAR \ref GExtra::el_type_

*/
DLL_PUBLIC uint32_t gt_el_circ_diameter_default(
    PGDevice d  //!< device handle
    );

/** Return the maximum el_circ_diameter value.

[1um]. The diameter for \ref ELECTRODE_CIRCULAR \ref GExtra::el_type_

*/
DLL_PUBLIC uint32_t gt_el_circ_diameter_max(
    PGDevice d  //!< device handle
    );

/** Return the minimum el_circ_diameter value.

[1um]. The diameter for \ref ELECTRODE_CIRCULAR \ref GExtra::el_type_

*/
DLL_PUBLIC uint32_t gt_el_circ_diameter_min(
    PGDevice d  //!< device handle
    );


/** Return the default el_depth_diameter value.

[1um]. The diameter for \ref ELECTRODE_DEPTH \ref GExtra::el_type_

*/
DLL_PUBLIC uint32_t gt_el_depth_diameter_default(
    PGDevice d  //!< device handle
    );

/** Return the maximum el_depth_diameter value.

[1um]. The diameter for \ref ELECTRODE_DEPTH \ref GExtra::el_type_

*/
DLL_PUBLIC uint32_t gt_el_depth_diameter_max(
    PGDevice d  //!< device handle
    );

/** Return the minimum el_depth_diameter value.

[1um]. The diameter for \ref ELECTRODE_DEPTH \ref GExtra::el_type_

*/
DLL_PUBLIC uint32_t gt_el_depth_diameter_min(
    PGDevice d  //!< device handle
    );


/** Return the default el_depth_length value.

[1um]. The depth for \ref ELECTRODE_DEPTH \ref GExtra::el_type_

*/
DLL_PUBLIC uint32_t gt_el_depth_length_default(
    PGDevice d  //!< device handle
    );

/** Return the maximum el_depth_length value.

[1um]. The depth for \ref ELECTRODE_DEPTH \ref GExtra::el_type_

*/
DLL_PUBLIC uint32_t gt_el_depth_length_max(
    PGDevice d  //!< device handle
    );

/** Return the minimum el_depth_length value.

[1um]. The depth for \ref ELECTRODE_DEPTH \ref GExtra::el_type_

*/
DLL_PUBLIC uint32_t gt_el_depth_length_min(
    PGDevice d  //!< device handle
    );


/** Return the default el_other_surface value.

[1um^2]. The surface for \ref ELECTRODE_OTHER \ref GExtra::el_type_

*/
DLL_PUBLIC uint64_t gt_el_other_surface_default(
    PGDevice d  //!< device handle
    );

/** Return the maximum el_other_surface value.

[1um^2]. The surface for \ref ELECTRODE_OTHER \ref GExtra::el_type_

*/
DLL_PUBLIC uint64_t gt_el_other_surface_max(
    PGDevice d  //!< device handle
    );

/** Return the minimum el_other_surface value.

[1um^2]. The surface for \ref ELECTRODE_OTHER \ref GExtra::el_type_

*/
DLL_PUBLIC uint64_t gt_el_other_surface_min(
    PGDevice d  //!< device handle
    );


/** Return the default total_charge value.

[10pC]. Total applied stimulation charge . Can only be reset.

*/
DLL_PUBLIC int32_t gt_total_charge_default(
    PGDevice d  //!< device handle
    );

/** Return the maximum total_charge value.

[10pC]. Total applied stimulation charge . Can only be reset.

*/
DLL_PUBLIC int32_t gt_total_charge_max(
    PGDevice d  //!< device handle
    );

/** Return the minimum total_charge value.

[10pC]. Total applied stimulation charge . Can only be reset.

*/
DLL_PUBLIC int32_t gt_total_charge_min(
    PGDevice d  //!< device handle
    );



//fields

/** Retrieves from the g.Estim device 

Device serial number e.g. &quot;ES-2014.12.14&quot;


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool rcv_serial(
        PGDevice d//!< device handle
        );

/** Returns the value in the local copy of the config.

Device serial number e.g. &quot;ES-2014.12.14&quot;

*/
DLL_PUBLIC const char* gt_serial(
        PGDevice d//!< device handle
        );

/** Sets the value in the local copy of the config.

Device serial number e.g. &quot;ES-2014.12.14&quot;

\return true, unless it is not allowed to set the value

*/
DLL_PUBLIC bool st_serial(
        PGDevice d,//!< device handle
        const char* serial//!< value to set. 
        );

/** Sends to device

Device serial number e.g. &quot;ES-2014.12.14&quot;


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool snd_serial(
        PGDevice d//!< device handle
        );





/** Retrieves from the g.Estim device 

Device identifier


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool rcv_identifier(
        PGDevice d//!< device handle
        );

/** Returns the value in the local copy of the config.

Device identifier

*/
DLL_PUBLIC const char* gt_identifier(
        PGDevice d//!< device handle
        );

/** Sets the value in the local copy of the config.

Device identifier

\return true, unless it is not allowed to set the value

*/
DLL_PUBLIC bool st_identifier(
        PGDevice d,//!< device handle
        const char* identifier//!< value to set. 
        );

/** Sends to device

Device identifier


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool snd_identifier(
        PGDevice d//!< device handle
        );





/** Retrieves from the g.Estim device 

Device firmware version


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool rcv_FW_version(
        PGDevice d//!< device handle
        );

/** Returns the value in the local copy of the config.

Device firmware version

*/
DLL_PUBLIC const char* gt_FW_version(
        PGDevice d//!< device handle
        );

/** Sets the value in the local copy of the config.

Device firmware version

\return true, unless it is not allowed to set the value

*/
DLL_PUBLIC bool st_FW_version(
        PGDevice d,//!< device handle
        const char* FW_version//!< value to set. 
        );

/** Sends to device

Device firmware version


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool snd_FW_version(
        PGDevice d//!< device handle
        );





/** Retrieves from the g.Estim device 

Device hardware version


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool rcv_HW_version(
        PGDevice d//!< device handle
        );

/** Returns the value in the local copy of the config.

Device hardware version

*/
DLL_PUBLIC const char* gt_HW_version(
        PGDevice d//!< device handle
        );

/** Sets the value in the local copy of the config.

Device hardware version

\return true, unless it is not allowed to set the value

*/
DLL_PUBLIC bool st_HW_version(
        PGDevice d,//!< device handle
        const char* HW_version//!< value to set. 
        );

/** Sends to device

Device hardware version


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool snd_HW_version(
        PGDevice d//!< device handle
        );





/** Retrieves from the g.Estim device 

Device current source 1 calibration coefficients [1/1000,1uA]


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool rcv_coeff_cs1(
        PGDevice d//!< device handle
        );

/** Returns the value in the local copy of the config.

Device current source 1 calibration coefficients [1/1000,1uA]

*/
DLL_PUBLIC const int16_t* gt_coeff_cs1(
        PGDevice d//!< device handle
        );

/** Sets the value in the local copy of the config.

Device current source 1 calibration coefficients [1/1000,1uA]

\return true, unless it is not allowed to set the value

*/
DLL_PUBLIC bool st_coeff_cs1(
        PGDevice d,//!< device handle
        const int16_t* coeff_cs1//!< value to set. 
        );

/** Sends to device

Device current source 1 calibration coefficients [1/1000,1uA]


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool snd_coeff_cs1(
        PGDevice d//!< device handle
        );





/** Retrieves from the g.Estim device 

Device current source 2 calibration coefficients [1/1000,1uA]


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool rcv_coeff_cs2(
        PGDevice d//!< device handle
        );

/** Returns the value in the local copy of the config.

Device current source 2 calibration coefficients [1/1000,1uA]

*/
DLL_PUBLIC const int16_t* gt_coeff_cs2(
        PGDevice d//!< device handle
        );

/** Sets the value in the local copy of the config.

Device current source 2 calibration coefficients [1/1000,1uA]

\return true, unless it is not allowed to set the value

*/
DLL_PUBLIC bool st_coeff_cs2(
        PGDevice d,//!< device handle
        const int16_t* coeff_cs2//!< value to set. 
        );

/** Sends to device

Device current source 2 calibration coefficients [1/1000,1uA]


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool snd_coeff_cs2(
        PGDevice d//!< device handle
        );





/** Retrieves from the g.Estim device 

Device current measurement calibration coefficients [1/1000,10uA]


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool rcv_coeff_i(
        PGDevice d//!< device handle
        );

/** Returns the value in the local copy of the config.

Device current measurement calibration coefficients [1/1000,10uA]

*/
DLL_PUBLIC const int16_t* gt_coeff_i(
        PGDevice d//!< device handle
        );

/** Sets the value in the local copy of the config.

Device current measurement calibration coefficients [1/1000,10uA]

\return true, unless it is not allowed to set the value

*/
DLL_PUBLIC bool st_coeff_i(
        PGDevice d,//!< device handle
        const int16_t* coeff_i//!< value to set. 
        );

/** Sends to device

Device current measurement calibration coefficients [1/1000,10uA]


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool snd_coeff_i(
        PGDevice d//!< device handle
        );





/** Retrieves from the g.Estim device 

Device voltage measurement calibration coefficients [1/1000,0.1V]


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool rcv_coeff_v(
        PGDevice d//!< device handle
        );

/** Returns the value in the local copy of the config.

Device voltage measurement calibration coefficients [1/1000,0.1V]

*/
DLL_PUBLIC const int16_t* gt_coeff_v(
        PGDevice d//!< device handle
        );

/** Sets the value in the local copy of the config.

Device voltage measurement calibration coefficients [1/1000,0.1V]

\return true, unless it is not allowed to set the value

*/
DLL_PUBLIC bool st_coeff_v(
        PGDevice d,//!< device handle
        const int16_t* coeff_v//!< value to set. 
        );

/** Sends to device

Device voltage measurement calibration coefficients [1/1000,0.1V]


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool snd_coeff_v(
        PGDevice d//!< device handle
        );





/** Retrieves from the g.Estim device 

Device resistance measurement calibration coefficients [1/1000,20 Ohm]


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool rcv_coeff_r(
        PGDevice d//!< device handle
        );

/** Returns the value in the local copy of the config.

Device resistance measurement calibration coefficients [1/1000,20 Ohm]

*/
DLL_PUBLIC const int16_t* gt_coeff_r(
        PGDevice d//!< device handle
        );

/** Sets the value in the local copy of the config.

Device resistance measurement calibration coefficients [1/1000,20 Ohm]

\return true, unless it is not allowed to set the value

*/
DLL_PUBLIC bool st_coeff_r(
        PGDevice d,//!< device handle
        const int16_t* coeff_r//!< value to set. 
        );

/** Sends to device

Device resistance measurement calibration coefficients [1/1000,20 Ohm]


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool snd_coeff_r(
        PGDevice d//!< device handle
        );





/** Retrieves from the g.Estim device 

Voltage above which GEVENT_HIGHVOLTAGE will happen


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool rcv_voltage_limit(
        PGDevice d//!< device handle
        );

/** Returns the value in the local copy of the config.

Voltage above which GEVENT_HIGHVOLTAGE will happen

*/
DLL_PUBLIC uint16_t gt_voltage_limit(
        PGDevice d//!< device handle
        );

/** Sets the value in the local copy of the config.

Voltage above which GEVENT_HIGHVOLTAGE will happen

\return true, unless it is not allowed to set the value

*/
DLL_PUBLIC bool st_voltage_limit(
        PGDevice d,//!< device handle
        uint16_t voltage_limit//!< value to set. 
        );

/** Sends to device

Voltage above which GEVENT_HIGHVOLTAGE will happen


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool snd_voltage_limit(
        PGDevice d//!< device handle
        );





/** Retrieves from the g.Estim device 

Alternating setting \see GAlternate


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool rcv_alternate(
        PGDevice d//!< device handle
        );

/** Returns the value in the local copy of the config.

Alternating setting \see GAlternate

*/
DLL_PUBLIC GAlternate gt_alternate(
        PGDevice d//!< device handle
        );

/** Sets the value in the local copy of the config.

Alternating setting \see GAlternate

\return true, unless it is not allowed to set the value

*/
DLL_PUBLIC bool st_alternate(
        PGDevice d,//!< device handle
        GAlternate alternate//!< value to set. 
        );

/** Sends to device

Alternating setting \see GAlternate


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool snd_alternate(
        PGDevice d//!< device handle
        );





/** Retrieves from the g.Estim device 

Current phase 1 [10 uA]


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool rcv_phase_current1(
        PGDevice d//!< device handle
        );

/** Returns the value in the local copy of the config.

Current phase 1 [10 uA]

*/
DLL_PUBLIC int16_t gt_phase_current1(
        PGDevice d//!< device handle
        );

/** Sets the value in the local copy of the config.

Current phase 1 [10 uA]

\return true, unless it is not allowed to set the value

*/
DLL_PUBLIC bool st_phase_current1(
        PGDevice d,//!< device handle
        int16_t phase_current1//!< value to set. 
        );

/** Sends to device

Current phase 1 [10 uA]


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool snd_phase_current1(
        PGDevice d//!< device handle
        );





/** Retrieves from the g.Estim device 

Current phase 2 [10 uA]. This must be minus phase_current1_. 


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool rcv_phase_current2(
        PGDevice d//!< device handle
        );

/** Returns the value in the local copy of the config.

Current phase 2 [10 uA]. This must be minus phase_current1_. 

*/
DLL_PUBLIC int16_t gt_phase_current2(
        PGDevice d//!< device handle
        );

/** Sets the value in the local copy of the config.

Current phase 2 [10 uA]. This must be minus phase_current1_. 

\return true, unless it is not allowed to set the value

*/
DLL_PUBLIC bool st_phase_current2(
        PGDevice d,//!< device handle
        int16_t phase_current2//!< value to set. 
        );

/** Sends to device

Current phase 2 [10 uA]. This must be minus phase_current1_. 


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool snd_phase_current2(
        PGDevice d//!< device handle
        );





/** Retrieves from the g.Estim device 

duration phase 1 [10 us]


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool rcv_phase_duration1(
        PGDevice d//!< device handle
        );

/** Returns the value in the local copy of the config.

duration phase 1 [10 us]

*/
DLL_PUBLIC uint16_t gt_phase_duration1(
        PGDevice d//!< device handle
        );

/** Sets the value in the local copy of the config.

duration phase 1 [10 us]

\return true, unless it is not allowed to set the value

*/
DLL_PUBLIC bool st_phase_duration1(
        PGDevice d,//!< device handle
        uint16_t phase_duration1//!< value to set. 
        );

/** Sends to device

duration phase 1 [10 us]


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool snd_phase_duration1(
        PGDevice d//!< device handle
        );





/** Retrieves from the g.Estim device 

duration phase 2 [10 us]. This must be equal to phase_current1_ (or 0 = monophasic, for PRO only)


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool rcv_phase_duration2(
        PGDevice d//!< device handle
        );

/** Returns the value in the local copy of the config.

duration phase 2 [10 us]. This must be equal to phase_current1_ (or 0 = monophasic, for PRO only)

*/
DLL_PUBLIC uint16_t gt_phase_duration2(
        PGDevice d//!< device handle
        );

/** Sets the value in the local copy of the config.

duration phase 2 [10 us]. This must be equal to phase_current1_ (or 0 = monophasic, for PRO only)

\return true, unless it is not allowed to set the value

*/
DLL_PUBLIC bool st_phase_duration2(
        PGDevice d,//!< device handle
        uint16_t phase_duration2//!< value to set. 
        );

/** Sends to device

duration phase 2 [10 us]. This must be equal to phase_current1_ (or 0 = monophasic, for PRO only)


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool snd_phase_duration2(
        PGDevice d//!< device handle
        );





/** Retrieves from the g.Estim device 

fade in time of train [ms]


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool rcv_fade_in_time(
        PGDevice d//!< device handle
        );

/** Returns the value in the local copy of the config.

fade in time of train [ms]

*/
DLL_PUBLIC uint16_t gt_fade_in_time(
        PGDevice d//!< device handle
        );

/** Sets the value in the local copy of the config.

fade in time of train [ms]

\return true, unless it is not allowed to set the value

*/
DLL_PUBLIC bool st_fade_in_time(
        PGDevice d,//!< device handle
        uint16_t fade_in_time//!< value to set. 
        );

/** Sends to device

fade in time of train [ms]


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool snd_fade_in_time(
        PGDevice d//!< device handle
        );





/** Retrieves from the g.Estim device 

fade out time of train [ms]


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool rcv_fade_out_time(
        PGDevice d//!< device handle
        );

/** Returns the value in the local copy of the config.

fade out time of train [ms]

*/
DLL_PUBLIC uint16_t gt_fade_out_time(
        PGDevice d//!< device handle
        );

/** Sets the value in the local copy of the config.

fade out time of train [ms]

\return true, unless it is not allowed to set the value

*/
DLL_PUBLIC bool st_fade_out_time(
        PGDevice d,//!< device handle
        uint16_t fade_out_time//!< value to set. 
        );

/** Sends to device

fade out time of train [ms]


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool snd_fade_out_time(
        PGDevice d//!< device handle
        );





/** Retrieves from the g.Estim device 

fade in/out using phase or amplitude or both \see GFade


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool rcv_fade_type(
        PGDevice d//!< device handle
        );

/** Returns the value in the local copy of the config.

fade in/out using phase or amplitude or both \see GFade

*/
DLL_PUBLIC GFade gt_fade_type(
        PGDevice d//!< device handle
        );

/** Sets the value in the local copy of the config.

fade in/out using phase or amplitude or both \see GFade

\return true, unless it is not allowed to set the value

*/
DLL_PUBLIC bool st_fade_type(
        PGDevice d,//!< device handle
        GFade fade_type//!< value to set. 
        );

/** Sends to device

fade in/out using phase or amplitude or both \see GFade


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool snd_fade_type(
        PGDevice d//!< device handle
        );





/** Retrieves from the g.Estim device 

duration between phases [10 us]. This must be 0 if phase_duration2_ is 0.


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool rcv_interphase(
        PGDevice d//!< device handle
        );

/** Returns the value in the local copy of the config.

duration between phases [10 us]. This must be 0 if phase_duration2_ is 0.

*/
DLL_PUBLIC uint16_t gt_interphase(
        PGDevice d//!< device handle
        );

/** Sets the value in the local copy of the config.

duration between phases [10 us]. This must be 0 if phase_duration2_ is 0.

\return true, unless it is not allowed to set the value

*/
DLL_PUBLIC bool st_interphase(
        PGDevice d,//!< device handle
        uint16_t interphase//!< value to set. 
        );

/** Sends to device

duration between phases [10 us]. This must be 0 if phase_duration2_ is 0.


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool snd_interphase(
        PGDevice d//!< device handle
        );





/** Retrieves from the g.Estim device 

stimulation pulse rate [0.1 Hz]


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool rcv_pulse_rate(
        PGDevice d//!< device handle
        );

/** Returns the value in the local copy of the config.

stimulation pulse rate [0.1 Hz]

*/
DLL_PUBLIC uint16_t gt_pulse_rate(
        PGDevice d//!< device handle
        );

/** Sets the value in the local copy of the config.

stimulation pulse rate [0.1 Hz]

\return true, unless it is not allowed to set the value

*/
DLL_PUBLIC bool st_pulse_rate(
        PGDevice d,//!< device handle
        uint16_t pulse_rate//!< value to set. 
        );

/** Sends to device

stimulation pulse rate [0.1 Hz]


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool snd_pulse_rate(
        PGDevice d//!< device handle
        );





/** Retrieves from the g.Estim device 

number of stimulation pulses


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool rcv_pulses(
        PGDevice d//!< device handle
        );

/** Returns the value in the local copy of the config.

number of stimulation pulses

*/
DLL_PUBLIC uint16_t gt_pulses(
        PGDevice d//!< device handle
        );

/** Sets the value in the local copy of the config.

number of stimulation pulses

\return true, unless it is not allowed to set the value

*/
DLL_PUBLIC bool st_pulses(
        PGDevice d,//!< device handle
        uint16_t pulses//!< value to set. 
        );

/** Sends to device

number of stimulation pulses


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool snd_pulses(
        PGDevice d//!< device handle
        );





/** Retrieves from the g.Estim device 

pre-stimulation time DO1 [ms]


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool rcv_do1_pre_time(
        PGDevice d//!< device handle
        );

/** Returns the value in the local copy of the config.

pre-stimulation time DO1 [ms]

*/
DLL_PUBLIC uint16_t gt_do1_pre_time(
        PGDevice d//!< device handle
        );

/** Sets the value in the local copy of the config.

pre-stimulation time DO1 [ms]

\return true, unless it is not allowed to set the value

*/
DLL_PUBLIC bool st_do1_pre_time(
        PGDevice d,//!< device handle
        uint16_t do1_pre_time//!< value to set. 
        );

/** Sends to device

pre-stimulation time DO1 [ms]


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool snd_do1_pre_time(
        PGDevice d//!< device handle
        );





/** Retrieves from the g.Estim device 

post-stimulation time DO1 [ms]


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool rcv_do1_post_time(
        PGDevice d//!< device handle
        );

/** Returns the value in the local copy of the config.

post-stimulation time DO1 [ms]

*/
DLL_PUBLIC uint16_t gt_do1_post_time(
        PGDevice d//!< device handle
        );

/** Sets the value in the local copy of the config.

post-stimulation time DO1 [ms]

\return true, unless it is not allowed to set the value

*/
DLL_PUBLIC bool st_do1_post_time(
        PGDevice d,//!< device handle
        uint16_t do1_post_time//!< value to set. 
        );

/** Sends to device

post-stimulation time DO1 [ms]


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool snd_do1_post_time(
        PGDevice d//!< device handle
        );





/** Retrieves from the g.Estim device 

pre-stimulation time DO2 [ms]


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool rcv_do2_pre_time(
        PGDevice d//!< device handle
        );

/** Returns the value in the local copy of the config.

pre-stimulation time DO2 [ms]

*/
DLL_PUBLIC uint16_t gt_do2_pre_time(
        PGDevice d//!< device handle
        );

/** Sets the value in the local copy of the config.

pre-stimulation time DO2 [ms]

\return true, unless it is not allowed to set the value

*/
DLL_PUBLIC bool st_do2_pre_time(
        PGDevice d,//!< device handle
        uint16_t do2_pre_time//!< value to set. 
        );

/** Sends to device

pre-stimulation time DO2 [ms]


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool snd_do2_pre_time(
        PGDevice d//!< device handle
        );





/** Retrieves from the g.Estim device 

post-stimulation time DO2 [ms]


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool rcv_do2_post_time(
        PGDevice d//!< device handle
        );

/** Returns the value in the local copy of the config.

post-stimulation time DO2 [ms]

*/
DLL_PUBLIC uint16_t gt_do2_post_time(
        PGDevice d//!< device handle
        );

/** Sets the value in the local copy of the config.

post-stimulation time DO2 [ms]

\return true, unless it is not allowed to set the value

*/
DLL_PUBLIC bool st_do2_post_time(
        PGDevice d,//!< device handle
        uint16_t do2_post_time//!< value to set. 
        );

/** Sends to device

post-stimulation time DO2 [ms]


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool snd_do2_post_time(
        PGDevice d//!< device handle
        );





/** Retrieves from the g.Estim device 

DO1 functionality setting \see GDO


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool rcv_do1_function(
        PGDevice d//!< device handle
        );

/** Returns the value in the local copy of the config.

DO1 functionality setting \see GDO

*/
DLL_PUBLIC GDO gt_do1_function(
        PGDevice d//!< device handle
        );

/** Sets the value in the local copy of the config.

DO1 functionality setting \see GDO

\return true, unless it is not allowed to set the value

*/
DLL_PUBLIC bool st_do1_function(
        PGDevice d,//!< device handle
        GDO do1_function//!< value to set. 
        );

/** Sends to device

DO1 functionality setting \see GDO


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool snd_do1_function(
        PGDevice d//!< device handle
        );





/** Retrieves from the g.Estim device 

DO2 functionality setting \see GDO


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool rcv_do2_function(
        PGDevice d//!< device handle
        );

/** Returns the value in the local copy of the config.

DO2 functionality setting \see GDO

*/
DLL_PUBLIC GDO gt_do2_function(
        PGDevice d//!< device handle
        );

/** Sets the value in the local copy of the config.

DO2 functionality setting \see GDO

\return true, unless it is not allowed to set the value

*/
DLL_PUBLIC bool st_do2_function(
        PGDevice d,//!< device handle
        GDO do2_function//!< value to set. 
        );

/** Sends to device

DO2 functionality setting \see GDO


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool snd_do2_function(
        PGDevice d//!< device handle
        );





/** Retrieves from the g.Estim device 

DIO enable settings e.g. (ioenabled&amp;(1&lt;&lt;GMASK_DI1))


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool rcv_dio_enabled(
        PGDevice d//!< device handle
        );

/** Returns the value in the local copy of the config.

DIO enable settings e.g. (ioenabled&amp;(1&lt;&lt;GMASK_DI1))

*/
DLL_PUBLIC uint16_t gt_dio_enabled(
        PGDevice d//!< device handle
        );

/** Sets the value in the local copy of the config.

DIO enable settings e.g. (ioenabled&amp;(1&lt;&lt;GMASK_DI1))

\return true, unless it is not allowed to set the value

*/
DLL_PUBLIC bool st_dio_enabled(
        PGDevice d,//!< device handle
        uint16_t dio_enabled//!< value to set. 
        );

/** Sends to device

DIO enable settings e.g. (ioenabled&amp;(1&lt;&lt;GMASK_DI1))


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool snd_dio_enabled(
        PGDevice d//!< device handle
        );





/** Retrieves from the g.Estim device 

Actual state of high voltage LED (1 = high-voltage)


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool rcv_highvoltage_led(
        PGDevice d//!< device handle
        );

/** Returns the value in the local copy of the config.

Actual state of high voltage LED (1 = high-voltage)

*/
DLL_PUBLIC uint16_t gt_highvoltage_led(
        PGDevice d//!< device handle
        );

/** Sets the value in the local copy of the config.

Actual state of high voltage LED (1 = high-voltage)

\return true, unless it is not allowed to set the value

*/
DLL_PUBLIC bool st_highvoltage_led(
        PGDevice d,//!< device handle
        uint16_t highvoltage_led//!< value to set. 
        );

/** Sends to device

Actual state of high voltage LED (1 = high-voltage)


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool snd_highvoltage_led(
        PGDevice d//!< device handle
        );





/** Retrieves from the g.Estim device 

Output functionality setting \see GOFunction


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool rcv_o_function(
        PGDevice d//!< device handle
        );

/** Returns the value in the local copy of the config.

Output functionality setting \see GOFunction

*/
DLL_PUBLIC GOFunction gt_o_function(
        PGDevice d//!< device handle
        );

/** Sets the value in the local copy of the config.

Output functionality setting \see GOFunction

\return true, unless it is not allowed to set the value

*/
DLL_PUBLIC bool st_o_function(
        PGDevice d,//!< device handle
        GOFunction o_function//!< value to set. 
        );

/** Sends to device

Output functionality setting \see GOFunction


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool snd_o_function(
        PGDevice d//!< device handle
        );





/** Retrieves from the g.Estim device 

Device state \see GState


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool rcv_state(
        PGDevice d//!< device handle
        );

/** Returns the value in the local copy of the config.

Device state \see GState

*/
DLL_PUBLIC GState gt_state(
        PGDevice d//!< device handle
        );

/** Sets the value in the local copy of the config.

Device state \see GState

\return true, unless it is not allowed to set the value

*/
DLL_PUBLIC bool st_state(
        PGDevice d,//!< device handle
        GState state//!< value to set. 
        );

/** Sends to device

Device state \see GState


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool snd_state(
        PGDevice d//!< device handle
        );





/** Retrieves from the g.Estim device 

Total applied stimulation charge [10pC]. Can only be reset.


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool rcv_total_charge(
        PGDevice d//!< device handle
        );

/** Returns the value in the local copy of the config.

Total applied stimulation charge [10pC]. Can only be reset.

*/
DLL_PUBLIC int32_t gt_total_charge(
        PGDevice d//!< device handle
        );

/** Sets the value in the local copy of the config.

Total applied stimulation charge [10pC]. Can only be reset.

\return true, unless it is not allowed to set the value

*/
DLL_PUBLIC bool st_total_charge(
        PGDevice d,//!< device handle
        int32_t total_charge//!< value to set. 
        );

/** Sends to device

Total applied stimulation charge [10pC]. Can only be reset.


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool snd_total_charge(
        PGDevice d//!< device handle
        );





/** Retrieves from the g.Estim device 

Shows which event lead to current device state \see GEvent


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool rcv_event(
        PGDevice d//!< device handle
        );

/** Returns the value in the local copy of the config.

Shows which event lead to current device state \see GEvent

*/
DLL_PUBLIC GEvent gt_event(
        PGDevice d//!< device handle
        );

/** Retrieves from the g.Estim device 

Device error state \see GError


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool rcv_error(
        PGDevice d//!< device handle
        );

/** Returns the value in the local copy of the config.

Device error state \see GError

*/
DLL_PUBLIC GError gt_error(
        PGDevice d//!< device handle
        );

/** Retrieves from the g.Estim device 

Shows which event lead to last stimulation state \see GEvent


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool rcv_stim_event(
        PGDevice d//!< device handle
        );

/** Returns the value in the local copy of the config.

Shows which event lead to last stimulation state \see GEvent

*/
DLL_PUBLIC GEvent gt_stim_event(
        PGDevice d//!< device handle
        );

/** Retrieves from the g.Estim device 

Device battery 1 charge with values {0, 100-200}


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool rcv_battery1(
        PGDevice d//!< device handle
        );

/** Returns the value in the local copy of the config.

Device battery 1 charge with values {0, 100-200}

*/
DLL_PUBLIC uint16_t gt_battery1(
        PGDevice d//!< device handle
        );

/** Retrieves from the g.Estim device 

Device battery 2 charge with values {0, 100-200}


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool rcv_battery2(
        PGDevice d//!< device handle
        );

/** Returns the value in the local copy of the config.

Device battery 2 charge with values {0, 100-200}

*/
DLL_PUBLIC uint16_t gt_battery2(
        PGDevice d//!< device handle
        );

/** Retrieves from the g.Estim device 

Device inputs state, bit meaning by GMask


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool rcv_di_state(
        PGDevice d//!< device handle
        );

/** Returns the value in the local copy of the config.

Device inputs state, bit meaning by GMask

*/
DLL_PUBLIC uint16_t gt_di_state(
        PGDevice d//!< device handle
        );

/** Retrieves from the g.Estim device 

Stores the progress of the fade in or fade out (&lt;0 fade in, &gt;0 fade out, =0 no fade)


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool rcv_fade_progress(
        PGDevice d//!< device handle
        );

/** Returns the value in the local copy of the config.

Stores the progress of the fade in or fade out (&lt;0 fade in, &gt;0 fade out, =0 no fade)

*/
DLL_PUBLIC int16_t gt_fade_progress(
        PGDevice d//!< device handle
        );

/** Retrieves from the g.Estim device 

Sum of stimulation current samples during phase 1 [uA]


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool rcv_current1_sum(
        PGDevice d//!< device handle
        );

/** Returns the value in the local copy of the config.

Sum of stimulation current samples during phase 1 [uA]

*/
DLL_PUBLIC int32_t gt_current1_sum(
        PGDevice d//!< device handle
        );

/** Retrieves from the g.Estim device 

Sum of stimulation current samples during phase 2 [uA]


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool rcv_current2_sum(
        PGDevice d//!< device handle
        );

/** Returns the value in the local copy of the config.

Sum of stimulation current samples during phase 2 [uA]

*/
DLL_PUBLIC int32_t gt_current2_sum(
        PGDevice d//!< device handle
        );

/** Retrieves from the g.Estim device 

Sum of stimulation voltage samples during phase 1 [0.1V]


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool rcv_voltage1_sum(
        PGDevice d//!< device handle
        );

/** Returns the value in the local copy of the config.

Sum of stimulation voltage samples during phase 1 [0.1V]

*/
DLL_PUBLIC int32_t gt_voltage1_sum(
        PGDevice d//!< device handle
        );

/** Retrieves from the g.Estim device 

Average stimulation voltage samples during phase 2 [0.1V]


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool rcv_voltage2_sum(
        PGDevice d//!< device handle
        );

/** Returns the value in the local copy of the config.

Average stimulation voltage samples during phase 2 [0.1V]

*/
DLL_PUBLIC int32_t gt_voltage2_sum(
        PGDevice d//!< device handle
        );

/** Retrieves from the g.Estim device 

Counts the samples used for current1_sum_ or voltage1_sum_


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool rcv_duration1_sum(
        PGDevice d//!< device handle
        );

/** Returns the value in the local copy of the config.

Counts the samples used for current1_sum_ or voltage1_sum_

*/
DLL_PUBLIC uint16_t gt_duration1_sum(
        PGDevice d//!< device handle
        );

/** Retrieves from the g.Estim device 

Counts the samples used for current2_sum_ or voltage2_sum_


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool rcv_duration2_sum(
        PGDevice d//!< device handle
        );

/** Returns the value in the local copy of the config.

Counts the samples used for current2_sum_ or voltage2_sum_

*/
DLL_PUBLIC uint16_t gt_duration2_sum(
        PGDevice d//!< device handle
        );

/** Retrieves from the g.Estim device 

Minimum current measured in first phase during last train  [uA] (not used)


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool rcv_min_current_ph1(
        PGDevice d//!< device handle
        );

/** Returns the value in the local copy of the config.

Minimum current measured in first phase during last train  [uA] (not used)

*/
DLL_PUBLIC int16_t gt_min_current_ph1(
        PGDevice d//!< device handle
        );

/** Retrieves from the g.Estim device 

Maximum current measured in first phase during last train [uA] (not used)


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool rcv_max_current_ph1(
        PGDevice d//!< device handle
        );

/** Returns the value in the local copy of the config.

Maximum current measured in first phase during last train [uA] (not used)

*/
DLL_PUBLIC int16_t gt_max_current_ph1(
        PGDevice d//!< device handle
        );

/** Retrieves from the g.Estim device 

Minimum current measured in second phase during last train [V] (not used)


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool rcv_min_current_ph2(
        PGDevice d//!< device handle
        );

/** Returns the value in the local copy of the config.

Minimum current measured in second phase during last train [V] (not used)

*/
DLL_PUBLIC int16_t gt_min_current_ph2(
        PGDevice d//!< device handle
        );

/** Retrieves from the g.Estim device 

Maximum current measured in second phase during last train [V] (not used)


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool rcv_max_current_ph2(
        PGDevice d//!< device handle
        );

/** Returns the value in the local copy of the config.

Maximum current measured in second phase during last train [V] (not used)

*/
DLL_PUBLIC int16_t gt_max_current_ph2(
        PGDevice d//!< device handle
        );

/** Retrieves from the g.Estim device 

Number of stimulated pulses


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool rcv_applied_pulses(
        PGDevice d//!< device handle
        );

/** Returns the value in the local copy of the config.

Number of stimulated pulses

*/
DLL_PUBLIC uint16_t gt_applied_pulses(
        PGDevice d//!< device handle
        );

/** Retrieves from the g.Estim device 

Measured resistance [20 Ohm]


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool rcv_resistance(
        PGDevice d//!< device handle
        );

/** Returns the value in the local copy of the config.

Measured resistance [20 Ohm]

*/
DLL_PUBLIC uint16_t gt_resistance(
        PGDevice d//!< device handle
        );

/** Retrieves from the g.Estim device 

Current measurement array of last pulse [uA]


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool rcv_current(
        PGDevice d//!< device handle
        );

/** Returns the value in the local copy of the config.

Current measurement array of last pulse [uA]

*/
DLL_PUBLIC const int16_t* gt_current(
        PGDevice d//!< device handle
        );

/** Retrieves from the g.Estim device 

Voltage measurement array of last pulse [0.1 V]


@snippet gEstimAPI.h rcv_snd
*/
DLL_PUBLIC bool rcv_voltage(
        PGDevice d//!< device handle
        );

/** Returns the value in the local copy of the config.

Voltage measurement array of last pulse [0.1 V]

*/
DLL_PUBLIC const int16_t* gt_voltage(
        PGDevice d//!< device handle
        );


/** Open or connect to shared memory.

The shared memory is used to communicate with the SIMULINK model.

No functionality for FES.
*/
DLL_PUBLIC bool open_connect_sm(
    PGDevice d//!< device handle
    );


/** Close or disconnect the shared memory.

The shared memory is used to communicate with the SIMULINK model.


No functionality for FES.
*/
DLL_PUBLIC bool close_disconnect_sm(
    PGDevice d//!< device handle
    );



/** Check acceptable FW and HW versions.

Returns false, if actual FW and HW version does not fit to any of G_ESTIM_FW an G_ESTIM_HW.

openDevice() must be called with check_version==false to open the device, then this function can be called.
*/
DLL_PUBLIC bool fw_hw_ok(
    PGDevice d              //!< receives the device; reset iteration with NULL
    );


#ifdef __cplusplus
}// extern "C"
#endif 

#endif



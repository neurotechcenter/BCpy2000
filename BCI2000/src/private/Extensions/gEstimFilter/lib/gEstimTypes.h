/// \file gEstimTypes.h
/// \brief Type definitions for g.Estim (g.EstimPRO and g.EstimFES)
///
/// This files contains the device structure \ref _GDevice, which models the device, 
/// and all necessary type definitions of its fields.

// Thomas Kerbl (kerbl@gtec.at)
// Franz Patz (patz@gtec.at)
// Roland Puntaier (puntaier@gtec.at)
// 18.12.2014
// Guger Technologies OG
// Project: g.Estim


#ifndef GESTIMTYPES_H
#define  GESTIMTYPES_H


#if defined(_MSC_VER)
#define ALIGN_2 __declspec(align(16)) ///< alignment on 2 byte limit for MSVC
#else
#define ALIGN_2 __attribute__((aligned(2))) ///< alignment on 2 byte limit for GCC
#endif

#ifdef  __cplusplus
extern "C" {
#endif

#ifndef __cplusplus
#ifdef _MSC_VER
typedef int bool;
#define false 0
#define true 1
#else
#include <stdbool.h>
#endif
#endif

#include <stdint.h>

#ifndef __inline__
#define __inline__ __inline  ///< GCC has __inline__ defined by default. This is for MSVC.
#endif

//these must be even numbers
#define SERIAL_SIZE 16          ///< Size of serial buffer definition
#define IDENTIFIER_SIZE 64      ///< Size of description buffer definition
#define FWVERSION_SIZE 20       ///< Size of firmware version buffer definition
#define HWVERSION_SIZE 20       ///< Size of hardware version buffer definition
#define SAMPLES_SIZE 400        ///< Number of measurement samples definition

/// GError enum Error codes for API and Firmware
typedef enum
{
  //GERROR_xxx lines are parsed and used in gt_error_text API function
  //only append new error codes, else old error codes get shifted
  //^\s*(GERROR_\w+),\s*                                               ///< \s*(.*)'
  GERROR_SUCCESS,                                                      ///< No error occured
  GERROR_SELFTEST_NOT_DONE,                                            ///< Self test not done. This is the beginning error state until selftest has finished
  GERROR_FUNCTION_NONE,                                                ///< Self test: disconnecting test resistance failed
  GERROR_SELFTESTMEASURE,                                              ///< Self test: measured values failed
  GERROR_SELFTESTHIGHVOLTAGE,                                          ///< Self test: highvoltage test failed
  GERROR_SELFTESTRESISTANCE,                                           ///< Self test: resistance tes tfailed
  GERROR_LOCK_BY_API_NOT_ALLOWED,                                      ///< It is not possible to lock the device via the API

  //MAX MIN ERROR constants
  GERROR_MIN_VOLTAGE_LIMIT,                                            ///< voltage limit is too small
  GERROR_MAX_VOLTAGE_LIMIT,                                            ///< voltage limit is too big

  GERROR_MIN_ALTERNATE,                                                ///< alternate is too small
  GERROR_MAX_ALTERNATE,                                                ///< alternate is too big
  GERROR_MIN_PHASE_CURRENT1,                                           ///< phase_current1 is too small
  GERROR_MAX_PHASE_CURRENT1,                                           ///< phase_current1 is too big
  GERROR_MIN_PHASE_CURRENT2,                                           ///< phase_current2 is too small
  GERROR_MAX_PHASE_CURRENT2,                                           ///< phase_current2 is too big
  GERROR_MIN_PHASE_DURATION1,                                          ///< phase_duration1 is too small
  GERROR_MAX_PHASE_DURATION1,                                          ///< phase_duration1 is too big
  GERROR_MIN_PHASE_DURATION2,                                          ///< phase_duration2 is too small
  GERROR_MAX_PHASE_DURATION2,                                          ///< phase_duration2 is too big
  GERROR_MIN_FADE_IN_TIME,                                             ///< fade_in_time is too small
  GERROR_MAX_FADE_IN_TIME,                                             ///< fade_in_time is too big
  GERROR_MIN_FADE_OUT_TIME,                                            ///< fade_out_time is too small
  GERROR_MAX_FADE_OUT_TIME,                                            ///< fade_out_time is too big
  GERROR_MIN_FADE_TYPE,                                                ///< fade_in_time is too small
  GERROR_MAX_FADE_TYPE,                                                ///< fade_in_time is too big
  GERROR_MIN_INTERPHASE,                                               ///< interphase is too small
  GERROR_MAX_INTERPHASE,                                               ///< interphase is too big
  GERROR_MIN_PULSE_RATE,                                               ///< pulse_rate_ is too small
  GERROR_MAX_PULSE_RATE,                                               ///< pulse_rate_ is too big
  GERROR_MIN_PULSES,                                                   ///< pulses is too small
  GERROR_MAX_PULSES,                                                   ///< pulses is too big
  GERROR_MIN_DO1_PRE_TIME,                                             ///< do1_pre_time is too small
  GERROR_MAX_DO1_PRE_TIME,                                             ///< do1_pre_time is too big
  GERROR_MIN_DO1_POST_TIME,                                            ///< do1_post_time is too small
  GERROR_MAX_DO1_POST_TIME,                                            ///< do1_post_time is too big
  GERROR_MIN_DO2_PRE_TIME,                                             ///< do2_pre_time is too small
  GERROR_MAX_DO2_PRE_TIME,                                             ///< do2_pre_time is too big
  GERROR_MIN_DO2_POST_TIME,                                            ///< do2_post_time is too small
  GERROR_MAX_DO2_POST_TIME,                                            ///< do2_post_time is too big
  GERROR_MIN_DO1_FUNCTION,                                             ///< do1_function is too small
  GERROR_MAX_DO1_FUNCTION,                                             ///< do1_function is too big
  GERROR_MIN_DO2_FUNCTION,                                             ///< do2_function is too small
  GERROR_MAX_DO2_FUNCTION,                                             ///< do2_function is too big
  GERROR_MIN_DIO_ENABLED,                                              ///< dio_enabled is too small
  GERROR_MAX_DIO_ENABLED,                                              ///< dio_enabled is too big
  GERROR_MIN_HIGHVOLTAGE_LED,                                          ///< highvoltage_led can only be set to zero
  GERROR_MAX_HIGHVOLTAGE_LED,                                          ///< highvoltage_led can only be set to zero
  GERROR_MIN_O_FUNCTION,                                               ///< o_function is too small
  GERROR_MAX_O_FUNCTION,                                               ///< o_function is too big
  GERROR_MAX_TOTAL_CHARGE,                                             ///< total_charge can only be set to zero

  GERROR_MIN_OPERATIONMODE,                                            ///< operationmode is too small
  GERROR_MAX_OPERATIONMODE,                                            ///< operationmode is too big
  GERROR_MIN_TRAIN_RATE,                                               ///< train_rate_ is too small
  GERROR_MAX_TRAIN_RATE,                                               ///< train_rate_ is too big
  GERROR_MIN_N_TRAINS,                                                 ///< n_trains is too small
  GERROR_MAX_N_TRAINS,                                                 ///< n_trains is too big
  GERROR_MIN_JITTER,                                                   ///< jitter is too small
  GERROR_MAX_JITTER,                                                   ///< jitter is too big
  GERROR_MIN_EL_TYPE,                                                  ///< el_type is too small
  GERROR_MAX_EL_TYPE,                                                  ///< el_type is too big
  GERROR_MIN_EL_CIRC_DIAMETER,                                         ///< el_circ_diameter is too small
  GERROR_MAX_EL_CIRC_DIAMETER,                                         ///< el_circ_diameter is too big
  GERROR_MIN_EL_DEPTH_DIAMETER,                                        ///< el_depth_diameter is too small
  GERROR_MAX_EL_DEPTH_DIAMETER,                                        ///< el_depth_diameter is too big
  GERROR_MIN_EL_DEPTH_LENGTH,                                          ///< el_depth_length is too small
  GERROR_MAX_EL_DEPTH_LENGTH,                                          ///< el_depth_length is too big
  GERROR_MIN_EL_OTHER_SURFACE,                                         ///< el_other_surface is too small
  GERROR_MAX_EL_OTHER_SURFACE,                                         ///< el_other_surface is too big

  GERROR_CHARGE_DENSITY,                                               ///< Stimulation with charge densities above MAX_CHARGE_DENSITY is dangerous and therefore not allowed.

  GERROR_PULSE_WRONG_PHASES,                                           ///< Phase current/duration 1 and 2, if not zero, must be equal in size and opposite in sign
  GERROR_FADE_OUT,                                                     ///< During fade out no stimulation parameter has to be changed
  GERROR_RAMP_DURATION,                                                ///< The fade in and fade out time lead to a ramp duration longer than the train duration
  GERROR_PULSE_PERIOD_DURATION,                                        ///< Pulse onset interval must leave space for the length of two phases and two inter-phase durations.
  GERROR_DO12_PULSE_TRAIN,                                             ///< Pre/post_time for train on one DO must be longer than for pulse on the other DO.
  GERROR_NO_DO1_PRE_TIME_FOR_PHASE,                                    ///< Setting DO1 pre_time for phase is not possible
  GERROR_NO_DO1_POST_TIME_FOR_PHASE,                                   ///< Setting DO1 post_time for phase is not possible
  GERROR_NO_DO2_PRE_TIME_FOR_PHASE,                                    ///< Setting DO2 pre time for phase is not possible
  GERROR_NO_DO2_POST_TIME_FOR_PHASE,                                   ///< Setting DO2 post_time for phase is not possible
  GERROR_ENABLE_SIMULTANEOUS_HAND_FOOT,                                ///< Enabling hand and foot switch simultaneously is not possible

  GERROR_RESISTANCE,                                                   ///< Unable to do resistance measurement

  //send errors: PC and UC that don't reach UC_other
  GERROR_TIMEOUT_FRAME,                                                ///< Communication timeout for next frame (GERROR_TIMEOUT_FRAME)
  GERROR_FTDIAPI,                                                      ///< Communication ERROR (FTDI did not return FT_OK)
  GERROR_FTDINONEFOUND,                                                ///< No device found
  GERROR_REQUEST_TIMEOUT_FRAME,                                        ///< Answer to request not within timeout (GERROR_REQUEST_TIMEOUT_FRAME)
  GERROR_REQUEST_TIMEOUT,                                              ///< Bytes of frame returned with too much delay (GERROR_REQUEST_TIMEOUT)
  GERROR_MIN_TOTAL_CHARGE,                                             ///< total_charge can only be set to zero
  GERROR_PHASE_WRONG_POLARITY,                                         ///< Phase current 1 or 2, has opposite polarity than current one (if changed during stimulation)
  GERROR_INVALIDDEVICE,                                                ///< Invalid device. SW is not made for this device.
  GERROR_EARLY_TRAIN_START,                                            ///< Between two train starts, at minimum one period has to be waited.
  GERROR_ACKNOWLEDGE,                                                  ///< No acknowledgement for message received.
  GERROR_CRC,                                                          ///< Invalid CRC checksum received
  GERROR_RESPONSE,                                                     ///< Response does not fit to request
  GERROR_TRAIN_DURATION_EXCEEDED,                                      ///< The maximal train duration exceeded.
  GERROR_TRAIN_PERIOD_EXCEEDED,                                        ///< The train rate is too high, leaving no room for the train
  GERROR_RESERVED09,                                                   ///< Not used.
  GERROR_WRITESTATE,                                                   ///< Invalid state received or invalid state change requested
  GERROR_NOT_ACTIVE,                                                   ///< Operation only works in active state, or stopped due to some error (e.g. not enabled due to foot switch not pressed)
  GERROR_OPERATION_ABORTED,                                            ///< Operation was aborted before completion
  GERROR_TRAIN_DURATION_BELOW_LIMIT,                                   ///< Train duration below minimal limit.
  GERROR_RESERVED11,                                                   ///< Not used.
  GERROR_RESERVED12,                                                   ///< Not used.
  GERROR_RESERVED13,                                                   ///< Not used.

  //device errors
  GERROR_RESERVED14,                                                   ///< Not used.
  GERROR_WRITENOTSTOPPED,                                              ///< Write command while device not stopped
  GERROR_WRITEATREADONLY,                                              ///< Write command in read only data segment received
  GERROR_WRITEDEVINFO,                                                 ///< Failed to program device program memory with serial, identifier,...
  GERROR_CALIBRATE,                                                    ///< Failed to calibrate device ,...
  GERROR_READSTIMDATA,                                                 ///< Read command at stimulation data while device not stopped
  GERROR_RESERVED15,                                                   ///< Not used.
  GERROR_CONTENT,                                                      ///< Invalid offset, length or combination of them received
  GERROR_LENGTH,                                                       ///< Invalid length (in combination with the offset) received
  GERROR_COMMUNICATIONUCS,                                             ///< UART communication between uC1 and uC2 failed
  GERROR_UC2TIMEOUT,                                                   ///< uC2 did not respond within time to uC1
  GERROR_RESERVED16,                                                   ///< Not used.
  GERROR_LOWBATSTOP,                                                   ///< Device in STOP state until batteries replaced
  GERROR_LOCKSTATE,                                                    ///< Device is in Lock state, no write command is processed
  GERROR_NOENABLESIGNAL,                                               ///< Device still in ACTIVE state, because foot switch not pressed
  GERROR_CHARGELIMIT,                                                  ///< Charge limit reached with next stimulation
  GERROR_TRANSITIONLIMIT,                                              ///< Transition limit reached --> stimulation is only possible between two read commands
  GERROR_RESERVED17,                                                   ///< Not used.
  GERROR_DONGLE,                                                       ///< GESTIM_MODE_ADVANCED requires a dongle with feature 106
  GERROR_WRITE_HW_FW_VERSION,                                          ///< Failed to program device program hardware- or firmware version, because already programmed
   //...

  GERROR_MMF,                                                          ///< Failed to create memory mapped file for SIMULINK.

  GERROR_LOWSUPPLY,                                                    ///< Failure due to low voltage supply via USB.

  GERROR_TIMEOUT,                                                      ///< communication timeout for next byte in frame (GERROR_TIMEOUT)

  GERROR_END                                                           ///< GError values must be smaller
} GError;

/// GState enum Device states
typedef enum
{
  GSTATE_STOP,                 ///< Device is in STOP state
  GSTATE_ACTIVE,               ///< Device is in ACTIVE state
  GSTATE_RESISTANCE,           ///< Device is in RESISTANCE measurement state
  GSTATE_STIMULATION,          ///< Device is in STIMULATION state
  GSTATE_LOCK,                 ///< Device is in LOCK state, this is private and can only be reset by g.tec
  GSTATE_END                   ///< GState values must be smaller
} GState;

/// GCommand enum Commands for communication with the device (read/write)
/// This is the MSB of the first uint16_t in the transfer buffer
typedef enum
{
  GCOMMAND_WRITE,              ///< Command to write data to device
  GCOMMAND_READ                ///< Command to read data from device
} GCommand;

/// GEvent enum Combines all events
typedef enum
{
  GEVENT_START,                ///< uC started
  GEVENT_API,                  ///< State change due to API
  GEVENT_UC1,                  ///< State change on UC2 due to "software-request" from uC1
  
  //event_chargelimitstop,
  GEVENT_HIGHVOLTAGE,          ///< State change due to high voltage 
  GEVENT_AUTOSTOP1,            ///< Automatic stop of device because too long in active state without activity
  GEVENT_AUTOSTOP2,            ///< Automatic stop of device because no heartbeat for more then a second
  GEVENT_AUTOSTOP3,            ///< Automatic stop because of uC1-uC2 connection failure
  GEVENT_DI1,                  ///< State change due to digital input 1
  GEVENT_FOOT,                 ///< State change due to FOOT enable/disable
  GEVENT_HANDABORT,            ///< State change due to HAND abort button
  GEVENT_HANDSTART,            ///< State change due to HAND start button
  GEVENT_ENDOFTRAIN,           ///< State change due to end of train
  GEVENT_HIGHCURRENT,          ///< State change due to high current detection
  GEVENT_LOWCURRENT,           ///< State change due to low current detection
  GEVENT_LOWBATTERY,           ///< State change due to low battery detection
  GEVENT_INVALID_STIM_TRIGGER, ///< UC2 received a STIM trigger from UC1 while not in ACTIVE
  GEVENT_INVALID_STIM_TIMING,  ///< UC2 detected a (stimulation)pulse trigger at wrong time instance during STIMULATION
  GEVENT_LOWSUPPLY,            ///< State change due to lowsupply voltage detection
  GEVENT_HIGHSUPPLY,           ///< State change due to high supply voltage detection
  GEVENT_UC2,                  ///< State change due to uC2 event (e.g. GEVENT_HIGHCURRENT, GEVENT_HIGHVOLTAGE)      
  GEVENT_ENDOFIMPMEAS,         ///< State change due to end of impedance measurement
  GEVENT_END                   ///< GEvent values must be smaller
} GEvent;

/// GMask enum Enum to mask digital inputs {usage e.g.: io&(1<<GMASK_HANDABORT)}
typedef enum
{
  GMASK_DI1,                  ///< Masks digital input 1
  GMASK_FOOT,                 ///< Masks digital input 2
  GMASK_HANDABORT,            ///< Masks digital input 3
  GMASK_HANDSTART,            ///< Masks digital input 4
  GMASK_DO1,                  ///< Masks digital output 1
  GMASK_DO2,                  ///< Masks digital output 2
  GMASK_BUZZER,               ///< Masks buzzer
  GMASK_MMF,                  ///< Masks SW trigger via memory mapped file temppath\serial.mmf (used for communication with Simulink) 
  GMASK_NOAUTOSTOP,           ///< suppress autostop of device
  GMASK_MEASUREMENT,          ///< Masks all stimulation measurements, 1... to disable standard measurements  0...to enable standard measurements
  GMASK_NOLIMITSCHECK,        ///< Suppresses GEVENT_HIGHCURRENT, GEVENT_HIGHVOLTAGE, GEVENT_LOWCURRENT, if internal jumper set
  GMASK_END                   ///< GMask values must be smaller
} GMask;

/// GFade codes the type of in/out fade
typedef enum
{
  GFADE_PHASE,                ///< in/out fade using phase width
  GFADE_AMPLITUDE,            ///< in/out fade using amplitude
  GFADE_BOTH,                 ///< in/out fade using both phase and amplitude
  GFADE_END                   ///< GFade values must be smaller
} GFade;

///Type for GDevice::do1_function_ and the GDevice::do2_function_
typedef enum
{
  GDO_TRAIN,           ///< Digital out codes train
  GDO_PULSE,           ///< Digital out codes pulses
  GDO_PHASE,           ///< Digital out codes phases
  GDO_END              ///< GDO values must be smaller
} GDO;

///Type for GDevice::o_function_
typedef enum
{
   GOFUNCTION_TEST,  ///< Test impedance is connected to current source, output is disconnected from current source
   GOFUNCTION_OUT,   ///< Test impedance is disconnected from current source
   GOFUNCTION_NONE,  ///< Test impedance is disconnected and output is disconnected from current source
   GOFUNCTION_END    ///< GFunction values must be smaller
} GOFunction;

///Type for GDevice::alternate_
typedef enum
{
  GALTERNATE_NO,    ///< not alternate, i.e. the pulses stay the same
  GALTERNATE_YES,   ///< alternate, i.e. the following pulse has the sign of the phases reversed
  GALTERNATE_END    ///< GAlternate values must be smaller
} GAlternate;

//requirements:
//- #include <stdbool.h>
//- #include <stdint.h>
//- parsed, keep format
//- order of fields is relevant //change also 201509021402 and constants below

/// \brief Device cofiguration data structure (R - read, W - write)
#pragma pack(push, 2)
typedef struct _GDevice
{
  // Device information
  char serial_[SERIAL_SIZE];             ///< RW - Device serial number e.g. "ES-2014.12.14"
  char identifier_[IDENTIFIER_SIZE];     ///< RW - Device identifier
  char FW_version_[FWVERSION_SIZE];      ///< RW - Device firmware version
  char HW_version_[HWVERSION_SIZE];      ///< RW - Device hardware version
  int16_t coeff_cs1_[2];                 ///< RW - Device current source 1 calibration coefficients [1/1000,1uA]
  int16_t coeff_cs2_[2];                 ///< RW - Device current source 2 calibration coefficients [1/1000,1uA]
  int16_t coeff_i_[2];                   ///< RW - Device current measurement calibration coefficients [1/1000,10uA]
  int16_t coeff_v_[2];                   ///< RW - Device voltage measurement calibration coefficients [1/1000,0.1V]
  int16_t coeff_r_[2];                   ///< RW - Device resistance measurement calibration coefficients [1/1000,20 Ohm]

  // Stimulation settings
  uint16_t voltage_limit_;               ///< RW - Voltage above which GEVENT_HIGHVOLTAGE will happen
  uint16_t alternate_;                   ///< RW - Alternating setting \see GAlternate
  int16_t phase_current1_;               ///< RW - Current phase 1 [10 uA]
  int16_t phase_current2_;               ///< RW - Current phase 2 [10 uA]. This must be minus phase_current1_. 
  uint16_t phase_duration1_;             ///< RW - duration phase 1 [10 us]
  uint16_t phase_duration2_;             ///< RW - duration phase 2 [10 us]. This must be equal to phase_current1_ (or 0 = monophasic, for PRO only)
  uint16_t fade_in_time_;                ///< RW - fade in time of train [ms]
  uint16_t fade_out_time_;               ///< RW - fade out time of train [ms]
  uint16_t fade_type_;                   ///< RW - fade in/out using phase or amplitude or both \see GFade
  uint16_t interphase_;                  ///< RW - duration between phases [10 us]. This must be 0 if phase_duration2_ is 0.
  uint16_t pulse_rate_;                  ///< RW - stimulation pulse rate [0.1 Hz]
  uint16_t pulses_;                      ///< RW - number of stimulation pulses
  uint16_t do1_pre_time_;                ///< RW - pre-stimulation time DO1 [ms]
  uint16_t do1_post_time_;               ///< RW - post-stimulation time DO1 [ms]
  uint16_t do2_pre_time_;                ///< RW - pre-stimulation time DO2 [ms]
  uint16_t do2_post_time_;               ///< RW - post-stimulation time DO2 [ms]
  uint16_t do1_function_;                ///< RW - DO1 functionality setting \see GDO
  uint16_t do2_function_;                ///< RW - DO2 functionality setting \see GDO
  uint16_t dio_enabled_;                 ///< RW - DIO enable settings e.g. (ioenabled&(1<<GMASK_DI1))
  uint16_t highvoltage_led_;             ///< RW - Actual state of high voltage LED (1 = high-voltage)
  uint16_t o_function_;                  ///< RW - Output functionality setting \see GOFunction
  uint16_t state_;                       ///< RW - Device state \see GState
  int32_t total_charge_;                 ///< RW - Total applied stimulation charge [10pC]. Can only be reset.
  uint16_t event_;                       ///< R - Shows which event lead to current device state \see GEvent
  uint16_t error_;                       ///< R - Device error state \see GError
  uint16_t stim_event_;                  ///< R - Shows which event lead to last stimulation state \see GEvent
  uint16_t battery1_;                    ///< R - Device battery 1 charge with values {0, 100-200}
  uint16_t battery2_;                    ///< R - Device battery 2 charge with values {0, 100-200}
  uint16_t di_state_;                    ///< R - Device inputs state, bit meaning by GMask
  int16_t fade_progress_;                ///< R - Stores the progress of the fade in or fade out (<0 fade in, >0 fade out, =0 no fade)
  int32_t current1_sum_;                 ///< R - Sum of stimulation current samples during phase 1 [uA]
  int32_t current2_sum_;                 ///< R - Sum of stimulation current samples during phase 2 [uA]
  int32_t voltage1_sum_;                 ///< R - Sum of stimulation voltage samples during phase 1 [0.1V]
  int32_t voltage2_sum_;                 ///< R - Average stimulation voltage samples during phase 2 [0.1V]
  uint16_t duration1_sum_;               ///< R - Counts the samples used for current1_sum_ or voltage1_sum_
  uint16_t duration2_sum_;               ///< R - Counts the samples used for current2_sum_ or voltage2_sum_
  int16_t min_current_ph1_;              ///< R - Minimum current measured in first phase during last train  [uA] (not used)
  int16_t max_current_ph1_;              ///< R - Maximum current measured in first phase during last train [uA] (not used)
  int16_t min_current_ph2_;              ///< R - Minimum current measured in second phase during last train [V] (not used)
  int16_t max_current_ph2_;              ///< R - Maximum current measured in second phase during last train [V] (not used)
  uint16_t applied_pulses_;              ///< R - Number of stimulated pulses
  uint16_t resistance_;                  ///< R - Measured resistance [20 Ohm]
  int16_t current_[SAMPLES_SIZE];        ///< R - Current measurement array of last pulse [uA]
  int16_t voltage_[SAMPLES_SIZE];        ///< R - Voltage measurement array of last pulse [0.1 V]
} 
ALIGN_2  // Compiler align variable to 2-->neccessary!
GDevice;
#pragma pack(pop)

typedef GDevice* PGDevice; ///< type for the pointer to GDevice

/// some compatibility stuff
#ifdef __XC16__ 
#define rwcode(m) ((uint16_t)(&((GDevice*)0)->m)) ///< Definition to get address of <b>m</b> in ::GDevice
#else
#define rwcode(m) ((uintptr_t)(&((GDevice*)0)->m)) ///< Definition to get address of <b>m</b> in ::GDevice
#endif

#define START_STIM rwcode(alternate_)             ///< Offset of the start of the stimulation parameters
#define END_STIM rwcode(do1_pre_time_)            ///< Offset of the end of stimulation parameters
#define END_WRITE rwcode(event_)                  ///< End of writable fields .
#define START_STATE rwcode(state_)                ///< Start of state fields. These will be updated by the communication thread every PING_TIME_MS (200)
#define START_MEASURES rwcode(current1_sum_)      ///< Begin of measured values
#define START_PULSE_DATA rwcode(min_current_ph1_) ///< Begin of pulse data
#define START_SAMPLES rwcode(current_)            ///< Begin of samples of the last pulse

#define NOT_AVAILABLE_CURRENT1_SUM_  2147483647 ///< this value means that the data is not available
#define NOT_AVAILABLE_CURRENT2_SUM_  2147483647 ///< this value means that the data is not available
#define NOT_AVAILABLE_VOLTAGE1_SUM_  2147483647 ///< this value means that the data is not available
#define NOT_AVAILABLE_VOLTAGE2_SUM_  2147483647 ///< this value means that the data is not available
#define NOT_AVAILABLE_MIN_CURRENT_PH1_  32767   ///< this value means that the data is not available
#define NOT_AVAILABLE_MAX_CURRENT_PH1_  -32768  ///< this value means that the data is not available
#define NOT_AVAILABLE_MIN_CURRENT_PH2_  32767   ///< this value means that the data is not available
#define NOT_AVAILABLE_MAX_CURRENT_PH2_  -32768  ///< this value means that the data is not available
#define NOT_AVAILABLE_APPLIED_PULSES_  65535    ///< this value means that the data is not available
#define NOT_AVAILABLE_RESISTANCE_  65535        ///< this value means that the data is not available

#ifdef  __cplusplus
} //extern
#endif

#endif  /* GESTIMTYPES_H */

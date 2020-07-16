/// \file gEstimDefaults.h
/// \brief Defaults and limits for g.Estim
///
/// This files contains defaults for the device. The API has additional defaults for the different modi set by st_operationmode() (\ref defaults).

// Thomas Kerbl (kerbl@gtec.at)
// Franz Patz (patz@gtec.at)
// Roland Puntaier (puntaier@gtec.at)
// 18.12.2014
// Guger Technologies OG
//
// Project: g.Estim
  

#ifndef GESTIMDEFAULTS_H
#define GESTIMDEFAULTS_H

#include "gEstimTypes.h"

//ONE OF THESE DEFINES NEEDS TO BE PROVIDED VIA COMMAND LINE BY THE BUILD SYSTEM
//#define GESTIMPRO ///< This define is used to compile for the g.EstimPRO device instead of the g.EstimFES device
//#define GESTIMFES ///< This define is used to compile for the g.EstimFES device instead of the g.EstimPRO device

#define UART_BAUDRATE 500000     ///< UART 1 baudrate definition. A value different from 1000000 must be placed at the end of the FTDI Description field as kBd. E.g. 500kBd.

//unit factors to SI unit
#define UNIT_RESISTANCE 20           ///< the unit factor used for GDevice::resistance_
#define UNIT_CURRENT 1e-5            ///< the unit factor used for GDevice::phase_current1_ and the like
#define UNIT_PULSE_RATE 0.1          ///< the unit factor used for GDevice::pulse_rate_ (GExtra::train_rate_ is in Hz, though)
#define UNIT_DO_AND_FADE_TIME 1e-3   ///< the unit factor used for GDevice::do1_pre_time_ and the like
#define UNIT_TOTAL_CHARGE 1e-11      ///< the unit factor used for GDevice::total_charge_
#define UNIT_VOLTAGE_uC2 0.1         ///< the unit factor used for GDevice::voltage_ and the like
#define UNIT_ELECTRODE_LEN 1e-6      ///< the unit factor used for GExtra::el_circ_diameter_ and the like
#define UNIT_TIME 1e-5               ///< the unit factor used for gt_pulse_duration() and the like

#define R_SENSE 150               ///< Sensing resistor for current measurement in ohm
#define NBR_CUT_SAMPLES_START 0   ///< number of samples that are ignored (for limits and sums) at the beginning of each phase
#define NBR_CUT_SAMPLES_END 0     ///< number of samples that are ignored (for limits and sums) at the end of each phase

/// current average in uA
#define calcCurAvg(cur_sum,dur_sum) (dur_sum)?((cur_sum)/(dur_sum-NBR_CUT_SAMPLES_START-NBR_CUT_SAMPLES_END)):0 

/// voltage average in V
#define calcVolAvg(vol_sum,dur_sum,cur_avg) (dur_sum)?(((UNIT_VOLTAGE_uC2*vol_sum)/(dur_sum-NBR_CUT_SAMPLES_START-NBR_CUT_SAMPLES_END))-R_SENSE*cur_avg*1E-6):0.0 

// Limits stimulation settings
#define G_ES_HIGH_PHASE_CURRENT 1000        ///< values above this current are considered dangerous
#define G_ES_MAX_PHASE_CURRENT 1500         ///< Max. allowed current in 10uA
#define G_ES_MAX_PHASE_CURRENT_CUBE 1500   ///< Max. allowed current @CUBE in 40uA -->60mA
#define G_ES_MIN_PHASE_CURRENT 5            ///< Min. allowed current in 10uA
#define G_ES_MIN_PHASE_CURRENT_FES  0       ///< Min. allowed current in 4x10uA
#define G_ES_MIN_PHASE_CURRENT_CUBE 0       ///< Min. allowed current @CUBE in 10uA --> 0mA
#define G_ES_MAX_PHASE_DURATION 100         ///< Max. allowed phase duration in 10us
#define G_ES_MAX_PHASE_DURATION_FES 40      ///< Max. allowed phase duration in 10us
#define G_ES_MIN_PHASE_DURATION 5           ///< Min. allowed phase duration in 10us
#define G_ES_MIN_FADE_IN_TIME 0             ///< Min. allowed fade in time is 0s
#define G_ES_MAX_FADE_IN_TIME 10000         ///< Min. allowed fade in time is 10s (10000ms)
#define G_ES_MIN_FADE_OUT_TIME 0            ///< Min. allowed fade in time is 0s
#define G_ES_MAX_FADE_OUT_TIME 10000        ///< Min. allowed fade in time is 10s (10000ms)
#define G_ES_MAX_INTERPHASE_DURATION 100    ///< Max. allowed interphase duration in 10us
#define G_ES_MAX_INTERPHASE_DURATION_FES 40 ///< Max. allowed interphase duration in 10us
#define G_ES_MAX_INTERPHASE_DURATION_CUBE 40 ///< Max. allowed interphase duration in 10us
#define G_ES_MIN_INTERPHASE_DURATION_CUBE 5 ///< Min. allowed interphase duration @CUBE in 10us
#define G_ES_MIN_INTERPHASE_DURATION 0      ///< Min. allowed interphase duration in 10us
#define G_ES_MAX_PULSE_RATE 5000            ///< Max. allowed pulse rate in 0.1Hz
#define G_ES_MIN_PULSE_RATE 1               ///< Min. allowed pulse rate in 0.1Hz
#define G_ES_MAX_PULSE_RATE_FES 1000        ///< Max. allowed pulse rate in 0.1Hz
#define G_ES_MIN_PULSE_RATE_FES 10          ///< Min. allowed pulse rate in 0.1Hz
#define G_ES_MAX_PULSES 30000               ///< Max. number of allowed pulses
#define G_ES_MAX_PULSES_FES 2000            ///< Max. number of allowed pulses
#define G_ES_MIN_PULSES 1                   ///< Min. number of allowed pulses
#define G_ES_MIN_PULSES_FES 2               ///< Min. number of allowed pulses
#define G_ES_MAX_TRAIN_DURATION_FES 20000   ///< Max. allowed train duration in ms
#define G_ES_MIN_TRAIN_DURATION_FES 2000    ///< Min. allowed train duration in ms
#define G_ES_MAX_DO1_PRE_TIME 100           ///< Max. allowed DO1 pre stimulation time in ms
#define G_ES_MIN_DO1_PRE_TIME 0             ///< Min. allowed DO1 pre stimulation time in ms
#define G_ES_MAX_DO1_POST_TIME 100          ///< Max. allowed DO1 post stimulation time in ms
#define G_ES_MIN_DO1_POST_TIME 0            ///< Min. allowed DO1 post stimulation time in ms
#define G_ES_MAX_DO2_PRE_TIME 100           ///< Max. allowed DO2 pre stimulation time in ms
#define G_ES_MIN_DO2_PRE_TIME 0             ///< Min. allowed DO2 pre stimulation time in ms
#define G_ES_MAX_DO2_POST_TIME 100          ///< Max. allowed DO2 post stimulation time in ms
#define G_ES_MIN_DO2_POST_TIME 0            ///< Min. allowed DO2 post stimulation time in ms
#define G_ES_MIN_BATTERY 100                ///< Minimum battery charge in %. This might be changed.
#define G_ES_20PC_BATTERY 120               ///< 20% battery charge
#define G_ES_10PC_BATTERY 110               ///< 10% battery charge
#define G_ES_MAX_BATTERY 200                ///< Maximum battery charge in %

#define TOTAL_CHARGE_LIMIT      1500000     ///< total charge limit in 10pC amounting to 15uC
#define TOTAL_CHARGE_LIMIT_FES   600000     ///< total charge limit in 40pC amounting to 24uC

#define G_ES_LOW_RESISTANCE 5               ///< Value below which resistance is considered low, without finer distinction.
#define G_ES_MAX_RESISTANCE 30000           ///< lower limit value for resistance measurement with GOFUNCTION_NONE, applicable for PRO and FES
#define TEST_RESISTANCE_PRO 280             ///< nominal value of test resitance (5600 Ohm) of PRO device in UNIT_RESISTANCE
#define TEST_RESISTANCE_FES 70              ///< nominal value of test resitance (1400 Ohm) of FES device in UNIT_RESISTANCE
#define RESISTANCE_MEAS_I   8               ///< auxiliary current during resistance measurement [uA]

#define FES_FACTOR 4                        ///< current for FES scaled by this factor with regard to settings

#define G_ES_VOLTAGE_LIMIT 800             ///< Voltage (in 0.1V units) above which GERROR_HIGHVOLTAGE will be raised
#define G_ES_MAX_VOLTAGE_LIMIT 800         ///< Max. allowed voltage limit in 0.1V
#define G_ES_MIN_VOLTAGE_LIMIT 100         ///< Min. allowed voltage limit in 0.1V

#define AUTOSTOP1_TIMOUT_SEC 300            ///< Device goes from GSTATE_ACTIVE to GSTATE_STOP after this time [s]
#define AUTOSTOP2_TIMOUT_SEC 2              ///< Device goes from GSTATE_ACTIVE to GSTATE_STOP after this time [s]

// use like: GDevice device = DEVICE_DEFAULTS;
// 201509021402
/// THE STIMULATION VALUES IN FOLLOWING SECTION MUST NOT BE CHANGED BY ANYBODY!!!
#ifdef GESTIMPRO
#define DEVICE_DEFAULTS {\
  /* serial_          */ {"\x0"},\
  /* identifier_      */ {"\x0"},\
  /* HW_version_      */ {"\x0"},\
  /* FW_version_      */ {"\x0"},\
  /* coeff_cs1        */ {1000,0},\
  /* coeff_cs2        */ {1000,0},\
  /* coeff_i          */ {1000,0},\
  /* coeff_v          */ {1000,0},\
  /* coeff_r          */ {1000,0},\
  /* voltage_limit_  */  G_ES_VOLTAGE_LIMIT,\
  /* alternate_       */  GALTERNATE_NO,\
  /* phase_current1_  */  0,\
  /* phase_current2_  */  0,\
  /* phase_duration1_ */  5,\
  /* phase_duration2_ */  5,\
  /* fade_in_time_    */  0,\
  /* fade_out_time_   */  0,\
  /* fade_type_       */  GFADE_PHASE,\
  /* interphase_      */  G_ES_MIN_INTERPHASE_DURATION,\
  /* pulse_rate_       */  G_ES_MIN_PULSE_RATE,\
  /* pulses_          */  G_ES_MIN_PULSES,\
  /* do1_pre_time_    */  G_ES_MIN_DO1_PRE_TIME,\
  /* do1_post_time_   */  G_ES_MIN_DO1_POST_TIME,\
  /* do2_pre_time_    */  G_ES_MIN_DO2_PRE_TIME,\
  /* do2_post_time_   */  G_ES_MIN_DO2_POST_TIME,\
  /* do1_function_    */  GDO_TRAIN,\
  /* do2_function_    */  GDO_TRAIN,\
  /* dio_enabled_     */  0,\
  /* highvoltage_led_    */  0,\
  /* o_function_      */  GOFUNCTION_TEST,\
  /* state_           */  GSTATE_STOP,\
  /* total_charge_    */  0,\
  /* event_           */  GEVENT_START,\
  /* error_           */  GERROR_SUCCESS,\
  /* stim_event_      */  GEVENT_START,\
  /* battery1_        */  G_ES_MIN_BATTERY,\
  /* battery2_        */  G_ES_MIN_BATTERY,\
  /* di_state_        */  0,\
  /* fade_progress_   */  0,\
  /* current1_sum_    */  NOT_AVAILABLE_CURRENT1_SUM_   ,\
  /* current2_sum_    */  NOT_AVAILABLE_CURRENT2_SUM_   ,\
  /* voltage1_sum_    */  NOT_AVAILABLE_VOLTAGE1_SUM_   ,\
  /* voltage2_sum_    */  NOT_AVAILABLE_VOLTAGE2_SUM_   ,\
  /* duration1_sum_   */  0 ,\
  /* duration2_sum_   */  0 ,\
  /* min_current_ph1_ */  NOT_AVAILABLE_MIN_CURRENT_PH1_,\
  /* max_current_ph1_ */  NOT_AVAILABLE_MAX_CURRENT_PH1_,\
  /* min_current_ph2_ */  NOT_AVAILABLE_MIN_CURRENT_PH2_,\
  /* max_current_ph2_ */  NOT_AVAILABLE_MAX_CURRENT_PH2_,\
  /* applied_pulses_  */  NOT_AVAILABLE_APPLIED_PULSES_ ,\
  /* resistance_      */  NOT_AVAILABLE_RESISTANCE_     ,\
  /* current_         */  {0},\
  /* voltage_         */  {0}\
  }
///THE STIMULATION VALUES IN ABOVE SECTION MUST NOT BE CHANGED BY ANYBODY!!!
#else
#define DEVICE_DEFAULTS {\
  /* serial_          */ {"\x0"},\
  /* identifier_      */ {"\x0"},\
  /* HW_version_      */ {"\x0"},\
  /* FW_version_      */ {"\x0"},\
  /* coeff_cs1        */ {1000,0},\
  /* coeff_cs2        */ {1000,0},\
  /* coeff_i          */ {1000,0},\
  /* coeff_v          */ {1000,0},\
  /* coeff_r          */ {1000,0},\
  /* voltage_limit_  */  G_ES_VOLTAGE_LIMIT,\
  /* alternate_       */  GALTERNATE_NO,\
  /* phase_current1_  */  0,\
  /* phase_current2_  */  0,\
  /* phase_duration1_ */  5,\
  /* phase_duration2_ */  5,\
  /* fade_in_time_    */  0,\
  /* fade_out_time_   */  0,\
  /* fade_type_       */  GFADE_PHASE,\
  /* interphase_      */  G_ES_MIN_INTERPHASE_DURATION,\
  /* pulse_rate_       */  G_ES_MIN_PULSE_RATE_FES,\
  /* pulses_          */  G_ES_MIN_PULSES_FES,\
  /* do1_pre_time_    */  G_ES_MIN_DO1_PRE_TIME,\
  /* do1_post_time_   */  G_ES_MIN_DO1_POST_TIME,\
  /* do2_pre_time_    */  G_ES_MIN_DO2_PRE_TIME,\
  /* do2_post_time_   */  G_ES_MIN_DO2_POST_TIME,\
  /* do1_function_    */  GDO_TRAIN,\
  /* do2_function_    */  GDO_TRAIN,\
  /* dio_enabled_     */  0,\
  /* highvoltage_led_    */  0,\
  /* o_function_      */  GOFUNCTION_TEST,\
  /* state_           */  GSTATE_STOP,\
  /* total_charge_    */  0,\
  /* event_           */  GEVENT_START,\
  /* error_           */  GERROR_SUCCESS,\
  /* stim_event_      */  GEVENT_START,\
  /* battery1_        */  G_ES_MIN_BATTERY,\
  /* battery2_        */  G_ES_MIN_BATTERY,\
  /* di_state_        */  0,\
  /* fade_progress_   */  0,\
  /* current1_sum_    */  NOT_AVAILABLE_CURRENT1_SUM_   ,\
  /* current2_sum_    */  NOT_AVAILABLE_CURRENT2_SUM_   ,\
  /* voltage1_sum_    */  NOT_AVAILABLE_VOLTAGE1_SUM_   ,\
  /* voltage2_sum_    */  NOT_AVAILABLE_VOLTAGE2_SUM_   ,\
  /* duration1_sum_   */  0 ,\
  /* duration2_sum_   */  0 ,\
  /* min_current_ph1_ */  NOT_AVAILABLE_MIN_CURRENT_PH1_,\
  /* max_current_ph1_ */  NOT_AVAILABLE_MAX_CURRENT_PH1_,\
  /* min_current_ph2_ */  NOT_AVAILABLE_MIN_CURRENT_PH2_,\
  /* max_current_ph2_ */  NOT_AVAILABLE_MAX_CURRENT_PH2_,\
  /* applied_pulses_  */  NOT_AVAILABLE_APPLIED_PULSES_ ,\
  /* resistance_      */  NOT_AVAILABLE_RESISTANCE_     ,\
  /* current_         */  {0},\
  /* voltage_         */  {0}\
  }
#endif

/// This function does the same as error_state_change()
uint16_t validStateChange(GState new_state, GDevice *d);

/** Checks whether the GDevice field produces any inconsistencies with the limits or other fields.

This function allows to see whether the field is OK, before sending the field to the device.

\return GERROR_SUCCESS or an appropriate \ref GError code.
*/
GError error_device_field(
    volatile GDevice *d, ///< device handle
    uint16_t ofs///< offset given by RW value or rwcode()
    );
/** Checks whether the next impulse would produces a charge limit error.

This depends on the charge of the next train, i.e. on the pulse settings.

\return GERROR_SUCCESS or GERROR_CHARGELIMIT
*/
GError error_chargelimit(
    volatile GDevice *d ///< device handle
    );
/** \brief Checks if a write command can be accepted (according to SRS)

\return GError constant
*/
uint16_t error_write (
     volatile GDevice *d,///< pointer to device configuration
     uint16_t command,   ///< a GCommand value
     uint16_t offset,    ///< the offset of the field using an RW value or rwcode()
     uint16_t length     ///< the length in bytes to read or write
     );
        
#define abs(x) ((x)<0 ? -(x) : (x)) ///< abs() function

#define PING_TIME_MS 200 ///< This is the ping (heartbeat) period in which the PC fetches status information from the device.
#define DEVICE_READY_TIMEOUT 800 ///< This is the timeout, after which deviceReady() returns false.

#define GESTIM_MODE_COUNT 5    ///< The number of different \ref GExtra::operationmode_ values
#define GESTIM_MODE_DEVICE 0   ///< Limits and defaults are as prescribed by the device. A value of \ref GExtra::operationmode_.
#define GESTIM_MODE_BASIC 1    ///< Limits and defaults for basic mode. A value of \ref GExtra::operationmode_
#define GESTIM_MODE_HANDHELD 2 ///< Limits and defaults for handheld mode. A value of \ref GExtra::operationmode_
#define GESTIM_MODE_ADVANCED 3 ///< Limits and defaults for advanced mode. A value of \ref GExtra::operationmode_
#define GESTIM_MODE_FES 4      ///< Limits and defaults for FES mode. A value of \ref GExtra::operationmode_

#define GESTIM_MODE_MASK 0xF   ///< Mask for bits containing actual \ref GExtra::operationmode_.
#define OPERATION_NO_HEARTBEAT 0x10 ///< An option also stored in \ref GExtra::operationmode_, but beyond \ref GESTIM_MODE_MASK.

#define MIN_TRAIN_RATE 0.001       ///< in Hz
#define MAX_N_TRAINS 1000          ///< maximum number of trains in sequence
#define MIN_N_TRAINS 1             ///< minimum number of trains in sequence
#define MAX_JITTER 100             ///< maximum jitter value
#define MIN_JITTER 0               ///< minimum jitter value

#define G_TEC "g.tec"              ///< The company name as stored in the FTDI chip
#define MAX_DEVICES 16             ///< The api can only connect to #MAX_DEVICES devices simultaneously.
#define MAX_TRAIN_RATE 5           ///< In Hz (limited by \ref PING_TIME_MS)

#define MAX_ERROR_TEXT_LEN 1024    ///< Error texts must not be longer

//this order because certain modes don't allow depth
#define ELECTRODE_CIRCULAR 0 ///< A \ref GExtra::el_type_ value.
#define ELECTRODE_OTHER 1    ///< A \ref GExtra::el_type_ value.
#define ELECTRODE_DEPTH 2    ///< A \ref GExtra::el_type_ value.
#define ELECTRODE_NONE 3     ///< A \ref GExtra::el_type_ value.

#define MAX_CHARGE_DENSITY 375   //!< in uC/cm²
#define MAX_CHARGE_DENSITY_FES 2000 //!< in uC/cm²

#endif //GESTIMDEFAULTS_H

// Import definitions for gEstimAPI library
// generated Fri Oct 12 11:14:09 2018 by DylibTool
#ifdef STATIC_LIBGESTIMAPI

namespace Dylib { bool gEstimAPI_Loaded() { return true; } }


#else // STATIC_LIBGESTIMAPI

#include "gEstimAPI.imports.h"
#include "DylibImports.h"
namespace { extern const Dylib::Import* functionImports; }

// Here you may specify a custom error message to be displayed when the library cannot be found.
static const char* notFoundMsg = "";
// Here you may specify an URL to some local or remote help resource.
static const char* notFoundURL = "";
RegisterDylibWithAliases( gEstimAPI, "gEstimAPI", functionImports, notFoundMsg, notFoundURL );

decltype(&__072d7979_abort_sequence) abort_sequence = 0;
decltype(&__1702000c_activate) activate = 0;
decltype(&__64210d1d_calibrate2) calibrate2 = 0;
decltype(&__6f4b6f05_closeDevice) closeDevice = 0;
decltype(&__6f4b6f76_closeDevices) closeDevices = 0;
decltype(&__44466a00_close_disconnect_sm) close_disconnect_sm = 0;
decltype(&__3f1a7359_clr_last_error) clr_last_error = 0;
decltype(&__77373d1e_count_openDevices) count_openDevices = 0;
decltype(&__03290d12_debugLog) debugLog = 0;
decltype(&__66645d0c_deviceReady) deviceReady = 0;
decltype(&__72417006_error_field) error_field = 0;
decltype(&__72417075_error_fields) error_fields = 0;
decltype(&__7254052a_error_state_change) error_state_change = 0;
decltype(&__11283003_fw_hw_ok) fw_hw_ok = 0;
decltype(&__09736049_gt_CrtVersion) gt_CrtVersion = 0;
decltype(&__2c58404c_gt_FW_version) gt_FW_version = 0;
decltype(&__2c584042_gt_HW_version) gt_HW_version = 0;
decltype(&__65614e76_gt_alternate) gt_alternate = 0;
decltype(&__5b704764_gt_alternate_default) gt_alternate_default = 0;
decltype(&__3a0c2f0e_gt_alternate_max) gt_alternate_max = 0;
decltype(&__3a0c2718_gt_alternate_min) gt_alternate_min = 0;
decltype(&__740c1f1d_gt_applied_pulses) gt_applied_pulses = 0;
decltype(&__73010512_gt_applied_trains) gt_applied_trains = 0;
decltype(&__74791a07_gt_battery1) gt_battery1 = 0;
decltype(&__74791907_gt_battery2) gt_battery2 = 0;
decltype(&__17797477_gt_battery_pc) gt_battery_pc = 0;
decltype(&__7d392015_gt_charge_density) gt_charge_density = 0;
decltype(&__57724a34_gt_coeff_cs1) gt_coeff_cs1 = 0;
decltype(&__57724a37_gt_coeff_cs2) gt_coeff_cs2 = 0;
decltype(&__57783905_gt_coeff_i) gt_coeff_i = 0;
decltype(&__57633905_gt_coeff_r) gt_coeff_r = 0;
decltype(&__57673905_gt_coeff_v) gt_coeff_v = 0;
decltype(&__7c722d06_gt_current) gt_current = 0;
decltype(&__1d047b59_gt_current1_avg) gt_current1_avg = 0;
decltype(&__0f077159_gt_current1_sum) gt_current1_sum = 0;
decltype(&__1d047859_gt_current2_avg) gt_current2_avg = 0;
decltype(&__0f077259_gt_current2_sum) gt_current2_sum = 0;
decltype(&__6f5f4910_gt_di_state) gt_di_state = 0;
decltype(&__051e626d_gt_dio_enabled) gt_dio_enabled = 0;
decltype(&__0c0c5c7c_gt_dio_enabled_default) gt_dio_enabled_default = 0;
decltype(&__64663d00_gt_dio_enabled_max) gt_dio_enabled_max = 0;
decltype(&__6c703d00_gt_dio_enabled_min) gt_dio_enabled_min = 0;
decltype(&__14440d76_gt_do1_function) gt_do1_function = 0;
decltype(&__054d1f48_gt_do1_function_default) gt_do1_function_default = 0;
decltype(&__79257529_gt_do1_function_max) gt_do1_function_max = 0;
decltype(&__792d6329_gt_do1_function_min) gt_do1_function_min = 0;
decltype(&__135f192e_gt_do1_post_time) gt_do1_post_time = 0;
decltype(&__2d4e103c_gt_do1_post_time_default) gt_do1_post_time_default = 0;
decltype(&__4c327856_gt_do1_post_time_max) gt_do1_post_time_max = 0;
decltype(&__4c327040_gt_do1_post_time_min) gt_do1_post_time_min = 0;
decltype(&__134d3a60_gt_do1_pre_time) gt_do1_pre_time = 0;
decltype(&__0244285e_gt_do1_pre_time_default) gt_do1_pre_time_default = 0;
decltype(&__7e2c423f_gt_do1_pre_time_max) gt_do1_pre_time_max = 0;
decltype(&__7e24543f_gt_do1_pre_time_min) gt_do1_pre_time_min = 0;
decltype(&__14470d76_gt_do2_function) gt_do2_function = 0;
decltype(&__054e1f48_gt_do2_function_default) gt_do2_function_default = 0;
decltype(&__79267529_gt_do2_function_max) gt_do2_function_max = 0;
decltype(&__792e6329_gt_do2_function_min) gt_do2_function_min = 0;
decltype(&__135c192e_gt_do2_post_time) gt_do2_post_time = 0;
decltype(&__2d4d103c_gt_do2_post_time_default) gt_do2_post_time_default = 0;
decltype(&__4c317856_gt_do2_post_time_max) gt_do2_post_time_max = 0;
decltype(&__4c317040_gt_do2_post_time_min) gt_do2_post_time_min = 0;
decltype(&__134e3a60_gt_do2_pre_time) gt_do2_pre_time = 0;
decltype(&__0247285e_gt_do2_pre_time_default) gt_do2_pre_time_default = 0;
decltype(&__7e2f423f_gt_do2_pre_time_max) gt_do2_pre_time_max = 0;
decltype(&__7e27543f_gt_do2_pre_time_min) gt_do2_pre_time_min = 0;
decltype(&__241a254c_gt_duration1_sum) gt_duration1_sum = 0;
decltype(&__241a254f_gt_duration2_sum) gt_duration2_sum = 0;
decltype(&__715f0516_gt_el_calc_surface) gt_el_calc_surface = 0;
decltype(&__644c7c0d_gt_el_circ_diameter) gt_el_circ_diameter = 0;
decltype(&__75456e33_gt_el_circ_diameter_default) gt_el_circ_diameter_default = 0;
decltype(&__092d0452_gt_el_circ_diameter_max) gt_el_circ_diameter_max = 0;
decltype(&__09251252_gt_el_circ_diameter_min) gt_el_circ_diameter_min = 0;
decltype(&__7a425740_gt_el_depth_diameter) gt_el_depth_diameter = 0;
decltype(&__44535e52_gt_el_depth_diameter_default) gt_el_depth_diameter_default = 0;
decltype(&__252f3638_gt_el_depth_diameter_max) gt_el_depth_diameter_max = 0;
decltype(&__252f3e2e_gt_el_depth_diameter_min) gt_el_depth_diameter_min = 0;
decltype(&__63523d38_gt_el_depth_length) gt_el_depth_length = 0;
decltype(&__6a400329_gt_el_depth_length_default) gt_el_depth_length_default = 0;
decltype(&__022a6255_gt_el_depth_length_max) gt_el_depth_length_max = 0;
decltype(&__0a3c6255_gt_el_depth_length_min) gt_el_depth_length_min = 0;
decltype(&__71585528_gt_el_other_surface) gt_el_other_surface = 0;
decltype(&__60514716_gt_el_other_surface_default) gt_el_other_surface_default = 0;
decltype(&__1c392d77_gt_el_other_surface_max) gt_el_other_surface_max = 0;
decltype(&__1c313b77_gt_el_other_surface_min) gt_el_other_surface_min = 0;
decltype(&__7b4e2b1c_gt_el_type) gt_el_type = 0;
decltype(&__725c150d_gt_el_type_default) gt_el_type_default = 0;
decltype(&__1a367471_gt_el_type_max) gt_el_type_max = 0;
decltype(&__12207471_gt_el_type_min) gt_el_type_min = 0;
decltype(&__15063017_gt_error) gt_error = 0;
decltype(&__3e72556f_gt_error_text) gt_error_text = 0;
decltype(&__11113111_gt_event) gt_event = 0;
decltype(&__0f03594d_gt_fade_factor) gt_fade_factor = 0;
decltype(&__6912162c_gt_fade_in_pulses) gt_fade_in_pulses = 0;
decltype(&__0613004d_gt_fade_in_time) gt_fade_in_time = 0;
decltype(&__171a1273_gt_fade_in_time_default) gt_fade_in_time_default = 0;
decltype(&__6b727812_gt_fade_in_time_max) gt_fade_in_time_max = 0;
decltype(&__6b7a6e12_gt_fade_in_time_min) gt_fade_in_time_min = 0;
decltype(&__7c632215_gt_fade_out_pulses) gt_fade_out_pulses = 0;
decltype(&__1d0c2303_gt_fade_out_time) gt_fade_out_time = 0;
decltype(&__231d2a11_gt_fade_out_time_default) gt_fade_out_time_default = 0;
decltype(&__4261427b_gt_fade_out_time_max) gt_fade_out_time_max = 0;
decltype(&__42614a6d_gt_fade_out_time_min) gt_fade_out_time_min = 0;
decltype(&__0407262d_gt_fade_progress) gt_fade_progress = 0;
decltype(&__72694a5c_gt_fade_type) gt_fade_type = 0;
decltype(&__4c78434e_gt_fade_type_default) gt_fade_type_default = 0;
decltype(&__2d042b24_gt_fade_type_max) gt_fade_type_max = 0;
decltype(&__2d042332_gt_fade_type_min) gt_fade_type_min = 0;
decltype(&__180c7350_gt_ftdi_driver_version) gt_ftdi_driver_version = 0;
decltype(&__01013167_gt_ftdi_library_version) gt_ftdi_library_version = 0;
decltype(&__637e1c13_gt_highvoltage_led) gt_highvoltage_led = 0;
decltype(&__6a6c2202_gt_highvoltage_led_default) gt_highvoltage_led_default = 0;
decltype(&__0206437e_gt_highvoltage_led_max) gt_highvoltage_led_max = 0;
decltype(&__0a10437e_gt_highvoltage_led_min) gt_highvoltage_led_min = 0;
decltype(&__18775878_gt_identifier) gt_identifier = 0;
decltype(&__1c685b68_gt_interphase) gt_interphase = 0;
decltype(&__0e564a61_gt_interphase_default) gt_interphase_default = 0;
decltype(&__64373609_gt_interphase_max) gt_interphase_max = 0;
decltype(&__72373601_gt_interphase_min) gt_interphase_min = 0;
decltype(&__7c002b0f_gt_jitter) gt_jitter = 0;
decltype(&__6e3e3a06_gt_jitter_default) gt_jitter_default = 0;
decltype(&__045f466e_gt_jitter_max) gt_jitter_max = 0;
decltype(&__125f4666_gt_jitter_min) gt_jitter_min = 0;
decltype(&__1175595c_gt_last_error) gt_last_error = 0;
decltype(&__64733934_gt_last_resistance) gt_last_resistance = 0;
decltype(&__753b2d1b_gt_max_current_ph1) gt_max_current_ph1 = 0;
decltype(&__75382d1b_gt_max_current_ph2) gt_max_current_ph2 = 0;
decltype(&__7d2d2d1b_gt_min_current_ph1) gt_min_current_ph1 = 0;
decltype(&__7d2e2d1b_gt_min_current_ph2) gt_min_current_ph2 = 0;
decltype(&__516e5e0f_gt_n_trains) gt_n_trains = 0;
decltype(&__40674c31_gt_n_trains_default) gt_n_trains_default = 0;
decltype(&__3c0f2650_gt_n_trains_max) gt_n_trains_max = 0;
decltype(&__3c073050_gt_n_trains_min) gt_n_trains_min = 0;
decltype(&__3566436e_gt_o_function) gt_o_function = 0;
decltype(&__27585267_gt_o_function_default) gt_o_function_default = 0;
decltype(&__4d392e0f_gt_o_function_max) gt_o_function_max = 0;
decltype(&__5b392e07_gt_o_function_min) gt_o_function_min = 0;
decltype(&__17145848_gt_openDevices) gt_openDevices = 0;
decltype(&__0e172605_gt_operationmode) gt_operationmode = 0;
decltype(&__30062f17_gt_operationmode_default) gt_operationmode_default = 0;
decltype(&__517a477d_gt_operationmode_max) gt_operationmode_max = 0;
decltype(&__517a4f6b_gt_operationmode_min) gt_operationmode_min = 0;
decltype(&__6675361f_gt_periods) gt_periods = 0;
decltype(&__22112174_gt_phase_charge) gt_phase_charge = 0;
decltype(&__13133713_gt_phase_current1) gt_phase_current1 = 0;
decltype(&__012d261a_gt_phase_current1_default) gt_phase_current1_default = 0;
decltype(&__6b4c5a72_gt_phase_current1_max) gt_phase_current1_max = 0;
decltype(&__7d4c5a7a_gt_phase_current1_min) gt_phase_current1_min = 0;
decltype(&__10133713_gt_phase_current2) gt_phase_current2 = 0;
decltype(&__022d261a_gt_phase_current2_default) gt_phase_current2_default = 0;
decltype(&__684c5a72_gt_phase_current2_max) gt_phase_current2_max = 0;
decltype(&__7e4c5a7a_gt_phase_current2_min) gt_phase_current2_min = 0;
decltype(&__5f343008_gt_phase_duration1) gt_phase_duration1 = 0;
decltype(&__56260e19_gt_phase_duration1_default) gt_phase_duration1_default = 0;
decltype(&__3e4c6f65_gt_phase_duration1_max) gt_phase_duration1_max = 0;
decltype(&__365a6f65_gt_phase_duration1_min) gt_phase_duration1_min = 0;
decltype(&__5f373008_gt_phase_duration2) gt_phase_duration2 = 0;
decltype(&__56250e19_gt_phase_duration2_default) gt_phase_duration2_default = 0;
decltype(&__3e4f6f65_gt_phase_duration2_max) gt_phase_duration2_max = 0;
decltype(&__36596f65_gt_phase_duration2_min) gt_phase_duration2_min = 0;
decltype(&__42083008_gt_pulse_duration) gt_pulse_duration = 0;
decltype(&__24072d67_gt_pulse_period) gt_pulse_period = 0;
decltype(&__286a4d61_gt_pulse_rate) gt_pulse_rate = 0;
decltype(&__3a545c68_gt_pulse_rate_default) gt_pulse_rate_default = 0;
decltype(&__50352000_gt_pulse_rate_max) gt_pulse_rate_max = 0;
decltype(&__46352008_gt_pulse_rate_min) gt_pulse_rate_min = 0;
decltype(&__61182c15_gt_pulses) gt_pulses = 0;
decltype(&__73263d1c_gt_pulses_default) gt_pulses_default = 0;
decltype(&__19474174_gt_pulses_max) gt_pulses_max = 0;
decltype(&__0f47417c_gt_pulses_min) gt_pulses_min = 0;
decltype(&__6c70507c_gt_remaining) gt_remaining = 0;
decltype(&__13665862_gt_resistance) gt_resistance = 0;
decltype(&__726e3e2b_gt_selftest_passed) gt_selftest_passed = 0;
decltype(&__7c7a5246_gt_sequence_duration) gt_sequence_duration = 0;
decltype(&__6e063612_gt_serial) gt_serial = 0;
decltype(&__6f2f0919_gt_sessionLogFile) gt_sessionLogFile = 0;
decltype(&__13152b16_gt_state) gt_state = 0;
decltype(&__026b5742_gt_stim_event) gt_stim_event = 0;
decltype(&__6073392c_gt_test_resistance) gt_test_resistance = 0;
decltype(&__25043379_gt_total_charge) gt_total_charge = 0;
decltype(&__340d2147_gt_total_charge_default) gt_total_charge_default = 0;
decltype(&__48654b26_gt_total_charge_max) gt_total_charge_max = 0;
decltype(&__486d5d26_gt_total_charge_min) gt_total_charge_min = 0;
decltype(&__38113b7b_gt_train_charge) gt_train_charge = 0;
decltype(&__45052a07_gt_train_duration) gt_train_duration = 0;
decltype(&__230a3768_gt_train_period) gt_train_period = 0;
decltype(&__2f67576e_gt_train_rate) gt_train_rate = 0;
decltype(&__3d594667_gt_train_rate_default) gt_train_rate_default = 0;
decltype(&__57383a0f_gt_train_rate_max) gt_train_rate_max = 0;
decltype(&__41383a07_gt_train_rate_min) gt_train_rate_min = 0;
decltype(&__6f7d2b17_gt_voltage) gt_voltage = 0;
decltype(&__0e0b7d48_gt_voltage1_avg) gt_voltage1_avg = 0;
decltype(&__1c087748_gt_voltage1_sum) gt_voltage1_sum = 0;
decltype(&__0e0b7e48_gt_voltage2_avg) gt_voltage2_avg = 0;
decltype(&__1c087448_gt_voltage2_sum) gt_voltage2_sum = 0;
decltype(&__06101d0f_gt_voltage_limit) gt_voltage_limit = 0;
decltype(&__3801141d_gt_voltage_limit_default) gt_voltage_limit_default = 0;
decltype(&__597d7c77_gt_voltage_limit_max) gt_voltage_limit_max = 0;
decltype(&__597d7461_gt_voltage_limit_min) gt_voltage_limit_min = 0;
decltype(&__606b1d38_has_dongle) has_dongle = 0;
decltype(&__7a746870_isOpenDevice) isOpenDevice = 0;
decltype(&__25526954_is_FES_serial) is_FES_serial = 0;
decltype(&__7a63162b_measure_resistance) measure_resistance = 0;
decltype(&__03784855_newSessionLog) newSessionLog = 0;
decltype(&__48701307_openDevice) openDevice = 0;
decltype(&__48706007_openDevices) openDevices = 0;
decltype(&__01050474_open_connect_sm) open_connect_sm = 0;
decltype(&__3e285a40_rcv_FW_version) rcv_FW_version = 0;
decltype(&__30285a40_rcv_HW_version) rcv_HW_version = 0;
decltype(&__130f1a5f_rcv_all) rcv_all = 0;
decltype(&__0461634e_rcv_alternate) rcv_alternate = 0;
decltype(&__6f700e1f_rcv_applied_pulses) rcv_applied_pulses = 0;
decltype(&__75707b1a_rcv_battery1) rcv_battery1 = 0;
decltype(&__75707b19_rcv_battery2) rcv_battery2 = 0;
decltype(&__4653704a_rcv_coeff_cs1) rcv_coeff_cs1 = 0;
decltype(&__4553704a_rcv_coeff_cs2) rcv_coeff_cs2 = 0;
decltype(&__77537a39_rcv_coeff_i) rcv_coeff_i = 0;
decltype(&__77536139_rcv_coeff_r) rcv_coeff_r = 0;
decltype(&__77536539_rcv_coeff_v) rcv_coeff_v = 0;
decltype(&__7478702d_rcv_current) rcv_current = 0;
decltype(&__2b0b0571_rcv_current1_sum) rcv_current1_sum = 0;
decltype(&__2b0b0572_rcv_current2_sum) rcv_current2_sum = 0;
decltype(&__626b5d49_rcv_di_state) rcv_di_state = 0;
decltype(&__1f011c62_rcv_dio_enabled) rcv_dio_enabled = 0;
decltype(&__0410460d_rcv_do1_function) rcv_do1_function = 0;
decltype(&__5c175d19_rcv_do1_post_time) rcv_do1_post_time = 0;
decltype(&__12174f3a_rcv_do1_pre_time) rcv_do1_pre_time = 0;
decltype(&__0410450d_rcv_do2_function) rcv_do2_function = 0;
decltype(&__5c175e19_rcv_do2_post_time) rcv_do2_post_time = 0;
decltype(&__12174c3a_rcv_do2_pre_time) rcv_do2_pre_time = 0;
decltype(&__3e201825_rcv_duration1_sum) rcv_duration1_sum = 0;
decltype(&__3d201825_rcv_duration2_sum) rcv_duration2_sum = 0;
decltype(&__65110430_rcv_error) rcv_error = 0;
decltype(&__63151331_rcv_event) rcv_event = 0;
decltype(&__3f021100_rcv_fade_in_time) rcv_fade_in_time = 0;
decltype(&__71190e23_rcv_fade_out_time) rcv_fade_out_time = 0;
decltype(&__5f000526_rcv_fade_progress) rcv_fade_progress = 0;
decltype(&__2e766b4a_rcv_fade_type) rcv_fade_type = 0;
decltype(&__61677c1c_rcv_highvoltage_led) rcv_highvoltage_led = 0;
decltype(&__0a1c7558_rcv_identifier) rcv_identifier = 0;
decltype(&__1a186a5b_rcv_interphase) rcv_interphase = 0;
decltype(&__6971392d_rcv_max_current_ph1) rcv_max_current_ph1 = 0;
decltype(&__69713a2d_rcv_max_current_ph2) rcv_max_current_ph2 = 0;
decltype(&__69792f2d_rcv_min_current_ph1) rcv_min_current_ph1 = 0;
decltype(&__69792c2d_rcv_min_current_ph2) rcv_min_current_ph2 = 0;
decltype(&__1c316443_rcv_o_function) rcv_o_function = 0;
decltype(&__61171137_rcv_phase_current1) rcv_phase_current1 = 0;
decltype(&__61141137_rcv_phase_current2) rcv_phase_current2 = 0;
decltype(&__7a5b3630_rcv_phase_duration1) rcv_phase_duration1 = 0;
decltype(&__7a5b3530_rcv_phase_duration2) rcv_phase_duration2 = 0;
decltype(&__132c684d_rcv_pulse_rate) rcv_pulse_rate = 0;
decltype(&__67651a2c_rcv_pulses) rcv_pulses = 0;
decltype(&__10176458_rcv_resistance) rcv_resistance = 0;
decltype(&__6d67682f_rcv_samples) rcv_samples = 0;
decltype(&__606a0436_rcv_serial) rcv_serial = 0;
decltype(&__6417172b_rcv_state) rcv_state = 0;
decltype(&__01171f32_rcv_stim) rcv_stim = 0;
decltype(&__30066957_rcv_stim_event) rcv_stim_event = 0;
decltype(&__0b210633_rcv_total_charge) rcv_total_charge = 0;
decltype(&__656b7f2b_rcv_voltage) rcv_voltage = 0;
decltype(&__3a180a77_rcv_voltage1_sum) rcv_voltage1_sum = 0;
decltype(&__3a180a74_rcv_voltage2_sum) rcv_voltage2_sum = 0;
decltype(&__7d02121d_rcv_voltage_limit) rcv_voltage_limit = 0;
decltype(&__3d55726e_register_with_polling) register_with_polling = 0;
decltype(&__17160575_request) request = 0;
decltype(&__76560a27_reset_total_charge) reset_total_charge = 0;
decltype(&__07001f12_selftest) selftest = 0;
decltype(&__27100919_sequence_running) sequence_running = 0;
decltype(&__756d1d3f_sessionLog) sessionLog = 0;
decltype(&__1f0a2344_set_socket_port_for_sim) set_socket_port_for_sim = 0;
decltype(&__3f254840_snd_FW_version) snd_FW_version = 0;
decltype(&__31254840_snd_HW_version) snd_HW_version = 0;
decltype(&__056c714e_snd_alternate) snd_alternate = 0;
decltype(&__475e624a_snd_coeff_cs1) snd_coeff_cs1 = 0;
decltype(&__445e624a_snd_coeff_cs2) snd_coeff_cs2 = 0;
decltype(&__765e6839_snd_coeff_i) snd_coeff_i = 0;
decltype(&__765e7339_snd_coeff_r) snd_coeff_r = 0;
decltype(&__765e7739_snd_coeff_v) snd_coeff_v = 0;
decltype(&__0625726e_snd_current_n_duration) snd_current_n_duration = 0;
decltype(&__1e0c0e62_snd_dio_enabled) snd_dio_enabled = 0;
decltype(&__051d540d_snd_do1_function) snd_do1_function = 0;
decltype(&__5d1a4f19_snd_do1_post_time) snd_do1_post_time = 0;
decltype(&__131a5d3a_snd_do1_pre_time) snd_do1_pre_time = 0;
decltype(&__051d570d_snd_do2_function) snd_do2_function = 0;
decltype(&__5d1a4c19_snd_do2_post_time) snd_do2_post_time = 0;
decltype(&__131a5e3a_snd_do2_pre_time) snd_do2_pre_time = 0;
decltype(&__3e0f0300_snd_fade_in_time) snd_fade_in_time = 0;
decltype(&__70141c23_snd_fade_out_time) snd_fade_out_time = 0;
decltype(&__2f7b794a_snd_fade_type) snd_fade_type = 0;
decltype(&__606a6e1c_snd_highvoltage_led) snd_highvoltage_led = 0;
decltype(&__0b116758_snd_identifier) snd_identifier = 0;
decltype(&__1b15785b_snd_interphase) snd_interphase = 0;
decltype(&__1d3c7643_snd_o_function) snd_o_function = 0;
decltype(&__601a0337_snd_phase_current1) snd_phase_current1 = 0;
decltype(&__60190337_snd_phase_current2) snd_phase_current2 = 0;
decltype(&__7b562430_snd_phase_duration1) snd_phase_duration1 = 0;
decltype(&__7b562730_snd_phase_duration2) snd_phase_duration2 = 0;
decltype(&__12217a4d_snd_pulse_rate) snd_pulse_rate = 0;
decltype(&__6668082c_snd_pulses) snd_pulses = 0;
decltype(&__61671636_snd_serial) snd_serial = 0;
decltype(&__651a052b_snd_state) snd_state = 0;
decltype(&__0a2c1433_snd_total_charge) snd_total_charge = 0;
decltype(&__7c0f001d_snd_voltage_limit) snd_voltage_limit = 0;
decltype(&__3858404c_st_FW_version) st_FW_version = 0;
decltype(&__38584042_st_HW_version) st_HW_version = 0;
decltype(&__71614e76_st_alternate) st_alternate = 0;
decltype(&__43724a34_st_coeff_cs1) st_coeff_cs1 = 0;
decltype(&__43724a37_st_coeff_cs2) st_coeff_cs2 = 0;
decltype(&__43783905_st_coeff_i) st_coeff_i = 0;
decltype(&__43633905_st_coeff_r) st_coeff_r = 0;
decltype(&__43673905_st_coeff_v) st_coeff_v = 0;
decltype(&__7a663e11_st_default) st_default = 0;
decltype(&__7a664d11_st_defaults) st_defaults = 0;
decltype(&__111e626d_st_dio_enabled) st_dio_enabled = 0;
decltype(&__00440d76_st_do1_function) st_do1_function = 0;
decltype(&__075f192e_st_do1_post_time) st_do1_post_time = 0;
decltype(&__074d3a60_st_do1_pre_time) st_do1_pre_time = 0;
decltype(&__00470d76_st_do2_function) st_do2_function = 0;
decltype(&__075c192e_st_do2_post_time) st_do2_post_time = 0;
decltype(&__074e3a60_st_do2_pre_time) st_do2_pre_time = 0;
decltype(&__704c7c0d_st_el_circ_diameter) st_el_circ_diameter = 0;
decltype(&__6e425740_st_el_depth_diameter) st_el_depth_diameter = 0;
decltype(&__77523d38_st_el_depth_length) st_el_depth_length = 0;
decltype(&__65585528_st_el_other_surface) st_el_other_surface = 0;
decltype(&__6f4e2b1c_st_el_type) st_el_type = 0;
decltype(&__1213004d_st_fade_in_time) st_fade_in_time = 0;
decltype(&__090c2303_st_fade_out_time) st_fade_out_time = 0;
decltype(&__66694a5c_st_fade_type) st_fade_type = 0;
decltype(&__777e1c13_st_highvoltage_led) st_highvoltage_led = 0;
decltype(&__0c775878_st_identifier) st_identifier = 0;
decltype(&__08685b68_st_interphase) st_interphase = 0;
decltype(&__68002b0f_st_jitter) st_jitter = 0;
decltype(&__67613600_st_maximum) st_maximum = 0;
decltype(&__6f773600_st_minimum) st_minimum = 0;
decltype(&__456e5e0f_st_n_trains) st_n_trains = 0;
decltype(&__2166436e_st_o_function) st_o_function = 0;
decltype(&__1a172605_st_operationmode) st_operationmode = 0;
decltype(&__07133713_st_phase_current1) st_phase_current1 = 0;
decltype(&__04133713_st_phase_current2) st_phase_current2 = 0;
decltype(&__4b343008_st_phase_duration1) st_phase_duration1 = 0;
decltype(&__4b373008_st_phase_duration2) st_phase_duration2 = 0;
decltype(&__30072d67_st_pulse_period) st_pulse_period = 0;
decltype(&__3c6a4d61_st_pulse_rate) st_pulse_rate = 0;
decltype(&__75182c15_st_pulses) st_pulses = 0;
decltype(&__382e3118_st_sequence_duration_by_n_trains) st_sequence_duration_by_n_trains = 0;
decltype(&__59503811_st_sequence_duration_by_train_rate) st_sequence_duration_by_train_rate = 0;
decltype(&__7a063612_st_serial) st_serial = 0;
decltype(&__07152b16_st_state) st_state = 0;
decltype(&__31043379_st_total_charge) st_total_charge = 0;
decltype(&__1c3b0760_st_train_duration_by_pulse_rate) st_train_duration_by_pulse_rate = 0;
decltype(&__7d4f4e12_st_train_duration_by_pulses) st_train_duration_by_pulses = 0;
decltype(&__370a3768_st_train_period) st_train_period = 0;
decltype(&__3b67576e_st_train_rate) st_train_rate = 0;
decltype(&__12101d0f_st_voltage_limit) st_voltage_limit = 0;
decltype(&__153b7779_start_sequence) start_sequence = 0;
decltype(&__73746f70_stop) stop = 0;
decltype(&__11072a64_toggle_debuglog) toggle_debuglog = 0;
decltype(&__07003d55_unregister_with_polling) unregister_with_polling = 0;
decltype(&__3e061c2d_wait_for_end_of_sequence) wait_for_end_of_sequence = 0;


namespace {
const Dylib::Import functionImports_[] =
{
  { "abort_sequence", (void**)&abort_sequence, Dylib::Import::cMangled },
  { "activate", (void**)&activate, Dylib::Import::cMangled },
  { "calibrate2", (void**)&calibrate2, Dylib::Import::cMangled },
  { "closeDevice", (void**)&closeDevice, Dylib::Import::cMangled },
  { "closeDevices", (void**)&closeDevices, Dylib::Import::cMangled },
  { "close_disconnect_sm", (void**)&close_disconnect_sm, Dylib::Import::cMangled },
  { "clr_last_error", (void**)&clr_last_error, Dylib::Import::cMangled },
  { "count_openDevices", (void**)&count_openDevices, Dylib::Import::cMangled },
  { "debugLog", (void**)&debugLog, Dylib::Import::cMangled },
  { "deviceReady", (void**)&deviceReady, Dylib::Import::cMangled },
  { "error_field", (void**)&error_field, Dylib::Import::cMangled },
  { "error_fields", (void**)&error_fields, Dylib::Import::cMangled },
  { "error_state_change", (void**)&error_state_change, Dylib::Import::cMangled },
  { "fw_hw_ok", (void**)&fw_hw_ok, Dylib::Import::cMangled },
  { "gt_CrtVersion", (void**)&gt_CrtVersion, Dylib::Import::cMangled },
  { "gt_FW_version", (void**)&gt_FW_version, Dylib::Import::cMangled },
  { "gt_HW_version", (void**)&gt_HW_version, Dylib::Import::cMangled },
  { "gt_alternate", (void**)&gt_alternate, Dylib::Import::cMangled },
  { "gt_alternate_default", (void**)&gt_alternate_default, Dylib::Import::cMangled },
  { "gt_alternate_max", (void**)&gt_alternate_max, Dylib::Import::cMangled },
  { "gt_alternate_min", (void**)&gt_alternate_min, Dylib::Import::cMangled },
  { "gt_applied_pulses", (void**)&gt_applied_pulses, Dylib::Import::cMangled },
  { "gt_applied_trains", (void**)&gt_applied_trains, Dylib::Import::cMangled },
  { "gt_battery1", (void**)&gt_battery1, Dylib::Import::cMangled },
  { "gt_battery2", (void**)&gt_battery2, Dylib::Import::cMangled },
  { "gt_battery_pc", (void**)&gt_battery_pc, Dylib::Import::cMangled },
  { "gt_charge_density", (void**)&gt_charge_density, Dylib::Import::cMangled },
  { "gt_coeff_cs1", (void**)&gt_coeff_cs1, Dylib::Import::cMangled },
  { "gt_coeff_cs2", (void**)&gt_coeff_cs2, Dylib::Import::cMangled },
  { "gt_coeff_i", (void**)&gt_coeff_i, Dylib::Import::cMangled },
  { "gt_coeff_r", (void**)&gt_coeff_r, Dylib::Import::cMangled },
  { "gt_coeff_v", (void**)&gt_coeff_v, Dylib::Import::cMangled },
  { "gt_current", (void**)&gt_current, Dylib::Import::cMangled },
  { "gt_current1_avg", (void**)&gt_current1_avg, Dylib::Import::cMangled },
  { "gt_current1_sum", (void**)&gt_current1_sum, Dylib::Import::cMangled },
  { "gt_current2_avg", (void**)&gt_current2_avg, Dylib::Import::cMangled },
  { "gt_current2_sum", (void**)&gt_current2_sum, Dylib::Import::cMangled },
  { "gt_di_state", (void**)&gt_di_state, Dylib::Import::cMangled },
  { "gt_dio_enabled", (void**)&gt_dio_enabled, Dylib::Import::cMangled },
  { "gt_dio_enabled_default", (void**)&gt_dio_enabled_default, Dylib::Import::cMangled },
  { "gt_dio_enabled_max", (void**)&gt_dio_enabled_max, Dylib::Import::cMangled },
  { "gt_dio_enabled_min", (void**)&gt_dio_enabled_min, Dylib::Import::cMangled },
  { "gt_do1_function", (void**)&gt_do1_function, Dylib::Import::cMangled },
  { "gt_do1_function_default", (void**)&gt_do1_function_default, Dylib::Import::cMangled },
  { "gt_do1_function_max", (void**)&gt_do1_function_max, Dylib::Import::cMangled },
  { "gt_do1_function_min", (void**)&gt_do1_function_min, Dylib::Import::cMangled },
  { "gt_do1_post_time", (void**)&gt_do1_post_time, Dylib::Import::cMangled },
  { "gt_do1_post_time_default", (void**)&gt_do1_post_time_default, Dylib::Import::cMangled },
  { "gt_do1_post_time_max", (void**)&gt_do1_post_time_max, Dylib::Import::cMangled },
  { "gt_do1_post_time_min", (void**)&gt_do1_post_time_min, Dylib::Import::cMangled },
  { "gt_do1_pre_time", (void**)&gt_do1_pre_time, Dylib::Import::cMangled },
  { "gt_do1_pre_time_default", (void**)&gt_do1_pre_time_default, Dylib::Import::cMangled },
  { "gt_do1_pre_time_max", (void**)&gt_do1_pre_time_max, Dylib::Import::cMangled },
  { "gt_do1_pre_time_min", (void**)&gt_do1_pre_time_min, Dylib::Import::cMangled },
  { "gt_do2_function", (void**)&gt_do2_function, Dylib::Import::cMangled },
  { "gt_do2_function_default", (void**)&gt_do2_function_default, Dylib::Import::cMangled },
  { "gt_do2_function_max", (void**)&gt_do2_function_max, Dylib::Import::cMangled },
  { "gt_do2_function_min", (void**)&gt_do2_function_min, Dylib::Import::cMangled },
  { "gt_do2_post_time", (void**)&gt_do2_post_time, Dylib::Import::cMangled },
  { "gt_do2_post_time_default", (void**)&gt_do2_post_time_default, Dylib::Import::cMangled },
  { "gt_do2_post_time_max", (void**)&gt_do2_post_time_max, Dylib::Import::cMangled },
  { "gt_do2_post_time_min", (void**)&gt_do2_post_time_min, Dylib::Import::cMangled },
  { "gt_do2_pre_time", (void**)&gt_do2_pre_time, Dylib::Import::cMangled },
  { "gt_do2_pre_time_default", (void**)&gt_do2_pre_time_default, Dylib::Import::cMangled },
  { "gt_do2_pre_time_max", (void**)&gt_do2_pre_time_max, Dylib::Import::cMangled },
  { "gt_do2_pre_time_min", (void**)&gt_do2_pre_time_min, Dylib::Import::cMangled },
  { "gt_duration1_sum", (void**)&gt_duration1_sum, Dylib::Import::cMangled },
  { "gt_duration2_sum", (void**)&gt_duration2_sum, Dylib::Import::cMangled },
  { "gt_el_calc_surface", (void**)&gt_el_calc_surface, Dylib::Import::cMangled },
  { "gt_el_circ_diameter", (void**)&gt_el_circ_diameter, Dylib::Import::cMangled },
  { "gt_el_circ_diameter_default", (void**)&gt_el_circ_diameter_default, Dylib::Import::cMangled },
  { "gt_el_circ_diameter_max", (void**)&gt_el_circ_diameter_max, Dylib::Import::cMangled },
  { "gt_el_circ_diameter_min", (void**)&gt_el_circ_diameter_min, Dylib::Import::cMangled },
  { "gt_el_depth_diameter", (void**)&gt_el_depth_diameter, Dylib::Import::cMangled },
  { "gt_el_depth_diameter_default", (void**)&gt_el_depth_diameter_default, Dylib::Import::cMangled },
  { "gt_el_depth_diameter_max", (void**)&gt_el_depth_diameter_max, Dylib::Import::cMangled },
  { "gt_el_depth_diameter_min", (void**)&gt_el_depth_diameter_min, Dylib::Import::cMangled },
  { "gt_el_depth_length", (void**)&gt_el_depth_length, Dylib::Import::cMangled },
  { "gt_el_depth_length_default", (void**)&gt_el_depth_length_default, Dylib::Import::cMangled },
  { "gt_el_depth_length_max", (void**)&gt_el_depth_length_max, Dylib::Import::cMangled },
  { "gt_el_depth_length_min", (void**)&gt_el_depth_length_min, Dylib::Import::cMangled },
  { "gt_el_other_surface", (void**)&gt_el_other_surface, Dylib::Import::cMangled },
  { "gt_el_other_surface_default", (void**)&gt_el_other_surface_default, Dylib::Import::cMangled },
  { "gt_el_other_surface_max", (void**)&gt_el_other_surface_max, Dylib::Import::cMangled },
  { "gt_el_other_surface_min", (void**)&gt_el_other_surface_min, Dylib::Import::cMangled },
  { "gt_el_type", (void**)&gt_el_type, Dylib::Import::cMangled },
  { "gt_el_type_default", (void**)&gt_el_type_default, Dylib::Import::cMangled },
  { "gt_el_type_max", (void**)&gt_el_type_max, Dylib::Import::cMangled },
  { "gt_el_type_min", (void**)&gt_el_type_min, Dylib::Import::cMangled },
  { "gt_error", (void**)&gt_error, Dylib::Import::cMangled },
  { "gt_error_text", (void**)&gt_error_text, Dylib::Import::cMangled },
  { "gt_event", (void**)&gt_event, Dylib::Import::cMangled },
  { "gt_fade_factor", (void**)&gt_fade_factor, Dylib::Import::cMangled },
  { "gt_fade_in_pulses", (void**)&gt_fade_in_pulses, Dylib::Import::cMangled },
  { "gt_fade_in_time", (void**)&gt_fade_in_time, Dylib::Import::cMangled },
  { "gt_fade_in_time_default", (void**)&gt_fade_in_time_default, Dylib::Import::cMangled },
  { "gt_fade_in_time_max", (void**)&gt_fade_in_time_max, Dylib::Import::cMangled },
  { "gt_fade_in_time_min", (void**)&gt_fade_in_time_min, Dylib::Import::cMangled },
  { "gt_fade_out_pulses", (void**)&gt_fade_out_pulses, Dylib::Import::cMangled },
  { "gt_fade_out_time", (void**)&gt_fade_out_time, Dylib::Import::cMangled },
  { "gt_fade_out_time_default", (void**)&gt_fade_out_time_default, Dylib::Import::cMangled },
  { "gt_fade_out_time_max", (void**)&gt_fade_out_time_max, Dylib::Import::cMangled },
  { "gt_fade_out_time_min", (void**)&gt_fade_out_time_min, Dylib::Import::cMangled },
  { "gt_fade_progress", (void**)&gt_fade_progress, Dylib::Import::cMangled },
  { "gt_fade_type", (void**)&gt_fade_type, Dylib::Import::cMangled },
  { "gt_fade_type_default", (void**)&gt_fade_type_default, Dylib::Import::cMangled },
  { "gt_fade_type_max", (void**)&gt_fade_type_max, Dylib::Import::cMangled },
  { "gt_fade_type_min", (void**)&gt_fade_type_min, Dylib::Import::cMangled },
  { "gt_ftdi_driver_version", (void**)&gt_ftdi_driver_version, Dylib::Import::cMangled },
  { "gt_ftdi_library_version", (void**)&gt_ftdi_library_version, Dylib::Import::cMangled },
  { "gt_highvoltage_led", (void**)&gt_highvoltage_led, Dylib::Import::cMangled },
  { "gt_highvoltage_led_default", (void**)&gt_highvoltage_led_default, Dylib::Import::cMangled },
  { "gt_highvoltage_led_max", (void**)&gt_highvoltage_led_max, Dylib::Import::cMangled },
  { "gt_highvoltage_led_min", (void**)&gt_highvoltage_led_min, Dylib::Import::cMangled },
  { "gt_identifier", (void**)&gt_identifier, Dylib::Import::cMangled },
  { "gt_interphase", (void**)&gt_interphase, Dylib::Import::cMangled },
  { "gt_interphase_default", (void**)&gt_interphase_default, Dylib::Import::cMangled },
  { "gt_interphase_max", (void**)&gt_interphase_max, Dylib::Import::cMangled },
  { "gt_interphase_min", (void**)&gt_interphase_min, Dylib::Import::cMangled },
  { "gt_jitter", (void**)&gt_jitter, Dylib::Import::cMangled },
  { "gt_jitter_default", (void**)&gt_jitter_default, Dylib::Import::cMangled },
  { "gt_jitter_max", (void**)&gt_jitter_max, Dylib::Import::cMangled },
  { "gt_jitter_min", (void**)&gt_jitter_min, Dylib::Import::cMangled },
  { "gt_last_error", (void**)&gt_last_error, Dylib::Import::cMangled },
  { "gt_last_resistance", (void**)&gt_last_resistance, Dylib::Import::cMangled },
  { "gt_max_current_ph1", (void**)&gt_max_current_ph1, Dylib::Import::cMangled },
  { "gt_max_current_ph2", (void**)&gt_max_current_ph2, Dylib::Import::cMangled },
  { "gt_min_current_ph1", (void**)&gt_min_current_ph1, Dylib::Import::cMangled },
  { "gt_min_current_ph2", (void**)&gt_min_current_ph2, Dylib::Import::cMangled },
  { "gt_n_trains", (void**)&gt_n_trains, Dylib::Import::cMangled },
  { "gt_n_trains_default", (void**)&gt_n_trains_default, Dylib::Import::cMangled },
  { "gt_n_trains_max", (void**)&gt_n_trains_max, Dylib::Import::cMangled },
  { "gt_n_trains_min", (void**)&gt_n_trains_min, Dylib::Import::cMangled },
  { "gt_o_function", (void**)&gt_o_function, Dylib::Import::cMangled },
  { "gt_o_function_default", (void**)&gt_o_function_default, Dylib::Import::cMangled },
  { "gt_o_function_max", (void**)&gt_o_function_max, Dylib::Import::cMangled },
  { "gt_o_function_min", (void**)&gt_o_function_min, Dylib::Import::cMangled },
  { "gt_openDevices", (void**)&gt_openDevices, Dylib::Import::cMangled },
  { "gt_operationmode", (void**)&gt_operationmode, Dylib::Import::cMangled },
  { "gt_operationmode_default", (void**)&gt_operationmode_default, Dylib::Import::cMangled },
  { "gt_operationmode_max", (void**)&gt_operationmode_max, Dylib::Import::cMangled },
  { "gt_operationmode_min", (void**)&gt_operationmode_min, Dylib::Import::cMangled },
  { "gt_periods", (void**)&gt_periods, Dylib::Import::cMangled },
  { "gt_phase_charge", (void**)&gt_phase_charge, Dylib::Import::cMangled },
  { "gt_phase_current1", (void**)&gt_phase_current1, Dylib::Import::cMangled },
  { "gt_phase_current1_default", (void**)&gt_phase_current1_default, Dylib::Import::cMangled },
  { "gt_phase_current1_max", (void**)&gt_phase_current1_max, Dylib::Import::cMangled },
  { "gt_phase_current1_min", (void**)&gt_phase_current1_min, Dylib::Import::cMangled },
  { "gt_phase_current2", (void**)&gt_phase_current2, Dylib::Import::cMangled },
  { "gt_phase_current2_default", (void**)&gt_phase_current2_default, Dylib::Import::cMangled },
  { "gt_phase_current2_max", (void**)&gt_phase_current2_max, Dylib::Import::cMangled },
  { "gt_phase_current2_min", (void**)&gt_phase_current2_min, Dylib::Import::cMangled },
  { "gt_phase_duration1", (void**)&gt_phase_duration1, Dylib::Import::cMangled },
  { "gt_phase_duration1_default", (void**)&gt_phase_duration1_default, Dylib::Import::cMangled },
  { "gt_phase_duration1_max", (void**)&gt_phase_duration1_max, Dylib::Import::cMangled },
  { "gt_phase_duration1_min", (void**)&gt_phase_duration1_min, Dylib::Import::cMangled },
  { "gt_phase_duration2", (void**)&gt_phase_duration2, Dylib::Import::cMangled },
  { "gt_phase_duration2_default", (void**)&gt_phase_duration2_default, Dylib::Import::cMangled },
  { "gt_phase_duration2_max", (void**)&gt_phase_duration2_max, Dylib::Import::cMangled },
  { "gt_phase_duration2_min", (void**)&gt_phase_duration2_min, Dylib::Import::cMangled },
  { "gt_pulse_duration", (void**)&gt_pulse_duration, Dylib::Import::cMangled },
  { "gt_pulse_period", (void**)&gt_pulse_period, Dylib::Import::cMangled },
  { "gt_pulse_rate", (void**)&gt_pulse_rate, Dylib::Import::cMangled },
  { "gt_pulse_rate_default", (void**)&gt_pulse_rate_default, Dylib::Import::cMangled },
  { "gt_pulse_rate_max", (void**)&gt_pulse_rate_max, Dylib::Import::cMangled },
  { "gt_pulse_rate_min", (void**)&gt_pulse_rate_min, Dylib::Import::cMangled },
  { "gt_pulses", (void**)&gt_pulses, Dylib::Import::cMangled },
  { "gt_pulses_default", (void**)&gt_pulses_default, Dylib::Import::cMangled },
  { "gt_pulses_max", (void**)&gt_pulses_max, Dylib::Import::cMangled },
  { "gt_pulses_min", (void**)&gt_pulses_min, Dylib::Import::cMangled },
  { "gt_remaining", (void**)&gt_remaining, Dylib::Import::cMangled },
  { "gt_resistance", (void**)&gt_resistance, Dylib::Import::cMangled },
  { "gt_selftest_passed", (void**)&gt_selftest_passed, Dylib::Import::cMangled },
  { "gt_sequence_duration", (void**)&gt_sequence_duration, Dylib::Import::cMangled },
  { "gt_serial", (void**)&gt_serial, Dylib::Import::cMangled },
  { "gt_sessionLogFile", (void**)&gt_sessionLogFile, Dylib::Import::cMangled },
  { "gt_state", (void**)&gt_state, Dylib::Import::cMangled },
  { "gt_stim_event", (void**)&gt_stim_event, Dylib::Import::cMangled },
  { "gt_test_resistance", (void**)&gt_test_resistance, Dylib::Import::cMangled },
  { "gt_total_charge", (void**)&gt_total_charge, Dylib::Import::cMangled },
  { "gt_total_charge_default", (void**)&gt_total_charge_default, Dylib::Import::cMangled },
  { "gt_total_charge_max", (void**)&gt_total_charge_max, Dylib::Import::cMangled },
  { "gt_total_charge_min", (void**)&gt_total_charge_min, Dylib::Import::cMangled },
  { "gt_train_charge", (void**)&gt_train_charge, Dylib::Import::cMangled },
  { "gt_train_duration", (void**)&gt_train_duration, Dylib::Import::cMangled },
  { "gt_train_period", (void**)&gt_train_period, Dylib::Import::cMangled },
  { "gt_train_rate", (void**)&gt_train_rate, Dylib::Import::cMangled },
  { "gt_train_rate_default", (void**)&gt_train_rate_default, Dylib::Import::cMangled },
  { "gt_train_rate_max", (void**)&gt_train_rate_max, Dylib::Import::cMangled },
  { "gt_train_rate_min", (void**)&gt_train_rate_min, Dylib::Import::cMangled },
  { "gt_voltage", (void**)&gt_voltage, Dylib::Import::cMangled },
  { "gt_voltage1_avg", (void**)&gt_voltage1_avg, Dylib::Import::cMangled },
  { "gt_voltage1_sum", (void**)&gt_voltage1_sum, Dylib::Import::cMangled },
  { "gt_voltage2_avg", (void**)&gt_voltage2_avg, Dylib::Import::cMangled },
  { "gt_voltage2_sum", (void**)&gt_voltage2_sum, Dylib::Import::cMangled },
  { "gt_voltage_limit", (void**)&gt_voltage_limit, Dylib::Import::cMangled },
  { "gt_voltage_limit_default", (void**)&gt_voltage_limit_default, Dylib::Import::cMangled },
  { "gt_voltage_limit_max", (void**)&gt_voltage_limit_max, Dylib::Import::cMangled },
  { "gt_voltage_limit_min", (void**)&gt_voltage_limit_min, Dylib::Import::cMangled },
  { "has_dongle", (void**)&has_dongle, Dylib::Import::cMangled },
  { "isOpenDevice", (void**)&isOpenDevice, Dylib::Import::cMangled },
  { "is_FES_serial", (void**)&is_FES_serial, Dylib::Import::cMangled },
  { "measure_resistance", (void**)&measure_resistance, Dylib::Import::cMangled },
  { "newSessionLog", (void**)&newSessionLog, Dylib::Import::cMangled },
  { "openDevice", (void**)&openDevice, Dylib::Import::cMangled },
  { "openDevices", (void**)&openDevices, Dylib::Import::cMangled },
  { "open_connect_sm", (void**)&open_connect_sm, Dylib::Import::cMangled },
  { "rcv_FW_version", (void**)&rcv_FW_version, Dylib::Import::cMangled },
  { "rcv_HW_version", (void**)&rcv_HW_version, Dylib::Import::cMangled },
  { "rcv_all", (void**)&rcv_all, Dylib::Import::cMangled },
  { "rcv_alternate", (void**)&rcv_alternate, Dylib::Import::cMangled },
  { "rcv_applied_pulses", (void**)&rcv_applied_pulses, Dylib::Import::cMangled },
  { "rcv_battery1", (void**)&rcv_battery1, Dylib::Import::cMangled },
  { "rcv_battery2", (void**)&rcv_battery2, Dylib::Import::cMangled },
  { "rcv_coeff_cs1", (void**)&rcv_coeff_cs1, Dylib::Import::cMangled },
  { "rcv_coeff_cs2", (void**)&rcv_coeff_cs2, Dylib::Import::cMangled },
  { "rcv_coeff_i", (void**)&rcv_coeff_i, Dylib::Import::cMangled },
  { "rcv_coeff_r", (void**)&rcv_coeff_r, Dylib::Import::cMangled },
  { "rcv_coeff_v", (void**)&rcv_coeff_v, Dylib::Import::cMangled },
  { "rcv_current", (void**)&rcv_current, Dylib::Import::cMangled },
  { "rcv_current1_sum", (void**)&rcv_current1_sum, Dylib::Import::cMangled },
  { "rcv_current2_sum", (void**)&rcv_current2_sum, Dylib::Import::cMangled },
  { "rcv_di_state", (void**)&rcv_di_state, Dylib::Import::cMangled },
  { "rcv_dio_enabled", (void**)&rcv_dio_enabled, Dylib::Import::cMangled },
  { "rcv_do1_function", (void**)&rcv_do1_function, Dylib::Import::cMangled },
  { "rcv_do1_post_time", (void**)&rcv_do1_post_time, Dylib::Import::cMangled },
  { "rcv_do1_pre_time", (void**)&rcv_do1_pre_time, Dylib::Import::cMangled },
  { "rcv_do2_function", (void**)&rcv_do2_function, Dylib::Import::cMangled },
  { "rcv_do2_post_time", (void**)&rcv_do2_post_time, Dylib::Import::cMangled },
  { "rcv_do2_pre_time", (void**)&rcv_do2_pre_time, Dylib::Import::cMangled },
  { "rcv_duration1_sum", (void**)&rcv_duration1_sum, Dylib::Import::cMangled },
  { "rcv_duration2_sum", (void**)&rcv_duration2_sum, Dylib::Import::cMangled },
  { "rcv_error", (void**)&rcv_error, Dylib::Import::cMangled },
  { "rcv_event", (void**)&rcv_event, Dylib::Import::cMangled },
  { "rcv_fade_in_time", (void**)&rcv_fade_in_time, Dylib::Import::cMangled },
  { "rcv_fade_out_time", (void**)&rcv_fade_out_time, Dylib::Import::cMangled },
  { "rcv_fade_progress", (void**)&rcv_fade_progress, Dylib::Import::cMangled },
  { "rcv_fade_type", (void**)&rcv_fade_type, Dylib::Import::cMangled },
  { "rcv_highvoltage_led", (void**)&rcv_highvoltage_led, Dylib::Import::cMangled },
  { "rcv_identifier", (void**)&rcv_identifier, Dylib::Import::cMangled },
  { "rcv_interphase", (void**)&rcv_interphase, Dylib::Import::cMangled },
  { "rcv_max_current_ph1", (void**)&rcv_max_current_ph1, Dylib::Import::cMangled },
  { "rcv_max_current_ph2", (void**)&rcv_max_current_ph2, Dylib::Import::cMangled },
  { "rcv_min_current_ph1", (void**)&rcv_min_current_ph1, Dylib::Import::cMangled },
  { "rcv_min_current_ph2", (void**)&rcv_min_current_ph2, Dylib::Import::cMangled },
  { "rcv_o_function", (void**)&rcv_o_function, Dylib::Import::cMangled },
  { "rcv_phase_current1", (void**)&rcv_phase_current1, Dylib::Import::cMangled },
  { "rcv_phase_current2", (void**)&rcv_phase_current2, Dylib::Import::cMangled },
  { "rcv_phase_duration1", (void**)&rcv_phase_duration1, Dylib::Import::cMangled },
  { "rcv_phase_duration2", (void**)&rcv_phase_duration2, Dylib::Import::cMangled },
  { "rcv_pulse_rate", (void**)&rcv_pulse_rate, Dylib::Import::cMangled },
  { "rcv_pulses", (void**)&rcv_pulses, Dylib::Import::cMangled },
  { "rcv_resistance", (void**)&rcv_resistance, Dylib::Import::cMangled },
  { "rcv_samples", (void**)&rcv_samples, Dylib::Import::cMangled },
  { "rcv_serial", (void**)&rcv_serial, Dylib::Import::cMangled },
  { "rcv_state", (void**)&rcv_state, Dylib::Import::cMangled },
  { "rcv_stim", (void**)&rcv_stim, Dylib::Import::cMangled },
  { "rcv_stim_event", (void**)&rcv_stim_event, Dylib::Import::cMangled },
  { "rcv_total_charge", (void**)&rcv_total_charge, Dylib::Import::cMangled },
  { "rcv_voltage", (void**)&rcv_voltage, Dylib::Import::cMangled },
  { "rcv_voltage1_sum", (void**)&rcv_voltage1_sum, Dylib::Import::cMangled },
  { "rcv_voltage2_sum", (void**)&rcv_voltage2_sum, Dylib::Import::cMangled },
  { "rcv_voltage_limit", (void**)&rcv_voltage_limit, Dylib::Import::cMangled },
  { "register_with_polling", (void**)&register_with_polling, Dylib::Import::cMangled },
  { "request", (void**)&request, Dylib::Import::cMangled },
  { "reset_total_charge", (void**)&reset_total_charge, Dylib::Import::cMangled },
  { "selftest", (void**)&selftest, Dylib::Import::cMangled },
  { "sequence_running", (void**)&sequence_running, Dylib::Import::cMangled },
  { "sessionLog", (void**)&sessionLog, Dylib::Import::cMangled },
  { "set_socket_port_for_sim", (void**)&set_socket_port_for_sim, Dylib::Import::cMangled },
  { "snd_FW_version", (void**)&snd_FW_version, Dylib::Import::cMangled },
  { "snd_HW_version", (void**)&snd_HW_version, Dylib::Import::cMangled },
  { "snd_alternate", (void**)&snd_alternate, Dylib::Import::cMangled },
  { "snd_coeff_cs1", (void**)&snd_coeff_cs1, Dylib::Import::cMangled },
  { "snd_coeff_cs2", (void**)&snd_coeff_cs2, Dylib::Import::cMangled },
  { "snd_coeff_i", (void**)&snd_coeff_i, Dylib::Import::cMangled },
  { "snd_coeff_r", (void**)&snd_coeff_r, Dylib::Import::cMangled },
  { "snd_coeff_v", (void**)&snd_coeff_v, Dylib::Import::cMangled },
  { "snd_current_n_duration", (void**)&snd_current_n_duration, Dylib::Import::cMangled },
  { "snd_dio_enabled", (void**)&snd_dio_enabled, Dylib::Import::cMangled },
  { "snd_do1_function", (void**)&snd_do1_function, Dylib::Import::cMangled },
  { "snd_do1_post_time", (void**)&snd_do1_post_time, Dylib::Import::cMangled },
  { "snd_do1_pre_time", (void**)&snd_do1_pre_time, Dylib::Import::cMangled },
  { "snd_do2_function", (void**)&snd_do2_function, Dylib::Import::cMangled },
  { "snd_do2_post_time", (void**)&snd_do2_post_time, Dylib::Import::cMangled },
  { "snd_do2_pre_time", (void**)&snd_do2_pre_time, Dylib::Import::cMangled },
  { "snd_fade_in_time", (void**)&snd_fade_in_time, Dylib::Import::cMangled },
  { "snd_fade_out_time", (void**)&snd_fade_out_time, Dylib::Import::cMangled },
  { "snd_fade_type", (void**)&snd_fade_type, Dylib::Import::cMangled },
  { "snd_highvoltage_led", (void**)&snd_highvoltage_led, Dylib::Import::cMangled },
  { "snd_identifier", (void**)&snd_identifier, Dylib::Import::cMangled },
  { "snd_interphase", (void**)&snd_interphase, Dylib::Import::cMangled },
  { "snd_o_function", (void**)&snd_o_function, Dylib::Import::cMangled },
  { "snd_phase_current1", (void**)&snd_phase_current1, Dylib::Import::cMangled },
  { "snd_phase_current2", (void**)&snd_phase_current2, Dylib::Import::cMangled },
  { "snd_phase_duration1", (void**)&snd_phase_duration1, Dylib::Import::cMangled },
  { "snd_phase_duration2", (void**)&snd_phase_duration2, Dylib::Import::cMangled },
  { "snd_pulse_rate", (void**)&snd_pulse_rate, Dylib::Import::cMangled },
  { "snd_pulses", (void**)&snd_pulses, Dylib::Import::cMangled },
  { "snd_serial", (void**)&snd_serial, Dylib::Import::cMangled },
  { "snd_state", (void**)&snd_state, Dylib::Import::cMangled },
  { "snd_total_charge", (void**)&snd_total_charge, Dylib::Import::cMangled },
  { "snd_voltage_limit", (void**)&snd_voltage_limit, Dylib::Import::cMangled },
  { "st_FW_version", (void**)&st_FW_version, Dylib::Import::cMangled },
  { "st_HW_version", (void**)&st_HW_version, Dylib::Import::cMangled },
  { "st_alternate", (void**)&st_alternate, Dylib::Import::cMangled },
  { "st_coeff_cs1", (void**)&st_coeff_cs1, Dylib::Import::cMangled },
  { "st_coeff_cs2", (void**)&st_coeff_cs2, Dylib::Import::cMangled },
  { "st_coeff_i", (void**)&st_coeff_i, Dylib::Import::cMangled },
  { "st_coeff_r", (void**)&st_coeff_r, Dylib::Import::cMangled },
  { "st_coeff_v", (void**)&st_coeff_v, Dylib::Import::cMangled },
  { "st_default", (void**)&st_default, Dylib::Import::cMangled },
  { "st_defaults", (void**)&st_defaults, Dylib::Import::cMangled },
  { "st_dio_enabled", (void**)&st_dio_enabled, Dylib::Import::cMangled },
  { "st_do1_function", (void**)&st_do1_function, Dylib::Import::cMangled },
  { "st_do1_post_time", (void**)&st_do1_post_time, Dylib::Import::cMangled },
  { "st_do1_pre_time", (void**)&st_do1_pre_time, Dylib::Import::cMangled },
  { "st_do2_function", (void**)&st_do2_function, Dylib::Import::cMangled },
  { "st_do2_post_time", (void**)&st_do2_post_time, Dylib::Import::cMangled },
  { "st_do2_pre_time", (void**)&st_do2_pre_time, Dylib::Import::cMangled },
  { "st_el_circ_diameter", (void**)&st_el_circ_diameter, Dylib::Import::cMangled },
  { "st_el_depth_diameter", (void**)&st_el_depth_diameter, Dylib::Import::cMangled },
  { "st_el_depth_length", (void**)&st_el_depth_length, Dylib::Import::cMangled },
  { "st_el_other_surface", (void**)&st_el_other_surface, Dylib::Import::cMangled },
  { "st_el_type", (void**)&st_el_type, Dylib::Import::cMangled },
  { "st_fade_in_time", (void**)&st_fade_in_time, Dylib::Import::cMangled },
  { "st_fade_out_time", (void**)&st_fade_out_time, Dylib::Import::cMangled },
  { "st_fade_type", (void**)&st_fade_type, Dylib::Import::cMangled },
  { "st_highvoltage_led", (void**)&st_highvoltage_led, Dylib::Import::cMangled },
  { "st_identifier", (void**)&st_identifier, Dylib::Import::cMangled },
  { "st_interphase", (void**)&st_interphase, Dylib::Import::cMangled },
  { "st_jitter", (void**)&st_jitter, Dylib::Import::cMangled },
  { "st_maximum", (void**)&st_maximum, Dylib::Import::cMangled },
  { "st_minimum", (void**)&st_minimum, Dylib::Import::cMangled },
  { "st_n_trains", (void**)&st_n_trains, Dylib::Import::cMangled },
  { "st_o_function", (void**)&st_o_function, Dylib::Import::cMangled },
  { "st_operationmode", (void**)&st_operationmode, Dylib::Import::cMangled },
  { "st_phase_current1", (void**)&st_phase_current1, Dylib::Import::cMangled },
  { "st_phase_current2", (void**)&st_phase_current2, Dylib::Import::cMangled },
  { "st_phase_duration1", (void**)&st_phase_duration1, Dylib::Import::cMangled },
  { "st_phase_duration2", (void**)&st_phase_duration2, Dylib::Import::cMangled },
  { "st_pulse_period", (void**)&st_pulse_period, Dylib::Import::cMangled },
  { "st_pulse_rate", (void**)&st_pulse_rate, Dylib::Import::cMangled },
  { "st_pulses", (void**)&st_pulses, Dylib::Import::cMangled },
  { "st_sequence_duration_by_n_trains", (void**)&st_sequence_duration_by_n_trains, Dylib::Import::cMangled },
  { "st_sequence_duration_by_train_rate", (void**)&st_sequence_duration_by_train_rate, Dylib::Import::cMangled },
  { "st_serial", (void**)&st_serial, Dylib::Import::cMangled },
  { "st_state", (void**)&st_state, Dylib::Import::cMangled },
  { "st_total_charge", (void**)&st_total_charge, Dylib::Import::cMangled },
  { "st_train_duration_by_pulse_rate", (void**)&st_train_duration_by_pulse_rate, Dylib::Import::cMangled },
  { "st_train_duration_by_pulses", (void**)&st_train_duration_by_pulses, Dylib::Import::cMangled },
  { "st_train_period", (void**)&st_train_period, Dylib::Import::cMangled },
  { "st_train_rate", (void**)&st_train_rate, Dylib::Import::cMangled },
  { "st_voltage_limit", (void**)&st_voltage_limit, Dylib::Import::cMangled },
  { "start_sequence", (void**)&start_sequence, Dylib::Import::cMangled },
  { "stop", (void**)&stop, Dylib::Import::cMangled },
  { "toggle_debuglog", (void**)&toggle_debuglog, Dylib::Import::cMangled },
  { "unregister_with_polling", (void**)&unregister_with_polling, Dylib::Import::cMangled },
  { "wait_for_end_of_sequence", (void**)&wait_for_end_of_sequence, Dylib::Import::cMangled },
  { 0, 0, 0 }
};
const Dylib::Import* functionImports = functionImports_;
}

#endif // STATIC_LIBGESTIMAPI


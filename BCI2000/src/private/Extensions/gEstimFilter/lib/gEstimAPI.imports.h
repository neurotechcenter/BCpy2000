// Import definitions for gEstimAPI library
// generated Fri Oct 12 11:14:09 2018 by DylibTool
#ifndef GESTIMAPI_IMPORTS_H
#define GESTIMAPI_IMPORTS_H

#include "Win32Defs.h"

#ifndef STATIC_LIBGESTIMAPI
// Disable function declarations in the original header
// file by #defining function names to point to symbols that
// are not referenced by any code.
#define abort_sequence __072d7979_abort_sequence
#define activate __1702000c_activate
#define calibrate2 __64210d1d_calibrate2
#define closeDevice __6f4b6f05_closeDevice
#define closeDevices __6f4b6f76_closeDevices
#define close_disconnect_sm __44466a00_close_disconnect_sm
#define clr_last_error __3f1a7359_clr_last_error
#define count_openDevices __77373d1e_count_openDevices
#define debugLog __03290d12_debugLog
#define deviceReady __66645d0c_deviceReady
#define error_field __72417006_error_field
#define error_fields __72417075_error_fields
#define error_state_change __7254052a_error_state_change
#define fw_hw_ok __11283003_fw_hw_ok
#define gt_CrtVersion __09736049_gt_CrtVersion
#define gt_FW_version __2c58404c_gt_FW_version
#define gt_HW_version __2c584042_gt_HW_version
#define gt_alternate __65614e76_gt_alternate
#define gt_alternate_default __5b704764_gt_alternate_default
#define gt_alternate_max __3a0c2f0e_gt_alternate_max
#define gt_alternate_min __3a0c2718_gt_alternate_min
#define gt_applied_pulses __740c1f1d_gt_applied_pulses
#define gt_applied_trains __73010512_gt_applied_trains
#define gt_battery1 __74791a07_gt_battery1
#define gt_battery2 __74791907_gt_battery2
#define gt_battery_pc __17797477_gt_battery_pc
#define gt_charge_density __7d392015_gt_charge_density
#define gt_coeff_cs1 __57724a34_gt_coeff_cs1
#define gt_coeff_cs2 __57724a37_gt_coeff_cs2
#define gt_coeff_i __57783905_gt_coeff_i
#define gt_coeff_r __57633905_gt_coeff_r
#define gt_coeff_v __57673905_gt_coeff_v
#define gt_current __7c722d06_gt_current
#define gt_current1_avg __1d047b59_gt_current1_avg
#define gt_current1_sum __0f077159_gt_current1_sum
#define gt_current2_avg __1d047859_gt_current2_avg
#define gt_current2_sum __0f077259_gt_current2_sum
#define gt_di_state __6f5f4910_gt_di_state
#define gt_dio_enabled __051e626d_gt_dio_enabled
#define gt_dio_enabled_default __0c0c5c7c_gt_dio_enabled_default
#define gt_dio_enabled_max __64663d00_gt_dio_enabled_max
#define gt_dio_enabled_min __6c703d00_gt_dio_enabled_min
#define gt_do1_function __14440d76_gt_do1_function
#define gt_do1_function_default __054d1f48_gt_do1_function_default
#define gt_do1_function_max __79257529_gt_do1_function_max
#define gt_do1_function_min __792d6329_gt_do1_function_min
#define gt_do1_post_time __135f192e_gt_do1_post_time
#define gt_do1_post_time_default __2d4e103c_gt_do1_post_time_default
#define gt_do1_post_time_max __4c327856_gt_do1_post_time_max
#define gt_do1_post_time_min __4c327040_gt_do1_post_time_min
#define gt_do1_pre_time __134d3a60_gt_do1_pre_time
#define gt_do1_pre_time_default __0244285e_gt_do1_pre_time_default
#define gt_do1_pre_time_max __7e2c423f_gt_do1_pre_time_max
#define gt_do1_pre_time_min __7e24543f_gt_do1_pre_time_min
#define gt_do2_function __14470d76_gt_do2_function
#define gt_do2_function_default __054e1f48_gt_do2_function_default
#define gt_do2_function_max __79267529_gt_do2_function_max
#define gt_do2_function_min __792e6329_gt_do2_function_min
#define gt_do2_post_time __135c192e_gt_do2_post_time
#define gt_do2_post_time_default __2d4d103c_gt_do2_post_time_default
#define gt_do2_post_time_max __4c317856_gt_do2_post_time_max
#define gt_do2_post_time_min __4c317040_gt_do2_post_time_min
#define gt_do2_pre_time __134e3a60_gt_do2_pre_time
#define gt_do2_pre_time_default __0247285e_gt_do2_pre_time_default
#define gt_do2_pre_time_max __7e2f423f_gt_do2_pre_time_max
#define gt_do2_pre_time_min __7e27543f_gt_do2_pre_time_min
#define gt_duration1_sum __241a254c_gt_duration1_sum
#define gt_duration2_sum __241a254f_gt_duration2_sum
#define gt_el_calc_surface __715f0516_gt_el_calc_surface
#define gt_el_circ_diameter __644c7c0d_gt_el_circ_diameter
#define gt_el_circ_diameter_default __75456e33_gt_el_circ_diameter_default
#define gt_el_circ_diameter_max __092d0452_gt_el_circ_diameter_max
#define gt_el_circ_diameter_min __09251252_gt_el_circ_diameter_min
#define gt_el_depth_diameter __7a425740_gt_el_depth_diameter
#define gt_el_depth_diameter_default __44535e52_gt_el_depth_diameter_default
#define gt_el_depth_diameter_max __252f3638_gt_el_depth_diameter_max
#define gt_el_depth_diameter_min __252f3e2e_gt_el_depth_diameter_min
#define gt_el_depth_length __63523d38_gt_el_depth_length
#define gt_el_depth_length_default __6a400329_gt_el_depth_length_default
#define gt_el_depth_length_max __022a6255_gt_el_depth_length_max
#define gt_el_depth_length_min __0a3c6255_gt_el_depth_length_min
#define gt_el_other_surface __71585528_gt_el_other_surface
#define gt_el_other_surface_default __60514716_gt_el_other_surface_default
#define gt_el_other_surface_max __1c392d77_gt_el_other_surface_max
#define gt_el_other_surface_min __1c313b77_gt_el_other_surface_min
#define gt_el_type __7b4e2b1c_gt_el_type
#define gt_el_type_default __725c150d_gt_el_type_default
#define gt_el_type_max __1a367471_gt_el_type_max
#define gt_el_type_min __12207471_gt_el_type_min
#define gt_error __15063017_gt_error
#define gt_error_text __3e72556f_gt_error_text
#define gt_event __11113111_gt_event
#define gt_fade_factor __0f03594d_gt_fade_factor
#define gt_fade_in_pulses __6912162c_gt_fade_in_pulses
#define gt_fade_in_time __0613004d_gt_fade_in_time
#define gt_fade_in_time_default __171a1273_gt_fade_in_time_default
#define gt_fade_in_time_max __6b727812_gt_fade_in_time_max
#define gt_fade_in_time_min __6b7a6e12_gt_fade_in_time_min
#define gt_fade_out_pulses __7c632215_gt_fade_out_pulses
#define gt_fade_out_time __1d0c2303_gt_fade_out_time
#define gt_fade_out_time_default __231d2a11_gt_fade_out_time_default
#define gt_fade_out_time_max __4261427b_gt_fade_out_time_max
#define gt_fade_out_time_min __42614a6d_gt_fade_out_time_min
#define gt_fade_progress __0407262d_gt_fade_progress
#define gt_fade_type __72694a5c_gt_fade_type
#define gt_fade_type_default __4c78434e_gt_fade_type_default
#define gt_fade_type_max __2d042b24_gt_fade_type_max
#define gt_fade_type_min __2d042332_gt_fade_type_min
#define gt_ftdi_driver_version __180c7350_gt_ftdi_driver_version
#define gt_ftdi_library_version __01013167_gt_ftdi_library_version
#define gt_highvoltage_led __637e1c13_gt_highvoltage_led
#define gt_highvoltage_led_default __6a6c2202_gt_highvoltage_led_default
#define gt_highvoltage_led_max __0206437e_gt_highvoltage_led_max
#define gt_highvoltage_led_min __0a10437e_gt_highvoltage_led_min
#define gt_identifier __18775878_gt_identifier
#define gt_interphase __1c685b68_gt_interphase
#define gt_interphase_default __0e564a61_gt_interphase_default
#define gt_interphase_max __64373609_gt_interphase_max
#define gt_interphase_min __72373601_gt_interphase_min
#define gt_jitter __7c002b0f_gt_jitter
#define gt_jitter_default __6e3e3a06_gt_jitter_default
#define gt_jitter_max __045f466e_gt_jitter_max
#define gt_jitter_min __125f4666_gt_jitter_min
#define gt_last_error __1175595c_gt_last_error
#define gt_last_resistance __64733934_gt_last_resistance
#define gt_max_current_ph1 __753b2d1b_gt_max_current_ph1
#define gt_max_current_ph2 __75382d1b_gt_max_current_ph2
#define gt_min_current_ph1 __7d2d2d1b_gt_min_current_ph1
#define gt_min_current_ph2 __7d2e2d1b_gt_min_current_ph2
#define gt_n_trains __516e5e0f_gt_n_trains
#define gt_n_trains_default __40674c31_gt_n_trains_default
#define gt_n_trains_max __3c0f2650_gt_n_trains_max
#define gt_n_trains_min __3c073050_gt_n_trains_min
#define gt_o_function __3566436e_gt_o_function
#define gt_o_function_default __27585267_gt_o_function_default
#define gt_o_function_max __4d392e0f_gt_o_function_max
#define gt_o_function_min __5b392e07_gt_o_function_min
#define gt_openDevices __17145848_gt_openDevices
#define gt_operationmode __0e172605_gt_operationmode
#define gt_operationmode_default __30062f17_gt_operationmode_default
#define gt_operationmode_max __517a477d_gt_operationmode_max
#define gt_operationmode_min __517a4f6b_gt_operationmode_min
#define gt_periods __6675361f_gt_periods
#define gt_phase_charge __22112174_gt_phase_charge
#define gt_phase_current1 __13133713_gt_phase_current1
#define gt_phase_current1_default __012d261a_gt_phase_current1_default
#define gt_phase_current1_max __6b4c5a72_gt_phase_current1_max
#define gt_phase_current1_min __7d4c5a7a_gt_phase_current1_min
#define gt_phase_current2 __10133713_gt_phase_current2
#define gt_phase_current2_default __022d261a_gt_phase_current2_default
#define gt_phase_current2_max __684c5a72_gt_phase_current2_max
#define gt_phase_current2_min __7e4c5a7a_gt_phase_current2_min
#define gt_phase_duration1 __5f343008_gt_phase_duration1
#define gt_phase_duration1_default __56260e19_gt_phase_duration1_default
#define gt_phase_duration1_max __3e4c6f65_gt_phase_duration1_max
#define gt_phase_duration1_min __365a6f65_gt_phase_duration1_min
#define gt_phase_duration2 __5f373008_gt_phase_duration2
#define gt_phase_duration2_default __56250e19_gt_phase_duration2_default
#define gt_phase_duration2_max __3e4f6f65_gt_phase_duration2_max
#define gt_phase_duration2_min __36596f65_gt_phase_duration2_min
#define gt_pulse_duration __42083008_gt_pulse_duration
#define gt_pulse_period __24072d67_gt_pulse_period
#define gt_pulse_rate __286a4d61_gt_pulse_rate
#define gt_pulse_rate_default __3a545c68_gt_pulse_rate_default
#define gt_pulse_rate_max __50352000_gt_pulse_rate_max
#define gt_pulse_rate_min __46352008_gt_pulse_rate_min
#define gt_pulses __61182c15_gt_pulses
#define gt_pulses_default __73263d1c_gt_pulses_default
#define gt_pulses_max __19474174_gt_pulses_max
#define gt_pulses_min __0f47417c_gt_pulses_min
#define gt_remaining __6c70507c_gt_remaining
#define gt_resistance __13665862_gt_resistance
#define gt_selftest_passed __726e3e2b_gt_selftest_passed
#define gt_sequence_duration __7c7a5246_gt_sequence_duration
#define gt_serial __6e063612_gt_serial
#define gt_sessionLogFile __6f2f0919_gt_sessionLogFile
#define gt_state __13152b16_gt_state
#define gt_stim_event __026b5742_gt_stim_event
#define gt_test_resistance __6073392c_gt_test_resistance
#define gt_total_charge __25043379_gt_total_charge
#define gt_total_charge_default __340d2147_gt_total_charge_default
#define gt_total_charge_max __48654b26_gt_total_charge_max
#define gt_total_charge_min __486d5d26_gt_total_charge_min
#define gt_train_charge __38113b7b_gt_train_charge
#define gt_train_duration __45052a07_gt_train_duration
#define gt_train_period __230a3768_gt_train_period
#define gt_train_rate __2f67576e_gt_train_rate
#define gt_train_rate_default __3d594667_gt_train_rate_default
#define gt_train_rate_max __57383a0f_gt_train_rate_max
#define gt_train_rate_min __41383a07_gt_train_rate_min
#define gt_voltage __6f7d2b17_gt_voltage
#define gt_voltage1_avg __0e0b7d48_gt_voltage1_avg
#define gt_voltage1_sum __1c087748_gt_voltage1_sum
#define gt_voltage2_avg __0e0b7e48_gt_voltage2_avg
#define gt_voltage2_sum __1c087448_gt_voltage2_sum
#define gt_voltage_limit __06101d0f_gt_voltage_limit
#define gt_voltage_limit_default __3801141d_gt_voltage_limit_default
#define gt_voltage_limit_max __597d7c77_gt_voltage_limit_max
#define gt_voltage_limit_min __597d7461_gt_voltage_limit_min
#define has_dongle __606b1d38_has_dongle
#define isOpenDevice __7a746870_isOpenDevice
#define is_FES_serial __25526954_is_FES_serial
#define measure_resistance __7a63162b_measure_resistance
#define newSessionLog __03784855_newSessionLog
#define openDevice __48701307_openDevice
#define openDevices __48706007_openDevices
#define open_connect_sm __01050474_open_connect_sm
#define rcv_FW_version __3e285a40_rcv_FW_version
#define rcv_HW_version __30285a40_rcv_HW_version
#define rcv_all __130f1a5f_rcv_all
#define rcv_alternate __0461634e_rcv_alternate
#define rcv_applied_pulses __6f700e1f_rcv_applied_pulses
#define rcv_battery1 __75707b1a_rcv_battery1
#define rcv_battery2 __75707b19_rcv_battery2
#define rcv_coeff_cs1 __4653704a_rcv_coeff_cs1
#define rcv_coeff_cs2 __4553704a_rcv_coeff_cs2
#define rcv_coeff_i __77537a39_rcv_coeff_i
#define rcv_coeff_r __77536139_rcv_coeff_r
#define rcv_coeff_v __77536539_rcv_coeff_v
#define rcv_current __7478702d_rcv_current
#define rcv_current1_sum __2b0b0571_rcv_current1_sum
#define rcv_current2_sum __2b0b0572_rcv_current2_sum
#define rcv_di_state __626b5d49_rcv_di_state
#define rcv_dio_enabled __1f011c62_rcv_dio_enabled
#define rcv_do1_function __0410460d_rcv_do1_function
#define rcv_do1_post_time __5c175d19_rcv_do1_post_time
#define rcv_do1_pre_time __12174f3a_rcv_do1_pre_time
#define rcv_do2_function __0410450d_rcv_do2_function
#define rcv_do2_post_time __5c175e19_rcv_do2_post_time
#define rcv_do2_pre_time __12174c3a_rcv_do2_pre_time
#define rcv_duration1_sum __3e201825_rcv_duration1_sum
#define rcv_duration2_sum __3d201825_rcv_duration2_sum
#define rcv_error __65110430_rcv_error
#define rcv_event __63151331_rcv_event
#define rcv_fade_in_time __3f021100_rcv_fade_in_time
#define rcv_fade_out_time __71190e23_rcv_fade_out_time
#define rcv_fade_progress __5f000526_rcv_fade_progress
#define rcv_fade_type __2e766b4a_rcv_fade_type
#define rcv_highvoltage_led __61677c1c_rcv_highvoltage_led
#define rcv_identifier __0a1c7558_rcv_identifier
#define rcv_interphase __1a186a5b_rcv_interphase
#define rcv_max_current_ph1 __6971392d_rcv_max_current_ph1
#define rcv_max_current_ph2 __69713a2d_rcv_max_current_ph2
#define rcv_min_current_ph1 __69792f2d_rcv_min_current_ph1
#define rcv_min_current_ph2 __69792c2d_rcv_min_current_ph2
#define rcv_o_function __1c316443_rcv_o_function
#define rcv_phase_current1 __61171137_rcv_phase_current1
#define rcv_phase_current2 __61141137_rcv_phase_current2
#define rcv_phase_duration1 __7a5b3630_rcv_phase_duration1
#define rcv_phase_duration2 __7a5b3530_rcv_phase_duration2
#define rcv_pulse_rate __132c684d_rcv_pulse_rate
#define rcv_pulses __67651a2c_rcv_pulses
#define rcv_resistance __10176458_rcv_resistance
#define rcv_samples __6d67682f_rcv_samples
#define rcv_serial __606a0436_rcv_serial
#define rcv_state __6417172b_rcv_state
#define rcv_stim __01171f32_rcv_stim
#define rcv_stim_event __30066957_rcv_stim_event
#define rcv_total_charge __0b210633_rcv_total_charge
#define rcv_voltage __656b7f2b_rcv_voltage
#define rcv_voltage1_sum __3a180a77_rcv_voltage1_sum
#define rcv_voltage2_sum __3a180a74_rcv_voltage2_sum
#define rcv_voltage_limit __7d02121d_rcv_voltage_limit
#define register_with_polling __3d55726e_register_with_polling
#define request __17160575_request
#define reset_total_charge __76560a27_reset_total_charge
#define selftest __07001f12_selftest
#define sequence_running __27100919_sequence_running
#define sessionLog __756d1d3f_sessionLog
#define set_socket_port_for_sim __1f0a2344_set_socket_port_for_sim
#define snd_FW_version __3f254840_snd_FW_version
#define snd_HW_version __31254840_snd_HW_version
#define snd_alternate __056c714e_snd_alternate
#define snd_coeff_cs1 __475e624a_snd_coeff_cs1
#define snd_coeff_cs2 __445e624a_snd_coeff_cs2
#define snd_coeff_i __765e6839_snd_coeff_i
#define snd_coeff_r __765e7339_snd_coeff_r
#define snd_coeff_v __765e7739_snd_coeff_v
#define snd_current_n_duration __0625726e_snd_current_n_duration
#define snd_dio_enabled __1e0c0e62_snd_dio_enabled
#define snd_do1_function __051d540d_snd_do1_function
#define snd_do1_post_time __5d1a4f19_snd_do1_post_time
#define snd_do1_pre_time __131a5d3a_snd_do1_pre_time
#define snd_do2_function __051d570d_snd_do2_function
#define snd_do2_post_time __5d1a4c19_snd_do2_post_time
#define snd_do2_pre_time __131a5e3a_snd_do2_pre_time
#define snd_fade_in_time __3e0f0300_snd_fade_in_time
#define snd_fade_out_time __70141c23_snd_fade_out_time
#define snd_fade_type __2f7b794a_snd_fade_type
#define snd_highvoltage_led __606a6e1c_snd_highvoltage_led
#define snd_identifier __0b116758_snd_identifier
#define snd_interphase __1b15785b_snd_interphase
#define snd_o_function __1d3c7643_snd_o_function
#define snd_phase_current1 __601a0337_snd_phase_current1
#define snd_phase_current2 __60190337_snd_phase_current2
#define snd_phase_duration1 __7b562430_snd_phase_duration1
#define snd_phase_duration2 __7b562730_snd_phase_duration2
#define snd_pulse_rate __12217a4d_snd_pulse_rate
#define snd_pulses __6668082c_snd_pulses
#define snd_serial __61671636_snd_serial
#define snd_state __651a052b_snd_state
#define snd_total_charge __0a2c1433_snd_total_charge
#define snd_voltage_limit __7c0f001d_snd_voltage_limit
#define st_FW_version __3858404c_st_FW_version
#define st_HW_version __38584042_st_HW_version
#define st_alternate __71614e76_st_alternate
#define st_coeff_cs1 __43724a34_st_coeff_cs1
#define st_coeff_cs2 __43724a37_st_coeff_cs2
#define st_coeff_i __43783905_st_coeff_i
#define st_coeff_r __43633905_st_coeff_r
#define st_coeff_v __43673905_st_coeff_v
#define st_default __7a663e11_st_default
#define st_defaults __7a664d11_st_defaults
#define st_dio_enabled __111e626d_st_dio_enabled
#define st_do1_function __00440d76_st_do1_function
#define st_do1_post_time __075f192e_st_do1_post_time
#define st_do1_pre_time __074d3a60_st_do1_pre_time
#define st_do2_function __00470d76_st_do2_function
#define st_do2_post_time __075c192e_st_do2_post_time
#define st_do2_pre_time __074e3a60_st_do2_pre_time
#define st_el_circ_diameter __704c7c0d_st_el_circ_diameter
#define st_el_depth_diameter __6e425740_st_el_depth_diameter
#define st_el_depth_length __77523d38_st_el_depth_length
#define st_el_other_surface __65585528_st_el_other_surface
#define st_el_type __6f4e2b1c_st_el_type
#define st_fade_in_time __1213004d_st_fade_in_time
#define st_fade_out_time __090c2303_st_fade_out_time
#define st_fade_type __66694a5c_st_fade_type
#define st_highvoltage_led __777e1c13_st_highvoltage_led
#define st_identifier __0c775878_st_identifier
#define st_interphase __08685b68_st_interphase
#define st_jitter __68002b0f_st_jitter
#define st_maximum __67613600_st_maximum
#define st_minimum __6f773600_st_minimum
#define st_n_trains __456e5e0f_st_n_trains
#define st_o_function __2166436e_st_o_function
#define st_operationmode __1a172605_st_operationmode
#define st_phase_current1 __07133713_st_phase_current1
#define st_phase_current2 __04133713_st_phase_current2
#define st_phase_duration1 __4b343008_st_phase_duration1
#define st_phase_duration2 __4b373008_st_phase_duration2
#define st_pulse_period __30072d67_st_pulse_period
#define st_pulse_rate __3c6a4d61_st_pulse_rate
#define st_pulses __75182c15_st_pulses
#define st_sequence_duration_by_n_trains __382e3118_st_sequence_duration_by_n_trains
#define st_sequence_duration_by_train_rate __59503811_st_sequence_duration_by_train_rate
#define st_serial __7a063612_st_serial
#define st_state __07152b16_st_state
#define st_total_charge __31043379_st_total_charge
#define st_train_duration_by_pulse_rate __1c3b0760_st_train_duration_by_pulse_rate
#define st_train_duration_by_pulses __7d4f4e12_st_train_duration_by_pulses
#define st_train_period __370a3768_st_train_period
#define st_train_rate __3b67576e_st_train_rate
#define st_voltage_limit __12101d0f_st_voltage_limit
#define start_sequence __153b7779_start_sequence
#define stop __73746f70_stop
#define toggle_debuglog __11072a64_toggle_debuglog
#define unregister_with_polling __07003d55_unregister_with_polling
#define wait_for_end_of_sequence __3e061c2d_wait_for_end_of_sequence

#define __declspec(x)
#endif // STATIC_LIBGESTIMAPI

#include "gEstimAPI.h"

#ifndef STATIC_LIBGESTIMAPI
#undef __declspec
// Use #undef to restore function names before declaring
// function pointers with the names originally used to
// declare imports.
#undef abort_sequence
extern decltype(&__072d7979_abort_sequence) abort_sequence;
#undef activate
extern decltype(&__1702000c_activate) activate;
#undef calibrate2
extern decltype(&__64210d1d_calibrate2) calibrate2;
#undef closeDevice
extern decltype(&__6f4b6f05_closeDevice) closeDevice;
#undef closeDevices
extern decltype(&__6f4b6f76_closeDevices) closeDevices;
#undef close_disconnect_sm
extern decltype(&__44466a00_close_disconnect_sm) close_disconnect_sm;
#undef clr_last_error
extern decltype(&__3f1a7359_clr_last_error) clr_last_error;
#undef count_openDevices
extern decltype(&__77373d1e_count_openDevices) count_openDevices;
#undef debugLog
extern decltype(&__03290d12_debugLog) debugLog;
#undef deviceReady
extern decltype(&__66645d0c_deviceReady) deviceReady;
#undef error_field
extern decltype(&__72417006_error_field) error_field;
#undef error_fields
extern decltype(&__72417075_error_fields) error_fields;
#undef error_state_change
extern decltype(&__7254052a_error_state_change) error_state_change;
#undef fw_hw_ok
extern decltype(&__11283003_fw_hw_ok) fw_hw_ok;
#undef gt_CrtVersion
extern decltype(&__09736049_gt_CrtVersion) gt_CrtVersion;
#undef gt_FW_version
extern decltype(&__2c58404c_gt_FW_version) gt_FW_version;
#undef gt_HW_version
extern decltype(&__2c584042_gt_HW_version) gt_HW_version;
#undef gt_alternate
extern decltype(&__65614e76_gt_alternate) gt_alternate;
#undef gt_alternate_default
extern decltype(&__5b704764_gt_alternate_default) gt_alternate_default;
#undef gt_alternate_max
extern decltype(&__3a0c2f0e_gt_alternate_max) gt_alternate_max;
#undef gt_alternate_min
extern decltype(&__3a0c2718_gt_alternate_min) gt_alternate_min;
#undef gt_applied_pulses
extern decltype(&__740c1f1d_gt_applied_pulses) gt_applied_pulses;
#undef gt_applied_trains
extern decltype(&__73010512_gt_applied_trains) gt_applied_trains;
#undef gt_battery1
extern decltype(&__74791a07_gt_battery1) gt_battery1;
#undef gt_battery2
extern decltype(&__74791907_gt_battery2) gt_battery2;
#undef gt_battery_pc
extern decltype(&__17797477_gt_battery_pc) gt_battery_pc;
#undef gt_charge_density
extern decltype(&__7d392015_gt_charge_density) gt_charge_density;
#undef gt_coeff_cs1
extern decltype(&__57724a34_gt_coeff_cs1) gt_coeff_cs1;
#undef gt_coeff_cs2
extern decltype(&__57724a37_gt_coeff_cs2) gt_coeff_cs2;
#undef gt_coeff_i
extern decltype(&__57783905_gt_coeff_i) gt_coeff_i;
#undef gt_coeff_r
extern decltype(&__57633905_gt_coeff_r) gt_coeff_r;
#undef gt_coeff_v
extern decltype(&__57673905_gt_coeff_v) gt_coeff_v;
#undef gt_current
extern decltype(&__7c722d06_gt_current) gt_current;
#undef gt_current1_avg
extern decltype(&__1d047b59_gt_current1_avg) gt_current1_avg;
#undef gt_current1_sum
extern decltype(&__0f077159_gt_current1_sum) gt_current1_sum;
#undef gt_current2_avg
extern decltype(&__1d047859_gt_current2_avg) gt_current2_avg;
#undef gt_current2_sum
extern decltype(&__0f077259_gt_current2_sum) gt_current2_sum;
#undef gt_di_state
extern decltype(&__6f5f4910_gt_di_state) gt_di_state;
#undef gt_dio_enabled
extern decltype(&__051e626d_gt_dio_enabled) gt_dio_enabled;
#undef gt_dio_enabled_default
extern decltype(&__0c0c5c7c_gt_dio_enabled_default) gt_dio_enabled_default;
#undef gt_dio_enabled_max
extern decltype(&__64663d00_gt_dio_enabled_max) gt_dio_enabled_max;
#undef gt_dio_enabled_min
extern decltype(&__6c703d00_gt_dio_enabled_min) gt_dio_enabled_min;
#undef gt_do1_function
extern decltype(&__14440d76_gt_do1_function) gt_do1_function;
#undef gt_do1_function_default
extern decltype(&__054d1f48_gt_do1_function_default) gt_do1_function_default;
#undef gt_do1_function_max
extern decltype(&__79257529_gt_do1_function_max) gt_do1_function_max;
#undef gt_do1_function_min
extern decltype(&__792d6329_gt_do1_function_min) gt_do1_function_min;
#undef gt_do1_post_time
extern decltype(&__135f192e_gt_do1_post_time) gt_do1_post_time;
#undef gt_do1_post_time_default
extern decltype(&__2d4e103c_gt_do1_post_time_default) gt_do1_post_time_default;
#undef gt_do1_post_time_max
extern decltype(&__4c327856_gt_do1_post_time_max) gt_do1_post_time_max;
#undef gt_do1_post_time_min
extern decltype(&__4c327040_gt_do1_post_time_min) gt_do1_post_time_min;
#undef gt_do1_pre_time
extern decltype(&__134d3a60_gt_do1_pre_time) gt_do1_pre_time;
#undef gt_do1_pre_time_default
extern decltype(&__0244285e_gt_do1_pre_time_default) gt_do1_pre_time_default;
#undef gt_do1_pre_time_max
extern decltype(&__7e2c423f_gt_do1_pre_time_max) gt_do1_pre_time_max;
#undef gt_do1_pre_time_min
extern decltype(&__7e24543f_gt_do1_pre_time_min) gt_do1_pre_time_min;
#undef gt_do2_function
extern decltype(&__14470d76_gt_do2_function) gt_do2_function;
#undef gt_do2_function_default
extern decltype(&__054e1f48_gt_do2_function_default) gt_do2_function_default;
#undef gt_do2_function_max
extern decltype(&__79267529_gt_do2_function_max) gt_do2_function_max;
#undef gt_do2_function_min
extern decltype(&__792e6329_gt_do2_function_min) gt_do2_function_min;
#undef gt_do2_post_time
extern decltype(&__135c192e_gt_do2_post_time) gt_do2_post_time;
#undef gt_do2_post_time_default
extern decltype(&__2d4d103c_gt_do2_post_time_default) gt_do2_post_time_default;
#undef gt_do2_post_time_max
extern decltype(&__4c317856_gt_do2_post_time_max) gt_do2_post_time_max;
#undef gt_do2_post_time_min
extern decltype(&__4c317040_gt_do2_post_time_min) gt_do2_post_time_min;
#undef gt_do2_pre_time
extern decltype(&__134e3a60_gt_do2_pre_time) gt_do2_pre_time;
#undef gt_do2_pre_time_default
extern decltype(&__0247285e_gt_do2_pre_time_default) gt_do2_pre_time_default;
#undef gt_do2_pre_time_max
extern decltype(&__7e2f423f_gt_do2_pre_time_max) gt_do2_pre_time_max;
#undef gt_do2_pre_time_min
extern decltype(&__7e27543f_gt_do2_pre_time_min) gt_do2_pre_time_min;
#undef gt_duration1_sum
extern decltype(&__241a254c_gt_duration1_sum) gt_duration1_sum;
#undef gt_duration2_sum
extern decltype(&__241a254f_gt_duration2_sum) gt_duration2_sum;
#undef gt_el_calc_surface
extern decltype(&__715f0516_gt_el_calc_surface) gt_el_calc_surface;
#undef gt_el_circ_diameter
extern decltype(&__644c7c0d_gt_el_circ_diameter) gt_el_circ_diameter;
#undef gt_el_circ_diameter_default
extern decltype(&__75456e33_gt_el_circ_diameter_default) gt_el_circ_diameter_default;
#undef gt_el_circ_diameter_max
extern decltype(&__092d0452_gt_el_circ_diameter_max) gt_el_circ_diameter_max;
#undef gt_el_circ_diameter_min
extern decltype(&__09251252_gt_el_circ_diameter_min) gt_el_circ_diameter_min;
#undef gt_el_depth_diameter
extern decltype(&__7a425740_gt_el_depth_diameter) gt_el_depth_diameter;
#undef gt_el_depth_diameter_default
extern decltype(&__44535e52_gt_el_depth_diameter_default) gt_el_depth_diameter_default;
#undef gt_el_depth_diameter_max
extern decltype(&__252f3638_gt_el_depth_diameter_max) gt_el_depth_diameter_max;
#undef gt_el_depth_diameter_min
extern decltype(&__252f3e2e_gt_el_depth_diameter_min) gt_el_depth_diameter_min;
#undef gt_el_depth_length
extern decltype(&__63523d38_gt_el_depth_length) gt_el_depth_length;
#undef gt_el_depth_length_default
extern decltype(&__6a400329_gt_el_depth_length_default) gt_el_depth_length_default;
#undef gt_el_depth_length_max
extern decltype(&__022a6255_gt_el_depth_length_max) gt_el_depth_length_max;
#undef gt_el_depth_length_min
extern decltype(&__0a3c6255_gt_el_depth_length_min) gt_el_depth_length_min;
#undef gt_el_other_surface
extern decltype(&__71585528_gt_el_other_surface) gt_el_other_surface;
#undef gt_el_other_surface_default
extern decltype(&__60514716_gt_el_other_surface_default) gt_el_other_surface_default;
#undef gt_el_other_surface_max
extern decltype(&__1c392d77_gt_el_other_surface_max) gt_el_other_surface_max;
#undef gt_el_other_surface_min
extern decltype(&__1c313b77_gt_el_other_surface_min) gt_el_other_surface_min;
#undef gt_el_type
extern decltype(&__7b4e2b1c_gt_el_type) gt_el_type;
#undef gt_el_type_default
extern decltype(&__725c150d_gt_el_type_default) gt_el_type_default;
#undef gt_el_type_max
extern decltype(&__1a367471_gt_el_type_max) gt_el_type_max;
#undef gt_el_type_min
extern decltype(&__12207471_gt_el_type_min) gt_el_type_min;
#undef gt_error
extern decltype(&__15063017_gt_error) gt_error;
#undef gt_error_text
extern decltype(&__3e72556f_gt_error_text) gt_error_text;
#undef gt_event
extern decltype(&__11113111_gt_event) gt_event;
#undef gt_fade_factor
extern decltype(&__0f03594d_gt_fade_factor) gt_fade_factor;
#undef gt_fade_in_pulses
extern decltype(&__6912162c_gt_fade_in_pulses) gt_fade_in_pulses;
#undef gt_fade_in_time
extern decltype(&__0613004d_gt_fade_in_time) gt_fade_in_time;
#undef gt_fade_in_time_default
extern decltype(&__171a1273_gt_fade_in_time_default) gt_fade_in_time_default;
#undef gt_fade_in_time_max
extern decltype(&__6b727812_gt_fade_in_time_max) gt_fade_in_time_max;
#undef gt_fade_in_time_min
extern decltype(&__6b7a6e12_gt_fade_in_time_min) gt_fade_in_time_min;
#undef gt_fade_out_pulses
extern decltype(&__7c632215_gt_fade_out_pulses) gt_fade_out_pulses;
#undef gt_fade_out_time
extern decltype(&__1d0c2303_gt_fade_out_time) gt_fade_out_time;
#undef gt_fade_out_time_default
extern decltype(&__231d2a11_gt_fade_out_time_default) gt_fade_out_time_default;
#undef gt_fade_out_time_max
extern decltype(&__4261427b_gt_fade_out_time_max) gt_fade_out_time_max;
#undef gt_fade_out_time_min
extern decltype(&__42614a6d_gt_fade_out_time_min) gt_fade_out_time_min;
#undef gt_fade_progress
extern decltype(&__0407262d_gt_fade_progress) gt_fade_progress;
#undef gt_fade_type
extern decltype(&__72694a5c_gt_fade_type) gt_fade_type;
#undef gt_fade_type_default
extern decltype(&__4c78434e_gt_fade_type_default) gt_fade_type_default;
#undef gt_fade_type_max
extern decltype(&__2d042b24_gt_fade_type_max) gt_fade_type_max;
#undef gt_fade_type_min
extern decltype(&__2d042332_gt_fade_type_min) gt_fade_type_min;
#undef gt_ftdi_driver_version
extern decltype(&__180c7350_gt_ftdi_driver_version) gt_ftdi_driver_version;
#undef gt_ftdi_library_version
extern decltype(&__01013167_gt_ftdi_library_version) gt_ftdi_library_version;
#undef gt_highvoltage_led
extern decltype(&__637e1c13_gt_highvoltage_led) gt_highvoltage_led;
#undef gt_highvoltage_led_default
extern decltype(&__6a6c2202_gt_highvoltage_led_default) gt_highvoltage_led_default;
#undef gt_highvoltage_led_max
extern decltype(&__0206437e_gt_highvoltage_led_max) gt_highvoltage_led_max;
#undef gt_highvoltage_led_min
extern decltype(&__0a10437e_gt_highvoltage_led_min) gt_highvoltage_led_min;
#undef gt_identifier
extern decltype(&__18775878_gt_identifier) gt_identifier;
#undef gt_interphase
extern decltype(&__1c685b68_gt_interphase) gt_interphase;
#undef gt_interphase_default
extern decltype(&__0e564a61_gt_interphase_default) gt_interphase_default;
#undef gt_interphase_max
extern decltype(&__64373609_gt_interphase_max) gt_interphase_max;
#undef gt_interphase_min
extern decltype(&__72373601_gt_interphase_min) gt_interphase_min;
#undef gt_jitter
extern decltype(&__7c002b0f_gt_jitter) gt_jitter;
#undef gt_jitter_default
extern decltype(&__6e3e3a06_gt_jitter_default) gt_jitter_default;
#undef gt_jitter_max
extern decltype(&__045f466e_gt_jitter_max) gt_jitter_max;
#undef gt_jitter_min
extern decltype(&__125f4666_gt_jitter_min) gt_jitter_min;
#undef gt_last_error
extern decltype(&__1175595c_gt_last_error) gt_last_error;
#undef gt_last_resistance
extern decltype(&__64733934_gt_last_resistance) gt_last_resistance;
#undef gt_max_current_ph1
extern decltype(&__753b2d1b_gt_max_current_ph1) gt_max_current_ph1;
#undef gt_max_current_ph2
extern decltype(&__75382d1b_gt_max_current_ph2) gt_max_current_ph2;
#undef gt_min_current_ph1
extern decltype(&__7d2d2d1b_gt_min_current_ph1) gt_min_current_ph1;
#undef gt_min_current_ph2
extern decltype(&__7d2e2d1b_gt_min_current_ph2) gt_min_current_ph2;
#undef gt_n_trains
extern decltype(&__516e5e0f_gt_n_trains) gt_n_trains;
#undef gt_n_trains_default
extern decltype(&__40674c31_gt_n_trains_default) gt_n_trains_default;
#undef gt_n_trains_max
extern decltype(&__3c0f2650_gt_n_trains_max) gt_n_trains_max;
#undef gt_n_trains_min
extern decltype(&__3c073050_gt_n_trains_min) gt_n_trains_min;
#undef gt_o_function
extern decltype(&__3566436e_gt_o_function) gt_o_function;
#undef gt_o_function_default
extern decltype(&__27585267_gt_o_function_default) gt_o_function_default;
#undef gt_o_function_max
extern decltype(&__4d392e0f_gt_o_function_max) gt_o_function_max;
#undef gt_o_function_min
extern decltype(&__5b392e07_gt_o_function_min) gt_o_function_min;
#undef gt_openDevices
extern decltype(&__17145848_gt_openDevices) gt_openDevices;
#undef gt_operationmode
extern decltype(&__0e172605_gt_operationmode) gt_operationmode;
#undef gt_operationmode_default
extern decltype(&__30062f17_gt_operationmode_default) gt_operationmode_default;
#undef gt_operationmode_max
extern decltype(&__517a477d_gt_operationmode_max) gt_operationmode_max;
#undef gt_operationmode_min
extern decltype(&__517a4f6b_gt_operationmode_min) gt_operationmode_min;
#undef gt_periods
extern decltype(&__6675361f_gt_periods) gt_periods;
#undef gt_phase_charge
extern decltype(&__22112174_gt_phase_charge) gt_phase_charge;
#undef gt_phase_current1
extern decltype(&__13133713_gt_phase_current1) gt_phase_current1;
#undef gt_phase_current1_default
extern decltype(&__012d261a_gt_phase_current1_default) gt_phase_current1_default;
#undef gt_phase_current1_max
extern decltype(&__6b4c5a72_gt_phase_current1_max) gt_phase_current1_max;
#undef gt_phase_current1_min
extern decltype(&__7d4c5a7a_gt_phase_current1_min) gt_phase_current1_min;
#undef gt_phase_current2
extern decltype(&__10133713_gt_phase_current2) gt_phase_current2;
#undef gt_phase_current2_default
extern decltype(&__022d261a_gt_phase_current2_default) gt_phase_current2_default;
#undef gt_phase_current2_max
extern decltype(&__684c5a72_gt_phase_current2_max) gt_phase_current2_max;
#undef gt_phase_current2_min
extern decltype(&__7e4c5a7a_gt_phase_current2_min) gt_phase_current2_min;
#undef gt_phase_duration1
extern decltype(&__5f343008_gt_phase_duration1) gt_phase_duration1;
#undef gt_phase_duration1_default
extern decltype(&__56260e19_gt_phase_duration1_default) gt_phase_duration1_default;
#undef gt_phase_duration1_max
extern decltype(&__3e4c6f65_gt_phase_duration1_max) gt_phase_duration1_max;
#undef gt_phase_duration1_min
extern decltype(&__365a6f65_gt_phase_duration1_min) gt_phase_duration1_min;
#undef gt_phase_duration2
extern decltype(&__5f373008_gt_phase_duration2) gt_phase_duration2;
#undef gt_phase_duration2_default
extern decltype(&__56250e19_gt_phase_duration2_default) gt_phase_duration2_default;
#undef gt_phase_duration2_max
extern decltype(&__3e4f6f65_gt_phase_duration2_max) gt_phase_duration2_max;
#undef gt_phase_duration2_min
extern decltype(&__36596f65_gt_phase_duration2_min) gt_phase_duration2_min;
#undef gt_pulse_duration
extern decltype(&__42083008_gt_pulse_duration) gt_pulse_duration;
#undef gt_pulse_period
extern decltype(&__24072d67_gt_pulse_period) gt_pulse_period;
#undef gt_pulse_rate
extern decltype(&__286a4d61_gt_pulse_rate) gt_pulse_rate;
#undef gt_pulse_rate_default
extern decltype(&__3a545c68_gt_pulse_rate_default) gt_pulse_rate_default;
#undef gt_pulse_rate_max
extern decltype(&__50352000_gt_pulse_rate_max) gt_pulse_rate_max;
#undef gt_pulse_rate_min
extern decltype(&__46352008_gt_pulse_rate_min) gt_pulse_rate_min;
#undef gt_pulses
extern decltype(&__61182c15_gt_pulses) gt_pulses;
#undef gt_pulses_default
extern decltype(&__73263d1c_gt_pulses_default) gt_pulses_default;
#undef gt_pulses_max
extern decltype(&__19474174_gt_pulses_max) gt_pulses_max;
#undef gt_pulses_min
extern decltype(&__0f47417c_gt_pulses_min) gt_pulses_min;
#undef gt_remaining
extern decltype(&__6c70507c_gt_remaining) gt_remaining;
#undef gt_resistance
extern decltype(&__13665862_gt_resistance) gt_resistance;
#undef gt_selftest_passed
extern decltype(&__726e3e2b_gt_selftest_passed) gt_selftest_passed;
#undef gt_sequence_duration
extern decltype(&__7c7a5246_gt_sequence_duration) gt_sequence_duration;
#undef gt_serial
extern decltype(&__6e063612_gt_serial) gt_serial;
#undef gt_sessionLogFile
extern decltype(&__6f2f0919_gt_sessionLogFile) gt_sessionLogFile;
#undef gt_state
extern decltype(&__13152b16_gt_state) gt_state;
#undef gt_stim_event
extern decltype(&__026b5742_gt_stim_event) gt_stim_event;
#undef gt_test_resistance
extern decltype(&__6073392c_gt_test_resistance) gt_test_resistance;
#undef gt_total_charge
extern decltype(&__25043379_gt_total_charge) gt_total_charge;
#undef gt_total_charge_default
extern decltype(&__340d2147_gt_total_charge_default) gt_total_charge_default;
#undef gt_total_charge_max
extern decltype(&__48654b26_gt_total_charge_max) gt_total_charge_max;
#undef gt_total_charge_min
extern decltype(&__486d5d26_gt_total_charge_min) gt_total_charge_min;
#undef gt_train_charge
extern decltype(&__38113b7b_gt_train_charge) gt_train_charge;
#undef gt_train_duration
extern decltype(&__45052a07_gt_train_duration) gt_train_duration;
#undef gt_train_period
extern decltype(&__230a3768_gt_train_period) gt_train_period;
#undef gt_train_rate
extern decltype(&__2f67576e_gt_train_rate) gt_train_rate;
#undef gt_train_rate_default
extern decltype(&__3d594667_gt_train_rate_default) gt_train_rate_default;
#undef gt_train_rate_max
extern decltype(&__57383a0f_gt_train_rate_max) gt_train_rate_max;
#undef gt_train_rate_min
extern decltype(&__41383a07_gt_train_rate_min) gt_train_rate_min;
#undef gt_voltage
extern decltype(&__6f7d2b17_gt_voltage) gt_voltage;
#undef gt_voltage1_avg
extern decltype(&__0e0b7d48_gt_voltage1_avg) gt_voltage1_avg;
#undef gt_voltage1_sum
extern decltype(&__1c087748_gt_voltage1_sum) gt_voltage1_sum;
#undef gt_voltage2_avg
extern decltype(&__0e0b7e48_gt_voltage2_avg) gt_voltage2_avg;
#undef gt_voltage2_sum
extern decltype(&__1c087448_gt_voltage2_sum) gt_voltage2_sum;
#undef gt_voltage_limit
extern decltype(&__06101d0f_gt_voltage_limit) gt_voltage_limit;
#undef gt_voltage_limit_default
extern decltype(&__3801141d_gt_voltage_limit_default) gt_voltage_limit_default;
#undef gt_voltage_limit_max
extern decltype(&__597d7c77_gt_voltage_limit_max) gt_voltage_limit_max;
#undef gt_voltage_limit_min
extern decltype(&__597d7461_gt_voltage_limit_min) gt_voltage_limit_min;
#undef has_dongle
extern decltype(&__606b1d38_has_dongle) has_dongle;
#undef isOpenDevice
extern decltype(&__7a746870_isOpenDevice) isOpenDevice;
#undef is_FES_serial
extern decltype(&__25526954_is_FES_serial) is_FES_serial;
#undef measure_resistance
extern decltype(&__7a63162b_measure_resistance) measure_resistance;
#undef newSessionLog
extern decltype(&__03784855_newSessionLog) newSessionLog;
#undef openDevice
extern decltype(&__48701307_openDevice) openDevice;
#undef openDevices
extern decltype(&__48706007_openDevices) openDevices;
#undef open_connect_sm
extern decltype(&__01050474_open_connect_sm) open_connect_sm;
#undef rcv_FW_version
extern decltype(&__3e285a40_rcv_FW_version) rcv_FW_version;
#undef rcv_HW_version
extern decltype(&__30285a40_rcv_HW_version) rcv_HW_version;
#undef rcv_all
extern decltype(&__130f1a5f_rcv_all) rcv_all;
#undef rcv_alternate
extern decltype(&__0461634e_rcv_alternate) rcv_alternate;
#undef rcv_applied_pulses
extern decltype(&__6f700e1f_rcv_applied_pulses) rcv_applied_pulses;
#undef rcv_battery1
extern decltype(&__75707b1a_rcv_battery1) rcv_battery1;
#undef rcv_battery2
extern decltype(&__75707b19_rcv_battery2) rcv_battery2;
#undef rcv_coeff_cs1
extern decltype(&__4653704a_rcv_coeff_cs1) rcv_coeff_cs1;
#undef rcv_coeff_cs2
extern decltype(&__4553704a_rcv_coeff_cs2) rcv_coeff_cs2;
#undef rcv_coeff_i
extern decltype(&__77537a39_rcv_coeff_i) rcv_coeff_i;
#undef rcv_coeff_r
extern decltype(&__77536139_rcv_coeff_r) rcv_coeff_r;
#undef rcv_coeff_v
extern decltype(&__77536539_rcv_coeff_v) rcv_coeff_v;
#undef rcv_current
extern decltype(&__7478702d_rcv_current) rcv_current;
#undef rcv_current1_sum
extern decltype(&__2b0b0571_rcv_current1_sum) rcv_current1_sum;
#undef rcv_current2_sum
extern decltype(&__2b0b0572_rcv_current2_sum) rcv_current2_sum;
#undef rcv_di_state
extern decltype(&__626b5d49_rcv_di_state) rcv_di_state;
#undef rcv_dio_enabled
extern decltype(&__1f011c62_rcv_dio_enabled) rcv_dio_enabled;
#undef rcv_do1_function
extern decltype(&__0410460d_rcv_do1_function) rcv_do1_function;
#undef rcv_do1_post_time
extern decltype(&__5c175d19_rcv_do1_post_time) rcv_do1_post_time;
#undef rcv_do1_pre_time
extern decltype(&__12174f3a_rcv_do1_pre_time) rcv_do1_pre_time;
#undef rcv_do2_function
extern decltype(&__0410450d_rcv_do2_function) rcv_do2_function;
#undef rcv_do2_post_time
extern decltype(&__5c175e19_rcv_do2_post_time) rcv_do2_post_time;
#undef rcv_do2_pre_time
extern decltype(&__12174c3a_rcv_do2_pre_time) rcv_do2_pre_time;
#undef rcv_duration1_sum
extern decltype(&__3e201825_rcv_duration1_sum) rcv_duration1_sum;
#undef rcv_duration2_sum
extern decltype(&__3d201825_rcv_duration2_sum) rcv_duration2_sum;
#undef rcv_error
extern decltype(&__65110430_rcv_error) rcv_error;
#undef rcv_event
extern decltype(&__63151331_rcv_event) rcv_event;
#undef rcv_fade_in_time
extern decltype(&__3f021100_rcv_fade_in_time) rcv_fade_in_time;
#undef rcv_fade_out_time
extern decltype(&__71190e23_rcv_fade_out_time) rcv_fade_out_time;
#undef rcv_fade_progress
extern decltype(&__5f000526_rcv_fade_progress) rcv_fade_progress;
#undef rcv_fade_type
extern decltype(&__2e766b4a_rcv_fade_type) rcv_fade_type;
#undef rcv_highvoltage_led
extern decltype(&__61677c1c_rcv_highvoltage_led) rcv_highvoltage_led;
#undef rcv_identifier
extern decltype(&__0a1c7558_rcv_identifier) rcv_identifier;
#undef rcv_interphase
extern decltype(&__1a186a5b_rcv_interphase) rcv_interphase;
#undef rcv_max_current_ph1
extern decltype(&__6971392d_rcv_max_current_ph1) rcv_max_current_ph1;
#undef rcv_max_current_ph2
extern decltype(&__69713a2d_rcv_max_current_ph2) rcv_max_current_ph2;
#undef rcv_min_current_ph1
extern decltype(&__69792f2d_rcv_min_current_ph1) rcv_min_current_ph1;
#undef rcv_min_current_ph2
extern decltype(&__69792c2d_rcv_min_current_ph2) rcv_min_current_ph2;
#undef rcv_o_function
extern decltype(&__1c316443_rcv_o_function) rcv_o_function;
#undef rcv_phase_current1
extern decltype(&__61171137_rcv_phase_current1) rcv_phase_current1;
#undef rcv_phase_current2
extern decltype(&__61141137_rcv_phase_current2) rcv_phase_current2;
#undef rcv_phase_duration1
extern decltype(&__7a5b3630_rcv_phase_duration1) rcv_phase_duration1;
#undef rcv_phase_duration2
extern decltype(&__7a5b3530_rcv_phase_duration2) rcv_phase_duration2;
#undef rcv_pulse_rate
extern decltype(&__132c684d_rcv_pulse_rate) rcv_pulse_rate;
#undef rcv_pulses
extern decltype(&__67651a2c_rcv_pulses) rcv_pulses;
#undef rcv_resistance
extern decltype(&__10176458_rcv_resistance) rcv_resistance;
#undef rcv_samples
extern decltype(&__6d67682f_rcv_samples) rcv_samples;
#undef rcv_serial
extern decltype(&__606a0436_rcv_serial) rcv_serial;
#undef rcv_state
extern decltype(&__6417172b_rcv_state) rcv_state;
#undef rcv_stim
extern decltype(&__01171f32_rcv_stim) rcv_stim;
#undef rcv_stim_event
extern decltype(&__30066957_rcv_stim_event) rcv_stim_event;
#undef rcv_total_charge
extern decltype(&__0b210633_rcv_total_charge) rcv_total_charge;
#undef rcv_voltage
extern decltype(&__656b7f2b_rcv_voltage) rcv_voltage;
#undef rcv_voltage1_sum
extern decltype(&__3a180a77_rcv_voltage1_sum) rcv_voltage1_sum;
#undef rcv_voltage2_sum
extern decltype(&__3a180a74_rcv_voltage2_sum) rcv_voltage2_sum;
#undef rcv_voltage_limit
extern decltype(&__7d02121d_rcv_voltage_limit) rcv_voltage_limit;
#undef register_with_polling
extern decltype(&__3d55726e_register_with_polling) register_with_polling;
#undef request
extern decltype(&__17160575_request) request;
#undef reset_total_charge
extern decltype(&__76560a27_reset_total_charge) reset_total_charge;
#undef selftest
extern decltype(&__07001f12_selftest) selftest;
#undef sequence_running
extern decltype(&__27100919_sequence_running) sequence_running;
#undef sessionLog
extern decltype(&__756d1d3f_sessionLog) sessionLog;
#undef set_socket_port_for_sim
extern decltype(&__1f0a2344_set_socket_port_for_sim) set_socket_port_for_sim;
#undef snd_FW_version
extern decltype(&__3f254840_snd_FW_version) snd_FW_version;
#undef snd_HW_version
extern decltype(&__31254840_snd_HW_version) snd_HW_version;
#undef snd_alternate
extern decltype(&__056c714e_snd_alternate) snd_alternate;
#undef snd_coeff_cs1
extern decltype(&__475e624a_snd_coeff_cs1) snd_coeff_cs1;
#undef snd_coeff_cs2
extern decltype(&__445e624a_snd_coeff_cs2) snd_coeff_cs2;
#undef snd_coeff_i
extern decltype(&__765e6839_snd_coeff_i) snd_coeff_i;
#undef snd_coeff_r
extern decltype(&__765e7339_snd_coeff_r) snd_coeff_r;
#undef snd_coeff_v
extern decltype(&__765e7739_snd_coeff_v) snd_coeff_v;
#undef snd_current_n_duration
extern decltype(&__0625726e_snd_current_n_duration) snd_current_n_duration;
#undef snd_dio_enabled
extern decltype(&__1e0c0e62_snd_dio_enabled) snd_dio_enabled;
#undef snd_do1_function
extern decltype(&__051d540d_snd_do1_function) snd_do1_function;
#undef snd_do1_post_time
extern decltype(&__5d1a4f19_snd_do1_post_time) snd_do1_post_time;
#undef snd_do1_pre_time
extern decltype(&__131a5d3a_snd_do1_pre_time) snd_do1_pre_time;
#undef snd_do2_function
extern decltype(&__051d570d_snd_do2_function) snd_do2_function;
#undef snd_do2_post_time
extern decltype(&__5d1a4c19_snd_do2_post_time) snd_do2_post_time;
#undef snd_do2_pre_time
extern decltype(&__131a5e3a_snd_do2_pre_time) snd_do2_pre_time;
#undef snd_fade_in_time
extern decltype(&__3e0f0300_snd_fade_in_time) snd_fade_in_time;
#undef snd_fade_out_time
extern decltype(&__70141c23_snd_fade_out_time) snd_fade_out_time;
#undef snd_fade_type
extern decltype(&__2f7b794a_snd_fade_type) snd_fade_type;
#undef snd_highvoltage_led
extern decltype(&__606a6e1c_snd_highvoltage_led) snd_highvoltage_led;
#undef snd_identifier
extern decltype(&__0b116758_snd_identifier) snd_identifier;
#undef snd_interphase
extern decltype(&__1b15785b_snd_interphase) snd_interphase;
#undef snd_o_function
extern decltype(&__1d3c7643_snd_o_function) snd_o_function;
#undef snd_phase_current1
extern decltype(&__601a0337_snd_phase_current1) snd_phase_current1;
#undef snd_phase_current2
extern decltype(&__60190337_snd_phase_current2) snd_phase_current2;
#undef snd_phase_duration1
extern decltype(&__7b562430_snd_phase_duration1) snd_phase_duration1;
#undef snd_phase_duration2
extern decltype(&__7b562730_snd_phase_duration2) snd_phase_duration2;
#undef snd_pulse_rate
extern decltype(&__12217a4d_snd_pulse_rate) snd_pulse_rate;
#undef snd_pulses
extern decltype(&__6668082c_snd_pulses) snd_pulses;
#undef snd_serial
extern decltype(&__61671636_snd_serial) snd_serial;
#undef snd_state
extern decltype(&__651a052b_snd_state) snd_state;
#undef snd_total_charge
extern decltype(&__0a2c1433_snd_total_charge) snd_total_charge;
#undef snd_voltage_limit
extern decltype(&__7c0f001d_snd_voltage_limit) snd_voltage_limit;
#undef st_FW_version
extern decltype(&__3858404c_st_FW_version) st_FW_version;
#undef st_HW_version
extern decltype(&__38584042_st_HW_version) st_HW_version;
#undef st_alternate
extern decltype(&__71614e76_st_alternate) st_alternate;
#undef st_coeff_cs1
extern decltype(&__43724a34_st_coeff_cs1) st_coeff_cs1;
#undef st_coeff_cs2
extern decltype(&__43724a37_st_coeff_cs2) st_coeff_cs2;
#undef st_coeff_i
extern decltype(&__43783905_st_coeff_i) st_coeff_i;
#undef st_coeff_r
extern decltype(&__43633905_st_coeff_r) st_coeff_r;
#undef st_coeff_v
extern decltype(&__43673905_st_coeff_v) st_coeff_v;
#undef st_default
extern decltype(&__7a663e11_st_default) st_default;
#undef st_defaults
extern decltype(&__7a664d11_st_defaults) st_defaults;
#undef st_dio_enabled
extern decltype(&__111e626d_st_dio_enabled) st_dio_enabled;
#undef st_do1_function
extern decltype(&__00440d76_st_do1_function) st_do1_function;
#undef st_do1_post_time
extern decltype(&__075f192e_st_do1_post_time) st_do1_post_time;
#undef st_do1_pre_time
extern decltype(&__074d3a60_st_do1_pre_time) st_do1_pre_time;
#undef st_do2_function
extern decltype(&__00470d76_st_do2_function) st_do2_function;
#undef st_do2_post_time
extern decltype(&__075c192e_st_do2_post_time) st_do2_post_time;
#undef st_do2_pre_time
extern decltype(&__074e3a60_st_do2_pre_time) st_do2_pre_time;
#undef st_el_circ_diameter
extern decltype(&__704c7c0d_st_el_circ_diameter) st_el_circ_diameter;
#undef st_el_depth_diameter
extern decltype(&__6e425740_st_el_depth_diameter) st_el_depth_diameter;
#undef st_el_depth_length
extern decltype(&__77523d38_st_el_depth_length) st_el_depth_length;
#undef st_el_other_surface
extern decltype(&__65585528_st_el_other_surface) st_el_other_surface;
#undef st_el_type
extern decltype(&__6f4e2b1c_st_el_type) st_el_type;
#undef st_fade_in_time
extern decltype(&__1213004d_st_fade_in_time) st_fade_in_time;
#undef st_fade_out_time
extern decltype(&__090c2303_st_fade_out_time) st_fade_out_time;
#undef st_fade_type
extern decltype(&__66694a5c_st_fade_type) st_fade_type;
#undef st_highvoltage_led
extern decltype(&__777e1c13_st_highvoltage_led) st_highvoltage_led;
#undef st_identifier
extern decltype(&__0c775878_st_identifier) st_identifier;
#undef st_interphase
extern decltype(&__08685b68_st_interphase) st_interphase;
#undef st_jitter
extern decltype(&__68002b0f_st_jitter) st_jitter;
#undef st_maximum
extern decltype(&__67613600_st_maximum) st_maximum;
#undef st_minimum
extern decltype(&__6f773600_st_minimum) st_minimum;
#undef st_n_trains
extern decltype(&__456e5e0f_st_n_trains) st_n_trains;
#undef st_o_function
extern decltype(&__2166436e_st_o_function) st_o_function;
#undef st_operationmode
extern decltype(&__1a172605_st_operationmode) st_operationmode;
#undef st_phase_current1
extern decltype(&__07133713_st_phase_current1) st_phase_current1;
#undef st_phase_current2
extern decltype(&__04133713_st_phase_current2) st_phase_current2;
#undef st_phase_duration1
extern decltype(&__4b343008_st_phase_duration1) st_phase_duration1;
#undef st_phase_duration2
extern decltype(&__4b373008_st_phase_duration2) st_phase_duration2;
#undef st_pulse_period
extern decltype(&__30072d67_st_pulse_period) st_pulse_period;
#undef st_pulse_rate
extern decltype(&__3c6a4d61_st_pulse_rate) st_pulse_rate;
#undef st_pulses
extern decltype(&__75182c15_st_pulses) st_pulses;
#undef st_sequence_duration_by_n_trains
extern decltype(&__382e3118_st_sequence_duration_by_n_trains) st_sequence_duration_by_n_trains;
#undef st_sequence_duration_by_train_rate
extern decltype(&__59503811_st_sequence_duration_by_train_rate) st_sequence_duration_by_train_rate;
#undef st_serial
extern decltype(&__7a063612_st_serial) st_serial;
#undef st_state
extern decltype(&__07152b16_st_state) st_state;
#undef st_total_charge
extern decltype(&__31043379_st_total_charge) st_total_charge;
#undef st_train_duration_by_pulse_rate
extern decltype(&__1c3b0760_st_train_duration_by_pulse_rate) st_train_duration_by_pulse_rate;
#undef st_train_duration_by_pulses
extern decltype(&__7d4f4e12_st_train_duration_by_pulses) st_train_duration_by_pulses;
#undef st_train_period
extern decltype(&__370a3768_st_train_period) st_train_period;
#undef st_train_rate
extern decltype(&__3b67576e_st_train_rate) st_train_rate;
#undef st_voltage_limit
extern decltype(&__12101d0f_st_voltage_limit) st_voltage_limit;
#undef start_sequence
extern decltype(&__153b7779_start_sequence) start_sequence;
#undef stop
extern decltype(&__73746f70_stop) stop;
#undef toggle_debuglog
extern decltype(&__11072a64_toggle_debuglog) toggle_debuglog;
#undef unregister_with_polling
extern decltype(&__07003d55_unregister_with_polling) unregister_with_polling;
#undef wait_for_end_of_sequence
extern decltype(&__3e061c2d_wait_for_end_of_sequence) wait_for_end_of_sequence;

#endif // STATIC_LIBGESTIMAPI

namespace Dylib { bool gEstimAPI_Loaded(); }

#endif // GESTIMAPI_IMPORTS_H

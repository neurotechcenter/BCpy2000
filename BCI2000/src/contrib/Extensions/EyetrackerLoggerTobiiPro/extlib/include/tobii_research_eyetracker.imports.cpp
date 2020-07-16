// Import definitions for tobii_research library
// generated Wed Aug 23 09:21:59 2017 by DylibTool
#ifdef STATIC_LIBTOBII_RESEARCH

namespace Dylib { bool tobii_research_Loaded() { return true; } }


#else // STATIC_LIBTOBII_RESEARCH

#include "tobii_research.imports.h"
#include "DylibImports.h"
namespace { extern const Dylib::Import* functionImports; }

// Here you may specify a custom error message to be displayed when the library cannot be found.
static const char* notFoundMsg = "";
// Here you may specify an URL to some local or remote help resource.
static const char* notFoundURL = "";
RegisterDylibWithAliases( tobii_research_eyetracker, "tobii_research", functionImports, notFoundMsg, notFoundURL );

decltype(&__01415f71_tobii_research_apply_calibration_data) tobii_research_apply_calibration_data = 0;
decltype(&__344f5860_tobii_research_apply_licenses) tobii_research_apply_licenses = 0;
decltype(&__1b52407d_tobii_research_clear_applied_licenses) tobii_research_clear_applied_licenses = 0;
decltype(&__73455c64_tobii_research_free_calibration_data) tobii_research_free_calibration_data = 0;
decltype(&__3f244e78_tobii_research_free_eye_tracking_modes) tobii_research_free_eye_tracking_modes = 0;
decltype(&__4d51481b_tobii_research_free_gaze_output_frequencies) tobii_research_free_gaze_output_frequencies = 0;
decltype(&__624b392e_tobii_research_free_string) tobii_research_free_string = 0;
decltype(&__7f5e031d_tobii_research_get_address) tobii_research_get_address = 0;
decltype(&__78201138_tobii_research_get_all_eye_tracking_modes) tobii_research_get_all_eye_tracking_modes = 0;
decltype(&__0d26724a_tobii_research_get_all_gaze_output_frequencies) tobii_research_get_all_gaze_output_frequencies = 0;
decltype(&__09300a6c_tobii_research_get_capabilities) tobii_research_get_capabilities = 0;
decltype(&__0505767f_tobii_research_get_device_name) tobii_research_get_device_name = 0;
decltype(&__12263f70_tobii_research_get_display_area) tobii_research_get_display_area = 0;
decltype(&__674c4e59_tobii_research_get_eye_tracking_mode) tobii_research_get_eye_tracking_mode = 0;
decltype(&__7f436524_tobii_research_get_firmware_version) tobii_research_get_firmware_version = 0;
decltype(&__04392d3b_tobii_research_get_gaze_output_frequency) tobii_research_get_gaze_output_frequency = 0;
decltype(&__653f2f39_tobii_research_get_hmd_lens_configuration) tobii_research_get_hmd_lens_configuration = 0;
decltype(&__072d1418_tobii_research_get_model) tobii_research_get_model = 0;
decltype(&__0c06130c_tobii_research_get_serial_number) tobii_research_get_serial_number = 0;
decltype(&__454a7d7e_tobii_research_get_track_box) tobii_research_get_track_box = 0;
decltype(&__01222e19_tobii_research_retrieve_calibration_data) tobii_research_retrieve_calibration_data = 0;
decltype(&__0505766b_tobii_research_set_device_name) tobii_research_set_device_name = 0;
decltype(&__674c4e4d_tobii_research_set_eye_tracking_mode) tobii_research_set_eye_tracking_mode = 0;
decltype(&__04392d2f_tobii_research_set_gaze_output_frequency) tobii_research_set_gaze_output_frequency = 0;
decltype(&__653f2f2d_tobii_research_set_hmd_lens_configuration) tobii_research_set_hmd_lens_configuration = 0;


namespace {
const Dylib::Import functionImports_[] =
{
  { "tobii_research_apply_calibration_data", (void**)&tobii_research_apply_calibration_data, Dylib::Import::cMangled },
  { "tobii_research_apply_licenses", (void**)&tobii_research_apply_licenses, Dylib::Import::cMangled },
  { "tobii_research_clear_applied_licenses", (void**)&tobii_research_clear_applied_licenses, Dylib::Import::cMangled },
  { "tobii_research_free_calibration_data", (void**)&tobii_research_free_calibration_data, Dylib::Import::cMangled },
  { "tobii_research_free_eye_tracking_modes", (void**)&tobii_research_free_eye_tracking_modes, Dylib::Import::cMangled },
  { "tobii_research_free_gaze_output_frequencies", (void**)&tobii_research_free_gaze_output_frequencies, Dylib::Import::cMangled },
  { "tobii_research_free_string", (void**)&tobii_research_free_string, Dylib::Import::cMangled },
  { "tobii_research_get_address", (void**)&tobii_research_get_address, Dylib::Import::cMangled },
  { "tobii_research_get_all_eye_tracking_modes", (void**)&tobii_research_get_all_eye_tracking_modes, Dylib::Import::cMangled },
  { "tobii_research_get_all_gaze_output_frequencies", (void**)&tobii_research_get_all_gaze_output_frequencies, Dylib::Import::cMangled },
  { "tobii_research_get_capabilities", (void**)&tobii_research_get_capabilities, Dylib::Import::cMangled },
  { "tobii_research_get_device_name", (void**)&tobii_research_get_device_name, Dylib::Import::cMangled },
  { "tobii_research_get_display_area", (void**)&tobii_research_get_display_area, Dylib::Import::cMangled },
  { "tobii_research_get_eye_tracking_mode", (void**)&tobii_research_get_eye_tracking_mode, Dylib::Import::cMangled },
  { "tobii_research_get_firmware_version", (void**)&tobii_research_get_firmware_version, Dylib::Import::cMangled },
  { "tobii_research_get_gaze_output_frequency", (void**)&tobii_research_get_gaze_output_frequency, Dylib::Import::cMangled },
  { "tobii_research_get_hmd_lens_configuration", (void**)&tobii_research_get_hmd_lens_configuration, Dylib::Import::cMangled },
  { "tobii_research_get_model", (void**)&tobii_research_get_model, Dylib::Import::cMangled },
  { "tobii_research_get_serial_number", (void**)&tobii_research_get_serial_number, Dylib::Import::cMangled },
  { "tobii_research_get_track_box", (void**)&tobii_research_get_track_box, Dylib::Import::cMangled },
  { "tobii_research_retrieve_calibration_data", (void**)&tobii_research_retrieve_calibration_data, Dylib::Import::cMangled },
  { "tobii_research_set_device_name", (void**)&tobii_research_set_device_name, Dylib::Import::cMangled },
  { "tobii_research_set_eye_tracking_mode", (void**)&tobii_research_set_eye_tracking_mode, Dylib::Import::cMangled },
  { "tobii_research_set_gaze_output_frequency", (void**)&tobii_research_set_gaze_output_frequency, Dylib::Import::cMangled },
  { "tobii_research_set_hmd_lens_configuration", (void**)&tobii_research_set_hmd_lens_configuration, Dylib::Import::cMangled },
  { 0, 0, 0 }
};
const Dylib::Import* functionImports = functionImports_;
}

#endif // STATIC_LIBTOBII_RESEARCH


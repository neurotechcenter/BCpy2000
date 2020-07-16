// Import definitions for tobii_research library
// generated Tue Aug 08 15:59:35 2017 by DylibTool
#ifdef STATIC_LIBTOBII_RESEARCH

namespace Dylib { bool tobii_research_stream_Loaded() { return true; } }


#else // STATIC_LIBTOBII_RESEARCH

#include "tobii_research.imports.h"
#include "DylibImports.h"
namespace { extern const Dylib::Import* functionImports; }

// Here you may specify a custom error message to be displayed when the library cannot be found.
static const char* notFoundMsg = "";
// Here you may specify an URL to some local or remote help resource.
static const char* notFoundURL = "";
RegisterDylibWithAliases( tobii_research_stream, "tobii_research", functionImports, notFoundMsg, notFoundURL );

decltype(&__17273404_tobii_research_subscribe_to_external_signal_data) tobii_research_subscribe_to_external_signal_data = 0;
decltype(&__3c56546c_tobii_research_subscribe_to_eye_image) tobii_research_subscribe_to_eye_image = 0;
decltype(&__636e5c79_tobii_research_subscribe_to_eye_image_as_gif) tobii_research_subscribe_to_eye_image_as_gif = 0;
decltype(&__0c474b45_tobii_research_subscribe_to_gaze_data) tobii_research_subscribe_to_gaze_data = 0;
decltype(&__642a2f1a_tobii_research_subscribe_to_hmd_gaze_data) tobii_research_subscribe_to_hmd_gaze_data = 0;
decltype(&__5a2d2832_tobii_research_subscribe_to_notifications) tobii_research_subscribe_to_notifications = 0;
decltype(&__46291226_tobii_research_subscribe_to_stream_errors) tobii_research_subscribe_to_stream_errors = 0;
decltype(&__3954424b_tobii_research_subscribe_to_time_synchronization_data) tobii_research_subscribe_to_time_synchronization_data = 0;
decltype(&__42427f69_tobii_research_unsubscribe_from_external_signal_data) tobii_research_unsubscribe_from_external_signal_data = 0;
decltype(&__69331f01_tobii_research_unsubscribe_from_eye_image) tobii_research_unsubscribe_from_eye_image = 0;
decltype(&__360b1714_tobii_research_unsubscribe_from_eye_image_as_gif) tobii_research_unsubscribe_from_eye_image_as_gif = 0;
decltype(&__59220028_tobii_research_unsubscribe_from_gaze_data) tobii_research_unsubscribe_from_gaze_data = 0;
decltype(&__314f6477_tobii_research_unsubscribe_from_hmd_gaze_data) tobii_research_unsubscribe_from_hmd_gaze_data = 0;
decltype(&__0f48635f_tobii_research_unsubscribe_from_notifications) tobii_research_unsubscribe_from_notifications = 0;
decltype(&__134c594b_tobii_research_unsubscribe_from_stream_errors) tobii_research_unsubscribe_from_stream_errors = 0;
decltype(&__6c310926_tobii_research_unsubscribe_from_time_synchronization_data) tobii_research_unsubscribe_from_time_synchronization_data = 0;


namespace {
const Dylib::Import functionImports_[] =
{
  { "tobii_research_subscribe_to_external_signal_data", (void**)&tobii_research_subscribe_to_external_signal_data, Dylib::Import::cMangled },
  { "tobii_research_subscribe_to_eye_image", (void**)&tobii_research_subscribe_to_eye_image, Dylib::Import::cMangled },
  { "tobii_research_subscribe_to_eye_image_as_gif", (void**)&tobii_research_subscribe_to_eye_image_as_gif, Dylib::Import::cMangled },
  { "tobii_research_subscribe_to_gaze_data", (void**)&tobii_research_subscribe_to_gaze_data, Dylib::Import::cMangled },
  { "tobii_research_subscribe_to_hmd_gaze_data", (void**)&tobii_research_subscribe_to_hmd_gaze_data, Dylib::Import::cMangled },
  { "tobii_research_subscribe_to_notifications", (void**)&tobii_research_subscribe_to_notifications, Dylib::Import::cMangled },
  { "tobii_research_subscribe_to_stream_errors", (void**)&tobii_research_subscribe_to_stream_errors, Dylib::Import::cMangled },
  { "tobii_research_subscribe_to_time_synchronization_data", (void**)&tobii_research_subscribe_to_time_synchronization_data, Dylib::Import::cMangled },
  { "tobii_research_unsubscribe_from_external_signal_data", (void**)&tobii_research_unsubscribe_from_external_signal_data, Dylib::Import::cMangled },
  { "tobii_research_unsubscribe_from_eye_image", (void**)&tobii_research_unsubscribe_from_eye_image, Dylib::Import::cMangled },
  { "tobii_research_unsubscribe_from_eye_image_as_gif", (void**)&tobii_research_unsubscribe_from_eye_image_as_gif, Dylib::Import::cMangled },
  { "tobii_research_unsubscribe_from_gaze_data", (void**)&tobii_research_unsubscribe_from_gaze_data, Dylib::Import::cMangled },
  { "tobii_research_unsubscribe_from_hmd_gaze_data", (void**)&tobii_research_unsubscribe_from_hmd_gaze_data, Dylib::Import::cMangled },
  { "tobii_research_unsubscribe_from_notifications", (void**)&tobii_research_unsubscribe_from_notifications, Dylib::Import::cMangled },
  { "tobii_research_unsubscribe_from_stream_errors", (void**)&tobii_research_unsubscribe_from_stream_errors, Dylib::Import::cMangled },
  { "tobii_research_unsubscribe_from_time_synchronization_data", (void**)&tobii_research_unsubscribe_from_time_synchronization_data, Dylib::Import::cMangled },
  { 0, 0, 0 }
};
const Dylib::Import* functionImports = functionImports_;
}

#endif // STATIC_LIBTOBII_RESEARCH


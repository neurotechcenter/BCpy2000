// Import definitions for tobii_research library
// generated Tue Aug 08 16:05:50 2017 by DylibTool
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
RegisterDylibWithAliases( tobii_research, "tobii_research", functionImports, notFoundMsg, notFoundURL );

decltype(&__77555307_tobii_research_find_all_eyetrackers) tobii_research_find_all_eyetrackers = 0;
decltype(&__0d323e58_tobii_research_free_eyetrackers) tobii_research_free_eyetrackers = 0;
decltype(&__024f6e6b_tobii_research_get_eyetracker) tobii_research_get_eyetracker = 0;
decltype(&__063e5d75_tobii_research_get_sdk_version) tobii_research_get_sdk_version = 0;
decltype(&__65614365_tobii_research_get_system_time_stamp) tobii_research_get_system_time_stamp = 0;
decltype(&__0b36070e_tobii_research_logging_subscribe) tobii_research_logging_subscribe = 0;
decltype(&__73481105_tobii_research_logging_unsubscribe) tobii_research_logging_unsubscribe = 0;


namespace {
const Dylib::Import functionImports_[] =
{
  { "tobii_research_find_all_eyetrackers", (void**)&tobii_research_find_all_eyetrackers, Dylib::Import::cMangled },
  { "tobii_research_free_eyetrackers", (void**)&tobii_research_free_eyetrackers, Dylib::Import::cMangled },
  { "tobii_research_free_string", (void**)&tobii_research_free_string, Dylib::Import::cMangled },
  { "tobii_research_get_eyetracker", (void**)&tobii_research_get_eyetracker, Dylib::Import::cMangled },
  { "tobii_research_get_sdk_version", (void**)&tobii_research_get_sdk_version, Dylib::Import::cMangled },
  { "tobii_research_get_system_time_stamp", (void**)&tobii_research_get_system_time_stamp, Dylib::Import::cMangled },
  { "tobii_research_logging_subscribe", (void**)&tobii_research_logging_subscribe, Dylib::Import::cMangled },
  { "tobii_research_logging_unsubscribe", (void**)&tobii_research_logging_unsubscribe, Dylib::Import::cMangled },
  { 0, 0, 0 }
};
const Dylib::Import* functionImports = functionImports_;
}

#endif // STATIC_LIBTOBII_RESEARCH


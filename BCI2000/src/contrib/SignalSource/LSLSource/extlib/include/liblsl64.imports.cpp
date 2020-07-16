// Import definitions for liblsl64 library
// generated Wed Sep 06 15:33:13 2017 by DylibTool
#ifdef STATIC_LIBLIBLSL64

namespace Dylib { bool liblsl64_Loaded() { return true; } }


#else // STATIC_LIBLIBLSL64

#include "liblsl64.imports.h"
#include "DylibImports.h"
namespace { extern const Dylib::Import* functionImports; }

// Here you may specify a custom error message to be displayed when the library cannot be found.
static const char* notFoundMsg = "";
// Here you may specify an URL to some local or remote help resource.
static const char* notFoundURL = "";
RegisterDylibWithAliases( liblsl64, "liblsl64", functionImports, notFoundMsg, notFoundURL );

decltype(&__0b0e2f3d_lsl_append_child) lsl_append_child = 0;
decltype(&__211d4e51_lsl_append_child_value) lsl_append_child_value = 0;
decltype(&__0c173a59_lsl_append_copy) lsl_append_copy = 0;
decltype(&__6b1b0533_lsl_child) lsl_child = 0;
decltype(&__07311652_lsl_child_value) lsl_child_value = 0;
decltype(&__6931160d_lsl_child_value_n) lsl_child_value_n = 0;
decltype(&__18251135_lsl_close_stream) lsl_close_stream = 0;
decltype(&__5b686a3d_lsl_copy_streaminfo) lsl_copy_streaminfo = 0;
decltype(&__1f3d5352_lsl_create_continuous_resolver) lsl_create_continuous_resolver = 0;
decltype(&__024d7e55_lsl_create_continuous_resolver_bypred) lsl_create_continuous_resolver_bypred = 0;
decltype(&__164d7e5f_lsl_create_continuous_resolver_byprop) lsl_create_continuous_resolver_byprop = 0;
decltype(&__15083323_lsl_create_inlet) lsl_create_inlet = 0;
decltype(&__7a103a34_lsl_create_outlet) lsl_create_outlet = 0;
decltype(&__0d7f5d4a_lsl_create_streaminfo) lsl_create_streaminfo = 0;
decltype(&__161d3f71_lsl_destroy_continuous_resolver) lsl_destroy_continuous_resolver = 0;
decltype(&__67170a11_lsl_destroy_inlet) lsl_destroy_inlet = 0;
decltype(&__70781218_lsl_destroy_outlet) lsl_destroy_outlet = 0;
decltype(&__0e0f7d7f_lsl_destroy_streaminfo) lsl_destroy_streaminfo = 0;
decltype(&__676a141d_lsl_destroy_string) lsl_destroy_string = 0;
decltype(&__701e1c2b_lsl_empty) lsl_empty = 0;
decltype(&__17291944_lsl_first_child) lsl_first_child = 0;
decltype(&__17626154_lsl_get_channel_bytes) lsl_get_channel_bytes = 0;
decltype(&__1174605f_lsl_get_channel_count) lsl_get_channel_count = 0;
decltype(&__0100675c_lsl_get_channel_format) lsl_get_channel_format = 0;
decltype(&__7d75193e_lsl_get_created_at) lsl_get_created_at = 0;
decltype(&__6f736b63_lsl_get_desc) lsl_get_desc = 0;
decltype(&__040d1203_lsl_get_fullinfo) lsl_get_fullinfo = 0;
decltype(&__0d180611_lsl_get_hostname) lsl_get_hostname = 0;
decltype(&__62787e6f_lsl_get_info) lsl_get_info = 0;
decltype(&__65777565_lsl_get_name) lsl_get_name = 0;
decltype(&__1d6a7842_lsl_get_nominal_srate) lsl_get_nominal_srate = 0;
decltype(&__6d664f61_lsl_get_sample_bytes) lsl_get_sample_bytes = 0;
decltype(&__7878052c_lsl_get_session_id) lsl_get_session_id = 0;
decltype(&__7f1c321b_lsl_get_source_id) lsl_get_source_id = 0;
decltype(&__7f6f6865_lsl_get_type) lsl_get_type = 0;
decltype(&__7e7f7c00_lsl_get_uid) lsl_get_uid = 0;
decltype(&__141c0473_lsl_get_version) lsl_get_version = 0;
decltype(&__737b7400_lsl_get_xml) lsl_get_xml = 0;
decltype(&__5a771831_lsl_have_consumers) lsl_have_consumers = 0;
decltype(&__6078472b_lsl_is_text) lsl_is_text = 0;
decltype(&__33157742_lsl_last_child) lsl_last_child = 0;
decltype(&__7e626b01_lsl_library_version) lsl_library_version = 0;
decltype(&__03200752_lsl_local_clock) lsl_local_clock = 0;
decltype(&__0212013a_lsl_name) lsl_name = 0;
decltype(&__310c132e_lsl_next_sibling) lsl_next_sibling = 0;
decltype(&__6e62132e_lsl_next_sibling_n) lsl_next_sibling_n = 0;
decltype(&__39111043_lsl_open_stream) lsl_open_stream = 0;
decltype(&__72661e3a_lsl_parent) lsl_parent = 0;
decltype(&__7e07041c_lsl_prepend_child) lsl_prepend_child = 0;
decltype(&__122d177d_lsl_prepend_child_value) lsl_prepend_child_value = 0;
decltype(&__1a001d09_lsl_prepend_copy) lsl_prepend_copy = 0;
decltype(&__46747b5f_lsl_previous_sibling) lsl_previous_sibling = 0;
decltype(&__191a7b5f_lsl_previous_sibling_n) lsl_previous_sibling_n = 0;
decltype(&__5f7d665b_lsl_protocol_version) lsl_protocol_version = 0;
decltype(&__58683724_lsl_pull_chunk_buf) lsl_pull_chunk_buf = 0;
decltype(&__2d0e3725_lsl_pull_chunk_c) lsl_pull_chunk_c = 0;
decltype(&__2d0e3722_lsl_pull_chunk_d) lsl_pull_chunk_d = 0;
decltype(&__2d0e3720_lsl_pull_chunk_f) lsl_pull_chunk_f = 0;
decltype(&__2d0e372f_lsl_pull_chunk_i) lsl_pull_chunk_i = 0;
decltype(&__2d0e372a_lsl_pull_chunk_l) lsl_pull_chunk_l = 0;
decltype(&__2d0e3735_lsl_pull_chunk_s) lsl_pull_chunk_s = 0;
decltype(&__597c3735_lsl_pull_chunk_str) lsl_pull_chunk_str = 0;
decltype(&__516c6201_lsl_pull_sample_buf) lsl_pull_sample_buf = 0;
decltype(&__50190401_lsl_pull_sample_c) lsl_pull_sample_c = 0;
decltype(&__57190401_lsl_pull_sample_d) lsl_pull_sample_d = 0;
decltype(&__55190401_lsl_pull_sample_f) lsl_pull_sample_f = 0;
decltype(&__5a190401_lsl_pull_sample_i) lsl_pull_sample_i = 0;
decltype(&__5f190401_lsl_pull_sample_l) lsl_pull_sample_l = 0;
decltype(&__40190401_lsl_pull_sample_s) lsl_pull_sample_s = 0;
decltype(&__406d7601_lsl_pull_sample_str) lsl_pull_sample_str = 0;
decltype(&__45190401_lsl_pull_sample_v) lsl_pull_sample_v = 0;
decltype(&__58682820_lsl_push_chunk_buf) lsl_push_chunk_buf = 0;
decltype(&__58685c20_lsl_push_chunk_buft) lsl_push_chunk_buft = 0;
decltype(&__58685c4e_lsl_push_chunk_buftn) lsl_push_chunk_buftn = 0;
decltype(&__28685c4e_lsl_push_chunk_buftnp) lsl_push_chunk_buftnp = 0;
decltype(&__58685c50_lsl_push_chunk_buftp) lsl_push_chunk_buftp = 0;
decltype(&__2d0e2821_lsl_push_chunk_c) lsl_push_chunk_c = 0;
decltype(&__590e2821_lsl_push_chunk_ct) lsl_push_chunk_ct = 0;
decltype(&__59602821_lsl_push_chunk_ctn) lsl_push_chunk_ctn = 0;
decltype(&__59605821_lsl_push_chunk_ctnp) lsl_push_chunk_ctnp = 0;
decltype(&__597e2821_lsl_push_chunk_ctp) lsl_push_chunk_ctp = 0;
decltype(&__2d0e2826_lsl_push_chunk_d) lsl_push_chunk_d = 0;
decltype(&__590e2826_lsl_push_chunk_dt) lsl_push_chunk_dt = 0;
decltype(&__59602826_lsl_push_chunk_dtn) lsl_push_chunk_dtn = 0;
decltype(&__59605826_lsl_push_chunk_dtnp) lsl_push_chunk_dtnp = 0;
decltype(&__597e2826_lsl_push_chunk_dtp) lsl_push_chunk_dtp = 0;
decltype(&__2d0e2824_lsl_push_chunk_f) lsl_push_chunk_f = 0;
decltype(&__590e2824_lsl_push_chunk_ft) lsl_push_chunk_ft = 0;
decltype(&__59602824_lsl_push_chunk_ftn) lsl_push_chunk_ftn = 0;
decltype(&__59605824_lsl_push_chunk_ftnp) lsl_push_chunk_ftnp = 0;
decltype(&__597e2824_lsl_push_chunk_ftp) lsl_push_chunk_ftp = 0;
decltype(&__2d0e282b_lsl_push_chunk_i) lsl_push_chunk_i = 0;
decltype(&__590e282b_lsl_push_chunk_it) lsl_push_chunk_it = 0;
decltype(&__5960282b_lsl_push_chunk_itn) lsl_push_chunk_itn = 0;
decltype(&__5960582b_lsl_push_chunk_itnp) lsl_push_chunk_itnp = 0;
decltype(&__597e282b_lsl_push_chunk_itp) lsl_push_chunk_itp = 0;
decltype(&__2d0e282e_lsl_push_chunk_l) lsl_push_chunk_l = 0;
decltype(&__590e282e_lsl_push_chunk_lt) lsl_push_chunk_lt = 0;
decltype(&__5960282e_lsl_push_chunk_ltn) lsl_push_chunk_ltn = 0;
decltype(&__5960582e_lsl_push_chunk_ltnp) lsl_push_chunk_ltnp = 0;
decltype(&__597e282e_lsl_push_chunk_ltp) lsl_push_chunk_ltp = 0;
decltype(&__2d0e2831_lsl_push_chunk_s) lsl_push_chunk_s = 0;
decltype(&__590e2831_lsl_push_chunk_st) lsl_push_chunk_st = 0;
decltype(&__59602831_lsl_push_chunk_stn) lsl_push_chunk_stn = 0;
decltype(&__59605831_lsl_push_chunk_stnp) lsl_push_chunk_stnp = 0;
decltype(&__597e2831_lsl_push_chunk_stp) lsl_push_chunk_stp = 0;
decltype(&__597c2831_lsl_push_chunk_str) lsl_push_chunk_str = 0;
decltype(&__597c5c31_lsl_push_chunk_strt) lsl_push_chunk_strt = 0;
decltype(&__597c5c5f_lsl_push_chunk_strtn) lsl_push_chunk_strtn = 0;
decltype(&__297c5c5f_lsl_push_chunk_strtnp) lsl_push_chunk_strtnp = 0;
decltype(&__597c5c41_lsl_push_chunk_strtp) lsl_push_chunk_strtp = 0;
decltype(&__516c7d05_lsl_push_sample_buf) lsl_push_sample_buf = 0;
decltype(&__516c7d71_lsl_push_sample_buft) lsl_push_sample_buft = 0;
decltype(&__216c7d71_lsl_push_sample_buftp) lsl_push_sample_buftp = 0;
decltype(&__50191b05_lsl_push_sample_c) lsl_push_sample_c = 0;
decltype(&__506d1b05_lsl_push_sample_ct) lsl_push_sample_ct = 0;
decltype(&__506d6b05_lsl_push_sample_ctp) lsl_push_sample_ctp = 0;
decltype(&__57191b05_lsl_push_sample_d) lsl_push_sample_d = 0;
decltype(&__576d1b05_lsl_push_sample_dt) lsl_push_sample_dt = 0;
decltype(&__576d6b05_lsl_push_sample_dtp) lsl_push_sample_dtp = 0;
decltype(&__55191b05_lsl_push_sample_f) lsl_push_sample_f = 0;
decltype(&__556d1b05_lsl_push_sample_ft) lsl_push_sample_ft = 0;
decltype(&__556d6b05_lsl_push_sample_ftp) lsl_push_sample_ftp = 0;
decltype(&__5a191b05_lsl_push_sample_i) lsl_push_sample_i = 0;
decltype(&__5a6d1b05_lsl_push_sample_it) lsl_push_sample_it = 0;
decltype(&__5a6d6b05_lsl_push_sample_itp) lsl_push_sample_itp = 0;
decltype(&__5f191b05_lsl_push_sample_l) lsl_push_sample_l = 0;
decltype(&__5f6d1b05_lsl_push_sample_lt) lsl_push_sample_lt = 0;
decltype(&__5f6d6b05_lsl_push_sample_ltp) lsl_push_sample_ltp = 0;
decltype(&__40191b05_lsl_push_sample_s) lsl_push_sample_s = 0;
decltype(&__406d1b05_lsl_push_sample_st) lsl_push_sample_st = 0;
decltype(&__406d6b05_lsl_push_sample_stp) lsl_push_sample_stp = 0;
decltype(&__406d6905_lsl_push_sample_str) lsl_push_sample_str = 0;
decltype(&__406d6971_lsl_push_sample_strt) lsl_push_sample_strt = 0;
decltype(&__306d6971_lsl_push_sample_strtp) lsl_push_sample_strtp = 0;
decltype(&__45191b05_lsl_push_sample_v) lsl_push_sample_v = 0;
decltype(&__456d1b05_lsl_push_sample_vt) lsl_push_sample_vt = 0;
decltype(&__456d6b05_lsl_push_sample_vtp) lsl_push_sample_vtp = 0;
decltype(&__001a3237_lsl_remove_child) lsl_remove_child = 0;
decltype(&__5f743237_lsl_remove_child_n) lsl_remove_child_n = 0;
decltype(&__130c166f_lsl_resolve_all) lsl_resolve_all = 0;
decltype(&__757d0a1d_lsl_resolve_bypred) lsl_resolve_bypred = 0;
decltype(&__7f690a1d_lsl_resolve_byprop) lsl_resolve_byprop = 0;
decltype(&__587e6b42_lsl_resolver_results) lsl_resolver_results = 0;
decltype(&__1b607175_lsl_samples_available) lsl_samples_available = 0;
decltype(&__7454620d_lsl_set_child_value) lsl_set_child_value = 0;
decltype(&__71777565_lsl_set_name) lsl_set_name = 0;
decltype(&__141f777e_lsl_set_postprocessing) lsl_set_postprocessing = 0;
decltype(&__0c777475_lsl_set_value) lsl_set_value = 0;
decltype(&__0d2a7656_lsl_smoothing_halftime) lsl_smoothing_halftime = 0;
decltype(&__1207296d_lsl_streaminfo_from_xml) lsl_streaminfo_from_xml = 0;
decltype(&__5c73633c_lsl_time_correction) lsl_time_correction = 0;
decltype(&__390b6363_lsl_time_correction_ex) lsl_time_correction_ex = 0;
decltype(&__7f12002a_lsl_value) lsl_value = 0;
decltype(&__1a116852_lsl_wait_for_consumers) lsl_wait_for_consumers = 0;
decltype(&__60447606_lsl_was_clock_reset) lsl_was_clock_reset = 0;


namespace {
const Dylib::Import functionImports_[] =
{
  { "lsl_append_child", (void**)&lsl_append_child, Dylib::Import::cMangled },
  { "lsl_append_child_value", (void**)&lsl_append_child_value, Dylib::Import::cMangled },
  { "lsl_append_copy", (void**)&lsl_append_copy, Dylib::Import::cMangled },
  { "lsl_child", (void**)&lsl_child, Dylib::Import::cMangled },
  { "lsl_child_value", (void**)&lsl_child_value, Dylib::Import::cMangled },
  { "lsl_child_value_n", (void**)&lsl_child_value_n, Dylib::Import::cMangled },
  { "lsl_close_stream", (void**)&lsl_close_stream, Dylib::Import::cMangled },
  { "lsl_copy_streaminfo", (void**)&lsl_copy_streaminfo, Dylib::Import::cMangled },
  { "lsl_create_continuous_resolver", (void**)&lsl_create_continuous_resolver, Dylib::Import::cMangled },
  { "lsl_create_continuous_resolver_bypred", (void**)&lsl_create_continuous_resolver_bypred, Dylib::Import::cMangled },
  { "lsl_create_continuous_resolver_byprop", (void**)&lsl_create_continuous_resolver_byprop, Dylib::Import::cMangled },
  { "lsl_create_inlet", (void**)&lsl_create_inlet, Dylib::Import::cMangled },
  { "lsl_create_outlet", (void**)&lsl_create_outlet, Dylib::Import::cMangled },
  { "lsl_create_streaminfo", (void**)&lsl_create_streaminfo, Dylib::Import::cMangled },
  { "lsl_destroy_continuous_resolver", (void**)&lsl_destroy_continuous_resolver, Dylib::Import::cMangled },
  { "lsl_destroy_inlet", (void**)&lsl_destroy_inlet, Dylib::Import::cMangled },
  { "lsl_destroy_outlet", (void**)&lsl_destroy_outlet, Dylib::Import::cMangled },
  { "lsl_destroy_streaminfo", (void**)&lsl_destroy_streaminfo, Dylib::Import::cMangled },
  { "lsl_destroy_string", (void**)&lsl_destroy_string, Dylib::Import::cMangled },
  { "lsl_empty", (void**)&lsl_empty, Dylib::Import::cMangled },
  { "lsl_first_child", (void**)&lsl_first_child, Dylib::Import::cMangled },
  { "lsl_get_channel_bytes", (void**)&lsl_get_channel_bytes, Dylib::Import::cMangled },
  { "lsl_get_channel_count", (void**)&lsl_get_channel_count, Dylib::Import::cMangled },
  { "lsl_get_channel_format", (void**)&lsl_get_channel_format, Dylib::Import::cMangled },
  { "lsl_get_created_at", (void**)&lsl_get_created_at, Dylib::Import::cMangled },
  { "lsl_get_desc", (void**)&lsl_get_desc, Dylib::Import::cMangled },
  { "lsl_get_fullinfo", (void**)&lsl_get_fullinfo, Dylib::Import::cMangled },
  { "lsl_get_hostname", (void**)&lsl_get_hostname, Dylib::Import::cMangled },
  { "lsl_get_info", (void**)&lsl_get_info, Dylib::Import::cMangled },
  { "lsl_get_name", (void**)&lsl_get_name, Dylib::Import::cMangled },
  { "lsl_get_nominal_srate", (void**)&lsl_get_nominal_srate, Dylib::Import::cMangled },
  { "lsl_get_sample_bytes", (void**)&lsl_get_sample_bytes, Dylib::Import::cMangled },
  { "lsl_get_session_id", (void**)&lsl_get_session_id, Dylib::Import::cMangled },
  { "lsl_get_source_id", (void**)&lsl_get_source_id, Dylib::Import::cMangled },
  { "lsl_get_type", (void**)&lsl_get_type, Dylib::Import::cMangled },
  { "lsl_get_uid", (void**)&lsl_get_uid, Dylib::Import::cMangled },
  { "lsl_get_version", (void**)&lsl_get_version, Dylib::Import::cMangled },
  { "lsl_get_xml", (void**)&lsl_get_xml, Dylib::Import::cMangled },
  { "lsl_have_consumers", (void**)&lsl_have_consumers, Dylib::Import::cMangled },
  { "lsl_is_text", (void**)&lsl_is_text, Dylib::Import::cMangled },
  { "lsl_last_child", (void**)&lsl_last_child, Dylib::Import::cMangled },
  { "lsl_library_version", (void**)&lsl_library_version, Dylib::Import::cMangled },
  { "lsl_local_clock", (void**)&lsl_local_clock, Dylib::Import::cMangled },
  { "lsl_name", (void**)&lsl_name, Dylib::Import::cMangled },
  { "lsl_next_sibling", (void**)&lsl_next_sibling, Dylib::Import::cMangled },
  { "lsl_next_sibling_n", (void**)&lsl_next_sibling_n, Dylib::Import::cMangled },
  { "lsl_open_stream", (void**)&lsl_open_stream, Dylib::Import::cMangled },
  { "lsl_parent", (void**)&lsl_parent, Dylib::Import::cMangled },
  { "lsl_prepend_child", (void**)&lsl_prepend_child, Dylib::Import::cMangled },
  { "lsl_prepend_child_value", (void**)&lsl_prepend_child_value, Dylib::Import::cMangled },
  { "lsl_prepend_copy", (void**)&lsl_prepend_copy, Dylib::Import::cMangled },
  { "lsl_previous_sibling", (void**)&lsl_previous_sibling, Dylib::Import::cMangled },
  { "lsl_previous_sibling_n", (void**)&lsl_previous_sibling_n, Dylib::Import::cMangled },
  { "lsl_protocol_version", (void**)&lsl_protocol_version, Dylib::Import::cMangled },
  { "lsl_pull_chunk_buf", (void**)&lsl_pull_chunk_buf, Dylib::Import::cMangled },
  { "lsl_pull_chunk_c", (void**)&lsl_pull_chunk_c, Dylib::Import::cMangled },
  { "lsl_pull_chunk_d", (void**)&lsl_pull_chunk_d, Dylib::Import::cMangled },
  { "lsl_pull_chunk_f", (void**)&lsl_pull_chunk_f, Dylib::Import::cMangled },
  { "lsl_pull_chunk_i", (void**)&lsl_pull_chunk_i, Dylib::Import::cMangled },
  { "lsl_pull_chunk_l", (void**)&lsl_pull_chunk_l, Dylib::Import::cMangled },
  { "lsl_pull_chunk_s", (void**)&lsl_pull_chunk_s, Dylib::Import::cMangled },
  { "lsl_pull_chunk_str", (void**)&lsl_pull_chunk_str, Dylib::Import::cMangled },
  { "lsl_pull_sample_buf", (void**)&lsl_pull_sample_buf, Dylib::Import::cMangled },
  { "lsl_pull_sample_c", (void**)&lsl_pull_sample_c, Dylib::Import::cMangled },
  { "lsl_pull_sample_d", (void**)&lsl_pull_sample_d, Dylib::Import::cMangled },
  { "lsl_pull_sample_f", (void**)&lsl_pull_sample_f, Dylib::Import::cMangled },
  { "lsl_pull_sample_i", (void**)&lsl_pull_sample_i, Dylib::Import::cMangled },
  { "lsl_pull_sample_l", (void**)&lsl_pull_sample_l, Dylib::Import::cMangled },
  { "lsl_pull_sample_s", (void**)&lsl_pull_sample_s, Dylib::Import::cMangled },
  { "lsl_pull_sample_str", (void**)&lsl_pull_sample_str, Dylib::Import::cMangled },
  { "lsl_pull_sample_v", (void**)&lsl_pull_sample_v, Dylib::Import::cMangled },
  { "lsl_push_chunk_buf", (void**)&lsl_push_chunk_buf, Dylib::Import::cMangled },
  { "lsl_push_chunk_buft", (void**)&lsl_push_chunk_buft, Dylib::Import::cMangled },
  { "lsl_push_chunk_buftn", (void**)&lsl_push_chunk_buftn, Dylib::Import::cMangled },
  { "lsl_push_chunk_buftnp", (void**)&lsl_push_chunk_buftnp, Dylib::Import::cMangled },
  { "lsl_push_chunk_buftp", (void**)&lsl_push_chunk_buftp, Dylib::Import::cMangled },
  { "lsl_push_chunk_c", (void**)&lsl_push_chunk_c, Dylib::Import::cMangled },
  { "lsl_push_chunk_ct", (void**)&lsl_push_chunk_ct, Dylib::Import::cMangled },
  { "lsl_push_chunk_ctn", (void**)&lsl_push_chunk_ctn, Dylib::Import::cMangled },
  { "lsl_push_chunk_ctnp", (void**)&lsl_push_chunk_ctnp, Dylib::Import::cMangled },
  { "lsl_push_chunk_ctp", (void**)&lsl_push_chunk_ctp, Dylib::Import::cMangled },
  { "lsl_push_chunk_d", (void**)&lsl_push_chunk_d, Dylib::Import::cMangled },
  { "lsl_push_chunk_dt", (void**)&lsl_push_chunk_dt, Dylib::Import::cMangled },
  { "lsl_push_chunk_dtn", (void**)&lsl_push_chunk_dtn, Dylib::Import::cMangled },
  { "lsl_push_chunk_dtnp", (void**)&lsl_push_chunk_dtnp, Dylib::Import::cMangled },
  { "lsl_push_chunk_dtp", (void**)&lsl_push_chunk_dtp, Dylib::Import::cMangled },
  { "lsl_push_chunk_f", (void**)&lsl_push_chunk_f, Dylib::Import::cMangled },
  { "lsl_push_chunk_ft", (void**)&lsl_push_chunk_ft, Dylib::Import::cMangled },
  { "lsl_push_chunk_ftn", (void**)&lsl_push_chunk_ftn, Dylib::Import::cMangled },
  { "lsl_push_chunk_ftnp", (void**)&lsl_push_chunk_ftnp, Dylib::Import::cMangled },
  { "lsl_push_chunk_ftp", (void**)&lsl_push_chunk_ftp, Dylib::Import::cMangled },
  { "lsl_push_chunk_i", (void**)&lsl_push_chunk_i, Dylib::Import::cMangled },
  { "lsl_push_chunk_it", (void**)&lsl_push_chunk_it, Dylib::Import::cMangled },
  { "lsl_push_chunk_itn", (void**)&lsl_push_chunk_itn, Dylib::Import::cMangled },
  { "lsl_push_chunk_itnp", (void**)&lsl_push_chunk_itnp, Dylib::Import::cMangled },
  { "lsl_push_chunk_itp", (void**)&lsl_push_chunk_itp, Dylib::Import::cMangled },
  { "lsl_push_chunk_l", (void**)&lsl_push_chunk_l, Dylib::Import::cMangled },
  { "lsl_push_chunk_lt", (void**)&lsl_push_chunk_lt, Dylib::Import::cMangled },
  { "lsl_push_chunk_ltn", (void**)&lsl_push_chunk_ltn, Dylib::Import::cMangled },
  { "lsl_push_chunk_ltnp", (void**)&lsl_push_chunk_ltnp, Dylib::Import::cMangled },
  { "lsl_push_chunk_ltp", (void**)&lsl_push_chunk_ltp, Dylib::Import::cMangled },
  { "lsl_push_chunk_s", (void**)&lsl_push_chunk_s, Dylib::Import::cMangled },
  { "lsl_push_chunk_st", (void**)&lsl_push_chunk_st, Dylib::Import::cMangled },
  { "lsl_push_chunk_stn", (void**)&lsl_push_chunk_stn, Dylib::Import::cMangled },
  { "lsl_push_chunk_stnp", (void**)&lsl_push_chunk_stnp, Dylib::Import::cMangled },
  { "lsl_push_chunk_stp", (void**)&lsl_push_chunk_stp, Dylib::Import::cMangled },
  { "lsl_push_chunk_str", (void**)&lsl_push_chunk_str, Dylib::Import::cMangled },
  { "lsl_push_chunk_strt", (void**)&lsl_push_chunk_strt, Dylib::Import::cMangled },
  { "lsl_push_chunk_strtn", (void**)&lsl_push_chunk_strtn, Dylib::Import::cMangled },
  { "lsl_push_chunk_strtnp", (void**)&lsl_push_chunk_strtnp, Dylib::Import::cMangled },
  { "lsl_push_chunk_strtp", (void**)&lsl_push_chunk_strtp, Dylib::Import::cMangled },
  { "lsl_push_sample_buf", (void**)&lsl_push_sample_buf, Dylib::Import::cMangled },
  { "lsl_push_sample_buft", (void**)&lsl_push_sample_buft, Dylib::Import::cMangled },
  { "lsl_push_sample_buftp", (void**)&lsl_push_sample_buftp, Dylib::Import::cMangled },
  { "lsl_push_sample_c", (void**)&lsl_push_sample_c, Dylib::Import::cMangled },
  { "lsl_push_sample_ct", (void**)&lsl_push_sample_ct, Dylib::Import::cMangled },
  { "lsl_push_sample_ctp", (void**)&lsl_push_sample_ctp, Dylib::Import::cMangled },
  { "lsl_push_sample_d", (void**)&lsl_push_sample_d, Dylib::Import::cMangled },
  { "lsl_push_sample_dt", (void**)&lsl_push_sample_dt, Dylib::Import::cMangled },
  { "lsl_push_sample_dtp", (void**)&lsl_push_sample_dtp, Dylib::Import::cMangled },
  { "lsl_push_sample_f", (void**)&lsl_push_sample_f, Dylib::Import::cMangled },
  { "lsl_push_sample_ft", (void**)&lsl_push_sample_ft, Dylib::Import::cMangled },
  { "lsl_push_sample_ftp", (void**)&lsl_push_sample_ftp, Dylib::Import::cMangled },
  { "lsl_push_sample_i", (void**)&lsl_push_sample_i, Dylib::Import::cMangled },
  { "lsl_push_sample_it", (void**)&lsl_push_sample_it, Dylib::Import::cMangled },
  { "lsl_push_sample_itp", (void**)&lsl_push_sample_itp, Dylib::Import::cMangled },
  { "lsl_push_sample_l", (void**)&lsl_push_sample_l, Dylib::Import::cMangled },
  { "lsl_push_sample_lt", (void**)&lsl_push_sample_lt, Dylib::Import::cMangled },
  { "lsl_push_sample_ltp", (void**)&lsl_push_sample_ltp, Dylib::Import::cMangled },
  { "lsl_push_sample_s", (void**)&lsl_push_sample_s, Dylib::Import::cMangled },
  { "lsl_push_sample_st", (void**)&lsl_push_sample_st, Dylib::Import::cMangled },
  { "lsl_push_sample_stp", (void**)&lsl_push_sample_stp, Dylib::Import::cMangled },
  { "lsl_push_sample_str", (void**)&lsl_push_sample_str, Dylib::Import::cMangled },
  { "lsl_push_sample_strt", (void**)&lsl_push_sample_strt, Dylib::Import::cMangled },
  { "lsl_push_sample_strtp", (void**)&lsl_push_sample_strtp, Dylib::Import::cMangled },
  { "lsl_push_sample_v", (void**)&lsl_push_sample_v, Dylib::Import::cMangled },
  { "lsl_push_sample_vt", (void**)&lsl_push_sample_vt, Dylib::Import::cMangled },
  { "lsl_push_sample_vtp", (void**)&lsl_push_sample_vtp, Dylib::Import::cMangled },
  { "lsl_remove_child", (void**)&lsl_remove_child, Dylib::Import::cMangled },
  { "lsl_remove_child_n", (void**)&lsl_remove_child_n, Dylib::Import::cMangled },
  { "lsl_resolve_all", (void**)&lsl_resolve_all, Dylib::Import::cMangled },
  { "lsl_resolve_bypred", (void**)&lsl_resolve_bypred, Dylib::Import::cMangled },
  { "lsl_resolve_byprop", (void**)&lsl_resolve_byprop, Dylib::Import::cMangled },
  { "lsl_resolver_results", (void**)&lsl_resolver_results, Dylib::Import::cMangled },
  { "lsl_samples_available", (void**)&lsl_samples_available, Dylib::Import::cMangled },
  { "lsl_set_child_value", (void**)&lsl_set_child_value, Dylib::Import::cMangled },
  { "lsl_set_name", (void**)&lsl_set_name, Dylib::Import::cMangled },
  { "lsl_set_postprocessing", (void**)&lsl_set_postprocessing, Dylib::Import::cMangled },
  { "lsl_set_value", (void**)&lsl_set_value, Dylib::Import::cMangled },
  { "lsl_smoothing_halftime", (void**)&lsl_smoothing_halftime, Dylib::Import::cMangled },
  { "lsl_streaminfo_from_xml", (void**)&lsl_streaminfo_from_xml, Dylib::Import::cMangled },
  { "lsl_time_correction", (void**)&lsl_time_correction, Dylib::Import::cMangled },
  { "lsl_time_correction_ex", (void**)&lsl_time_correction_ex, Dylib::Import::cMangled },
  { "lsl_value", (void**)&lsl_value, Dylib::Import::cMangled },
  { "lsl_wait_for_consumers", (void**)&lsl_wait_for_consumers, Dylib::Import::cMangled },
  { "lsl_was_clock_reset", (void**)&lsl_was_clock_reset, Dylib::Import::cMangled },
  { 0, 0, 0 }
};
const Dylib::Import* functionImports = functionImports_;
}

#endif // STATIC_LIBLIBLSL64


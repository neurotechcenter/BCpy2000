// Import definitions for liblsl64 library
// generated Wed Sep 06 15:33:13 2017 by DylibTool
#ifndef LIBLSL64_IMPORTS_H
#define LIBLSL64_IMPORTS_H

#include "Win32Defs.h"

#ifndef STATIC_LIBLIBLSL64
// Disable function declarations in the original header
// file by #defining function names to point to symbols that
// are not referenced by any code.
#define lsl_append_child __0b0e2f3d_lsl_append_child
#define lsl_append_child_value __211d4e51_lsl_append_child_value
#define lsl_append_copy __0c173a59_lsl_append_copy
#define lsl_child __6b1b0533_lsl_child
#define lsl_child_value __07311652_lsl_child_value
#define lsl_child_value_n __6931160d_lsl_child_value_n
#define lsl_close_stream __18251135_lsl_close_stream
#define lsl_copy_streaminfo __5b686a3d_lsl_copy_streaminfo
#define lsl_create_continuous_resolver __1f3d5352_lsl_create_continuous_resolver
#define lsl_create_continuous_resolver_bypred __024d7e55_lsl_create_continuous_resolver_bypred
#define lsl_create_continuous_resolver_byprop __164d7e5f_lsl_create_continuous_resolver_byprop
#define lsl_create_inlet __15083323_lsl_create_inlet
#define lsl_create_outlet __7a103a34_lsl_create_outlet
#define lsl_create_streaminfo __0d7f5d4a_lsl_create_streaminfo
#define lsl_destroy_continuous_resolver __161d3f71_lsl_destroy_continuous_resolver
#define lsl_destroy_inlet __67170a11_lsl_destroy_inlet
#define lsl_destroy_outlet __70781218_lsl_destroy_outlet
#define lsl_destroy_streaminfo __0e0f7d7f_lsl_destroy_streaminfo
#define lsl_destroy_string __676a141d_lsl_destroy_string
#define lsl_empty __701e1c2b_lsl_empty
#define lsl_first_child __17291944_lsl_first_child
#define lsl_get_channel_bytes __17626154_lsl_get_channel_bytes
#define lsl_get_channel_count __1174605f_lsl_get_channel_count
#define lsl_get_channel_format __0100675c_lsl_get_channel_format
#define lsl_get_created_at __7d75193e_lsl_get_created_at
#define lsl_get_desc __6f736b63_lsl_get_desc
#define lsl_get_fullinfo __040d1203_lsl_get_fullinfo
#define lsl_get_hostname __0d180611_lsl_get_hostname
#define lsl_get_info __62787e6f_lsl_get_info
#define lsl_get_name __65777565_lsl_get_name
#define lsl_get_nominal_srate __1d6a7842_lsl_get_nominal_srate
#define lsl_get_sample_bytes __6d664f61_lsl_get_sample_bytes
#define lsl_get_session_id __7878052c_lsl_get_session_id
#define lsl_get_source_id __7f1c321b_lsl_get_source_id
#define lsl_get_type __7f6f6865_lsl_get_type
#define lsl_get_uid __7e7f7c00_lsl_get_uid
#define lsl_get_version __141c0473_lsl_get_version
#define lsl_get_xml __737b7400_lsl_get_xml
#define lsl_have_consumers __5a771831_lsl_have_consumers
#define lsl_is_text __6078472b_lsl_is_text
#define lsl_last_child __33157742_lsl_last_child
#define lsl_library_version __7e626b01_lsl_library_version
#define lsl_local_clock __03200752_lsl_local_clock
#define lsl_name __0212013a_lsl_name
#define lsl_next_sibling __310c132e_lsl_next_sibling
#define lsl_next_sibling_n __6e62132e_lsl_next_sibling_n
#define lsl_open_stream __39111043_lsl_open_stream
#define lsl_parent __72661e3a_lsl_parent
#define lsl_prepend_child __7e07041c_lsl_prepend_child
#define lsl_prepend_child_value __122d177d_lsl_prepend_child_value
#define lsl_prepend_copy __1a001d09_lsl_prepend_copy
#define lsl_previous_sibling __46747b5f_lsl_previous_sibling
#define lsl_previous_sibling_n __191a7b5f_lsl_previous_sibling_n
#define lsl_protocol_version __5f7d665b_lsl_protocol_version
#define lsl_pull_chunk_buf __58683724_lsl_pull_chunk_buf
#define lsl_pull_chunk_c __2d0e3725_lsl_pull_chunk_c
#define lsl_pull_chunk_d __2d0e3722_lsl_pull_chunk_d
#define lsl_pull_chunk_f __2d0e3720_lsl_pull_chunk_f
#define lsl_pull_chunk_i __2d0e372f_lsl_pull_chunk_i
#define lsl_pull_chunk_l __2d0e372a_lsl_pull_chunk_l
#define lsl_pull_chunk_s __2d0e3735_lsl_pull_chunk_s
#define lsl_pull_chunk_str __597c3735_lsl_pull_chunk_str
#define lsl_pull_sample_buf __516c6201_lsl_pull_sample_buf
#define lsl_pull_sample_c __50190401_lsl_pull_sample_c
#define lsl_pull_sample_d __57190401_lsl_pull_sample_d
#define lsl_pull_sample_f __55190401_lsl_pull_sample_f
#define lsl_pull_sample_i __5a190401_lsl_pull_sample_i
#define lsl_pull_sample_l __5f190401_lsl_pull_sample_l
#define lsl_pull_sample_s __40190401_lsl_pull_sample_s
#define lsl_pull_sample_str __406d7601_lsl_pull_sample_str
#define lsl_pull_sample_v __45190401_lsl_pull_sample_v
#define lsl_push_chunk_buf __58682820_lsl_push_chunk_buf
#define lsl_push_chunk_buft __58685c20_lsl_push_chunk_buft
#define lsl_push_chunk_buftn __58685c4e_lsl_push_chunk_buftn
#define lsl_push_chunk_buftnp __28685c4e_lsl_push_chunk_buftnp
#define lsl_push_chunk_buftp __58685c50_lsl_push_chunk_buftp
#define lsl_push_chunk_c __2d0e2821_lsl_push_chunk_c
#define lsl_push_chunk_ct __590e2821_lsl_push_chunk_ct
#define lsl_push_chunk_ctn __59602821_lsl_push_chunk_ctn
#define lsl_push_chunk_ctnp __59605821_lsl_push_chunk_ctnp
#define lsl_push_chunk_ctp __597e2821_lsl_push_chunk_ctp
#define lsl_push_chunk_d __2d0e2826_lsl_push_chunk_d
#define lsl_push_chunk_dt __590e2826_lsl_push_chunk_dt
#define lsl_push_chunk_dtn __59602826_lsl_push_chunk_dtn
#define lsl_push_chunk_dtnp __59605826_lsl_push_chunk_dtnp
#define lsl_push_chunk_dtp __597e2826_lsl_push_chunk_dtp
#define lsl_push_chunk_f __2d0e2824_lsl_push_chunk_f
#define lsl_push_chunk_ft __590e2824_lsl_push_chunk_ft
#define lsl_push_chunk_ftn __59602824_lsl_push_chunk_ftn
#define lsl_push_chunk_ftnp __59605824_lsl_push_chunk_ftnp
#define lsl_push_chunk_ftp __597e2824_lsl_push_chunk_ftp
#define lsl_push_chunk_i __2d0e282b_lsl_push_chunk_i
#define lsl_push_chunk_it __590e282b_lsl_push_chunk_it
#define lsl_push_chunk_itn __5960282b_lsl_push_chunk_itn
#define lsl_push_chunk_itnp __5960582b_lsl_push_chunk_itnp
#define lsl_push_chunk_itp __597e282b_lsl_push_chunk_itp
#define lsl_push_chunk_l __2d0e282e_lsl_push_chunk_l
#define lsl_push_chunk_lt __590e282e_lsl_push_chunk_lt
#define lsl_push_chunk_ltn __5960282e_lsl_push_chunk_ltn
#define lsl_push_chunk_ltnp __5960582e_lsl_push_chunk_ltnp
#define lsl_push_chunk_ltp __597e282e_lsl_push_chunk_ltp
#define lsl_push_chunk_s __2d0e2831_lsl_push_chunk_s
#define lsl_push_chunk_st __590e2831_lsl_push_chunk_st
#define lsl_push_chunk_stn __59602831_lsl_push_chunk_stn
#define lsl_push_chunk_stnp __59605831_lsl_push_chunk_stnp
#define lsl_push_chunk_stp __597e2831_lsl_push_chunk_stp
#define lsl_push_chunk_str __597c2831_lsl_push_chunk_str
#define lsl_push_chunk_strt __597c5c31_lsl_push_chunk_strt
#define lsl_push_chunk_strtn __597c5c5f_lsl_push_chunk_strtn
#define lsl_push_chunk_strtnp __297c5c5f_lsl_push_chunk_strtnp
#define lsl_push_chunk_strtp __597c5c41_lsl_push_chunk_strtp
#define lsl_push_sample_buf __516c7d05_lsl_push_sample_buf
#define lsl_push_sample_buft __516c7d71_lsl_push_sample_buft
#define lsl_push_sample_buftp __216c7d71_lsl_push_sample_buftp
#define lsl_push_sample_c __50191b05_lsl_push_sample_c
#define lsl_push_sample_ct __506d1b05_lsl_push_sample_ct
#define lsl_push_sample_ctp __506d6b05_lsl_push_sample_ctp
#define lsl_push_sample_d __57191b05_lsl_push_sample_d
#define lsl_push_sample_dt __576d1b05_lsl_push_sample_dt
#define lsl_push_sample_dtp __576d6b05_lsl_push_sample_dtp
#define lsl_push_sample_f __55191b05_lsl_push_sample_f
#define lsl_push_sample_ft __556d1b05_lsl_push_sample_ft
#define lsl_push_sample_ftp __556d6b05_lsl_push_sample_ftp
#define lsl_push_sample_i __5a191b05_lsl_push_sample_i
#define lsl_push_sample_it __5a6d1b05_lsl_push_sample_it
#define lsl_push_sample_itp __5a6d6b05_lsl_push_sample_itp
#define lsl_push_sample_l __5f191b05_lsl_push_sample_l
#define lsl_push_sample_lt __5f6d1b05_lsl_push_sample_lt
#define lsl_push_sample_ltp __5f6d6b05_lsl_push_sample_ltp
#define lsl_push_sample_s __40191b05_lsl_push_sample_s
#define lsl_push_sample_st __406d1b05_lsl_push_sample_st
#define lsl_push_sample_stp __406d6b05_lsl_push_sample_stp
#define lsl_push_sample_str __406d6905_lsl_push_sample_str
#define lsl_push_sample_strt __406d6971_lsl_push_sample_strt
#define lsl_push_sample_strtp __306d6971_lsl_push_sample_strtp
#define lsl_push_sample_v __45191b05_lsl_push_sample_v
#define lsl_push_sample_vt __456d1b05_lsl_push_sample_vt
#define lsl_push_sample_vtp __456d6b05_lsl_push_sample_vtp
#define lsl_remove_child __001a3237_lsl_remove_child
#define lsl_remove_child_n __5f743237_lsl_remove_child_n
#define lsl_resolve_all __130c166f_lsl_resolve_all
#define lsl_resolve_bypred __757d0a1d_lsl_resolve_bypred
#define lsl_resolve_byprop __7f690a1d_lsl_resolve_byprop
#define lsl_resolver_results __587e6b42_lsl_resolver_results
#define lsl_samples_available __1b607175_lsl_samples_available
#define lsl_set_child_value __7454620d_lsl_set_child_value
#define lsl_set_name __71777565_lsl_set_name
#define lsl_set_postprocessing __141f777e_lsl_set_postprocessing
#define lsl_set_value __0c777475_lsl_set_value
#define lsl_smoothing_halftime __0d2a7656_lsl_smoothing_halftime
#define lsl_streaminfo_from_xml __1207296d_lsl_streaminfo_from_xml
#define lsl_time_correction __5c73633c_lsl_time_correction
#define lsl_time_correction_ex __390b6363_lsl_time_correction_ex
#define lsl_value __7f12002a_lsl_value
#define lsl_wait_for_consumers __1a116852_lsl_wait_for_consumers
#define lsl_was_clock_reset __60447606_lsl_was_clock_reset

#define __declspec(x)
#endif // STATIC_LIBLIBLSL64

#include "lsl_c.h"

#ifndef STATIC_LIBLIBLSL64
#undef __declspec
// Use #undef to restore function names before declaring
// function pointers with the names originally used to
// declare imports.
#undef lsl_append_child
extern decltype(&__0b0e2f3d_lsl_append_child) lsl_append_child;
#undef lsl_append_child_value
extern decltype(&__211d4e51_lsl_append_child_value) lsl_append_child_value;
#undef lsl_append_copy
extern decltype(&__0c173a59_lsl_append_copy) lsl_append_copy;
#undef lsl_child
extern decltype(&__6b1b0533_lsl_child) lsl_child;
#undef lsl_child_value
extern decltype(&__07311652_lsl_child_value) lsl_child_value;
#undef lsl_child_value_n
extern decltype(&__6931160d_lsl_child_value_n) lsl_child_value_n;
#undef lsl_close_stream
extern decltype(&__18251135_lsl_close_stream) lsl_close_stream;
#undef lsl_copy_streaminfo
extern decltype(&__5b686a3d_lsl_copy_streaminfo) lsl_copy_streaminfo;
#undef lsl_create_continuous_resolver
extern decltype(&__1f3d5352_lsl_create_continuous_resolver) lsl_create_continuous_resolver;
#undef lsl_create_continuous_resolver_bypred
extern decltype(&__024d7e55_lsl_create_continuous_resolver_bypred) lsl_create_continuous_resolver_bypred;
#undef lsl_create_continuous_resolver_byprop
extern decltype(&__164d7e5f_lsl_create_continuous_resolver_byprop) lsl_create_continuous_resolver_byprop;
#undef lsl_create_inlet
extern decltype(&__15083323_lsl_create_inlet) lsl_create_inlet;
#undef lsl_create_outlet
extern decltype(&__7a103a34_lsl_create_outlet) lsl_create_outlet;
#undef lsl_create_streaminfo
extern decltype(&__0d7f5d4a_lsl_create_streaminfo) lsl_create_streaminfo;
#undef lsl_destroy_continuous_resolver
extern decltype(&__161d3f71_lsl_destroy_continuous_resolver) lsl_destroy_continuous_resolver;
#undef lsl_destroy_inlet
extern decltype(&__67170a11_lsl_destroy_inlet) lsl_destroy_inlet;
#undef lsl_destroy_outlet
extern decltype(&__70781218_lsl_destroy_outlet) lsl_destroy_outlet;
#undef lsl_destroy_streaminfo
extern decltype(&__0e0f7d7f_lsl_destroy_streaminfo) lsl_destroy_streaminfo;
#undef lsl_destroy_string
extern decltype(&__676a141d_lsl_destroy_string) lsl_destroy_string;
#undef lsl_empty
extern decltype(&__701e1c2b_lsl_empty) lsl_empty;
#undef lsl_first_child
extern decltype(&__17291944_lsl_first_child) lsl_first_child;
#undef lsl_get_channel_bytes
extern decltype(&__17626154_lsl_get_channel_bytes) lsl_get_channel_bytes;
#undef lsl_get_channel_count
extern decltype(&__1174605f_lsl_get_channel_count) lsl_get_channel_count;
#undef lsl_get_channel_format
extern decltype(&__0100675c_lsl_get_channel_format) lsl_get_channel_format;
#undef lsl_get_created_at
extern decltype(&__7d75193e_lsl_get_created_at) lsl_get_created_at;
#undef lsl_get_desc
extern decltype(&__6f736b63_lsl_get_desc) lsl_get_desc;
#undef lsl_get_fullinfo
extern decltype(&__040d1203_lsl_get_fullinfo) lsl_get_fullinfo;
#undef lsl_get_hostname
extern decltype(&__0d180611_lsl_get_hostname) lsl_get_hostname;
#undef lsl_get_info
extern decltype(&__62787e6f_lsl_get_info) lsl_get_info;
#undef lsl_get_name
extern decltype(&__65777565_lsl_get_name) lsl_get_name;
#undef lsl_get_nominal_srate
extern decltype(&__1d6a7842_lsl_get_nominal_srate) lsl_get_nominal_srate;
#undef lsl_get_sample_bytes
extern decltype(&__6d664f61_lsl_get_sample_bytes) lsl_get_sample_bytes;
#undef lsl_get_session_id
extern decltype(&__7878052c_lsl_get_session_id) lsl_get_session_id;
#undef lsl_get_source_id
extern decltype(&__7f1c321b_lsl_get_source_id) lsl_get_source_id;
#undef lsl_get_type
extern decltype(&__7f6f6865_lsl_get_type) lsl_get_type;
#undef lsl_get_uid
extern decltype(&__7e7f7c00_lsl_get_uid) lsl_get_uid;
#undef lsl_get_version
extern decltype(&__141c0473_lsl_get_version) lsl_get_version;
#undef lsl_get_xml
extern decltype(&__737b7400_lsl_get_xml) lsl_get_xml;
#undef lsl_have_consumers
extern decltype(&__5a771831_lsl_have_consumers) lsl_have_consumers;
#undef lsl_is_text
extern decltype(&__6078472b_lsl_is_text) lsl_is_text;
#undef lsl_last_child
extern decltype(&__33157742_lsl_last_child) lsl_last_child;
#undef lsl_library_version
extern decltype(&__7e626b01_lsl_library_version) lsl_library_version;
#undef lsl_local_clock
extern decltype(&__03200752_lsl_local_clock) lsl_local_clock;
#undef lsl_name
extern decltype(&__0212013a_lsl_name) lsl_name;
#undef lsl_next_sibling
extern decltype(&__310c132e_lsl_next_sibling) lsl_next_sibling;
#undef lsl_next_sibling_n
extern decltype(&__6e62132e_lsl_next_sibling_n) lsl_next_sibling_n;
#undef lsl_open_stream
extern decltype(&__39111043_lsl_open_stream) lsl_open_stream;
#undef lsl_parent
extern decltype(&__72661e3a_lsl_parent) lsl_parent;
#undef lsl_prepend_child
extern decltype(&__7e07041c_lsl_prepend_child) lsl_prepend_child;
#undef lsl_prepend_child_value
extern decltype(&__122d177d_lsl_prepend_child_value) lsl_prepend_child_value;
#undef lsl_prepend_copy
extern decltype(&__1a001d09_lsl_prepend_copy) lsl_prepend_copy;
#undef lsl_previous_sibling
extern decltype(&__46747b5f_lsl_previous_sibling) lsl_previous_sibling;
#undef lsl_previous_sibling_n
extern decltype(&__191a7b5f_lsl_previous_sibling_n) lsl_previous_sibling_n;
#undef lsl_protocol_version
extern decltype(&__5f7d665b_lsl_protocol_version) lsl_protocol_version;
#undef lsl_pull_chunk_buf
extern decltype(&__58683724_lsl_pull_chunk_buf) lsl_pull_chunk_buf;
#undef lsl_pull_chunk_c
extern decltype(&__2d0e3725_lsl_pull_chunk_c) lsl_pull_chunk_c;
#undef lsl_pull_chunk_d
extern decltype(&__2d0e3722_lsl_pull_chunk_d) lsl_pull_chunk_d;
#undef lsl_pull_chunk_f
extern decltype(&__2d0e3720_lsl_pull_chunk_f) lsl_pull_chunk_f;
#undef lsl_pull_chunk_i
extern decltype(&__2d0e372f_lsl_pull_chunk_i) lsl_pull_chunk_i;
#undef lsl_pull_chunk_l
extern decltype(&__2d0e372a_lsl_pull_chunk_l) lsl_pull_chunk_l;
#undef lsl_pull_chunk_s
extern decltype(&__2d0e3735_lsl_pull_chunk_s) lsl_pull_chunk_s;
#undef lsl_pull_chunk_str
extern decltype(&__597c3735_lsl_pull_chunk_str) lsl_pull_chunk_str;
#undef lsl_pull_sample_buf
extern decltype(&__516c6201_lsl_pull_sample_buf) lsl_pull_sample_buf;
#undef lsl_pull_sample_c
extern decltype(&__50190401_lsl_pull_sample_c) lsl_pull_sample_c;
#undef lsl_pull_sample_d
extern decltype(&__57190401_lsl_pull_sample_d) lsl_pull_sample_d;
#undef lsl_pull_sample_f
extern decltype(&__55190401_lsl_pull_sample_f) lsl_pull_sample_f;
#undef lsl_pull_sample_i
extern decltype(&__5a190401_lsl_pull_sample_i) lsl_pull_sample_i;
#undef lsl_pull_sample_l
extern decltype(&__5f190401_lsl_pull_sample_l) lsl_pull_sample_l;
#undef lsl_pull_sample_s
extern decltype(&__40190401_lsl_pull_sample_s) lsl_pull_sample_s;
#undef lsl_pull_sample_str
extern decltype(&__406d7601_lsl_pull_sample_str) lsl_pull_sample_str;
#undef lsl_pull_sample_v
extern decltype(&__45190401_lsl_pull_sample_v) lsl_pull_sample_v;
#undef lsl_push_chunk_buf
extern decltype(&__58682820_lsl_push_chunk_buf) lsl_push_chunk_buf;
#undef lsl_push_chunk_buft
extern decltype(&__58685c20_lsl_push_chunk_buft) lsl_push_chunk_buft;
#undef lsl_push_chunk_buftn
extern decltype(&__58685c4e_lsl_push_chunk_buftn) lsl_push_chunk_buftn;
#undef lsl_push_chunk_buftnp
extern decltype(&__28685c4e_lsl_push_chunk_buftnp) lsl_push_chunk_buftnp;
#undef lsl_push_chunk_buftp
extern decltype(&__58685c50_lsl_push_chunk_buftp) lsl_push_chunk_buftp;
#undef lsl_push_chunk_c
extern decltype(&__2d0e2821_lsl_push_chunk_c) lsl_push_chunk_c;
#undef lsl_push_chunk_ct
extern decltype(&__590e2821_lsl_push_chunk_ct) lsl_push_chunk_ct;
#undef lsl_push_chunk_ctn
extern decltype(&__59602821_lsl_push_chunk_ctn) lsl_push_chunk_ctn;
#undef lsl_push_chunk_ctnp
extern decltype(&__59605821_lsl_push_chunk_ctnp) lsl_push_chunk_ctnp;
#undef lsl_push_chunk_ctp
extern decltype(&__597e2821_lsl_push_chunk_ctp) lsl_push_chunk_ctp;
#undef lsl_push_chunk_d
extern decltype(&__2d0e2826_lsl_push_chunk_d) lsl_push_chunk_d;
#undef lsl_push_chunk_dt
extern decltype(&__590e2826_lsl_push_chunk_dt) lsl_push_chunk_dt;
#undef lsl_push_chunk_dtn
extern decltype(&__59602826_lsl_push_chunk_dtn) lsl_push_chunk_dtn;
#undef lsl_push_chunk_dtnp
extern decltype(&__59605826_lsl_push_chunk_dtnp) lsl_push_chunk_dtnp;
#undef lsl_push_chunk_dtp
extern decltype(&__597e2826_lsl_push_chunk_dtp) lsl_push_chunk_dtp;
#undef lsl_push_chunk_f
extern decltype(&__2d0e2824_lsl_push_chunk_f) lsl_push_chunk_f;
#undef lsl_push_chunk_ft
extern decltype(&__590e2824_lsl_push_chunk_ft) lsl_push_chunk_ft;
#undef lsl_push_chunk_ftn
extern decltype(&__59602824_lsl_push_chunk_ftn) lsl_push_chunk_ftn;
#undef lsl_push_chunk_ftnp
extern decltype(&__59605824_lsl_push_chunk_ftnp) lsl_push_chunk_ftnp;
#undef lsl_push_chunk_ftp
extern decltype(&__597e2824_lsl_push_chunk_ftp) lsl_push_chunk_ftp;
#undef lsl_push_chunk_i
extern decltype(&__2d0e282b_lsl_push_chunk_i) lsl_push_chunk_i;
#undef lsl_push_chunk_it
extern decltype(&__590e282b_lsl_push_chunk_it) lsl_push_chunk_it;
#undef lsl_push_chunk_itn
extern decltype(&__5960282b_lsl_push_chunk_itn) lsl_push_chunk_itn;
#undef lsl_push_chunk_itnp
extern decltype(&__5960582b_lsl_push_chunk_itnp) lsl_push_chunk_itnp;
#undef lsl_push_chunk_itp
extern decltype(&__597e282b_lsl_push_chunk_itp) lsl_push_chunk_itp;
#undef lsl_push_chunk_l
extern decltype(&__2d0e282e_lsl_push_chunk_l) lsl_push_chunk_l;
#undef lsl_push_chunk_lt
extern decltype(&__590e282e_lsl_push_chunk_lt) lsl_push_chunk_lt;
#undef lsl_push_chunk_ltn
extern decltype(&__5960282e_lsl_push_chunk_ltn) lsl_push_chunk_ltn;
#undef lsl_push_chunk_ltnp
extern decltype(&__5960582e_lsl_push_chunk_ltnp) lsl_push_chunk_ltnp;
#undef lsl_push_chunk_ltp
extern decltype(&__597e282e_lsl_push_chunk_ltp) lsl_push_chunk_ltp;
#undef lsl_push_chunk_s
extern decltype(&__2d0e2831_lsl_push_chunk_s) lsl_push_chunk_s;
#undef lsl_push_chunk_st
extern decltype(&__590e2831_lsl_push_chunk_st) lsl_push_chunk_st;
#undef lsl_push_chunk_stn
extern decltype(&__59602831_lsl_push_chunk_stn) lsl_push_chunk_stn;
#undef lsl_push_chunk_stnp
extern decltype(&__59605831_lsl_push_chunk_stnp) lsl_push_chunk_stnp;
#undef lsl_push_chunk_stp
extern decltype(&__597e2831_lsl_push_chunk_stp) lsl_push_chunk_stp;
#undef lsl_push_chunk_str
extern decltype(&__597c2831_lsl_push_chunk_str) lsl_push_chunk_str;
#undef lsl_push_chunk_strt
extern decltype(&__597c5c31_lsl_push_chunk_strt) lsl_push_chunk_strt;
#undef lsl_push_chunk_strtn
extern decltype(&__597c5c5f_lsl_push_chunk_strtn) lsl_push_chunk_strtn;
#undef lsl_push_chunk_strtnp
extern decltype(&__297c5c5f_lsl_push_chunk_strtnp) lsl_push_chunk_strtnp;
#undef lsl_push_chunk_strtp
extern decltype(&__597c5c41_lsl_push_chunk_strtp) lsl_push_chunk_strtp;
#undef lsl_push_sample_buf
extern decltype(&__516c7d05_lsl_push_sample_buf) lsl_push_sample_buf;
#undef lsl_push_sample_buft
extern decltype(&__516c7d71_lsl_push_sample_buft) lsl_push_sample_buft;
#undef lsl_push_sample_buftp
extern decltype(&__216c7d71_lsl_push_sample_buftp) lsl_push_sample_buftp;
#undef lsl_push_sample_c
extern decltype(&__50191b05_lsl_push_sample_c) lsl_push_sample_c;
#undef lsl_push_sample_ct
extern decltype(&__506d1b05_lsl_push_sample_ct) lsl_push_sample_ct;
#undef lsl_push_sample_ctp
extern decltype(&__506d6b05_lsl_push_sample_ctp) lsl_push_sample_ctp;
#undef lsl_push_sample_d
extern decltype(&__57191b05_lsl_push_sample_d) lsl_push_sample_d;
#undef lsl_push_sample_dt
extern decltype(&__576d1b05_lsl_push_sample_dt) lsl_push_sample_dt;
#undef lsl_push_sample_dtp
extern decltype(&__576d6b05_lsl_push_sample_dtp) lsl_push_sample_dtp;
#undef lsl_push_sample_f
extern decltype(&__55191b05_lsl_push_sample_f) lsl_push_sample_f;
#undef lsl_push_sample_ft
extern decltype(&__556d1b05_lsl_push_sample_ft) lsl_push_sample_ft;
#undef lsl_push_sample_ftp
extern decltype(&__556d6b05_lsl_push_sample_ftp) lsl_push_sample_ftp;
#undef lsl_push_sample_i
extern decltype(&__5a191b05_lsl_push_sample_i) lsl_push_sample_i;
#undef lsl_push_sample_it
extern decltype(&__5a6d1b05_lsl_push_sample_it) lsl_push_sample_it;
#undef lsl_push_sample_itp
extern decltype(&__5a6d6b05_lsl_push_sample_itp) lsl_push_sample_itp;
#undef lsl_push_sample_l
extern decltype(&__5f191b05_lsl_push_sample_l) lsl_push_sample_l;
#undef lsl_push_sample_lt
extern decltype(&__5f6d1b05_lsl_push_sample_lt) lsl_push_sample_lt;
#undef lsl_push_sample_ltp
extern decltype(&__5f6d6b05_lsl_push_sample_ltp) lsl_push_sample_ltp;
#undef lsl_push_sample_s
extern decltype(&__40191b05_lsl_push_sample_s) lsl_push_sample_s;
#undef lsl_push_sample_st
extern decltype(&__406d1b05_lsl_push_sample_st) lsl_push_sample_st;
#undef lsl_push_sample_stp
extern decltype(&__406d6b05_lsl_push_sample_stp) lsl_push_sample_stp;
#undef lsl_push_sample_str
extern decltype(&__406d6905_lsl_push_sample_str) lsl_push_sample_str;
#undef lsl_push_sample_strt
extern decltype(&__406d6971_lsl_push_sample_strt) lsl_push_sample_strt;
#undef lsl_push_sample_strtp
extern decltype(&__306d6971_lsl_push_sample_strtp) lsl_push_sample_strtp;
#undef lsl_push_sample_v
extern decltype(&__45191b05_lsl_push_sample_v) lsl_push_sample_v;
#undef lsl_push_sample_vt
extern decltype(&__456d1b05_lsl_push_sample_vt) lsl_push_sample_vt;
#undef lsl_push_sample_vtp
extern decltype(&__456d6b05_lsl_push_sample_vtp) lsl_push_sample_vtp;
#undef lsl_remove_child
extern decltype(&__001a3237_lsl_remove_child) lsl_remove_child;
#undef lsl_remove_child_n
extern decltype(&__5f743237_lsl_remove_child_n) lsl_remove_child_n;
#undef lsl_resolve_all
extern decltype(&__130c166f_lsl_resolve_all) lsl_resolve_all;
#undef lsl_resolve_bypred
extern decltype(&__757d0a1d_lsl_resolve_bypred) lsl_resolve_bypred;
#undef lsl_resolve_byprop
extern decltype(&__7f690a1d_lsl_resolve_byprop) lsl_resolve_byprop;
#undef lsl_resolver_results
extern decltype(&__587e6b42_lsl_resolver_results) lsl_resolver_results;
#undef lsl_samples_available
extern decltype(&__1b607175_lsl_samples_available) lsl_samples_available;
#undef lsl_set_child_value
extern decltype(&__7454620d_lsl_set_child_value) lsl_set_child_value;
#undef lsl_set_name
extern decltype(&__71777565_lsl_set_name) lsl_set_name;
#undef lsl_set_postprocessing
extern decltype(&__141f777e_lsl_set_postprocessing) lsl_set_postprocessing;
#undef lsl_set_value
extern decltype(&__0c777475_lsl_set_value) lsl_set_value;
#undef lsl_smoothing_halftime
extern decltype(&__0d2a7656_lsl_smoothing_halftime) lsl_smoothing_halftime;
#undef lsl_streaminfo_from_xml
extern decltype(&__1207296d_lsl_streaminfo_from_xml) lsl_streaminfo_from_xml;
#undef lsl_time_correction
extern decltype(&__5c73633c_lsl_time_correction) lsl_time_correction;
#undef lsl_time_correction_ex
extern decltype(&__390b6363_lsl_time_correction_ex) lsl_time_correction_ex;
#undef lsl_value
extern decltype(&__7f12002a_lsl_value) lsl_value;
#undef lsl_wait_for_consumers
extern decltype(&__1a116852_lsl_wait_for_consumers) lsl_wait_for_consumers;
#undef lsl_was_clock_reset
extern decltype(&__60447606_lsl_was_clock_reset) lsl_was_clock_reset;

#endif // STATIC_LIBLIBLSL64

namespace Dylib { bool liblsl64_Loaded(); }

#endif // LIBLSL64_IMPORTS_H

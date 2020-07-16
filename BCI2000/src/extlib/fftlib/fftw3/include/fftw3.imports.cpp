// Import declarations for libfftw3-3 library
// generated Tue Mar 15 14:08:05 2016 by DylibTool
#if STATIC_LIBFFTW3

namespace Dylib { bool fftw3_Loaded() { return true; } }
extern "C" {
void* p_fftw_cc = &fftw_cc;
void* p_fftw_codelet_optim = &fftw_codelet_optim;
void* p_fftw_version = &fftw_version;
}

#else

#include "DylibImports.h"
namespace { extern const Dylib::Import* functionImports; }

// Here you may specify a custom error message to be displayed when the library cannot be found.
static const char* notFoundMsg = "";
// Here you may specify an URL to some local or remote help resource.
static const char* notFoundURL = "";
RegisterDylibWithAliases( fftw3, "libfftw3-3", functionImports, notFoundMsg, notFoundURL );

extern "C" {
void* p_fftw_cc = Dylib::fftw3.ResolveDataExport( "fftw_cc" );
void* p_fftw_codelet_optim = Dylib::fftw3.ResolveDataExport( "fftw_codelet_optim" );
void* p_fftw_version = Dylib::fftw3.ResolveDataExport( "fftw_version" );

// NOTE: When loading the library, function bodies will be overwritten with absolute jumps
// to imported functions, i.e. function bodies must be roughly 2 * sizeof(void*) bytes long.
// A call to a member function with a pointer argument should be large enough.
//
// In addition, having distinct function bodies for each function stub
// is important -- that makes it impossible for size optimizers to
// collapse them into a single definition.
//
void fftw_alignment_of() { Dylib::fftw3.UninitializedFunctionExport( "fftw_alignment_of" ); }
void fftw_alloc_complex() { Dylib::fftw3.UninitializedFunctionExport( "fftw_alloc_complex" ); }
void fftw_alloc_real() { Dylib::fftw3.UninitializedFunctionExport( "fftw_alloc_real" ); }
void fftw_assertion_failed() { Dylib::fftw3.UninitializedFunctionExport( "fftw_assertion_failed" ); }
void fftw_cleanup() { Dylib::fftw3.UninitializedFunctionExport( "fftw_cleanup" ); }
void fftw_cleanup_threads() { Dylib::fftw3.UninitializedFunctionExport( "fftw_cleanup_threads" ); }
void fftw_cost() { Dylib::fftw3.UninitializedFunctionExport( "fftw_cost" ); }
void fftw_destroy_plan() { Dylib::fftw3.UninitializedFunctionExport( "fftw_destroy_plan" ); }
void fftw_estimate_cost() { Dylib::fftw3.UninitializedFunctionExport( "fftw_estimate_cost" ); }
void fftw_execute() { Dylib::fftw3.UninitializedFunctionExport( "fftw_execute" ); }
void fftw_execute_dft() { Dylib::fftw3.UninitializedFunctionExport( "fftw_execute_dft" ); }
void fftw_execute_dft_c2r() { Dylib::fftw3.UninitializedFunctionExport( "fftw_execute_dft_c2r" ); }
void fftw_execute_dft_r2c() { Dylib::fftw3.UninitializedFunctionExport( "fftw_execute_dft_r2c" ); }
void fftw_execute_r2r() { Dylib::fftw3.UninitializedFunctionExport( "fftw_execute_r2r" ); }
void fftw_execute_split_dft() { Dylib::fftw3.UninitializedFunctionExport( "fftw_execute_split_dft" ); }
void fftw_execute_split_dft_c2r() { Dylib::fftw3.UninitializedFunctionExport( "fftw_execute_split_dft_c2r" ); }
void fftw_execute_split_dft_r2c() { Dylib::fftw3.UninitializedFunctionExport( "fftw_execute_split_dft_r2c" ); }
void fftw_export_wisdom() { Dylib::fftw3.UninitializedFunctionExport( "fftw_export_wisdom" ); }
void fftw_export_wisdom_to_file() { Dylib::fftw3.UninitializedFunctionExport( "fftw_export_wisdom_to_file" ); }
void fftw_export_wisdom_to_filename() { Dylib::fftw3.UninitializedFunctionExport( "fftw_export_wisdom_to_filename" ); }
void fftw_export_wisdom_to_string() { Dylib::fftw3.UninitializedFunctionExport( "fftw_export_wisdom_to_string" ); }
void fftw_flops() { Dylib::fftw3.UninitializedFunctionExport( "fftw_flops" ); }
void fftw_forget_wisdom() { Dylib::fftw3.UninitializedFunctionExport( "fftw_forget_wisdom" ); }
void fftw_fprint_plan() { Dylib::fftw3.UninitializedFunctionExport( "fftw_fprint_plan" ); }
void fftw_free() { Dylib::fftw3.UninitializedFunctionExport( "fftw_free" ); }
void fftw_ifree() { Dylib::fftw3.UninitializedFunctionExport( "fftw_ifree" ); }
void fftw_import_system_wisdom() { Dylib::fftw3.UninitializedFunctionExport( "fftw_import_system_wisdom" ); }
void fftw_import_wisdom() { Dylib::fftw3.UninitializedFunctionExport( "fftw_import_wisdom" ); }
void fftw_import_wisdom_from_file() { Dylib::fftw3.UninitializedFunctionExport( "fftw_import_wisdom_from_file" ); }
void fftw_import_wisdom_from_filename() { Dylib::fftw3.UninitializedFunctionExport( "fftw_import_wisdom_from_filename" ); }
void fftw_import_wisdom_from_string() { Dylib::fftw3.UninitializedFunctionExport( "fftw_import_wisdom_from_string" ); }
void fftw_init_threads() { Dylib::fftw3.UninitializedFunctionExport( "fftw_init_threads" ); }
void fftw_malloc() { Dylib::fftw3.UninitializedFunctionExport( "fftw_malloc" ); }
void fftw_malloc_plain() { Dylib::fftw3.UninitializedFunctionExport( "fftw_malloc_plain" ); }
void fftw_mkprinter_file() { Dylib::fftw3.UninitializedFunctionExport( "fftw_mkprinter_file" ); }
void fftw_plan_awake() { Dylib::fftw3.UninitializedFunctionExport( "fftw_plan_awake" ); }
void fftw_plan_dft() { Dylib::fftw3.UninitializedFunctionExport( "fftw_plan_dft" ); }
void fftw_plan_dft_1d() { Dylib::fftw3.UninitializedFunctionExport( "fftw_plan_dft_1d" ); }
void fftw_plan_dft_2d() { Dylib::fftw3.UninitializedFunctionExport( "fftw_plan_dft_2d" ); }
void fftw_plan_dft_3d() { Dylib::fftw3.UninitializedFunctionExport( "fftw_plan_dft_3d" ); }
void fftw_plan_dft_c2r() { Dylib::fftw3.UninitializedFunctionExport( "fftw_plan_dft_c2r" ); }
void fftw_plan_dft_c2r_1d() { Dylib::fftw3.UninitializedFunctionExport( "fftw_plan_dft_c2r_1d" ); }
void fftw_plan_dft_c2r_2d() { Dylib::fftw3.UninitializedFunctionExport( "fftw_plan_dft_c2r_2d" ); }
void fftw_plan_dft_c2r_3d() { Dylib::fftw3.UninitializedFunctionExport( "fftw_plan_dft_c2r_3d" ); }
void fftw_plan_dft_r2c() { Dylib::fftw3.UninitializedFunctionExport( "fftw_plan_dft_r2c" ); }
void fftw_plan_dft_r2c_1d() { Dylib::fftw3.UninitializedFunctionExport( "fftw_plan_dft_r2c_1d" ); }
void fftw_plan_dft_r2c_2d() { Dylib::fftw3.UninitializedFunctionExport( "fftw_plan_dft_r2c_2d" ); }
void fftw_plan_dft_r2c_3d() { Dylib::fftw3.UninitializedFunctionExport( "fftw_plan_dft_r2c_3d" ); }
void fftw_plan_guru64_dft() { Dylib::fftw3.UninitializedFunctionExport( "fftw_plan_guru64_dft" ); }
void fftw_plan_guru64_dft_c2r() { Dylib::fftw3.UninitializedFunctionExport( "fftw_plan_guru64_dft_c2r" ); }
void fftw_plan_guru64_dft_r2c() { Dylib::fftw3.UninitializedFunctionExport( "fftw_plan_guru64_dft_r2c" ); }
void fftw_plan_guru64_r2r() { Dylib::fftw3.UninitializedFunctionExport( "fftw_plan_guru64_r2r" ); }
void fftw_plan_guru64_split_dft() { Dylib::fftw3.UninitializedFunctionExport( "fftw_plan_guru64_split_dft" ); }
void fftw_plan_guru64_split_dft_c2r() { Dylib::fftw3.UninitializedFunctionExport( "fftw_plan_guru64_split_dft_c2r" ); }
void fftw_plan_guru64_split_dft_r2c() { Dylib::fftw3.UninitializedFunctionExport( "fftw_plan_guru64_split_dft_r2c" ); }
void fftw_plan_guru_dft() { Dylib::fftw3.UninitializedFunctionExport( "fftw_plan_guru_dft" ); }
void fftw_plan_guru_dft_c2r() { Dylib::fftw3.UninitializedFunctionExport( "fftw_plan_guru_dft_c2r" ); }
void fftw_plan_guru_dft_r2c() { Dylib::fftw3.UninitializedFunctionExport( "fftw_plan_guru_dft_r2c" ); }
void fftw_plan_guru_r2r() { Dylib::fftw3.UninitializedFunctionExport( "fftw_plan_guru_r2r" ); }
void fftw_plan_guru_split_dft() { Dylib::fftw3.UninitializedFunctionExport( "fftw_plan_guru_split_dft" ); }
void fftw_plan_guru_split_dft_c2r() { Dylib::fftw3.UninitializedFunctionExport( "fftw_plan_guru_split_dft_c2r" ); }
void fftw_plan_guru_split_dft_r2c() { Dylib::fftw3.UninitializedFunctionExport( "fftw_plan_guru_split_dft_r2c" ); }
void fftw_plan_many_dft() { Dylib::fftw3.UninitializedFunctionExport( "fftw_plan_many_dft" ); }
void fftw_plan_many_dft_c2r() { Dylib::fftw3.UninitializedFunctionExport( "fftw_plan_many_dft_c2r" ); }
void fftw_plan_many_dft_r2c() { Dylib::fftw3.UninitializedFunctionExport( "fftw_plan_many_dft_r2c" ); }
void fftw_plan_many_r2r() { Dylib::fftw3.UninitializedFunctionExport( "fftw_plan_many_r2r" ); }
void fftw_plan_r2r() { Dylib::fftw3.UninitializedFunctionExport( "fftw_plan_r2r" ); }
void fftw_plan_r2r_1d() { Dylib::fftw3.UninitializedFunctionExport( "fftw_plan_r2r_1d" ); }
void fftw_plan_r2r_2d() { Dylib::fftw3.UninitializedFunctionExport( "fftw_plan_r2r_2d" ); }
void fftw_plan_r2r_3d() { Dylib::fftw3.UninitializedFunctionExport( "fftw_plan_r2r_3d" ); }
void fftw_plan_with_nthreads() { Dylib::fftw3.UninitializedFunctionExport( "fftw_plan_with_nthreads" ); }
void fftw_print_plan() { Dylib::fftw3.UninitializedFunctionExport( "fftw_print_plan" ); }
void fftw_printer_destroy() { Dylib::fftw3.UninitializedFunctionExport( "fftw_printer_destroy" ); }
void fftw_set_timelimit() { Dylib::fftw3.UninitializedFunctionExport( "fftw_set_timelimit" ); }
void fftw_sprint_plan() { Dylib::fftw3.UninitializedFunctionExport( "fftw_sprint_plan" ); }
void fftw_the_planner() { Dylib::fftw3.UninitializedFunctionExport( "fftw_the_planner" ); }
}

namespace {
const Dylib::Import functionImports_[] =
{
  { "fftw_alignment_of", (void**)&fftw_alignment_of, Dylib::Import::functionStub },
  { "fftw_alloc_complex", (void**)&fftw_alloc_complex, Dylib::Import::functionStub },
  { "fftw_alloc_real", (void**)&fftw_alloc_real, Dylib::Import::functionStub },
  { "fftw_assertion_failed", (void**)&fftw_assertion_failed, Dylib::Import::functionStub },
  { "fftw_cleanup", (void**)&fftw_cleanup, Dylib::Import::functionStub },
  { "fftw_cleanup_threads", (void**)&fftw_cleanup_threads, Dylib::Import::functionStub },
  { "fftw_cost", (void**)&fftw_cost, Dylib::Import::functionStub },
  { "fftw_destroy_plan", (void**)&fftw_destroy_plan, Dylib::Import::functionStub },
  { "fftw_estimate_cost", (void**)&fftw_estimate_cost, Dylib::Import::functionStub },
  { "fftw_execute", (void**)&fftw_execute, Dylib::Import::functionStub },
  { "fftw_execute_dft", (void**)&fftw_execute_dft, Dylib::Import::functionStub },
  { "fftw_execute_dft_c2r", (void**)&fftw_execute_dft_c2r, Dylib::Import::functionStub },
  { "fftw_execute_dft_r2c", (void**)&fftw_execute_dft_r2c, Dylib::Import::functionStub },
  { "fftw_execute_r2r", (void**)&fftw_execute_r2r, Dylib::Import::functionStub },
  { "fftw_execute_split_dft", (void**)&fftw_execute_split_dft, Dylib::Import::functionStub },
  { "fftw_execute_split_dft_c2r", (void**)&fftw_execute_split_dft_c2r, Dylib::Import::functionStub },
  { "fftw_execute_split_dft_r2c", (void**)&fftw_execute_split_dft_r2c, Dylib::Import::functionStub },
  { "fftw_export_wisdom", (void**)&fftw_export_wisdom, Dylib::Import::functionStub },
  { "fftw_export_wisdom_to_file", (void**)&fftw_export_wisdom_to_file, Dylib::Import::functionStub },
  { "fftw_export_wisdom_to_filename", (void**)&fftw_export_wisdom_to_filename, Dylib::Import::functionStub },
  { "fftw_export_wisdom_to_string", (void**)&fftw_export_wisdom_to_string, Dylib::Import::functionStub },
  { "fftw_flops", (void**)&fftw_flops, Dylib::Import::functionStub },
  { "fftw_forget_wisdom", (void**)&fftw_forget_wisdom, Dylib::Import::functionStub },
  { "fftw_fprint_plan", (void**)&fftw_fprint_plan, Dylib::Import::functionStub },
  { "fftw_free", (void**)&fftw_free, Dylib::Import::functionStub },
  { "fftw_ifree", (void**)&fftw_ifree, Dylib::Import::functionStub },
  { "fftw_import_system_wisdom", (void**)&fftw_import_system_wisdom, Dylib::Import::functionStub },
  { "fftw_import_wisdom", (void**)&fftw_import_wisdom, Dylib::Import::functionStub },
  { "fftw_import_wisdom_from_file", (void**)&fftw_import_wisdom_from_file, Dylib::Import::functionStub },
  { "fftw_import_wisdom_from_filename", (void**)&fftw_import_wisdom_from_filename, Dylib::Import::functionStub },
  { "fftw_import_wisdom_from_string", (void**)&fftw_import_wisdom_from_string, Dylib::Import::functionStub },
  { "fftw_init_threads", (void**)&fftw_init_threads, Dylib::Import::functionStub },
  { "fftw_malloc", (void**)&fftw_malloc, Dylib::Import::functionStub },
  { "fftw_malloc_plain", (void**)&fftw_malloc_plain, Dylib::Import::functionStub },
  { "fftw_mkprinter_file", (void**)&fftw_mkprinter_file, Dylib::Import::functionStub },
  { "fftw_plan_awake", (void**)&fftw_plan_awake, Dylib::Import::functionStub },
  { "fftw_plan_dft", (void**)&fftw_plan_dft, Dylib::Import::functionStub },
  { "fftw_plan_dft_1d", (void**)&fftw_plan_dft_1d, Dylib::Import::functionStub },
  { "fftw_plan_dft_2d", (void**)&fftw_plan_dft_2d, Dylib::Import::functionStub },
  { "fftw_plan_dft_3d", (void**)&fftw_plan_dft_3d, Dylib::Import::functionStub },
  { "fftw_plan_dft_c2r", (void**)&fftw_plan_dft_c2r, Dylib::Import::functionStub },
  { "fftw_plan_dft_c2r_1d", (void**)&fftw_plan_dft_c2r_1d, Dylib::Import::functionStub },
  { "fftw_plan_dft_c2r_2d", (void**)&fftw_plan_dft_c2r_2d, Dylib::Import::functionStub },
  { "fftw_plan_dft_c2r_3d", (void**)&fftw_plan_dft_c2r_3d, Dylib::Import::functionStub },
  { "fftw_plan_dft_r2c", (void**)&fftw_plan_dft_r2c, Dylib::Import::functionStub },
  { "fftw_plan_dft_r2c_1d", (void**)&fftw_plan_dft_r2c_1d, Dylib::Import::functionStub },
  { "fftw_plan_dft_r2c_2d", (void**)&fftw_plan_dft_r2c_2d, Dylib::Import::functionStub },
  { "fftw_plan_dft_r2c_3d", (void**)&fftw_plan_dft_r2c_3d, Dylib::Import::functionStub },
  { "fftw_plan_guru64_dft", (void**)&fftw_plan_guru64_dft, Dylib::Import::functionStub },
  { "fftw_plan_guru64_dft_c2r", (void**)&fftw_plan_guru64_dft_c2r, Dylib::Import::functionStub },
  { "fftw_plan_guru64_dft_r2c", (void**)&fftw_plan_guru64_dft_r2c, Dylib::Import::functionStub },
  { "fftw_plan_guru64_r2r", (void**)&fftw_plan_guru64_r2r, Dylib::Import::functionStub },
  { "fftw_plan_guru64_split_dft", (void**)&fftw_plan_guru64_split_dft, Dylib::Import::functionStub },
  { "fftw_plan_guru64_split_dft_c2r", (void**)&fftw_plan_guru64_split_dft_c2r, Dylib::Import::functionStub },
  { "fftw_plan_guru64_split_dft_r2c", (void**)&fftw_plan_guru64_split_dft_r2c, Dylib::Import::functionStub },
  { "fftw_plan_guru_dft", (void**)&fftw_plan_guru_dft, Dylib::Import::functionStub },
  { "fftw_plan_guru_dft_c2r", (void**)&fftw_plan_guru_dft_c2r, Dylib::Import::functionStub },
  { "fftw_plan_guru_dft_r2c", (void**)&fftw_plan_guru_dft_r2c, Dylib::Import::functionStub },
  { "fftw_plan_guru_r2r", (void**)&fftw_plan_guru_r2r, Dylib::Import::functionStub },
  { "fftw_plan_guru_split_dft", (void**)&fftw_plan_guru_split_dft, Dylib::Import::functionStub },
  { "fftw_plan_guru_split_dft_c2r", (void**)&fftw_plan_guru_split_dft_c2r, Dylib::Import::functionStub },
  { "fftw_plan_guru_split_dft_r2c", (void**)&fftw_plan_guru_split_dft_r2c, Dylib::Import::functionStub },
  { "fftw_plan_many_dft", (void**)&fftw_plan_many_dft, Dylib::Import::functionStub },
  { "fftw_plan_many_dft_c2r", (void**)&fftw_plan_many_dft_c2r, Dylib::Import::functionStub },
  { "fftw_plan_many_dft_r2c", (void**)&fftw_plan_many_dft_r2c, Dylib::Import::functionStub },
  { "fftw_plan_many_r2r", (void**)&fftw_plan_many_r2r, Dylib::Import::functionStub },
  { "fftw_plan_r2r", (void**)&fftw_plan_r2r, Dylib::Import::functionStub },
  { "fftw_plan_r2r_1d", (void**)&fftw_plan_r2r_1d, Dylib::Import::functionStub },
  { "fftw_plan_r2r_2d", (void**)&fftw_plan_r2r_2d, Dylib::Import::functionStub },
  { "fftw_plan_r2r_3d", (void**)&fftw_plan_r2r_3d, Dylib::Import::functionStub },
  { "fftw_plan_with_nthreads", (void**)&fftw_plan_with_nthreads, Dylib::Import::functionStub },
  { "fftw_print_plan", (void**)&fftw_print_plan, Dylib::Import::functionStub },
  { "fftw_printer_destroy", (void**)&fftw_printer_destroy, Dylib::Import::functionStub },
  { "fftw_set_timelimit", (void**)&fftw_set_timelimit, Dylib::Import::functionStub },
  { "fftw_sprint_plan", (void**)&fftw_sprint_plan, Dylib::Import::functionStub },
  { "fftw_the_planner", (void**)&fftw_the_planner, Dylib::Import::functionStub },
  { 0, 0, 0 }
};
const Dylib::Import* functionImports = functionImports_;
}

#endif // STATIC_LIBFFTW3


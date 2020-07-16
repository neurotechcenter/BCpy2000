// Import declarations for libmx library
// generated Tue Mar 01 12:23:00 2016 by DylibTool
#include "DylibImports.h"
namespace { extern const Dylib::Import* functionImports; }

// Here you may specify a custom error message to be displayed when the library cannot be found.
static const char* notFoundMsg = "";
// Here you may specify an URL to some local or remote help resource.
static const char* notFoundURL = "";
RegisterDylibWithAliases( libmx, "libmx", functionImports, notFoundMsg, notFoundURL );

extern "C" {
// NOTE: When loading the library, function bodies will be overwritten with absolute jumps
// to imported functions, i.e. function bodies must be roughly 2 * sizeof(void*) bytes long.
// A call to a member function with a pointer argument should be large enough.
//
// In addition, having distinct function bodies for each function stub
// is important -- that makes it impossible for size optimizers to
// collapse them into a single definition.
//
void mxAddField() { Dylib::libmx.UninitializedFunctionExport( "mxAddField" ); }
void mxArrayToString() { Dylib::libmx.UninitializedFunctionExport( "mxArrayToString" ); }
void mxArrayToUTF8String() { Dylib::libmx.UninitializedFunctionExport( "mxArrayToUTF8String" ); }
void mxCalcSingleSubscript() { Dylib::libmx.UninitializedFunctionExport( "mxCalcSingleSubscript" ); }
void mxCalcSingleSubscript_700() { Dylib::libmx.UninitializedFunctionExport( "mxCalcSingleSubscript_700" ); }
void mxCalcSingleSubscript_730() { Dylib::libmx.UninitializedFunctionExport( "mxCalcSingleSubscript_730" ); }
void mxCalloc() { Dylib::libmx.UninitializedFunctionExport( "mxCalloc" ); }
void mxCreateCellArray() { Dylib::libmx.UninitializedFunctionExport( "mxCreateCellArray" ); }
void mxCreateCellArray_700() { Dylib::libmx.UninitializedFunctionExport( "mxCreateCellArray_700" ); }
void mxCreateCellArray_730() { Dylib::libmx.UninitializedFunctionExport( "mxCreateCellArray_730" ); }
void mxCreateCellMatrix() { Dylib::libmx.UninitializedFunctionExport( "mxCreateCellMatrix" ); }
void mxCreateCellMatrix_700() { Dylib::libmx.UninitializedFunctionExport( "mxCreateCellMatrix_700" ); }
void mxCreateCellMatrix_730() { Dylib::libmx.UninitializedFunctionExport( "mxCreateCellMatrix_730" ); }
void mxCreateCharArray() { Dylib::libmx.UninitializedFunctionExport( "mxCreateCharArray" ); }
void mxCreateCharArray_700() { Dylib::libmx.UninitializedFunctionExport( "mxCreateCharArray_700" ); }
void mxCreateCharArray_730() { Dylib::libmx.UninitializedFunctionExport( "mxCreateCharArray_730" ); }
void mxCreateCharMatrixFromStrings() { Dylib::libmx.UninitializedFunctionExport( "mxCreateCharMatrixFromStrings" ); }
void mxCreateCharMatrixFromStrings_700() { Dylib::libmx.UninitializedFunctionExport( "mxCreateCharMatrixFromStrings_700" ); }
void mxCreateCharMatrixFromStrings_730() { Dylib::libmx.UninitializedFunctionExport( "mxCreateCharMatrixFromStrings_730" ); }
void mxCreateDoubleMatrix() { Dylib::libmx.UninitializedFunctionExport( "mxCreateDoubleMatrix" ); }
void mxCreateDoubleMatrix_700() { Dylib::libmx.UninitializedFunctionExport( "mxCreateDoubleMatrix_700" ); }
void mxCreateDoubleMatrix_730() { Dylib::libmx.UninitializedFunctionExport( "mxCreateDoubleMatrix_730" ); }
void mxCreateDoubleScalar() { Dylib::libmx.UninitializedFunctionExport( "mxCreateDoubleScalar" ); }
void mxCreateLogicalArray() { Dylib::libmx.UninitializedFunctionExport( "mxCreateLogicalArray" ); }
void mxCreateLogicalArray_700() { Dylib::libmx.UninitializedFunctionExport( "mxCreateLogicalArray_700" ); }
void mxCreateLogicalArray_730() { Dylib::libmx.UninitializedFunctionExport( "mxCreateLogicalArray_730" ); }
void mxCreateLogicalMatrix() { Dylib::libmx.UninitializedFunctionExport( "mxCreateLogicalMatrix" ); }
void mxCreateLogicalMatrix_700() { Dylib::libmx.UninitializedFunctionExport( "mxCreateLogicalMatrix_700" ); }
void mxCreateLogicalMatrix_730() { Dylib::libmx.UninitializedFunctionExport( "mxCreateLogicalMatrix_730" ); }
void mxCreateLogicalScalar() { Dylib::libmx.UninitializedFunctionExport( "mxCreateLogicalScalar" ); }
void mxCreateNumericArray() { Dylib::libmx.UninitializedFunctionExport( "mxCreateNumericArray" ); }
void mxCreateNumericArray_700() { Dylib::libmx.UninitializedFunctionExport( "mxCreateNumericArray_700" ); }
void mxCreateNumericArray_730() { Dylib::libmx.UninitializedFunctionExport( "mxCreateNumericArray_730" ); }
void mxCreateNumericMatrix() { Dylib::libmx.UninitializedFunctionExport( "mxCreateNumericMatrix" ); }
void mxCreateNumericMatrix_700() { Dylib::libmx.UninitializedFunctionExport( "mxCreateNumericMatrix_700" ); }
void mxCreateNumericMatrix_730() { Dylib::libmx.UninitializedFunctionExport( "mxCreateNumericMatrix_730" ); }
void mxCreateSharedDataCopy() { Dylib::libmx.UninitializedFunctionExport( "mxCreateSharedDataCopy" ); }
void mxCreateSparse() { Dylib::libmx.UninitializedFunctionExport( "mxCreateSparse" ); }
void mxCreateSparseLogicalMatrix() { Dylib::libmx.UninitializedFunctionExport( "mxCreateSparseLogicalMatrix" ); }
void mxCreateSparseLogicalMatrix_700() { Dylib::libmx.UninitializedFunctionExport( "mxCreateSparseLogicalMatrix_700" ); }
void mxCreateSparseLogicalMatrix_730() { Dylib::libmx.UninitializedFunctionExport( "mxCreateSparseLogicalMatrix_730" ); }
void mxCreateSparse_700() { Dylib::libmx.UninitializedFunctionExport( "mxCreateSparse_700" ); }
void mxCreateSparse_730() { Dylib::libmx.UninitializedFunctionExport( "mxCreateSparse_730" ); }
void mxCreateString() { Dylib::libmx.UninitializedFunctionExport( "mxCreateString" ); }
void mxCreateStringFromNChars() { Dylib::libmx.UninitializedFunctionExport( "mxCreateStringFromNChars" ); }
void mxCreateStringFromNChars_700() { Dylib::libmx.UninitializedFunctionExport( "mxCreateStringFromNChars_700" ); }
void mxCreateStringFromNChars_730() { Dylib::libmx.UninitializedFunctionExport( "mxCreateStringFromNChars_730" ); }
void mxCreateStructArray() { Dylib::libmx.UninitializedFunctionExport( "mxCreateStructArray" ); }
void mxCreateStructArray_700() { Dylib::libmx.UninitializedFunctionExport( "mxCreateStructArray_700" ); }
void mxCreateStructArray_730() { Dylib::libmx.UninitializedFunctionExport( "mxCreateStructArray_730" ); }
void mxCreateStructMatrix() { Dylib::libmx.UninitializedFunctionExport( "mxCreateStructMatrix" ); }
void mxCreateStructMatrix_700() { Dylib::libmx.UninitializedFunctionExport( "mxCreateStructMatrix_700" ); }
void mxCreateStructMatrix_730() { Dylib::libmx.UninitializedFunctionExport( "mxCreateStructMatrix_730" ); }
void mxCreateUninitDoubleMatrix() { Dylib::libmx.UninitializedFunctionExport( "mxCreateUninitDoubleMatrix" ); }
void mxCreateUninitNumericArray() { Dylib::libmx.UninitializedFunctionExport( "mxCreateUninitNumericArray" ); }
void mxCreateUninitNumericMatrix() { Dylib::libmx.UninitializedFunctionExport( "mxCreateUninitNumericMatrix" ); }
void mxDestroyArray() { Dylib::libmx.UninitializedFunctionExport( "mxDestroyArray" ); }
void mxDuplicateArray() { Dylib::libmx.UninitializedFunctionExport( "mxDuplicateArray" ); }
void mxErrMsgTxt() { Dylib::libmx.UninitializedFunctionExport( "mxErrMsgTxt" ); }
void mxFastZeros() { Dylib::libmx.UninitializedFunctionExport( "mxFastZeros" ); }
void mxFree() { Dylib::libmx.UninitializedFunctionExport( "mxFree" ); }
void mxGetCell() { Dylib::libmx.UninitializedFunctionExport( "mxGetCell" ); }
void mxGetCell_700() { Dylib::libmx.UninitializedFunctionExport( "mxGetCell_700" ); }
void mxGetCell_730() { Dylib::libmx.UninitializedFunctionExport( "mxGetCell_730" ); }
void mxGetChars() { Dylib::libmx.UninitializedFunctionExport( "mxGetChars" ); }
void mxGetClassID() { Dylib::libmx.UninitializedFunctionExport( "mxGetClassID" ); }
void mxGetClassName() { Dylib::libmx.UninitializedFunctionExport( "mxGetClassName" ); }
void mxGetData() { Dylib::libmx.UninitializedFunctionExport( "mxGetData" ); }
void mxGetDimensions() { Dylib::libmx.UninitializedFunctionExport( "mxGetDimensions" ); }
void mxGetDimensions_700() { Dylib::libmx.UninitializedFunctionExport( "mxGetDimensions_700" ); }
void mxGetDimensions_730() { Dylib::libmx.UninitializedFunctionExport( "mxGetDimensions_730" ); }
void mxGetElementSize() { Dylib::libmx.UninitializedFunctionExport( "mxGetElementSize" ); }
void mxGetEps() { Dylib::libmx.UninitializedFunctionExport( "mxGetEps" ); }
void mxGetField() { Dylib::libmx.UninitializedFunctionExport( "mxGetField" ); }
void mxGetFieldByNumber() { Dylib::libmx.UninitializedFunctionExport( "mxGetFieldByNumber" ); }
void mxGetFieldByNumber_700() { Dylib::libmx.UninitializedFunctionExport( "mxGetFieldByNumber_700" ); }
void mxGetFieldByNumber_730() { Dylib::libmx.UninitializedFunctionExport( "mxGetFieldByNumber_730" ); }
void mxGetFieldNameByNumber() { Dylib::libmx.UninitializedFunctionExport( "mxGetFieldNameByNumber" ); }
void mxGetFieldNumber() { Dylib::libmx.UninitializedFunctionExport( "mxGetFieldNumber" ); }
void mxGetField_700() { Dylib::libmx.UninitializedFunctionExport( "mxGetField_700" ); }
void mxGetField_730() { Dylib::libmx.UninitializedFunctionExport( "mxGetField_730" ); }
void mxGetImagData() { Dylib::libmx.UninitializedFunctionExport( "mxGetImagData" ); }
void mxGetInf() { Dylib::libmx.UninitializedFunctionExport( "mxGetInf" ); }
void mxGetIr() { Dylib::libmx.UninitializedFunctionExport( "mxGetIr" ); }
void mxGetIr_700() { Dylib::libmx.UninitializedFunctionExport( "mxGetIr_700" ); }
void mxGetIr_730() { Dylib::libmx.UninitializedFunctionExport( "mxGetIr_730" ); }
void mxGetJc() { Dylib::libmx.UninitializedFunctionExport( "mxGetJc" ); }
void mxGetJc_700() { Dylib::libmx.UninitializedFunctionExport( "mxGetJc_700" ); }
void mxGetJc_730() { Dylib::libmx.UninitializedFunctionExport( "mxGetJc_730" ); }
void mxGetLogicals() { Dylib::libmx.UninitializedFunctionExport( "mxGetLogicals" ); }
void mxGetM() { Dylib::libmx.UninitializedFunctionExport( "mxGetM" ); }
void mxGetN() { Dylib::libmx.UninitializedFunctionExport( "mxGetN" ); }
void mxGetNChars() { Dylib::libmx.UninitializedFunctionExport( "mxGetNChars" ); }
void mxGetNChars_700() { Dylib::libmx.UninitializedFunctionExport( "mxGetNChars_700" ); }
void mxGetNChars_730() { Dylib::libmx.UninitializedFunctionExport( "mxGetNChars_730" ); }
void mxGetNaN() { Dylib::libmx.UninitializedFunctionExport( "mxGetNaN" ); }
void mxGetNumberOfDimensions() { Dylib::libmx.UninitializedFunctionExport( "mxGetNumberOfDimensions" ); }
void mxGetNumberOfDimensions_700() { Dylib::libmx.UninitializedFunctionExport( "mxGetNumberOfDimensions_700" ); }
void mxGetNumberOfDimensions_730() { Dylib::libmx.UninitializedFunctionExport( "mxGetNumberOfDimensions_730" ); }
void mxGetNumberOfElements() { Dylib::libmx.UninitializedFunctionExport( "mxGetNumberOfElements" ); }
void mxGetNumberOfFields() { Dylib::libmx.UninitializedFunctionExport( "mxGetNumberOfFields" ); }
void mxGetNzmax() { Dylib::libmx.UninitializedFunctionExport( "mxGetNzmax" ); }
void mxGetNzmax_700() { Dylib::libmx.UninitializedFunctionExport( "mxGetNzmax_700" ); }
void mxGetNzmax_730() { Dylib::libmx.UninitializedFunctionExport( "mxGetNzmax_730" ); }
void mxGetPi() { Dylib::libmx.UninitializedFunctionExport( "mxGetPi" ); }
void mxGetPr() { Dylib::libmx.UninitializedFunctionExport( "mxGetPr" ); }
void mxGetProperty() { Dylib::libmx.UninitializedFunctionExport( "mxGetProperty" ); }
void mxGetProperty_700() { Dylib::libmx.UninitializedFunctionExport( "mxGetProperty_700" ); }
void mxGetProperty_730() { Dylib::libmx.UninitializedFunctionExport( "mxGetProperty_730" ); }
void mxGetScalar() { Dylib::libmx.UninitializedFunctionExport( "mxGetScalar" ); }
void mxGetString() { Dylib::libmx.UninitializedFunctionExport( "mxGetString" ); }
void mxGetString_700() { Dylib::libmx.UninitializedFunctionExport( "mxGetString_700" ); }
void mxGetString_730() { Dylib::libmx.UninitializedFunctionExport( "mxGetString_730" ); }
void mxGetUserBits() { Dylib::libmx.UninitializedFunctionExport( "mxGetUserBits" ); }
void mxIsCell() { Dylib::libmx.UninitializedFunctionExport( "mxIsCell" ); }
void mxIsChar() { Dylib::libmx.UninitializedFunctionExport( "mxIsChar" ); }
void mxIsClass() { Dylib::libmx.UninitializedFunctionExport( "mxIsClass" ); }
void mxIsComplex() { Dylib::libmx.UninitializedFunctionExport( "mxIsComplex" ); }
void mxIsDouble() { Dylib::libmx.UninitializedFunctionExport( "mxIsDouble" ); }
void mxIsEmpty() { Dylib::libmx.UninitializedFunctionExport( "mxIsEmpty" ); }
void mxIsFinite() { Dylib::libmx.UninitializedFunctionExport( "mxIsFinite" ); }
void mxIsFromGlobalWS() { Dylib::libmx.UninitializedFunctionExport( "mxIsFromGlobalWS" ); }
void mxIsFunctionHandle() { Dylib::libmx.UninitializedFunctionExport( "mxIsFunctionHandle" ); }
void mxIsInf() { Dylib::libmx.UninitializedFunctionExport( "mxIsInf" ); }
void mxIsInt16() { Dylib::libmx.UninitializedFunctionExport( "mxIsInt16" ); }
void mxIsInt32() { Dylib::libmx.UninitializedFunctionExport( "mxIsInt32" ); }
void mxIsInt64() { Dylib::libmx.UninitializedFunctionExport( "mxIsInt64" ); }
void mxIsInt8() { Dylib::libmx.UninitializedFunctionExport( "mxIsInt8" ); }
void mxIsIntVectorWrapper() { Dylib::libmx.UninitializedFunctionExport( "mxIsIntVectorWrapper" ); }
void mxIsLogical() { Dylib::libmx.UninitializedFunctionExport( "mxIsLogical" ); }
void mxIsLogicalScalar() { Dylib::libmx.UninitializedFunctionExport( "mxIsLogicalScalar" ); }
void mxIsLogicalScalarTrue() { Dylib::libmx.UninitializedFunctionExport( "mxIsLogicalScalarTrue" ); }
void mxIsNaN() { Dylib::libmx.UninitializedFunctionExport( "mxIsNaN" ); }
void mxIsNumeric() { Dylib::libmx.UninitializedFunctionExport( "mxIsNumeric" ); }
void mxIsObject() { Dylib::libmx.UninitializedFunctionExport( "mxIsObject" ); }
void mxIsOpaque() { Dylib::libmx.UninitializedFunctionExport( "mxIsOpaque" ); }
void mxIsScalar() { Dylib::libmx.UninitializedFunctionExport( "mxIsScalar" ); }
void mxIsSingle() { Dylib::libmx.UninitializedFunctionExport( "mxIsSingle" ); }
void mxIsSparse() { Dylib::libmx.UninitializedFunctionExport( "mxIsSparse" ); }
void mxIsStruct() { Dylib::libmx.UninitializedFunctionExport( "mxIsStruct" ); }
void mxIsUint16() { Dylib::libmx.UninitializedFunctionExport( "mxIsUint16" ); }
void mxIsUint32() { Dylib::libmx.UninitializedFunctionExport( "mxIsUint32" ); }
void mxIsUint64() { Dylib::libmx.UninitializedFunctionExport( "mxIsUint64" ); }
void mxIsUint8() { Dylib::libmx.UninitializedFunctionExport( "mxIsUint8" ); }
void mxMalloc() { Dylib::libmx.UninitializedFunctionExport( "mxMalloc" ); }
void mxRealloc() { Dylib::libmx.UninitializedFunctionExport( "mxRealloc" ); }
void mxRemoveField() { Dylib::libmx.UninitializedFunctionExport( "mxRemoveField" ); }
void mxSetCell() { Dylib::libmx.UninitializedFunctionExport( "mxSetCell" ); }
void mxSetCell_700() { Dylib::libmx.UninitializedFunctionExport( "mxSetCell_700" ); }
void mxSetCell_730() { Dylib::libmx.UninitializedFunctionExport( "mxSetCell_730" ); }
void mxSetClassName() { Dylib::libmx.UninitializedFunctionExport( "mxSetClassName" ); }
void mxSetData() { Dylib::libmx.UninitializedFunctionExport( "mxSetData" ); }
void mxSetDimensions() { Dylib::libmx.UninitializedFunctionExport( "mxSetDimensions" ); }
void mxSetDimensions_700() { Dylib::libmx.UninitializedFunctionExport( "mxSetDimensions_700" ); }
void mxSetDimensions_730() { Dylib::libmx.UninitializedFunctionExport( "mxSetDimensions_730" ); }
void mxSetField() { Dylib::libmx.UninitializedFunctionExport( "mxSetField" ); }
void mxSetFieldByNumber() { Dylib::libmx.UninitializedFunctionExport( "mxSetFieldByNumber" ); }
void mxSetFieldByNumber_700() { Dylib::libmx.UninitializedFunctionExport( "mxSetFieldByNumber_700" ); }
void mxSetFieldByNumber_730() { Dylib::libmx.UninitializedFunctionExport( "mxSetFieldByNumber_730" ); }
void mxSetField_700() { Dylib::libmx.UninitializedFunctionExport( "mxSetField_700" ); }
void mxSetField_730() { Dylib::libmx.UninitializedFunctionExport( "mxSetField_730" ); }
void mxSetFromGlobalWS() { Dylib::libmx.UninitializedFunctionExport( "mxSetFromGlobalWS" ); }
void mxSetImagData() { Dylib::libmx.UninitializedFunctionExport( "mxSetImagData" ); }
void mxSetIr() { Dylib::libmx.UninitializedFunctionExport( "mxSetIr" ); }
void mxSetIr_700() { Dylib::libmx.UninitializedFunctionExport( "mxSetIr_700" ); }
void mxSetIr_730() { Dylib::libmx.UninitializedFunctionExport( "mxSetIr_730" ); }
void mxSetJc() { Dylib::libmx.UninitializedFunctionExport( "mxSetJc" ); }
void mxSetJc_700() { Dylib::libmx.UninitializedFunctionExport( "mxSetJc_700" ); }
void mxSetJc_730() { Dylib::libmx.UninitializedFunctionExport( "mxSetJc_730" ); }
void mxSetM() { Dylib::libmx.UninitializedFunctionExport( "mxSetM" ); }
void mxSetM_700() { Dylib::libmx.UninitializedFunctionExport( "mxSetM_700" ); }
void mxSetM_730() { Dylib::libmx.UninitializedFunctionExport( "mxSetM_730" ); }
void mxSetN() { Dylib::libmx.UninitializedFunctionExport( "mxSetN" ); }
void mxSetN_700() { Dylib::libmx.UninitializedFunctionExport( "mxSetN_700" ); }
void mxSetN_730() { Dylib::libmx.UninitializedFunctionExport( "mxSetN_730" ); }
void mxSetNzmax() { Dylib::libmx.UninitializedFunctionExport( "mxSetNzmax" ); }
void mxSetNzmax_700() { Dylib::libmx.UninitializedFunctionExport( "mxSetNzmax_700" ); }
void mxSetNzmax_730() { Dylib::libmx.UninitializedFunctionExport( "mxSetNzmax_730" ); }
void mxSetPi() { Dylib::libmx.UninitializedFunctionExport( "mxSetPi" ); }
void mxSetPr() { Dylib::libmx.UninitializedFunctionExport( "mxSetPr" ); }
void mxSetProperty() { Dylib::libmx.UninitializedFunctionExport( "mxSetProperty" ); }
void mxSetProperty_700() { Dylib::libmx.UninitializedFunctionExport( "mxSetProperty_700" ); }
void mxSetProperty_730() { Dylib::libmx.UninitializedFunctionExport( "mxSetProperty_730" ); }
void mxSetUserBits() { Dylib::libmx.UninitializedFunctionExport( "mxSetUserBits" ); }
void mxUnreference() { Dylib::libmx.UninitializedFunctionExport( "mxUnreference" ); }
void mxUnshareArray() { Dylib::libmx.UninitializedFunctionExport( "mxUnshareArray" ); }
void mxVErrMsgIdAndTxt() { Dylib::libmx.UninitializedFunctionExport( "mxVErrMsgIdAndTxt" ); }
}

namespace {
enum { flags = Dylib::Import::functionStub | Dylib::Import::optional, };
const Dylib::Import functionImports_[] =
{
  { "mxAddField", (void**)&mxAddField, flags },
  { "mxArrayToString", (void**)&mxArrayToString, flags },
  { "mxArrayToUTF8String", (void**)&mxArrayToUTF8String, flags },
  { "mxCalcSingleSubscript", (void**)&mxCalcSingleSubscript, flags },
  { "mxCalcSingleSubscript_700", (void**)&mxCalcSingleSubscript_700, flags },
  { "mxCalcSingleSubscript_730", (void**)&mxCalcSingleSubscript_730, flags },
  { "mxCalloc", (void**)&mxCalloc, flags },
  { "mxCreateCellArray", (void**)&mxCreateCellArray, flags },
  { "mxCreateCellArray_700", (void**)&mxCreateCellArray_700, flags },
  { "mxCreateCellArray_730", (void**)&mxCreateCellArray_730, flags },
  { "mxCreateCellMatrix", (void**)&mxCreateCellMatrix, flags },
  { "mxCreateCellMatrix_700", (void**)&mxCreateCellMatrix_700, flags },
  { "mxCreateCellMatrix_730", (void**)&mxCreateCellMatrix_730, flags },
  { "mxCreateCharArray", (void**)&mxCreateCharArray, flags },
  { "mxCreateCharArray_700", (void**)&mxCreateCharArray_700, flags },
  { "mxCreateCharArray_730", (void**)&mxCreateCharArray_730, flags },
  { "mxCreateCharMatrixFromStrings", (void**)&mxCreateCharMatrixFromStrings, flags },
  { "mxCreateCharMatrixFromStrings_700", (void**)&mxCreateCharMatrixFromStrings_700, flags },
  { "mxCreateCharMatrixFromStrings_730", (void**)&mxCreateCharMatrixFromStrings_730, flags },
  { "mxCreateDoubleMatrix", (void**)&mxCreateDoubleMatrix, flags },
  { "mxCreateDoubleMatrix_700", (void**)&mxCreateDoubleMatrix_700, flags },
  { "mxCreateDoubleMatrix_730", (void**)&mxCreateDoubleMatrix_730, flags },
  { "mxCreateDoubleScalar", (void**)&mxCreateDoubleScalar, flags },
  { "mxCreateLogicalArray", (void**)&mxCreateLogicalArray, flags },
  { "mxCreateLogicalArray_700", (void**)&mxCreateLogicalArray_700, flags },
  { "mxCreateLogicalArray_730", (void**)&mxCreateLogicalArray_730, flags },
  { "mxCreateLogicalMatrix", (void**)&mxCreateLogicalMatrix, flags },
  { "mxCreateLogicalMatrix_700", (void**)&mxCreateLogicalMatrix_700, flags },
  { "mxCreateLogicalMatrix_730", (void**)&mxCreateLogicalMatrix_730, flags },
  { "mxCreateLogicalScalar", (void**)&mxCreateLogicalScalar, flags },
  { "mxCreateNumericArray", (void**)&mxCreateNumericArray, flags },
  { "mxCreateNumericArray_700", (void**)&mxCreateNumericArray_700, flags },
  { "mxCreateNumericArray_730", (void**)&mxCreateNumericArray_730, flags },
  { "mxCreateNumericMatrix", (void**)&mxCreateNumericMatrix, flags },
  { "mxCreateNumericMatrix_700", (void**)&mxCreateNumericMatrix_700, flags },
  { "mxCreateNumericMatrix_730", (void**)&mxCreateNumericMatrix_730, flags },
  { "mxCreateSharedDataCopy", (void**)&mxCreateSharedDataCopy, flags },
  { "mxCreateSparse", (void**)&mxCreateSparse, flags },
  { "mxCreateSparseLogicalMatrix", (void**)&mxCreateSparseLogicalMatrix, flags },
  { "mxCreateSparseLogicalMatrix_700", (void**)&mxCreateSparseLogicalMatrix_700, flags },
  { "mxCreateSparseLogicalMatrix_730", (void**)&mxCreateSparseLogicalMatrix_730, flags },
  { "mxCreateSparse_700", (void**)&mxCreateSparse_700, flags },
  { "mxCreateSparse_730", (void**)&mxCreateSparse_730, flags },
  { "mxCreateString", (void**)&mxCreateString, flags },
  { "mxCreateStringFromNChars", (void**)&mxCreateStringFromNChars, flags },
  { "mxCreateStringFromNChars_700", (void**)&mxCreateStringFromNChars_700, flags },
  { "mxCreateStringFromNChars_730", (void**)&mxCreateStringFromNChars_730, flags },
  { "mxCreateStructArray", (void**)&mxCreateStructArray, flags },
  { "mxCreateStructArray_700", (void**)&mxCreateStructArray_700, flags },
  { "mxCreateStructArray_730", (void**)&mxCreateStructArray_730, flags },
  { "mxCreateStructMatrix", (void**)&mxCreateStructMatrix, flags },
  { "mxCreateStructMatrix_700", (void**)&mxCreateStructMatrix_700, flags },
  { "mxCreateStructMatrix_730", (void**)&mxCreateStructMatrix_730, flags },
  { "mxCreateUninitDoubleMatrix", (void**)&mxCreateUninitDoubleMatrix, flags },
  { "mxCreateUninitNumericArray", (void**)&mxCreateUninitNumericArray, flags },
  { "mxCreateUninitNumericMatrix", (void**)&mxCreateUninitNumericMatrix, flags },
  { "mxDestroyArray", (void**)&mxDestroyArray, flags },
  { "mxDuplicateArray", (void**)&mxDuplicateArray, flags },
  { "mxErrMsgTxt", (void**)&mxErrMsgTxt, flags },
  { "mxFastZeros", (void**)&mxFastZeros, flags },
  { "mxFree", (void**)&mxFree, flags },
  { "mxGetCell", (void**)&mxGetCell, flags },
  { "mxGetCell_700", (void**)&mxGetCell_700, flags },
  { "mxGetCell_730", (void**)&mxGetCell_730, flags },
  { "mxGetChars", (void**)&mxGetChars, flags },
  { "mxGetClassID", (void**)&mxGetClassID, flags },
  { "mxGetClassName", (void**)&mxGetClassName, flags },
  { "mxGetData", (void**)&mxGetData, flags },
  { "mxGetDimensions", (void**)&mxGetDimensions, flags },
  { "mxGetDimensions_700", (void**)&mxGetDimensions_700, flags },
  { "mxGetDimensions_730", (void**)&mxGetDimensions_730, flags },
  { "mxGetElementSize", (void**)&mxGetElementSize, flags },
  { "mxGetEps", (void**)&mxGetEps, flags },
  { "mxGetField", (void**)&mxGetField, flags },
  { "mxGetFieldByNumber", (void**)&mxGetFieldByNumber, flags },
  { "mxGetFieldByNumber_700", (void**)&mxGetFieldByNumber_700, flags },
  { "mxGetFieldByNumber_730", (void**)&mxGetFieldByNumber_730, flags },
  { "mxGetFieldNameByNumber", (void**)&mxGetFieldNameByNumber, flags },
  { "mxGetFieldNumber", (void**)&mxGetFieldNumber, flags },
  { "mxGetField_700", (void**)&mxGetField_700, flags },
  { "mxGetField_730", (void**)&mxGetField_730, flags },
  { "mxGetImagData", (void**)&mxGetImagData, flags },
  { "mxGetInf", (void**)&mxGetInf, flags },
  { "mxGetIr", (void**)&mxGetIr, flags },
  { "mxGetIr_700", (void**)&mxGetIr_700, flags },
  { "mxGetIr_730", (void**)&mxGetIr_730, flags },
  { "mxGetJc", (void**)&mxGetJc, flags },
  { "mxGetJc_700", (void**)&mxGetJc_700, flags },
  { "mxGetJc_730", (void**)&mxGetJc_730, flags },
  { "mxGetLogicals", (void**)&mxGetLogicals, flags },
  { "mxGetM", (void**)&mxGetM, flags },
  { "mxGetN", (void**)&mxGetN, flags },
  { "mxGetNChars", (void**)&mxGetNChars, flags },
  { "mxGetNChars_700", (void**)&mxGetNChars_700, flags },
  { "mxGetNChars_730", (void**)&mxGetNChars_730, flags },
  { "mxGetNaN", (void**)&mxGetNaN, flags },
  { "mxGetNumberOfDimensions", (void**)&mxGetNumberOfDimensions, flags },
  { "mxGetNumberOfDimensions_700", (void**)&mxGetNumberOfDimensions_700, flags },
  { "mxGetNumberOfDimensions_730", (void**)&mxGetNumberOfDimensions_730, flags },
  { "mxGetNumberOfElements", (void**)&mxGetNumberOfElements, flags },
  { "mxGetNumberOfFields", (void**)&mxGetNumberOfFields, flags },
  { "mxGetNzmax", (void**)&mxGetNzmax, flags },
  { "mxGetNzmax_700", (void**)&mxGetNzmax_700, flags },
  { "mxGetNzmax_730", (void**)&mxGetNzmax_730, flags },
  { "mxGetPi", (void**)&mxGetPi, flags },
  { "mxGetPr", (void**)&mxGetPr, flags },
  { "mxGetProperty", (void**)&mxGetProperty, flags },
  { "mxGetProperty_700", (void**)&mxGetProperty_700, flags },
  { "mxGetProperty_730", (void**)&mxGetProperty_730, flags },
  { "mxGetScalar", (void**)&mxGetScalar, flags },
  { "mxGetString", (void**)&mxGetString, flags },
  { "mxGetString_700", (void**)&mxGetString_700, flags },
  { "mxGetString_730", (void**)&mxGetString_730, flags },
  { "mxGetUserBits", (void**)&mxGetUserBits, flags },
  { "mxIsCell", (void**)&mxIsCell, flags },
  { "mxIsChar", (void**)&mxIsChar, flags },
  { "mxIsClass", (void**)&mxIsClass, flags },
  { "mxIsComplex", (void**)&mxIsComplex, flags },
  { "mxIsDouble", (void**)&mxIsDouble, flags },
  { "mxIsEmpty", (void**)&mxIsEmpty, flags },
  { "mxIsFinite", (void**)&mxIsFinite, flags },
  { "mxIsFromGlobalWS", (void**)&mxIsFromGlobalWS, flags },
  { "mxIsFunctionHandle", (void**)&mxIsFunctionHandle, flags },
  { "mxIsInf", (void**)&mxIsInf, flags },
  { "mxIsInt16", (void**)&mxIsInt16, flags },
  { "mxIsInt32", (void**)&mxIsInt32, flags },
  { "mxIsInt64", (void**)&mxIsInt64, flags },
  { "mxIsInt8", (void**)&mxIsInt8, flags },
  { "mxIsIntVectorWrapper", (void**)&mxIsIntVectorWrapper, flags },
  { "mxIsLogical", (void**)&mxIsLogical, flags },
  { "mxIsLogicalScalar", (void**)&mxIsLogicalScalar, flags },
  { "mxIsLogicalScalarTrue", (void**)&mxIsLogicalScalarTrue, flags },
  { "mxIsNaN", (void**)&mxIsNaN, flags },
  { "mxIsNumeric", (void**)&mxIsNumeric, flags },
  { "mxIsObject", (void**)&mxIsObject, flags },
  { "mxIsOpaque", (void**)&mxIsOpaque, flags },
  { "mxIsScalar", (void**)&mxIsScalar, flags },
  { "mxIsSingle", (void**)&mxIsSingle, flags },
  { "mxIsSparse", (void**)&mxIsSparse, flags },
  { "mxIsStruct", (void**)&mxIsStruct, flags },
  { "mxIsUint16", (void**)&mxIsUint16, flags },
  { "mxIsUint32", (void**)&mxIsUint32, flags },
  { "mxIsUint64", (void**)&mxIsUint64, flags },
  { "mxIsUint8", (void**)&mxIsUint8, flags },
  { "mxMalloc", (void**)&mxMalloc, flags },
  { "mxRealloc", (void**)&mxRealloc, flags },
  { "mxRemoveField", (void**)&mxRemoveField, flags },
  { "mxSetCell", (void**)&mxSetCell, flags },
  { "mxSetCell_700", (void**)&mxSetCell_700, flags },
  { "mxSetCell_730", (void**)&mxSetCell_730, flags },
  { "mxSetClassName", (void**)&mxSetClassName, flags },
  { "mxSetData", (void**)&mxSetData, flags },
  { "mxSetDimensions", (void**)&mxSetDimensions, flags },
  { "mxSetDimensions_700", (void**)&mxSetDimensions_700, flags },
  { "mxSetDimensions_730", (void**)&mxSetDimensions_730, flags },
  { "mxSetField", (void**)&mxSetField, flags },
  { "mxSetFieldByNumber", (void**)&mxSetFieldByNumber, flags },
  { "mxSetFieldByNumber_700", (void**)&mxSetFieldByNumber_700, flags },
  { "mxSetFieldByNumber_730", (void**)&mxSetFieldByNumber_730, flags },
  { "mxSetField_700", (void**)&mxSetField_700, flags },
  { "mxSetField_730", (void**)&mxSetField_730, flags },
  { "mxSetFromGlobalWS", (void**)&mxSetFromGlobalWS, flags },
  { "mxSetImagData", (void**)&mxSetImagData, flags },
  { "mxSetIr", (void**)&mxSetIr, flags },
  { "mxSetIr_700", (void**)&mxSetIr_700, flags },
  { "mxSetIr_730", (void**)&mxSetIr_730, flags },
  { "mxSetJc", (void**)&mxSetJc, flags },
  { "mxSetJc_700", (void**)&mxSetJc_700, flags },
  { "mxSetJc_730", (void**)&mxSetJc_730, flags },
  { "mxSetM", (void**)&mxSetM, flags },
  { "mxSetM_700", (void**)&mxSetM_700, flags },
  { "mxSetM_730", (void**)&mxSetM_730, flags },
  { "mxSetN", (void**)&mxSetN, flags },
  { "mxSetN_700", (void**)&mxSetN_700, flags },
  { "mxSetN_730", (void**)&mxSetN_730, flags },
  { "mxSetNzmax", (void**)&mxSetNzmax, flags },
  { "mxSetNzmax_700", (void**)&mxSetNzmax_700, flags },
  { "mxSetNzmax_730", (void**)&mxSetNzmax_730, flags },
  { "mxSetPi", (void**)&mxSetPi, flags },
  { "mxSetPr", (void**)&mxSetPr, flags },
  { "mxSetProperty", (void**)&mxSetProperty, flags },
  { "mxSetProperty_700", (void**)&mxSetProperty_700, flags },
  { "mxSetProperty_730", (void**)&mxSetProperty_730, flags },
  { "mxSetUserBits", (void**)&mxSetUserBits, flags },
  { "mxUnreference", (void**)&mxUnreference, flags },
  { "mxUnshareArray", (void**)&mxUnshareArray, flags },
  { "mxVErrMsgIdAndTxt", (void**)&mxVErrMsgIdAndTxt, flags },
  { 0, 0, 0 }
};
const Dylib::Import* functionImports = functionImports_;
}


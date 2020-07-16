//   $Id: PyffFilter.cpp 5529 2016-09-14 18:47:45Z mellinger $
//
//   This file is a modified copy of part of the BCPy2000 foundation,
//   a set of modules for
//   the BCI2000 <http://bci2000.org/> that allow communication with a
//   Python framework built on top.
//
//   Copyright (C) 2012 Juergen Mellinger
//   Copyright (C) 2007-11  Jeremy Hill, Thomas Schreiner,
//                         Christian Puzicha, Jason Farquhar
//
//   bcpy2000@bci2000.org
//
//   The BCPy2000 foundation is free software: you can redistribute it
//   and/or modify it under the terms of the GNU Lesser General Public
//   License as published by the Free Software Foundation, either
//   version 3 of the License, or (at your option) any later version.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU Lesser General Public License for more details.
//
//   You should have received a copy of the GNU Lesser General Public License
//   along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
#include "PyffFilter.h"
#include "BCIStream.h"
#include "FileUtils.h"
#include "ProcessUtils.h"
#include "ThreadUtils.h"

#include <cstdio>
#include <ctime>

#if _WIN32
# include <Windows.h>
# if _MSC_VER
#  include <direct.h>
# else
#  include <dir.h>
# endif
#else
# include <unistd.h>
#endif

using namespace std;

RegisterFilter( FILTER_NAME, 3 );

EndUserError::EndUserError(const char* s) : bci::Exception(s) {}

////////////////////////////////////////////////////////////////////////////////
// Implementations of the usual GenericFilter virtual methods
////////////////////////////////////////////////////////////////////////////////

FILTER_NAME::FILTER_NAME()
:
  mpInstance( NULL ),
  mStayOpen( false ),
  mUseConsole( false ),
  mpThreadState( NULL )
{
  try {
    BEGIN_STATE_DEFINITIONS
      "AppStartTime 16 0 0 0",
      "StimulusTime 16 17528 0 0",
    END_STATE_DEFINITIONS

    string originalDir = FileUtils::WorkingDirectory();

    string dllname       = OptionalParameter(FILTER_PREFIX "DLL",       "python27");
    string logFile       = OptionalParameter(FILTER_PREFIX "Log",       "");
    string frameworkDir  = OptionalParameter(FILTER_PREFIX "Dir",       FileUtils::InstallationDirectory() + FILTER_PREFIX);
    string workingDir    = OptionalParameter(FILTER_PREFIX "WD",        ".");
    mUseConsole          = ( OptionalParameter(FILTER_PREFIX "Console", 1) != 0 );
    mStayOpen            = ( OptionalParameter(FILTER_PREFIX "StayOpen", 0) != 0 );

    workingDir = FileUtils::AbsolutePath( workingDir );

    string date_escape = "###";
    size_t found = logFile.find(date_escape, 0);
    if(found != string::npos) {
      char * datestr = new char[32];
      time_t tt;
      time(&tt);
      struct tm *t = localtime(&tt);
      sprintf(datestr, "%04d%02d%02d%02d%02d%02d", t->tm_year+1900, t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
      logFile.replace(found, date_escape.length(), datestr);
      delete [] datestr;
    }
    if(mUseConsole) OpenConsole(FILTER_PREFIX " Console");

    if(frameworkDir.length()) {
      ChangeDir(frameworkDir);  // for some strange reason this needs to be done before Py_Initialize, otherwise the frameworkDir is not found correctly...
    }

#if DYNAMIC_PYTHON
    if(LoadPythonLinks(dllname.c_str())!=0) return;
#endif
    Py_Initialize();
    PyEval_InitThreads();
    ChangeDir(originalDir);             // ...so undo it...
    if(logFile.length() && logFile != "-") {
      string cmd;
      cmd = EscapePythonString(logFile);
      cmd = "a=open('" + cmd + "', 'w', 0); import sys; sys.stderr = a; sys.stdout = a";
      EvalPythonString(cmd);
    }

    if(frameworkDir.length()) {
      ChangeDir(frameworkDir); // ...and then redo it. <sigh>
      EvalPythonString("import sys,os");
      EvalPythonString("if not os.getcwd() in sys.path: sys.path.append(os.getcwd())");
    }

    string module = PYTHON_MODULE;
    EvalPythonString("from " + module + " import *");
    ChangeDir(workingDir);
    mpInstance = CallModuleMember("__main__", PYTHON_CLASS);

    PyObject*key,*val;

    key = PyString_FromString("installation_dir");
    val = PyString_FromString(FileUtils::InstallationDirectory().c_str());
    CallMethod("__setattr__", key, val);
    Py_DecRef(key); // Py_DecRef(val);

    key = PyString_FromString("original_working_dir");
    val = PyString_FromString(originalDir.c_str());
    CallMethod("__setattr__", key, val);
    Py_DecRef(key); // Py_DecRef(val);

    PyObject *py_ret = CallHook("_Construct");

    PyObject *py_params = PyTuple_GetItem(py_ret, 0);
    PyObject *py_states = PyTuple_GetItem(py_ret, 1);

    // Add the parameters and states requested by the Python _Construct function.
    size_t numParamDefs = PyList_Size(py_params);
    for( size_t i = 0; i < numParamDefs; ++i ) {
      string paramDef = PyString_AsString(PyList_GetItem(py_params, i));
      // cerr << "registering parameter: " << paramDef << endl;
      if(!Parameters->Add( paramDef ) ) DoubleErr(("Error in parameter definition: " + paramDef).c_str(), "Construct");
    }

    size_t numStateDefs = PyList_Size(py_states);
    for( size_t i = 0; i < numStateDefs; ++i ) {
      string stateDef = PyString_AsString(PyList_GetItem(py_states, i));
      // cerr << "registering state: " << stateDef << endl;
      if(!States->Add( stateDef )) DoubleErr(("Error in state definition: " + stateDef).c_str(), "Construct");
    }

    Py_DecRef( py_ret );
    UnblockThreads();
    // cerr << "done creating engine." << endl;
  }
  catch(EndUserError& e) {
    HandleEndUserError(e, "Construct");
  }
  catch(std::exception& e) {
    HandleException(e, "Construct");
  }
}

FILTER_NAME::~FILTER_NAME()
{
  if(mStayOpen) {
    if(mUseConsole) {
#ifdef _WIN32
      while(GetConsoleWindow()) ::Sleep(1);
#else // _WIN32
      while(1) ThreadUtils::SleepFor(1);
#endif // _WIN32
    }
  }
  else {
    if( mpInstance )
    {
      Halt();
      try {
        BlockThreads();
        CallHook("_Destruct");
        UnblockThreads();
      }
      catch(EndUserError& e) {
        HandleEndUserError(e, "Destruct");
      }
      catch(std::exception& e) {
        HandleException(e, "Destruct");
      }
    }
#ifdef _WIN32
    if(mUseConsole) FreeConsole();
#endif // _WIN32
  }
}

void
FILTER_NAME::Preflight( const SignalProperties& inSignalProperties,
                               SignalProperties& outSignalProperties ) const
{
  try {
    BlockThreads();

    SendParametersToPython();
    SendStatesToPython(); // NB: allows access to all states
    SendStatePrecisionsToPython();

    PyObject* pyInSignalProperties = ConvertPropertiesToPyObject(inSignalProperties);
    PyObject* pyOutSignalProperties = CallHook( "_Preflight", pyInSignalProperties);
    Py_DecRef(pyInSignalProperties);
    ConvertPyObjectToProperties(pyOutSignalProperties, outSignalProperties);
    Py_DecRef(pyOutSignalProperties);

    PyObject* py_list;
    py_list = PyObject_GetAttrString(mpInstance, (char*)"_writeable_params");
    if( py_list ) {
      if(PyList_Check(py_list)) {
        size_t n = PyList_Size(py_list);
        for(size_t i = 0; i < n; i++) {
          const char *name = PyString_AsString(PyList_GetItem(py_list, i));
          Parameter(name); // allows write access to this parameter
        }
      }
      Py_DecRef(py_list);
    }

    PreflightCondition(SignalType::ConversionIsSafe(inSignalProperties.Type(), outSignalProperties.Type()));

    UnblockThreads();
  }
  catch(EndUserError& e) {
    HandleEndUserError(e, "Preflight");
  }
  catch(std::exception& e) {
    HandleException(e, "Preflight");
  }
}

void
FILTER_NAME::Initialize( const SignalProperties& inSignalProperties,
                          const SignalProperties& outSignalProperties )
{
  try {
    BlockThreads();
    SendStatesToPython();

    PyObject* pyInSignalProperties = ConvertPropertiesToPyObject(inSignalProperties);
    PyObject* pyOutSignalProperties = ConvertPropertiesToPyObject(outSignalProperties);

    StateMap before = ReceiveStatesFromPython();
    CallHook( "_Initialize", pyInSignalProperties, pyOutSignalProperties);
    StateMap after = ReceiveStatesFromPython();
    UpdateStateChangesFromPython(before, after);
    UnblockThreads();
  }
  catch(EndUserError& e) {
    HandleEndUserError(e, "Initialize");
  }
  catch(std::exception& e) {
    HandleException(e, "Initialize");
  }
}

void
FILTER_NAME::Process( const GenericSignal& input, GenericSignal& output )
{
  State( "AppStartTime" ) = PrecisionTime::Now();
  try {
      BlockThreads();
      SendStatesToPython();
      PyObject* py_input = ConvertSignalToPyObject(input);
      StateMap before;
      // unlike StartRun and Initialize, here we do not call before = ReceiveStatesFromPython();
      // Python may have other threads that update states asynchronously. They are passed on here.
      PyObject* py_output = CallHook("_Process", py_input);
      Py_DecRef(py_input);
      StateMap after = ReceiveStatesFromPython();
      UpdateStateChangesFromPython(before, after);
      ConvertPyObjectToSignal(py_output, output);
      Py_DecRef(py_output);
      UnblockThreads();
  }
  catch(EndUserError& e) {
    State("Running") = 0;
    HandleEndUserError(e, "Process");
  }
  catch(std::exception& e) {
    State("Running") = 0;
    HandleException(e, "Process");
  }
  State( "StimulusTime" ) = PrecisionTime::Now();
}

void
FILTER_NAME::StartRun()
{
  try {
    BlockThreads();
    SendParametersToPython();
    SendStatesToPython();
    StateMap before = ReceiveStatesFromPython();
    CallHook("_StartRun");
    StateMap after = ReceiveStatesFromPython();
    UpdateStateChangesFromPython(before, after);
    UnblockThreads();
  }
  catch(EndUserError& e) {
    HandleEndUserError(e, "StartRun");
  }
  catch(std::exception& e) {
    HandleException(e, "StartRun");
  }
}

void
FILTER_NAME::StopRun()
{
  try {
    BlockThreads();
    CallHook("_StopRun");
    ReceiveParametersFromPython();
    SendParametersToPython();
    UnblockThreads();
  }
  catch(EndUserError& e) {
    HandleEndUserError(e, "StopRun");
  }
  catch(std::exception& e) {
    HandleException(e, "StopRun");
  }
}

void
FILTER_NAME::Resting()
{
  try {
    BlockThreads();
    PyObject* ret = CallHook("_Resting");
    UnblockThreads();
  }
  catch(EndUserError& e) {
    HandleEndUserError(e, "Resting");
  }
  catch(std::exception& e) {
    HandleException(e, "Resting");
  }
}

void
FILTER_NAME::Halt()
{
  try {
    BlockThreads();
    CallHook("_Halt");
    UnblockThreads();
  }
  catch(EndUserError& e) {
    HandleEndUserError(e, "Halt");
  }
  catch(std::exception& e) {
    HandleException(e, "Halt");
  }
}

////////////////////////////////////////////////////////////////////////////////
// Exchangers of parameter and state info between C and Python
////////////////////////////////////////////////////////////////////////////////

void
FILTER_NAME::SendParametersToPython() const
{
  PyObject* params = PyDict_New();
  PyObject* value;

  for( int i = 0; i < Parameters->Size(); ++i ) {
    const Param& p = ( *Parameters )[ i ];
    if(p.NumRows() == 0 || p.NumColumns() == 0) {
      value = PyString_FromString("");
      PyDict_SetItemString(params, p.Name().c_str(), value);
      Py_DecRef(value);
      continue;
    }
    if(p.Type().size() >= 6 && strcmp(p.Type().c_str() + p.Type().size() - 6, "matrix") == 0) {
      int nrows = p.NumRows();
      int ncols = p.NumColumns();

      PyObject* list = PyList_New(nrows);

      for( int i = 0; i < nrows; i++) {
        PyObject* tempRow = PyList_New(ncols);
        for( int j = 0; j < ncols; j++) {
          PyObject* val = PyString_FromString(((string)p.Value(i,j)).c_str());
          PyList_SetItem(tempRow, j, val);
        }
        PyList_SetItem(list, i, tempRow);
      }
      PyDict_SetItemString(params, p.Name().c_str(), list);
      Py_DecRef( list );
    }
    else if(p.Type().size() >= 4 && strcmp(p.Type().c_str() + p.Type().size() - 4, "list") == 0) {
      int entries = max(p.NumRows(),p.NumColumns());

      PyObject* list = PyList_New(entries);

      for( int i = 0; i < entries; i++) {
        PyObject* val = PyString_FromString(((string)p.Value(i)).c_str());
        PyList_SetItem(list, i, val);
      }
      PyDict_SetItemString(params, p.Name().c_str(), list);
      Py_DecRef( list );
    }
    else {
      string tmp = p.Value();
      value = PyString_FromString(tmp.c_str());
      PyDict_SetItemString(params, p.Name().c_str(), value);
      Py_DecRef(value);
    }
  }
  CallMethod("_set_parameters", params);
  Py_DecRef(params);

  for( int i = 0; i < Parameters->Size(); ++i ) {
    const Param& p = ( *Parameters )[ i ];
    PyObject* py_name = PyString_FromString(p.Name().c_str());
    PyObject* py_rowlab = ConvertLabelIndexToPyList(p.RowLabels());
    PyObject* py_collab = ConvertLabelIndexToPyList(p.ColumnLabels());
    PyObject* py_result = CallMethod("_param_labels", py_name, py_rowlab, py_collab);
    Py_DecRef(py_result);
    Py_DecRef(py_collab);
    Py_DecRef(py_rowlab);
    Py_DecRef(py_name);
  }
}

void
FILTER_NAME::ReceiveParametersFromPython()
{
  PyObject* py_params = CallMethod("_get_parameters");
  PyObject* py_value;

  for( int ip = 0; ip < Parameters->Size(); ++ip ) {

    const Param& p = ( *Parameters )[ ip ];
    const char* name = p.Name().c_str();
    py_value = PyDict_GetItemString(py_params, name); // py_value doesn't need to be DECREFfed - it returns only a *borrowed* reference
    if(py_value == NULL) continue;

    if(p.NumRows() == 0 || p.NumColumns() == 0) {
      if(PyString_Check(py_value) && PyString_Size(py_value) == 0) continue;
      if(PyList_Check(py_value) && PyList_Size(py_value) == 0) continue;
    }
    if(strcmp(p.Type().c_str(), "matrix") == 0) {
      int nrows = p.NumRows();
      int ncols = p.NumColumns();

      if(!PyList_Check(py_value)) {
        bcierr << "failed to propagate self.params['" << name << "'] because of wrong type (python list was expected)" << endl;
        mStayOpen = 1; return;
      }
      else if (PyList_Size(py_value) != nrows) {
        bcierr << "failed to propagate self.params['" << name << "'] because of wrong length (" << nrows << " entries were expected)" << endl;
        mStayOpen = 1; return;
      }
      else {
        for(int i = 0; i < nrows; i++) {
          PyObject* py_row = PyList_GetItem(py_value, i);
          if(!PyList_Check(py_row)) {
            bcierr << "failed to propagate self.params['" << name << "'][" << i << "] because of wrong type (python list was expected)" << endl;
            mStayOpen = 1; return;
          }
          else if (PyList_Size(py_row) != ncols) {
            bcierr << "failed to propagate self.params['" << name << "'][" << i << "] because of wrong length (" << ncols << " entries were expected)" << endl;
            mStayOpen = 1; return;
          }
          else {
            for(int j = 0; j < ncols; j++) {
              PyObject* py_item = PyList_GetItem(py_row, j);
              if(!PyString_Check(py_item)) {
                bcierr << "failed to propagate self.params['" << name << "'][" << i << "][" << j << "] because of wrong type (python string was expected)" << endl;
                mStayOpen = 1; return;
              }
              else {
                string tmp = PyString_AsString(py_item);
                Parameter(name)(i,j) = tmp;
              }
            }
          }
        }
      }
    }
    else if(p.Type().size() >= 4 && strcmp(p.Type().c_str() + p.Type().size() - 4, "list") == 0) {
      int entries = max(p.NumRows(),p.NumColumns());
      if(!PyList_Check(py_value)) {
        bcierr << "failed to propagate self.params['" << name << "'] because of wrong type (python list was expected)" << endl;
        mStayOpen = 1; return;
      }
      else if (PyList_Size(py_value) != entries) {
        bcierr << "failed to propagate self.params['" << name << "'] because of wrong length (" << entries << " entries were expected)" << endl;
        mStayOpen = 1; return;
      }
      else {
        int ind = 0;
        for(int i = 0; i < p.NumRows(); i++) {
          for(int j = 0; j < p.NumColumns(); j++, ind++) {
            PyObject* py_item = PyList_GetItem(py_value, ind);
            if(!PyString_Check(py_item)) {
              bcierr << "failed to propagate self.params['" << name << "'][" << ind << "] because of wrong type (python string was expected)" << endl;
              mStayOpen = 1; return;
            }
            else {
              string tmp = PyString_AsString(py_item);
              Parameter(name)(i,j) = tmp;
            }
          }
        }
      }
    }
    else {
      if(!PyString_Check(py_value)) {
        bcierr << "failed to propagate self.params['" << name << "'] because of wrong type (python string was expected)" << endl;
        mStayOpen = 1; return;
      }
      else {
        string tmp = PyString_AsString(py_value);
        Parameter(name) = tmp;
      }
    }
  }
  Py_DecRef(py_params);
}

void
FILTER_NAME::SendStatesToPython() const
{
  PyObject* states = PyDict_New();
  PyObject* pyvalue;
  for( const auto& s : *States ) {
    const char* name = s.Name().c_str();
    pyvalue = PyInt_FromLong(State(name));
    PyDict_SetItemString(states, name, pyvalue);
    Py_DecRef(pyvalue);
  }
  CallMethod("_set_states", states);
  Py_DecRef(states);
}

StateMap
FILTER_NAME::ReceiveStatesFromPython() const
{
  PyObject *obj = CallMethod("_get_states");
  PyObject *state_value;
  long value;

  StateMap m;
  for( const auto& s : *States ) {
    const char* name = s.Name().c_str();
    state_value = PyDict_GetItemString(obj, name); // state_value doesn't need to be DECREFfed - it returns only a *borrowed* reference
    value = PyInt_AsLong(state_value); // must use PyInt_AsLong - Float_AsDouble returns strange numbers
    if (value == -1 && PyErr_Occurred()) PyErr_Clear(); // in case of an exception, we set the state to -1 and ignore the exception.
    m[name] = value;
  }
  Py_DecRef(obj);
  return m;
}

void
FILTER_NAME::UpdateStateChangesFromPython( StateMap& before, StateMap& after ) const
{
  for( const auto& s : *States ) {
    const char* name = s.Name().c_str();
    if( strcmp( name, "SourceTime" ) == 0 ) continue;
    if( strcmp( name, "StimulusTime" ) == 0 ) continue;
    if( strcmp( name, "AppStartTime" ) == 0 ) continue;
    StateType value = after[name];
    if( before.empty() || value != before[name] ) {
      StateRef s = State(name);
      if(s != value) s = value;
    }
  }
}

void
FILTER_NAME::SendStatePrecisionsToPython() const
{
  PyObject* bits = PyDict_New();
  PyObject* pyvalue;
  for( const auto& s : *States ) {
    const char* name = s.Name().c_str();
    pyvalue = PyInt_FromLong( s.Length() );
    PyDict_SetItemString(bits, name, pyvalue);
    Py_DecRef(pyvalue);
  }
  CallMethod("_set_state_precisions", bits);
}

////////////////////////////////////////////////////////////////////////////////
// Converters between BCI2000 classes and Python objects
////////////////////////////////////////////////////////////////////////////////

PyObject*
FILTER_NAME::ConvertSignalToPyObject(const GenericSignal& sig) const
{
  PyObject* pResult = PyTuple_New( sig.Channels() );
  for( int ch = 0; ch < sig.Channels(); ++ch )
  {
    if( sig.Elements() == 1 )
    {
      PyTuple_SetItem( pResult, ch, PyFloat_FromDouble( sig( ch, 0 ) ) );
    }
    else
    {
      PyObject* pList = PyList_New( sig.Elements() );
      for( int el = 0; el < sig.Elements(); ++el )
        PyList_SetItem( pList, el, PyFloat_FromDouble( sig( ch, el ) ) );
      PyTuple_SetItem( pResult, ch, pList );
    }
  }
  return pResult;
}

void
FILTER_NAME::ConvertPyObjectToSignal(PyObject* pObject, GenericSignal& sig) const
{
  int channels = static_cast<int>( PyTuple_Size( pObject ) );
  for( int ch = 0; ch < channels && ch < sig.Channels(); ++ch )
  {
    PyObject* pEntry = PyTuple_GetItem( pObject, ch );
    if( PyList_Check( pEntry ) )
    {
      int elements = static_cast<int>( PyList_Size( pEntry ) );
      for( int el = 0; el < elements && el < sig.Elements(); ++el )
      {
        PyObject* pElement = PyList_GetItem( pEntry, el );
        sig( ch, el ) = PyFloat_AsDouble( pElement );
      }
    }
    else
      sig( ch, 0 ) = PyFloat_AsDouble( pEntry );
  }
}

PyObject*
FILTER_NAME::ConvertPropertiesToPyObject(const SignalProperties& inSignalProperties) const
{
  PyObject* pyProperties = PyDict_New();

  PyDict_SetItem(pyProperties, PyString_FromString("Name"), PyString_FromString(inSignalProperties.Name().c_str()));
  PyDict_SetItem(pyProperties, PyString_FromString("Type"), PyString_FromString(inSignalProperties.Type().Name()));

  PyDict_SetItem(pyProperties, PyString_FromString("ChannelLabels"), ConvertLabelIndexToPyList(inSignalProperties.ChannelLabels()));
  PyDict_SetItem(pyProperties, PyString_FromString("ElementLabels"), ConvertLabelIndexToPyList(inSignalProperties.ElementLabels()));

  PyDict_SetItem(pyProperties, PyString_FromString("ChannelUnit"), ConvertPhysicalUnitToPyDict(inSignalProperties.ChannelUnit()));
  PyDict_SetItem(pyProperties, PyString_FromString("ElementUnit"), ConvertPhysicalUnitToPyDict(inSignalProperties.ElementUnit()));
  PyDict_SetItem(pyProperties, PyString_FromString("ValueUnit"),   ConvertPhysicalUnitToPyDict(inSignalProperties.ValueUnit()));

  return pyProperties;
}

void
FILTER_NAME::ConvertPyObjectToProperties(PyObject* pyOutSignalProperties, SignalProperties& outSignalProperties) const
{
  //converting name
  PyObject* value = PyDict_GetItemString(pyOutSignalProperties, "Name");
  char* name = PyString_AsString(value);
  outSignalProperties.SetName(string(name));

  //converting type
  value = PyDict_GetItemString(pyOutSignalProperties, "Type");
  name = PyString_AsString(value);
  if( strcmp(name, "none") == 0){
    outSignalProperties.SetType(SignalType::none);
  }
  else if( strcmp(name, "int16") == 0){
    outSignalProperties.SetType(SignalType::int16);
  }
  else if( strcmp(name, "float24") == 0){
    outSignalProperties.SetType(SignalType::float24);
  }
  else if( strcmp(name, "float32") == 0){
    outSignalProperties.SetType(SignalType::float32);
  }
  else if( strcmp(name, "int32") == 0){
    outSignalProperties.SetType(SignalType::int32);
  }
  else {
    throw bciexception << "'" << name << "' is not one of the supported data types";
  }
  ConvertPyListToLabelIndex(PyDict_GetItemString(pyOutSignalProperties, "ChannelLabels"), outSignalProperties.ChannelLabels());
  ConvertPyListToLabelIndex(PyDict_GetItemString(pyOutSignalProperties, "ElementLabels"), outSignalProperties.ElementLabels());

  ConvertPyDictToPhysicalUnit(PyDict_GetItemString(pyOutSignalProperties, "ChannelUnit"), outSignalProperties.ChannelUnit());
  ConvertPyDictToPhysicalUnit(PyDict_GetItemString(pyOutSignalProperties, "ElementUnit"), outSignalProperties.ElementUnit());
  ConvertPyDictToPhysicalUnit(PyDict_GetItemString(pyOutSignalProperties, "ValueUnit"), outSignalProperties.ValueUnit());
}

PyObject*
FILTER_NAME::ConvertLabelIndexToPyList(LabelIndex from) const
{
  PyObject* channelNames = PyList_New(from.Size());
  for(int i = 0; i < from.Size(); i++){
    PyList_SetItem(channelNames , i, PyString_FromString(from[i].c_str()));
  }
  return channelNames;
}


void
FILTER_NAME::ConvertPyListToLabelIndex(PyObject* from, LabelIndex& to) const
{
  stringstream converter;

  to.Resize(PyList_Size(from));
  for(int i = 0; i < to.Size(); i++){
    string channelName = PyString_AsString(PyList_GetItem(from, i));

    int converted;
    converter << channelName;
    converter >> converted;

    to[i] = channelName;
  }
}

PyObject*
FILTER_NAME::ConvertPhysicalUnitToPyDict(PhysicalUnit from) const
{
  PyObject* valueUnit = PyDict_New();

  PyDict_SetItem(valueUnit, PyString_FromString("Offset"), PyFloat_FromDouble(from.Offset()));
  PyDict_SetItem(valueUnit, PyString_FromString("Gain"),   PyFloat_FromDouble(from.Gain()));
  PyDict_SetItem(valueUnit, PyString_FromString("Symbol"), PyString_FromString(from.Symbol().c_str()));
  PyDict_SetItem(valueUnit, PyString_FromString("RawMin"), PyFloat_FromDouble(from.RawMin()));
  PyDict_SetItem(valueUnit, PyString_FromString("RawMax"), PyFloat_FromDouble(from.RawMax()));

  return valueUnit;
}

void
FILTER_NAME::ConvertPyDictToPhysicalUnit(PyObject* from, PhysicalUnit& to) const
{
  PyObject* valueUnit = PyDict_GetItemString(from, "Offset");
  to.SetOffset(PyFloat_AsDouble(valueUnit));
  valueUnit = PyDict_GetItemString(from, "Gain");
  to.SetGain(PyFloat_AsDouble(valueUnit));
  valueUnit = PyDict_GetItemString(from, "Symbol");
  to.SetSymbol(PyString_AsString(valueUnit));
  valueUnit = PyDict_GetItemString(from, "RawMin");
  to.SetRawMin(PyFloat_AsDouble(valueUnit));
  valueUnit = PyDict_GetItemString(from, "RawMax");
  to.SetRawMax(PyFloat_AsDouble(valueUnit));
}

////////////////////////////////////////////////////////////////////////////////
// General-purpose functions
////////////////////////////////////////////////////////////////////////////////

void
FILTER_NAME::DoubleErr(const char *msg, const char *qualifier, bool notify_restart) const
{
  if(qualifier && strlen(qualifier)) cerr << "[" << qualifier << "] ";
  cerr   << (notify_restart ? "BCI2000 must be restarted: " : "")  << msg << endl;
  bcierr << (notify_restart ? "BCI2000 must be restarted: " : "")  << msg << endl;
}

void
FILTER_NAME::HandleEndUserError(const EndUserError& e, string qualifier) const
{
  UnblockThreads();
  bcierr << e.What() << endl;
}

void
FILTER_NAME::HandleException(const std::exception& e, string qualifier) const
{
  UnblockThreads();
  DoubleErr(e.what(), qualifier.c_str(), true);
}

void
FILTER_NAME::ChangeDir(const string& d)
{
  if( ::chdir( d.c_str() ) || !FileUtils::ChangeDirectory( d ) )
    throw bciexception << "failed to change working directory from " << FileUtils::WorkingDirectory() << " to " << d;
}

void
FILTER_NAME::OpenConsole(const char *title)
{
#ifdef _WIN32
  AllocConsole();
  if(title && *title) SetConsoleTitle(title);
#endif // _WIN32
}

////////////////////////////////////////////////////////////////////////////////
// Python thread handling
////////////////////////////////////////////////////////////////////////////////

void
FILTER_NAME::BlockThreads() const
{
  // always run this before calling Python functions, otherwise we get random crashes...
  // see http://docs.python.org/api/threads.html
  PyEval_RestoreThread(mpThreadState);
}

void
FILTER_NAME::UnblockThreads() const
{
  // ... and call this when done calling them, otherwise we're not multithreading anymore.
  // Make sure that the singlethreaded code is much faster than a frame length,
  // otherwise we're likely to miss a frame.
  mpThreadState = PyEval_SaveThread();
}

////////////////////////////////////////////////////////////////////////////////
// Python-specific tools (NB: more convenient than efficient - better not to use in time-critical code)
////////////////////////////////////////////////////////////////////////////////

string
FILTER_NAME::EscapePythonString(string in)
{
  string out;
  for(string::iterator s = in.begin(); s != in.end(); s++) {
    if(*s=='\\' || *s=='\'') out += "\\";
    out += *s;
  }
  return out;
}

void
FILTER_NAME::EvalPythonString(string s)
{
  int error = PyRun_SimpleString(s.c_str());
  HandlePythonError("evaluation of \"" + s + "\"", error!=0);
}

PyObject*
FILTER_NAME::CallModuleMember(string module, string member, PyObject* arg)
{
  PyObject* mod = PyImport_AddModule((char*)module.c_str());
  HandlePythonError("PyImport_AddModule on " + module);
  if(!mod){throw bciexception << "module object for " + module + " is NULL";}

  PyObject* dict = PyModule_GetDict(mod);
  HandlePythonError("PyModule_GetDict on " + module);
  if(!dict){throw bciexception << "dict object for module " + module + " is NULL";}

  PyObject* func = PyDict_GetItemString(dict, member.c_str());
  HandlePythonError("PyDict_GetItemString on " + member + " from " + module);
  if(!func){throw bciexception << "item " + member + " from module " + module + " is NULL";}

  PyObject* result = PyObject_CallFunctionObjArgs(func, arg, NULL);
  HandlePythonError("PyObject_CallFunctionObjArgs on " + member + " from " + module);
  if(!result){throw bciexception << "result of calling " + member + "() from module " + module + " is NULL";}

  // apparently should not decref mod, dict and func here
  return result;
}

PyObject*
FILTER_NAME::CallMethod(const char* name, PyObject* arg1, PyObject* arg2, PyObject* arg3) const
{
  HandlePythonError("Python Engine is in the error state before CallMethod");
  PyObject* py_name = PyString_FromString((char*)name);
  PyObject* out = PyObject_CallMethodObjArgs(mpInstance, py_name, arg1, arg2, arg3, NULL);
  HandlePythonError(name);
  Py_DecRef (py_name);
  return out;
}

PyObject*
FILTER_NAME::CallHook(const char* name, PyObject* arg1, PyObject* arg2) const
{
  HandlePythonError("Python Engine is in the error state before CallHook");
  PyObject* py_method = PyObject_GetAttrString(mpInstance, (char*)name);
  PyObject* py_wrappername = PyString_FromString("_call_hook");
  PyObject* out = PyObject_CallMethodObjArgs(mpInstance, py_wrappername, py_method, arg1, arg2, NULL);
  HandlePythonError(name);
  Py_DecRef (py_wrappername);
  Py_DecRef (py_method);
  return out;
}

void
FILTER_NAME::HandlePythonError(string msg, bool errorCodeReturned) const
{
  int error = 0;
  int isForEndUser = 0;
  string report;

  if(mpInstance) {
    PyObject* py_error_occurred = PyObject_GetAttrString(mpInstance, (char*)"_error_reported");
    if(py_error_occurred != NULL) {
      error = PyInt_AsLong(py_error_occurred);
      Py_DecRef(py_error_occurred);
    }
    if(error) {
      PyObject* py_error_info = PyObject_CallMethod(mpInstance, (char*)"_flush_error_info", NULL);
      report = PyString_AsString(PyTuple_GetItem(py_error_info, 0));
      isForEndUser = PyInt_AsLong(PyTuple_GetItem(py_error_info, 1));
      Py_DecRef(py_error_info);
    }
  }
  if(error == 0 && (errorCodeReturned || PyErr_Occurred())) {
    error = 1;
    PyErr_Print();
    PyErr_Clear();
  }
  if(error == 0) return;

  if(isForEndUser) throw EndUserError(report.c_str());

  mStayOpen = 1;
  if(msg.length()) msg = "Python error during " + msg;
  else msg = "Python error";
  if(report.length()) msg = msg + ": " + report;
  throw bciexception << msg;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

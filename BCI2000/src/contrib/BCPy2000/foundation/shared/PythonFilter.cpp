//   $Id: PythonFilter.cpp 5897 2019-04-05 15:13:41Z madamek $
//
//   This file is part of the BCPy2000 foundation, a set of modules for
//   the BCI2000 <http://bci2000.org/> that allow communication with a
//   Python framework built on top. It is distributed together with the
//   BCPy2000 framework.
//
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
#ifdef _WIN32
# ifndef __BORLANDC__
#  undef _WIN32_WINNT
#  define _WIN32_WINNT 0x500
# else // __BORLANDC__
# include <float.h>
# endif // __BORLANDC__
#include <Windows.h>
#endif // _WIN32

#include "PCHIncludes.h"
#pragma hdrstop

#include "FileUtils.h"
#include "PythonFilter.h"

#include <cstdio>
#include <ctime>


#ifdef _WIN32

#define FILESEP "\\"
#if _MSC_VER
# include <direct.h>
#else
# include <dir.h>
#endif

#else // _WIN32

#define FILESEP "/"
#include <sys/stat.h>
#include <sys/select.h>
#include <time.h>
void Sleep(long msec)
{
  struct timeval t;
  t.tv_sec = msec / 1000L;
  t.tv_usec = (msec % 1000L) * 1000L;
  select(0,0,0,0,&t);
}

#endif // _WIN32

#if __BORLANDC__
# define TO_CSTRING(x) AnsiString(x).c_str()
#else
# define TO_CSTRING(x) std::string(x).c_str()
#endif // __BORLANDC__


using namespace std;
#if MODTYPE == 1
RegisterFilter( FILTER_NAME, 1 );
#elif MODTYPE == 2
#include "SpatialFilter.h"
Filter( SpatialFilter, 2.B );
Filter( FILTER_NAME, 2.C );
#elif MODTYPE == 3
RegisterFilter( FILTER_NAME, 3 );
#endif

#define FAIL(a) throw (bciexception << a)

EndUserError::EndUserError(const char* s) : bci::Exception(s) {}

stringstream DBSTRING;
void _DB() {DBSTRING << " && pause"; string s = DBSTRING.str(), t = "echo ", u = t + s; system(u.c_str()); DBSTRING.str("");}
#define DB(a) (DBSTRING << a);_DB()

////////////////////////////////////////////////////////////////////////////////
// Implementations of the usual GenericFilter virtual methods
////////////////////////////////////////////////////////////////////////////////

FILTER_NAME::FILTER_NAME()
:
  cur_time( new PrecisionTime )
{
  try {
    _save = 0;
    bci2000_instance = 0;
    stay_open = 0;
    use_console = 0;

#if MODTYPE == 3
    BEGIN_STATE_DEFINITIONS
      "AppStartTime 16 0 0 0",
      "StimulusTime 16 17528 0 0",
    END_STATE_DEFINITIONS
#endif

    std::string originalDir = FileUtils::WorkingDirectory();

    std::string dllname       = OptionalParameter(FILTER_ABBREV "DLL",       "");
		std::string profilename   = OptionalParameter(FILTER_ABBREV "Profile",       "");
    std::string logFile       = OptionalParameter(FILTER_ABBREV "Log",       "");
    std::string frameworkDir  = OptionalParameter(FILTER_ABBREV "Framework", "");
    std::string workingDir    = OptionalParameter(FILTER_ABBREV "WD",        std::string("..") + FILESEP + "python");
    std::string developerFile = OptionalParameter(FILTER_ABBREV "ClassFile", DEFAULT_CLASS_FILE);
    int         ipshell       = OptionalParameter(FILTER_ABBREV "Shell",      1);

    std::string date_escape = "###";
    size_t found = logFile.find(date_escape, 0);
    if(found != std::string::npos) {
      char * datestr = new char[32];
      time_t tt;
      time(&tt);
      struct tm *t = localtime(&tt);
      sprintf(datestr, "%04d%02d%02d%02d%02d%02d", t->tm_year+1900, t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
      logFile.replace(found, date_escape.length(), datestr);
      delete [] datestr;
    }
    use_console = (ipshell || !logFile.length());
    if(use_console) OpenConsole(FILTER_ABBREV);

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
      std::string cmd;
      cmd = EscapePythonString(logFile);
      cmd = "a=open('" + cmd + "', 'w'); import sys; sys.stderr = a; sys.stdout = a";
      EvalPythonString(cmd);
    }

    std::string conmod, coremod, genmod;

    if(frameworkDir.length()) {
      ChangeDir(frameworkDir); // ...and then redo it. <sigh>
      EvalPythonString("import sys,os");
      EvalPythonString("if not os.getcwd() in sys.path: sys.path.append(os.getcwd())");
      conmod = PYTHON_CONSOLE;
      coremod = PYTHON_COREMODULE;
      genmod = PYTHON_MODULE;
    }
    else {
			
      conmod = PYTHON_CONSOLE_INSTALLED;
      coremod = PYTHON_COREMODULE_INSTALLED;
      genmod = PYTHON_MODULE_INSTALLED;

    }

    if(use_console) {
			bciout << conmod << std::endl;
      PyImport_ImportModule((char*)conmod.c_str());
      HandlePythonError("attempt to import " + conmod);
    }

#if defined __BORLANDC__
    _control87(MCW_EM, MCW_EM); // required to avoid Borland-specific crashes with numpy
#endif

    EvalPythonString("from " + coremod + " import *");
    EvalPythonString("from " + genmod + " import *");
    CallModuleMember(coremod, "register_framework_dir"); //TODO: need to decref the result?
    ChangeDir(originalDir); // change in two stages to cope with both absolute and relative paths
    if(workingDir.length()) ChangeDir(workingDir);
    EvalPythonString("import os;os.chdir(\"" + EscapePythonString( FileUtils::WorkingDirectory() ) + "\")");
    CallModuleMember(coremod, "register_working_dir"); //TODO: need to decref the result?
    if(developerFile.length()) {
      PyObject *py_devfile = PyString_FromString(developerFile.c_str());
      PyObject *py_resolved = CallModuleMember(coremod, "search_for_file", py_devfile);
      Py_DecRef(py_devfile);
      string cpp_resolved = PyString_AsString(py_resolved);
      Py_DecRef(py_resolved);
#if PYVERSION == 2
      EvalPythonString("execfile('" + EscapePythonString(cpp_resolved) + "')");
#else
			
			if (profilename.length() && profilename != "-")
			{
				std::string cmd = EscapePythonString(profilename);
				EvalPythonString("import sys; sys.argv=['"+EscapePythonString(cpp_resolved)+ "'];import profile; profile.run(open('"+EscapePythonString(cpp_resolved)+"').read(), '"+cmd+"')");
			}
			else
			{
				std::string startupString = "import sys; sys.argv=['" + EscapePythonString(cpp_resolved) + "'];";
				//startupString += std::string("globals={};locals={};");
				//startupString += std::string("globals.update({'__file__': '" +EscapePythonString(cpp_resolved)+"','__name__':'__main__',});");
				startupString += std::string("file=open('"+EscapePythonString(cpp_resolved)+"');exec(compile(file.read(),'"+EscapePythonString(cpp_resolved)+"','exec'),globals(),locals()); file.close()");
				EvalPythonString(startupString);
			}
				//EvalPythonString("exec(open('"+EscapePythonString(cpp_resolved)+"').read())");
#endif
      bci2000_instance = CallModuleMember("__main__", PYTHON_SUBCLASS);
    }
    else {
      bci2000_instance = CallModuleMember("__main__", PYTHON_SUPERCLASS);
    }

    if(ipshell) CallMethod("_enable_shell");

    PyObject*key,*val;

    key = PyString_FromString("installation_dir");
    val = PyString_FromString(FileUtils::InstallationDirectory().c_str());
    CallMethod("__setattr__", key, val);
    Py_DecRef(key); // Py_DecRef(val);

    key = PyString_FromString("original_working_dir");
    val = PyString_FromString(originalDir.c_str());
    CallMethod("__setattr__", key, val);
    Py_DecRef(key); // Py_DecRef(val);

    CallMethod("_start");


    PyObject *py_ret = CallHook("_Construct");

    PyObject *py_params = PyTuple_GetItem(py_ret, 0);
    PyObject *py_states = PyTuple_GetItem(py_ret, 1);

    // Add the parameters and states requested by the Python _Construct function.
    size_t numParamDefs = PyList_Size(py_params);
    for( size_t i = 0; i < numParamDefs; ++i ) {
      string paramDef = PyString_AsString(PyList_GetItem(py_params, i));
      // std::cerr << "registering parameter: " << paramDef << std::endl;
      if(!Parameters->Add( paramDef ) ) DoubleErr(("Error in parameter definition: " + paramDef).c_str(), "Construct");
    }

    size_t numStateDefs = PyList_Size(py_states);
    for( size_t i = 0; i < numStateDefs; ++i ) {
      string stateDef = PyString_AsString(PyList_GetItem(py_states, i));
      // std::cerr << "registering state: " << stateDef << std::endl;
      if(!States->Add( stateDef )) DoubleErr(("Error in state definition: " + stateDef).c_str(), "Construct");
    }
    UnblockThreads();
    // std::cerr << "done creating engine." << std::endl;
  }
  catch(EndUserError& e) {
    HandleEndUserError(e, "Construct");
  }
  catch(bci::Exception& e) {
    HandleException(e, "Construct");
  }
}

FILTER_NAME::~FILTER_NAME()
{
  if(stay_open) {
    if(use_console) {
#ifdef _WIN32
      while(GetConsoleWindow()) ::Sleep(1);
#else // _WIN32
      while(1) ::Sleep(1);
#endif // _WIN32
    }
  }
  else {
    Halt();
    try {
      BlockThreads();
      CallHook("_Destruct");
      UnblockThreads();
    }
    catch(EndUserError& e) {
      HandleEndUserError(e, "Destruct");
    }
    catch(bci::Exception& e) {
      HandleException(e, "Destruct");
    }
#ifdef _WIN32
    if(use_console) FreeConsole();
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
    py_list = PyObject_GetAttrString(bci2000_instance, (char*)"_writeable_params");
    if(py_list && PyList_Check(py_list)) {
      size_t n = PyList_Size(py_list);
      for(size_t i = 0; i < n; i++) {
        const char *name = PyString_AsString(PyList_GetItem(py_list, i));
        Parameter(name); // allows write access to this parameter
      }
    }
    if( py_list )
      Py_DecRef(py_list);

    PreflightCondition(SignalType::ConversionIsSafe(inSignalProperties.Type(), outSignalProperties.Type()));

    UnblockThreads();
  }
  catch(EndUserError& e) {
    HandleEndUserError(e, "Preflight");
  }
  catch(bci::Exception& e) {
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
  catch(bci::Exception& e) {
    HandleException(e, "Initialize");
  }
}

void
FILTER_NAME::Process( const GenericSignal& input, GenericSignal& output )
{
#if MODTYPE == 3
  State( "AppStartTime" ) = cur_time->Now();
#endif
try {
    BlockThreads();
    SendStatesToPython();
    // Watch out for the memory leaks. They come when you least expect them.
    // http://www-cgi.uni-regensburg.de/WWW_Server/Dokumentation/Python/ext.pdf
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
  catch(bci::Exception& e) {
    State("Running") = 0;
    HandleException(e, "Process");
  }
#if MODTYPE == 3
  State( "StimulusTime" ) = cur_time->Now();
#endif
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
  catch(bci::Exception& e) {
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
  catch(bci::Exception& e) {
    HandleException(e, "StopRun");
  }
}

void
FILTER_NAME::Resting()
{
  try {
    BlockThreads();
    SendParametersToPython();
    // BCI2000 doc seemed to suggest that params would propagate to other modules when StopRun returned
    // but actually, this call seems to have no effect---params don't seem to propagate until StartRun
    PyObject* ret = CallHook("_Resting");
    UnblockThreads();
  }
  catch(EndUserError& e) {
    HandleEndUserError(e, "Resting");
  }
  catch(bci::Exception& e) {
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
  catch(bci::Exception& e) {
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
          PyObject* val = PyString_FromString(((std::string)p.Value(i,j)).c_str());
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
        PyObject* val = PyString_FromString(((std::string)p.Value(i)).c_str());
        PyList_SetItem(list, i, val);
      }
      PyDict_SetItemString(params, p.Name().c_str(), list);
      Py_DecRef( list );
    }
    else {
      std::string tmp = p.Value();
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
        stay_open = 1; return;
      }
      else if (PyList_Size(py_value) != nrows) {
        bcierr << "failed to propagate self.params['" << name << "'] because of wrong length (" << nrows << " entries were expected)" << endl;
        stay_open = 1; return;
      }
      else {
        for(int i = 0; i < nrows; i++) {
          PyObject* py_row = PyList_GetItem(py_value, i);
          if(!PyList_Check(py_row)) {
            bcierr << "failed to propagate self.params['" << name << "'][" << i << "] because of wrong type (python list was expected)" << endl;
            stay_open = 1; return;
          }
          else if (PyList_Size(py_row) != ncols) {
            bcierr << "failed to propagate self.params['" << name << "'][" << i << "] because of wrong length (" << ncols << " entries were expected)" << endl;
            stay_open = 1; return;
          }
          else {
            for(int j = 0; j < ncols; j++) {
              PyObject* py_item = PyList_GetItem(py_row, j);
              if(!PyString_Check(py_item)) {
                bcierr << "failed to propagate self.params['" << name << "'][" << i << "][" << j << "] because of wrong type (python string was expected)" << endl;
                stay_open = 1; return;
              }
              else {
                std::string tmp = PyString_AsString(py_item);
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
        stay_open = 1; return;
      }
      else if (PyList_Size(py_value) != entries) {
        bcierr << "failed to propagate self.params['" << name << "'] because of wrong length (" << entries << " entries were expected)" << endl;
        stay_open = 1; return;
      }
      else {
        int ind = 0;
        for(int i = 0; i < p.NumRows(); i++) {
          for(int j = 0; j < p.NumColumns(); j++, ind++) {
            PyObject* py_item = PyList_GetItem(py_value, ind);
            if(!PyString_Check(py_item)) {
              bcierr << "failed to propagate self.params['" << name << "'][" << ind << "] because of wrong type (python string was expected)" << endl;
              stay_open = 1; return;
            }
            else {
              std::string tmp = PyString_AsString(py_item);
              Parameter(name)(i,j) = tmp;
            }
          }
        }
      }
    }
    else {
      if(!PyString_Check(py_value)) {
        bcierr << "failed to propagate self.params['" << name << "'] because of wrong type (python string was expected)" << endl;
        stay_open = 1; return;
      }
      else {
        std::string tmp = PyString_AsString(py_value);
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

#if PYVERSION == 2
PyObject*
FILTER_NAME::ConvertSignalToPyObject(const GenericSignal& sig, PyObject* pyobj) const
{
  size_t nCh = (size_t)sig.Channels();
  size_t nEl = (size_t)sig.Elements();
  size_t elSize = sizeof(double);
  if(elSize != 8) FAIL("internal error: unsupported architecture - sizeof(double) is not 8");
  if(!pyobj) pyobj = PyString_FromStringAndSize(NULL, nCh*nEl*elSize);
  if(!pyobj) FAIL("failed to allocate memory for signal buffer");
  double* vals = (double*)PyString_AsString(pyobj);
  for(size_t ch = 0; ch < nCh; ch++)
    for(size_t el = 0; el < nEl; el++)
    *vals++ = sig(ch, el);
  return pyobj;
}

void
FILTER_NAME::ConvertPyObjectToSignal(PyObject* pyobj, GenericSignal& sig) const
{
  size_t nCh = (size_t)sig.Channels();
  size_t nEl = (size_t)sig.Elements();
  size_t elSize = sizeof(double);
  if(elSize != 8) FAIL("internal error: unsupported architecture - sizeof(double) is not 8");
  if(!pyobj) FAIL("No python signal data available (null pointer)");
  if(!PyString_Check(pyobj)) FAIL("python signal is supposed to be a string");
  if(PyString_Size(pyobj) != nCh * nEl * elSize) FAIL("python signal contains unexpected number of bytes");
  double* vals = (double*)PyString_AsString(pyobj);
  for(size_t ch = 0; ch < nCh; ch++)
    for(size_t el = 0; el < nEl; el++)
    sig(ch, el) = *vals++;
}
#else

PyObject*
FILTER_NAME::ConvertSignalToPyObject(const GenericSignal& sig, PyObject* pyobj) const
{
  size_t nCh = (size_t)sig.Channels();
  size_t nEl = (size_t)sig.Elements();
  size_t elSize = sizeof(double);
  if(elSize != 8) FAIL("internal error: unsupported architecture - sizeof(double) is not 8");
  if(!pyobj) pyobj = PyBytes_FromStringAndSize(NULL, nCh*nEl*elSize);
  if(!pyobj) FAIL("failed to allocate memory for signal buffer");
  double* vals = (double*)PyBytes_AsString(pyobj);
  for(size_t ch = 0; ch < nCh; ch++)
    for(size_t el = 0; el < nEl; el++)
    *vals++ = sig(ch, el);
  return pyobj;
}

void
FILTER_NAME::ConvertPyObjectToSignal(PyObject* pyobj, GenericSignal& sig) const
{
  size_t nCh = (size_t)sig.Channels();
  size_t nEl = (size_t)sig.Elements();
  size_t elSize = sizeof(double);
  if(elSize != 8) FAIL("internal error: unsupported architecture - sizeof(double) is not 8");
  if(!pyobj) FAIL("No python signal data available (null pointer)");
  if(!PyBytes_Check(pyobj)) FAIL("python signal is supposed to be a string");
  if(PyBytes_Size(pyobj) != nCh * nEl * elSize) FAIL("python signal contains unexpected number of bytes");
  double* vals = (double*)PyBytes_AsString(pyobj);
  for(size_t ch = 0; ch < nCh; ch++)
    for(size_t el = 0; el < nEl; el++)
    sig(ch, el) = *vals++;
}
#endif

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
    FAIL("'" << name << "' is not one of the supported data types");
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
FILTER_NAME::HandleEndUserError(EndUserError& e, std::string qualifier) const
{
  UnblockThreads();
  bcierr << e.What() << endl;
}

void
FILTER_NAME::HandleException(bci::Exception& e, std::string qualifier) const
{
  UnblockThreads();
  DoubleErr(e.What().c_str(), qualifier.c_str(), true);
}

void
FILTER_NAME::ChangeDir(std::string& d)
{
  if( ::chdir( d.c_str() ) || !FileUtils::ChangeDirectory( d ) )
    FAIL( "failed to change working directory from " << FileUtils::WorkingDirectory() << " to " << d );
}

void
FILTER_NAME::OpenConsole(const char *title)
{
#ifdef _WIN32
  AllocConsole();
  if(title && *title) SetConsoleTitle(title);
  //freopen("conin$", "r", stdin);   // why were these calls necessary?
  //freopen("conout$", "w", stdout);
  //freopen("conout$", "w", stderr);
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
  PyEval_RestoreThread(_save);
}

void
FILTER_NAME::UnblockThreads() const
{
  // ... and call this when done calling them, otherwise we're not multithreading anymore.
  // Make sure that the singlethreaded code is much faster than a frame length,
  // otherwise we're likely to miss a frame.
  _save = PyEval_SaveThread();
}

////////////////////////////////////////////////////////////////////////////////
// Python-specific tools (NB: more convenient than efficient - better not to use in time-critical code)
////////////////////////////////////////////////////////////////////////////////

std::string
FILTER_NAME::EscapePythonString(std::string in)
{
  std::string out;
  for(std::string::iterator s = in.begin(); s != in.end(); s++) {
    if(*s=='\\' || *s=='\'') out += "\\";
    out += *s;
  }
  return out;
}

void
FILTER_NAME::EvalPythonString(std::string s)
{
  int error = PyRun_SimpleString(s.c_str());
  HandlePythonError("evaluation of \"" + s + "\"", error!=0);
}

PyObject*
FILTER_NAME::CallModuleMember(std::string module, std::string member, PyObject* arg)
{
  PyObject* mod = PyImport_AddModule((char*)module.c_str());
  HandlePythonError("PyImport_AddModule on " + module);
  if(!mod){FAIL("module object for " << module << " is NULL"); return NULL;}

  PyObject* dict = PyModule_GetDict(mod);
  HandlePythonError("PyModule_GetDict on " + module);
  if(!dict){FAIL("dict object for module " << module << " is NULL"); return NULL;}

  PyObject* func = PyDict_GetItemString(dict, member.c_str());
  HandlePythonError("PyDict_GetItemString on " + member + " from " + module);
  if(!func){FAIL("item " << member << " from module " << module << " is NULL"); return NULL;}

  PyObject* result = PyObject_CallFunctionObjArgs(func, arg, NULL);
  HandlePythonError("PyObject_CallFunctionObjArgs on " + member + " from " + module);
  if(!result){FAIL("result of calling " << member << "() from module " << module << " is NULL"); return NULL;}

  // apparently should not decref mod, dict and func here
  return result;
}

PyObject*
FILTER_NAME::CallMethod(const char* name, PyObject* arg1, PyObject* arg2, PyObject* arg3) const
{
  HandlePythonError("Python Engine is in the error state before CallMethod");
  PyObject* py_name = PyString_FromString((char*)name);
  PyObject* out = PyObject_CallMethodObjArgs(bci2000_instance, py_name, arg1, arg2, arg3, NULL);
  HandlePythonError(name);
  Py_DecRef (py_name);
  return out;
}

PyObject*
FILTER_NAME::CallHook(const char* name, PyObject* arg1, PyObject* arg2) const
{
  HandlePythonError("Python Engine is in the error state before CallHook");
  PyObject* py_method = PyObject_GetAttrString(bci2000_instance, (char*)name);
  PyObject* py_wrappername = PyString_FromString("_call_hook");
  PyObject* out = PyObject_CallMethodObjArgs(bci2000_instance, py_wrappername, py_method, arg1, arg2, NULL);
  HandlePythonError(name);
  Py_DecRef (py_wrappername);
  Py_DecRef (py_method);
  return out;
}

void
FILTER_NAME::HandlePythonError(std::string msg, bool errorCodeReturned) const
{
  int error = 0;
  int isForEndUser = 0;
  std::string report;

  if(bci2000_instance) {
    PyObject* py_error_occurred = PyObject_GetAttrString(bci2000_instance, (char*)"_error_reported");
    if(py_error_occurred != NULL) {
      error = PyInt_AsLong(py_error_occurred);
      Py_DecRef(py_error_occurred);
    }
    if(error) {
      PyObject* py_error_info = PyObject_CallMethod(bci2000_instance, (char*)"_flush_error_info", NULL);
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

  stay_open = 1;
  if(msg.length()) msg = "Python error during " + msg;
  else msg = "Python error";
  if(report.length()) msg = msg + ": " + report;
  FAIL(msg);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

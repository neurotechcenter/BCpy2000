//   $Id: PythonFilter.h 4472 2013-05-23 19:52:03Z mellinger $
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
#ifndef PythonFilterH
#define PythonFilterH

#include "BCIException.h"
#include "PrecisionTime.h"

#define PYTHON_CONSOLE              "EmbeddedPythonConsole"
#define PYTHON_CONSOLE_INSTALLED    "BCPy2000.EmbeddedPythonConsole"

#define PYTHON_COREMODULE           "BCI2000PythonCore"
#define PYTHON_COREMODULE_INSTALLED "BCPy2000.Generic"

#if   MODTYPE == 1
#include                            "GenericADC.h"
#define    FILTER_SUPERCLASS         GenericADC
#define    FILTER_NAME               PythonSrcADC
#define    FILTER_ABBREV            "PythonSrc"
#define    PYTHON_MODULE            "BCI2000PythonSource"
#define    PYTHON_MODULE_INSTALLED  "BCPy2000.GenericSource"
#define    PYTHON_SUPERCLASS        "BciGenericSource"
#define    PYTHON_SUBCLASS          "BciSource"
#define    DEFAULT_CLASS_FILE       "BciSource.py"
#elif MODTYPE == 2
#include                            "GenericFilter.h"
#define    FILTER_SUPERCLASS         GenericFilter
#define    FILTER_NAME               PythonSigFilter
#define    FILTER_ABBREV            "PythonSig"
#define    PYTHON_MODULE            "BCI2000PythonSignalProcessing"
#define    PYTHON_MODULE_INSTALLED  "BCPy2000.GenericSignalProcessing"
#define    PYTHON_SUPERCLASS        "BciGenericSignalProcessing"
#define    PYTHON_SUBCLASS          "BciSignalProcessing"
#define    DEFAULT_CLASS_FILE       "BciSignalProcessing.py"
#elif MODTYPE == 3
#include                            "GenericFilter.h"
#define    FILTER_SUPERCLASS         GenericFilter
#define    FILTER_NAME               PythonAppFilter
#define    FILTER_ABBREV            "PythonApp"
#define    PYTHON_MODULE            "BCI2000PythonApplication"
#define    PYTHON_MODULE_INSTALLED  "BCPy2000.GenericApplication"
#define    PYTHON_SUPERCLASS        "BciGenericApplication"
#define    PYTHON_SUBCLASS          "BciApplication"
#define    DEFAULT_CLASS_FILE       "BciApplication.py"
#endif


#ifndef DYNAMIC_PYTHON
#define DYNAMIC_PYTHON 1
#endif
#if DYNAMIC_PYTHON
#include "PythonWrapper.h"
#else
#include "Python.h"
#endif


class EndUserError : public bci::Exception {
  public:
    EndUserError(const char* s);
};

typedef                            long   StateType;
typedef std::map<std::string, StateType>  StateMap;
class FILTER_NAME : public FILTER_SUPERCLASS
{
  public:
    FILTER_NAME();
    virtual ~FILTER_NAME();
    virtual void Preflight( const SignalProperties&, SignalProperties& ) const;
    virtual void Initialize( const SignalProperties&, const SignalProperties& );
    virtual void Process( const GenericSignal& Input, GenericSignal& Output );
    virtual void StartRun();
    virtual void StopRun();
    virtual void Resting();
    virtual void Halt();

#if MODTYPE == 1
    virtual bool IsRealTimeSource() const { return false; } // permits --EvaluateTiming=0, to launch without realtime checking
#endif // MODTYPE

  private:
    PrecisionTime* cur_time;
    PyObject*      bci2000_instance;
    bool           use_console;

    mutable bool           stay_open;
    mutable PyThreadState* _save;

  protected:
    void        SendParametersToPython() const;
    void        ReceiveParametersFromPython();
    void        SendStatesToPython() const;
    StateMap    ReceiveStatesFromPython() const;
    void        UpdateStateChangesFromPython(StateMap& before, StateMap& after) const;
    void        SendStatePrecisionsToPython() const;

    PyObject*   ConvertSignalToPyObject(const GenericSignal& inSignal, PyObject* pyInSignal=NULL) const;
    void        ConvertPyObjectToSignal(PyObject* pyOutSignal, GenericSignal& outSignal) const;

    PyObject*   ConvertPropertiesToPyObject(const SignalProperties& inSignalProperties) const;
    void        ConvertPyObjectToProperties(PyObject* pyOutSignalProperties, SignalProperties& outSignalProperties) const;

    PyObject*   ConvertLabelIndexToPyList(LabelIndex from) const;
    void        ConvertPyListToLabelIndex(PyObject*, LabelIndex& to) const;

    PyObject*   ConvertPhysicalUnitToPyDict(PhysicalUnit from) const;
    void        ConvertPyDictToPhysicalUnit(PyObject* from, PhysicalUnit& to) const;

    void        DoubleErr(const char *msg, const char *qualifier=NULL, bool notify_restart=false) const;
    void        HandleEndUserError(EndUserError& e, std::string qualifier) const;
    void        HandleException(bci::Exception& e, std::string qualifier) const;
    void        ChangeDir(std::string& d);
    void        OpenConsole(const char *title);

    void        BlockThreads() const;
    void        UnblockThreads() const;

    std::string EscapePythonString(std::string in);
    void        EvalPythonString(std::string s);
    PyObject*   CallModuleMember(std::string module, std::string member, PyObject* arg=NULL);
    PyObject*   CallMethod(const char* name, PyObject* arg1=NULL, PyObject* arg2=NULL, PyObject* arg3=NULL) const;
    PyObject*   CallHook(const char* name, PyObject* arg1=NULL, PyObject* arg2=NULL) const;
    void        HandlePythonError(std::string msg, bool errorCodeReturned=false) const;

};

#endif // PythonFilterH

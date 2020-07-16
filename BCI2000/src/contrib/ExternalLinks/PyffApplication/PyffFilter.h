//   $Id: PyffFilter.h 4982 2015-08-26 20:01:03Z mellinger $
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
#ifndef PYFF_FILTER_H
#define PYFF_FILTER_H

#include "GenericFilter.h"
#include "PrecisionTime.h"
#include "BCIException.h"

#ifndef DYNAMIC_PYTHON
# if _WIN32
#  define DYNAMIC_PYTHON 1
# else
#  define DYNAMIC_PYTHON 0
# endif // _WIN32
#endif
#if DYNAMIC_PYTHON
# include "PythonWrapper.h"
#else
# include "Python.h"
#endif

#ifndef FILTER_PREFIX
# define FILTER_PREFIX "Pyff"
#endif
#ifndef FILTER_NAME
# define FILTER_NAME PyffFilter
#endif
#ifndef PYTHON_MODULE
# define PYTHON_MODULE "bci2000pyff"
#endif
#ifndef PYTHON_CLASS
# define PYTHON_CLASS "Bci2000PyffAdapter"
#endif

class EndUserError : public bci::Exception {
  public:
    EndUserError(const char* s);
};

typedef                            long   StateType;
typedef std::map<std::string, StateType>  StateMap;
class FILTER_NAME : public GenericFilter
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

  private:
    PyObject*      mpInstance;
    bool           mUseConsole;

    mutable bool           mStayOpen;
    mutable PyThreadState* mpThreadState;

  protected:
    void        SendParametersToPython() const;
    void        ReceiveParametersFromPython();
    void        SendStatesToPython() const;
    StateMap    ReceiveStatesFromPython() const;
    void        UpdateStateChangesFromPython(StateMap& before, StateMap& after) const;
    void        SendStatePrecisionsToPython() const;

    PyObject*   ConvertSignalToPyObject(const GenericSignal& inSignal) const;
    void        ConvertPyObjectToSignal(PyObject* pyOutSignal, GenericSignal& outSignal) const;

    PyObject*   ConvertPropertiesToPyObject(const SignalProperties& inSignalProperties) const;
    void        ConvertPyObjectToProperties(PyObject* pyOutSignalProperties, SignalProperties& outSignalProperties) const;

    PyObject*   ConvertLabelIndexToPyList(LabelIndex from) const;
    void        ConvertPyListToLabelIndex(PyObject*, LabelIndex& to) const;

    PyObject*   ConvertPhysicalUnitToPyDict(PhysicalUnit from) const;
    void        ConvertPyDictToPhysicalUnit(PyObject* from, PhysicalUnit& to) const;

    void        DoubleErr(const char *msg, const char *qualifier=NULL, bool notify_restart=false) const;
    void        HandleEndUserError(const EndUserError& e, std::string qualifier) const;
    void        HandleException(const std::exception& e, std::string qualifier) const;
    void        ChangeDir(const std::string& d);
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

#endif // PYFF_FILTER_H

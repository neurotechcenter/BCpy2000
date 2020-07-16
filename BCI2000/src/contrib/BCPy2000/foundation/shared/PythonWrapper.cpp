//   $Id: PythonWrapper.cpp 5897 2019-04-05 15:13:41Z madamek $
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
// Description: Allows Python functions to be loaded dynamically from a DLL at
// run-time (no linking against import libraries). Each of the *macros* that
// we need from the Python and NumPy APIs is wrapped into multiple functions,
// according to the different supported Python versions,  and only the versions
// corresponding to the loaded DLL are made available. For the subset of the
// Python and NumPy APIs that we need, only very minimal changes were needed to
// the client code, and the client code can still be compiled without the wrapper.
#ifndef DYNAMIC_PYTHON
#define DYNAMIC_PYTHON 1
#endif // DYNAMIC_PYTHON

#if DYNAMIC_PYTHON

////////////////////////////////////////////////////////////////
#ifndef PYTHON_API_NAMESPACE  //////// Define and load functions
////////////////////////////////////////////////////////////////


#include "BCIStream.h"
#ifdef _WIN32
#include <windows.h>
#else
#include <iostream>
#include <dlfcn.h>
#define HINSTANCE            void*
#define LoadLibrary(a)       dlopen(a, RTLD_NOW | RTLD_GLOBAL)
#define GetProcAddress(a,b)  dlsym((a),(b))
#endif

#define  PYTHON_LINK_HEADER_MODE   2 // normal header stuff, plus actual
#include "PythonWrapper.h"           // instantiation of function pointers


#include <vector>
#include <string>

int LoadPythonLinks(const char *dllname)
{
	std::vector<std::string> trynames;
#if PYVERSION== 2
	trynames.push_back("python25");
	trynames.push_back("python27");
	trynames.push_back("python26");
#else
	trynames.push_back("python37");
	trynames.push_back("python36");
#endif
	HINSTANCE dll = NULL;
	if(dllname && *dllname) {
		dll = ::LoadLibrary(dllname);
		if(!dll) bcierr << "failed to load dynamic library \"" << dllname << "\"" << std::endl;
	}
	else {
		for(std::vector<std::string>::iterator i = trynames.begin(); i != trynames.end(); i++) {
			dllname = (*i).c_str();
			dll = ::LoadLibrary(dllname);
			if(dll) break;
		}
		if(!dll) bcierr << "Failed to find a dynamic library named python25, python26 or python27. Is python installed?" << std::endl;
	}
	bool all_loaded = (dll != 0);
#define PYTHON_LINK_HEADER_MODE 3 
#include "PythonWrapper.h"
#undef PYTHON_LINK_HEADER_MODE
	return !all_loaded;
}


#if PYVERSION== 2
// PyList_Check() and friends are not, as they appear, functions, but macros.
// Some macros like Py_DECREF have dynamically-loadable twins (Py_DecRef),
// but *these* particular macros do not appear to have alternatives, and depend
// on static data objects, so we can't use them, even wrapped, with a dynamic
// loading strategy at all. We'll have to settle for these clumsy alternatives:
int PyList_Check(PyObject* a)
{
	static PyObject* py_class = NULL;
	if(py_class == NULL)
	{
		PyObject* py_template = PyList_New(0);
		py_class = PyObject_GetAttrString(py_template, "__class__");
		Py_DecRef(py_template);
		// probably there's single API call for getting class too, but the API
		// doc is impossible to navigate unless you already know the name of
		// what you're looking for...
	}
	return PyObject_IsInstance(a, py_class);
}


int PyString_Check(PyObject* a)
{
	static PyObject* py_class = NULL;
	if(py_class == NULL)
	{
		PyObject* py_template = PyString_FromString("");
		py_class = PyObject_GetAttrString(py_template, "__class__");
		Py_DecRef(py_template);
	}
	return PyObject_IsInstance(a, py_class);
}
#else //python 3, implementation from python header
int PyString_Check(PyObject* a)
{
	return ((PyType_GetFlags(a) & Py_TPFLAGS_UNICODE_SUBCLASS) != 0);

}

int PyBytes_Check(PyObject* a)
{
	static PyObject* py_class = NULL;
	if(py_class == NULL)
	{
		PyObject* py_template = PyBytes_FromString("");
		py_class = PyObject_GetAttrString(py_template, "__class__");
		Py_DecRef(py_template);
	}
	return PyObject_IsInstance(a, py_class);
	//return ((PyType_GetFlags(a) & Py_TPFLAGS_BYTES_SUBCLASS) != 0);

}

int PyList_Check(PyObject* a)
{
	return ((PyType_GetFlags(a) & Py_TPFLAGS_LIST_SUBCLASS) != 0);
}

//Py_ssize_t PyString_Size(PyObject* o)
//{
//	return ((PyASCIIObject *)(o))->length;
//}
#endif
////////////////////////////////////////////////////////////////
#else // (i.e. if PYTHON_API_NAMESPACE is #defined) ////////////
//////////////// wrap (possibly-version-dependent) macros //////

// This second half of the file is the only place where we
// assume that version-dependent Python headers have been included.

#define PyObject PyObject
#define PyThreadState PyThreadState
#define PYTHON_LINK_HEADER_MODE 0 //
#include "PythonWrapper.h"
namespace PYTHON_API_NAMESPACE { ///////////////////////////////

// Maybe we don't need this mechanism at all any more, now that
// the numpy dependency has been removed and macro-wrapping avoided...

////////////////////////////////////////////////////////////////
} // end of namespace PYTHON_API_NAMESPACE /////////////////////
#endif // (whether PYTHON_API_NAMESPACE defined) ///////////////
////////////////////////////////////////////////////////////////

#endif // DYNAMIC_PYTHON

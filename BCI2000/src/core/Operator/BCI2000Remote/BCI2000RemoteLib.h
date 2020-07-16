/*********************************************************************
 * $Id: BCI2000RemoteLib.h 4536 2013-08-05 14:30:13Z mellinger $
 * Author: juergen.mellinger@uni-tuebingen.de
 * Description: C-style interface to the BCI2000Remote class.
 *   The BCI2000Remote class allows to remotely connect to, and control,
 *   the BCI2000 Operator module.
 *
 * $BEGIN_BCI2000_LICENSE$
 *
 * This file is part of BCI2000, a platform for real-time bio-signal research.
 * [ Copyright (C) 2000-2012: BCI2000 team and many external contributors ]
 *
 * BCI2000 is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * BCI2000 is distributed in the hope that it will be useful, but
 *                         WITHOUT ANY WARRANTY
 * - without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * $END_BCI2000_LICENSE$
 *********************************************************************/
#ifndef BCI2000_REMOTE_LIB_H
#define BCI2000_REMOTE_LIB_H

#ifdef _WIN32
# define STDCALL __stdcall
#  ifdef LIBREMOTE_LIBRARY
#   define DLLEXPORT __declspec( dllexport )
#  else
#   define DLLEXPORT
# endif /* LIBREMOTE_LIBRARY */
#elif __GNUC__ && __GNUC__ >= 4
# define STDCALL
# define DLLEXPORT __attribute__ ((visibility ("default")))
#else
# define STDCALL
# define DLLEXPORT
#endif /* _WIN32 */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*
function:  BCI2000Release
purpose:   Releases a string buffer, or other object allocated by the library.
arguments: String pointer, or handle to object allocated by one of the library functions.
returns:   1 if successful, 0 otherwise.
*/
DLLEXPORT int
STDCALL BCI2000Release( void* );

/*
function:  BCI2000Remote_New
purpose:   Creates a new BCI2000Remote object.
arguments: None.
returns:   A handle to the newly created object.
*/
DLLEXPORT void*
STDCALL BCI2000Remote_New( void );

/*
function:  BCI2000Remote_Delete
purpose:   Deletes a BCI2000Remote object.
arguments: Handle to an existing BCI2000Remote object.
returns:   1 if successful, 0 otherwise.
*/
DLLEXPORT int
STDCALL BCI2000Remote_Delete( void* );

/* The following functions are property accessors, and member functions of BCI2000Remote
   objects. For documentation, see "Programming Reference:BCI2000Remote Class" on
   the BCI2000 wiki.
   In this C-style interface, each function takes a handle to an existing BCI2000Remote
   object as its first argument. Use BCI2000Remote_New() to create such objects.
   When a function returns a C string, the caller is responsible for releasing that string
   once it is done with it. This is done by calling BCI2000Release() on the string.
   Function that return a C string may return NULL to indicate failure.
*/
DLLEXPORT double
STDCALL BCI2000Remote_GetTimeout( void* );
DLLEXPORT void
STDCALL BCI2000Remote_SetTimeout( void*, double timeout );

DLLEXPORT const char*
STDCALL BCI2000Remote_GetOperatorPath( void* );
DLLEXPORT void
STDCALL BCI2000Remote_SetOperatorPath( void*, const char* );

enum
{
  BCI2000Remote_Invisible = 0,
  BCI2000Remote_Visible = 1,
  BCI2000Remote_NoChange = 2,
};
DLLEXPORT int
STDCALL BCI2000Remote_GetWindowVisible( void* );
DLLEXPORT void
STDCALL BCI2000Remote_SetWindowVisible( void*, int visible );

DLLEXPORT const char*
STDCALL BCI2000Remote_GetWindowTitle( void* );
DLLEXPORT void
STDCALL BCI2000Remote_SetWindowTitle( void*, const char* );

DLLEXPORT const char*
STDCALL BCI2000Remote_GetTelnetAddress( void* );
DLLEXPORT void
STDCALL BCI2000Remote_SetTelnetAddress( void*, const char* );

DLLEXPORT int
STDCALL BCI2000Remote_GetConnected( void* );

DLLEXPORT const char*
STDCALL BCI2000Remote_GetResult( void* );

DLLEXPORT const char*
STDCALL BCI2000Remote_GetSubjectID( void* );
DLLEXPORT void
STDCALL BCI2000Remote_SetSubjectID( void*, const char* );

DLLEXPORT const char*
STDCALL BCI2000Remote_GetSessionID( void* );
DLLEXPORT void
STDCALL BCI2000Remote_SetSessionID( void*, const char* );

DLLEXPORT const char*
STDCALL BCI2000Remote_GetDataDirectory( void* );
DLLEXPORT void
STDCALL BCI2000Remote_SetDataDirectory( void*, const char* );

DLLEXPORT int
STDCALL BCI2000Remote_Connect( void* );
DLLEXPORT int
STDCALL BCI2000Remote_Disconnect( void* );

DLLEXPORT const char*
STDCALL BCI2000Remote_Encode( void*, const char* );
/* In Execute(), the last argument may be a null pointer. */
DLLEXPORT int
STDCALL BCI2000Remote_Execute( void*, const char*, int* );

/* Provide a zero-delimited list of C strings, containing module names
   with command line arguments in the form
   const char* list = "module1 -arg1 -arg2\0module2 -arg1\0module3 -arg1\0\0";
*/
DLLEXPORT int
STDCALL BCI2000Remote_StartupModules( void*, const char* );
DLLEXPORT int
STDCALL BCI2000Remote_LoadParametersLocal( void*, const char* );
DLLEXPORT int
STDCALL BCI2000Remote_LoadParametersRemote( void*, const char* );

DLLEXPORT int
STDCALL BCI2000Remote_SetConfig( void* );
DLLEXPORT int
STDCALL BCI2000Remote_Start( void* );
DLLEXPORT int
STDCALL BCI2000Remote_Stop( void* );
DLLEXPORT int
STDCALL BCI2000Remote_Quit( void* );

DLLEXPORT const char* 
STDCALL BCI2000Remote_GetSystemState( void* );
DLLEXPORT int
STDCALL BCI2000Remote_GetControlSignal( void*, int, int, double* );
DLLEXPORT int
STDCALL BCI2000Remote_SetParameter( void*, const char* name, const char* value );
DLLEXPORT const char*
STDCALL BCI2000Remote_GetParameter( void*, const char* name );
DLLEXPORT int
STDCALL BCI2000Remote_AddStateVariable( void*, const char* name, unsigned int bitWidth, double initialValue );
DLLEXPORT int
STDCALL BCI2000Remote_SetStateVariable( void*, const char*, double );
DLLEXPORT int
STDCALL BCI2000Remote_GetStateVariable( void*, const char*, double* );

DLLEXPORT int
STDCALL BCI2000Remote_SetScript( void*, const char*, const char* );
DLLEXPORT const char*
STDCALL BCI2000Remote_GetScript( const char* );

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // BCI2000_REMOTE_LIB_H

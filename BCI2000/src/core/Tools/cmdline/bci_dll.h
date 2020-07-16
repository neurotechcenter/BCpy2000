/*********************************************************************/
/* $Id: bci_dll.h 3798 2012-02-01 18:07:06Z mellinger $
/* Author:      juergen.mellinger@uni-tuebingen.de
/* Description: Provides a framework for dlls that contain BCI2000
/*              filters.
/*
/*              NOTE: If you read state vector information from a file,
/*              you must call PutState() for each state before calling
/*              Instantiate(). This will result in a binary state
/*              vector layout matching the one from the file.
/*              Without previous calls to PutState(), Instantiate()
/*              will create a new state vector using the states
/*              requested by the filter(s) present in the DLL.
/*
/* $BEGIN_BCI2000_LICENSE$
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
/*********************************************************************/
#ifndef BCI_DLL_H
#define BCI_DLL_H

#ifdef BCI_DLL
# define DLLEXPORT __stdcall __declspec( dllexport )
#else
# define DLLEXPORT __stdcall
#endif /* BCI_DLL */

#ifdef __cplusplus
extern "C" {
#pragma }
#endif /* __cplusplus */

/*
function:  GetInfo
purpose:   Reports filter name and compilation information.
arguments: None.
returns:   Pointer to a null-terminated string holding the information requested.
           The output buffer is allocated inside the DLL, and not meant to be
           deallocated by the caller.
*/
char* DLLEXPORT
GetInfo( void );

/*
function:  GetError
purpose:   Retrieve error output from the previously called function.
arguments: None.
returns:   Pointer to a null-terminated string containing error output.
           The output buffer is allocated inside the DLL, and not meant to be
           deallocated by the caller.
*/
char* DLLEXPORT
GetError( void );

/*
function:  PutParameter
purpose:   Parses a BCI2000 parameter definition line, and adds the resulting
           parameter object to the filter's parameter list, or changes the value of
           a parameter if it exists.
arguments: Pointer to a NULL terminated parameter line string.
returns:   True (1) if no error occurred.
*/
int DLLEXPORT
PutParameter( char* parameterLine );

/*
function:  GetParameter
purpose:   Returns the parameter with the given index from the DLL's internal
           parameter list.
arguments: Parameter index.
returns:   Pointer to a null-terminated string containing a parameter line, or NULL.
           The output buffer is allocated inside the DLL, and not meant to be
           deallocated by the caller.
*/
char* DLLEXPORT
GetParameter( long index );

/*
function:  PutState
purpose:   Adds a state to the filter's state list, and initializes its value
           with 0.
           Instead of a state name, a full BCI2000 state definition line may be given.
           In this case, the line is parsed, and the resulting state object is added
           to the filter's state list.
arguments: Pointer to a NULL terminated state line string.
returns:   True (1) if no error occurred.
*/
int DLLEXPORT
PutState( char* stateName );

/*
function:  SetStateValue
purpose:   Sets the value of a state to a given value.
arguments: Pointer to a NULL terminated state name string; new state value.
returns:   True (1) if no error occurred.
*/
int DLLEXPORT
SetStateValue( char* stateName, long value );

/*
function:  GetStateValue
purpose:   Gets the value of a state.
arguments: Pointer to a NULL terminated state name string; pointer to state value.
returns:   True (1) if no error occurred.
*/
int DLLEXPORT
GetStateValue( char* stateName, long* valuePtr );

/*
function:  GetStatevectorLength
purpose:   Gets the DLL's state vector length.
arguments: Pointer reference to receive the state vector length.
returns:   True (1) if no error occurred.
*/
int DLLEXPORT
GetStatevectorLength( long* statevectorLengthPtr );

/*
function:  SetStatevector
purpose:   Sets the DLL's state vector to the binary values contained in a state vector.
arguments: Pointer to state vector data. The length must match the length returned by
           GetStatevectorLength.
returns:   True (1) if no error occurred.
*/
int DLLEXPORT
SetStatevector( unsigned char* statevectorData );

/*
function:  GetStatevector
purpose:   Gets the binary data contained in the DLL's state vector.
arguments: Pointer and length of state vector data. The length must match the length
           returned by GetStatevectorLength.
returns:   True (1) if no error occurred.
*/
int DLLEXPORT
GetStatevector( unsigned char* statevectorData );

/*
function:  Instantiate
purpose:   Instantiate the filters contained in the dll.
arguments: n/a
returns:   True (1) if no error occurred.
*/
int DLLEXPORT
Instantiate( void );

/*
function:  Dispose
purpose:   Dispose of all filter instances, and clear parameter and state information.
arguments: n/a
returns:   True (1) if no error occurred.
*/
int DLLEXPORT
Dispose( void );

/*
function:  Preflight
purpose:   Calls the filters' Preflight() function.
arguments: Pointers to input signal dimensions and optional signal properties.
returns:   True (1) if no error occurred;
           dimensions pointers contain output signal dimensions on return,
           signal properties string contains output signal properties string on return.
*/
int DLLEXPORT
Preflight( long* ioChannels, long* ioElements, char** inSignalProperties );

/*
function:  Initialize
purpose:   Calls the filters' Initialize() function.
arguments: n/a
returns:   True (1) if no error occurred.
*/
int DLLEXPORT
Initialize( void );

/*
function:  StartRun
purpose:   Calls the filters' StartRun() function.
arguments: n/a
returns:   True (1) if no error occurred.
*/
int DLLEXPORT
StartRun( void );

/*
function:  Process
purpose:   Calls the filters' Process() function.
arguments: Pointer to input array pointer.
           Input and output arrays are expected to have the dimensions specified/obtained
           using the Preflight() function.
returns:   True (1) if no error occurred.
*/
int DLLEXPORT
Process( double* inputSignal, double* outputSignal );

/*
function:  StopRun
purpose:   Calls the filters' StopRun() function.
arguments: n/a
returns:   True (1) if no error occurred.
*/
int DLLEXPORT
StopRun( void );

/*
function:  Resting
purpose:   Calls the filters' Resting() function.
arguments: n/a
returns:   True (1) if no error occurred.
*/
int DLLEXPORT
Resting( void );

/*
function:  Halt
purpose:   Calls the filters' Halt() function.
arguments: n/a
returns:   True (1) if no error occurred.
*/
int DLLEXPORT
Halt( void );

/* Typedefs for function pointers intended to simplify calling GetProcAddress(). */
typedef char* __stdcall ( *GetInfoPtr )( void );
typedef char* __stdcall ( *GetErrorPtr )( void );
typedef int __stdcall ( *PutParameterPtr )( char* );
typedef int __stdcall ( *PutStatePtr )( char* );
typedef int __stdcall ( *SetStateValuePtr )( char*, long );
typedef int __stdcall ( *GetStateValuePtr )( char*, long* );
typedef int __stdcall ( *SetStatevectorPtr )( char*, long );
typedef int __stdcall ( *InstantiatePtr )( void );
typedef int __stdcall ( *DisposePtr )( void );
typedef char* __stdcall ( *PreflightPtr )( long*, long*, char* );
typedef int __stdcall ( *InitializePtr )( void );
typedef int __stdcall ( *StartRunPtr )( void );
typedef int __stdcall ( *ProcessPtr )( double*, double* );
typedef int __stdcall ( *StopRunPtr )( void );
typedef int __stdcall ( *RestingPtr )( void );
typedef int __stdcall ( *HaltPtr )( void );

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* BCI_DLL_H */

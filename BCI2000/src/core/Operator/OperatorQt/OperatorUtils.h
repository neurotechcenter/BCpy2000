////////////////////////////////////////////////////////////////////////////////
// $Id: OperatorUtils.h 5794 2018-08-14 13:39:22Z mellinger $
// Description: A file intended to hold global utility functions common to
//              various operator source files.
//
// $BEGIN_BCI2000_LICENSE$
// 
// This file is part of BCI2000, a platform for real-time bio-signal research.
// [ Copyright (C) 2000-2012: BCI2000 team and many external contributors ]
// 
// BCI2000 is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
// 
// BCI2000 is distributed in the hope that it will be useful, but
//                         WITHOUT ANY WARRANTY
// - without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License along with
// this program.  If not, see <http://www.gnu.org/licenses/>.
// 
// $END_BCI2000_LICENSE$
////////////////////////////////////////////////////////////////////////////////

#ifndef OPERATOR_UTILS_H
#define OPERATOR_UTILS_H

#include <QFileInfo>

class QWidget;
class QString;
class Param;

#define MATRIX_EXTENSION "bmt"

namespace OperatorUtils
{
// **************************************************************************
// Function:   PutPersistentData
// Purpose:    Write binary data to persistent storage.
// Parameters: A name and a data array.
// Returns:    n/a
// **************************************************************************
  void PutPersistentData(const char*, const QByteArray&);

// **************************************************************************
// Function:   GetPersistentData
// Purpose:    Read binary data from persistent storage.
// Parameters: A name and a data array.
// Returns:    n/a
// **************************************************************************
  void GetPersistentData(const char*, QByteArray&);

// **************************************************************************
// Function:   SaveWidget
// Purpose:    Save control properties to persistent storage.
// Parameters: A pointer to a QWidget object.
// Returns:    n/a
// **************************************************************************
  void SaveWidget(const QWidget*, const QByteArray* = nullptr);
  
// **************************************************************************
// Function:   RestoreWidget
// Purpose:    Restore control properties from persistent storage.
// Parameters: A pointer to a QWidget object.
// Returns:    true if size was set from storage, false otherwise
// **************************************************************************
  bool RestoreWidget(QWidget*, QByteArray* = nullptr);

// **************************************************************************
// Function:   AppRelativeToAbsolutePath
// Purpose:    Convert an application-relative into an absolute path.
// Parameters: Application-relative path.
// Returns:    A QFileInfo containing an absolute path.
// **************************************************************************
  QFileInfo AppRelativeToAbsolutePath( const QString& );

// **************************************************************************
// Function:   CurrentDir
// Purpose:    Access the default directory for file open dialogs.
// Parameters: n/a
// Returns:    Directory object.
// **************************************************************************
  QDir& CurrentDir();

  // **************************************************************************
// Function:   UserLevel
// Purpose:    Get the global user level.
// Parameters: n/a
// Returns:    Global user level.
// **************************************************************************
  int UserLevel();

// **************************************************************************
// Function:   GetUserLevel
// Purpose:    Read a parameter's user level from the registry.
// Parameters: A pointer to the parameter's name.
// Returns:    User level.
// **************************************************************************
  int GetUserLevel( const char* Name );

// **************************************************************************
// Function:   SetUserLevel
// Purpose:    Change a parameter's user level in the registry.
// Parameters: A pointer to the parameter's name, and the user level.
// Returns:    n/a
// **************************************************************************
  void SetUserLevel( const char* Name, int UserLevel );

  enum FilterType
  {
    saveFilter = 1,
    loadFilter = 2,
  };
// **************************************************************************
// Function:   GetFilterStatus
// Purpose:    Read a parameter's filter status from the registry.
// Parameters: A pointer to the parameter's name, and the filter type.
// Returns:    n/a
// **************************************************************************
  int GetFilterStatus( const char* Name, int filtertype );
// **************************************************************************
// Function:   SetFilterStatus
// Purpose:    Change a parameter's filter status in the registry.
// Parameters: A pointer to the parameter's name, and the filter type.
// Returns:    n/a
// **************************************************************************
void SetFilterStatus( const char* Name, int filtertype, int );

enum {
  NoError = 0,

  MatLoadColsDiff,
  MatNotFound,
  MatMultipleParams,
  CannotWriteNestedMatrixAsText,
  CouldNotWrite,
};
// **************************************************************************
// Function:   LoadMatrix
// Purpose:    Loads a matrix that is delimited by white spaces
// Parameters: - filename of the matrix file, containing the full path
//             - pointer to the parameter that contains the matrix
// Returns:    ERR_NOERR - no error
//             ERR_MATLOADCOLSDIFF - number of columns in different rows is different
//             ERR_MATNOTFOUND - could not open input matrix file or file contains no data
// **************************************************************************
  int LoadMatrix( const QString& inFileName, Param& outParam );

// **************************************************************************
// Function:   SaveMatrix
// Purpose:    Saves a matrix to a file, delimited by white spaces
// Parameters: - filename of the matrix file, containing the full path
//             - pointer to the parameter that contains the matrix
// Returns:    ERR_NOERR - no error
//             ERR_COULDNOTWRITE - could not write matrix to output file
// **************************************************************************
  int SaveMatrix( const QString& inFileName, const Param& inParam );

};

#endif // OPERATOR_UTILS_H

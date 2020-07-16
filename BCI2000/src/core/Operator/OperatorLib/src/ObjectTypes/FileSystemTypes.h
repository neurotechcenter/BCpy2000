////////////////////////////////////////////////////////////////////////////////
// $Id: FileSystemTypes.h 5553 2016-09-27 18:58:34Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: CommandInterpreter types for files and directories.
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
#ifndef FILE_SYSTEM_TYPES_H
#define FILE_SYSTEM_TYPES_H

#include "ObjectType.h"

namespace Interpreter {

class DirectoryType : public ObjectType
{
 protected:
  virtual const char* Name() const { return "Directory"; }
  virtual const MethodEntry* MethodTable() const { return sMethodTable; }

 public:
  static bool Extract( CommandInterpreter& );
  static bool Parent( CommandInterpreter& );
  static bool Exists( CommandInterpreter& );
  static bool Current( CommandInterpreter& );
  static bool Change( CommandInterpreter& );
  static bool List( CommandInterpreter& );
  static bool Make( CommandInterpreter& );
  static bool Rename( CommandInterpreter& );
  static bool Remove( CommandInterpreter& );
  static bool ForceRemove( CommandInterpreter& );

  static bool ListSelection( CommandInterpreter&, const std::string& dir, const std::string& wildcard, bool (*)( const std::string& ) );
  static std::string ListDirectory( const std::string& );
  
 private:
  static const MethodEntry sMethodTable[];
  static DirectoryType sInstance;
};

class DirectoriesType : public ObjectType
{
 protected:
  virtual const char* Name() const { return "Directories"; }
  virtual const MethodEntry* MethodTable() const { return sMethodTable; }

 public:
  static bool List( CommandInterpreter& );
  
 private:
  static const MethodEntry sMethodTable[];
  static DirectoriesType sInstance;
};

class FileType : public ObjectType
{
 protected:
  virtual const char* Name() const { return "File"; }
  virtual const MethodEntry* MethodTable() const { return sMethodTable; }

 public:
  static bool Extract( CommandInterpreter& );
  static bool Exists( CommandInterpreter& );
  static bool List( CommandInterpreter& );
  static bool Rename( CommandInterpreter& );
  static bool Remove( CommandInterpreter& );

 private:
  static const MethodEntry sMethodTable[];
  static FileType sInstance;
};

class FilesType : public ObjectType
{
 protected:
  virtual const char* Name() const { return "Files"; }
  virtual const MethodEntry* MethodTable() const { return sMethodTable; }

 public:
  static bool List( CommandInterpreter& );
  static bool ListFiles( CommandInterpreter&, const std::string& );

 private:
  static const MethodEntry sMethodTable[];
  static FilesType sInstance;
};

class PathType : public ObjectType
{
 protected:
  const char* Name() const override { return "Path"; }
  const MethodEntry* MethodTable() const override { return sMethodTable; }

 public:
  static bool Canonicalize( CommandInterpreter& );
  static bool Normalize( CommandInterpreter& );
  static bool Real( CommandInterpreter& );
  static bool Exists( CommandInterpreter& );

 private:
  static const MethodEntry sMethodTable[];
  static PathType sInstance;
};

} // namespace

#endif // FILE_SYSTEM_TYPES_H

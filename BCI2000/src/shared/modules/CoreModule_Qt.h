////////////////////////////////////////////////////////////////////////////////
// $Id: CoreModule_Qt.h 5120 2015-11-21 16:11:24Z mellinger $
// Author: griffin.milsap@gmail.com
// Description: A class that implements the CoreModule GUI interface functions
//          for QT-based modules
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
#ifndef CORE_MODULE_QT_H
#define CORE_MODULE_QT_H

#include "CoreModule.h"

class CoreModuleQt : public CoreModule
{
 public:
  CoreModuleQt();
  ~CoreModuleQt();

 private:
  virtual void OnInitialize( int& argc, char** argv );
  virtual void OnProcessGUIMessages();
  virtual bool OnGUIMessagesPending();
  
  int mCount;
};

typedef CoreModuleQt CoreModuleQT;

#endif // CORE_MODULE_QT_H

////////////////////////////////////////////////////////////////////////////////
// $Id: BCI2000PCLLibrary.h 3918 2012-04-04 14:52:38Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: An IPCLLibrary implementation that provides information about
//   BCI2000PresentationLink data types.
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
#ifndef BCI2000_PCL_LIBRARY_H
#define BCI2000_PCL_LIBRARY_H

#include <vector>
#include <string>
#include "ComCoClass.h"
#include "BCI2000PresentationLink_i.h"
#include "PCLExtension.h"

class CBCI2000PCLLibrary
: public com::CoClass<IPCLLibrary, &IID_IPCLLibrary, CBCI2000PCLLibrary, &CLSID_BCI2000PCLLibrary>
{
public:
  static const wchar_t* Name() { return L"BCI2000PresentationLink"; }
  static const wchar_t* Author() { return L"juergen.mellinger@uni-tuebingen.de"; }
  static const wchar_t* Description() { return L"PCL Extension to control BCI2000 from Presentation"; }
  static const wchar_t* Version();
  static const wchar_t* NamespaceName() { return L"bci2000"; }

  CBCI2000PCLLibrary();

public:
  // IPCLLibrary Methods
  STDMETHOD(getName)(BSTR*);
  STDMETHOD(getAuthor)(BSTR*);
  STDMETHOD(getVersion)(BSTR*);
  STDMETHOD(getDescription)(BSTR*);
  STDMETHOD(getPresentationMinimumVersion)(double*);
  STDMETHOD(getNamespaceName)(BSTR*);
  STDMETHOD(getTypeCount)(long*);
  STDMETHOD(getType)(long index, IPCLType**);
  STDMETHOD(getLastError)(BSTR*);

private:
  std::wstring mLastError;
  typedef com::Ptr<IPCLType> (*TypeInstantiator)( IPCLLibrary* );
  std::vector<TypeInstantiator> mTypeInstantiators;
};

#endif // BCI2000_PCL_LIBRARY_H

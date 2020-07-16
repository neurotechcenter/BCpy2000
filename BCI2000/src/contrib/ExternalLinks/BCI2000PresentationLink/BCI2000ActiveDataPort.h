////////////////////////////////////////////////////////////////////////////////
// $Id: BCI2000ActiveDataPort.h 3918 2012-04-04 14:52:38Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: An IActiveDataPort implementation for BCI2000PresentationLink.
//   The ActiveDataPort uses a BCI2000Remote object to communicate with
//   BCI2000.
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
#ifndef BCI2000_ACTIVE_DATA_PORT
#define BCI2000_ACTIVE_DATA_PORT

#include <string>
#include "ComCoClass.h"
#include "BCI2000PresentationLink_i.h"
#include "BCI2000Remote.h"

// CBCI2000ActiveDataPort
class CBCI2000ActiveDataPort
: public com::CoClass<IActiveDataPort, &IID_IActiveDataPort, CBCI2000ActiveDataPort, &CLSID_BCI2000ActiveDataPort>
{
public:
  CBCI2000ActiveDataPort();
  ~CBCI2000ActiveDataPort();

  // IActiveDataPort Methods
public:
  STDMETHOD(getName)(BSTR*);
  STDMETHOD(getServerTime)( double*, double* );
  STDMETHOD(sendData)( BSTR, BSTR, double, double, BSTR );
  STDMETHOD(processMessage)(BSTR, BSTR* );
  STDMETHOD(getLastError)(BSTR*);

  // Interface to other BCI2000PresentationLink classes
  void Initialize( const struct DataPortSettings& );
  BCI2000Remote& BCI2000() { return mBCI2000; }
  static CBCI2000ActiveDataPort* FirstInstance() { return spFirstInstance; }

private:
  std::vector<std::string> mEventTypes;
  bool mUseResumePause;
  BCI2000Remote mBCI2000;
  std::wstring mLastError;

  static CBCI2000ActiveDataPort* spFirstInstance;
};

#endif // BCI2000_ACTIVE_DATA_PORT

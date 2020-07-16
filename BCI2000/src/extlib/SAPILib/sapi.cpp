////////////////////////////////////////////////////////////////////////////////
// $Id: sapi.cpp 3798 2012-02-01 18:07:06Z mellinger $
// Author: Vaishali Kamat @ Cambridge Consultants
// Description: This file has been created to support the use of the
//              Microsoft Speech API in conjunction with the sapi.lib which
//              contains the imported Microsoft sapi.dll
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

#pragma hdrstop
#include <cstdlib>
#include <iostream>
#include <windows.h>
#include <Servprov.h>
#include <sapi.h>

const IID IID_ISpVoice = {0x6C44DF74, 0x72B9, 0x4992, {0xA1, 0xEC, 0xEF,
  0x99, 0x6E, 0x04, 0x22, 0xD4}};

const CLSID CLSID_SpVoice = {0x96749377, 0x3391, 0x11D2, {0x9E, 0xE3,
  0x00, 0xC0, 0x4F, 0x79, 0x73, 0x96}};



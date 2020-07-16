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
 */
//---------------------------------------------------------------------------


#pragma hdrstop

#include "vAmpChannelInfo.h"

///////////////////////////////////////////////////////////
// class CChannelInfo
///////////////////////////////////////////////////////////
CChannelInfo::CChannelInfo()
: nType(DEVICE_CHAN_TYPE_UNKNOWN)
, nPhysIdx(0)
, szLabel("")
, dResolution(1.0)
, bUsrUnit(FALSE)
, szUnitName("")
, fGradient(1.0f)
, fOffset(0.0f)
{
}

const CChannelInfo &
	CChannelInfo::operator = (const CChannelInfo &c2)
{
	nPhysIdx = c2.nPhysIdx;
	szLabel = c2.szLabel;
	bUsrUnit = c2.bUsrUnit;
	szUnitName = c2.szUnitName;
	fGradient = c2.fGradient;
	fOffset = c2.fOffset;
	nType = c2.nType;
	return *this;
}

const bool CChannelInfo::operator == (const CChannelInfo &c2)
{
	return (szLabel == c2.szLabel &&
		nPhysIdx == c2.nPhysIdx) ? TRUE : FALSE;
}
//---------------------------------------------------------------------------

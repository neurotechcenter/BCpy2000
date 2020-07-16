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

#ifndef vAmpChannelInfoH
#define vAmpChannelInfoH


#include <windows.h>
#include <string>
# include <math.h>

#include "FirstAmp.h"

#define DEVICE_DIGITMAX				9	// 9 bit trigger port.
#define DEVICE_MAX_IMP_RANGES		5	// 5 main impedance groups.
#define DEVICE_CHAN_TYPE_UNKNOWN	0	// Unknown channel type.
#define DEVICE_CHAN_TYPE_EEG		1	// EEG channel type.
#define DEVICE_CHAN_TYPE_AUX		2	// Auxiliary channel type.
#define DEVICE_CHAN_TYPE_TRIGGER	3	// Trigger port type.

#define DEVICE_CHANMODE_8			0	// 8 EEG channels system.
#define DEVICE_CHANMODE_16			1	// 16 EEG channels system.
#define DEVICE_CHANMODE_4			2	// 4 EEG channels, joints the FA systems.

class CChannelInfo
	//; Represents a single channel info.
	{
	public :
		char	    nType;			// Channel type.
		int		    nPhysIdx;		// Physical channel index.
		std::string szLabel;		// Label of channel.
		double	    dResolution;	// Resolution [µV] for all analog channels.
		bool	    bUsrUnit;		// User unit used if TRUE otherwise FALSE.
		std::string szUnitName;		// Unit name.
		float	    fGradient;		// Slope of equation for usr unit (e.g. [mV/C]).
		float	    fOffset;		// Offset of equation (constant) in [mV].

		CChannelInfo();
			//; Constructor.
		const CChannelInfo & operator = (const CChannelInfo &);
			//; Assign operator.
		const bool operator == (const CChannelInfo &);
			//; Compare operator.
		float ToUserUnit(float fRawData)
			//: Conversion to the digitized raw data, equation U(T) = m*T + Uoff,
			//+		with m [mV/C], U[mV], Uoff[mV]
			//+	Return
			//+		Digitized sample as result of user equation.
			//+	Parameters
			//.		fRawData - digitized raw data of this channel (direct from driver).
		{
			// Conversion only allowed for AUX.
			if (nType != DEVICE_CHAN_TYPE_AUX || !bUsrUnit) return fRawData;
			if (fabs(fGradient) < 1e-6) fGradient = 1; // default.
			// [µV] => [mV] because of gradient.
			return float(((fRawData * dResolution * 1e-3) - fOffset) / fGradient);
		}
	};
//---------------------------------------------------------------------------
#endif // vAmpChannelInfoH


////////////////////////////////////////////////////////////////////////////////
// $Id: Pulse.h 2018-09-17 abelsten $
// Authors: belsten@neurotechcenter.org
// Description: Class that represents the TBSI USB-SPI wireless stimulator 
//				pulse. Methods build a string pattern that is sent to the 
//				TBSI system
// Known Issues: PI2 and PD2 not implemented 
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
#ifndef PULSE_H
#define PULSE_H

#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <cmath>

// ================================================================
// Pulse Class
//
// Represents a pulse that is to be administered by TBSI stimulator. 
//
// Info on values encoded in default string pattern can be found 
//	at www.bci2000.org/mediawiki/index.php/Contributions:TBSIStimulator
//
// Structure:
//		PI1: inital pulse current in uA
//		PD1: duration of PI1 in us
//		PI2: pulse that follows PI1 in uA
//		PD2: duration of PI2 in us
//	These are the units that should be used exclusively
// ================================================================

class Pulse
{
public:
	// constructors
	Pulse();
	Pulse(const std::string& _pattern);
	Pulse(const std::string& _pattern,
				  /*uA*/ int _PI1,
			      /*us*/ int _PD1,
			      /*uA*/ int _PI2 = 0,
				  /*us*/ int _PD2 = 0);

	// accessor
	const std::string& Pattern() const { return mStringPattern; }

private:
	void SetProperties(int _PI1, int _PD1, int _PI2, int _PD2);
	void SetString(const std::string& _pattern);
	bool SetPI1(int _PI1);
	bool SetPD1(int _PD2);
	bool SetPI2(int _PI2);		
	bool SetPD2(int _PD2);		

	std::string mStringPattern = "555500000200000000000A0000000000000032000000BC003200E7031F03B603000000000000330B5C070100010001000100000000080000000000000100000000000000000064000000000064000000000032000000E7032003FFFF00000000000000000000A4085C070A00010001000000000000080000000000000100AAAA";
};

#endif

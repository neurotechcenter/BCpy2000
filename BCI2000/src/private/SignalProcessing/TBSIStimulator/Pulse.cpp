////////////////////////////////////////////////////////////////////////////////
// $Id: Pulse.cpp 2018-09-17 abelsten $
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
#include "PCHIncludes.h"
#include "Pulse.h"

std::string IntToHex(int _in, int _len);

Pulse::Pulse()
{
	// use the default string
}


Pulse::Pulse(const std::string& _pattern)
{
	SetString(_pattern);
}


// allow for PI2 and PD2 to not be specified as custom values are not yet supported
Pulse::Pulse(const std::string& _pattern, int _PI1, int _PD1, int _PI2, int _PD2)
{
	SetString(_pattern);
	SetProperties(_PI1, _PD1, _PI2, _PD2);
}


void Pulse::SetString(const std::string& _pattern) 
{
	if (_pattern.length() != mStringPattern.length())
		std::cerr << "Error: Provided pattern string is of incorrect length." << std::endl;
	else
		mStringPattern = _pattern;
}


void Pulse::SetProperties(int _PI1, int _PD1, int _PI2, int _PD2)
{
	if (!SetPI1(_PI1))
	{
		std::cerr << "Error: PI1 value not valid" << std::endl;
	}
	if (!SetPD1(_PD1))
	{
		std::cerr << "Error: PD1 value not valid" << std::endl;
	}
	if (_PI2 || _PD2)
		std::cout << "Warning: Functionalilty for setting PI2 and PD2 does not exist yet.\n"
		<< "Pulse will use default values of 100 uA for PI2 and 10ms for PD2." << std::endl;
}


bool Pulse::SetPI1(int _PI1)
{
	if (_PI1 < 1 || _PI1 > 1000)	// PI1 must be within the range of 1 to 1000 uA
	{
		return false;
	}

	// decimalStringValue = 1.638 * _PI1 + 2048.1
	std::string hexPI1 = IntToHex(1.638 * _PI1 + 2048.1, 3);

	// the hex value has 3 digits in string[76], string[77] and string[79].
	// MSB -> string[79], LSB -> string[77]
	mStringPattern[79] = hexPI1[0];
	mStringPattern[76] = hexPI1[1];
	mStringPattern[77] = hexPI1[2];
	return true;
}


bool Pulse::SetPD1(int _PD1)
{
	if (_PD1 < 50 || _PD1 > 65535)	// PI1 must be within the range of 50 to 65535 us
	{
		return false;
	}

	// Two string values are set based on the decimal values thousand magnitudes. i.e. (18,000) -> HEX(18) -> 0x12
	//		MSB of thousands hex value goes to string[8], LSB goes to string[9] 
	int TopInt = floor(_PD1 / 1000);
	std::string TopHex = IntToHex(TopInt, 2);
	mStringPattern[8] = TopHex[0];
	mStringPattern[9] = TopHex[1];

	// Three string values are set based on the decimal values of the lower 3 digits. i.e. (43,123) -> HEX(123) -> 0x07B
	//		MSB of this value goes to string[15], LSB goes to string[13]
	int BottomInt = _PD1 - (TopInt * 1000);
	std::string BottomHex = IntToHex(BottomInt, 3);
	mStringPattern[15] = BottomHex[0];
	mStringPattern[12] = BottomHex[1];
	mStringPattern[13] = BottomHex[2];
	return true;
}


bool Pulse::SetPI2(int _PI2)
{
	// NOT YET IMPLEMENTED
	// requires reverse engineering of StimWare strings
	return false;
}

bool Pulse::SetPD2(int _PD2)
{
	// NOT YET IMPLEMENTED
	// requires reverse engineering of StimWare strings
	return false;
}


std::string IntToHex(int _in, int _len)
{
	std::stringstream stream;
	stream << std::setfill('0') << std::setw(_len)
		   << std::uppercase << std::hex << _in;
	return stream.str();
}
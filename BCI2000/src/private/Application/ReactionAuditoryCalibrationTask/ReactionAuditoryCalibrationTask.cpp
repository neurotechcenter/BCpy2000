////////////////////////////////////////////////////////////////////////////////
// $Id: $
// Authors:
// Description: ReactionAuditoryCalibrationTask implementation
//
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
#pragma hdrstop

#include "ReactionAuditoryCalibrationTask.h"
#include "BCIStream.h"
#include "RandomGenerator.h"
#include "TextStimulus.h"
#include <utility>
#include <math.h>
#include <Windows.h>
#include <cmath>
#include <fstream>


using namespace std;
using namespace GUI;

RegisterFilter( ReactionAuditoryCalibrationTask, 3 );
     // Change the location if appropriate, to determine where your filter gets
     // sorted into the chain. By convention:
     //  - filter locations within SignalSource modules begin with "1."
     //  - filter locations within SignalProcessing modules begin with "2."
     //       (NB: SignalProcessing modules must specify this with a Filter() command in their PipeDefinition.cpp file too)
     //  - filter locations within Application modules begin with "3."


ReactionAuditoryCalibrationTask::ReactionAuditoryCalibrationTask() :
  mrDisplay( Window() )
{
       BEGIN_PARAMETER_DEFINITIONS
      "Application:ReactionAuditoryCalibrationTask matrix PestStimuli= "
      "{ 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64 65 66 67 68 69 70 71 72 73 74 75 76 77 78 79 80 81 82 83 84 85 86 87 88 89 90 91 92 93 94 95 96 97 98 99 100 101 102 103 104 105 106 107 108 109 110 111 112 113 114 115 116 117 118 119 120 121 122 123 124 125 126 127 128 129 130 131 132 133 134 135 136 137 138 139 140 141 142 143 144 145 146 147 148 149 150 151 152 153 154 155 156 157 158 159 160 161 162 163 164 165 166 167 168 169 170 171 172 173 174 175 176 177 178 179 180 181 182 183 184 185 186 187 188 189 190 191 192 193 194 195 196 197 198 199 200 201} " // row labels
	  "{ frequency attenuation filename} " // column labels
      "1000 -20   ..\\tasks\\ladan_auditory_Pest\\20.00_1000Hz.wav  "
      "1000 -20.5 ..\\tasks\\ladan_auditory_Pest\\20.50_1000Hz.wav  "
      "1000 -21   ..\\tasks\\ladan_auditory_Pest\\21.00_1000Hz.wav  "
      "1000 -21.5 ..\\tasks\\ladan_auditory_Pest\\21.50_1000Hz.wav  "
      "1000 -22   ..\\tasks\\ladan_auditory_Pest\\22.00_1000Hz.wav  "
      "1000 -22.5 ..\\tasks\\ladan_auditory_Pest\\22.50_1000Hz.wav  "
      "1000 -23   ..\\tasks\\ladan_auditory_Pest\\23.00_1000Hz.wav  "
      "1000 -23.5 ..\\tasks\\ladan_auditory_Pest\\23.50_1000Hz.wav	"
      "1000 -24   ..\\tasks\\ladan_auditory_Pest\\24.00_1000Hz.wav	"
      "1000 -24.5 ..\\tasks\\ladan_auditory_Pest\\24.50_1000Hz.wav	"
      "1000 -25   ..\\tasks\\ladan_auditory_Pest\\25.00_1000Hz.wav	"
      "1000 -25.5 ..\\tasks\\ladan_auditory_Pest\\25.50_1000Hz.wav	"
      "1000 -26   ..\\tasks\\ladan_auditory_Pest\\26.00_1000Hz.wav	"
      "1000 -26.5 ..\\tasks\\ladan_auditory_Pest\\26.50_1000Hz.wav	"
      "1000 -27   ..\\tasks\\ladan_auditory_Pest\\27.00_1000Hz.wav	"
      "1000 -27.5 ..\\tasks\\ladan_auditory_Pest\\27.50_1000Hz.wav	"
      "1000 -28   ..\\tasks\\ladan_auditory_Pest\\28.00_1000Hz.wav	"
      "1000 -28.5 ..\\tasks\\ladan_auditory_Pest\\28.50_1000Hz.wav	"
      "1000 -29   ..\\tasks\\ladan_auditory_Pest\\29.00_1000Hz.wav	"
      "1000 -29.5 ..\\tasks\\ladan_auditory_Pest\\29.50_1000Hz.wav	"
      "1000 -30   ..\\tasks\\ladan_auditory_Pest\\30.00_1000Hz.wav	"
      "1000 -30.5 ..\\tasks\\ladan_auditory_Pest\\30.50_1000Hz.wav	"
      "1000 -31   ..\\tasks\\ladan_auditory_Pest\\31.00_1000Hz.wav	"
      "1000 -31.5 ..\\tasks\\ladan_auditory_Pest\\31.50_1000Hz.wav	"
      "1000 -32   ..\\tasks\\ladan_auditory_Pest\\32.00_1000Hz.wav	"
      "1000 -32.5 ..\\tasks\\ladan_auditory_Pest\\32.50_1000Hz.wav	"
      "1000 -33   ..\\tasks\\ladan_auditory_Pest\\33.00_1000Hz.wav	"
      "1000 -33.5 ..\\tasks\\ladan_auditory_Pest\\33.50_1000Hz.wav	"
      "1000 -34   ..\\tasks\\ladan_auditory_Pest\\34.00_1000Hz.wav	"
      "1000 -34.5 ..\\tasks\\ladan_auditory_Pest\\34.50_1000Hz.wav	"
      "1000 -35   ..\\tasks\\ladan_auditory_Pest\\35.00_1000Hz.wav	"
      "1000 -35.5 ..\\tasks\\ladan_auditory_Pest\\35.50_1000Hz.wav	"
      "1000 -36   ..\\tasks\\ladan_auditory_Pest\\36.00_1000Hz.wav	"
      "1000 -36.5 ..\\tasks\\ladan_auditory_Pest\\36.50_1000Hz.wav	"
      "1000 -37   ..\\tasks\\ladan_auditory_Pest\\37.00_1000Hz.wav	"
      "1000 -37.5 ..\\tasks\\ladan_auditory_Pest\\37.50_1000Hz.wav	"
      "1000 -38   ..\\tasks\\ladan_auditory_Pest\\38.00_1000Hz.wav	"
      "1000 -38.5 ..\\tasks\\ladan_auditory_Pest\\38.50_1000Hz.wav	"
      "1000 -39   ..\\tasks\\ladan_auditory_Pest\\39.00_1000Hz.wav	"
      "1000 -39.5 ..\\tasks\\ladan_auditory_Pest\\39.50_1000Hz.wav	"
      "1000 -40   ..\\tasks\\ladan_auditory_Pest\\40.00_1000Hz.wav  "
      "1000 -40.5 ..\\tasks\\ladan_auditory_Pest\\40.50_1000Hz.wav  "
      "1000 -41   ..\\tasks\\ladan_auditory_Pest\\41.00_1000Hz.wav  "
      "1000 -41.5 ..\\tasks\\ladan_auditory_Pest\\41.50_1000Hz.wav  "
      "1000 -42   ..\\tasks\\ladan_auditory_Pest\\42.00_1000Hz.wav  "
      "1000 -42.5 ..\\tasks\\ladan_auditory_Pest\\42.50_1000Hz.wav  "
      "1000 -43   ..\\tasks\\ladan_auditory_Pest\\43.00_1000Hz.wav  "
      "1000 -43.5 ..\\tasks\\ladan_auditory_Pest\\43.50_1000Hz.wav	"
      "1000 -44   ..\\tasks\\ladan_auditory_Pest\\44.00_1000Hz.wav	"
      "1000 -44.5 ..\\tasks\\ladan_auditory_Pest\\44.50_1000Hz.wav	"
      "1000 -45   ..\\tasks\\ladan_auditory_Pest\\45.00_1000Hz.wav	"
      "1000 -45.5 ..\\tasks\\ladan_auditory_Pest\\45.50_1000Hz.wav	"
      "1000 -46   ..\\tasks\\ladan_auditory_Pest\\46.00_1000Hz.wav	"
      "1000 -46.5 ..\\tasks\\ladan_auditory_Pest\\46.50_1000Hz.wav	"
      "1000 -47   ..\\tasks\\ladan_auditory_Pest\\47.00_1000Hz.wav	"
      "1000 -47.5 ..\\tasks\\ladan_auditory_Pest\\47.50_1000Hz.wav	"
      "1000 -48   ..\\tasks\\ladan_auditory_Pest\\48.00_1000Hz.wav	"
      "1000 -48.5 ..\\tasks\\ladan_auditory_Pest\\48.50_1000Hz.wav	"
      "1000 -49   ..\\tasks\\ladan_auditory_Pest\\49.00_1000Hz.wav	"
      "1000 -49.5 ..\\tasks\\ladan_auditory_Pest\\49.50_1000Hz.wav	"
      "1000 -50   ..\\tasks\\ladan_auditory_Pest\\50.00_1000Hz.wav	"
      "1000 -50.5 ..\\tasks\\ladan_auditory_Pest\\50.50_1000Hz.wav	"
      "1000 -51   ..\\tasks\\ladan_auditory_Pest\\51.00_1000Hz.wav	"
      "1000 -51.5 ..\\tasks\\ladan_auditory_Pest\\51.50_1000Hz.wav	"
      "1000 -52   ..\\tasks\\ladan_auditory_Pest\\52.00_1000Hz.wav	"
      "1000 -52.5 ..\\tasks\\ladan_auditory_Pest\\52.50_1000Hz.wav	"
      "1000 -53   ..\\tasks\\ladan_auditory_Pest\\53.00_1000Hz.wav	"
      "1000 -53.5 ..\\tasks\\ladan_auditory_Pest\\53.50_1000Hz.wav	"
      "1000 -54   ..\\tasks\\ladan_auditory_Pest\\54.00_1000Hz.wav	"
      "1000 -54.5 ..\\tasks\\ladan_auditory_Pest\\54.50_1000Hz.wav	"
      "1000 -55   ..\\tasks\\ladan_auditory_Pest\\55.00_1000Hz.wav	"
      "1000 -55.5 ..\\tasks\\ladan_auditory_Pest\\55.50_1000Hz.wav	"
      "1000 -56   ..\\tasks\\ladan_auditory_Pest\\56.00_1000Hz.wav	"
      "1000 -56.5 ..\\tasks\\ladan_auditory_Pest\\56.50_1000Hz.wav	"
      "1000 -57   ..\\tasks\\ladan_auditory_Pest\\57.00_1000Hz.wav	"
      "1000 -57.5 ..\\tasks\\ladan_auditory_Pest\\57.50_1000Hz.wav	"
      "1000 -58   ..\\tasks\\ladan_auditory_Pest\\58.00_1000Hz.wav	"
      "1000 -58.5 ..\\tasks\\ladan_auditory_Pest\\58.50_1000Hz.wav	"
      "1000 -59   ..\\tasks\\ladan_auditory_Pest\\59.00_1000Hz.wav	"
      "1000 -59.5 ..\\tasks\\ladan_auditory_Pest\\59.50_1000Hz.wav	"
      "1000 -60   ..\\tasks\\ladan_auditory_Pest\\60.00_1000Hz.wav	"
      "1000 -60.5 ..\\tasks\\ladan_auditory_Pest\\60.50_1000Hz.wav	"
      "1000 -61   ..\\tasks\\ladan_auditory_Pest\\61.00_1000Hz.wav	"
      "1000 -61.5 ..\\tasks\\ladan_auditory_Pest\\61.50_1000Hz.wav	"
      "1000 -62   ..\\tasks\\ladan_auditory_Pest\\62.00_1000Hz.wav	"
      "1000 -62.5 ..\\tasks\\ladan_auditory_Pest\\62.50_1000Hz.wav	"
      "1000 -63   ..\\tasks\\ladan_auditory_Pest\\63.00_1000Hz.wav	"
      "1000 -63.5 ..\\tasks\\ladan_auditory_Pest\\63.50_1000Hz.wav	"
      "1000 -64   ..\\tasks\\ladan_auditory_Pest\\64.00_1000Hz.wav	"
      "1000 -64.5 ..\\tasks\\ladan_auditory_Pest\\64.50_1000Hz.wav	"
      "1000 -65   ..\\tasks\\ladan_auditory_Pest\\65.00_1000Hz.wav	"
      "1000 -65.5 ..\\tasks\\ladan_auditory_Pest\\65.50_1000Hz.wav	"
      "1000 -66   ..\\tasks\\ladan_auditory_Pest\\66.00_1000Hz.wav	"
      "1000 -66.5 ..\\tasks\\ladan_auditory_Pest\\66.50_1000Hz.wav	"
      "1000 -67   ..\\tasks\\ladan_auditory_Pest\\67.00_1000Hz.wav	"
      "1000 -67.5 ..\\tasks\\ladan_auditory_Pest\\67.50_1000Hz.wav	"   
      "1000 -68   ..\\tasks\\ladan_auditory_Pest\\68.00_1000Hz.wav	"
      "1000 -68.5 ..\\tasks\\ladan_auditory_Pest\\68.50_1000Hz.wav	"
      "1000 -69   ..\\tasks\\ladan_auditory_Pest\\69.00_1000Hz.wav	"
      "1000 -69.5 ..\\tasks\\ladan_auditory_Pest\\69.50_1000Hz.wav	"
      "1000 -70   ..\\tasks\\ladan_auditory_Pest\\70.00_1000Hz.wav	"
      "1000 -70.5 ..\\tasks\\ladan_auditory_Pest\\70.50_1000Hz.wav	"
      "1000 -71   ..\\tasks\\ladan_auditory_Pest\\71.00_1000Hz.wav	"
      "1000 -71.5 ..\\tasks\\ladan_auditory_Pest\\71.50_1000Hz.wav	"
      "1000 -72   ..\\tasks\\ladan_auditory_Pest\\72.00_1000Hz.wav	"
      "1000 -72.5 ..\\tasks\\ladan_auditory_Pest\\72.50_1000Hz.wav	"
      "1000 -73   ..\\tasks\\ladan_auditory_Pest\\73.00_1000Hz.wav	"
      "1000 -73.5 ..\\tasks\\ladan_auditory_Pest\\73.50_1000Hz.wav	"
      "1000 -74   ..\\tasks\\ladan_auditory_Pest\\74.00_1000Hz.wav	"
      "1000 -74.5 ..\\tasks\\ladan_auditory_Pest\\74.50_1000Hz.wav	"
      "1000 -75   ..\\tasks\\ladan_auditory_Pest\\75.00_1000Hz.wav	"
      "1000 -75.5 ..\\tasks\\ladan_auditory_Pest\\75.50_1000Hz.wav	"
      "1000 -76   ..\\tasks\\ladan_auditory_Pest\\76.00_1000Hz.wav	"
      "1000 -76.5 ..\\tasks\\ladan_auditory_Pest\\76.50_1000Hz.wav	"
      "1000 -77   ..\\tasks\\ladan_auditory_Pest\\77.00_1000Hz.wav	"
      "1000 -77.5 ..\\tasks\\ladan_auditory_Pest\\77.50_1000Hz.wav	"
      "1000 -78   ..\\tasks\\ladan_auditory_Pest\\78.00_1000Hz.wav	"
      "1000 -78.5 ..\\tasks\\ladan_auditory_Pest\\78.50_1000Hz.wav	"
      "1000 -79   ..\\tasks\\ladan_auditory_Pest\\79.00_1000Hz.wav	"
      "1000 -79.5 ..\\tasks\\ladan_auditory_Pest\\79.50_1000Hz.wav	"
      "1000 -80   ..\\tasks\\ladan_auditory_Pest\\80.00_1000Hz.wav	"
      "1000 -80.5 ..\\tasks\\ladan_auditory_Pest\\80.50_1000Hz.wav	"
      "1000 -81   ..\\tasks\\ladan_auditory_Pest\\81.00_1000Hz.wav	"
      "1000 -81.5 ..\\tasks\\ladan_auditory_Pest\\81.50_1000Hz.wav	"
      "1000 -82   ..\\tasks\\ladan_auditory_Pest\\82.00_1000Hz.wav	"
      "1000 -82.5 ..\\tasks\\ladan_auditory_Pest\\82.50_1000Hz.wav	"
      "1000 -83   ..\\tasks\\ladan_auditory_Pest\\83.00_1000Hz.wav	"
      "1000 -83.5 ..\\tasks\\ladan_auditory_Pest\\83.50_1000Hz.wav	"
      "1000 -84   ..\\tasks\\ladan_auditory_Pest\\84.00_1000Hz.wav	"
      "1000 -84.5 ..\\tasks\\ladan_auditory_Pest\\84.50_1000Hz.wav	"
      "1000 -85   ..\\tasks\\ladan_auditory_Pest\\85.00_1000Hz.wav	"
      "1000 -85.5 ..\\tasks\\ladan_auditory_Pest\\85.50_1000Hz.wav	"
      "1000 -86   ..\\tasks\\ladan_auditory_Pest\\86.00_1000Hz.wav	"   
      "1000 -86.5 ..\\tasks\\ladan_auditory_Pest\\86.50_1000Hz.wav	"
      "1000 -87   ..\\tasks\\ladan_auditory_Pest\\87.00_1000Hz.wav	"
      "1000 -87.5 ..\\tasks\\ladan_auditory_Pest\\87.50_1000Hz.wav	"
      "1000 -88   ..\\tasks\\ladan_auditory_Pest\\88.00_1000Hz.wav	"
      "1000 -88.5 ..\\tasks\\ladan_auditory_Pest\\88.50_1000Hz.wav	"
      "1000 -89   ..\\tasks\\ladan_auditory_Pest\\89.00_1000Hz.wav	"
      "1000 -89.5 ..\\tasks\\ladan_auditory_Pest\\89.50_1000Hz.wav	"
      "1000 -90   ..\\tasks\\ladan_auditory_Pest\\90.00_1000Hz.wav	"
      "1000 -90.5 ..\\tasks\\ladan_auditory_Pest\\90.50_1000Hz.wav	"
      "1000 -91   ..\\tasks\\ladan_auditory_Pest\\91.00_1000Hz.wav	"
      "1000 -91.5 ..\\tasks\\ladan_auditory_Pest\\91.50_1000Hz.wav	"
      "1000 -92   ..\\tasks\\ladan_auditory_Pest\\92.00_1000Hz.wav	"
      "1000 -92.5 ..\\tasks\\ladan_auditory_Pest\\92.50_1000Hz.wav	"
      "1000 -93   ..\\tasks\\ladan_auditory_Pest\\93.00_1000Hz.wav	"
      "1000 -93.5 ..\\tasks\\ladan_auditory_Pest\\93.50_1000Hz.wav	"
      "1000 -94   ..\\tasks\\ladan_auditory_Pest\\94.00_1000Hz.wav	"
      "1000 -94.5 ..\\tasks\\ladan_auditory_Pest\\94.50_1000Hz.wav	"
      "1000 -95   ..\\tasks\\ladan_auditory_Pest\\95.00_1000Hz.wav	"
      "1000 -95.5 ..\\tasks\\ladan_auditory_Pest\\95.50_1000Hz.wav	"
      "1000 -96   ..\\tasks\\ladan_auditory_Pest\\96.00_1000Hz.wav	"
      "1000 -96.5 ..\\tasks\\ladan_auditory_Pest\\96.50_1000Hz.wav	"
      "1000 -97   ..\\tasks\\ladan_auditory_Pest\\97.00_1000Hz.wav	"
      "1000 -97.5 ..\\tasks\\ladan_auditory_Pest\\97.50_1000Hz.wav	"
      "1000 -98   ..\\tasks\\ladan_auditory_Pest\\98.00_1000Hz.wav	"
      "1000 -98.5 ..\\tasks\\ladan_auditory_Pest\\98.50_1000Hz.wav	"
      "1000 -99   ..\\tasks\\ladan_auditory_Pest\\99.00_1000Hz.wav	"
      "1000 -99.5 ..\\tasks\\ladan_auditory_Pest\\99.50_1000Hz.wav	"
      "1000 -100  ..\\tasks\\ladan_auditory_Pest\\100.00_1000Hz.wav	"
      "1000 -100.5 ..\\tasks\\ladan_auditory_Pest\\100.50_1000Hz.wav	"
      "1000 -101   ..\\tasks\\ladan_auditory_Pest\\101.00_1000Hz.wav	"
      "1000 -101.5 ..\\tasks\\ladan_auditory_Pest\\101.50_1000Hz.wav	"
      "1000 -102   ..\\tasks\\ladan_auditory_Pest\\102.00_1000Hz.wav	"
      "1000 -102.5 ..\\tasks\\ladan_auditory_Pest\\102.50_1000Hz.wav	"
      "1000 -103   ..\\tasks\\ladan_auditory_Pest\\103.00_1000Hz.wav	"
      "1000 -103.5 ..\\tasks\\ladan_auditory_Pest\\103.50_1000Hz.wav	"
      "1000 -104   ..\\tasks\\ladan_auditory_Pest\\104.00_1000Hz.wav	"
      "1000 -104.5 ..\\tasks\\ladan_auditory_Pest\\104.50_1000Hz.wav	"
      "1000 -105   ..\\tasks\\ladan_auditory_Pest\\105.00_1000Hz.wav	"
      "1000 -105.5 ..\\tasks\\ladan_auditory_Pest\\105.50_1000Hz.wav	"
      "1000 -106   ..\\tasks\\ladan_auditory_Pest\\106.00_1000Hz.wav	"   
      "1000 -106.5 ..\\tasks\\ladan_auditory_Pest\\106.50_1000Hz.wav	"
      "1000 -107   ..\\tasks\\ladan_auditory_Pest\\107.00_1000Hz.wav	"
      "1000 -107.5 ..\\tasks\\ladan_auditory_Pest\\107.50_1000Hz.wav	"
      "1000 -108   ..\\tasks\\ladan_auditory_Pest\\108.00_1000Hz.wav	"
      "1000 -108.5 ..\\tasks\\ladan_auditory_Pest\\108.50_1000Hz.wav	"
      "1000 -109   ..\\tasks\\ladan_auditory_Pest\\109.00_1000Hz.wav	"
      "1000 -109.5 ..\\tasks\\ladan_auditory_Pest\\109.50_1000Hz.wav	"
      "1000 -110   ..\\tasks\\ladan_auditory_Pest\\110.00_1000Hz.wav	"
      "1000 -110.5 ..\\tasks\\ladan_auditory_Pest\\110.50_1000Hz.wav	"
      "1000 -111   ..\\tasks\\ladan_auditory_Pest\\111.00_1000Hz.wav	"
      "1000 -111.5 ..\\tasks\\ladan_auditory_Pest\\111.50_1000Hz.wav	"
      "1000 -112   ..\\tasks\\ladan_auditory_Pest\\112.00_1000Hz.wav	"
      "1000 -112.5 ..\\tasks\\ladan_auditory_Pest\\112.50_1000Hz.wav	"
      "1000 -113   ..\\tasks\\ladan_auditory_Pest\\113.00_1000Hz.wav	"
      "1000 -113.5 ..\\tasks\\ladan_auditory_Pest\\113.50_1000Hz.wav	"
      "1000 -114   ..\\tasks\\ladan_auditory_Pest\\114.00_1000Hz.wav	"
      "1000 -114.5 ..\\tasks\\ladan_auditory_Pest\\114.50_1000Hz.wav	"
      "1000 -115   ..\\tasks\\ladan_auditory_Pest\\115.00_1000Hz.wav	"
      "1000 -115.5 ..\\tasks\\ladan_auditory_Pest\\115.50_1000Hz.wav	"
      "1000 -116   ..\\tasks\\ladan_auditory_Pest\\116.00_1000Hz.wav	"
      "1000 -116.5 ..\\tasks\\ladan_auditory_Pest\\116.50_1000Hz.wav	"
      "1000 -117   ..\\tasks\\ladan_auditory_Pest\\117.00_1000Hz.wav	"
      "1000 -117.5 ..\\tasks\\ladan_auditory_Pest\\117.50_1000Hz.wav	"
      "1000 -118   ..\\tasks\\ladan_auditory_Pest\\118.00_1000Hz.wav	"
      "1000 -118.5 ..\\tasks\\ladan_auditory_Pest\\118.50_1000Hz.wav	"
      "1000 -119   ..\\tasks\\ladan_auditory_Pest\\119.00_1000Hz.wav	"
      "1000 -119.5 ..\\tasks\\ladan_auditory_Pest\\119.50_1000Hz.wav	"
      "1000 -120  ..\\tasks\\ladan_auditory_Pest\\120.00_1000Hz.wav	"
      " // experiment definition ",  
      "Application:ReactionAuditoryCalibrationTask int UseSpaceButton= 1 1 0 1 // enable keyboard space button ? (boolean)",
      "Application:ReactionAuditoryCalibrationTask int UseDigitalInputButton= 1 1 0 1 // enable Digital input button ? (boolean)",
      "Application:ReactionAuditoryCalibrationTask string DigitalInputNum= DigitalInput1 // Digital Input nemuber",
      "Application:ReactionAuditoryCalibrationTask int PushButtonChannel= 2 2 1 % // channel number of push button",
      "Application:ReactionAuditoryCalibrationTask float PushButtonThreshold= 125mV 125mV 0 % // threshold for push button",
     END_PARAMETER_DEFINITIONS

    BEGIN_PARAMETER_DEFINITIONS
	// Pest
	"Application:ReactionAuditoryCalibrationTask float PestStartLevel= -60 -60 -120 -20 % // starting intensity",  
    "Application:ReactionAuditoryCalibrationTask float PestMinLevel= -100 -60 -120 -20 % // Minimum intensity",  
	"Application:ReactionAuditoryCalibrationTask float PestStartStepSize= 8 8 0.5 16 % // starting value for intensity change",      
	"Application:ReactionAuditoryCalibrationTask float PestFinalStepSize= 0.5 0.5 0.1 8 % // smallest value for intensity change", 
    "Application:ReactionAuditoryCalibrationTask float PestMaxStepSize= 16 16 4 16 % // Max value for intensity change",
	"Application:ReactionAuditoryCalibrationTask float PestWaldFactor= 2 2 1 5 % // number of missed trials x target likelihood without intensity change",      
	"Application:ReactionAuditoryCalibrationTask float PestTargetPerformance= 0.5 0.5 0 1 % // target performance ratio",   
	"Application:ReactionAuditoryCalibrationTask float PerceptionThreshold= -60 -60 -120 -20 % // final performance ratio", 
  "Application:ReactionAuditoryCalibrationTask float MaxItrNum= 80 80 20 200 % // starting value for intensity", 
	"Application:ReactionAuditoryCalibrationTask string PerceptionThresholdFilePath= ..\\parms.ECoG\\LadanAuditoryExperiment\\fragment_perception_threshold.prm // final performance ratio parameter fragment filepath",   
  "Application:ReactionAuditoryCalibrationTask string PestStartLevelFilePath= ..\\parms.ECoG\\LadanAuditoryExperiment\\fragment_PestStart_Level.prm // final Pest Start Level parameter fragment filepath",   
  "Application:ReactionAuditoryCalibrationTask string PestStartStepSizeFilePath= ..\\parms.ECoG\\LadanAuditoryExperiment\\fragment_PestStart_Step.prm // final Pest Start step parameter fragment filepath",   
    END_PARAMETER_DEFINITIONS

    BEGIN_PARAMETER_DEFINITIONS
	// Timing   
	"Application:ReactionAuditoryCalibrationTask float CueDuration= 1000ms 1000ms 0 % // cue duration",  
    "Application:ReactionAuditoryCalibrationTask float ISIDuration= 1000ms 1000ms 0 % // cue duration",  
    "Application:ReactionAuditoryCalibrationTask float soundcheckDuration= 7000ms 7000ms 0 % // sound check update duration",  
    "Application:ReactionAuditoryCalibrationTask float FeedbackDuration= 500ms 1000ms 0 % // feedback duration", 
    "Application:ReactionAuditoryCalibrationTask float StimuliDuration= 1500ms 4000ms 0 % // stimuli duration", 
    "Application:ReactionAuditoryCalibrationTask float WaitMin= 1s 1s 0 % // min time to present stimulus",              
	"Application:ReactionAuditoryCalibrationTask float WaitMax= 3s 3s 0 % // max time to present stimulus",
    "Application:ReactionAuditoryCalibrationTask float RTMin= 200ms 100ms 0 % // min reaction time (False Start)",              
	"Application:ReactionAuditoryCalibrationTask float RTMax= 1500ms 2000ms 0 % // max reaction time (Too Long)",
    END_PARAMETER_DEFINITIONS

        BEGIN_PARAMETER_DEFINITIONS
	// Visual Stimulus Size and Color
	"Application:ReactionAuditoryCalibrationTask string TextFeedbackPreCueColor= 0x00CC00 0xFFFF00 0x000000 0xFFFFFF // Color of negative feedback (color)",
    "Application:ReactionAuditoryCalibrationTask string TextFeedbackPreCue= Pay%20Attention%20With%20Your... // Text of pre-cue instructions",
	"Application:ReactionAuditoryCalibrationTask string AuditoryCueImageFile= ..\\tasks\\ladan_auditory_Pest\\auditory.bmp // auditory cue image file (inputfile)",
    "Application:ReactionAuditoryCalibrationTask string WrongstartWarningTone= ..\\tasks\\WrongstartWarningTone.wav // Warning tone (inputfile)",
    "Application:ReactionAuditoryCalibrationTask string TextFeedbackPreCue= Pay%20Attention%20With%20Your... // Text of pre-cue instructions",
    "Application:ReactionAuditoryCalibrationTask string TextFeedbackNegColor= 0xFF0000 0xFFFF00 0x000000 0xFFFFFF // Color of negative feedback (color)",
    "Application:ReactionAuditoryCalibrationTask string TextFeedbackPositiveColor= 0x00CC00 0xFFFF00 0x000000 0xFFFFFF // Color of negative feedback (color)",
    "Application:ReactionAuditoryCalibrationTask string TextFeedbackFalseStart= Incorrect! // Text of false start instructions",
	END_PARAMETER_DEFINITIONS
  // ...and likewise any state variables:
   
    BEGIN_STATE_DEFINITIONS
    "CurrentTrial           8  0 0 0", // The Current Trial
    "ExperimentState        4  0 0 0", //
  	"CueAuditory            1  0 0 0", //
  	"StimAuditory           1  0 0 0", //
    "Threshold              14 0 0 0",
    "StimulusLevel          14 0 0 0",//
    "Performance      14  0 0 0",//
    END_STATE_DEFINITIONS

    m_list_auditory_stimuli.clear();
    p_warning_tone = NULL;

    // text characteristics
    m_full_rectangle.left = 0.0f;
    m_full_rectangle.right = 1.0f;
    m_full_rectangle.top = 0.2f;
    m_full_rectangle.bottom = 0.8f;

    mp_text_stimulus = new TextStimulus(mrDisplay);
	mp_text_stimulus->SetText("");
	mp_text_stimulus->SetTextHeight(0.1f);
    mp_text_stimulus->SetDisplayRect(m_full_rectangle);
	mp_text_stimulus->SetColor(RGBColor::Black);
	mp_text_stimulus->SetTextColor(RGBColor::Green);
	mp_text_stimulus->Present();

    // + marker characteristics
    m_marker_rectangle.left = 0.35f;
    m_marker_rectangle.right =  0.65f;
    m_marker_rectangle.top = 0.35f;
    m_marker_rectangle.bottom = 0.65f;

    mp_text_marker = new TextStimulus(mrDisplay);
	mp_text_marker->SetText("");
	mp_text_marker->SetTextHeight(0.2f);
	mp_text_marker->SetDisplayRect(m_marker_rectangle);
	mp_text_marker->SetColor(RGBColor::Black);
	mp_text_marker->SetTextColor(RGBColor::LtGray);
	mp_text_marker->Present();

    mp_input             = NULL;

    // cue image characteristics 
    m_cue_image_rectangle.left = 0.35f;
    m_cue_image_rectangle.right = 0.65f;
    m_cue_image_rectangle.top = 0.35f;
    m_cue_image_rectangle.bottom = 0.65f;

    mp_cue_image = new ImageStimulus(mrDisplay);
	mp_cue_image->SetDisplayRect(m_cue_image_rectangle);
	mp_cue_image->SetPresentationMode( VisualStimulus::ShowHide );
	mp_cue_image->SetAspectRatioMode( GUI::AspectRatioModes::AdjustBoth );
    mp_cue_image->SetFile( Parameter("AuditoryCueImageFile") );
	mp_cue_image->Present();
	mp_cue_image->Conceal();
}


ReactionAuditoryCalibrationTask::~ReactionAuditoryCalibrationTask()
{
  Halt();

  // Free memory allocated to auditory stimuli
  for (unsigned int idx=0; idx < m_list_auditory_stimuli.size(); idx++)
      delete(m_list_auditory_stimuli[idx]);

  m_list_auditory_stimuli.clear();

  if (p_warning_tone)
  {
      if (p_warning_tone->IsPlaying())
      {
          p_warning_tone->Stop();
      }

      delete(p_warning_tone);
  }

}

void
ReactionAuditoryCalibrationTask::Halt()
{
  
}

void
ReactionAuditoryCalibrationTask::Preflight( const SignalProperties& Input, SignalProperties& Output ) const
{
  Output = Input; // this simply passes information through about SampleBlock dimensions, etc....
  
  // create pointer to parameters
    ParamRef PestStimuli            = Parameter("PestStimuli");
    ParamRef PestStartLevel         = Parameter("PestStartLevel");
    ParamRef PestStartStepSize      = Parameter("PestStartStepSize"); 
    ParamRef PestFinalStepSize      = Parameter("PestFinalStepSize");
    ParamRef PestWaldFactor         = Parameter("PestWaldFactor");
    ParamRef PestTargetPerformance  = Parameter("PestTargetPerformance");
    
    // touching states
    State("Running");
    State("KeyDown");
    State("CurrentTrial");
    State("ExperimentState");
    State("CueAuditory");
    State("StimAuditory");
    State("Threshold");
    State(Parameter("DigitalInputNum"));
    State("StimulusLevel");
    State("Performance");

    // touching parameters
    Parameter("CueDuration");
    Parameter("soundcheckDuration");
    Parameter("StimuliDuration");
    Parameter("SampleBlockSize");
    Parameter("TextFeedbackPreCueColor");
    Parameter("TextFeedbackPreCue");
    Parameter("WrongstartWarningTone");
    Parameter("SamplingRate");
    Parameter("AuditoryCueImageFile");
    Parameter("UseSpaceButton");
    Parameter("PushButtonChannel");
	  Parameter("PushButtonThreshold");
    Parameter("WaitMin");
	  Parameter("WaitMax");
    Parameter("ISIDuration");
    Parameter("PestMaxStepSize");
    Parameter("PestMinLevel");
    Parameter("PerceptionThreshold");
    Parameter("PerceptionThresholdFilePath");
    Parameter("PestStartLevelFilePath");
    Parameter("PestStartStepSizeFilePath");
    Parameter("TextFeedbackPreCue");
    Parameter("UseDigitalInputButton");
    Parameter("RTMax");
    Parameter("RTMin");
    Parameter("TextFeedbackNegColor");
    Parameter("TextFeedbackFalseStart");
    Parameter("TextFeedbackPositiveColor");
    Parameter("MaxItrNum");
    Parameter("DigitalInputNum");

     // check min reaction time is smaller than max reaction time
   if ( Parameter("RTMin").InMilliseconds() > Parameter("RTMax").InMilliseconds() )
		bcierr <<  "Parameter RTMin (" << Parameter("RTMin") << ") must be smaller or equal to RTMax (" << Parameter("RTMax") << ")." << endl;

    // check that WaitMin is smaller than WaitMax
    if ( Parameter("WaitMin").InMilliseconds() > Parameter("WaitMax").InMilliseconds() )
		bcierr <<  "Parameter WaitMin (" << Parameter("WaitMax") << ") must be smaller or equal to WaitMax (" << Parameter("WaitMax") << ")." << endl;

	double block_size_msec = (double)(Parameter("SampleBlockSize")) / (double)(Parameter("SamplingRate").InHertz()) * 1000;
  
    // check that WaitMin is a multiple of the sample block time
	if (fmod((double)Parameter("WaitMin").InMilliseconds(),block_size_msec) > 1e-6)
		bcierr <<  "Parameter WaitMin (" << Parameter("WaitMin") << ") must be a multiple of the sample block time (" << block_size_msec << "ms)." << endl;

     // check that ISIDuration is a multiple of the sample block time
	if (fmod((double)Parameter("ISIDuration").InMilliseconds(),block_size_msec) > 1e-6)
		bcierr <<  "Parameter ISIDuration (" << Parameter("ISIDuration") << ") must be a multiple of the sample block time (" << block_size_msec << "ms)." << endl;
   
    // Check that the PestStimuli matrix has at least 3 columns
	if (Parameter("PestStimuli")->NumColumns() != 3)
		bcierr <<  "Parameter PestStimuli has " << Parameter("PestStimuli")->NumColumns() << " columns, but must have 3 columns." << endl; 

    // Check that the PestStimuli matrix has at least 10 rows (i.e., stimuli)
	if (Parameter("PestStimuli")->NumRows() < 10)
		bcierr <<  "Parameter PestStimuli has " << Parameter("PestStimuli")->NumRows() << " rows, but must have at least 10 rows (i.e., number of stimuli)." << endl; 

    // Check that the PestStartStepSize is smaller than the PestFinalStepSize
    if (Parameter("PestStartStepSize") < Parameter("PestFinalStepSize"))
        bcierr <<  "PestStartStepSize " << Parameter("PestStartStepSize") << " should be larger than PestFinalStepSize " << Parameter("PestFinalStepSize") << endl; 
  
    // check that cueDuration is a multiple of the sample block time
    if (fmod((double)Parameter("CueDuration").InMilliseconds(),block_size_msec) > 1e-6)
		bcierr <<  "Parameter CueDuration (" << Parameter("CueDuration") << ") must be a multiple of the sample block time (" << block_size_msec << "ms)." << endl;

    // check that FeedbackDuration is a multiple of the sample block time
    if (fmod((double)Parameter("FeedbackDuration").InMilliseconds(),block_size_msec) > 1e-6)
		bcierr <<  "Parameter FeedbackDuration (" << Parameter("FeedbackDuration") << ") must be a multiple of the sample block time (" << block_size_msec << "ms)." << endl;

      // check that soundcheckDuration is a multiple of the sample block time
    if (fmod((double)Parameter("soundcheckDuration").InMilliseconds(),block_size_msec) > 1e-6)
		bcierr <<  "Parameter soundcheckDuration (" << Parameter("soundcheckDuration") << ") must be a multiple of the sample block time (" << block_size_msec << "ms)." << endl;

    int num_stimuli = PestStimuli->NumRows(); // get the total number of different intensities

    // vector of all the auditory stimulus intensity values
    std::vector<float> list_attenuation; 
    list_attenuation.clear();

	for (unsigned int row=0; row < num_stimuli; row++) 
        list_attenuation.push_back( PestStimuli(row,1) );
    
    float attenuation_min = *std::min_element(std::begin(list_attenuation), std::end(list_attenuation));  // find min intensity value
    float attenuation_max = *std::max_element(std::begin(list_attenuation), std::end(list_attenuation)); // find max intensity value

    // Check that the PestStartLevel is well within the minimum range of the PestStimuli attenuation range
    if ((PestStartLevel - 2.0 * PestStartStepSize) < attenuation_min) 
        bcierr <<  "PestStartLevel (" << PestStartLevel << ") must be at least 2*PestStartStepSize (" << PestStartStepSize << ") larger than the minimum attenuation in the PestStimuli (" << attenuation_min << ")." << endl;

    // Check that the PestStartLevel is well within the maximum range of the PestStimuli attenuation range
    if ((PestStartLevel + 2.0 * PestStartStepSize) > attenuation_max) 
        bcierr <<  "PestStartLevel (" << PestStartLevel << ") must be at most 2*PestStartStepSize (" << PestStartStepSize << ") smaller than the maximum attenuation in the PestStimuli (" << attenuation_max << ")." << endl;
 
    // Check that the PestStimuli have a constant stepping
    for (unsigned int row=1; row < num_stimuli; row++) 
    {
       if ((PestStimuli(row,1) - PestStimuli(row-1,1)) != ((PestStimuli(1,1) - PestStimuli(0,1))))
           bcierr <<  "Stepping in attenuation within the PestStimuli parameter has to be constant" << endl; 
    }

    // Check that the stepping in the PestStimuli is at least 0.125
    if ((PestStimuli(1,1) - PestStimuli(2,1)) < 0.25)
        bcierr <<  "Stepping in attenuation within the PestStimuli parameter has to be at least 0.25" << endl; 
    
}


void
ReactionAuditoryCalibrationTask::Initialize( const SignalProperties& Input, const SignalProperties& Output )
{
    // Create pointers to paramters
    ParamRef PestStimuli            = Parameter("PestStimuli"); //Input stimulus
    ParamRef PestStartLevel         = Parameter("PestStartLevel"); //Initial value for intensity presenting to subject
    ParamRef PestStartStepSize      = Parameter("PestStartStepSize"); // Initial step size
    ParamRef PestFinalStepSize      = Parameter("PestFinalStepSize"); // Smallest possible step size
    ParamRef PestWaldFactor         = Parameter("PestWaldFactor"); // Wald factor, PEST toleration of error
    ParamRef PestTargetPerformance  = Parameter("PestTargetPerformance"); // Target perception threshold

    // Read PEST parameters
 	m_PestStartLevel                  = Parameter("PestStartLevel");                 // starting value for intensity            
	m_PestFinalStepSize               = Parameter("PestFinalStepSize");           // smallest value for intensity change 
  m_PestStartStepSize             = Parameter("PestStartStepSize");           // smallest value for intensity change 
	m_PestWaldFactor                  = Parameter("PestWaldFactor");              // number of missed trials x target likelihood without intensity change     
	m_PestTargetPerformance           = Parameter("PestTargetPerformance");       // target performance ratio    
  m_PestStepsize                  = m_PestStartStepSize; 
  m_Pest_num_correct              = 0;
  m_Pest_doubled                  = 0;
  m_PestMaxstepsize               = Parameter("PestMaxStepSize"); // largest possible step size
  m_PestMinlevel                  = Parameter("PestMinLevel"); // Smallest possible stimulus intensity
  m_text_feedback_pre_cue			    = Parameter("TextFeedbackPreCue");
  m_max_num_trials                = Parameter("MaxItrNum");

    // Calculate cue duration parameters
    m_sample_rate		        	= Parameter("SamplingRate"); //Sampling rate
    m_block_size			        = Parameter("SampleBlockSize"); // Sample block size
    m_block_size_msec               = (float)(m_block_size) / (float)(m_sample_rate) * 1000; //Sample block size in msec                        
    m_cue_duration_blocks			= Parameter("CueDuration").InMilliseconds() / m_block_size_msec; // duration of cue state
    m_isi_duration_blocks			= Parameter("ISIDuration").InMilliseconds() / m_block_size_msec; // duration of inter stimulus trial state
    m_soundcheck_duration_blocks    = Parameter("soundcheckDuration").InMilliseconds() / m_block_size_msec; // Duration of cheching if the subject is hearing the sound
    m_wait_min_blocks				= Parameter("WaitMin").InMilliseconds() / m_block_size_msec; // Min time duration between cue and stimulus presentation
	m_wait_max_blocks				= Parameter("WaitMax").InMilliseconds() / m_block_size_msec;// Max time duration between cue and stimulus presentation
    m_stimuli_duration_blocks      = Parameter("StimuliDuration").InMilliseconds() / m_block_size_msec; // Waiting time duration since stimulus onset until feedback presentation 
    
    // Feedback parameters
    m_feedback_blocks				= Parameter("FeedbackDuration").InMilliseconds() / m_block_size_msec; // Feedback duration in msec
    m_text_feedback_pre_cue_color	= RGBColor( Parameter("TextFeedbackPreCueColor") ); // RGB color of Get ready Text
    m_text_feedback_pre_cue			= Parameter("TextFeedbackPreCue"); // Text which can be presented if we had different types of stimulus!!!
    m_text_feedback_positive_color  = RGBColor( Parameter("TextFeedbackPositiveColor") );
    m_text_feedback_negative_color  = RGBColor(Parameter("TextFeedbackNegColor"));
    m_text_feedback_false_start     = Parameter("TextFeedbackFalseStart");
    
    // Reaction time parameters
    m_max_rt                        = Parameter("RTMax");
    m_min_rt                        = Parameter("RTMin");


    // push botton parameters
    m_use_space_button				= Parameter("UseSpaceButton"); // if we use space botton
    m_push_button_channel			= Parameter("PushButtonChannel"); // push botton channel
    m_push_button_threshold			= Parameter("PushButtonThreshold").InMicrovolts(); // Push botton threshold
    m_use_Digital_input             = Parameter("UseDigitalInputButton"); // Use Digital Input 
    
     // Trial counter parameters
    m_runtime_counter       = 0;
    m_trial_counter			= 0;
    m_block_delta			= 0;
    m_is_first_trial        = true; // first state
    m_num_trials            = 0;
    m_Pest_samelevelcounter = 0;

    m_reaction_time.clear(); // reaction time vector
    m_stimulus_Level.clear(); // Stimulus intensity vector for all trials
    m_stimulus_Level_copy.clear(); // just a copy helped for coding!
    m_PEST_performance.clear(); // Subject performance vector
    m_Pest_percentage_check.clear(); // final perception percentages based on subject's performance in the total of trials
    m_Pesttrialcounter      = 0; // trial counter
    m_Pest_history_correct.clear(); // Subject's performance
   
    // pushe botton flag
    m_pushed_botton_flag    = 0;

    // State initiation
    m_current_state			= e_soundcheck;
  
    // Volume Instensity
    m_auditory_intensity    = m_PestStartLevel;

    // Determine number of Stimuli
    m_num_stimuli   = Parameter("PestStimuli")->NumRows();

    // Read attenuation from Stimuli
    m_list_attenuation.clear();
	for (unsigned int row=0; row < m_num_stimuli; row++) 
        m_list_attenuation.push_back( PestStimuli(row,1) );

    // Determine range and stepping of attenuation in Stimuli
    m_attenuation_min = *std::min_element(std::begin(m_list_attenuation), std::end(m_list_attenuation));
    m_attenuation_max = *std::max_element(std::begin(m_list_attenuation), std::end(m_list_attenuation));
    m_attenuation_stepping = PestStimuli(1,1) - PestStimuli(2,1);

    if (p_warning_tone)
        delete (p_warning_tone);

     p_warning_tone = new WavePlayer();
     p_warning_tone->SetFile(Parameter("WrongstartWarningTone"));

    // Free memory from previously used auditory stimuli
    for (unsigned int idx=0; idx < m_list_auditory_stimuli.size(); idx++)
        delete(m_list_auditory_stimuli[idx]);

    m_list_auditory_stimuli.clear();

    // Read auditory stimuli and put all the auditory files in a vector
    for (unsigned int row=0; row < m_num_stimuli; row++) 
    {
        WavePlayer *p_auditory_stimulus = new WavePlayer();
        p_auditory_stimulus->SetFile(PestStimuli(row,2));
        m_list_auditory_stimuli.push_back(p_auditory_stimulus);
    }
}

void
ReactionAuditoryCalibrationTask::StartRun()
{
  bciout << "Hello World!" << endl;
}


void
ReactionAuditoryCalibrationTask::Process( const GenericSignal& Input, GenericSignal& Output )
{
  Output = Input; // This passes the signal through unmodified.
  mp_input = &Input;

  	m_runtime_counter++;

	if (m_trial_counter >= m_num_stimuli) 
	{
		return;
	}

    // Go to first trial state if it is the first trial
	if ( m_is_first_trial )
	{
		m_current_state = e_first_trial;
	}

	// keep track of the states
	State("ExperimentState")	= m_current_state;
	State("CurrentTrial")		= m_trial_counter+1;
    State("Threshold")          = (unsigned int)floor(-1*m_auditory_intensity);

     switch(m_current_state)
    {

		//////////////////////////////////////////////////////////////////////////
		case e_first_trial:
			{
                // This state provides instructions for the subject, e.g. Get ready
				m_block_delta++;

				// present "get ready" statement
				mp_text_stimulus->SetTextColor( m_text_feedback_pre_cue_color );
				mp_text_stimulus->SetText("Get Ready...");
				mp_text_stimulus->Present();

				// check if first trial period has expired
				if(m_block_delta >= 2*m_cue_duration_blocks) //Hold "get ready" for 3 seconds...
				{
					// go to next state
                    mp_text_stimulus->Conceal();
			        m_current_state = e_soundcheck;
				    m_block_delta = 0;
					m_is_first_trial = false;
				}
			}

			break;
            ////////////////////////////////////////////////////////////////////////
        case e_soundcheck:
            {
                m_block_delta++;

                if (m_block_delta == 1)
                {
                   
                    mp_text_stimulus->SetTextColor( m_text_feedback_pre_cue_color );
                    mp_text_stimulus->SetText("Please push the button if you heard the sound...");
                    mp_text_stimulus->Present();
                    AppLog << "State: checking the sound ..." << endl;
                    StimulusOn(-30); // stimulus presentation to check if the subject is hearing the sounds/ the system is working properly
                }

                // Go to the next state only if the subject has heard the sound presented
                if (m_block_delta >= 5     )
                {    
                    if ( !IsButtonPressed() )
                    {
                       // mp_text_stimulus->Conceal();
                        m_current_state = e_soundcheck;
                        m_block_delta = 0;
                        StimulusOff(-30);
                    }
                    if (IsButtonPressed())
                    {
                        mp_text_stimulus->Conceal();
                        m_current_state = e_cue;
                        m_block_delta = 0;
                        StimulusOff(-30);
                    }
                }
               
            }

            break;
            ///////////////////////////////////////////////////////////////////////
        case e_cue:
            {
            
                 State("Performance") = 0;
                 State("StimulusLevel") = 0;
                 m_block_delta++;
                 
                 if (m_block_delta == 1)
				{
                   // p_warning_tone -> Stop();
                    AppLog << "=============================================" << endl;
                    AppLog << "---------------------------------------------" << endl;
                    AppLog << "Trial #" << m_Pesttrialcounter+1 << endl;
                    AppLog << "---------------------------------------------" << endl;
          					AppLog << "State: Presenting cues ..." << endl;
                    
                    if (m_auditory_intensity >= -20)
                    {
                        m_auditory_intensity = -20;
                  			AppLog << "............WARNING!!!!................" << endl;
                  			AppLog << " ...........The patient is not hearing a very loud sound!!!!!.............." << endl;
                  			AppLog << " ............STOP THE EXPERIMENT AND CHECK THE HEADPHONE!!!!..............." << endl;
                        mp_text_stimulus->SetTextColor(RGBColor::Yellow);
                        mp_text_stimulus->SetText("EXPERIMENT STOPPED");
                        mp_text_stimulus->Present();
                        State("Running") = 0;
                        SummarizeExperiment();
                    }
                    
                    if (m_auditory_intensity <= -120)
                    {
                        m_auditory_intensity = -120;
                        AppLog << "............WARNING!!!!................" << endl;
                   			AppLog << " ...........The patient is hearing a very LOW sound!!!!!.............." << endl;
                  			AppLog << " ............STOP THE EXPERIMENT!!!!..............." << endl;
                        mp_text_stimulus->SetTextColor(RGBColor::Yellow);
                        mp_text_stimulus->SetText("EXPERIMENT STOPPED");
                        mp_text_stimulus->Present();
                        State("Running") = 0;
                        SummarizeExperiment();

                    }

                       AppLog << "stimulus Intensity : " << m_auditory_intensity << endl;
				}



                 // presentation of + marker
                 if (m_block_delta == 0.5*m_cue_duration_blocks)
                 {
                    if (m_PestStepsize > m_PestFinalStepSize)
                    {
                        VisualMarkerOn();
                    }
                     // Stop the experiment if the smallest step size is reached
                    if (m_PestStepsize <= m_PestFinalStepSize)
                    {
                        mp_text_stimulus->SetTextColor(RGBColor::Yellow);
                        mp_text_stimulus->SetText("End of experiment");
                        mp_text_stimulus->Present();
                        AppLog << "==== Finished !!! =====" << endl;
                        State("Running") = 0;
                        SummarizeExperiment();
                    }
                 }
                 

                 if(m_block_delta >= m_cue_duration_blocks)
                 {
                      if (IsButtonPressed())
                  {
                      m_current_state = e_false_start;
                      m_block_delta = 0;
                  }
                      else
                      {
                          m_current_state = e_pre_stimuli;
                          m_block_delta = 0;
                      }
                 }
            
            }
            break;
            ////////////////////////////////////////////////////////////////////
        case e_pre_stimuli:
            {
                 m_block_delta++;

                 if (m_block_delta ==1)
                 {
                      m_rand_blocks = RandomNumber(m_wait_min_blocks,m_wait_max_blocks);	// random delay in presenting the auditory stimuli after CueOff
                      AppLog << "State: Waiting for " << ((float)(m_rand_blocks) * m_block_size_msec) << "ms ..." << endl;
                 }

                  if (IsButtonPressed())
                  {
                      m_current_state = e_false_start;
                      m_block_delta = 0;
                  }

                 if (m_block_delta >= m_rand_blocks)
                 {
                   m_current_state = e_stimuli;
                   m_block_delta = 0;
                 }

            }
            break;
            ///////////////////////////////////////////////////////////////////
        case e_stimuli:
            {
             m_block_delta++;
            
             if (m_block_delta ==1)
             {
              AppLog << "State: Presenting auditory stimuli ..." << endl;
              StimulusOn(m_auditory_intensity);// Auditory stimuli onset
              State("StimulusLevel")        = (unsigned int)(-1*m_auditory_intensity);
             }

             // Wait for a duration or a push botton and then go to the next state
              if (IsButtonPressed() || (m_block_delta >= m_stimuli_duration_blocks))
             {
                 if (IsButtonPressed())
                 {
                      if (ReactionTime() <= m_max_rt & ReactionTime() >= m_min_rt )
                      {
                          m_pushed_botton_flag = 1;
                          m_reaction_time.push_back(ReactionTime());// Recording the reaction time if there is a botton press
                          m_PEST_performance.push_back(m_pushed_botton_flag);// Recording the performance of subject, pushed the botton or not?
                          m_stimulus_Level.push_back(m_auditory_intensity); // recording the intensity of the presented stimuli
                         // State("StimulusLevel")        = (unsigned int)(-1*m_auditory_intensity);
                           State("Performance")        = (unsigned int)(m_pushed_botton_flag);  
                          StimulusOff(m_auditory_intensity); // Offset of the stimuli
                          m_current_state = e_feedback;
                          m_block_delta = 0;
                      }
                       else if (ReactionTime() < m_min_rt || ReactionTime() > m_max_rt) 
                            {
                            StimulusOff(m_auditory_intensity); // Offset of the stimuli
                            m_current_state = e_false_start;
                            m_block_delta = 0;
                            }
                 }
                 else if (!IsButtonPressed() & m_block_delta!=0)
                        {
                            m_pushed_botton_flag = 0;
                            m_reaction_time.push_back(ReactionTime());
                            m_PEST_performance.push_back(m_pushed_botton_flag);// Recording the performance of subject, pushed the botton or not?
                            m_stimulus_Level.push_back(m_auditory_intensity); // recording the intensity of the presented stimuli
                           // State("StimulusLevel")        = (unsigned int)(-1*m_auditory_intensity);
                            State("Performance")        = (unsigned int)(m_pushed_botton_flag);  
                            StimulusOff(m_auditory_intensity); // Offset of the stimuli
                            m_current_state = e_feedback;
                            m_block_delta = 0;
                        }  
               
              }
               
                  
            }
            break;
            ////////////////////////////////////////////////////////////////
        case e_feedback:
            {
                m_block_delta++;
                
                if (m_block_delta ==1)
                {
                    AppLog << "State: Presenting feedback ..." << endl;
                    VisualMarkerOff();
            
                    // present "Good job!" statement if the subject pushed the botton
                    if (m_pushed_botton_flag == 1)
                    { 
                        AppLog << "State: Response time = " << m_reaction_time[m_Pesttrialcounter] << "ms ..." << endl;
                        m_Pest_num_correct++;

                       /*mp_text_stimulus->SetTextColor(RGBColor::Yellow);
                       mp_text_stimulus->SetText("Good job!");
                       mp_text_stimulus-> Present();*/

                       mp_text_stimulus->SetTextColor( m_text_feedback_positive_color );
                      // m_text_stimulus_string.str("Good job! \n Your Response Time: \n ");
                       m_text_stimulus_string.str("Your Response Time: \n ");
                       m_text_stimulus_string << m_text_stimulus_string.str() << "\t\t\t" << (unsigned int)( m_reaction_time[m_Pesttrialcounter])  << " ms";
		               mp_text_stimulus->SetText(m_text_stimulus_string.str());
                       mp_text_stimulus->Present();
                    }

                    // present "Try harder!" statement if the subject didn't pushed the botton
                    if (m_pushed_botton_flag == 0)
                    {
                        AppLog << "State: trial is missed" << endl;
                        mp_text_stimulus->SetTextColor(RGBColor::Red);
                        mp_text_stimulus->SetText("Try Harder!");
                        mp_text_stimulus->Present();
                    }
                }
                
                // present the feedback text for a while and then go to next state
                if (m_block_delta >= m_feedback_blocks)
                {
                    mp_text_stimulus->Conceal();
                    m_current_state = e_isi;
                    m_block_delta = 0;
                }
            }
            break;
            /////////////////////////////////////////////////////////////
        case e_isi:
            {
                m_block_delta++;

                if (m_block_delta ==1)
                {
                      AppLog << "State: Inter Stimulus Interval (ISI) ..." << endl;
                      // mp_text_stimulus->Conceal();
                }

                // wait for isi duration and then go to next state
                if (m_block_delta >= m_isi_duration_blocks)
                {
                    m_current_state = e_PEST;
                    m_block_delta = 0;
                }
            }
            break;
            ///////////////////////////////////////////////////////////
        case e_prePEST:
            {
                m_block_delta++;

               if (m_block_delta ==1)
               {
                   if (m_PestStepsize > m_PestFinalStepSize)
                   {
                       m_current_state = e_PEST;
                       m_block_delta = 0;
                   }
               }
            }
            break;
            ////////////////////////////////////////////////////////////
        case e_PEST:
            {
                m_block_delta++;
                if (m_block_delta ==1)
                {

                    m_Pesttrialcounter++;
                    
                    if (m_Pesttrialcounter == m_max_num_trials)
                    {
                        mp_text_stimulus->SetTextColor(RGBColor::Yellow);
                        mp_text_stimulus->SetText("End of experiment");
                        mp_text_stimulus->Present();
                        AppLog << "==== Finished !!! =====" << endl;
                        State("Running") = 0;
                        SummarizeExperiment();
                    }

                    if (((m_PestTargetPerformance * m_num_trials) - m_PestWaldFactor < m_Pest_num_correct) && (m_Pest_num_correct < (m_PestTargetPerformance * m_num_trials) + m_PestWaldFactor))
                    {
                        m_num_trials++;
                        m_current_state = e_cue;
                        m_block_delta = 0;
                        m_Pest_samelevelcounter++;
                    }
                    else
                    {
                        m_Pest_samelevelcounter =0;
                        
                        if (m_Pest_num_correct >= ((m_num_trials*m_PestTargetPerformance)+m_PestWaldFactor))
                        {
                            m_Pest_history_correct.append("d");
                        }
                         if (m_Pest_num_correct <= ((m_num_trials*m_PestTargetPerformance)-m_PestWaldFactor))
                        {
                            m_Pest_history_correct.append("u");
                        }

                         // PEST rule #1
                         if (m_Pest_history_correct.length() > 1)
                         {
                             m_historylength = m_Pest_history_correct.length();
                             a = m_Pest_history_correct[m_historylength-1];
                             b = m_Pest_history_correct[m_historylength-2];

                             if (a.compare(b) != 0)
                                 m_PestStepsize = m_PestStepsize/2;
                         }

                         // PEST rule #3,4
                         if (m_Pest_history_correct.length() > 2)
                         {
                              if (m_Pest_history_correct.length() == 3)
                              { 
                                    m_historylength = m_Pest_history_correct.length();
                                    a = m_Pest_history_correct.substr(m_historylength-3);
                                   
                                    if (!(a.compare("ddd")) || !(a.compare("uuu")))
                                    {
                                         m_PestStepsize = m_PestStepsize*2;
                                         m_Pest_doubled = 1;

                                    }
                              }
                              if (m_Pest_history_correct.length() > 3)
                              {
                                   m_historylength = m_Pest_history_correct.length();
                                    a = m_Pest_history_correct.substr(m_historylength-4);
                                   
                                    if (!(a.compare("uddd")) || !(a.compare("duuu")))
                                    {
                                        if (m_Pest_doubled == 1)
                                            m_Pest_doubled = 0;
                                        else
                                        {
                                          m_PestStepsize = m_PestStepsize*2;
                                          m_Pest_doubled = 1;
                                        }

                                    }
                                    if (!(a.compare("uuuu")) || !(a.compare("dddd")))
                                    {
                                        m_PestStepsize = m_PestStepsize*2;
                                        m_Pest_doubled = 1;
                                    }
                              }   
                         }
                          // Exception to rule #3 (TAYLOR & CREELMAN, 1967)
                         if (m_PestStepsize > m_PestMaxstepsize)
                             m_PestStepsize = m_PestMaxstepsize;

                          m_historylength = m_Pest_history_correct.length();
                          a = m_Pest_history_correct.substr(m_historylength-1);
                          
                          if (!(a.compare("u")))
                          {
                                 m_auditory_intensity = m_auditory_intensity + m_PestStepsize;
                                 AppLog << "intensity is increased" << endl;
                          }
                          else
                          {
                                  m_auditory_intensity = m_auditory_intensity - m_PestStepsize;
                                  AppLog << "intensity is decreased" << endl;
                          }

                          if (m_auditory_intensity <= m_PestMinlevel)
                              m_auditory_intensity = m_PestMinlevel + m_PestFinalStepSize;

                         m_num_trials = 0;
                         m_Pest_num_correct = 0;
                         m_current_state = e_cue;
                         m_block_delta = 0;
                    }
                
                }
            }
            break;
            //////////////////////////////
           case e_false_start:
                    {
                        m_block_delta++;
                        if (m_block_delta ==1)
                        {
                            mp_text_stimulus->SetTextColor(m_text_feedback_negative_color);
                            mp_text_stimulus->SetText(m_text_feedback_false_start);
                            mp_text_stimulus->Present();

                           // p_warning_tone->Play();
                        }
                        if (m_block_delta > m_feedback_blocks)
                        {
                           
                            mp_text_stimulus->Conceal();
                            m_current_state = e_cue;
                            m_block_delta = 0;
                        }
                    }
                    break;
                    ///////////////////////////////////////
    }
}

void
ReactionAuditoryCalibrationTask::StopRun()
{
 
  Parameter("PerceptionThreshold") = m_auditory_intensity;

  std::ofstream f(Parameter("PerceptionThresholdFilePath"));
  f << Parameters->ByName("PerceptionThreshold");
//  Parameters->ByName("PerceptionThreshold").Serialize(f);


 Parameter("PestStartLevel") = m_auditory_intensity;

  std::ofstream ff(Parameter("PestStartLevelFilePath"));
  ff << Parameters->ByName("PestStartLevel");

 //Parameter("PestStartStepSize") = m_PestStepsize;

 // std::ofstream fff(Parameter("PestStartStepSizeFilePath"));
 // fff << Parameters->ByName("PestStartStepSize");



}


unsigned int
ReactionAuditoryCalibrationTask::CalculateStimulusIndex( float attenuation )
{
    float k = (float)(m_num_stimuli-1) / (float)(m_attenuation_min - m_attenuation_max);
    float d = (float)(m_num_stimuli-1) * ((float)m_attenuation_max / (float)(m_attenuation_max - m_attenuation_min));

    float y = attenuation * k + d; 
    int index = float (y);

    if (index > m_num_stimuli-1)
        index = m_num_stimuli-1;

    if (index < 0) 
        index = 0;

    return (index);
 
}

void 
ReactionAuditoryCalibrationTask::StimulusOn( float attenuation )
{

    unsigned int index = CalculateStimulusIndex( attenuation );

    m_list_auditory_stimuli[index]->Play();
	bciout << "auditory stimulus started" << endl; 
}

void 
ReactionAuditoryCalibrationTask::StimulusOff( float attenuation )
{

    unsigned int index = CalculateStimulusIndex( attenuation );

	m_list_auditory_stimuli[index]->Stop();
	bciout << "auditory stimulus stopped" << endl; 
}

bool
ReactionAuditoryCalibrationTask::IsButtonPressed()
{

	//Check for keypresses
	bool press = false;

   if(m_use_Digital_input) 
	{
		for( unsigned int i = 0; i < m_block_size; i++ )
		{
			if( State( Parameter("DigitalInputNum") )( i ) == 1)
			{
				press = true;
				//bciout << "key pressed" << endl;
			}
		}	
	} 
	else if(m_use_space_button) 
	{
		for( unsigned int i = 0; i < m_block_size; i++ )
		{
			if( State( "KeyDown" )( i ) == VK_SPACE)
			{
				press = true;
				//bciout << "key pressed" << endl;
			}
		}	
	} 
	else
	{		
		for( unsigned int i = 0; i < m_block_size; i++ )
		{
			if (mp_input != NULL) {

				if( mp_input->Value(m_push_button_channel-1,i) > m_push_button_threshold )
				{
					press = true;
				//	bciout << "button pressed" << endl;
					break;
				}
			}
		}	
	}

	return press;
}

void
ReactionAuditoryCalibrationTask::CueImageOn()
{
	if (mp_cue_image)
	{
		mp_cue_image->Present();
	}
}

void
ReactionAuditoryCalibrationTask::CueImageOff()
{
	if (mp_cue_image)
	{
		mp_cue_image->Conceal();
	}
}

void
ReactionAuditoryCalibrationTask::VisualMarkerOn()
{
		mp_text_marker->SetText("+");
}


void
ReactionAuditoryCalibrationTask::VisualMarkerOff()
{
		mp_text_marker->SetText("");
}

int
ReactionAuditoryCalibrationTask::RandomNumber(int min, int max)
{
	int random_number = min + m_random_generator.Random() % (max - min);
	
        //bciout << random_number << endl;

	return random_number;
}

float ReactionAuditoryCalibrationTask::ReactionTime( )
{
	unsigned int offset = 0;
	bool press = false; 
      if(m_use_Digital_input) 
	{
		for( offset = 0; offset < m_block_size; offset++ )
		{
			if( State( Parameter("DigitalInputNum") )( offset ) == 1 )
			{
				press = true;
                break;
				//bciout << "key pressed" << endl;
			}
		}	
	} 
	else if(m_use_space_button) 
	{
		//Check for keypresses
		for( offset = 0; offset < m_block_size; offset++ )
		{
			if( State( "KeyDown" )( offset ) == VK_SPACE )
			{
				press = true;
				break;
			}
		}	
	}
	else
	{
		for( offset = 0; offset < m_block_size; offset++ )
		{
			if (mp_input != NULL) {

				if( mp_input->Value(m_push_button_channel-1,offset) > m_push_button_threshold )
				{
					press = true;
//					bciout << "button pressed with offset " << offset << endl;
					break;
				}
			}
		}	
	}
		


	if (press) 
	{
		return (float)(m_block_delta-2) * m_block_size_msec + (float)(offset+1) / (float)(m_block_size) * m_block_size_msec;
	}
	else
	{
		return -1;
	}

}


void ReactionAuditoryCalibrationTask::SummarizeExperiment()
{
    float rt_sumation = 0;
    unsigned int rt_counter = 0; 


    for (unsigned int i=0; i<m_reaction_time.size(); i++)
    {
        if (m_reaction_time[i] != -1)
       {
           rt_sumation+=m_reaction_time[i];
           rt_counter++;
        }
    }

    AppLog << "=============================================" << endl;
	AppLog << "*************** S U M M A R Y ***************" << endl;
	AppLog << "=============================================" << endl;
	AppLog << "---------------------------------------------" << endl;
	//AppLog << "Total Runtime: " << ( m_Pesttrialcounter * m_block_size_msec ) / 1000 << "s" << endl;
	AppLog << "Total trials: " << m_Pesttrialcounter+1 << endl;
    AppLog << "Total trials push botton: " << rt_counter << endl;
    AppLog << "Total missed trials: " << m_reaction_time.size()-rt_counter << endl;
	AppLog << "perception threshold: " << m_auditory_intensity << endl;
    AppLog << "average reaction time: " << rt_sumation/rt_counter << endl;

    AppLog << "stimulus level: " << " ";
    for (unsigned int i=0; i<m_stimulus_Level.size(); i++)
    {
        AppLog <<  m_stimulus_Level[i] << " ";
    }
    AppLog << endl;

     AppLog << "subject performance: "<<" ";
     for (unsigned int i=0; i<m_PEST_performance.size(); i++)
    {
        AppLog <<  m_PEST_performance[i] << " ";
    }
     AppLog << endl;
     m_stimulus_Level_copy = m_stimulus_Level;
     std::sort(m_stimulus_Level_copy.begin(), m_stimulus_Level_copy.end()); 
     auto last = std::unique(m_stimulus_Level_copy.begin(), m_stimulus_Level_copy.end());
     m_stimulus_Level_copy.erase(last, m_stimulus_Level_copy.end()); 

     AppLog << "unique stimulus level: "<< " " ;
      for (unsigned int i=0; i<m_stimulus_Level_copy.size(); i++)
    {
        AppLog <<  m_stimulus_Level_copy[i] << " ";
    }
      AppLog << endl;
      float counter;
      float summation_count;
      AppLog << "Performance percentage: " << " ";
      for (unsigned int i=0; i<m_stimulus_Level_copy.size(); i++)
      {
           counter = 0;
           summation_count = 0;
          for (unsigned int j=0; j<m_stimulus_Level.size()-1; j++)
          {
              if (m_stimulus_Level_copy[i] == m_stimulus_Level[j])
              {
                  counter++;
                  summation_count += m_PEST_performance[j];
              }
          }
          m_Pest_percentage_check.push_back(summation_count/counter);
          AppLog <<  m_Pest_percentage_check[i] << " ";
      }
      AppLog << endl;
}
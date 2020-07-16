////////////////////////////////////////////////////////////////////////////////
// Authors:
// Description: ReactionAuditoryTask implementation
////////////////////////////////////////////////////////////////////////////////

#include "ReactionAuditoryTask.h"
#include "BCIStream.h"
#include <math.h>
#include <Windows.h>
#include "RandomGenerator.h"
#include "TextStimulus.h"
#include <utility>
#include <algorithm>
#include <iostream>



using namespace std;


RegisterFilter( ReactionAuditoryTask, 3 );
     // Change the location if appropriate, to determine where your filter gets
     // sorted into the chain. By convention:
     //  - filter locations within SignalSource modules begin with "1."
     //  - filter locations within SignalProcessing modules begin with "2."
     //       (NB: SignalProcessing modules must specify this with a Filter() command in their PipeDefinition.cpp file too)
     //  - filter locations within Application modules begin with "3."


ReactionAuditoryTask::ReactionAuditoryTask() :
  mrDisplay( Window() )
{
     BEGIN_PARAMETER_DEFINITIONS
     "Application:ReactionAuditoryTask matrix Stimuli= "
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
    "Application:ReactionAuditoryTask int UseSpaceButton= 1 1 0 1 // enable keyboard space button ? (boolean)",
    "Application:ReactionAuditoryTask int UseDigitalInputButton= 1 1 0 1 // enable Digital input button ? (boolean)",
    "Application:ReactionAuditoryTask string DigitalInputNum= DigitalInput1 // Digital Input nemuber",
    "Application:ReactionAuditoryTask int PushButtonChannel= 2 2 1 % // channel number of push button",
    "Application:ReactionAuditoryTask float PushButtonThreshold= 125mV 125mV 0 % // threshold for push button",
    "Application:ReactionAuditoryTask float PerceptionThreshold= -60 -60 -100 -40 % // final performance ratio",   
    "Application:ReactionAuditoryTask int NumBlocks= 15 15 1 100 % // number of blocks per stimuli intensity", 
    "Application:ReactionAuditoryTask int ZeroIntensityPercentage= 15 15 0 100 % // Percentages of catch trials",
    "Application:ReactionAuditoryTask int IntensityPositiveDelta= 32 32 0 64 % // Positive value added to perception threshold",
    "Application:ReactionAuditoryTask int IntensityNegativeDelta= -80 -80 -160 0 % // Negative value added to perception threshold",
    END_PARAMETER_DEFINITIONS

      BEGIN_PARAMETER_DEFINITIONS
	// Timing   
	  "Application:ReactionAuditoryTask float CueDuration= 1000ms 1000ms 0 % // cue duration",  
    "Application:ReactionAuditoryTask float ISIDuration= 1000ms 1000ms 0 % // inter stimulus interval duration",  
    "Application:ReactionAuditoryTask float RestDuration= 60000ms 120000ms 0 % // Rest duration",  
    "Application:ReactionAuditoryTask float soundcheckDuration= 7000ms 7000ms 0 % // sound check update duration",  
    "Application:ReactionAuditoryTask float FeedbackDuration= 1000ms 1000ms 0 % // feedback duration", 
    "Application:ReactionAuditoryTask float StimuliDuration= 1500ms 4000ms 0 % // stimuli duration", 
    "Application:ReactionAuditoryTask float WaitMin= 1s 1s 0 % // min time to present stimulus",              
	  "Application:ReactionAuditoryTask float WaitMax= 3s 3s 0 % // max time to present stimulus",
    "Application:ReactionAuditoryTask float RTMin= 200ms 100ms 0 % // min reaction time (False Start)",              
  	"Application:ReactionAuditoryTask float RTMax= 1500ms 2000ms 0 % // max reaction time (Too Long)",
    END_PARAMETER_DEFINITIONS

        BEGIN_PARAMETER_DEFINITIONS
	// Visual Stimulus Size and Color
    "Application:ReactionAuditoryTask int DoPreCue= 1 1 0 1 // do pre-cue state ? (boolean)",
     "Application:ReactionAuditoryTask string WrongstartWarningTone= ..\\tasks\\WrongstartWarningTone.wav // Warning tone (inputfile)",
     "Application:ReactionAuditoryTask string SoundhearingcheckTone= ..\\tasks\\ladan_auditory_Pest\\40.00_1000Hz.wav // Warning tone (inputfile)",
    "Application:ReactionAuditoryTask float TextHeight= 0.1 0.1 0 % // text height in percent of screeen size",  
	  "Application:ReactionAuditoryTask string TextFeedbackPreCueColor= 0x00CC00 0xFFFF00 0x000000 0xFFFFFF // Color of pre cue feedback (color)",
    "Application:ReactionAuditoryTask string TextFeedbackNegColor= 0xFF0000 0xFFFF00 0x000000 0xFFFFFF // Color of negative feedback (color)",
    "Application:ReactionAuditoryTask string TextFeedbackPositiveColor= 0x00CC00 0xFFFF00 0x000000 0xFFFFFF // Color of negative feedback (color)",
    "Application:ReactionAuditoryTask string TextFeedbackPreCue= Pay%20Attention%20With%20Your... // Text of pre-cue instructions",
    "Application:ReactionAuditoryTask string TextFeedbackFalseStart= Incorrect!... // Text of false start instructions",
	  "Application:ReactionAuditoryTask string AuditoryCueImageFile= ..//tasks//ladan_auditory_Pest//auditory.bmp"
      " // auditory cue image file (inputfile)",
	END_PARAMETER_DEFINITIONS
  
     BEGIN_STATE_DEFINITIONS
    "CurrentTrial     8  0 0 0", // The Current Trial
    "ExperimentState  4  0 0 0", //
	  "CueAuditory      1  0 0 0", //
	  "StimAuditory     1  0 0 0", //
    "ReactionTime     12  0 0 0", //
    "Performance      12  0 0 0",//
    "StimulusLevel    12 0 0 0",//
    END_STATE_DEFINITIONS

    m_list_auditory_stimuli.clear();
    p_warning_tone = NULL;

   // Set characteristics of text feedback
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
   // mp_text_stimulus->Conceal();

    // Set characteristics of marker
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
    //mp_text_marker->Conceal();

    mp_input             = NULL;

    // Set characteristics of cue image
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

ReactionAuditoryTask::~ReactionAuditoryTask()
{
  Halt();
  
 // Stop all auditory stimuli
 for (unsigned int idx=0; idx < m_list_auditory_stimuli.size(); idx++)
 {
     if (m_list_auditory_stimuli[idx]->IsPlaying())
     {
         m_list_auditory_stimuli[idx]->Stop();
     }
 }

 // Free memory allocated to auditory stimuli
 for (unsigned int idx=0; idx < m_list_auditory_stimuli.size(); idx++)
 {
      delete(m_list_auditory_stimuli[idx]);
 }

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
ReactionAuditoryTask::Halt()
{

}


void
ReactionAuditoryTask::Preflight( const SignalProperties& Input, SignalProperties& Output ) const
{
     ParamRef Stimuli            = Parameter("Stimuli");

    State(Parameter("DigitalInputNum"));
    State("Running");
    State("KeyDown");
    State("CurrentTrial");
    State("ExperimentState");
    State("CueAuditory");
    State("StimAuditory");
    State("ReactionTime");
    State("Performance");
    State("StimulusLevel");

    Parameter("DigitalInputNum");
    Parameter("UseSpaceButton");
    Parameter("PushButtonChannel");
    Parameter("PushButtonThreshold");
    Parameter("UseDigitalInputButton");
    Parameter("CueDuration");
    Parameter("ISIDuration");
    Parameter("soundcheckDuration"); 
    Parameter("FeedbackDuration"); 
    Parameter("StimuliDuration");
    Parameter("WaitMin");             
    Parameter("WaitMax");
    Parameter("DoPreCue");
    Parameter("TextHeight");
    Parameter("TextFeedbackPreCueColor");
    Parameter("TextFeedbackPositiveColor");
    Parameter("TextFeedbackPreCue");
    Parameter("AuditoryCueImageFile");
    Parameter("PerceptionThreshold");
    Parameter("NumBlocks");
    Parameter("ZeroIntensityPercentage");
    Parameter("RTMax");
    Parameter("RTMin");
    Parameter("TextFeedbackNegColor");
    Parameter("TextFeedbackFalseStart");
    Parameter("IntensityPositiveDelta");
    Parameter("IntensityNegativeDelta");
    Parameter("WrongstartWarningTone");
    Parameter("SoundhearingcheckTone");

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
   
    // check that cueDuration is a multiple of the sample block time
    if (fmod((double)Parameter("CueDuration").InMilliseconds(),block_size_msec) > 1e-6)
		bcierr <<  "Parameter CueDuration (" << Parameter("CueDuration") << ") must be a multiple of the sample block time (" << block_size_msec << "ms)." << endl;

    // check that FeedbackDuration is a multiple of the sample block time
    if (fmod((double)Parameter("FeedbackDuration").InMilliseconds(),block_size_msec) > 1e-6)
		bcierr <<  "Parameter FeedbackDuration (" << Parameter("FeedbackDuration") << ") must be a multiple of the sample block time (" << block_size_msec << "ms)." << endl;

      // check that soundcheckDuration is a multiple of the sample block time
    if (fmod((double)Parameter("soundcheckDuration").InMilliseconds(),block_size_msec) > 1e-6)
		bcierr <<  "Parameter soundcheckDuration (" << Parameter("soundcheckDuration") << ") must be a multiple of the sample block time (" << block_size_msec << "ms)." << endl;

  Output = Input; // this simply passes information through about SampleBlock dimensions, etc....

}


void
ReactionAuditoryTask::Initialize( const SignalProperties& Input, const SignalProperties& Output )
{
 
    // Calculate duration parameters
       
    m_sample_rate		        	= Parameter("SamplingRate");
    m_block_size			        = Parameter("SampleBlockSize"); 
    m_block_size_msec               = (float)(m_block_size) / (float)(m_sample_rate) * 1000;  
    m_cue_duration_blocks			= Parameter("CueDuration").InMilliseconds() / m_block_size_msec;
    m_isi_duration_blocks			= Parameter("ISIDuration").InMilliseconds() / m_block_size_msec;
    m_rest_duration_blocks			= Parameter("RestDuration").InMilliseconds() / m_block_size_msec;
    m_soundcheck_duration_blocks    = Parameter("soundcheckDuration").InMilliseconds() / m_block_size_msec;
    m_wait_min_blocks				= Parameter("WaitMin").InMilliseconds() / m_block_size_msec;
	m_wait_max_blocks				= Parameter("WaitMax").InMilliseconds() / m_block_size_msec;
    m_stimuli_duration_blocks       = Parameter("StimuliDuration").InMilliseconds() / m_block_size_msec;
 
    // Auditory intensity values
    m_stimuli_intensity_pos_delta   = Parameter("IntensityPositiveDelta");
    m_stimuli_intensity_neg_delta   = Parameter("IntensityNegativeDelta");

   // Reaction time parameters
    m_max_rt                         = Parameter("RTMax");
    m_min_rt                         = Parameter("RTMin");

    // Sequence parameters
    m_number_blocks                 = Parameter("NumBlocks");
    m_zero_intensity_percentage     = Parameter("ZeroIntensityPercentage");

    // Feedback parameters
    m_feedback_blocks				= Parameter("FeedbackDuration").InMilliseconds() / m_block_size_msec;
    m_text_feedback_pre_cue_color	= RGBColor( Parameter("TextFeedbackPreCueColor") );
    m_text_feedback_positive_color  = RGBColor( Parameter("TextFeedbackPositiveColor") );
    m_text_feedback_negative_color  = RGBColor(Parameter("TextFeedbackNegColor"));
    m_text_feedback_false_start     = Parameter("TextFeedbackFalseStart");
    
    // push botton parameters
    m_use_space_button				= Parameter("UseSpaceButton"); 
    m_use_Digital_input             = Parameter("UseDigitalInputButton");
    m_push_button_channel			= Parameter("PushButtonChannel");
    m_push_button_threshold			= Parameter("PushButtonThreshold").InMicrovolts();
    m_do_pre_cue                    = Parameter("DoPreCue") == 1;
    m_text_feedback_pre_cue			= Parameter("TextFeedbackPreCue");
    m_text_height                   = Parameter("TextHeight");
    m_perception_threshold          = Parameter("PerceptionThreshold"); 

    mp_text_stimulus->SetTextHeight(m_text_height);

    // Trial counter parameters
    m_runtime_counter       = 0;
    m_trial_counter			= 0;
    m_block_delta			= 0;
    m_is_first_trial        = true;
    m_num_trials            = 0;
    m_rand_blocks           = 0;
    m_counter_blocks        = 0;
    m_num_unresponded_trial_counter = 0;
    m_stimuli_intensity_trial.clear();

    // pushe botton flag
    m_pushed_botton_flag    = 0;
    m_stimulus_onset_flag   = 0;
 
    // outputs
    m_reaction_time.clear();
    m_subject_performance.clear();

    // State initiation
    m_current_state		    	= e_soundcheck;

    ParamRef Stimuli            = Parameter("Stimuli");
    m_num_stimuli               = Parameter("Stimuli")->NumRows();
   
    // Index estimation for any stimuli intensity based on the Stimuli matrix 
    m_list_attenuation.clear();
	for (unsigned int row=0; row < m_num_stimuli; row++) 
        m_list_attenuation.push_back( Stimuli(row,1) );

    m_attenuation_min = *std::min_element(std::begin(m_list_attenuation), std::end(m_list_attenuation));
    m_attenuation_max = *std::max_element(std::begin(m_list_attenuation), std::end(m_list_attenuation));

    m_perception_threshold_index = CalculateStimulusIndex(m_perception_threshold);
  
   // Free memory from previously used auditory stimuli
    for (unsigned int idx=0; idx < m_list_auditory_stimuli.size(); idx++)
        delete(m_list_auditory_stimuli[idx]);

    if (p_warning_tone)
        delete(p_warning_tone);

    // Loaad auditory stimuli audio files
    m_list_auditory_stimuli.clear();

     WavePlayer *p_auditory_stimulus_zero = new WavePlayer();
     p_auditory_stimulus_zero->SetFile(Stimuli(800,2));
     m_list_auditory_stimuli.push_back(p_auditory_stimulus_zero);

      WavePlayer *p_auditory_stimulus_below_PT = new WavePlayer();
     p_auditory_stimulus_below_PT->SetFile(Stimuli(m_perception_threshold_index+m_stimuli_intensity_neg_delta,2));
     m_list_auditory_stimuli.push_back(p_auditory_stimulus_below_PT);

      WavePlayer *p_auditory_stimulus_PT = new WavePlayer();
     p_auditory_stimulus_PT->SetFile(Stimuli(m_perception_threshold_index,2));
     m_list_auditory_stimuli.push_back(p_auditory_stimulus_PT);

      WavePlayer *p_auditory_stimulus_above_PT = new WavePlayer();
     p_auditory_stimulus_above_PT->SetFile(Stimuli(m_perception_threshold_index+m_stimuli_intensity_pos_delta,2));
     m_list_auditory_stimuli.push_back(p_auditory_stimulus_above_PT);
     /*
     p_sound_hearing_check_tone = new WavePlayer();
     p_sound_hearing_check_tone->SetFile(Parameter("SoundhearingcheckTone"));
     */
     p_warning_tone = new WavePlayer();
     p_warning_tone->SetFile(Parameter("WrongstartWarningTone"));

    // stimuli intensity
    m_stimuli_intensity.push_back(-200);
    m_stimuli_intensity.push_back(Stimuli(m_perception_threshold_index+m_stimuli_intensity_neg_delta,1));
    m_stimuli_intensity.push_back(Stimuli(m_perception_threshold_index,1));
    m_stimuli_intensity.push_back(Stimuli(m_perception_threshold_index+m_stimuli_intensity_pos_delta,1));

     // generating sequence of stimulus presentation
     m_stimuli_sequence.clear();

    m_total_number_blocks = ceil((3*m_number_blocks)*(1+(m_zero_intensity_percentage/100)));
    m_total_number_zero_intensity = ceil((float)(m_zero_intensity_percentage/100)*(3*m_number_blocks));
       
      for (unsigned int i=0; i<m_total_number_zero_intensity; i++)
      {
          m_stimuli_sequence.push_back(1);
      }

      for (unsigned int i=0; i<m_number_blocks; i++)
      {
          m_stimuli_sequence.push_back(2);
      }

     for (unsigned int i=0; i<m_number_blocks; i++)
      {
          m_stimuli_sequence.push_back(3);
      }

        for (unsigned int i=0; i<m_number_blocks; i++)
      {
          m_stimuli_sequence.push_back(4);
      }
std::srand(time(0));
    std::random_shuffle ( m_stimuli_sequence.begin(), m_stimuli_sequence.end() );// randomly shuffle zero intensity sequence vector
    //std::random_shuffle ( m_stimuli_sequence.begin(), m_stimuli_sequence.end() );// randomly shuffle zero intensity sequence vector
}

void
ReactionAuditoryTask::StartRun()
{
  // The user has just pressed "Start" (or "Resume")
  bciout << "Hello World!" << endl;
}


void
ReactionAuditoryTask::Process( const GenericSignal& Input, GenericSignal& Output )
{
  Output = Input; // This passes the signal through unmodified.
   mp_input = &Input;

   m_runtime_counter++;
   	if (m_trial_counter >= m_num_stimuli) 
	{
		return;
	}

   if ( m_is_first_trial )
	{
		m_current_state = e_first_trial;
	}

	// keep track of the states
	State("ExperimentState")	= m_current_state;
	State("CurrentTrial")		= m_trial_counter+1;

     switch(m_current_state)
    {
     case e_first_trial:
              {
                  // This state provides instructions for the subject, e.g. Get ready
				m_block_delta++;
                bciout << "e_first trial state  "<< endl;
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
               
                // Present auditory stimuli 
                if (m_block_delta == 1)
                {
                   // mp_text_stimulus->Conceal();
                    mp_text_marker->Conceal();
                    mp_text_stimulus->SetTextColor( m_text_feedback_pre_cue_color );
                    mp_text_stimulus->SetText("Push the button if you heard the sound...");
                    mp_text_stimulus->Present();
                    AppLog << "State: checking the sound ..." << endl;
                    StimulusOn(2);
                } 
                  if (m_block_delta > 5)
                {

                    if ( !IsButtonPressed() )
                    {
                       // mp_text_stimulus->Conceal();
                        m_current_state = e_soundcheck;
                        m_block_delta = 0;
                        StimulusOff(2);
                    }

                    if ( IsButtonPressed() )
                    {
                       // mp_text_stimulus->Conceal();
                        m_current_state = e_cue;
                        m_block_delta = 0;
                        StimulusOff(2);
                    }
                  }
           }
           break;
           ////////////////////////////////////////////////////////////////////////
      case e_cue:
            {
               // Initialization of states
                State("ReactionTime") = 0;
                State("Performance") = 0;
                State("StimulusLevel") = 0;
               
                 m_block_delta++;
                 
               //  p_warning_tone -> Stop();
                 mp_text_stimulus->Conceal();

                 if (m_block_delta == 1)
				{
                   
                    AppLog << "=============================================" << endl;
                    AppLog << "---------------------------------------------" << endl;
                    AppLog << "Trial #" << m_num_trials+1 << endl;
                    AppLog << "---------------------------------------------" << endl;
					AppLog << "State: Presenting cues ..." << endl;
				}

                 // Presntation of + marker
                 if (m_block_delta == 0.5*m_cue_duration_blocks)
                 {
                     VisualMarkerOn();
                 }

                 if(m_block_delta >= m_cue_duration_blocks)
                 {
                       if (IsButtonPressed())
                       {
                            VisualMarkerOff();
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
        case    e_pre_stimuli:
            {
                 m_block_delta++;

                 if (m_block_delta ==1)
                 {
                      m_rand_blocks = RandomNumber(m_wait_min_blocks,m_wait_max_blocks);	// random delay in presenting the auditory stimuli after CueOff
                      AppLog << "State: Waiting for " << ((float)(m_rand_blocks) * m_block_size_msec) << "ms ..." << endl;
                 }

                   if (IsButtonPressed())
                     {
                          VisualMarkerOff();
                         m_current_state = e_false_start;
                         m_block_delta = 0;
                     }

                 // Addition of random waiting time before presentation of auditory stimuli to make sure subject is not expecting a tone!
                 if (m_block_delta >= m_rand_blocks)
                 {
                     m_current_state = e_stimuli;
                     m_block_delta = 0;
                 }

            }
            break;
            ////////////////////////////////////////////////////////////////////
            case  e_stimuli:
                {
                    m_block_delta++;
                    
                    if (m_block_delta ==1)
                    {
                        AppLog << "State: Presenting auditory stimuli ..." << endl;
                        AppLog << "stimuli sequence = "   <<    m_stimuli_sequence[m_num_trials]   <<  endl;
                        AppLog << "auditory file presenting = "   <<    m_stimuli_intensity[m_stimuli_sequence[m_num_trials]-1]   <<  endl;
                        StimulusOn(   m_stimuli_sequence[m_num_trials]  ); // Auditory stimuli onset
                        State("StimulusLevel")        = (unsigned int)(-1*(m_stimuli_intensity[m_stimuli_sequence[m_num_trials]-1]));
                    }

                    if (IsButtonPressed()|| (m_block_delta >= m_stimuli_duration_blocks))
                    {
                        if (IsButtonPressed())
                        { 
                            // if reaction time is normal go to feedback state
                            if (ReactionTime() <= m_max_rt & ReactionTime() >= m_min_rt )
                            {
                            m_pushed_botton_flag = 1;
                            m_reaction_time.push_back(ReactionTime());
                            m_subject_performance.push_back(1);
                            m_stimuli_intensity_trial.push_back(m_stimuli_intensity[m_stimuli_sequence[m_num_trials]-1] );
                           
                            StimulusOff(   m_stimuli_sequence[m_num_trials]  );
                            m_current_state = e_feedback;
                            m_block_delta = 0;
                            State("ReactionTime")       = m_reaction_time[m_num_trials] > 0 ? (unsigned int)(m_reaction_time[m_num_trials]) : 0;
                            State("Performance")        = (unsigned int)(m_subject_performance[m_num_trials]);  
                            }

                            // if reaction time is not normal, go to false start state 
                            else if (ReactionTime() < m_min_rt || ReactionTime() > m_max_rt) 
                            {
                            StimulusOff(   m_stimuli_sequence[m_num_trials]  );
                            m_current_state = e_false_start;
                            m_block_delta = 0;
                            }
                        }

                        // if botton is not pushed, go to feedback state
                        else if (!IsButtonPressed() & m_block_delta!=0)
                        {
                            m_pushed_botton_flag = 0;
                            m_num_unresponded_trial_counter = m_num_unresponded_trial_counter+1;
                            m_reaction_time.push_back(ReactionTime());
                            m_subject_performance.push_back(0);
                            m_stimuli_intensity_trial.push_back(m_stimuli_intensity[m_stimuli_sequence[m_num_trials]-1] );
                           // State("StimulusLevel")        = (unsigned int)(-1*m_stimuli_intensity_trial[m_num_trials]);
                            StimulusOff(   m_stimuli_sequence[m_num_trials]  );
                            m_current_state = e_feedback;
                            m_block_delta = 0;
                            State("ReactionTime")       = m_reaction_time[m_num_trials] > 0 ? (unsigned int)(m_reaction_time[m_num_trials]) : 0;
                            State("Performance")        = (unsigned int)(m_subject_performance[m_num_trials]);  
                        }  
                    }
                }
                break;
                ////////////////////////////////////////////
            case e_feedback:
                {
                m_block_delta++;
                // mp_text_marker->Conceal();
                if (m_block_delta ==1)
                {
                    AppLog << "State: Presenting feedback ..." << endl;
                    VisualMarkerOff(); // +marker disappearance

                    // present "Well done!" statement if botton is pushed and the stimuli wasn't zero intensity
                    if (m_pushed_botton_flag == 1 & m_stimuli_intensity[m_stimuli_sequence[m_num_trials]-1] != -200)
                    { 
                       AppLog << "State: Response time = " << m_reaction_time[m_num_trials] << "ms ..." << endl;
                       mp_text_stimulus->SetTextColor( m_text_feedback_positive_color );
                       m_text_stimulus_string.str("Good job! \n\n Response Time: \n ");
                       m_text_stimulus_string << m_text_stimulus_string.str() << "\t\t\t" << (unsigned int)(m_reaction_time[m_num_trials])  << " ms";
		               mp_text_stimulus->SetText(m_text_stimulus_string.str());
                       mp_text_stimulus->Present();
                    }

                    // present "Try harder!" statement if botton is not pushed and it wasn't zero intensity
                    else if (m_pushed_botton_flag == 0  & m_stimuli_intensity[m_stimuli_sequence[m_num_trials]-1] != -200)
                    {
                        AppLog << "State: trial is missed" << endl;
                        mp_text_stimulus->SetTextColor( m_text_feedback_negative_color );
                        mp_text_stimulus->SetText("Try Harder!");
                        mp_text_stimulus->Present();
                    }
                    // Present "Incorrect" if zero intensity trial but subject pushed botton
                    else if (m_pushed_botton_flag == 1 & m_stimuli_intensity[m_stimuli_sequence[m_num_trials]-1] == -200)
                    {
                        mp_text_stimulus->SetTextColor( m_text_feedback_negative_color );
                        mp_text_stimulus->SetText("Incorrect!");
                        mp_text_stimulus->Present();
                       // p_warning_tone->Play();
                    }
                    // If zeor intensity trial and botton wasn't pushed, just present "no sound played!" 
                    else if (m_pushed_botton_flag == 0 & m_stimuli_intensity[m_stimuli_sequence[m_num_trials]-1] == -200)
                    {
                       // mp_text_stimulus->SetTextColor(m_text_feedback_positive_color);
                       // mp_text_stimulus->SetText("No sound played!");
                       // mp_text_stimulus->Present();
                    }
                }
                
                // present the feedback text for a while and then go to next state
                if (m_block_delta >= m_feedback_blocks)
                {
                   // p_warning_tone->Stop();
                    mp_text_stimulus->Conceal();
                    m_current_state = e_isi;
                    m_block_delta = 0;
                }
                
                }
                break;
                ///////////////////////////////////////////////////////////
                case e_isi:
                {
                 m_block_delta++;

                  //p_warning_tone -> Stop();
                  mp_text_stimulus->Conceal();

                if (m_block_delta ==1)
                {
                      AppLog << "State: Inter Stimulus Interval (ISI) ..." << endl;
                
                if ( m_num_trials == 10 ||  m_num_trials == 20 ||  m_num_trials == 30 ||  m_num_trials == 40 ||  m_num_trials == 50)
{
                      if ( m_num_unresponded_trial_counter > 7)
                        {  AppLog << ".........WARNING!!!........" << endl;
                          AppLog << "..........Patient has not been responding........." << endl;
                          AppLog << ".........." << m_num_unresponded_trial_counter <<" out of 10 last trials is not responded" << endl;
                          m_num_unresponded_trial_counter = 0;
}
}
                       m_num_trials++;
                }


                // wait for isi duration and then go to next state
                if (m_block_delta >= m_isi_duration_blocks)
                {
                    // If already reached the total number of trials, end the experiment and summarize 
                    if (m_num_trials >= m_total_number_blocks)
                    {
                        mp_text_stimulus->SetTextColor( m_text_feedback_pre_cue_color );
                        mp_text_stimulus->SetText("Rest");
                        mp_text_stimulus->Present();
                        AppLog << "==== Finished !!! =====" << endl;
                        State("Running") = 0;
                        SummarizeExperiment();
                    } 
                    else
                    {
                        m_current_state = e_cue;
                        m_block_delta = 0;
                    }
                }

                }
                break;
                /////////////////////////////////////
                case e_false_start:
                    {
                        m_block_delta++;
                        if (m_block_delta ==1)
                        {
                            VisualMarkerOff();
                            mp_text_stimulus->SetTextColor(m_text_feedback_negative_color);
                            mp_text_stimulus->SetText("Incorrect!");
                            mp_text_stimulus->Present();

                           // p_warning_tone->Play();
                        }
                        if (m_block_delta > m_feedback_blocks)
                        {
                            
                            m_current_state = e_cue;
                            m_block_delta = 0;
                        }
                    }
                    break;
                    ///////////////////////////////////////
          }
}

void
ReactionAuditoryTask::StopRun()
{
  bciwarn << "Goodbye World." << endl;
}


void 
ReactionAuditoryTask::StimulusOn(unsigned int idx)
{
    m_list_auditory_stimuli[idx-1]->Play();// idx-1 to compensate for the sequence values starting from 1 and index startingg from 0
	bciout << "auditory stimulus started" << endl; 
}

void 
ReactionAuditoryTask::StimulusOff(unsigned int idx)
{
	m_list_auditory_stimuli[idx-1]->Stop();
	bciout << "auditory stimulus stopped" << endl; 
}

bool
ReactionAuditoryTask::IsButtonPressed()
{
	//Check for keypresses
	bool press = false;

   if(m_use_Digital_input) 
	{
		for( unsigned int i = 0; i < m_block_size; i++ )
		{
			if( State(  Parameter("DigitalInputNum") )( i ) == 1 )
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
ReactionAuditoryTask::VisualMarkerOn()
{
		mp_text_marker->SetText("+");
        mp_text_marker->Present();
}


void
ReactionAuditoryTask::VisualMarkerOff()
{
		//mp_text_marker->SetText("");
        mp_text_marker->Conceal();
}

void
ReactionAuditoryTask::CueImageOn()
{
	if (mp_cue_image)
	{
		mp_cue_image->Present();
	}
}

void
ReactionAuditoryTask::CueImageOff()
{
	if (mp_cue_image)
	{
		mp_cue_image->Conceal();
	}
}

int
ReactionAuditoryTask::RandomNumber(int min, int max)
{
	int random_number = min + m_random_generator.Random() % (max - min);
	
        //bciout << random_number << endl;

	return random_number;
}

float ReactionAuditoryTask::ReactionTime( )
{
	unsigned int offset = 0;
	bool press = false; 
      if(m_use_Digital_input) 
	{
		for( offset = 0; offset < m_block_size; offset++ )
		{
			if( State(  Parameter("DigitalInputNum") )( offset ) == 1 )
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

unsigned int
ReactionAuditoryTask::CalculateStimulusIndex( float attenuation )
{
    float k = (float)(m_num_stimuli-1) / (float)(m_attenuation_min - m_attenuation_max);
    float d = (float)(m_num_stimuli-1) * ((float)m_attenuation_max / (float)(m_attenuation_max - m_attenuation_min));

    float y = attenuation * k + d; 

    int index = (int)(y);

    if (index > m_num_stimuli-1)
        index = m_num_stimuli-1;

    if (index < 0) 
        index = 0;

    return (index);

}


void ReactionAuditoryTask::SummarizeExperiment()
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
	
	AppLog << "Total trials: " << m_num_trials << endl;
    AppLog << "Total trials push botton: " << rt_counter << endl;
    AppLog << "Total missed trials: " << m_reaction_time.size()-rt_counter << endl;
    AppLog << "average reaction time: " << rt_sumation/rt_counter << endl;

    // showing stimulus presented in order
    AppLog << "stimulus level: " << " ";
    for (unsigned int i=0; i<m_stimuli_intensity_trial.size(); i++)
    {
        AppLog <<  m_stimuli_intensity_trial[i] << " ";
    }
    AppLog << endl;

    // subject's performance for each stimulus presented in order
     AppLog << "subject performance: "<<" ";
     for (unsigned int i=0; i<m_subject_performance.size(); i++)
    {
        AppLog <<  m_subject_performance[i] << " ";
    }
     AppLog << endl;

     //showing the unique values of stimulus intensity presented
       m_stimulus_Level_copy = m_stimuli_intensity_trial;
     std::sort(m_stimulus_Level_copy.begin(), m_stimulus_Level_copy.end()); 
     auto last = std::unique(m_stimulus_Level_copy.begin(), m_stimulus_Level_copy.end());
     m_stimulus_Level_copy.erase(last, m_stimulus_Level_copy.end()); 

     AppLog << "unique stimulus level: "<< " " ;
      for (unsigned int i=0; i<m_stimulus_Level_copy.size(); i++)
    {
        AppLog <<  m_stimulus_Level_copy[i] << " ";
    }
      AppLog << endl;

      // calculating the subject's performance% for all the stimuli
      float counter;
      float summation_count;
      //AppLog << "Performance percentage: " << " ";
      for (unsigned int i=0; i<m_stimulus_Level_copy.size(); i++)
      {
           counter = 0;
           summation_count = 0;
          for (unsigned int j=0; j<m_stimuli_intensity_trial.size()-1; j++)
          {
              if (m_stimulus_Level_copy[i] == m_stimuli_intensity_trial[j])
              {
                  counter++;
                  summation_count += m_subject_performance[j];
              }
          }
          m_Pest_percentage_check.push_back(summation_count/counter);
        if (i==0)
            AppLog <<"Performance percentage for catch trials (zero intensity stimulus): " << " = " << m_Pest_percentage_check[i] << "\n";
        if (i==1)
            AppLog <<"Performance percentage for 25% intensity stimulus (softest stimulus): " << " = " << m_Pest_percentage_check[i] << "\n";
        if (i==2)
            AppLog <<"Performance percentage for 50% intensity stimulus: " << " = " << m_Pest_percentage_check[i] << "\n";
        if (i==3)
            AppLog <<"Performance percentage for 95% intensity stimulus (loudest stimulus): " << " = " << m_Pest_percentage_check[i] << "\n";
      }

      AppLog << endl;

}
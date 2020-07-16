#ifndef _P300_Classify_h
#define _P300_Classify_h

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "ap.h"

using namespace std;

void P300_Classify(ap::real_2d_array pscore, 
				   ap::template_1d_array<unsigned short int, true>& StimulusCode,
				   ap::template_1d_array<unsigned short int, true>& StimulusType,
				   ap::template_1d_array<short int, true>& trialnr,
				   int NumberOfSequences,
				   int NumMatrixRows,
				   int NumMatrixColumns,
				   vector<string> TargetDefinitions,
				   vector<double>& result,
				   vector<string>& predicted);
#endif

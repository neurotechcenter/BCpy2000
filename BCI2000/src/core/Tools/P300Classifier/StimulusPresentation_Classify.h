#ifndef _StimulusPresentation_Classify_h
#define _StimulusPresentation_Classify_h

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "ap.h"

using namespace std;

void StimulusPresentation_Classify(ap::real_2d_array pscore,
								   ap::template_1d_array<unsigned short int, true>& StimulusCode,
								   ap::template_1d_array<unsigned short int, true>& StimulusType,
								   ap::template_1d_array<short int, true>& trialnr,
								   int NumberOfSequences,
								   int NumberOfChoices,
								   vector<string> TargetDefinitions,
								   vector<double>& result,
								   vector<string>& predicted);
#endif

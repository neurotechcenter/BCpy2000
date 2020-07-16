#ifndef _GetScore_h
#define _GetScore_h

#include <iostream>
#include <algorithm>
#include <vector>
#include "SWLDA.h"
#include "ap.h"
#include "blas.h"

using namespace std;

bool GetScore(ap::template_2d_array<float,true>& Responses,
              ap::template_1d_array<unsigned short int, true>& Code,
              parameters tMUD,
              ap::template_1d_array<short int,true>& trialnr,
              ap::template_1d_array<double,true>& windowlen,
              int numchannels,
              int NumberOfSequences,
              int NumberOfChoices,
              int mode,
              ap::real_2d_array &pscore);
#endif

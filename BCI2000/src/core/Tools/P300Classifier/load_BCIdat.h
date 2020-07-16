#ifndef _load_BCIdat_h
#define _load_BCIdat_h

#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include <algorithm>
#include "BCI2000FileReader.h"
#include "ap.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////
/// \struct prm
/// This structure contains some of the parameters provided in the BCI2000 data file.
struct prm
{
	/// Total number of channels used when the data was recorded.
	int SoftwareCh;
	/// Sampling rate.
	int SamplingRate;
	/// Number of sequences.
	int NumberOfSequences;
	/// Number of matrix rows when P3Speller Task.
	int NumMatrixRows;
	/// Number of columns when P3Speller Task.
	int NumMatrixColumns;
};

///////////////////////////////////////////////////////////////////////////////
/// \struct ste
/// This structure contains some of the states provided in the BCI2000 data file.
struct ste
{
	/// Stimulus code.
	ap::template_1d_array<unsigned short int, true> StimulusCode;
	/// Stimulus type.
	ap::template_1d_array<unsigned short int, true> StimulusType;
	/// Flashing.
	ap::template_1d_array<unsigned char, true> Flashing;
	/// trialnr.
	ap::template_1d_array<short int, true> trialnr;
	/// Target definitions.
	vector<string> TargetDefinitions;
};

void load_BCIdat(const string& filename, int mode, ap::template_2d_array<float, true>& sig, prm &parms, ste &state);
#endif

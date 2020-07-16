#ifndef _ReadIniParameters_h
#define _ReadIniParameters_h

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include "CDataFile.h"

///////////////////////////////////////////////////////////////////////////////
/// \struct InitialParameter
/// This structure contains all the initial parameters to train or test the
/// BCI 2000 data files using the SWLDA (Stepwise Linear Discriminant Analysis).

struct InitialParameter
{
	string dir;
	/// Decimation Frequency. Used to specify the temporal decimation frequency
	/// of the data in Hz. Set to the sampling rate for no decimation. The lower
	/// the Decimation Frequency, the less original data retained for processing.
	/// Default = 20.
	int Decimation_Frequency;
	/// Spatial Filter. Raw is no spatial filter applied to the data and CAR is a
	/// common average reference filter using all the channels contained in the data
	/// file. The RAW & CAR option will evaluate both types simultaneously for
	/// comparison purposes. Default = 2.
	/// - Raw && CAR = 1
	/// - Raw Only = 2
	/// - CAR only = 3
	int SF;
	/// Maximum p-value for a predictor to be added. Default = 0.1000.
	float penter;
	/// Minimum p-value for a predictor to be removed. Default = 0.1500.
	float premove;
	/// Maximum model features. Used to specify the maximum number of features to
	/// be kept in the SWLDA model. Only a single value can be entered for
	/// evaluation. Default = 60.
	int maxiter;
	/// Channel Set. Used to specify the channels set that will be used to create
	/// feature weights. The specified channels must be a subset of the channels
	/// contained in the training data file.
	vector<unsigned int> channel_set;
	/// Response Window. Used to specify the \e begin and \e end time points in
	/// milliseconds (ms) following the stimuli collected for the analysis. This
	/// automatically converted into samples according to the sampling rate of the
	/// data (rounded). Only a single data window can be entered and will be evaluated.
	/// Negative values are accepted and \e begin < \e end. Default [0 800].
	vector<float> windlen;
};

///////////////////////////////////////////////////////////////////////////////
/// Stores the Type value <T>  of a string into a STL vector. Therefore
/// the STL vector is resized anyway.
/// @param [in]  inputvector
/// @param [out] outputvector
///////////////////////////////////////////////////////////////////////////////
template <class T>
void read_vector(string inputvector, vector<T> &outputvector)
{
const char *inputline = inputvector.c_str();
int length = static_cast<int>(inputvector.length());
int num_samples = 0;
int idx         = 0;
outputvector.resize(length);

while (idx < length)
{
	while ((inputline[idx] == ' ') && (idx < length)) idx++;
    if (inputline[idx] == '\0') break;
      outputvector[num_samples] = static_cast<T>(atoi(&inputline[idx]));
    while ((inputline[idx] != ' ') && (idx < length)) idx++;
      num_samples++;
}
outputvector.resize(num_samples);
}
bool ReadIniParameters(string szFile, InitialParameter &IniParam);

bool CheckCfgParameterKeyExist(CDataFile CfgFile, string key, string section);
#endif

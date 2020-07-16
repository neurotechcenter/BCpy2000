#include "Check_Consistency_Files.h"
#include "BCI2000FileReader.h"
#include "BCIStream.h"
#include <stdlib.h>
using namespace std;
///////////////////////////////////////////////////////////////////
/// Check if the provided files are consistent in terms of their
/// parameters and states
/// @param [in]		fPathArr		Array containing all the paths of the files 
/// @param [out]	numSamples		Total number of samples 
/// @param [out]	numChannels		Total number of channels  
/// \author Cristhian Potes
/// \date July 12, 2009

int Check_Consistency_Files(vector<string> fPathArr, long long &numSamples, int &numChannels)
{
///////////////////////////////////////////////////////////////////////////////
// Section: Define variables
size_t files;
int i, mode;
numSamples = 0;
vector<int> channelsInFile;
vector<long long> samplesInFile;
vector<int> SamplingRate;
vector<int> NumberOfSequences;
vector<int> NumMatrixRows;
vector<int> NumMatrixColumns;
vector<int> NumStimuli;
vector<int> InterpretMode;
vector<string> ApplicationFilterChain;
vector<string> Experiment;

///////////////////////////////////////////////////////////////////////////////
// Section: Open BCI2000 File, check if the file is open, and check compatibility 
// among the files
BCI2000FileReader CurrentFile;
Experiment.push_back("P3SpellerTask");
Experiment.push_back("StimulusPresentationTask");
// Check if the BCI2000 file exists
for (files=0; files<fPathArr.size(); files++)
{
	CurrentFile.Open(fPathArr[files].c_str()); 
	if( !CurrentFile.IsOpen() )
        CurrentFile.Open( ( fPathArr[files] + ".dat" ).c_str() );
	if( !CurrentFile.IsOpen() )
      {
        throw bcierr << "Could not open \"" << fPathArr[files].c_str() << "\" as a BCI2000 data file.";
      }
	// Check whether files are compatible.
	const StateList* statelist = CurrentFile.States();
	for (i=0; i<statelist->Size(); i++ )
        if( !CurrentFile.States()->Exists((*statelist).ByIndex(i).Name()))
		{
      throw bcierr << "Incompatible state information across input files.";
   	}

	channelsInFile.push_back(CurrentFile.SignalProperties().Channels());
	samplesInFile.push_back(CurrentFile.NumSamples());
	
	// Check the Interpret Mode
	if (CurrentFile.Parameters()->Exists("InterpretMode"))
	{
		InterpretMode.push_back(CurrentFile.Parameter("InterpretMode"));
		if (*InterpretMode.rbegin() == 0)
		{
			throw bcierr << "Mode of data file " << fPathArr[files] << " is not supported";
		}
	}
	else
	{
		throw bcierr << "Interpret mode of file " << fPathArr[files] << " does not exist";
	}

	// Check the Application Filter Chain
	if (CurrentFile.Parameters()->Exists("ApplicationFilterChain"))
		ApplicationFilterChain.push_back(CurrentFile.Parameter("ApplicationFilterChain")(1));
	else
	{
		throw bcierr << "Application Filter Chain of file " << fPathArr[files] << " does not exist";
	}

	// Check the Sampling Rate
	if (CurrentFile.Parameters()->Exists("SamplingRate"))
    SamplingRate.push_back(static_cast<int>(CurrentFile.SamplingRate()));
	else
	{
		throw bcierr << "The sampling rate of file " << fPathArr[files] << " does not exist";
	}

	// Check the NumMatrixRows
	if (CurrentFile.Parameters()->Exists("NumMatrixRows"))
		NumMatrixRows.push_back(CurrentFile.Parameter("NumMatrixRows"));
	else
	{
		if (ApplicationFilterChain[files] == Experiment[0])
		{
			throw bcierr << "The number of rows for the P300 Speller paradigm of file " << fPathArr[files] << " does not exist";
		}
	}

	// Check the NumMatrixColumns
	if (CurrentFile.Parameters()->Exists("NumMatrixColumns"))
		NumMatrixColumns.push_back(CurrentFile.Parameter("NumMatrixColumns"));
	else
	{
		if (ApplicationFilterChain[files] == Experiment[0])
		{
			throw bcierr << "The number of columns for the P300 Speller paradigm of file " << fPathArr[files] << " does not exist";
		}
	}

	// Check the number of stimulus in the Stimulus Presentation Task
	if (CurrentFile.Parameters()->Exists("Stimuli"))
	{
		ParamRef parameter = CurrentFile.Parameter("Stimuli");
		NumStimuli.push_back(parameter->NumValues());
	}
	else
	{
		if (ApplicationFilterChain[files] == Experiment[1])
      throw bcierr << "There is no stimuli for the Stimulus Presentation Task of file " << fPathArr[files];
	}


	if (files>0)
	{
		if (channelsInFile[files-1] != channelsInFile[files])
		{	
			throw bcierr << "All input files must have the same numbers of channels.";
		}
		if (SamplingRate[files-1] != SamplingRate[files])
		{	
			throw bcierr << "All input files must have the same sampling rate.";
  	}
		if (InterpretMode[files-1] != InterpretMode[files])
		{	
			throw bcierr << "All input files must have the same Interpret Mode.";
		}
		if (ApplicationFilterChain[files-1] != ApplicationFilterChain[files])
		{	
			throw bcierr << "All input files must have been recorded from the same experiment.";
		}
		if (ApplicationFilterChain[files] == Experiment[0])
			if (NumMatrixRows[files-1] != NumMatrixRows[files])
			{	
				throw bcierr << "All input files must be recorded under the same paradigm.";
			}
		if (ApplicationFilterChain[files] == Experiment[0])
			if (NumMatrixColumns[files-1] != NumMatrixColumns[files])
			{	
				throw bcierr << "All input files must be recorded under the same paradigm.";
  		}
		if (ApplicationFilterChain[files] == Experiment[1])
			if (NumStimuli[files-1] != NumStimuli[files])
			{	
				throw bcierr << "All input files must have the same number of stimulus.";
			}

	}
	numSamples += samplesInFile[files];
}
numChannels = channelsInFile[0];
// Set mode according to the Interpret Mode and Application Filter Chain parameters
// mode = 1		->		P3SpellerTask/Online Free Mode
// mode = 2		->		P3SpellerTask/Copy Mode
// mode = 3		->		StimulusPresentationTask/Online Free Mode
// mode = 4		->		StimulusPresentationTask/Copy Mode

if ((Experiment[0] == ApplicationFilterChain[0]) && InterpretMode[0] == 1)
	mode = 1;
if ((Experiment[0] == ApplicationFilterChain[0]) && InterpretMode[0] == 2)
	mode = 2;
if ((Experiment[1] == ApplicationFilterChain[0]) && InterpretMode[0] == 1)
	mode = 3;
if ((Experiment[1] == ApplicationFilterChain[0]) && InterpretMode[0] == 2)
	mode = 4;
return mode;
}

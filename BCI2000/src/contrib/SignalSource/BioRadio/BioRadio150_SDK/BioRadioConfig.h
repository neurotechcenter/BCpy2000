//---------------------------------------------------------------------------
#ifndef BioRadioConfigH
#define BioRadioConfigH      
//---------------------------------------------------------------------------
#ifndef MAXPATH
#define MAXPATH 260
#endif
//---------------------------------------------------------------------------
#define TOTAL_DAQ_INPUTS 14
#define FAST_DAQ_INPUTS 10
#define SLOW_DAQ_INPUTS 4
#define MAIN_DAQ_INPUTS 8
#define AUX_DAQ_INPUTS 6
//---------------------------------------------------------------------------
// Structs to 'mirror' BioRadio 150 Config object --
// to allow for config programming without using an .ini file.     
//---------------------------------------------------------------------------
struct TBioRadioChannel{
	bool Enabled;
	int  RangeIndex;
	char InputName[25];
	char Units[25];
	int  BitResolution;
	double SampleRate;
	double UpperScale;
	double LowerScale;
	double ADCUpper;
	double ADCLower;
	int InputType;
};  
//---------------------------------------------------------------------------
struct TBioRadioConfig{
	char Version[10];
	char Name[MAXPATH];
	char Description[255];

  int SystemSampleRate;
  int SystemBitResolution;
  
	TBioRadioChannel FastInputs[FAST_DAQ_INPUTS];
  bool SlowInputsEnabledStates[SLOW_DAQ_INPUTS];
};
//---------------------------------------------------------------------------
#endif

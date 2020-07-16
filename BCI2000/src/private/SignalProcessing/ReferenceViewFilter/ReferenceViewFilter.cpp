////////////////////////////////////////////////////////////////////////////////
// Authors:
// Description: ReferenceViewFilter implementation
////////////////////////////////////////////////////////////////////////////////

#include "ReferenceViewFilter.h"
#include "BCIStream.h"
#include <vector>
using namespace std;
RegisterFilter( ReferenceViewFilter, 2.A );
#define PARAM( x ) auto x = Parameter( #x );
#define TRUE_PARAM( x ) auto True_##x = ActualParameter( #x );
    

//destructor
ReferenceViewFilter::~ReferenceViewFilter()
{
  Halt();
}

//constructor
ReferenceViewFilter::ReferenceViewFilter()
//specify the address for visualization
: mRefToChannelsVis("P5FLT"),
mRefToRestVis("P4FLT"),
mBipoleRefVis("P3FLT")
{
//set the parameter
 BEGIN_PARAMETER_DEFINITIONS

   "Filtering:IIRPeakFilter matrix FrequencyCenter= "
   "{ Filter1 Filter2 Filter3} "
   "{Fcenter}"
     "60 120 180",
	 
	 "Filtering:IIRPeakFilter matrix BandWidth= "
   "{ Filter1 Filter2 Filter3} "
   "{BandWith}"
     "0.0001 0.0001 0.0001",
	 "Filtering:IIRPeakFilter double FrequencyDelta= 5 5 0 % ",
	 "Filtering:IIRPeakFilter double BandWidthDelta= 0.0001 0.0001 0 1 ",
	 "Filtering list RefChList= 0"
	  "// reference channel for each amp, 0 or empty for unipolar measurement",
	 "Filtering list CARListExclude= 0"
	  "// channels will not be used as reference channels for each amp, 0 or empty for unipolar measurement",
	  "Filtering list BadChList= 0"
	  "// channels will be turne off ",
	 "Filtering  matrix BipolarMontage= "
     "{ TG1-TG2 TG2-TG3 TG3-TG4 TG5-TG6 TG6-TG7 TG7-TG8 TG9-TG10 TG10-TG11 TG11-TG12 TG13-TG14 TG14-TG15 TG15-TG16 TG17-TG18 TG18-TG19 TG19-TG20 TG21-TG22 TG22-TG23 TG23-TG24 TG25-TG26 TG26-TG27 TG27-TG28 TG28-TG29 TG29-TG30 TG30-TG31 TG32-TG33 TG33-TG34 TG34-TG35 TG35-TG36 TG36-TG37 TG37-TG38 TG39-TG40 TG40-TG41 TG41-TG42 TG42-TG43 TG43-TG44 TG44-TG45 TG46-TG47 TG47-TG48 TG48-TG49 TG49-TG50 TG50-TG51 TG51-TG52 TG53-TG54 TG54-TG55 TG55-TG56 TG56-TG57 TG57-TG58 TG58-TG59 TG60-TG61 TG61-TG62 TG62-TG63 TG63-TG64 TG64-TG65 TG65-TG66 FG3-FG4 FG4-FG5 FG5-FG6 FG6-FG7 FG7-FG8 FG11-FG12 FG12-FG13 FG13-FG14 FG14-FG15 FG15-FG16 FG19-FG20 FG20-FG21 FG21-FG22 FG22-FG23 FG23-FG24 FG27-FG28 FG28-FG29 FG29-FG30 FG30-FG31 FG31-FG32 FG35-FG36 FG36-FG37 FG37-FG38 FG38-FG39 FG39-FG40 FG43-FG44 FG44-FG45 FG45-FG46 FG46-FG47 FG47-FG48 FG51-FG52 FG52-FG53 FG53-FG54 FG54-FG55 FG55-FG56 FG59-FG60 FG60-FG61 FG61-FG62 FG62-FG63 FG63-FG64 SS1-SS2 SS2-SS3 SS3-SS4 SS4-SS5 SS5-SS6 MS1-MS2 MS2-MS3 MS3-MS4 MS4-MS5 MS5-MS6 IS1-IS2 IS2-IS3 IS3-IS4 OS1-OS2 OS2-OS3 OS3-OS4 PS1-PS2 PS2-PS3 PS3-PS4 GND2-GND3 GND3-GND4 } "
     " { CH+ CH- } "
	 "1 2 2 3 3 4 5 6 6 7 7 8 9 10 10 11 11 12 13 14 14 15 15 16 17 18 18 19 19 20 21 22 22 23 23 24 25 26 26 27 27 28 28 29 29 30 30 31 32 33 33 34 34 35 35 36 36 37 37 38 39 40 40 41 41 42 42 43 43 44 44 45 46 47 47 48 48 49 49 50 50 51 51 52 53 54 54 55 55 56 56 57 57 58 58 59 60 61 61 62 62 63 63 64 64 65 65 66 67 68 68 69 69 70 70 71 71 72 73 74 74 75 75 76 76 77 77 78 79 80 80 81 81 82 82 83 83 84 85 86 86 87 87 88 88 89 89 90 91 92 92 93 93 94 94 95 95 96 97 98 98 99 99 100 100 101 101 102 103 104 104 105 105 106 106 107 107 108 109 110 110 111 111 112 112 113 113 114 115 116 116 117 117 118 118 119 119 120 121 122 122 123 123 124 124 125 125 126 127 128 128 129 129 130 131 132 132 133 133 134 135 136 136 137 137 138 139 140 140 141",


 END_PARAMETER_DEFINITIONS
  //set the pointer to null
  mpFcenter = NULL;
  mpBandWidth = NULL;
  mpFilter =NULL;
  mpDataOutput = NULL;
  ////////
  mpFcenterL = NULL;
  mpBandWidthL = NULL;
  mpFilterL = NULL;
  mpDataOutputL = NULL;
  ///
  mpFcenterR = NULL;
  mpBandWidthR = NULL;
  mpFilterR = NULL;
  mpDataOutputR = NULL;


}



//check all the parameters
void ReferenceViewFilter::Preflight( const SignalProperties& Input, SignalProperties& Output ) const
{
   Parameter("FrequencyDelta");
   Parameter("BandWidthDelta");
   PARAM(RefChList);
   PARAM( CARListExclude );
   Parameter("BipolarMontage");
   PARAM(ChannelNames);
   PARAM(BadChList);
   int SamplingRate = Parameter("SamplingRate");
   ParamRef FrequencyCenter = Parameter("FrequencyCenter");
   ParamRef BandWidth = Parameter("BandWidth");
   //read the # of filters
   int NumFilter  = FrequencyCenter->NumRows();
   for (int index = 0; index < NumFilter; index++)
   {
	   //check if the frequency center and the bandwith is legit 
	   if (FrequencyCenter(index,0)/(SamplingRate/2.0)<=0 || FrequencyCenter(index,0)/(SamplingRate/2.0)>=1 )
		   bcierr <<  "The frequency Wo must be within 0 and 1" << endl;
	   if (BandWidth(index,0)<=0 || BandWidth(index,0)>=1)
		   bcierr <<  "The bandwidth BW must be within 0 and 1" << endl;
   }
   //set up the output property
  Output = Input; 
}


void
ReferenceViewFilter::Initialize( const SignalProperties& Input, const SignalProperties& Output )
{
   ParamRef FrequencyCenter = Parameter("FrequencyCenter");
   ParamRef BandWidth = Parameter("BandWidth");
   mpSamplingRate = Parameter("SamplingRate");
   mpNumFilter  = FrequencyCenter->NumRows();
   mDeltaF=Parameter("FrequencyDelta");
    mDeltaB=Parameter("BandWidthDelta");
   //delete the old data
   if (mpFcenter != NULL) delete[]mpFcenter;
   if (mpBandWidth != NULL) delete[]mpBandWidth;
   if (mpFilter != NULL) delete[]mpFilter;
   if (mpDataOutput != NULL) delete[]mpDataOutput;
   ////////
   if (mpFcenterL != NULL) delete[]mpFcenterL;
   if (mpBandWidthL != NULL) delete[]mpBandWidthL;
   if (mpFilterL != NULL) delete[]mpFilterL;
   if (mpDataOutputL != NULL) delete[]mpDataOutputL;
   ///////
   if (mpFcenterR != NULL) delete[]mpFcenterR;
   if (mpBandWidthR != NULL) delete[]mpBandWidthR;
   if (mpFilterR != NULL) delete[]mpFilterR;
   if (mpDataOutputR != NULL) delete[]mpDataOutputR;

   //initilize a new array
   mpFcenter = new double[mpNumFilter];
   mpBandWidth = new double[mpNumFilter];
   mpFilter = new IIRPeakFilter[mpNumFilter];
   mpDataOutput = new GenericSignal[mpNumFilter];
   //set up the parameters for filters 
   for (int index_filter = 0; index_filter < mpNumFilter; index_filter++)
   {
	   mpFcenter[index_filter] = FrequencyCenter(index_filter,0);
	   mpBandWidth[index_filter] = BandWidth(index_filter,0);
	  
	   mpFilter[index_filter].SetSamplingRate(mpSamplingRate);
	   mpFilter[index_filter].SetFcenter(mpFcenter[index_filter]);
	   mpFilter[index_filter].SetBandWidth(mpBandWidth[index_filter]);
	   mpFilter[index_filter].Initialize(Input.Channels());
	   mpDataOutput[index_filter].SetProperties(Input);
   }
   //initial a new array
   mpFcenterL = new double[mpNumFilter];
   mpBandWidthL = new double[mpNumFilter];
   mpFilterL = new IIRPeakFilter[mpNumFilter];
   mpDataOutputL = new GenericSignal[mpNumFilter];
   //set up the parameter for the filter to the left 
   for (int index_filter = 0; index_filter < mpNumFilter; index_filter++)
   {
	   mpFcenterL[index_filter] = FrequencyCenter(index_filter,0) - mDeltaF;
	   mpBandWidthL[index_filter] = mDeltaB;
	  
	   mpFilterL[index_filter].SetSamplingRate(mpSamplingRate);
	   mpFilterL[index_filter].SetFcenter(mpFcenterL[index_filter]);
	   mpFilterL[index_filter].SetBandWidth(mpBandWidthL[index_filter]);
	   mpFilterL[index_filter].Initialize(Input.Channels());
	   mpDataOutputL[index_filter].SetProperties(Input);
   }
   //initial a new array
   mpFcenterR = new double[mpNumFilter];
   mpBandWidthR = new double[mpNumFilter];
   mpFilterR = new IIRPeakFilter[mpNumFilter];
   mpDataOutputR = new GenericSignal[mpNumFilter];
   //set up the parameter for the filter to the left 
   for (int index_filter = 0; index_filter < mpNumFilter; index_filter++)
   {
	   mpFcenterR[index_filter] = FrequencyCenter(index_filter,0) + mDeltaF;
	   mpBandWidthR[index_filter] = mDeltaB;
	  
	   mpFilterR[index_filter].SetSamplingRate(mpSamplingRate);
	   mpFilterR[index_filter].SetFcenter(mpFcenterR[index_filter]);
	   mpFilterR[index_filter].SetBandWidth(mpBandWidthR[index_filter]);
	   mpFilterR[index_filter].Initialize(Input.Channels());
	   mpDataOutputR[index_filter].SetProperties(Input);
   }


  //read channel names 
  PARAM(ChannelNames);
  //check if the channel labels are empty
  if (ChannelNames->NumValues()!=0)
  {
	  mUserChannelLabels.clear();
	  //read the input channel names 
	  for (int ch = 0; ch < ChannelNames->NumValues(); ch++)
		  mUserChannelLabels.push_back(ChannelNames(ch));	  
  }
	//reference to a list of channels
	PARAM( RefChList );
	mListRefChannels.clear();
	//read the referenced channels 
	for (int i = 0; i < RefChList->NumValues(); i++)
		mListRefChannels.push_back(RefChList(i)-1);
	//set up the property for the referenced channel 
    SignalProperties RefToChannelsProp = Input;
	//set up the window title 
	String WindowName = "Source Signal Referenced (";
	for (int ch = 0; ch < mListRefChannels.size(); ch++)
		WindowName+=(" " + mUserChannelLabels[ch] +"("+to_string(ch+1)+")");
	WindowName+=" )";
	RefToChannelsProp.SetName(WindowName);
	//set channel labels 
	if (mUserChannelLabels.size()==141)
	{
		for (int ch = 0; ch < mUserChannelLabels.size(); ch++)
			RefToChannelsProp.ChannelLabels()[ch] = mUserChannelLabels[ch]+"("+to_string(ch+1)+")" ;
	}
	mRefToChannelsSig.SetProperties(RefToChannelsProp);
	mRefToChannelsVis.Send(mRefToChannelsSig.Properties())
		            .Send(mRefToChannelsSig);
    //reference to the rest of channels 
	PARAM( CARListExclude );
	mListNotRefChannels.clear();
	//read the channels that will not be used as referenced channels 
	for (int i = 0; i < CARListExclude->NumValues(); i++)
		 mListNotRefChannels.push_back(CARListExclude(i)-1);
	mRestRefChannels.clear();
	//
	for (int ch = 0; ch < Input.Channels(); ch++)
	{
		//if find the channel is not in list
		if (std::find(mListNotRefChannels.begin(),mListNotRefChannels.end(),ch)==mListNotRefChannels.end())
			mRestRefChannels.push_back(ch);
	}
	//create the visualization signal
	SignalProperties RefToRestProp = Input;
	RefToRestProp.SetName("Source Signal Referenced (CAR)");
	//set channel labels 
	if ( mUserChannelLabels.size() == 141)
	{
		for (int ch = 0; ch < mUserChannelLabels.size(); ch++)
			RefToRestProp.ChannelLabels()[ch] = mUserChannelLabels[ch]+"("+to_string(ch+1)+")" ;
	}
	mRefToRestSig.SetProperties(RefToRestProp);
	mRefToRestVis.Send(mRefToRestSig.Properties())
		        .Send(mRefToRestSig);
	//create the reference signal
	SignalProperties RefChannelsProp = Input;
	RefChannelsProp.SetChannels(1);
	mRefChannelsSig.SetProperties(RefChannelsProp);

	//bipole reference 
	ParamRef BipolarMontage = Parameter("BipolarMontage");
	int NumMontage = BipolarMontage->NumRows();
	mCHplus.clear();
	mCHminor.clear();
	//read the bipolor referenced channels 
	for (int index_row = 0; index_row < NumMontage; index_row++)
	{
		mCHplus.push_back(BipolarMontage(index_row, 0)-1);
		mCHminor.push_back(BipolarMontage(index_row, 1)-1);
	}
	//set up the window title
	SignalProperties BipoleRefProp = Input;
	BipoleRefProp.SetChannels(NumMontage)
	             .SetName("Source Signal Referenced (Bipolor)");
	//check if there are enought channels 
	if (NumMontage == 115)
	{
		//if there are enought channels set the channel labels 
		for (int ch = 0; ch < 115; ch++)
			BipoleRefProp.ChannelLabels()[ch] = BipolarMontage->RowLabels()[ch] + "("+to_string(mCHplus[ch]+1)
			+"-"+to_string(mCHminor[ch]+1)+")";
	}
	//if not set the channel labels as channel # 
	else
	{
		for (int ch = 0; ch < NumMontage; ch++)
		{
			string Ch_Lable = "Ch "+to_string(mCHplus[ch]+1) + "-" + to_string(mCHminor[ch]+1); 
			BipoleRefProp.ChannelLabels()[ch] = Ch_Lable;
		}
	}
	mBipoleRefSig.SetProperties(BipoleRefProp);
	mBipoleRefVis.Send(mBipoleRefSig.Properties());
	mBipoleRefVis.Send(mBipoleRefSig);
	//read the channel that will not be displayed 
	mChannelWillTurnOff.clear();
	PARAM(BadChList);
	for (int ch = 0; ch < BadChList->NumValues(); ch++)
		mChannelWillTurnOff.push_back(BadChList(ch)-1);
	mBipolerChannelWillTurnOff.clear();
	//find the turn off channels for bipoler 
	for (int ch = 0; ch < mCHplus.size(); ch++)
	{
		//if find this channel in the turn off list
		if (std::find(mChannelWillTurnOff.begin(),mChannelWillTurnOff.end(),mCHplus[ch])!=mChannelWillTurnOff.end())
			mBipolerChannelWillTurnOff.insert(ch);
		if (std::find(mChannelWillTurnOff.begin(),mChannelWillTurnOff.end(),mCHminor[ch])!=mChannelWillTurnOff.end())
			mBipolerChannelWillTurnOff.insert(ch);
	}
	
}

void ReferenceViewFilter::StartRun()
{
}


void ReferenceViewFilter::Process( const GenericSignal& Input, GenericSignal& Output )
{
	//process the input signal and store it 
	for (int idx_filter=0; idx_filter<mpNumFilter; idx_filter++) 
		mpFilter[idx_filter].Process(Input,mpDataOutput[idx_filter]); 
	for (int idx_filter=0; idx_filter<mpNumFilter; idx_filter++) 
		mpFilterL[idx_filter].Process(Input,mpDataOutputL[idx_filter]); 
	for (int idx_filter=0; idx_filter<mpNumFilter; idx_filter++) 
		mpFilterR[idx_filter].Process(Input,mpDataOutputR[idx_filter]); 
	//initialize all elements in the output to zero 
	for( int idx_channel = 0; idx_channel < Input.Channels(); idx_channel++ )
	{
		for( int idx_sample = 0; idx_sample < Output.Elements(); idx_sample++ )
		       Output( idx_channel , idx_sample ) =0;
    }
	//combine the output from three filters
    for (int idx_filter=0; idx_filter<mpNumFilter; idx_filter++) 
    {
		for( int idx_channel = 0; idx_channel < Input.Channels(); idx_channel++ )
		{
		   for( int idx_sample = 0; idx_sample < Output.Elements(); idx_sample++ )
		       Output( idx_channel , idx_sample ) += 
			   abs(mpDataOutput[idx_filter]( idx_channel, idx_sample )) / 
			   ( (  abs(mpDataOutputL[idx_filter]( idx_channel, idx_sample )) +
			   abs(mpDataOutputR[idx_filter]( idx_channel, idx_sample ))    )  /2  );
        }
    }
	//reference to a list of channels
	for (int el = 0; el < Input.Elements(); el++)
		mRefChannelsSig(0,el) = 0;
	//calculate the referenced channel
	for (int ch = 0; ch < mListRefChannels.size(); ch++)
	{
		for (int el = 0; el < Input.Elements(); el++)
			mRefChannelsSig(0,el)+=Input(mListRefChannels[ch],el);
	}
	//calculate the avg value for the referenced channel 
	if (mListRefChannels.size()>0)
	{
		for (int el = 0; el < Input.Elements(); el++)
			mRefChannelsSig(0,el)/=mListRefChannels.size();
	}
	//rereference input channels 
	for (int ch = 0; ch < Input.Channels(); ch++)
	{
		for (int el = 0; el < Input.Elements(); el++)	
			mRefToChannelsSig(ch,el) = Input(ch,el) - mRefChannelsSig(0,el);	
	}
	//turn off bad channels 
	for (int ch = 0; ch < mChannelWillTurnOff.size() ; ch++)
	{
		for (int el = 0; el < Input.Elements(); el++)
			mRefToChannelsSig(mChannelWillTurnOff[ch],el) =0;
	}
	//send the signal 
	mRefToChannelsVis.Send(mRefToChannelsSig);
    //reference to the rest of channels
	for (int el = 0; el < Input.Elements(); el++)
		mRefChannelsSig(0,el) = 0;
	//accumulate all channels except bad channels 
	for (int ch = 0; ch < mRestRefChannels.size(); ch++)
	{
		for (int el = 0; el < Input.Elements(); el++)
				mRefChannelsSig(0,el)+=Input(mRestRefChannels[ch],el);	
	}
	//calculate common avg
	if (Input.Channels()-mListNotRefChannels.size()>0)
	{
		for (int el = 0; el < mRefChannelsSig.Elements(); el++)
			mRefChannelsSig(0,el)/=(Input.Channels()-mListNotRefChannels.size());
	}
	//rereference all channels  
	for (int ch = 0; ch < Input.Channels(); ch++)
	{
		for (int el = 0	; el < Input.Elements(); el++)
			mRefToRestSig(ch,el) = Input(ch,el) - mRefChannelsSig(0,el);
	}
	//tunr off the bad channels 
	for (int ch = 0; ch < mChannelWillTurnOff.size() ; ch++)
	{
		for (int el = 0; el < Input.Elements(); el++)
			mRefToRestSig(mChannelWillTurnOff[ch],el) =0;
	}
	mRefToRestVis.Send(mRefToRestSig);
	//bipole reference
	for (int ch = 0; ch < mBipoleRefSig.Channels(); ch++)
	{
		for (int el = 0; el < mBipoleRefSig.Elements(); el++)
			mBipoleRefSig(ch,el) = Input(mCHplus[ch],el) - Input(mCHminor[ch],el);
	}
	//turn off bad channels 
	for (set<int>::iterator itr = mBipolerChannelWillTurnOff.begin(); itr != mBipolerChannelWillTurnOff.end(); itr++)
	{
		for (int el = 0; el < mBipoleRefSig.Elements(); el++)
			mBipoleRefSig(*itr,el) = 0;
	}
	mBipoleRefVis.Send(mBipoleRefSig);

}

void ReferenceViewFilter::StopRun()
{
}

void ReferenceViewFilter::Halt()
{
}


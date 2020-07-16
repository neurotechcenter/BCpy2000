////////////////////////////////////////////////////////////////////////////////
// Authors: Zhiyang Xu
// Description: StimResponseFilter implementation
////////////////////////////////////////////////////////////////////////////////



#include "PCHIncludes.h"
#pragma hdrstop

#include "StimResponseFilter.h"
#include "BCIStream.h"

#include "IIRBandpass.h"
#include "FilterDesign.h"

#include <numeric>
#include <math.h>



#include "PCHIncludes.h"
#pragma hdrstop
#include <map>
#include <algorithm>
#include <limits>
#include "defines.h"
#include "Color.h"
#include "Label.h"
#include <vector>
#include "PrecisionTime.h"

using namespace std;
int DownSampleRate =0;

RegisterFilter( StimResponseFilter, 2.B );
#define PARAM( x ) auto x = Parameter( #x );
#define TRUE_PARAM( x ) auto True_##x = ActualParameter( #x );

//take the specifcation of the filter and design it
void StimResponseFilter::DesignFilter( SignalProperties& Signal,
                           Real& outGain,
                           ComplexVector& outZeros,
                           ComplexVector& outPoles,
						   FilterParameters mparameter
						   ) const
{
  typedef Ratpoly<FilterDesign::Complex> TransferFunction;
  TransferFunction tf( 1.0 );
  Real gain = 1.0;

  // High pass
  Real hpCorner = mparameter.HighPassCorner/ Signal.SamplingRate();
  if( hpCorner > 0.5 )
    bcierr << "HighPassCorner parameter exceeds Nyquist frequency" << endl;
  if( hpCorner < 0 )
    bcierr << "HighPassCorner must be >= 0" << endl;
  int hpOrder = mparameter.HighPassOrder;
  if( hpCorner > 0 && hpCorner < 0.5 && hpOrder )
  {
    TransferFunction hp =
      FilterDesign::Butterworth()
      .Order( hpOrder )
      .Highpass( hpCorner )
      .TransferFunction();
    tf *= hp;
    gain *= abs( hp.Evaluate( -1.0 ) ); // HF gain
  }

  // Notch
  Real notchCenter = mparameter.NotchCenter / Signal.SamplingRate(),
       corner1 = 0.9 * notchCenter,
       corner2 = 1.1 * notchCenter;
  if( corner2 > 0.5 )
    bcierr << "NotchCenter parameter is close to or exceeds Nyquist frequency" << endl;
  if( corner1 < 0 )
    bcierr << "NotchCenter parameter is close to 0, or negative" << endl;
  int notchOrder = mparameter.NotchOrder;
  if( corner1 > 0 && corner2 < 0.5 && notchOrder )
  {
    TransferFunction notch =
      FilterDesign::Chebyshev()
      .Ripple_dB( -0.1 )
      .Order( notchOrder )
      .Bandstop( corner1, corner2 )
      .TransferFunction();
    tf *= notch;
    gain *= abs( notch.Evaluate( 1.0 ) ); // LF gain
  }

  // Low pass
  Real lpCorner = mparameter.LowPassCorner / Signal.SamplingRate();
  if( lpCorner > 0.5 )
    bcierr << "LowPassCorner parameter exceeds Nyquist frequency" << endl;
  if( lpCorner < 0 )
    bcierr << "LowPassCorner must be >= 0" << endl;
  int lpOrder = mparameter.LowPassOrder;
  if( lpCorner > 0 && lpCorner < 0.5 && lpOrder )
  {
    TransferFunction lp =
      FilterDesign::Butterworth()
      .Order( lpOrder )
      .Lowpass( lpCorner )
      .TransferFunction();
    tf *= lp;
    gain *= abs( lp.Evaluate( 1.0 ) ); // LF gain
    if( mparameter.Downsample != 0 )
    {
      int f = Floor( 1 / ( 4 * lpCorner ) );
      while( f > 1 && Signal.Elements() % f )
        --f;
      if( f > 1 )
      {
        Signal.SetElements( Signal.Elements() / f );
        PhysicalUnit& u = Signal.ElementUnit();
        u.SetGain( u.Gain() * f );
        u.SetRawMax( u.RawMax() / f );
		DownSampleRate=f;
      }

    }
  }

  // User gain
  outGain = mparameter.FilterGain / gain;
  outZeros = tf.Numerator().Roots();
  outPoles = tf.Denominator().Roots();
}


//constructor
StimResponseFilter::StimResponseFilter()
: 
//open port for visualization
mVisVoltageAvg( "P5FLT" ),
mVisBandpower("P4FLT"),
mVisVoltage("P3FLT"),
//initialize all the visualization flag to zero
mbVisVoltageAvg( false ),
mbVisBandpower(false),
mbVisVoltage(false)
{
  //begin to define the user input 
  BEGIN_PARAMETER_DEFINITIONS
    "Visualize matrix StimulusMarkers= 2 1 "
	  "StimulusBegin "
	  "StimulusEnd "
	  "% % % // Time-valued parameters that will be indicated by markers",
    "Filtering int EpochLength= 400ms 400ms 0 % "
      "// Length of data epoch from stimulus onset",
	"Filtering int StimulusLength= 48 48 0 % "
	  "// Length of Digital Input Stimulus",
	"Filtering string DigitalInputNum= DigitalInput1 "
	  "// Digital Input nemuber",
	"Visualize int VisualizeAvgTimeResponse= 1 0 0 1 "
	  "// Visualize current epochs (0=no 1=yes) (boolean)",
	"Visualize int VisualizeFre= 1 0 0 1 "
	  "// Visualize frequency response (0=no 1=yes) (boolean)",
	"Visualize int VisualizeTime= 1 0 0 1 "
	  "// Visualize time response (0=no 1=yes) (boolean)",
	"Filtering list ReRefChList= 0"
	  "// reference channel for each amp, 0 or empty for unipolar measurement",
	"Filtering bool StimTriggerType= 0 0 0 1 "
	  "// spatial filter type "
	  "0: Signal, "
	  "1: DigitalInput, "
	  "(enumeration)"
	  "// if use signal the stimulus can only be on channel one",
	"Filtering int MoveStimForward= 2 2 0 % "
	  "// Move forward xx number of elements",
	"Filtering:Voltage float TBHighPassCorner= 0.1 0.1 0 %",
	"Filtering:Voltage int TBHighPassOrder= 2 2 0 %",
	"Filtering:Voltage float TBLowPassCorner= 170 170 0 %",
	"Filtering:Voltage int TBLowPassOrder= 4 4 0 %",
	"Filtering:Voltage float TBNotchCenter= 0 0 0 %",
	"Filtering:Voltage int TBNotchOrder= 4 4 0 %",
	"Filtering:Voltage float TBFilterGain= 1.0 1.0 0 %",
	"Filtering:Voltage int TBDownsample= 1 1 0 %",

	"Filtering:Bandpower float FBHighPassCorner= 70 70 0 %",
	"Filtering:Bandpower int FBHighPassOrder= 2 2 0 %",
	"Filtering:Bandpower float FBLowPassCorner= 170 170 0 %",
	"Filtering:Bandpower int FBLowPassOrder= 4 4 0 %",
	"Filtering:Bandpower float FBNotchCenter= 0 0 0 %",
	"Filtering:Bandpower int FBNotchOrder= 4 4 0 %",
	"Filtering:Bandpower float FBFilterGain= 1.0 1.0 0 %",
	"Filtering:Bandpower int FBDownsample= 1 1 0 %",

	"Filtering:FreLowpass float FLHighPassCorner= 0 0 0 %",
	"Filtering:FreLowpass int FLHighPassOrder= 2 2 0 %",
	"Filtering:FreLowpass float FLLowPassCorner= 10 10 0 %",
	"Filtering:FreLowpass int FLLowPassOrder= 4 4 0 %",
	"Filtering:FreLowpass float FLNotchCenter= 0 0 0 %",
	"Filtering:FreLowpass int FLNotchOrder= 4 4 0 %",
	"Filtering:FreLowpass float FLFilterGain= 1.0 1.0 0 %",
	"Filtering:FreLowpass int FLDownsample= 0 0 0 %",
  END_PARAMETER_DEFINITIONS
  //begin the defination of states  
  BEGIN_STATE_DEFINITIONS
    "Update       1 0 0 0",    // indiccate when the output changes 
	"DigitalInput1 1 0 0 0",
  END_STATE_DEFINITIONS
  //specify the number of filters 
  int m_num_filters =3;
  //initialize the parameter array of filter
  mpFilter      = NULL;
  mpZeros       = NULL;
  mpPoles       = NULL;
  mpGain        = NULL;
  mpDataBuffer  = NULL;
  mpDataOutput  = NULL;
  //set the lenght of each parameter array
  mpFilter      = new IIRFilter<float>[m_num_filters];
  mpZeros       = new ComplexVector[m_num_filters]; 
  mpPoles       = new ComplexVector[m_num_filters]; 
  mpGain        = new Real[m_num_filters];
  mpDataBuffer  = new GenericSignal[m_num_filters];
  mpDataOutput  = new GenericSignal[m_num_filters];
  //set parameters for filters 
  mFilterParamVoltage.HighPassCorner =Parameter("TBHighPassCorner");
  mFilterParamVoltage.HighPassOrder =Parameter("TBHighPassOrder");
  mFilterParamVoltage.LowPassCorner = Parameter("TBLowPassCorner");
  mFilterParamVoltage.LowPassOrder = Parameter("TBLowPassOrder");
  mFilterParamVoltage.NotchCenter = Parameter("TBNotchCenter");
  mFilterParamVoltage.NotchOrder = Parameter("TBNotchOrder");
  mFilterParamVoltage.FilterGain = Parameter("TBFilterGain");
  mFilterParamVoltage.Downsample = Parameter("TBDownsample");

  mFilterParamBandpower.HighPassCorner =Parameter("FBHighPassCorner");
  mFilterParamBandpower.HighPassOrder =Parameter("FBHighPassOrder");
  mFilterParamBandpower.LowPassCorner = Parameter("FBLowPassCorner");
  mFilterParamBandpower.LowPassOrder = Parameter("FBLowPassOrder");
  mFilterParamBandpower.NotchCenter = Parameter("FBNotchCenter");
  mFilterParamBandpower.NotchOrder = Parameter("FBNotchOrder");
  mFilterParamBandpower.FilterGain = Parameter("FBFilterGain");
  mFilterParamBandpower.Downsample = Parameter("FBDownsample");
  
  mFilterParamBandpowerLowpass.HighPassCorner = Parameter("FLHighPassCorner");
  mFilterParamBandpowerLowpass.HighPassOrder = Parameter("FLHighPassOrder");
  mFilterParamBandpowerLowpass.LowPassCorner = Parameter("FLLowPassCorner");
  mFilterParamBandpowerLowpass.LowPassOrder = Parameter("FLLowPassOrder");
  mFilterParamBandpowerLowpass.NotchCenter = Parameter("FLNotchCenter");
  mFilterParamBandpowerLowpass.NotchOrder = Parameter("FLNotchOrder");
  mFilterParamBandpowerLowpass.FilterGain = Parameter("FLFilterGain");
  mFilterParamBandpowerLowpass.Downsample = Parameter("FLDownsample"); 
}

StimResponseFilter::~StimResponseFilter()
{}



void
StimResponseFilter::Preflight( const SignalProperties& Input, SignalProperties& Output ) const
{
  //read the user input
  PARAM( ReRefChList );
  Parameter( "StimulusMarkers" );
  Parameter("StimulusLength");
  State(Parameter("DigitalInputNum"));
  Parameter("DigitalInputNum");
  Parameter("StimTriggerType");
  double outputSamples = Parameter( "EpochLength" ).InSampleBlocks();
  int outputblocks_ = Parameter( "EpochLength" ).InSampleBlocks();
  outputSamples *= Input.Elements();
  outputSamples = ::ceil( outputSamples );

  ///////////////////
  // design filter //
  ///////////////////

  //design frequency response filter 
  SignalProperties FrepassPty = Input;
  FrepassPty.SetChannels( Input.Channels())
			.SetElements( Input.Elements() )
            .SetType( SignalType::float32 )
            .SetIsStream( false )
            .ElementUnit().SetRawMin( 0 )
                          .SetRawMax( Input.Elements() - 1 );
  DesignFilter(FrepassPty,
		       mpGain[Bandpower] ,
				 mpZeros[Bandpower] ,
				  mpPoles[Bandpower] ,
				  mFilterParamBandpower);
  mpFilter[Bandpower].SetZeros(mpZeros[Bandpower]);
  mpFilter[Bandpower].SetPoles(mpPoles[Bandpower]);
  mpFilter[Bandpower].SetGain(mpGain[Bandpower]);
  mpFilter[Bandpower].Initialize(FrepassPty.Channels());
  mpDataBuffer[Bandpower].SetProperties(FrepassPty);
  mpDataOutput[Bandpower].SetProperties(FrepassPty);
  //set the property for output signal
  Output = FrepassPty;
  Output.SetChannels(2 * Input.Channels()  )
	     .SetElements(outputblocks_*FrepassPty.Elements() )
         .SetType( SignalType::float32 )
         .SetIsStream( false )
         .ElementUnit().SetRawMin( 0 )
                       .SetRawMax( outputblocks_*FrepassPty.Elements() - 1 );
  //set up the labels for output channels
  for (int idx_filter = 0; idx_filter < 2; idx_filter++)
  {
    for( int idx_ch = 0; idx_ch < Input.Channels(); idx_ch++ )
    {
       string label_ch     = "ch"+to_string(idx_ch+1);
       string label_filter;
	   if(idx_filter==0)
		   label_filter = " Fre";
	   else
		   label_filter = " Time";
       string label_out = label_ch + string( "@" ) + label_filter;
       Output.ChannelLabels()[idx_ch + idx_filter * Input.Channels()] = label_out;
     }

  }
  //design the bandpass filter for time response
  SignalProperties TimepassPty = Input;
  TimepassPty.SetChannels( Input.Channels())
			  .SetElements( Input.Elements() )
              .SetType( SignalType::float32 )
              .SetIsStream( false )
              .ElementUnit().SetRawMin( 0 )
                            .SetRawMax( Input.Elements() - 1 );
  DesignFilter(TimepassPty,
		          mpGain[Voltage] ,
				  mpZeros[Voltage] ,
				  mpPoles[Voltage],
				  mFilterParamVoltage
  );
  mpFilter[Voltage].SetZeros(mpZeros[Voltage]);
  mpFilter[Voltage].SetPoles(mpPoles[Voltage]);
  mpFilter[Voltage].SetGain(mpGain[Voltage]);
  mpFilter[Voltage].Initialize(Input.Channels());
  mpDataBuffer[Voltage].SetProperties(TimepassPty);
  mpDataOutput[Voltage].SetProperties(TimepassPty);
  //design the BandpowerLowpass filter for the time response 
  SignalProperties LowpassPty = FrepassPty;
  LowpassPty.SetChannels( Input.Channels())
			  .SetElements( FrepassPty.Elements()  )
              .SetType( SignalType::float32 )
              .SetIsStream( false )
              .ElementUnit().SetRawMin( 0 )
                            .SetRawMax(FrepassPty.Elements() - 1 );
  DesignFilter(LowpassPty,
		          mpGain[BandpowerLowpass] ,
				  mpZeros[BandpowerLowpass] ,
				  mpPoles[BandpowerLowpass],
				  mFilterParamBandpowerLowpass
				  );
  mpFilter[BandpowerLowpass].SetZeros(mpZeros[BandpowerLowpass]);
  mpFilter[BandpowerLowpass].SetPoles(mpPoles[BandpowerLowpass]);
  mpFilter[BandpowerLowpass].SetGain(mpGain[BandpowerLowpass]);
  mpFilter[BandpowerLowpass].Initialize(Input.Channels());
  mpDataBuffer[BandpowerLowpass].SetProperties(LowpassPty);
  mpDataOutput[BandpowerLowpass].SetProperties(LowpassPty);
 
  

}


void
StimResponseFilter::Initialize( const SignalProperties& Input, const SignalProperties& Output )
{
   mNumStimulusLength = Parameter("StimulusLength");
   //set the update counter to 0
   mNumUpdateCounter=0;
   //initial flags to 0
   mbStimulusInBlock =0;
   CurrentNumBlocks =0;  
   //check the visualization flag
   mbVisVoltageAvg = int( Parameter( "VisualizeAvgTimeResponse" ) );
   mbVisBandpower = int( Parameter( "VisualizeFre" ) );
   mbVisVoltage = int(Parameter( "VisualizeTime" ));	
   //calculatethe numbr of samples for output
   double outputSamples = Parameter( "EpochLength" ).InSampleBlocks();
   outputSamples *= Input.Elements();
   outputSamples = ::ceil( outputSamples );
   //create the output property
   SignalProperties mSigHelperproperties = Input;
   mSigHelperproperties.SetChannels( Input.Channels())
	     .SetElements( static_cast<size_t>( outputSamples ) )
         .SetType( SignalType::float32 )
         .SetIsStream( false )
         .ElementUnit().SetRawMin( 0 )
                       .SetRawMax( outputSamples - 1 );
   mSigHelperSignal = GenericSignal( mSigHelperproperties );
   //create the property for 3 blocks of input 
   mPropThreeBlock = Input;
   mPropThreeBlock.SetChannels( Input.Channels())
	              .SetElements(3*Input.Elements())
				  .SetType( SignalType::float32 )
				  .SetIsStream( false )
				  .ElementUnit().SetRawMin( 0 )
                  .SetRawMax( 3*Input.Elements() - 1 );
   //set up the signal for the frequency visualization 
   if(mbVisBandpower){
   SignalProperties FrevisProperties = Output;
   FrevisProperties.SetChannels( Input.Channels())
	              .SetElements( Output.Elements())
				  .SetType( SignalType::float32 )
				  .SetIsStream( false )
				  .ElementUnit().SetRawMin( 0 )
                  .SetRawMax( Output.Elements() - 1 );
   string windowTitles = "Frequency Response ";
   FrevisProperties.SetName( windowTitles );
   SigBandpower = GenericSignal( FrevisProperties );
   mVisBandpower.Send(SigBandpower.Properties() )
	  .Send(SigBandpower);
   }
   //set up the signal for the time visualization 
   if(mbVisVoltage){
   SignalProperties TimevisProperties = Output;
   TimevisProperties.SetChannels( Input.Channels())
	              .SetElements(Output.Elements())
				  .SetType( SignalType::float32 )
				  .SetIsStream( false )
				  .ElementUnit().SetRawMin( 0 )
                  .SetRawMax( Output.Elements() - 1 );
   string windowTitles_ = "Time Response ";
   TimevisProperties.SetName( windowTitles_ );
   SigVoltage = GenericSignal( TimevisProperties);
    mVisVoltage.Send(SigVoltage.Properties() )
      .Send(SigVoltage);	
   }
   //read the number of output blocks 
   mNumBlocksPerEpoch = Parameter( "EpochLength" ).InSampleBlocks();
   SignalProperties RawDataProperty = Output;
   RawDataProperty.SetChannels( Input.Channels())
	              .SetElements( Output.Elements()+Output.Elements()/mNumBlocksPerEpoch)
				  .SetType( SignalType::float32 )
				  .SetIsStream( false )
				  .ElementUnit().SetRawMin( 0 )
                  .SetRawMax( Output.Elements()+Output.Elements()/mNumBlocksPerEpoch - 1 );
   mSigRawSignal = GenericSignal(RawDataProperty);
   //read the reference channel list
   PARAM( ReRefChList );
   mbUseDigitalInput= Parameter("StimTriggerType");
   //if the stimulus is in the signal
   if (!mbUseDigitalInput)
   {
	   for (int i = 0; i < ReRefChList->NumValues(); i++)
		 mListRefChannels.push_back(ReRefChList(i));
   }
   else
   {
	 for (int i = 0; i < ReRefChList->NumValues(); i++)
		 mListRefChannels.push_back(ReRefChList(i)-1);
   }
   SignalProperties ReferenceSigProperty =Input;
   ReferenceSigProperty.SetChannels( 1)
	              .SetElements( Input.Elements())
				  .SetType( SignalType::float32 )
				  .SetIsStream( false )
				  .ElementUnit().SetRawMin( 0 )
                  .SetRawMax( Input.Elements() - 1 );
   mSigReferenceChannel = GenericSignal(ReferenceSigProperty);
   SignalProperties RefInputProperty =Input;
   RefInputProperty.SetChannels( Input.Channels() )
	              .SetElements( Input.Elements())
				  .SetType( SignalType::float32 )
				  .SetIsStream( false )
				  .ElementUnit().SetRawMin( 0 )
                  .SetRawMax( Input.Elements() - 1 );
   mSigReferencedSignal = GenericSignal( RefInputProperty );
   SignalProperties CopyDigitalInputProperty = Input;
   CopyDigitalInputProperty.SetChannels( 1 )
	              .SetElements( Input.Elements())
				  .SetType( SignalType::float32 )
				  .SetIsStream( false )
				  .ElementUnit().SetRawMin( 0 )
                  .SetRawMax( Input.Elements() - 1 );
   mSigDigitalInput = GenericSignal(CopyDigitalInputProperty);
   //set up the property for average of time response 
   if (mbVisVoltageAvg)
   {
      SignalProperties	TimeAvgSigProperty = Input;
      TimeAvgSigProperty.SetChannels( Input.Channels() )
	                .SetElements( Output.Elements())
				    .SetType( SignalType::float32 )
				    .SetIsStream( false )
				    .ElementUnit().SetRawMin( 0 )
                    .SetRawMax( Output.Elements() - 1 );
      TimeAvgSigProperty.SetName( "AvgTResponse" ); 
      for (int ch = 0; ch < Input.Channels(); ch++)
      {
         string label_ch     = "ch "+to_string(ch+1);
	     TimeAvgSigProperty.ChannelLabels()[ch] = label_ch;
      }
      mSigVoltageAvg = GenericSignal(TimeAvgSigProperty);
      mSigVoltageAccumulated = GenericSignal(TimeAvgSigProperty);
      mVisVoltageAvg.Send(mSigVoltageAvg.Properties());
      mVisVoltageAvg.Send(mSigVoltageAvg);
	  //set all elements to zero;
      for (int ch = 0; ch < mSigVoltageAccumulated.Channels(); ch++)
      {
	     for (int el = 0; el < mSigVoltageAccumulated.Elements(); el++)
	     {
		   mSigVoltageAccumulated(ch,el) = 0;
	     }
      }
   }
   //ste up the label for the visualization 
   LabelList markerLabels;
   for( int i = 0; i < Parameter( "StimulusMarkers" )->NumRows(); ++i )
   {
     string markerName = Parameter( "StimulusMarkers" )( i, 0 );
	 int position;
     if(i==0)
		 position = Input.Elements()/DownSampleRate;
	 else
		 position = (Input.Elements()+48)/DownSampleRate;
     markerLabels.push_back( Label( position, markerName ) );
   }
   mVisBandpower.Send( CfgID::XAxisMarkers, markerLabels );
   mVisVoltage.Send( CfgID::XAxisMarkers, markerLabels );
   mVisVoltageAvg.Send( CfgID::XAxisMarkers, markerLabels );
}


void
StimResponseFilter::StartRun()
{
	//bciout<<"break1"<<endl;
}


void
StimResponseFilter::Process( const GenericSignal& Input, GenericSignal& Output )
{
	/////////////////////////
	// re-reference signal //
	/////////////////////////
	//bciout<<"break2"<<endl;
	//initialize the reference channel
	for (int el = 0; el < mSigReferenceChannel.Elements(); el++)
		mSigReferenceChannel(0,el)=0;
	//accumulate all the reference channels
	for (int refch = 0; refch <mListRefChannels.size(); refch++)
	{
		for (int el = 0; el < Input.Elements(); el++)
		{
			mSigReferenceChannel(0,el)+=Input(mListRefChannels[refch],el);
		}
	}
	//do averaging
	for (int el = 0; el < Input.Elements(); el++)
	{
		if(mListRefChannels.size()!=0)
			mSigReferenceChannel(0,el)/=mListRefChannels.size();

	}
	//reference all the input channels
	for (int ch = 0; ch < Input.Channels(); ch++)
	{
		for (int el = 0; el < Input.Elements(); el++)
		{
			if(ch ==0)
				mSigReferencedSignal(ch,el) = Input(ch,el);
			else
			{
				mSigReferencedSignal(ch,el) = Input(ch,el) - mSigReferenceChannel(0,el);
			}
		}
	}
	
	///////////////////////
	// Initial condition //
	///////////////////////

	//turn off the update flag 
	State("update") =0;
	//accumulate 2 blocks of data 
	if (mSigBlockContainer.size()<2)
	{
		//push back the input into a container
		mSigBlockContainer.push_back(mSigReferencedSignal);
		//if read from digital input
		if (mbUseDigitalInput)
		{
			//makea copy of digital input
			for (int el = 0; el < Input.Elements(); el++)
			{
				mSigDigitalInput(0,el) = State(Parameter("DigitalInputNum"))(el);
			}
			//push the digitl input to a list and store it 
			mListSigDigitalInput.push_back(mSigDigitalInput);
		}
	}

	////////////////////////
	// STIMULUS DETECTION // 
	////////////////////////

	//after get 2 blocks of data, start check
	else
	{   
		//if did not find stimulus in the second block 
		if(mbStimulusInBlock == 0)
		{
			//start check if there is a stimulus in current second block
			//if use signal
			if (!mbUseDigitalInput)
			{
				//initialize an iterator  
				std::list<GenericSignal>::iterator itr_CheckStimulus = mSigBlockContainer.begin();
				//grab the second block from the inputs 
				itr_CheckStimulus++;
				mSigSecondBlock =  *(itr_CheckStimulus);
				// check if there is a stimulus in this block
				for (int el = 0; el < mSigSecondBlock.Elements() ; el++)
				{
					// check each element of the current block if it is the onset of the stimulus
					if (mSigSecondBlock(0,el)>0 )
					{
						// mark that there is a stimulus onset in the current block
						mbStimulusInBlock =1;
						// mark the onset element of the current block
						if (el >= mNumStimulusOffsetSamples) 
							mNumStimulusStartSamples = el-mNumStimulusOffsetSamples; // Shift Foward Stimulus
						//if there is not enough data
						else
						{
							mNumStimulusStartSamples = el;
							bciout<<"there are not enough elements in this block can't shift forward"<<endl;
						}
						// no more stimulus onsets expected in the current bock
						break;
					}
				
				}
			}
			//if use digital input
			else
			{
				std::list<GenericSignal>::iterator itr_CheckDigital = mListSigDigitalInput.begin();
                //grab the second block from the digital input
				itr_CheckDigital++;
				mSigSecondBlock = *(itr_CheckDigital);
				for (int el = 0; el < mSigSecondBlock.Elements() ; el++)
				{
					// check each element of the current block if it is the onset of the stimulus
					if (mSigSecondBlock(0,el)>0 )
					{
						// mark that there is a stimulus onset in the current block
						mbStimulusInBlock =1;
						// mark the onset element of the current block
						if (el >= mNumStimulusOffsetSamples) 
							mNumStimulusStartSamples = el-mNumStimulusOffsetSamples; // Shift Foward Stimulus
						//if there is not enough data
						else
						{
							mNumStimulusStartSamples = el;
							bciout<<"there are not enough elements in this block can't shift forward"<<endl;
						}
						// no more stimulus onsets expected in the current bock
						break;
					}
				}
			}
			//if there is no stimulus in the second block 
			if (mbStimulusInBlock == 0)
			{
				//pop out the first block 
				mSigBlockContainer.pop_front();
				//if use digital input also pop out the first digital input
				if (mbUseDigitalInput)
					mListSigDigitalInput.pop_front();
			}
			//push back a new block
			mSigBlockContainer.push_back(mSigReferencedSignal);
			//if use the digital input push back a new digital input 
			if (mbUseDigitalInput)
			{
				for (int el = 0; el < Input.Elements(); el++)
					mSigDigitalInput(0,el) = State(Parameter("DigitalInputNum"))(el);
				//push the digitl input to a list and store it 
				mListSigDigitalInput.push_back(mSigDigitalInput);
			}
			CurrentNumBlocks = 0;
			//now we have three blocks of data 
		}

		///////////////
		// Epoching  //
		///////////////

		// we found an onset in the current block, remove it's artifact  
	    if (mbStimulusInBlock == 1)
		{
			
			////////////////////////
			// artifact removing  // 
			////////////////////////

			//if the first time find the stimulus 
			//artifact removing needs to be done 
			if(CurrentNumBlocks==0)
			{
				if(mSigBlockContainer.size()!=3)
				bciout<<"wrong number of data in the preepoch "<<mSigBlockContainer.size()<<endl;
				//create an array with length of three blocks
				GenericSignal m3data = GenericSignal(mPropThreeBlock);
				//move the data from list to an arry
				int mycursor =0;
				for (std::list<GenericSignal>::iterator itr = mSigBlockContainer.begin(); itr != mSigBlockContainer.end(); itr++)
				{
					for (int el = 0; el < itr->Elements(); el++)
					{
						for (int ch = 0; ch < itr->Channels(); ch++)
						{
							m3data(ch,mycursor) = (*itr)(ch,el); 
						}
						mycursor++;
					}
				}
			    //start removing artifacts 
				int StimulusAt3 =  Input.Elements()+mNumStimulusStartSamples;
				mycursor = 0;
				for (int el = StimulusAt3-mNumStimulusLength; el < StimulusAt3; el++)
				{
					// iterate through all channels
					for (int ch = 0; ch < mSigHelperSignal.Channels(); ch++)
					{
						// fading out the signal before the stimulus onset 
						mSigHelperSignal(ch,el)=(m3data(ch,el)*mycursor/mNumStimulusLength);	
						if (ch==0)
						{
							// sanity check the ensure that no stimulus has happened before
							if(mSigHelperSignal(ch,el)>0)
								bciout<<"stimulus happend before "<<endl;
						}
					}
					// increment cursor
					mycursor++;
				}
				for (int el = StimulusAt3+mNumStimulusLength; el < StimulusAt3+mNumStimulusLength*2; el++)
				{
					// copy the data after the stimulus in a buffer
					for (int ch = 0; ch < mSigHelperSignal.Channels(); ch++)
					{
						// fading out the signal after the stimulus onset 
						mSigHelperSignal(ch,el)=(m3data(ch,el)*mycursor/mNumStimulusLength);
					}
					// decrement cursor
					mycursor--;
				}
				// combine the two faded out signals (i.e., before and after the stimulus onset) 
				for (int ch = 0; ch < mSigHelperSignal.Channels(); ch++)
				{
					// iterate through all elements
					for (int el = 0; el < mNumStimulusLength; el++)
					{
						// copy the combined data into the visualization
						m3data(ch,StimulusAt3+el) = mSigHelperSignal(ch,StimulusAt3+mNumStimulusLength*2-el-1) + mSigHelperSignal(ch,StimulusAt3-el-1);
					}
				}

				/////////////////////////////////////////////////////////////////
				//  FILTERING FOR TIME AND FREQUENCY PATH FOR THE FIRST BLOCK  //
				/////////////////////////////////////////////////////////////////
				
				//copy the data back to the list
                mycursor = 0; 
				for (std::list<GenericSignal>::iterator itr = mSigBlockContainer.begin(); itr != mSigBlockContainer.end(); itr++)
				{
					for (int el = 0; el < itr->Elements(); el++)
					{
						for (int ch = 0; ch < itr->Channels(); ch++)
						{
							(*itr)(ch,el) =m3data(ch,mycursor); 
						}
						mycursor++;
					}
				}
				//fetch the first block from the previous 3 blocks
				std::list<GenericSignal>::iterator itr_First = mSigBlockContainer.begin();
				// apply time-domain path bandpass to a block of data
				mpFilter[Voltage].Process(*itr_First,mpDataOutput[Voltage]); 
				// apply frequency-domain path bandpass to a block of data 
				mpFilter[Bandpower].Process(*itr_First,mpDataOutput[Bandpower]); 
				// rectify the frequency-domain filtering output
				// iterate through each channel
				for (int idx_channel = 0; idx_channel < mpDataOutput[Bandpower].Channels(); idx_channel++)
				{
					// interate through each sample
					for (int idx_sample = 0; idx_sample < mpDataOutput[Bandpower].Elements(); idx_sample++)
					{
						// rectify the frequency-domain path band-pass filtered signal
						mpDataOutput[Bandpower](idx_channel,	idx_sample) = fabs(mpDataOutput[Bandpower](idx_channel,	idx_sample));

						// sanity check to ensure that rectification was sucessful
						if (mpDataOutput[Bandpower](idx_channel,idx_sample)<0)
						{
							bciout<<"my fabs fails"<<endl;
						}
					}
				}
				//time domain BandpowerLowpass 
				mpFilter[BandpowerLowpass].Process(mpDataOutput[Bandpower],mpDataOutput[BandpowerLowpass]);
				//mSigVoltageBlock.push_back(mpDataOutput[Voltage]);
				CurrentNumBlocks++;
				//push back the frequency response to epoch
				mSigBandpowerBlock.push_back(mpDataOutput[BandpowerLowpass]);
				//push back the time response to epoch
				mSigVoltageBlock.push_back(mpDataOutput[Voltage]);
				//remove the data from last time 
				mSigBlockContainer.pop_front();
				//if using digital input 
				if (mbUseDigitalInput)
					mListSigDigitalInput.pop_front();
			}

			//if it's not the first time to find the stimulus 
			//don't need to do artifact removing
			else
			{   
				////////////////////////////////////////////////////////////////////
				//  FILTERING FOR TIME AND FREQUENCY PATH FOR THE REST OF BLOCKS  //
				////////////////////////////////////////////////////////////////////

				//add the new data to the mSigBlockContainer
				mSigBlockContainer.push_back(mSigReferencedSignal);
				//add new digital data if using digital input
				if (mbUseDigitalInput)
				{
					//makea copy of digital input
					for (int el = 0; el < Input.Elements(); el++)
					{
						mSigDigitalInput(0,el) = State(Parameter("DigitalInputNum"))(el);
					}
					//push the digitl input to a list and store it 
					mListSigDigitalInput.push_back(mSigDigitalInput);
				}
				//fetch the first block from the previous 3 blocks
				std::list<GenericSignal>::iterator itr1 = mSigBlockContainer.begin();
				// apply frequency-domain path bandpass to a block of data 
			    mpFilter[Bandpower].Process(*itr1,mpDataOutput[Bandpower]); 
				// apply time-domain path bandpass to a block of data
				mpFilter[Voltage].Process(*itr1,mpDataOutput[Voltage]); 
				// rectify the frequency-domain filtering output
				// iterate through each channel
				for (int idx_channel = 0; idx_channel < mpDataOutput[Bandpower].Channels(); idx_channel++)
				{
					// interate through each sample
					for (int idx_sample = 0; idx_sample < mpDataOutput[Bandpower].Elements(); idx_sample++)
					{
						// rectify the frequency-domain path band-pass filtered signal
						mpDataOutput[Bandpower](idx_channel,	idx_sample) = fabs(mpDataOutput[Bandpower](idx_channel,	idx_sample));

						// sanity check to ensure that rectification was sucessful
						if (mpDataOutput[Bandpower](idx_channel,idx_sample)<0)
						{
							bciout<<"my fabs fails"<<endl;
						}
					}
				}
				//frequency domain BandpowerLowpass 
				mpFilter[BandpowerLowpass].Process(mpDataOutput[Bandpower],mpDataOutput[BandpowerLowpass]);
				//push the data to the frequency output epochs 
				mSigBandpowerBlock.push_back(mpDataOutput[BandpowerLowpass]);
				//push the data to the time output epochs
				mSigVoltageBlock.push_back(mpDataOutput[Voltage]);
				//increment number of blocks
				CurrentNumBlocks++;
				//remove the data from last time 
				mSigBlockContainer.pop_front();
				//if use digital input 
				if (mbUseDigitalInput)
					mListSigDigitalInput.pop_front();
				
				///////////////////
				// Visualization //
				///////////////////

				//check if have enough blocks to finish an epoch 
				if (CurrentNumBlocks==9)
				{
				    //increament the update counter
					mNumUpdateCounter++;
					//turn on the update flag 
					State("update") =1;
					//down sample the start point
					mNumStimulusStartSamples/=DownSampleRate;
					//if have enough data copy it to an temperary container.
					//copy the frequency response first
					int mycursor_ =0;
					for (std::list<GenericSignal>::iterator itr = mSigBandpowerBlock.begin(); itr != mSigBandpowerBlock.end(); itr++)
					{
						for (int el = 0; el < itr->Elements(); el++)
						{
							for (int ch = 0; ch < itr->Channels(); ch++)
							{
								mSigRawSignal(ch,mycursor_)=(*itr)(ch,el); 
							}
							mycursor_++;
						}
						
					}
					//eopching
					//if stimulus inside the signal
					if (!mbUseDigitalInput)
					{
						for (int ch = 1; ch < SigBandpower.Channels(); ch++)
						{
							for (int el = 0; el < SigBandpower.Elements(); el++)
							{
								SigBandpower(ch-1,el) = mSigRawSignal(ch,el+mNumStimulusStartSamples);
							}
						}
					}
					//if use digital input 
					else
					{
						for (int ch = 0; ch < SigBandpower.Channels(); ch++)
						{
							for (int el = 0; el < SigBandpower.Elements(); el++)
							{
								SigBandpower(ch,el) = mSigRawSignal(ch,el+mNumStimulusStartSamples);
							}
						}
					}
					//send the frequency response
					mVisBandpower.Send(SigBandpower);
				    // copy the time response
					mycursor_=0;
					for (std::list<GenericSignal>::iterator itr = mSigVoltageBlock.begin(); itr != mSigVoltageBlock.end(); itr++)
					{
						for (int el = 0; el < itr->Elements(); el++)
						{
							for (int ch = 0; ch < itr->Channels(); ch++)
							{
								mSigRawSignal(ch,mycursor_)=(*itr)(ch,el); 
							}
							mycursor_++;
						}
						
					}
					//epoching
					//if stimulus inside signal
					if (!mbUseDigitalInput)
					{
						for (int ch = 1; ch < SigVoltage.Channels(); ch++)
						{
							for (int el = 0; el < SigVoltage.Elements(); el++)
							{
								SigVoltage(ch-1,el) = mSigRawSignal(ch,el+mNumStimulusStartSamples);
							}
						}
					}
					//if use digital input
					else
					{
						for (int ch = 0; ch < SigVoltage.Channels(); ch++)
						{
							for (int el = 0; el < SigVoltage.Elements(); el++)
							{
								SigVoltage(ch,el) = mSigRawSignal(ch,el+mNumStimulusStartSamples);
							}
						}
					}
					//send time response
					mVisVoltage.Send(SigVoltage);
					//copy time and frequecy response to output
					for (int idx_filter = 0; idx_filter < 2; idx_filter++)
					{
						for( int idx_ch = 0; idx_ch < mSigReferencedSignal.Channels(); idx_ch++ )
						{
							for (int idx_el = 0; idx_el < Output.Elements(); idx_el++)
							{
								if (idx_filter==0)
									Output(idx_ch+idx_filter*mSigReferencedSignal.Channels(),idx_el)=SigBandpower(idx_ch,idx_el) ;
								else
									Output(idx_ch+idx_filter*mSigReferencedSignal.Channels(),idx_el)=SigVoltage(idx_ch,idx_el);
							}
						}
					}
					for (int ch = 0; ch < mSigVoltageAccumulated.Channels(); ch++)
					{
						for (int el = 0; el < Output.Elements(); el++)
						{
							mSigVoltageAccumulated(ch,el) += Output(ch,el);
							mSigVoltageAvg(ch,el) = mSigVoltageAccumulated(ch,el)/mNumUpdateCounter;
						}
					}
					mVisVoltageAvg.Send(mSigVoltageAvg);
					//after send all the signal empty the container 
				    mSigBandpowerBlock.clear();
					mSigVoltageBlock.clear();
					//after finishing visualization set stimulus flags to 0
					mbStimulusInBlock =0;
				}
			}

		}
	}	

}



		
 

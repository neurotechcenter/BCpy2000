////////////////////////////////////////////////////////////////////////////////
// $Id: $
// Authors:Peter Brunner, Kosta Andoni
// Description: NeurofeedbackTaskFilter implementation
//
//
// $BEGIN_BCI2000_LICENSE$
//
// This file is part of BCI2000, a platform for real-time bio-signal research.
// [ Copyright (C) 2000-2012: BCI2000 team and many external contributors ]
//
// BCI2000 is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// BCI2000 is distributed in the hope that it will be useful, but
//                         WITHOUT ANY WARRANTY
// - without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// this program.  If not, see <http://www.gnu.org/licenses/>.
//
// $END_BCI2000_LICENSE$
////////////////////////////////////////////////////////////////////////////////
#include "NeurofeedbackTaskFilter.h"
#include "BCIStream.h"
#include "RandomGenerator.h"
#include "TextStimulus.h"
#include <utility>
#include <math.h>
#include <qpainter>

#include "MeasurementUnits.h"
#include "TextStimulus.h"
#include "ImageStimulus.h"
#include "AudioStimulus.h"
#include "Localization.h"
#include "BarGauge.h"
#include "AnalogGauge.h"
#include "BarPlotGauge.h"
#include "ScrollingHistoryGauge.h"
#include "HistoryGauge.h"
#include "XYGauge.h"

#include <algorithm>



using namespace std;
using namespace GUI;


using namespace std;


RegisterFilter( NeurofeedbackTaskFilter, 3 );
     // Change the location if appropriate, to determine where your filter gets
     // sorted into the chain. By convention:
     //  - filter locations within SignalSource modules begin with "1."
     //  - filter locations within SignalProcessing modules begin with "2."
     //       (NB: SignalProcessing modules must specify this with a Filter() command in their PipeDefinition.cpp file too)
     //  - filter locations within Application modules begin with "3."


NeurofeedbackTaskFilter::NeurofeedbackTaskFilter() :
  mrDisplay( Window() )
{

  // Declare any parameters that the filter needs....


   BEGIN_PARAMETER_DEFINITIONS
	// Experiment Paramters
	"Application:NeurofeedbackTask int ExperimentProtocolStart= 1 1 1 3 //Starting point of experiment: 1 Baseline, 2 Training, 3 NeuroFeedback (enumeration)",
	"Application:NeurofeedbackTask int NeurofeedbackBarSignal= 1 1 1 2 //Source of NF bar signal: 1 Cumulative Signal, 2 Raw Signal (enumeration)", 
    "Application:NeurofeedbackTask int MaxPainRatingToProceed= 5 5 1 9 // max pain rating to proceed to stimulus",                       
    "Application:NeurofeedbackTask float FocusThreshold= 0.25 0.25 0.1 0.9 // percentage of feedback bar to target",
	"Application:NeurofeedbackTask double MinThreshold= 0 0 % % //minimum value of neurofeedback bar",
	"Application:NeurofeedbackTask double MaxThreshold= 100 100 % % //maximum value of neurofeedback bar",
	"Application:NeurofeedbackTask int DisplayFeedback= 1 1 0 1 //Choose to display feedback (boolean)",
	"Application:NeurofeedbackTask int TestSession= 0 0 0 1 //Only check for neurofeedback test sessions (boolean)",
	"Application:NeurofeedbackTask int NumBaselineTrials= 2 2 0 % //number of baseline fixations",
	"Application:NeurofeedbackTask int NumTrainingTrials= 5 5 0 % //number of NF training sequences",
	"Application:NeurofeedbackTask int NumAnticipationNFTrials= 10 10 0 % //number of anticipation NF sequences",
	"Application:NeurofeedbackTask int NumStimulusNFTrials= 10 10 0 % //number of stimulus NF sequences",
	END_PARAMETER_DEFINITIONS


   BEGIN_PARAMETER_DEFINITIONS
	// Experiment Images
	"Application:NeurofeedbackTask string ImageBaselineFile= ..\\tasks\\neurofeedback_task\\hand.bmp"
      " // baseline image (inputfile)",
	"Application:NeurofeedbackTask string ImagePainRatingFile= ..\\tasks\\neurofeedback_task\\hand.bmp"
      " // pain rating image (inputfile)",
	"Application:NeurofeedbackTask string ImagePainExceptionFile= ..\\tasks\\neurofeedback_task\\hand.bmp"
      " // pain exception image (inputfile)",
	"Application:NeurofeedbackTask string ImageRelaxFile= ..\\tasks\\neurofeedback_task\\hand.bmp"
      " // relax image (inputfile)",
	"Application:NeurofeedbackTask string ImageTrainingFile= ..\\tasks\\neurofeedback_task\\hand.bmp"
	" // training image (inputfile)",
	"Application:NeurofeedbackTask string ImageAntiFile= ..\\tasks\\neurofeedback_task\\hand.bmp"
      " // anticipation image (inputfile)",
	"Application:NeurofeedbackTask string ImageStimFile= ..\\tasks\\neurofeedback_task\\hand_pain.bmp"
      " // stimulus image (inputfile)",
	"Application:NeurofeedbackTask string ImageRewardPassFile= ..\\tasks\\neurofeedback_task\\reward.png"
      " // reward pass image (inputfile)",
	 "Application:NeurofeedbackTask string ImageRewardFailFile= ..\\tasks\\neurofeedback_task\\reward.png"
     " // reward fail image (inputfile)",
	END_PARAMETER_DEFINITIONS


    BEGIN_PARAMETER_DEFINITIONS
	// Experiment Strings
    "Application:NeurofeedbackTask string TextBaseline= relax // Text of baseline feedback",
    "Application:NeurofeedbackTask string TextPainRating= pain rating // Text of pain rating feedback",
    "Application:NeurofeedbackTask string TextPainException= relax4 // Text of pain exception feedback",
    "Application:NeurofeedbackTask string TextRelax= relax // Text of relax feedback",
	"Application:NeurofeedbackTask string TextTraining= training // Text of training feedback",
	"Application:NeurofeedbackTask string TextAnti= anticipation // Text of anticipation feedback",
    "Application:NeurofeedbackTask string TextStim= stimulus // Text of stimulus feedback",
	"Application:NeurofeedbackTask string TextReward= reward // Text of reward feedback",
    END_PARAMETER_DEFINITIONS


    BEGIN_PARAMETER_DEFINITIONS
	// Experiment Size and Colors
	"Application:NeurofeedbackTask float  TextHeight= 1.0 1.0 0 % // text height in percent of screeen size",             
	"Application:NeurofeedbackTask string TextBaselineColor= 0x000000 0x000000 0x000000 0xFFFFFF // Color of stim text (color)",
	"Application:NeurofeedbackTask string TextPainRatingColor= 0x000000 0x000000 0x000000 0xFFFFFF // Color of stim text (color)",
	"Application:NeurofeedbackTask string TextPainExceptionColor= 0x000000 0x000000 0x000000 0xFFFFFF // Color of stim text (color)",
	"Application:NeurofeedbackTask string TextRelaxColor= 0x000000 0x000000 0x000000 0xFFFFFF // Color of stim text (color)",
	"Application:NeurofeedbackTask string TextTrainingColor= 0x000000 0x000000 0x000000 0xFFFFFF // Color of stim text (color)",
	"Application:NeurofeedbackTask string TextAntiColor= 0x000000 0x000000 0x000000 0xFFFFFF // Color of stim text (color)",
	"Application:NeurofeedbackTask string TextStimColor= 0x000000 0x000000 0x000000 0xFFFFFF // Color of stim text (color)",
	"Application:NeurofeedbackTask string TextRewardColor= 0x000000 0x000000 0x000000 0xFFFFFF // Color of reward text (color)",
	END_PARAMETER_DEFINITIONS


  // Gauges
  BEGIN_PARAMETER_DEFINITIONS
    "Application:Gauges matrix Gauges= "
    "1 " // row labels
    "{ caption type      xpos ypos width height other } " // columns
    "  Feedback  bar       0.45    0.5    0.1   0.5    0 "
    " // Parameterization of Gauges ",

    "Application:Gauges float UpdateTime= 100ms 100ms 0 % "
    " // The time which elapses between gauge updates. ",
  END_PARAMETER_DEFINITIONS


    BEGIN_PARAMETER_DEFINITIONS
	// Timing
	"Application:NeurofeedbackTask float BaselinePeriod= 180s 180s 0 % // baseline period",              
	"Application:NeurofeedbackTask float PainExceptionPeriod= 30s 30s 0 % // perid to wait if pain is above threshold",
	"Application:NeurofeedbackTask float RelaxPeriod= 15s 15s 0 % // period of relaxation prior to stimulus",
	"Application:NeurofeedbackTask float TrainingPeriod= 15s 15s 0 % // training period",
	"Application:NeurofeedbackTask float StimAnticipationPeriodMin= 10s 10s 0 % // min stim anticipation period",
	"Application:NeurofeedbackTask float StimAnticipationPeriodMax= 15s 15Ss 0 % // max stim anticipation period",       
	"Application:NeurofeedbackTask float StimPeriod= 15s 15s 0 % // stim delivery duration",  
	"Application:NeurofeedbackTask float RewardPeriod= 15s 15s 0 % // reward duration",
    END_PARAMETER_DEFINITIONS


    BEGIN_STATE_DEFINITIONS

	"ExperimentProtocol 2  0 0 0", //
	"CurrentTrial		10  0 0 0", // The Current Trial
    "ExperimentState	4  0 0 0", //
	"NormalizePeriod	1  0 0 0", //
	"PainRatingValue    4  0 0 0", //
	"FeedbackPeriod     1  0 0 0", //
	"Baseline			1  0 0 0", //
	"PainRating			1  0 0 0", //
	"PainException		1  0 0 0", //
	"Training			1  0 0 0", //
	"Relax				1  0 0 0", //
	"StimAnticipation	1  0 0 0", //
	"StimDelivery		1  0 0 0", //
	"Reward				1  0 0 0", //
	"NormalizePeriod	1  0 0 0", //

    END_STATE_DEFINITIONS

	LANGUAGES "English",
	BEGIN_LOCALIZED_STRINGS
	"Timeout",
	END_LOCALIZED_STRINGS

	m_timeout_rectangle.left = 0.5f;
	m_timeout_rectangle.right = 0.5f;
	m_timeout_rectangle.top = 0.4f;
	m_timeout_rectangle.bottom = 0.6f;

	m_full_rectangle.left = 0.0f;
    m_full_rectangle.right = 1.0f;
    m_full_rectangle.top = 0.05f;
    m_full_rectangle.bottom = 0.15f;

	mp_text_stimulus = new TextStimulus(mrDisplay);
	mp_text_stimulus->SetText("");
	mp_text_stimulus->SetTextHeight(1.0f);
    mp_text_stimulus->SetDisplayRect(m_full_rectangle);
	mp_text_stimulus->SetColor(RGBColor::White);
	mp_text_stimulus->SetTextColor(RGBColor(0x90EE90));
	mp_text_stimulus->Present();


	m_image_stim_rectangle.left = 0.4f;
    m_image_stim_rectangle.right = 0.6f;
    m_image_stim_rectangle.top = 0.2f;
    m_image_stim_rectangle.bottom = 0.4f;

	mp_image_stim = new ImageStimulus(mrDisplay);
    mp_image_stim->SetDisplayRect(m_image_stim_rectangle);
	mp_image_stim->SetRenderingMode(GUI::RenderingMode::Transparent);
	mp_image_stim->SetPresentationMode( VisualStimulus::ShowHide );
	mp_image_stim->SetScalingMode(1);
    mp_image_stim->SetFile( Parameter("ImageStimFile") );
	mp_image_stim->Present();
	mp_image_stim->Conceal();

	mp_image_anti = new ImageStimulus(mrDisplay);
	mp_image_anti->SetDisplayRect(m_image_stim_rectangle);
	mp_image_anti->SetRenderingMode(GUI::RenderingMode::Transparent);
	mp_image_anti->SetPresentationMode( VisualStimulus::ShowHide );
	mp_image_anti->SetScalingMode(1);
    mp_image_anti->SetFile( Parameter("ImageAntiFile") );
	mp_image_anti->Present();
	mp_image_anti->Conceal();
	
	mp_image_reward = new ImageStimulus(mrDisplay);
	mp_image_reward->SetDisplayRect(m_image_stim_rectangle);
	mp_image_reward->SetRenderingMode(GUI::RenderingMode::Transparent);
	mp_image_reward->SetPresentationMode( VisualStimulus::ShowHide );
	mp_image_reward->SetScalingMode(1);
    mp_image_reward->SetFile( Parameter("ImageRewardPassFile") );
	mp_image_reward->Present();
	mp_image_reward->Conceal();

	m_gauge_rectangle.left = 0.45f;
    m_gauge_rectangle.right = 0.55f;
    m_gauge_rectangle.top = 0.5f;
    m_gauge_rectangle.bottom = 1.0f;

	m_gauge_hide_rectangle.left = 0.499f;
	m_gauge_hide_rectangle.right = 0.501f;
    m_gauge_hide_rectangle.top = 0.499f;
    m_gauge_hide_rectangle.bottom = 0.501f;
	
	mpMessage = new TextField( mrDisplay );
}


NeurofeedbackTaskFilter::~NeurofeedbackTaskFilter()
{
  DeleteGauges();
  Halt();
}

void
NeurofeedbackTaskFilter::Halt()
{
  // De-allocate any memory reserved in Initialize, stop any threads, etc.
  // Good practice is to write the Halt() method such that it is safe to call it even *before*
  // Initialize, and safe to call it twice (e.g. make sure you do not delete [] pointers that
  // have already been deleted:  set them to NULL after deletion).
}

void
NeurofeedbackTaskFilter::Preflight( const SignalProperties& Input, SignalProperties& Output ) const
{
  // The user has pressed "Set Config" and we need to sanity-check everything.
  // For example, check that all necessary parameters and states are accessible:
  //
  // Parameter( "Milk" );
  // State( "Bananas" );
  //
  // Also check that the values of any parameters are sane:
  //
  // if( (float)Parameter( "Denominator" ) == 0.0f )
  //      bcierr << "Denominator cannot be zero" << endl;
  //
  // Errors issued in this way, during Preflight, still allow the user to open
  // the Config dialog box, fix bad parameters and re-try.  By contrast, errors
  // and C++ exceptions at any other stage (outside Preflight) will make the
  // system stop, such that BCI2000 will need to be relaunched entirely.

  // Note that the NeurofeedbackTaskFilter instance itself, and its members, are read-only during
  // this phase, due to the "const" at the end of the Preflight prototype above.
  // Any methods called by Preflight must also be "const" in the same way.

  Output = Input; // this simply passes information through about SampleBlock dimensions, etc....


	State("Running");
	State("KeyDown");
	State("KeyUp");

	State("CurrentTrial");
	State("ExperimentProtocol");
	State("ExperimentState");
	State("NormalizePeriod");
	State("PainRatingValue");
	State("FeedbackPeriod");

	State("Baseline");
	State("PainRating");
	State("PainException");
	State("Relax");
	State("Training");
	State("StimAnticipation");
	State("StimDelivery");
	State("Reward");

	Parameter("SamplingRate");
	Parameter("SampleBlockSize");

	Parameter("ExperimentProtocolStart");
	Parameter("NeurofeedbackBarSignal");
	Parameter("MaxPainRatingToProceed");
	Parameter("FocusThreshold");
	Parameter("MinThreshold");
	Parameter("MaxThreshold");
	Parameter("DisplayFeedback");
	Parameter("TestSession");
	Parameter("NumBaselineTrials");
	Parameter("NumTrainingTrials");
	Parameter("NumAnticipationNFTrials");
	Parameter("NumStimulusNFTrials");

	Parameter("TextHeight");

	Parameter("TextBaseline");
	Parameter("TextPainRating");
	Parameter("TextPainException");
	Parameter("TextRelax");
	Parameter("TextTraining");
	Parameter("TextAnti");
	Parameter("TextStim");
	Parameter("TextReward");

	Parameter("ImageBaselineFile");
	Parameter("ImagePainRatingFile");
	Parameter("ImagePainExceptionFile");
	Parameter("ImageRelaxFile");
	Parameter("ImageTrainingFile");
	Parameter("ImageAntiFile");
	Parameter("ImageStimFile");
	Parameter("ImageRewardPassFile");
	Parameter("ImageRewardFailFile");

	Parameter("TextBaselineColor");
	Parameter("TextPainRatingColor");
	Parameter("TextPainExceptionColor");
	Parameter("TextRelaxColor");
	Parameter("TextTrainingColor");
	Parameter("TextAntiColor");
	Parameter("TextStimColor");
	Parameter("TextRewardColor");


	Parameter("BaselinePeriod");
	Parameter("PainExceptionPeriod");
	Parameter("RelaxPeriod");
	Parameter("TrainingPeriod");
	Parameter("StimAnticipationPeriodMin");
	Parameter("StimAnticipationPeriodMax");
	Parameter("StimPeriod");
	Parameter("RewardPeriod");


	if (Parameter("StimAnticipationPeriodMin").InMilliseconds() > Parameter("StimAnticipationPeriodMax").InMilliseconds())
		bcierr <<  "Parameter StimAnticipationPeriodMin (" << Parameter("StimAnticipationPeriodMin") << ") must be smaller or equal to StimAnticipationPeriodMax (" << Parameter("StimAnticipationPeriodMax") << ")." << endl;

	double block_size_msec = (double)(Parameter("SampleBlockSize")) / (double)(Parameter("SamplingRate").InHertz()) * 1000;

	if (Parameter("NumBaselineTrials") < 1)
		bcierr <<  "Parameter NumBaselineTrials (" << Parameter("NumBaselineTrials") << ") must be larger than 1." << endl;

	if (Parameter("NumTrainingTrials") < 1)
		bcierr <<  "Parameter NumTrainingTrials (" << Parameter("NumTrainingTrials") << ") must be larger than 1." << endl;

	if (Parameter("NumAnticipationNFTrials") < 1)
		bcierr <<  "Parameter NumAnticipationNFTrials (" << Parameter("NumAnticipationNFTrials") << ") must be larger than 1." << endl;

	if (Parameter("NumStimulusNFTrials") < 1)
		bcierr <<  "Parameter NumStimulusNFTrials (" << Parameter("NumStimulusNFTrials") << ") must be larger than 1." << endl;

	if (Parameter("MaxPainRatingToProceed") < 0 || Parameter("MaxPainRatingToProceed") > 9)
		bcierr <<  "Parameter MaxPainRatingToProceed (" << Parameter("MaxPainRatingToProceed") << ") must be between 1 and 9." << endl;

	if (Parameter("FocusThreshold") < 0.1 || Parameter("FocusThreshold") > 0.9)
		bcierr <<  "Parameter FocusThreshold (" << Parameter("FocusThreshold") << ") must be between 0.1 and 0.9." << endl;

	if (Parameter("MinThreshold") > Parameter("MaxThreshold"))
		bcierr << "Parameter MinThreshold (" << Parameter("MinThreshold") 
		       << ") must be less than Parameter MaxThreshold (" << Parameter("MinThreshold") << ")." << endl; 
	
	if (Parameter("NumBaselineTrials") < 1)
		bcierr <<  "Parameter NumBaselineTrials (" << Parameter("NumBaselineTrials") << ") must be larger than 1." << endl;

	if (Parameter("NumTrainingTrials") < 1)
		bcierr <<  "Parameter NumTrainingTrials (" << Parameter("NumTrainingTrials") << ") must be larger than 1." << endl;

	if (Parameter("NumAnticipationNFTrials") < 1)
		bcierr <<  "Parameter NumAnticipationNFTrials (" << Parameter("NumAnticipationNFTrials") << ") must be larger than 1." << endl;

	if (Parameter("NumStimulusNFTrials") < 1)
		bcierr <<  "Parameter NumStimulusNFTrials (" << Parameter("NumStimulusNFTrials") << ") must be larger than 1." << endl;

	if (fmod((double)Parameter("BaselinePeriod").InMilliseconds(),block_size_msec) > 1e-6)
		bcierr <<  "Parameter BaselinePeriod (" << Parameter("BaselinePeriod") << ") must be a multiple of the sample block time (" << block_size_msec << "ms)." << endl;

	if (fmod((double)Parameter("PainExceptionPeriod").InMilliseconds(),block_size_msec) > 1e-6)
		bcierr <<  "Parameter PainExceptionPeriod (" << Parameter("PainExceptionPeriod") << ") must be a multiple of the sample block time (" << block_size_msec << "ms)." << endl;

	if (fmod((double)Parameter("RelaxPeriod").InMilliseconds(),block_size_msec) > 1e-6)
		bcierr <<  "Parameter RelaxPeriod (" << Parameter("RelaxPeriod") << ") must be a multiple of the sample block time (" << block_size_msec << "ms)." << endl;

	if (fmod((double)Parameter("TrainingPeriod").InMilliseconds(),block_size_msec) > 1e-6)
		bcierr <<  "Parameter TrainingPeriod (" << Parameter("TrainingPeriod") << ") must be a multiple of the sample block time (" << block_size_msec << "ms)." << endl;

	if (fmod((double)Parameter("StimAnticipationPeriodMin").InMilliseconds(),block_size_msec) > 1e-6)
		bcierr <<  "Parameter StimAnticipationPeriodMin (" << Parameter("StimAnticipationPeriodMin") << ") must be a multiple of the sample block time (" << block_size_msec << "ms)." << endl;

	if (fmod((double)Parameter("StimAnticipationPeriodMax").InMilliseconds(),block_size_msec) > 1e-6)
		bcierr <<  "Parameter StimAnticipationPeriodMax (" << Parameter("StimAnticipationPeriodMax") << ") must be a multiple of the sample block time (" << block_size_msec << "ms)." << endl;

	if (fmod((double)Parameter("StimPeriod").InMilliseconds(),block_size_msec) > 1e-6)
		bcierr <<  "Parameter StimPeriod (" << Parameter("StimPeriod") << ") must be a multiple of the sample block time (" << block_size_msec << "ms)." << endl;

	if (fmod((double)Parameter("RewardPeriod").InMilliseconds(),block_size_msec) > 1e-6)
		bcierr <<  "Parameter RewardPeriod (" << Parameter("RewardPeriod") << ") must be a multiple of the sample block time (" << block_size_msec << "ms)." << endl;

	if (fmod((double)Parameter("StimAnticipationPeriodMin").InMilliseconds(),1000) > 1e-6)
		bcierr <<  "Parameter StimAnticipationPeriodMin (" << Parameter("StimAnticipationPeriodMin") << ") must be a multiple of 1000ms." << endl;

	if (fmod((double)Parameter("StimAnticipationPeriodMax").InMilliseconds(),1000) > 1e-6)
		bcierr <<  "Parameter StimAnticipationPeriodMin (" << Parameter("StimAnticipationPeriodMax") << ") must be a multiple of 1000ms." << endl;

	if (fmod((double)Parameter("StimPeriod").InMilliseconds(),1000) > 1e-6)
		bcierr <<  "Parameter StimPeriod (" << Parameter("StimPeriod") << ") must be a multiple of 1000ms." << endl;



  // Do some error checking on the gauge parameters
  ParamRef gauges = Parameter( "Gauges" );
  for( int iGauge = 0; iGauge < gauges->NumRows(); iGauge++ )
  {
    std::string type = gauges( iGauge, "type" );
    ParamRef extraParameters = gauges( iGauge, "other" );

    if( type == "" ) // We don't need to check anything, gauge doesn't exist
      continue;

    double val;
    val = GetGaugeAttr( iGauge, "xpos", 0.0 );
    if( val < 0.0 || val > 1.0 ) bcierr << "xpos on gauge " << iGauge + 1 << " should be from 0.0 to 1.0" << endl;
    val = GetGaugeAttr( iGauge, "ypos", 0.0 );
    if( val < 0.0 || val > 1.0 ) bcierr << "ypos on gauge " << iGauge + 1 << " should be from 0.0 to 1.0" << endl;
    val = GetGaugeAttr( iGauge, "captionsize", 0.5 );
    if( val <= 0.0 || val > 1.0 ) bcierr << "captionsize on gauge " << iGauge + 1 << " should be between 0.0 and 1.0" << endl;

    val = GetGaugeAttr( iGauge, "hprecision", 0.0 );
    if( val < 0.0 ) bcierr << "hprecision on gauge " << iGauge + 1 << " should be >= 0.0" << endl;
    val = GetGaugeAttr( iGauge, "vprecision", 0.0 );
    if( val < 0.0 ) bcierr << "vprecision on gauge " << iGauge + 1 << " should be >= 0.0" << endl;
    val = GetGaugeAttr( iGauge, "precision", 0.0 );
    if( val < 0.0 ) bcierr << "precision on gauge " << iGauge + 1 << " should be >= 0.0" << endl;

    for( unsigned int whichDim = 0; ; whichDim++ )
    {
      stringstream ss;
      ss.str(""); ss << (whichDim+1); string suffix = ss.str();
      bool mentioned = GaugeHasAttr( iGauge, "signal"+suffix )
	                || GaugeHasAttr( iGauge, "color"+suffix )
	                || GaugeHasAttr( iGauge, "min"+suffix )
	                || GaugeHasAttr( iGauge, "max"+suffix )
	                || GaugeHasAttr( iGauge, "qmin"+suffix )
	                || GaugeHasAttr( iGauge, "qmax"+suffix ) ;
	  if( whichDim > 0 && !mentioned ) break;
      ss.str(""); ss << "Signal(" << (whichDim+1) << ",1)"; string defaultExpression = ss.str();

      double qmin, qmax;
      qmin = GetGaugeAttr( iGauge, "qmin1", 0.0 );
      qmax = GetGaugeAttr( iGauge, "qmax1", 0.1 );
      if( qmin < 0.0 || qmin > 1.0 || qmax < 0.0 || qmax > 1.0 ) bcierr << "qmin" << suffix << " and qmax" << suffix << " on gauge " << iGauge + 1 << " should be from 0.0 to 1.0" << endl;

      string cs = GetGaugeAttr( iGauge, "signal"+suffix, defaultExpression );
      string extraMsg = " (which, since you have not changed it explicitly for this gauge, is equal to the default value of \"" + cs + "\")";
      if( GaugeHasAttr( iGauge, "signal"+suffix ) ) extraMsg = " (\"" + cs + "\")";

      GenericSignal fakeInput( Input );
      if( !Expression( cs ).IsValid( &fakeInput ) ) bcierr << "signal" << suffix << " Expression for gauge " << iGauge+1 << extraMsg << " is invalid. " << endl;
      Expression( cs ).Evaluate( &fakeInput );
    }
  }

  // Check the update time parameter for existence
  Parameter( "UpdateTime" );

}


void
NeurofeedbackTaskFilter::Initialize( const SignalProperties& Input, const SignalProperties& Output )
{
  // The user has pressed "Set Config" and all Preflight checks have passed.
  // The signal properties can no longer be modified, but the const limitation has gone, so
  // the NeurofeedbackTaskFilter instance itself can be modified. Allocate any memory you need, start any
  // threads, store any information you need in private member variables.

    m_sample_rate					= Parameter("SamplingRate");
    m_block_size					= Parameter("SampleBlockSize");
	m_protocol                      = Parameter("ExperimentProtocolStart");
	m_signal						= Parameter("NeurofeedbackBarSignal");
	m_max_pain_rating_to_proceed	= Parameter("MaxPainRatingToProceed");
	m_focus_threshold				= Parameter("FocusThreshold");
	m_min_threshold					= Parameter("MinThreshold");
	m_max_threshold					= Parameter("MaxThreshold");
	m_display_feedback				= Parameter("DisplayFeedback");
	m_test_session					= Parameter("TestSession");
	m_num_baseline_prot_trials		= Parameter("NumBaselineTrials");
	m_num_training_prot_trials		= Parameter("NumTrainingTrials");
	m_num_anticipation_prot_trials	= Parameter("NumAnticipationNFTrials");
	m_num_stimulus_prot_trials		= Parameter("NumStimulusNFTrials");

	m_trial_counter					= 1;
	m_block_delta					= 0;
	m_pain_exception                = 0;
	m_running_sum					= 0;

	/*
	m_relax_runtime_counter			= 0;
	m_feedback_runtime_counter		= 0;
	m_relax_power_sum				= 0;
	m_feedback_power_sum			= 0;
	m_trial_relax_runtime_counter	= 0;
	m_trial_feedback_runtime_counter= 0;
	m_trial_relax_power_sum			= 0;
	m_trial_feedback_power_sum		= 0;
	*/ 

	m_counter						= 0;
	m_product_sum                   = 0;
	m_period_sum                    = 0;
	m_value_sum                     = 0;
	m_period_squared_sum            = 0;
	m_value_squared_sum             = 0;

	m_counter_trial                 = 0;
	m_product_sum_trial             = 0; 
	m_period_sum_trial              = 0;
	m_value_sum_trial               = 0;
	m_period_squared_sum_trial      = 0;
	m_value_squared_sum_trial       = 0;

	if ( m_protocol == e_baseline_prot){
		m_current_state				= e_pain_rating;
		m_num_trials				= m_num_baseline_prot_trials;
	}
	else if (m_protocol == e_training_prot){
		m_current_state				= e_relax;
		m_num_trials				= m_num_training_prot_trials;
	}
	else if (m_protocol == e_neurofeedback_prot){
		m_current_state				= e_relax;
		m_num_trials				= m_num_anticipation_prot_trials + m_num_stimulus_prot_trials;
	}

	if (m_test_session){
		m_protocol					= e_baseline_prot;
		m_current_state				= e_pain_rating;
		m_num_training_prot_trials	= 999;
	}

	m_block_size_msec				= (float)(m_block_size) / (float)(m_sample_rate) * 1000;
	m_baseline_pain_rating			= true;
	m_anticipation_NF				= true;


	m_text_height					= Parameter("TextHeight");

	m_text_baseline_color			= RGBColor( Parameter("TextBaselineColor") );
	m_text_pain_rating_color		= RGBColor( Parameter("TextPainRatingColor") );
	m_text_pain_exception_color		= RGBColor( Parameter("TextPainExceptionColor") );
	m_text_relax_color				= RGBColor( Parameter("TextRelaxColor") );
	m_text_training_color			= RGBColor( Parameter("TextTrainingColor") );
	m_text_anti_color				= RGBColor( Parameter("TextAntiColor") );
	m_text_stim_color				= RGBColor( Parameter("TextStimColor") );
	m_text_reward_color				= RGBColor( Parameter("TextRewardColor") );

	m_text_baseline_string			= Parameter("TextBaseline");
	m_text_pain_rating_string		= Parameter("TextPainRating");
	m_text_pain_exception_string	= Parameter("TextPainException");
	m_text_relax_string				= Parameter("TextRelax");
	m_text_training_string			= Parameter("TextTraining");
	m_text_anti_string				= Parameter("TextAnti");
	m_text_stim_string				= Parameter("TextStim");
	m_text_reward_string			= Parameter("TextReward");

	m_stim_anticipation_period_min_seconds		= Parameter("StimAnticipationPeriodMin").InMilliseconds() / 1000;
	m_stim_anticipation_period_max_seconds		= Parameter("StimAnticipationPeriodMax").InMilliseconds() / 1000;

	// convert wait time from ms into blocks 
	m_baseline_period_blocks					= Parameter("BaselinePeriod").InMilliseconds() / m_block_size_msec;
	m_pain_exception_period_blocks				= Parameter("PainExceptionPeriod").InMilliseconds() / m_block_size_msec;
	m_relax_period_blocks						= Parameter("RelaxPeriod").InMilliseconds() / m_block_size_msec;
	m_training_period_blocks					= Parameter("TrainingPeriod").InMilliseconds() / m_block_size_msec;
	m_stim_anticipation_period_min_blocks		= Parameter("StimAnticipationPeriodMin").InMilliseconds() / m_block_size_msec;
	m_stim_anticipation_period_max_blocks		= Parameter("StimAnticipationPeriodMax").InMilliseconds() / m_block_size_msec;
	m_stim_period_blocks						= Parameter("StimPeriod").InMilliseconds() / m_block_size_msec;
	m_reward_period_blocks						= Parameter("RewardPeriod").InMilliseconds() / m_block_size_msec;

	mp_text_stimulus->SetTextHeight(m_text_height);
	mp_text_stimulus->SetColor(RGBColor::White);
	mp_text_stimulus->SetTextColor(m_text_relax_color);

	mpMessage->SetTextColor( RGBColor::Black )
            .SetTextHeight( 0.8f )
            .SetColor( RGBColor::White )
            .SetAspectRatioMode( GUI::AspectRatioModes::AdjustWidth )
            .SetObjectRect( m_timeout_rectangle );

  // Delete the Current Gauges
  DeleteGauges();

  // Grab the update time
  mUpdateTime = Parameter( "UpdateTime" ).InSampleBlocks();

  // Create new Gauges
  for( int iGauge = 0; iGauge < Parameter( "Gauges" )->NumRows(); iGauge++ )
  {
    // Make a display rect for this gauge
    double width  = GetGaugeAttr( iGauge, "width",  0.9 );
    double height = GetGaugeAttr( iGauge, "height", 0.9 );
    double xpos   = GetGaugeAttr( iGauge, "xpos",   1.0 - 2.0 * width );
    double ypos   = GetGaugeAttr( iGauge, "ypos",   1.0 - 2.0 * height );
    // TODO:  maybe use more tricks for specifying layout
    GUI::Rect gaugeRect = { xpos, ypos, xpos + width, ypos + height };

    // Get a handle to the optional parameters
    std::string type    = GetGaugeAttr( iGauge, "type", "", true );
    std::string caption = GetGaugeAttr( iGauge, "caption", "" );
    std::string hformat = GetGaugeAttr( iGauge, "hformat", GetGaugeAttr( iGauge, "format", "%g" ) );
    std::string vformat = GetGaugeAttr( iGauge, "vformat", GetGaugeAttr( iGauge, "format", "%g" ) );
    int    vprecision  = (int)GetGaugeAttr( iGauge, "vprecision", GetGaugeAttr( iGauge, "precision", 0.0) );
    int    hprecision  = (int)GetGaugeAttr( iGauge, "hprecision", GetGaugeAttr( iGauge, "precision", 0.0) );
    double captionSize = GetGaugeAttr( iGauge, "captionsize", 0.5 );
    bool    showscale  = (bool)GetGaugeAttr( iGauge, "showscale", 0.0 );

    // Creating a bar gauge
    if( type == "bar" )
    {
      // Create the gauge using our values
      BarGauge *g = new BarGauge( mrDisplay, 0 );
      //g->SetCaption( caption );
      //g->SetCaptionSize( 0.25 );
	  g->SetDisplayRect( m_gauge_rectangle );
      //g->SetDisplayRect( gaugeRect );
      InitDimension( g, iGauge, 0, Input.Channels(), true );
      g->SetVScalePrecision( vprecision );
      g->SetVScaleFormat( vformat);
      g->ShowScale( showscale );
      mpGauges.push_back( g );
    }

    // Blank gauge type means we don't want to create a gauge
    else if( type == "" )
    {
      //  Do nothing
    }

    // User made a mistake
    else
      bcierr << "Gauge type " << type << " not known." << endl;
  }
 
	mrDisplay.Hide();

  //Create shaded target regions
  float gauge_target_bottom = 0.5 + (0.5 * m_focus_threshold);
	m_gauge_target_rectangle.left = 0.0f;
	m_gauge_target_rectangle.right = 1.0f;
    m_gauge_target_rectangle.top = 0.5f;
    m_gauge_target_rectangle.bottom = gauge_target_bottom;

	mp_shaded_region = new RectangularShape(mrDisplay);
	mp_shaded_region->SetDisplayRect(m_gauge_target_rectangle);
	mp_shaded_region->SetScalingMode(1);
    mp_shaded_region->SetColor(RGBColor::White);
	mp_shaded_region->SetFillColor(RGBColor(0x90EE90));

	//Initiate gauges
	for( size_t iGauge = 0; iGauge < mpGauges.size(); iGauge++ ){
		mpGauges[iGauge]->GetDimension(0).SetBounds(m_min_threshold, m_max_threshold);
		mpGauges[iGauge]->GetDimension(0).SetBorderColor(RGBColor::White);
	}
	mrDisplay.Update();

	//Define thresholds
	m_green_threshold = (1 - m_focus_threshold)*m_max_threshold;
	m_red_threshold = ((1 - m_focus_threshold)/2)*m_max_threshold;
	m_mid_threshold = (m_green_threshold + m_red_threshold)/2;

	//Prepare for beginning of task
	Speak("task is ready to run");
	DisplayMessage("");
	HideGauges();
	ResetStates();
}

void
NeurofeedbackTaskFilter::StartRun()
{
  // The user has just pressed "Start" (or "Resume")

  mTimeSinceLastUpdate = INT_MAX;
  mTheoreticalMilliseconds = 0.0;
  DisplayMessage("");
  ShowGauges();
}

void
NeurofeedbackTaskFilter::Process( const GenericSignal& Input, GenericSignal& Output )
{

  // And now we're processing a single SampleBlock of data.
  // Remember not to take too much CPU time here, or you will break the real-time constraint.

    Output = Input; // This passes the signal through unmodified.
    
    // Update the Gauges
    for( size_t iGauge = 0; iGauge < mpGauges.size(); iGauge++ ){

		//Grab gauge value
		float val;
		if (State("FeedbackPeriod"))
		{
			val = Input(0,0);

			if (m_signal == 1)
			{
				m_running_sum += val;
				val = m_running_sum;
			}
		}
		else
		{
			val = 0;
		}
    
	//Decide the color for the bar depending on the location of the value
	if (val < m_red_threshold) mpGauges[iGauge]->GetDimension(0).SetColor(RGBColor::Red);
	if (val > m_red_threshold && val < m_green_threshold){
		if (val < m_mid_threshold){
			int green = ((val-m_red_threshold)/(m_mid_threshold-m_red_threshold))*255;
			mpGauges[iGauge]->GetDimension(0).SetColor(RGBColor(255,green,0));
		}
		if (val > m_mid_threshold){
			int red = ((m_green_threshold-val)/(m_green_threshold-m_mid_threshold))*255;
			mpGauges[iGauge]->GetDimension(0).SetColor(RGBColor(red,255,0));
		}
	}
	if (val > m_green_threshold) mpGauges[iGauge]->GetDimension(0).SetColor(RGBColor::Green);
    
	mpGauges[iGauge]->Update(val); 
	}
  
	mrDisplay.Update();
   
	 /*
	if ( State("FeedbackPeriod") ){
		m_feedback_runtime_counter += (Output.Channels()*Output.Elements());
		m_trial_feedback_runtime_counter += (Output.Channels()*Output.Elements());
	}
	else if ( State("NormalizePeriod") ) {
		m_relax_runtime_counter += (Output.Channels()*Output.Elements());
		m_trial_relax_runtime_counter += (Output.Channels()*Output.Elements());
	}
	*/

	 if ( State("FeedbackPeriod") || State("NormalizePeriod") ){
		 m_counter += (Output.Channels()*Output.Elements());
		 m_counter_trial += (Output.Channels()*Output.Elements());
		 GetPowerSums(Output);
	 }


	if (m_protocol == 4) 
	{
		StopRun();
		return;
	}

	// keep track of the states
	State("ExperimentProtocol") = m_protocol;
	State("CurrentTrial")		= m_trial_counter;
	State("ExperimentState")	= m_current_state;


	switch(m_current_state)
    {

		//////////////////////////////////////////////////////////////////////////
		case e_baseline:
			{
				State("NormalizePeriod") = 1;
				State("Baseline") = 1;
				State("FeedbackPeriod") = 0;
				m_block_delta++;
				
				if (m_block_delta == 1) {

					//Print experiment info to applog
					AppLog << "=============================================" << endl;
					AppLog << "---------------------------------------------" << endl;
					AppLog << "Experiment Protocol: " << m_protocol << endl;
					AppLog << "Trial #" << m_trial_counter << "/" << m_num_trials << endl;
					AppLog << "---------------------------------------------" << endl;
					SummarizeExperiment();

					//Print current state and period to applog
					AppLog << "State: Baseline for " << ((float)(m_baseline_period_blocks) * m_block_size_msec) << "ms ..." << endl;
					Feedback(m_text_baseline_string, m_text_baseline_color, true);
				}

				// check if baseline period has expired
				if(m_block_delta >= m_baseline_period_blocks) {
					
					// go to pain rating state
					ResetStates();
					m_current_state = e_pain_rating;
					m_block_delta = 0;
				}

			}

			break;
		//////////////////////////////////////////////////////////////////////////
        case e_pain_rating:

			{
				State("PainRating") = 1;
				State("FeedbackPeriod") = 0;
				m_block_delta++;

				//Print current state to applog
				if (m_block_delta == 1) {

					if (m_baseline_pain_rating && m_protocol == 1)
						AppLog << "Welcome to the Neurofeedback Task! Please enter the baseline pain rating:" << endl;

					AppLog << "State: Waiting for pain rating..." << endl;
					Feedback(m_text_pain_rating_string, m_text_pain_rating_color, true);
				}


				//Look for keyboard number input
				m_pain_rating = ReadKeyboardLoggerNumber();

				//Check conditons for next state
				if( m_pain_rating > -1 )
				{
					State("PainRatingValue") = m_pain_rating;
					ResetStates();

					//If this is the first input, make it the baseline (Can only be used for the first protocol)
					//Otherwise be sure to indicate your baseline pain rating in the parameters
					if (m_baseline_pain_rating && m_protocol == 1){
						m_max_pain_rating_to_proceed = m_pain_rating;
						m_baseline_pain_rating = false;
						
						m_current_state = e_baseline;
						m_block_delta = 0;
						m_pain_exception = 0;
					}

					//Pain rating exceeds the baseline. Go to pain exception
					else if (m_pain_rating > m_max_pain_rating_to_proceed){
						m_current_state = e_pain_exception;
						m_block_delta = 0;
						m_pain_exception++;

						AppLog << "State: Pain rating value " << m_pain_rating << " which is above threshold of " << m_max_pain_rating_to_proceed << "." << endl;
					}
					
					//Baseline protocol. Go to baseline and increment trial counter
					else if (m_protocol == 1){
						m_current_state = e_baseline;
						m_block_delta = 0;
						m_pain_exception = 0;
						m_trial_counter++;

						//Last trial. Reset trial counter and go to next protocol
						if ((m_trial_counter - 1) == m_num_baseline_prot_trials){
							m_current_state = e_relax;
							m_trial_counter = 1;
							m_protocol++;
							m_num_trials = m_num_training_prot_trials;
						}
					}

					//Training protocol. Go to relax and increment trial counter
					else if (m_protocol == 2){
						m_current_state = e_relax;
						m_block_delta = 0;
						m_pain_exception = 0;
						m_trial_counter++;

						//Last trial. Reset trial counter and go to next protocol
						if ((m_trial_counter - 1) == m_num_training_prot_trials){
							m_current_state = e_randomize;
							m_trial_counter = 1;
							m_protocol++;
							m_num_trials = m_num_anticipation_prot_trials + m_num_stimulus_prot_trials;
						}
					}
					
					//Anticipation NF and Stimulus NF protocol. Go to reward
					else if (m_protocol == 3){
						m_current_state = e_reward;
						m_block_delta = 0;
						m_pain_exception = 0;
					}

					//Print pain rating to applog if it meets the baseline threshold
					if ( m_pain_exception == 0) 
						AppLog << "State: Pain rating value " << m_pain_rating << " which is at or below threshold of " << m_max_pain_rating_to_proceed << "." << endl;

				} 
			}

            break;

		//////////////////////////////////////////////////////////////////////////
		case e_pain_exception:
			{
				State("PainException") = 1;
				State("FeedbackPeriod") = 0;
				m_block_delta++;
			
				//Print current state and period to applog
				if (m_block_delta == 1) 
				{
					AppLog << "State: Pain exception for " << ((float)(m_pain_exception_period_blocks) * m_block_size_msec) << "ms ..." << endl;
					Feedback(m_text_pain_exception_string, m_text_pain_exception_color, true);
				}


				// check if pain exception period has expired
				if(m_block_delta >= m_pain_exception_period_blocks) 
				{
					// go to pain rating
					ResetStates();
			        m_current_state = e_pain_rating;
				    m_block_delta = 0;

				}

			}
			
			break;

		//////////////////////////////////////////////////////////////////////////
		case e_relax:
			{
				State("NormalizePeriod") = 1;
				State("Relax") = 1;
				State("FeedbackPeriod") = 0;
				m_block_delta++;
		
				if (m_block_delta == 1) 
				{
					//Print experiment info to applog
					AppLog << "=============================================" << endl;
					AppLog << "---------------------------------------------" << endl;
					AppLog << "Experiment Protocol: " << m_protocol << endl;
					AppLog << "Trial #" << m_trial_counter << "/" << m_num_trials << endl;
					AppLog << "---------------------------------------------" << endl;
					SummarizeExperiment();

					//Print current state and period to applog
					AppLog << "State: Relax for " << ((float)(m_relax_period_blocks) * m_block_size_msec) << "ms ..." << endl;
					Feedback(m_text_relax_string, m_text_relax_color, true);
				}

				// check if relax period has expired
				if(m_block_delta >= m_relax_period_blocks) 
				{
					ResetStates();

					//First training protocol. Go to training
					if (m_protocol == 2)
					{
						m_current_state = e_training;
						m_block_delta = 0;
					}

					//Anticipation NF training protocol. Go to anticipation
					else if (m_protocol == 3)
					{
						m_current_state = e_stimulus_anticipation;
						m_block_delta = 0;
					}
				}

			}
			
			break;
			
		//////////////////////////////////////////////////////////////////////////
			case e_training:
			{
				State("Training") = 1;
				State("FeedbackPeriod") = 1;
				m_block_delta++;
		
				//Print current state and period to applog
				if (m_block_delta == 1) 
				{
					AppLog << "State: Neurofeedback Training for " << ((float)(m_training_period_blocks) * m_block_size_msec) << "ms ..." << endl;
					Feedback(m_text_training_string, m_text_training_color, true);
					m_running_sum = 0;
					ShowGauges();
				}

				// check if training period has expired
				if(m_block_delta >= m_training_period_blocks) 
				{
					//Go to pain rating
					ResetStates();
					m_current_state = e_pain_rating;
					m_block_delta = 0;
//					HideGauges();
				}

			}
			
			break;
			
		//////////////////////////////////////////////////////////////////////////
		case e_stimulus_anticipation:
			{
				State("StimAnticipation") = 1;
				State("FeedbackPeriod") = 1;
				m_block_delta++;
				
				//Print current state and period to applog
				if (m_block_delta == 1) 
				{
					m_stim_anticipation_period_blocks = ((float)RandomNumber(m_stim_anticipation_period_min_seconds,m_stim_anticipation_period_max_seconds)) * 1000 / m_block_size_msec;	
					AppLog << "State: Stimulus Anticipation for " << ((float)(m_stim_anticipation_period_blocks) * m_block_size_msec) << "ms ..." << endl;

					//Anticipation will only appear to the subject if it is the Anticipation neurofeedback protocol
					if (m_anticipation_NF){
						Feedback(m_text_anti_string, m_text_anti_color, false);
						VisualAnticipationImageOn();
						ShowGauges();
					}

					//Show relax otherwise
					else{
						Feedback(m_text_relax_string, m_text_relax_color, false);
					}
				}

				m_blocks_to_wait = m_stim_anticipation_period_blocks-m_block_delta+1;
			
				//Countdown to the end of stimulus anticipation
				if ((float)m_blocks_to_wait * m_block_size_msec - floor((float)m_blocks_to_wait * m_block_size_msec / 1000.0) * 1000 < 1e-6 && (float)(m_blocks_to_wait * m_block_size_msec) < 10000)
				{
					AppLog << "State: Stimulus Delivery in " << (float)m_blocks_to_wait * m_block_size_msec << "ms ..." << endl;

					m_count_down_thread.SetCounter((unsigned int)((float)m_blocks_to_wait * m_block_size_msec / 1000.0));
					m_count_down_thread.start();
				}

				// check if anticipation period has expired
				if(m_block_delta >= m_stim_anticipation_period_blocks) // Hold "baseline" for 3 seconds...
				{
					// go to next state
					ResetStates();
			        m_current_state = e_stimulus_delivery;
				    m_block_delta = 0;
					VisualAnticipationImageOff();
//					HideGauges();	
				}

			}

			break;

		//////////////////////////////////////////////////////////////////////////
		case e_stimulus_delivery:
			{
				State("StimDelivery") = 1;
				State("FeedbackPeriod") = 1;
				m_block_delta++;
				
				//Print current state and period to applog
				if (m_block_delta == 1) 
				{
					AppLog << "State: Stimulus Delivery for " << ((float)(m_stim_period_blocks) * m_block_size_msec) << "ms ..." << endl;

					//Stimulus feedback gauge will only appear in the Stimulus NF protocol
					if (!m_anticipation_NF){
						Feedback(m_text_stim_string, m_text_stim_color, false);
						VisualStimulusImageOn();
						ShowGauges();
					}

					//Otherwise just the image and text will appear. No gauges
					else{
						Feedback(m_text_stim_string, m_text_stim_color, false);
						VisualStimulusImageOn();
					}
				}

				m_blocks_to_wait = m_stim_period_blocks-m_block_delta+1;

				//Countdown to the end of the stimulus delivery
				if ((float)m_blocks_to_wait * m_block_size_msec - floor((float)m_blocks_to_wait * m_block_size_msec / 1000.0) * 1000 < 1e-6 && (float)(m_blocks_to_wait * m_block_size_msec) < 10000)
				{
					AppLog << "State: End of Stimulus Delivery in " << (float)m_blocks_to_wait * m_block_size_msec << "ms ..." << endl;

					m_count_down_thread.SetCounter((unsigned int)((float)m_blocks_to_wait * m_block_size_msec / 1000.0));
					m_count_down_thread.start();
				}

				// check if stimulus period has expired
				if(m_block_delta >= m_stim_period_blocks) // Hold "baseline" for 3 seconds...
				{
					// go to next state
					ResetStates();
			        m_current_state = e_pain_rating;
				    m_block_delta = 0;
					VisualStimulusImageOff();
//					HideGauges();
				}

			}

			break;

		//////////////////////////////////////////////////////////////////////////
		case e_reward:
			{
				State("Reward") = 1;
				State("FeedbackPeriod") = 0;
				m_block_delta++;
				
				//Print current state and period to applog
				if (m_block_delta == 1) 
				{
					AppLog << "State: Reward for " << ((float)(m_reward_period_blocks) * m_block_size_msec) << "ms ..." << endl;
					
					int score = m_running_sum * 10;
					m_text_reward_string = to_string(score);
					if (score > (m_max_threshold * (1-m_focus_threshold))){
						m_text_reward_color = RGBColor::Green;
						mp_image_reward->SetFile( Parameter("ImageRewardPassFile") );
						VisualRewardImageOn();
					}
					else{
						m_text_reward_color = RGBColor::Red;
						mp_image_reward->SetFile( Parameter("ImageRewardFailFile") );
					}

					Feedback(m_text_reward_string, m_text_reward_color, false);
				}

				// check if first trial period has expired
				if(m_block_delta >= m_reward_period_blocks) // Hold "baseline" for 3 seconds...
				{
					ResetStates();

					//Anticipation protocol. Go to e_randomize
					if (m_protocol == 3){ 
						m_current_state = e_randomize;
						m_block_delta = 0;
						m_trial_counter++;
					}
					if (m_num_anticipation_prot_trials == 0 && m_num_stimulus_prot_trials == 0){
						m_protocol++;
						m_current_state = e_randomize;
					}

					VisualRewardImageOff();
				}

			}

			break;

		//////////////////////////////////////////////////////////////////////////
		case e_randomize:
		{
			m_current_state = e_relax;
			int rand = RandomNumber(1,10);

			if (rand%2 == 0){
				if( m_num_anticipation_prot_trials > 0){
					m_anticipation_NF = true;
					m_num_anticipation_prot_trials--;
				}
				else{
					m_anticipation_NF = false;
					m_num_stimulus_prot_trials--;
				}
			}
			else{
				if (m_num_stimulus_prot_trials > 0){
					m_anticipation_NF = false;
					m_num_stimulus_prot_trials--;
				}
				else{
					m_anticipation_NF = true;
					m_num_anticipation_prot_trials--;
				}
			}
		}

			break;

	}

}

void
NeurofeedbackTaskFilter::StopRun()
{
  // The Running state has been set to 0, either because the user has pressed "Suspend",
  // or because the run has reached its natural end.
	mp_text_stimulus->SetText("");
	DisplayMessage( "Timeout" );

  // You know, you can delete methods if you're not using them.
  // Remove the corresponding declaration from NeurofeedbackTaskFilter.h too, if so.
}





bool
NeurofeedbackTaskFilter::IsButtonPressed()
{

	//Check for keypresses
	bool press = false;

	for( unsigned int i = 0; i < m_block_size; i++ )
	{
		if( State( "KeyDown" )( i ) == 0x20 )
		{
			press = true;
//				bciout << "key pressed" << endl;
		}
	}	

	return press;
}


int
NeurofeedbackTaskFilter::RandomNumber(int min, int max)
{
	if (min == max)
	{
		return min;
	} else {
		return ( min + m_random_generator.Random() % (max - min) );
	}

}

int
NeurofeedbackTaskFilter::ReadKeyboardLoggerNumber()
{

	//Check for keypresses
	int number = -1;

	for( unsigned int i = 0; i < m_block_size; i++ )
	{
		if( State( "KeyDown" )( i ) >=  0x61 && State( "KeyDown" )( i ) <= 0x69)
		{
			number = State( "KeyDown" )( i ) - 0x60;
		} else
		if( State( "KeyDown" )( i ) >=  0x31 && State( "KeyDown" )( i ) <= 0x39) 
		{
			number = State( "KeyDown" )( i ) - 0x30;
		}
	}	

	return number;

}


void
NeurofeedbackTaskFilter::VisualAnticipationImageOn()
{
    mp_image_anti->Present();
}

void
NeurofeedbackTaskFilter::VisualStimulusImageOn()
{
    mp_image_stim->Present();
}

void
NeurofeedbackTaskFilter::VisualRewardImageOn()
{
    mp_image_reward->Present();
}


void
NeurofeedbackTaskFilter::VisualAnticipationImageOff()
{
	mp_image_anti->Conceal();
}

void
NeurofeedbackTaskFilter::VisualStimulusImageOff()
{
	mp_image_stim->Conceal();
}

void
NeurofeedbackTaskFilter::VisualRewardImageOff()
{
	mp_image_reward->Conceal();
}

void
NeurofeedbackTaskFilter::Speak(std::string sz_text)
{
  
  m_speech_output.SetText(sz_text);
	m_speech_output.Present();
  
}

void
NeurofeedbackTaskFilter::Feedback(std::string sz_text, RGBColor color, bool b_speak)
{
  
	mp_text_stimulus->SetText(sz_text.c_str());
	mp_text_stimulus->SetTextColor( color );
	mp_text_stimulus->Present();
	if (b_speak) {
		Speak(sz_text.c_str());
	}
  
}



bool
NeurofeedbackTaskFilter::GaugeHasAttr( int inGaugeIndex, string inAttrName ) const
{
  bool found = false;
  ParamRef p = GetGaugeAttr( inGaugeIndex, inAttrName, found, false );
  return found;
}
double
NeurofeedbackTaskFilter::GetGaugeAttr( int inGaugeIndex, string inAttrName, double inDefaultValue, bool inRequired ) const
{
  bool found = false;
  ParamRef p = GetGaugeAttr( inGaugeIndex, inAttrName, found, inRequired );
  if( (string)p == "" ) found = false;
  return ( found ? p : inDefaultValue );
}
string
NeurofeedbackTaskFilter::GetGaugeAttr( int inGaugeIndex, string inAttrName, string inDefaultValue, bool inRequired ) const
{
  bool found = false;
  ParamRef p = GetGaugeAttr( inGaugeIndex, inAttrName, found, inRequired );
  return ( found ? p : inDefaultValue );
}

ParamRef
NeurofeedbackTaskFilter::GetGaugeAttr( int inGaugeIndex, string inAttrName, bool & outFound, bool inRequired ) const
{
  ParamRef p = Parameter( "Gauges" );
  outFound = true;
  for( int j = 0; j < p->NumColumns(); j++ )
  {
    const string & label = p->ColumnLabels()[j];
    ParamRef p_ij = p( inGaugeIndex, j);
    bool issubmatrix = ( p_ij->NumColumns() == 2 );
    if( issubmatrix )
      for( int i = 0; i < p_ij->NumRows(); i++ )
        if( (string)p_ij( i, 0 ) == inAttrName )
          return p_ij( i, 1 );
    if( label == inAttrName )
      return p_ij;
  }
  outFound = false;
  if( inRequired ) bcierr << "Gauge #" << inGaugeIndex+1 << " has no \"" << inAttrName << "\" attribute" << endl;
  return p;
}

bool
NeurofeedbackTaskFilter::InitDimension( Gauge* gauge, unsigned int gaugeIndex, unsigned int whichDim, unsigned int nChannels, bool force )
{
  stringstream ss;
  ss.str(""); ss << (whichDim+1); string suffix = ss.str();
  if( force || GaugeHasAttr( gaugeIndex, "signal"+suffix ) )
  {
    ss.str(""); ss << "Signal(" << (whichDim+1) << ",1)"; string defaultSignalStr = ss.str();
    string expr    = GetGaugeAttr( gaugeIndex, "signal"+suffix, ( whichDim < nChannels ) ? defaultSignalStr : "0" );
    double rawMin  = GetGaugeAttr( gaugeIndex, "min"+suffix,  -1.0 );
    double rawMax  = GetGaugeAttr( gaugeIndex, "max"+suffix,  +1.0 );
    double qMin    = GetGaugeAttr( gaugeIndex, "qmin"+suffix, -1.0 );
    double qMax    = GetGaugeAttr( gaugeIndex, "qmax"+suffix, -1.0 );
	double qCapacity = MeasurementUnits::TimeInSampleBlocks( GetGaugeAttr( gaugeIndex, "qmemory"+suffix, "10s" ) ) / (double)mUpdateTime;
    double qLifetime = MeasurementUnits::TimeInSampleBlocks( GetGaugeAttr( gaugeIndex, "qlimit"+suffix,  "0" ) ) / (double)mUpdateTime;
	double qExpansion = GetGaugeAttr( gaugeIndex, "qexpand"+suffix, 0.0 );
	double qWait     = MeasurementUnits::TimeInSampleBlocks( GetGaugeAttr( gaugeIndex, "qwait"+suffix,   "250ms" ) ) / (double)mUpdateTime; // TODO:  a prospective way, not yet implemented, of automating the "hedge against ringing" and smoothing away the feedValueToQuantilizer argument to Gauge::Evaluate
    int qColor = GetGaugeAttr( gaugeIndex, "color"+suffix, 0x0000FF );
    if( gauge )
    {
      Dimension& d = gauge->GetDimension( whichDim );
      d.SetControlExpression( expr );
      d.SetBounds( rawMin, rawMax );
      d.InitAutoscale( qMin, qMax, qCapacity, qLifetime ); // , qWait );
      d.SetColor( qColor );
	  d.SetExpansion( qExpansion );
    }
    return true;
  }
  return false;
}

//Deletes the feedback gauge
void
NeurofeedbackTaskFilter::DeleteGauges()
{
  for( size_t iGauge = 0; iGauge < mpGauges.size(); iGauge++ )
  {
    if( mpGauges[iGauge] )
    {
      mpGauges[iGauge]->Hide();
      delete mpGauges[iGauge];
      mpGauges[iGauge] = NULL;
    }
  }
  mpGauges.clear();
}

//Makes the feedback gauge visible
void 
NeurofeedbackTaskFilter::ShowGauges()
{
//	State("FeedbackPeriod") = 1;
//	m_running_sum = 0;

	if (m_display_feedback){
		for( size_t iGauge = 0; iGauge < mpGauges.size(); iGauge++ ){
		  mpGauges[iGauge]->Show();
		  mpGauges[iGauge]->SetDisplayRect( m_gauge_rectangle );
		}
		mp_shaded_region->SetFillColor(RGBColor(0x90EE90));
		mrDisplay.Update();
	}

}

//Makes the feedback gauge invisible
void 
NeurofeedbackTaskFilter::HideGauges()
{
	State("FeedbackPeriod") = 0;
  
	for( size_t iGauge = 0; iGauge < mpGauges.size(); iGauge++ ){
	  mpGauges[iGauge]->Hide();
	  mpGauges[iGauge]->SetDisplayRect( m_gauge_hide_rectangle );
	}
	mp_shaded_region->SetFillColor(RGBColor::White);
	mrDisplay.Update();
	
}

//Displays a message to the center of the screen. Used to denote end of experiment
void
NeurofeedbackTaskFilter::DisplayMessage( const string& inMessage )
{
  if( inMessage.empty() )
  {
    mpMessage->Hide();
  }
  else
  {
    mpMessage->SetText( " " + inMessage + " " );
    mpMessage->Show();
  }
}

//Sets all the experiment states to zero so that only the current new state will be set to one
void 
NeurofeedbackTaskFilter::ResetStates()
{
	State("FeedbackPeriod") = 0;
	State("NormalizePeriod") = 0;

	State("Baseline") = 0;
	State("PainRating") = 0;
	State("PainException") = 0;
	State("Relax") = 0;
	State("Training") = 0;
	State("StimAnticipation") = 0;
	State("StimDelivery") = 0;
	State("Reward") = 0;
}

void 
NeurofeedbackTaskFilter::SummarizeExperiment()
{
	/*
	if (m_feedback_power_sum == 0)
		return;

	double G = pow(m_feedback_power_sum + m_relax_power_sum, 2)			/ (double) (m_feedback_runtime_counter + m_relax_runtime_counter);
	double numerator = pow(m_feedback_power_sum, 2)						/ (double) (m_feedback_runtime_counter)
					 + pow(m_relax_power_sum, 2)						/ (double) (m_relax_runtime_counter)
					 - pow(m_feedback_power_sum + m_relax_power_sum, 2)	/ (double) (m_feedback_runtime_counter + m_relax_runtime_counter);
	double denominator = pow(m_feedback_power_sum, 2) + pow(m_relax_power_sum, 2) - G;

	double trial_G = pow(m_trial_feedback_power_sum + m_trial_relax_power_sum, 2)			/ (double) (m_trial_feedback_runtime_counter + m_trial_relax_runtime_counter);
	double trial_numerator = pow(m_trial_feedback_power_sum, 2)								/ (double) (m_trial_feedback_runtime_counter)
					 + pow(m_trial_relax_power_sum, 2)										/ (double) (m_trial_relax_runtime_counter)
					 - pow(m_trial_feedback_power_sum + m_trial_relax_power_sum, 2)			/ (double) (m_trial_feedback_runtime_counter + m_trial_relax_runtime_counter);
	double trial_denominator = pow(m_trial_feedback_power_sum, 2) + pow(m_trial_relax_power_sum, 2) - trial_G;

	AppLog << "The correlation value for the whole experiment is: " << numerator/denominator << endl;
	AppLog << "The correlation value for the previous trial is: " << trial_numerator/trial_denominator << endl;

	m_trial_feedback_runtime_counter = 0;
	m_trial_relax_runtime_counter = 0;
	m_trial_feedback_power_sum = 0;
	m_trial_relax_power_sum = 0;
	*/
	
	if (m_period_sum == 0)
		return;

	double numerator = (m_counter*m_product_sum) - (m_period_sum*m_value_sum);
	double denominator = sqrt( ( (m_counter*m_period_squared_sum) - pow(m_period_sum,2) )*( (m_counter*m_value_squared_sum) - pow(m_value_sum,2) ) );

	AppLog << "The correlation value for the whole experiment is: " << numerator/denominator << endl;

	double trial_numerator = (m_counter_trial*m_product_sum_trial) - (m_period_sum_trial*m_value_sum_trial);
	double trial_denominator = sqrt( ( (m_counter_trial*m_period_squared_sum_trial) - pow(m_period_sum_trial,2) )*( (m_counter_trial*m_value_squared_sum_trial) - pow(m_value_sum_trial,2) ) );

	AppLog << "The correlation value for the previous trial is: " << trial_numerator/trial_denominator << endl;

	m_product_sum_trial = 0;
	m_period_sum_trial = 0;
	m_value_sum_trial = 0;
	m_period_squared_sum_trial = 0;
	m_value_squared_sum_trial = 0;
}

void
NeurofeedbackTaskFilter::GetPowerSums(const GenericSignal& sig)
{
	/*
	if (State("FeedbackPeriod") == 1){
		for (int channel = 0; channel < sig.Channels(); channel++){
			for (int element = 0; element < sig.Elements(); element++){
				m_feedback_power_sum += sig(channel,element);
				m_trial_feedback_power_sum += sig(channel, element);
			}
		}	
	}
	else if (State("NormalizePeriod") == 1){
		for (int channel = 0; channel < sig.Channels(); channel++){
			for (int element = 0; element < sig.Elements(); element++){
				m_relax_power_sum += sig(channel,element);
				m_trial_relax_power_sum += sig(channel, element);
			}	
		}
	}
	*/
	double x = 0;
	double y = 0;

	m_counter++;
	m_counter_trial++;

	for (int channel = 0; channel < sig.Channels(); channel++){
		for (int element = 0; element < sig.Elements(); element++){
			x += sig(channel,element);
		}
	}	

	if (State("FeedbackPeriod"))
		y = 1;
	else if (State("NormalizePeriod"))
		y = -1;

	m_product_sum += x*y;
	m_period_sum += x;
	m_value_sum += y;
	m_period_squared_sum += pow(x,2);
	m_value_squared_sum += pow(y,2);

	m_product_sum_trial += x*y;
	m_period_sum_trial += x;
	m_value_sum_trial += y;
	m_period_squared_sum_trial += pow(x,2);
	m_value_squared_sum_trial += pow(y,2);
}

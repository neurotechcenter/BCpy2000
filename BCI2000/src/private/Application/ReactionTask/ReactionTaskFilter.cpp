////////////////////////////////////////////////////////////////////////////////
// $Id: $
// Authors: Peter Brunner, William Coon
// Description: ReactionTaskFilter implementation
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
#include "ReactionTaskFilter.h"
#include "RandomGenerator.h"
#include "TextStimulus.h"
#include <utility>
#include <math.h>
#include "gSTIMbox.imports.h"


using namespace std;
using namespace GUI;


RegisterFilter( ReactionTaskFilter, 3 );
     // Change the location if appropriate, to determine where your filter gets
     // sorted into the chain. By convention:
     //  - filter locations within SignalSource modules begin with "1."
     //  - filter locations within SignalProcessing modules begin with "2."  
     //       (NB: SignalProcessing modules must specify this with a Filter() command in their PipeDefinition.cpp file too)
     //  - filter locations within Application modules begin with "3."


ReactionTaskFilter::ReactionTaskFilter():
mr_window( Window() )
{

    // Declare any parameters that the filter needs....

    BEGIN_PARAMETER_DEFINITIONS
	// Experiment
    "Application:ReactionTask int UseSpaceButton= 1 1 0 1 // enable keyboard space button ? (boolean)",                       // These are just examples:
    "Application:ReactionTask int PushButtonChannel= 2 2 1 % // channel number of push button",
	"Application:ReactionTask float PushButtonThreshold= 125mV 125mV 0 % // threshold for push button",      
    "Application:ReactionTask int PresentMarker= 1 1 0 1 // enable visual marker ? (boolean)",                       // These are just examples:
    "Application:ReactionTask int gSTIMboxCOMport= 6 6 1 32 // com port of g.STIMbox",   
    "Application:ReactionTask matrix Experiment= "
      "{ 1 2 3 4 5 6 7 8 } " // row labels
	  "{type cue_visual cue_auditory cue_tactile stim_visual stim_auditory stim_tactile } " // column labels
      "all 1 1 1 1 1 1	"
      "valid 1 0 0 1 0 0	"
      "valid 0 1 0 0 1 0	"
      "valid 0 0 1 0 0 1	"
      "valid 0 0 0 1 1 1	"
      "valid 0 0 0 1 0 0	"
      "valid 0 0 0 0 1 0	"
      "valid 0 0 0 0 0 1	"
      " // experiment definition ", 
    "Application:ReactionTask string AuditoryStimulusFile= ..\\tasks\\reaction_task\\880Hz_2s.wav"
      " // auditory stimulus file (inputfile)",
	"Application:ReactionTask string VisualCueImageFile= ..\\tasks\\reaction_task\\visual.bmp"
      " // visual cue image file (inputfile)",
	"Application:ReactionTask string AuditoryCueImageFile= ..\\tasks\\reaction_task\\auditory.bmp"
      " // auditory cue image file (inputfile)",
	"Application:ReactionTask string TactileCueImageFile= ..\\tasks\\reaction_task\\tactile.bmp"
      " // tactile cue image file (inputfile)",
	"Application:ReactionTask string AllCueImageFile= ..\\tasks\\reaction_task\\allmodalities.bmp"
      " // all cues image file (inputfile)",
	"Application:ReactionTask string VisualStimImageFile= ..\\tasks\\reaction_task\\visualGO.bmp"
      " // visual stimulus image file (inputfile)",
	END_PARAMETER_DEFINITIONS


    BEGIN_PARAMETER_DEFINITIONS
	// Visual Stimulus Size and Color
	"Application:ReactionTask int UseClock= 0 0 0 1 // use counting clock as visual stimulus ? (boolean)",
    "Application:ReactionTask int FeedbackPosText= 0 0 0 1 // present TextFeedBackPos ? (boolean)",                       
    "Application:ReactionTask int FeedbackPosTime= 1 1 0 1 // present Reaction time ? (boolean)",    
	"Application:ReactionTask int DoPreCue= 1 1 0 1 // do pre-cue state ? (boolean)",
	"Application:ReactionTask int DoPriming= 0 0 0 1 // prime sensory modalities during pre-cue ? (boolean)",
	"Application:ReactionTask float TextHeight= 0.1 0.1 0 % // text height in percent of screeen size",             
	"Application:ReactionTask string FixCrossColor= 0xC0C0C0 0xC0C0C0 0x000000 0xFFFFFF // Color of fixation cross (color)",
	"Application:ReactionTask string TextStimColor= 0xFFFF00 0xFFFF00 0x000000 0xFFFFFF // Color of stim text (color)",
	"Application:ReactionTask string TextFeedbackPosColor= 0x00FF00 0x00FF00 0x000000 0xFFFFFF // Color of positive feedback (color)",
	"Application:ReactionTask string TextFeedbackNegColor= 0xFF0000 0xFF0000 0x000000 0xFFFFFF // Color of negative feedback (color)",
	"Application:ReactionTask string TextFeedbackPreCueColor= 0xFFFF00 0xFFFF00 0x000000 0xFFFFFF // Color of negative feedback (color)",
	"Application:ReactionTask string TextFeedbackPreStimColor= 0x00FF00 0x00FF00 0x000000 0xFFFFFF // Color of negative feedback (color)",
    "Application:ReactionTask string TextFeedbackPos= Good%20Job! // Text of positive feedback",
    "Application:ReactionTask string TextFeedbackNegFalseStart= False%20Start! // Text of false start feedback",
    "Application:ReactionTask string TextFeedbackNegTooFast= Too%20Fast! // Text of false start feedback",
    "Application:ReactionTask string TextFeedbackNegTooSlow= Too%20Slow! // Text of too slow feedback",
    "Application:ReactionTask string TextFeedbackPreCue= Pay%20Attention%20With%20Your... // Text of pre-cue instructions",
    "Application:ReactionTask string TextFeedbackPreStim= Get%20Ready! // Text of pre-wait stimulus",
	"Application:ReactionTask string TextFeedbackTaskFinished= Good%20Job! // Text for end of run",
	END_PARAMETER_DEFINITIONS

    BEGIN_PARAMETER_DEFINITIONS
	// Timing
	"Application:ReactionTask float WaitMin= 1s 1s 0 % // min time to present stimulus",              
	"Application:ReactionTask float WaitMax= 3s 3s 0 % // max time to present stimulus",
	"Application:ReactionTask float RTMin= 100ms 100ms 0 % // min reaction time (False Start)",              
	"Application:ReactionTask float RTMax= 2000ms 2000ms 0 % // max reaction time (Too Long)",
	"Application:ReactionTask float ISI= 1500ms 1500ms 0 % // ISI/pre-cue text display duration",       
	"Application:ReactionTask float CueDuration= 1000ms 1000ms 0 % // cue duration",    
	"Application:ReactionTask float FeedbackDuration= 1000ms 1000ms 0 % // feedback duration",    
    END_PARAMETER_DEFINITIONS


    // ...and likewise any state variables:

    BEGIN_STATE_DEFINITIONS

    "CurrentTrial     8  0 0 0", // The Current Trial
    "ExperimentState  4  0 0 0", //
	"CueVisual        1  0 0 0", //
	"CueAuditory      1  0 0 0", //
	"CueTactile       1  0 0 0", //
	"StimVisual       1  0 0 0", //
	"StimAuditory     1  0 0 0", //
	"StimTactile      1  0 0 0", //

    END_STATE_DEFINITIONS

	m_tactile_stimulus   = NULL;
	mp_auditory_stimulus = NULL;
	mp_input             = NULL;


    m_full_rectangle.left = 0.0f;
    m_full_rectangle.right = 1.0f;
    m_full_rectangle.top = 0.2f;
    m_full_rectangle.bottom = 0.8f;

	mp_text_stimulus = new TextStimulus(mr_window);
	mp_text_stimulus->SetText("");
	mp_text_stimulus->SetTextHeight(0.2f);
    mp_text_stimulus->SetDisplayRect(m_full_rectangle);
	mp_text_stimulus->SetColor(RGBColor::Black);
	mp_text_stimulus->SetTextColor(RGBColor::Green);
	mp_text_stimulus->Present();

    m_marker_rectangle.left = 0.8f;
    m_marker_rectangle.right = 1.0f;
    m_marker_rectangle.top = 0.0f;
    m_marker_rectangle.bottom = 0.2f;

	mp_text_marker = new TextStimulus(mr_window);
	mp_text_marker->SetText("");
	mp_text_marker->SetTextHeight(1.0f);
	mp_text_marker->SetDisplayRect(m_marker_rectangle);
	mp_text_marker->SetColor(RGBColor::Black);
	mp_text_marker->SetTextColor(RGBColor::White);
	mp_text_marker->Present();

    m_cue_image_rectangle.left = 0.35f;
    m_cue_image_rectangle.right = 0.65f;
    m_cue_image_rectangle.top = 0.35f;
    m_cue_image_rectangle.bottom = 0.65f;

	mp_cue_image = new ImageStimulus(mr_window);
	mp_cue_image->SetDisplayRect(m_cue_image_rectangle);
	mp_cue_image->SetPresentationMode( VisualStimulus::ShowHide );
	mp_cue_image->SetAspectRatioMode( GUI::AspectRatioModes::AdjustBoth );
    mp_cue_image->SetFile( Parameter("VisualCueImageFile") );
	mp_cue_image->Present();
	mp_cue_image->Conceal();

	m_stim_image_rectangle.left = 0.35f;
    m_stim_image_rectangle.right = 0.65f;
    m_stim_image_rectangle.top = 0.35f;
    m_stim_image_rectangle.bottom = 0.65f;

	mp_vis_stim_image = new ImageStimulus(mr_window);
    mp_vis_stim_image->SetDisplayRect(m_stim_image_rectangle);
	mp_vis_stim_image->SetRenderingMode(GUI::RenderingMode::Transparent);
	mp_vis_stim_image->SetPresentationMode( VisualStimulus::ShowHide );
	mp_vis_stim_image->SetAspectRatioMode( GUI::AspectRatioModes::AdjustBoth );
    mp_vis_stim_image->SetFile( Parameter("VisualStimImageFile") );
	mp_vis_stim_image->Present();
	mp_vis_stim_image->Conceal();


				  
}


ReactionTaskFilter::~ReactionTaskFilter()
{

	if (m_tactile_stimulus) 
	{
		int error = gSTIMboxclose(m_tactile_stimulus);
		if(error) StimBoxErrorMessage(error);
	}

	if (mp_auditory_stimulus != NULL)
		delete mp_auditory_stimulus;



  Halt();

}

void
ReactionTaskFilter::Halt()
{
  // De-allocate any memory reserved in Initialize, stop any threads, etc.
  // Good practice is to write the Halt() method such that it is safe to call it even *before*
  // Initialize, and safe to call it twice (e.g. make sure you do not delete [] pointers that
  // have already been deleted:  set them to NULL after deletion).
}

void
ReactionTaskFilter::Preflight( const SignalProperties& Input, SignalProperties& Output ) const
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
  
  // Note that the ReactionTaskFilter instance itself, and its members, are read-only during
  // this phase, due to the "const" at the end of the Preflight prototype above.
  // Any methods called by Preflight must also be "const" in the same way.

	Output = Input; // this simply passes information through about SampleBlock dimensions, etc....
  
	State("Running");
	State("KeyDown");
	Parameter("SamplingRate");
	Parameter("SampleBlockSize");
	Parameter("WaitMin");
	Parameter("WaitMax");
	Parameter("RTMin");
	Parameter("RTMax");
	Parameter("ISI");
	Parameter("gSTIMboxCOMport");
	Parameter("Experiment");
	Parameter("AuditoryStimulusFile");
	Parameter("VisualCueImageFile");
	Parameter("AuditoryCueImageFile");
	Parameter("TactileCueImageFile");
	Parameter("AllCueImageFile");
	Parameter("CueDuration");
	Parameter("UseSpaceButton");
	Parameter("PushButtonChannel");
	Parameter("PushButtonThreshold");
	Parameter("PresentMarker");
	Parameter("TextHeight");
	Parameter("TextStimColor");
	Parameter("TextFeedbackPosColor");
	Parameter("TextFeedbackNegColor");
	Parameter("TextFeedbackPreCueColor");
	Parameter("TextFeedbackPreStimColor");
	Parameter("TextFeedbackPos");
	Parameter("TextFeedbackNegFalseStart");
	Parameter("TextFeedbackNegTooFast");
	Parameter("TextFeedbackNegTooSlow");
	Parameter("TextFeedbackPreCue");
	Parameter("TextFeedbackPreStim");
	Parameter("FeedbackPosText");
	Parameter("FeedbackPosTime");  
	Parameter("TextFeedbackTaskFinished");
	Parameter("DoPriming");
	Parameter("DoPreCue");
	Parameter("VisualStimImageFile");
	Parameter("UseClock");
	Parameter("FixCrossColor");

	State("ExperimentState");
    State("CurrentTrial");
    State("ExperimentState");
	State("CueVisual");
	State("CueAuditory");
	State("CueTactile");
	State("StimVisual");
	State("StimAuditory");
	State("StimTactile");


	if ( Parameter("RTMin").InMilliseconds() > Parameter("RTMax").InMilliseconds() )
		bcierr <<  "Parameter RTMin (" << Parameter("RTMin") << ") must be smaller or equal to RTMax (" << Parameter("RTMax") << ")." << endl;

	if ( Parameter("WaitMin").InMilliseconds() > Parameter("WaitMax").InMilliseconds() )
		bcierr <<  "Parameter WaitMin (" << Parameter("WaitMax") << ") must be smaller or equal to WaitMax (" << Parameter("WaitMax") << ")." << endl;

	double block_size_msec = (double)(Parameter("SampleBlockSize")) / (double)(Parameter("SamplingRate").InHertz()) * 1000;
  
	if (fmod((double)Parameter("WaitMin").InMilliseconds(),block_size_msec) > 1e-6)
		bcierr <<  "Parameter WaitMin (" << Parameter("WaitMin") << ") must be a multiple of the sample block time (" << block_size_msec << "ms)." << endl;

	if (fmod((double)Parameter("RTMax").InMilliseconds(),block_size_msec) > 1e-6)
		bcierr <<  "Parameter WaitMax (" << Parameter("WaitMax") << ") must be a multiple of the sample block time (" << block_size_msec << "ms)." << endl;

	if (fmod((double)Parameter("CueDuration").InMilliseconds(),block_size_msec) > 1e-6)
		bcierr <<  "Parameter CueDuration (" << Parameter("CueDuration") << ") must be a multiple of the sample block time (" << block_size_msec << "ms)." << endl;

	if (fmod((double)Parameter("ISI").InMilliseconds(),block_size_msec) > 1e-6)
		bcierr <<  "Parameter ISI (" << Parameter("ISI") << ") must be a multiple of the sample block time (" << block_size_msec << "ms)." << endl;

	if (fmod((double)Parameter("FeedbackDuration").InMilliseconds(),block_size_msec) > 1e-6)
		bcierr <<  "Parameter FeedbackDuration (" << Parameter("FeedbackDuration") << ") must be a multiple of the sample block time (" << block_size_msec << "ms)." << endl;

	if (Parameter("Experiment")->NumColumns() != 7)
		bcierr <<  "Parameter Experiment has " << Parameter("Experiment")->NumColumns() << " rows, but must have 7 rows." << endl; 

	if (Parameter("PushButtonChannel") > Input.Channels())
		bcierr <<  "Parameter PushButtonChannel (" << Parameter("PushButtonChannel") << ") has to be smaller or equal to the number of channels (" << Input.Channels() << ")" << endl; 

	if ((OptionalParameter("FilterEnabled") == 1) && (Parameter("UseSpaceButton") == 0))
			bcierr << "Parameter FilterEnabled needs to be disabled for the push button detection to work" << endl; 


}


void
ReactionTaskFilter::Initialize( const SignalProperties& Input, const SignalProperties& Output )
{
	int error;

	m_do_priming            = Parameter("DoPriming") == 1;
	m_use_clock             = Parameter("UseClock") == 1;
	m_is_first_trial        = true;
    m_current_state			= e_gaze;
    m_sample_rate			= Parameter("SamplingRate");
    m_block_size			= Parameter("SampleBlockSize");
	m_num_trials			= Parameter("Experiment")->NumRows();
	m_runtime_counter       = 0;
	m_trial_counter			= 0;
	m_block_delta			= 0;
	m_too_fast_counter		= 0; 
	m_too_slow_counter		= 0;
	m_false_start_counter	= 0;
	m_reaction_time.clear();


	m_block_size_msec = (float)(m_block_size) / (float)(m_sample_rate) * 1000;

	// convert wait time from ms into blocks 
	m_wait_min_blocks				= Parameter("WaitMin").InMilliseconds() / m_block_size_msec;
	m_wait_max_blocks				= Parameter("WaitMax").InMilliseconds() / m_block_size_msec;
	m_cue_duration_blocks			= Parameter("CueDuration").InMilliseconds() / m_block_size_msec;
	m_rt_min                        = Parameter("RTMin").InMilliseconds();
	m_rt_min_blocks					= Parameter("RTMin").InMilliseconds() / m_block_size_msec;
	m_rt_max						= Parameter("RTMax").InMilliseconds();
	m_rt_max_blocks					= Parameter("RTMax").InMilliseconds() / m_block_size_msec;
	m_isi_blocks					= Parameter("ISI").InMilliseconds() / m_block_size_msec;
	m_feedback_blocks				= Parameter("FeedbackDuration").InMilliseconds() / m_block_size_msec;

	m_present_marker                = Parameter("PresentMarker") == 1; 

	m_use_space_button				= Parameter("UseSpaceButton") == 1; 
	m_push_button_channel			= Parameter("PushButtonChannel");
	m_push_button_threshold			= Parameter("PushButtonThreshold").InMicrovolts();

	m_do_pre_cue                    = Parameter("DoPreCue") == 1;

	m_feedback_pos_text             = Parameter("FeedbackPosText") == 1;
	m_feedback_pos_time             = Parameter("FeedbackPosTime") == 1;

	m_text_height					= Parameter("TextHeight");
	m_text_stim_color				= RGBColor( Parameter("TextStimColor") );
	m_text_feedback_pos_color		= RGBColor( Parameter("TextFeedbackPosColor") );
	m_text_feedback_neg_color		= RGBColor( Parameter("TextFeedbackNegColor") );
	m_text_feedback_pre_cue_color	= RGBColor( Parameter("TextFeedbackPreCueColor") );
	m_text_feedback_pre_stim_color	= RGBColor( Parameter("TextFeedbackPreStimColor") );

	m_text_feedback_pos				= Parameter("TextFeedbackPos");
	m_text_feedback_neg_false_start = Parameter("TextFeedbackNegFalseStart");
	m_text_feedback_neg_too_fast	= Parameter("TextFeedbackNegTooFast");
	m_text_feedback_neg_too_slow	= Parameter("TextFeedbackNegTooSlow");
	m_text_feedback_pre_cue			= Parameter("TextFeedbackPreCue");
	m_text_feedback_pre_stim		= Parameter("TextFeedbackPreStim");
	m_text_feedback_task_finished   = Parameter("TextFeedbackTaskFinished");

	m_fix_cross_color               = RGBColor( Parameter("FixCrossColor") );

	mp_text_stimulus->SetTextHeight(m_text_height);


	// check if g.STIMbox has already been initialized
	if (m_tactile_stimulus) 
	{
		// close connection to g.STIMbox
		error = gSTIMboxclose(m_tactile_stimulus);
		if(error) StimBoxErrorMessage(error);
	}
	
	// initialize g.STIMbox 
	m_tactile_stimulus = NULL;
    error = gSTIMboxinit(&m_tactile_stimulus, Parameter("gSTIMboxCOMport"), 256, 8, 0);
	if(error) StimBoxErrorMessage(error);

	int ports[] = {0, 1, 2, 3, 4, 5, 6, 7};
	int modes[] = {0, 0, 0, 0, 0, 0, 0, 0};
		
	// set the port modes
	error = gSTIMboxsetMode(m_tactile_stimulus, 8, ports, modes);
	if(error) StimBoxErrorMessage(error);

	TactileStimulusOff();

	if (mp_auditory_stimulus != NULL)
		delete mp_auditory_stimulus;

	mp_auditory_stimulus = NULL;
	mp_auditory_stimulus = new WavePlayer();
	mp_auditory_stimulus->SetFile(Parameter("AuditoryStimulusFile"));


	ParamRef Experiment = Parameter( "Experiment" );

	m_stimuli.clear();
	for (unsigned int row=0; row < m_num_trials; row++) 
	{

		Trial newTrial;
		newTrial.cue_visual		= Experiment(row,1) == 1;
		newTrial.cue_auditory	= Experiment(row,2) == 1;
		newTrial.cue_tactile	= Experiment(row,3) == 1;
		newTrial.stim_visual	= Experiment(row,4) == 1;
		newTrial.stim_auditory	= Experiment(row,5) == 1;
		newTrial.stim_tactile	= Experiment(row,6) == 1;
		m_stimuli.push_back( newTrial );

	}


}

void
ReactionTaskFilter::StartRun()
{
  // The user has just pressed "Start" (or "Resume")

}


void
ReactionTaskFilter::Process( const GenericSignal& Input, GenericSignal& Output )
{

  // And now we're processing a single SampleBlock of data.
  // Remember not to take too much CPU time here, or you will break the real-time constraint.
  
	Output = Input; // This passes the signal through unmodified.      
	mp_input = &Input; 

	m_runtime_counter++;

	if (m_trial_counter >= m_num_trials) 
	{
//		SummarizeExperiment();
		return;
	}

	if ( m_is_first_trial )
	{
		m_current_state = e_first_trial;
	}

	// keep track of the states
	State("ExperimentState")	= m_current_state;
	State("CurrentTrial")		= m_trial_counter+1;
	State("CueVisual")			= m_stimuli[m_trial_counter].cue_visual    ? 1 : 0;
	State("CueAuditory")		= m_stimuli[m_trial_counter].cue_auditory  ? 1 : 0;
	State("CueTactile")			= m_stimuli[m_trial_counter].cue_tactile   ? 1 : 0;
	State("StimVisual")			= m_stimuli[m_trial_counter].stim_visual   ? 1 : 0;
	State("StimAuditory")		= m_stimuli[m_trial_counter].stim_auditory ? 1 : 0;
	State("StimTactile")		= m_stimuli[m_trial_counter].stim_tactile  ? 1 : 0;

	

    switch(m_current_state)
    {

		//////////////////////////////////////////////////////////////////////////
		case e_first_trial:
			{

				m_block_delta++;

				// present "get ready" statement
				mp_text_stimulus->SetTextColor( m_text_feedback_pre_cue_color );
				mp_text_stimulus->SetText("Get Ready...");
				mp_text_stimulus->Present();

				// check if first trial period has expired
				if(m_block_delta >= m_cue_duration_blocks*3) //Hold "get ready" for 3 seconds...
				{
					// go to next state
			        m_current_state = e_gaze;
				    m_block_delta = 0;
					m_is_first_trial = false;

				}

			}

			break;
		//////////////////////////////////////////////////////////////////////////
        case e_gaze:

			m_block_delta++;

			if (m_block_delta == 1)
			{   AppLog << "=============================================" << endl;
				AppLog << "---------------------------------------------" << endl;
			    AppLog << "Trial #" << m_trial_counter+1 << "/" << m_num_trials << endl;
				AppLog << "---------------------------------------------" << endl;
				AppLog << "Cue:" <<      (m_stimuli[m_trial_counter].cue_visual ? " Visual" : "") << 
					                     (m_stimuli[m_trial_counter].cue_auditory ? " Auditory" : "") << 
									     (m_stimuli[m_trial_counter].cue_tactile ? " Tactile" : "") << endl;
				AppLog << "Stimulus:" << (m_stimuli[m_trial_counter].stim_visual ? " Visual" : "") << 
					                     (m_stimuli[m_trial_counter].stim_auditory ? " Auditory" : "") << 
									     (m_stimuli[m_trial_counter].stim_tactile ? " Tactile" : "") << endl;
				AppLog << "---------------------------------------------" << endl;
				AppLog << "State: Waiting for eye gaze fixation ..." << endl;
			}

			//Wait for gaze fixation
            //TODO: Find out how to do this.
            //For now just space bar outta here.

            if (1) //if( IsButtonPressed() )
            {
                m_current_state = e_pre_cue;
                m_block_delta = 0;

            } 

            break;

		//////////////////////////////////////////////////////////////////////////
		case e_pre_cue:

			if (m_do_pre_cue)
			{
			    m_block_delta++;

			    // Make sure text box is showing...
		        mp_text_stimulus->Present();

		    	if (m_block_delta == 1)
		    	{
		    		AppLog << "State: Preparing subject for cue ..." << endl;
			    	FeedbackPreCue();
			    }

                if(m_block_delta >= m_feedback_blocks)
			    {
				    FeedbackOff();
                    m_current_state = e_cue;
                    m_block_delta = 0;
                }
			}

			else
			{
				FeedbackOff();
				m_current_state = e_cue;
				m_block_delta = 0;
			}

            break;

		//////////////////////////////////////////////////////////////////////////
        case e_cue:

			m_block_delta++;

			// no cue necessary
			if (!(m_stimuli[m_trial_counter].cue_visual || m_stimuli[m_trial_counter].cue_auditory || m_stimuli[m_trial_counter].cue_tactile))
			{

				if (m_block_delta == 1)
					AppLog << "State: No Presentation of cues ..." << endl;

				// go to next state
				m_current_state = e_wait;
				m_block_delta = 0;
			}
			else
			{

				

				if (m_block_delta == 1)
				{
					AppLog << "State: Presenting cues ..." << endl;
				}

				// Commented out to prevent priming cues from starting...
				/*
				// continiously update presentation of visual cue
				if (m_stimuli[m_trial_counter].cue_visual)
					VisualStimulusOn(m_block_delta);
			    */

			    if (m_block_delta == 1)
				{

					// Set Correct Cue Image for This State
					if  (m_stimuli[m_trial_counter].cue_visual)
						mp_cue_image->SetFile( Parameter("VisualCueImageFile") );
					
					if  (m_stimuli[m_trial_counter].cue_auditory)
						mp_cue_image->SetFile( Parameter("AuditoryCueImageFile") );

					if  (m_stimuli[m_trial_counter].cue_tactile)
						mp_cue_image->SetFile( Parameter("TactileCueImageFile") );

					if  ( (m_stimuli[m_trial_counter].cue_visual) && (m_stimuli[m_trial_counter].cue_tactile) )
						mp_cue_image->SetFile( Parameter("AllCueImageFile") );

					// Conceal text box
					mp_text_stimulus->Conceal();

					// Show Cue Image
					CueImageOn();
					//AppLog << "Debug: Cue Image should be on..." << endl;


//                  Comment out to prevent priming cues from starting...
					// present visual stimulus image file
				    if (m_stimuli[m_trial_counter].stim_visual && m_do_priming)
						VisualStimulusOn();

					// present auditory cue
					if (m_stimuli[m_trial_counter].cue_auditory && m_do_priming)
						AuditoryStimulusOn();

					// present tactile cue
					if (m_stimuli[m_trial_counter].cue_tactile && m_do_priming)
						TactileStimulusOn();


				}


			}
		
			// check if cue period has expired
            if(m_block_delta >= m_cue_duration_blocks)
            {
				// go to next state
                m_current_state = e_pre_stim;
                m_block_delta = 0;
            } 

			if ( IsButtonPressed( ) )
            {

				// stop any stimuli
				AllStimulusOff();
				CueImageOff();
				AppLog << "Debug: Cue Image should be off..." << endl;

				// go back to the start
                m_current_state = e_false_start;
                m_block_delta = 0;

				// Make sure text box is showing...
		        mp_text_stimulus->Present();

            } 


            break;
            

		//////////////////////////////////////////////////////////////////////////
		case e_pre_stim:

			m_block_delta++;

			if (m_block_delta == 1)
			{
				AppLog << "State: Preparing subject for stimulus ..." << endl;
				// stop any stimuli
				AllStimulusOff();
				CueImageOff();
/*				if (!m_use_clock)
    			{
					FeedbackPreStim();
*///			}
		        FixationCrossOn();
			}

            if(m_block_delta >= m_feedback_blocks)
			{
/*				if (!m_use_clock)
				{
					FeedbackOff();
*///			}
                m_current_state = e_wait;
                m_block_delta = 0;
            }

			if ( IsButtonPressed( ) )
            {

				// stop any stimuli
				AllStimulusOff();

				// fixation cross off
				FixationCrossOff();

				// go back to the start
                m_current_state = e_false_start;
                m_block_delta = 0;

            } 
           
            break;

		//////////////////////////////////////////////////////////////////////////
        case e_wait:

			m_block_delta++;
		

			// get number of waiting blocks
			if (m_block_delta == 1)
			{
				m_rand_blocks = RandomNumber(m_wait_min_blocks,m_wait_max_blocks);	
				AppLog << "State: Waiting for " << ((float)(m_rand_blocks) * m_block_size_msec) << "ms ..." << endl;
				
				// present fixation cross
				FixationCrossOn();
				
			}

			// check if state has reached the number of waiting blocks
            if (m_block_delta >= m_rand_blocks)
            {				
				// go to next state
                m_current_state = e_present;
                m_block_delta = 0;
                m_rand_blocks = 0;
            } 

			if ( IsButtonPressed( ) )
            {

				// stop any stimuli
				AllStimulusOff();

				// fixation cross off
				FixationCrossOff();

				// go back to the start
                m_current_state = e_false_start;
                m_block_delta = 0;

            } 


            break;

		//////////////////////////////////////////////////////////////////////////
        case e_present:
			
			m_block_delta++;

			// Make sure text box is showing...
		    //mp_text_stimulus->Present();

			if (m_block_delta == 1)
			{
				AppLog << "State: Presenting stimuli ..." << endl;
//				m_time = PrecisionTime::Now();
			}
			
			if (!(m_stimuli[m_trial_counter].stim_visual || m_stimuli[m_trial_counter].stim_auditory || m_stimuli[m_trial_counter].stim_tactile))
			{
				// no stimulus necessary

				// go to next state
				m_current_state = e_isi;
				m_block_delta = 0;
			}
			else
			{
				// continuously update presentation of visual cue // FOR COUNTING CLOCK VISUAL STIMULUS
				if ( (m_stimuli[m_trial_counter].stim_visual) && m_use_clock )
				{
					if (m_block_delta == 1)
					{
						mp_text_stimulus->Present();
					}
					VisualClockStimulusOn(m_block_delta);
				}

			    if (m_block_delta == 1)
				{

					// present auditory cue
					if (m_stimuli[m_trial_counter].stim_auditory)
						AuditoryStimulusOn();

					// present tactile cue
					if (m_stimuli[m_trial_counter].stim_tactile)
						TactileStimulusOn();

					if (!m_use_clock)
					{
					    // present visual stimulus image file
					    if (m_stimuli[m_trial_counter].stim_visual)
					    {
					        FixationCrossOff();
						    VisualStimulusOn();			    
					    }
						
					}

				}

			}
			
			if (m_block_delta > m_rt_max_blocks)
			{

				AllStimulusOff();
				FixationCrossOff();

				// go to too slow feedback
				m_current_state = e_slow;
				m_block_delta = 0;  
			} 
			else if( IsButtonPressed() )
			{

				AppLog << "State: Response time = " << ReactionTime() << "ms ..." << endl;
//				AppLog << m_time.SignedDiff(PrecisionTime::Now(),m_time) << endl;
//				m_time = PrecisionTime::Now();

				// stop any stimuli
				AllStimulusOff();
				FixationCrossOff();

				// too fast
				if (ReactionTime() < m_rt_min)
				{
					// go to too fast feedback
					m_current_state = e_fast;
					m_block_delta = 0;  
				} 
				else
				{
					// go to next state
					m_reaction_time.push_back(ReactionTime());
					m_current_state = e_time;
					m_block_delta = 0;  
				}
        
            } 

            break;

		//////////////////////////////////////////////////////////////////////////
        case e_time:

			m_block_delta++;

			// Make sure text box is showing...
		    mp_text_stimulus->Present();
			
			if (m_block_delta == 1)
			{
				AppLog << "State: Giving feedback for in-time response ..." << endl;
				FeedbackOk();
//				AppLog << m_time.SignedDiff(PrecisionTime::Now(),m_time) << endl;
//				m_time = PrecisionTime::Now();
			}

            if(m_block_delta >= m_feedback_blocks)
			{
				FeedbackOff();
                m_current_state = e_isi;
                m_block_delta = 0;
            }

            break;

		//////////////////////////////////////////////////////////////////////////
        case e_slow:

			m_block_delta++;

			// Make sure text box is showing...
		    mp_text_stimulus->Present();

			if (m_block_delta == 1)
			{
				AppLog << "State: Giving feedback for too slow response ..." << endl;
				FeedbackTooSlow();
				m_too_slow_counter++;
			}

            if(m_block_delta >= m_feedback_blocks)
			{
				FeedbackOff();
                m_current_state = e_gaze;
                m_block_delta = 0;
            }
                    
            break;

		//////////////////////////////////////////////////////////////////////////
		case e_fast:

			m_block_delta++;

			// Make sure text box is showing...
		    mp_text_stimulus->Present();

			if (m_block_delta == 1)
			{
				AppLog << "State: Giving feedback for too fast response ..." << endl;
				FeedbackTooFast();
				m_too_fast_counter++;
			}

            if(m_block_delta >= m_feedback_blocks)
			{
				FeedbackOff();
                m_current_state = e_gaze;
                m_block_delta = 0;
            }
           
            break;

		//////////////////////////////////////////////////////////////////////////
        case e_isi:

			m_block_delta++;

			if (m_block_delta == 1)
			{
				AppLog << "State: Interstimulus interval ..." << endl;
//				AppLog << m_time.SignedDiff(PrecisionTime::Now(),m_time) << endl;
//				m_time = PrecisionTime::Now();
			}

			// check if state has reached the number of waiting blocks
            if(m_block_delta >= m_isi_blocks)
            {
				// go to next state
                m_current_state = e_gaze;
                m_block_delta = 0;
                m_rand_blocks = 0;
				m_trial_counter++;

				if (m_trial_counter == m_num_trials)
				{
					AppLog << "==== Finished !!! =====" << endl;
					State("Running") = 0;
				}

            }

            break;

		//////////////////////////////////////////////////////////////////////////
        case e_false_start:
			
			m_block_delta++;

			// Make sure text box is showing...
		    mp_text_stimulus->Present();

			if (m_block_delta == 1)
			{
				AppLog << "Feedback for false start" << endl;
				FeedbackFalseStart(); 
				m_false_start_counter++;
			}
	
            if(m_block_delta >= m_feedback_blocks)
			{
				FeedbackOff();
                m_current_state = e_gaze;
                m_block_delta = 0;
            }

            break;


        default:
            bcierr<<"Compiler Error. Do not pass go, do not collect $200." <<endl;
    
    }
}

void
ReactionTaskFilter::StopRun()
{

//	bciout << "suspended ..." << endl;

	AllStimulusOff();
	FeedBackFinished();
	SummarizeExperiment();

  // The Running state has been set to 0, either because the user has pressed "Suspend",
  // or because the run has reached its natural end.
  // You know, you can delete methods if you're not using them.
  // Remove the corresponding declaration from ReactionTaskFilter.h too, if so.

}

void
ReactionTaskFilter::FeedBackFinished()
{
//	mp_text_stimulus->SetText(m_text_feedback_task_finished);
//	mp_text_stimulus->SetTextColor(m_text_feedback_pos_color);
//	mp_text_stimulus->Present();

	float reaction_time_runningavg_all				= 0;
	unsigned int i_count                            = 0;

	for (unsigned int i=0; i<m_reaction_time.size(); i++)
	{
		reaction_time_runningavg_all+=m_reaction_time[i];
		i_count++;
	}

	mp_text_stimulus->SetTextColor(m_text_feedback_pos_color);
	m_text_stimulus_string.str("Good Job!\nYour Average Response Time Was\n\n ");
	if (i_count > 0)
		m_text_stimulus_string << m_text_stimulus_string.str() << (unsigned int)(reaction_time_runningavg_all+0.5)/(i_count) << " ms";
	if (i_count < 1)
		m_text_stimulus_string << m_text_stimulus_string.str() << "NULL";
	mp_text_stimulus->SetText(m_text_stimulus_string.str());
	mp_text_stimulus->Present();

}

void
ReactionTaskFilter::AllStimulusOff()
{
	// stop any stimuli
    VisualStimulusOff();
	TactileStimulusOff();
	AuditoryStimulusOff();

}

void
ReactionTaskFilter::FeedbackOff()
{
	mp_text_stimulus->SetText("");
}

void
ReactionTaskFilter::FeedbackOk()
{

	mp_text_stimulus->SetTextColor(m_text_feedback_pos_color);

	if (m_feedback_pos_text && !m_feedback_pos_time) 
	{
		mp_text_stimulus->SetText(m_text_feedback_pos);
	}
	else if (!m_feedback_pos_text && m_feedback_pos_time) 
	{
		m_text_stimulus_string.str("");
		m_text_stimulus_string << (unsigned int)(m_reaction_time[m_reaction_time.size()-1]+0.5) << " ms";
		mp_text_stimulus->SetText(m_text_stimulus_string.str());
	}
	else if (m_feedback_pos_text && m_feedback_pos_time) 
	{
		m_text_stimulus_string.str("");
		m_text_stimulus_string << m_text_feedback_pos << "\n" << (unsigned int)(m_reaction_time[m_reaction_time.size()-1]+0.5) << " ms";
		mp_text_stimulus->SetText(m_text_stimulus_string.str());
	}

}

void
ReactionTaskFilter::FeedbackTooSlow()
{
	mp_text_stimulus->SetTextColor(m_text_feedback_neg_color);
	mp_text_stimulus->SetText(m_text_feedback_neg_too_slow);
}

void
ReactionTaskFilter::FeedbackTooFast()
{
	mp_text_stimulus->SetTextColor(m_text_feedback_neg_color);
	mp_text_stimulus->SetText(m_text_feedback_neg_too_fast);
}

void
ReactionTaskFilter::FeedbackFalseStart()
{
	mp_text_stimulus->SetTextColor(m_text_feedback_neg_color);
	mp_text_stimulus->SetText(m_text_feedback_neg_false_start);
}

void
ReactionTaskFilter::FeedbackPreCue()
{
	mp_text_stimulus->SetTextColor(m_text_feedback_pre_cue_color);
	mp_text_stimulus->SetText(m_text_feedback_pre_cue);
}

void
ReactionTaskFilter::FeedbackPreStim()
{
	mp_text_stimulus->SetTextColor(m_text_feedback_pre_stim_color);
	mp_text_stimulus->SetText(m_text_feedback_pre_stim);
}

int
ReactionTaskFilter::RandomNumber(int min, int max)
{
	int random_number = min + m_random_generator.Random() % (max - min);
	
///	bciout << random_number << endl;

	return random_number;
}


void
ReactionTaskFilter::CueImageOn()
{
	if (mp_cue_image)
	{
		mp_cue_image->Present();
	}
}

void
ReactionTaskFilter::CueImageOff()
{
	if (mp_cue_image)
	{
		mp_cue_image->Conceal();
	}
}

void
ReactionTaskFilter::VisualMarkerOn()
{
	if (m_present_marker) 
	{
		mp_text_marker->SetText("+");
	}
}


void
ReactionTaskFilter::VisualMarkerOff()
{
	if (m_present_marker) 
	{
		mp_text_marker->SetText("");
	}
}

void
ReactionTaskFilter::FixationCrossOn()
{	
	mp_text_stimulus->SetText("+");
	mp_text_stimulus->SetTextColor(m_fix_cross_color);
	mp_text_stimulus->Present();
}

void
ReactionTaskFilter::FixationCrossOff()
{
	mp_text_stimulus->SetText("");
	mp_text_stimulus->Conceal();
}

//FUNCTION FOR COUNTING CLOCK VISUAL STIMULUS
void
ReactionTaskFilter::VisualClockStimulusOn(unsigned int block)
{
	
	m_text_stimulus_string.str("");
	m_text_stimulus_string << (unsigned int)(block * m_block_size_msec);
	mp_text_stimulus->SetTextColor(m_text_stim_color);
	mp_text_stimulus->SetText(m_text_stimulus_string.str());

	if (block == 1)
		VisualMarkerOn();

}

// FOR COUNTING CLOCK VISUAL STIMULUS
void
ReactionTaskFilter::VisualClockStimulusOff()
{
	mp_text_stimulus->SetText("");
	VisualMarkerOff();
	mp_vis_stim_image->Conceal();
}

// FUNCTION FOR IMAGE VISUAL STIMULUS
void
ReactionTaskFilter::VisualStimulusOn()
{

	// Conceal text box
    mp_text_stimulus->Conceal();
	VisualMarkerOn();
    mp_vis_stim_image->Present();

}

void
ReactionTaskFilter::VisualStimulusOff()
{
	VisualMarkerOff();
	mp_vis_stim_image->Conceal();
}

void 
ReactionTaskFilter::AuditoryStimulusOn()
{
	mp_auditory_stimulus->Play();
//	bciout << "auditory stimulus started" << endl; 
}

void 
ReactionTaskFilter::AuditoryStimulusOff()
{
	mp_auditory_stimulus->Stop();
//	bciout << "auditory stimulus stopped" << endl; 
}



void 
ReactionTaskFilter::TactileStimulusOn()
{
	int ports[] = {0, 1, 2, 3, 4, 5, 6, 7};
	int states[] = {1, 1, 1, 1, 1, 1, 1, 1};
	// set the port states
	int error = gSTIMboxsetPortState(m_tactile_stimulus, 8, ports, states);
	if(error) StimBoxErrorMessage(error);
}

void 
ReactionTaskFilter::TactileStimulusOff()
{
	int ports[] = {0, 1, 2, 3, 4, 5, 6, 7};
	int states[] = {0, 0, 0, 0, 0, 0, 0, 0};
	// set the port states
	int error = gSTIMboxsetPortState(m_tactile_stimulus, 8, ports, states);
	if(error) StimBoxErrorMessage(error);
}

bool
ReactionTaskFilter::IsButtonPressed()
{

	//Check for keypresses
	bool press = false;

	if(m_use_space_button) 
	{
		for( unsigned int i = 0; i < m_block_size; i++ )
		{
			if( State( "KeyDown" )( i ) == VK_SPACE )
			{
				press = true;
//				bciout << "key pressed" << endl;
			}
		}	
	} 
	else
	{		
		for( unsigned int i = 0; i < m_block_size; i++ )
		{
			if (mp_input != NULL) {

				if( mp_input->Value(m_push_button_channel-1,i) > m_push_button_threshold )
				{
					press = true;
//					bciout << "button pressed" << endl;
					break;
				}
			}
		}	
		
	}

	return press;
}

float ReactionTaskFilter::ReactionTime( )
{
	unsigned int offset = 0;
	bool press = false; 

	if(m_use_space_button) 
	{
		//Check for keypresses
		for( offset = 0; offset < m_block_size; offset++ )
		{
			if( State( "KeyDown" )( offset ) == VK_SPACE )
			{
				press = true;
				break;
			}
		}	
	}
	else
	{
		for( offset = 0; offset < m_block_size; offset++ )
		{
			if (mp_input != NULL) {

				if( mp_input->Value(m_push_button_channel-1,offset) > m_push_button_threshold )
				{
					press = true;
//					bciout << "button pressed with offset " << offset << endl;
					break;
				}
			}
		}	
	}
		


	if (press) 
	{
		return (float)(m_block_delta-2) * m_block_size_msec + (float)(offset+1) / (float)(m_block_size) * m_block_size_msec;
	}
	else
	{
		return -1;
	}

}


void ReactionTaskFilter::SummarizeExperiment()
{
	float reaction_time_avg_all				= 0;	unsigned int num_all            = 0;
	float reaction_time_avg_visual			= 0;    unsigned int num_visual			= 0;
	float reaction_time_avg_auditory		= 0;	unsigned int num_auditory		= 0;
	float reaction_time_avg_tactile 		= 0;	unsigned int num_tactile		= 0;
	float reaction_time_avg_visual_cued		= 0;	unsigned int num_visual_cued	= 0;
	float reaction_time_avg_auditory_cued	= 0;	unsigned int num_auditory_cued  = 0;
	float reaction_time_avg_tactile_cued	= 0;	unsigned int num_tactile_cued   = 0;
//	float reaction_time_std_all				= 0;	
//	float reaction_time_std_visual			= 0;	
//	float reaction_time_std_auditory		= 0;	
//	float reaction_time_std_tactile 		= 0;	
//	float reaction_time_std_visual_cued		= 0;	
//	float reaction_time_std_auditory_cued	= 0;	
//	float reaction_time_std_tactile_cued	= 0;	

	for (unsigned int i=0; i<m_reaction_time.size(); i++)
	{

		reaction_time_avg_all+=m_reaction_time[i];

		if (m_stimuli[i].stim_visual & m_stimuli[i].cue_visual & m_stimuli[i].cue_auditory & m_stimuli[i].cue_tactile)
		{
			reaction_time_avg_visual+=m_reaction_time[i];
			num_visual++;
		}

		if (m_stimuli[i].stim_auditory & m_stimuli[i].cue_visual & m_stimuli[i].cue_auditory & m_stimuli[i].cue_tactile)
		{
			reaction_time_avg_auditory+=m_reaction_time[i];
			num_auditory++;
		}

		if (m_stimuli[i].stim_tactile & m_stimuli[i].cue_visual & m_stimuli[i].cue_auditory & m_stimuli[i].cue_tactile)
		{
			reaction_time_avg_tactile+=m_reaction_time[i];
			num_tactile++;
		}

		if (m_stimuli[i].stim_visual & m_stimuli[i].cue_visual & !m_stimuli[i].cue_auditory & !m_stimuli[i].cue_tactile)
		{
			reaction_time_avg_visual_cued+=m_reaction_time[i];
			num_visual_cued++;
		}

		if (m_stimuli[i].stim_auditory & !m_stimuli[i].cue_visual & m_stimuli[i].cue_auditory & !m_stimuli[i].cue_tactile)
		{
			reaction_time_avg_auditory_cued+=m_reaction_time[i];
			num_auditory_cued++;
		}

		if (m_stimuli[i].stim_tactile & !m_stimuli[i].cue_visual & !m_stimuli[i].cue_auditory & m_stimuli[i].cue_tactile)
		{
			reaction_time_avg_tactile_cued+=m_reaction_time[i];
			num_tactile_cued++;
		}
	}


	if (m_reaction_time.size() > 0)
		reaction_time_avg_all = reaction_time_avg_all / ((float)(m_reaction_time.size()));

	if (num_visual > 0) 
		reaction_time_avg_visual = reaction_time_avg_visual / ((float)(num_visual));

	if (num_auditory > 0) 
		reaction_time_avg_auditory = reaction_time_avg_auditory / ((float)(num_auditory));

	if (num_tactile > 0) 
		reaction_time_avg_tactile = reaction_time_avg_tactile / ((float)(num_tactile));

	if (num_visual_cued > 0) 
		reaction_time_avg_visual_cued = reaction_time_avg_visual_cued / ((float)(num_visual_cued));

	if (num_auditory_cued > 0) 
		reaction_time_avg_auditory_cued = reaction_time_avg_auditory_cued / ((float)(num_auditory_cued));

	if (num_tactile_cued > 0) 
		reaction_time_avg_tactile_cued = reaction_time_avg_tactile_cued / ((float)(num_tactile_cued));

	AppLog << "=============================================" << endl;
	AppLog << "*************** S U M M A R Y ***************" << endl;
	AppLog << "=============================================" << endl;
	AppLog << "---------------------------------------------" << endl;
	//AppLog << "Total Runtime: " << floor(fmod(( m_runtime_counter * m_block_size_msec ) / 1000 / 60, fmod(( m_runtime_counter * m_block_size_msec ) / 1000, 60) )) << ":" << floor(fmod(( m_runtime_counter * m_block_size_msec ) / 1000, 60)) << " mins" << endl;
	AppLog << "Total Runtime: " << ( m_runtime_counter * m_block_size_msec ) / 1000 << "s" << endl;
	AppLog << "Total trials: " << m_reaction_time.size() << endl;
	AppLog << "False starts: " << m_false_start_counter << endl;
	AppLog << "Response too fast: " << m_too_fast_counter << endl;
	AppLog << "Response too slow: " << m_too_slow_counter << endl; 
	AppLog << "Average response time: " << reaction_time_avg_all << "ms" << endl; 
	AppLog << "Un-cued Visual : " << reaction_time_avg_visual << "ms" << endl; 
	AppLog << "Un-cued Auditory : " << reaction_time_avg_auditory << "ms" << endl; 
	AppLog << "Un-cued Tactile : " << reaction_time_avg_tactile << "ms" << endl; 
	AppLog << "Cued Visual : " << reaction_time_avg_visual_cued << "ms" << endl; 
	AppLog << "Cued Auditory : " << reaction_time_avg_auditory_cued << "ms" << endl; 
	AppLog << "Cued Tactile : " << reaction_time_avg_tactile_cued << "ms" << endl; 
	AppLog << "---------------------------------------------" << endl;
	AppLog << "=============================================" << endl;

	


}


void ReactionTaskFilter::StimBoxErrorMessage(int error)
{
//	bciout<<"g.STIMbox error code " << error << endl;

	// allocating storage for the error message
	int messagelength = 50;
	char* errormessage = new char[messagelength];
	if(!errormessage)
	{
		bciout << "Unable to retrieve  g.STIMbox error message!" << endl;
	}
	else
	{
		// try to retrieve and print error message
		int error1 = gSTIMboxgetErrorMessage(error, messagelength, errormessage);
		if(error1)
		{
			bciout << "Unable to retrieve g.STIMbox error message!" << endl;
		}
		else
		{
			bciout << errormessage << "\n";
			delete[] errormessage;
		}
	}

}


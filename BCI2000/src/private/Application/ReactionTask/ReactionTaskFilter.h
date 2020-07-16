////////////////////////////////////////////////////////////////////////////////
// $Id: $
// Authors: 
// Description: ReactionTaskFilter header
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

#ifndef INCLUDED_ReactionTaskFilter_H  // makes sure this header is not included more than once
#define INCLUDED_ReactionTaskFilter_H

#include <Map>
#include <string>
#include <sstream>
#include "ApplicationBase.h"
#include "Stimulus.h"
#include "ImageStimulus.h"
#include "TextStimulus.h"
#include "WavePlayer.h"
#include "RandomGenerator.h"
//#include "PrecisionTime.h"
#include "ImageStimulus.h"

enum e_state {e_gaze=1, e_pre_cue, e_cue, e_pre_stim, e_wait, e_present, e_time, e_slow, e_fast, e_isi, e_false_start, e_first_trial};

struct Trial
{
  public:
    Trial()
    {
		cue_visual		= false;
		cue_auditory	= false;
		cue_tactile		= false;
		stim_visual		= false;
		stim_auditory	= false;
		stim_tactile	= false;
    }

    //Member variables
    bool cue_visual;
    bool cue_auditory;
    bool cue_tactile;
	bool stim_visual;
	bool stim_auditory;
	bool stim_tactile; 
};

class ReactionTaskFilter : public ApplicationBase
{
 public:
           ReactionTaskFilter();
  virtual ~ReactionTaskFilter();
  virtual void Halt();
  virtual void Preflight(  const SignalProperties& Input,       SignalProperties& Output ) const;
  virtual void Initialize( const SignalProperties& Input, const SignalProperties& Output );
  virtual void StartRun();
  virtual void Process(    const GenericSignal&    Input,       GenericSignal&    Output );
  virtual void StopRun();

 private:
    
    ApplicationWindow& mr_window;
    e_state m_current_state;

	bool		  m_present_marker;
	bool          m_use_space_button;
	bool		  m_feedback_pos_text;
	bool	      m_feedback_pos_time;
	bool          m_do_priming;
	bool          m_do_pre_cue;
	bool          m_is_first_trial;
	bool          m_use_clock;
	unsigned int  m_push_button_channel; 
	float         m_push_button_threshold; 
    unsigned int  m_block_delta;
    unsigned int  m_sample_rate;
    unsigned int  m_block_size;
	float         m_block_size_msec;
	unsigned int  m_cue_duration_blocks;
	unsigned int  m_wait_min_blocks;
	unsigned int  m_wait_max_blocks;
    unsigned int  m_rand_blocks;
	float         m_rt_min;
	float         m_rt_max;
	float         m_rt_min_blocks;
	float         m_rt_max_blocks;
	unsigned int  m_isi_blocks;
	unsigned int  m_feedback_blocks;
	unsigned int  m_num_trials;
	unsigned int  m_trial_counter;
	unsigned int  m_runtime_counter;

    GUI::Rect     m_full_rectangle;
	GUI::Rect     m_marker_rectangle;
	RGBColor      m_text_stim_color;
	RGBColor      m_text_feedback_pos_color;
	RGBColor      m_text_feedback_neg_color;
	RGBColor      m_text_feedback_pre_cue_color;
	RGBColor      m_text_feedback_pre_stim_color;
	RGBColor      m_fix_cross_color;
	float         m_text_height;
	std::string   m_text_feedback_pos; 
	std::string   m_text_feedback_neg_false_start;
	std::string   m_text_feedback_neg_too_fast;
	std::string   m_text_feedback_neg_too_slow;
	std::string   m_text_feedback_pre_cue;
	std::string   m_text_feedback_pre_stim;
	std::string   m_text_feedback_task_finished;

	std::vector<Trial> m_stimuli;
	std::vector<float> m_reaction_time;
	unsigned int  m_too_fast_counter; 
	unsigned int  m_too_slow_counter;
	unsigned int  m_false_start_counter;

	WavePlayer   *mp_auditory_stimulus; 
	TextStimulus *mp_text_stimulus;
    void*        m_tactile_stimulus;

	TextStimulus *mp_text_marker;

	GUI::Rect     m_cue_image_rectangle;
	ImageStimulus *mp_cue_image;

	GUI::Rect     m_stim_image_rectangle;
	ImageStimulus *mp_vis_stim_image;

    std::stringstream m_text_stimulus_string;
	RandomGenerator m_random_generator;

	const GenericSignal *mp_input; 
//	PrecisionTime m_time;

    // Use this space to declare any APPWINDOW-specific methods and member variables you'll need
    bool IsButtonPressed(); 
	void StimBoxErrorMessage(int error);
    int RandomNumber(int min, int max);

	void CueImageOn();
	void CueImageOff();

	void VisualMarkerOn();
	void VisualMarkerOff();

	void VisualStimulusOn(); // FOR STATIC (.bmp) IMAGE VISUAL STIMULUS
	void VisualStimulusOff();

	void VisualClockStimulusOn(unsigned int block); // FOR COUNTING CLOCK VISUAL STIMULUS
	void VisualClockStimulusOff();

	void FixationCrossOn();
	void FixationCrossOff();

	void AuditoryStimulusOn();
	void AuditoryStimulusOff();

	void TactileStimulusOn();
	void TactileStimulusOff();

	void FeedbackPreCue();
	void FeedbackPreStim();
	void FeedbackFalseStart();
	void FeedbackOk();
	void FeedbackTooSlow();
    void FeedbackTooFast();
	void FeedbackOff();
	void FeedBackFinished();
	
	void AllStimulusOff();
	
	float ReactionTime();

	void SummarizeExperiment();


};

#endif // INCLUDED_ReactionTaskFilter_H

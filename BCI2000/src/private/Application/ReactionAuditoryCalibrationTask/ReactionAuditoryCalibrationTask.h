////////////////////////////////////////////////////////////////////////////////
// $Id: $
// Authors: 
// Description: ReactionAuditoryCalibrationTask header
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

#ifndef INCLUDED_ReactionAuditoryCalibrationTask_H  // makes sure this header is not included more than once
#define INCLUDED_ReactionAuditoryCalibrationTask_H

#include "ApplicationBase.h"
#include "WavePlayer.h"
#include <string>
#include "TextStimulus.h"
#include "ImageStimulus.h"

enum e_state {e_first_trial, e_soundcheck, e_cue, e_pre_stimuli,  e_stimuli, e_feedback, e_isi, e_prePEST, e_PEST, e_false_start};

class ReactionAuditoryCalibrationTask : public ApplicationBase
{
 public:
           ReactionAuditoryCalibrationTask();
  virtual ~ReactionAuditoryCalibrationTask();
  virtual void Halt();
  virtual void Preflight(  const SignalProperties& Input,       SignalProperties& Output ) const;
  virtual void Initialize( const SignalProperties& Input, const SignalProperties& Output );
  virtual void StartRun();
  virtual void Process(    const GenericSignal&    Input,       GenericSignal&    Output );
  virtual void StopRun();

 private:
   ApplicationWindow& mrDisplay;
    e_state m_current_state;
   // Use this space to declare any APPWINDOW-specific methods and member variables you'll need

    int m_num_stimuli;

    float         m_attenuation_min;
    float         m_attenuation_max;
    float         m_attenuation_stepping;
    float         m_PestStartLevel;           // starting value for intensity                 
	float         m_PestFinalStepSize;        // smallest value for intensity change     
	float         m_PestWaldFactor;           // number of missed trials x target likelihood without intensity change     
	float         m_PestTargetPerformance;    // target performance ratio
    float         m_PestStepsize;
    float         m_PestStartStepSize;
    float         m_auditory_intensity;
    float         m_PestMaxstepsize;
    float         m_PestMinlevel;
    float         m_min_rt; 
    float         m_max_rt;  
    float         m_block_size_msec;
    float         m_push_button_threshold; 
    float         m_copy_stimulus_level;

	unsigned int  m_runtime_counter;
    unsigned int  m_num_trials;
	unsigned int  m_trial_counter;
    unsigned int  m_block_delta;
    unsigned int  m_block_size;
    unsigned int  m_cue_duration_blocks;
    unsigned int  m_isi_duration_blocks;
    unsigned int  m_soundcheck_duration_blocks;
    unsigned int  m_stimuli_duration_blocks;
    unsigned int  m_feedback_blocks;
    unsigned int  m_sample_rate;
    unsigned int  m_push_button_channel; 
    unsigned int  m_rand_blocks;
    unsigned int  m_wait_min_blocks;
	unsigned int  m_wait_max_blocks;
    unsigned int  m_Pest_num_correct;
    unsigned int  m_historylength;
    unsigned int  m_Pest_doubled;
    unsigned int m_Pest_samelevelcounter;
    unsigned int m_Pesttrialcounter;
    unsigned int m_max_num_trials;
    bool          m_is_first_trial;
    bool          m_do_pre_cue;
    bool          m_use_space_button;
    bool		  m_present_marker;
    bool          m_use_Digital_input;
       
    const GenericSignal *mp_input; 
    GUI::Rect     m_cue_image_rectangle;
    GUI::Rect     m_marker_rectangle;
    GUI::Rect     m_full_rectangle;

    ImageStimulus *mp_cue_image;
    RGBColor      m_text_feedback_pre_cue_color;
    RGBColor      m_text_feedback_positive_color;
    RGBColor      m_text_feedback_negative_color;

    TextStimulus *mp_text_marker;

     WavePlayer *p_warning_tone;

    std::vector<WavePlayer*> m_list_auditory_stimuli;
    std::vector<float> m_reaction_time;
    std::vector<float> m_stimulus_Level;
    std::vector<float> m_stimulus_Level_copy;
    std::vector<float> m_Pest_percentage_check;
    std::vector<float> m_PEST_performance;
    std::vector<float> m_list_attenuation; 
    std::string         m_Pest_history_correct;
    std::string         a;
    std::string         b;
    std::string         m_text_feedback_pre_cue;
    std::stringstream   m_text_stimulus_string;
    std::string         m_text_feedback_false_start;

    TextStimulus        *mp_text_stimulus;

    unsigned int CalculateStimulusIndex(float attenuation);
    void StimulusOn( float attenuation );
    void StimulusOff( float attenuation );
    void CueImageOn();
	void CueImageOff();
    void SummarizeExperiment();
    void VisualMarkerOn();
    void VisualMarkerOff();
    float ReactionTime();
    bool IsButtonPressed();
    unsigned int m_pushed_botton_flag;
    int RandomNumber(int min, int max);
    RandomGenerator m_random_generator;
};

#endif // INCLUDED_ReactionAuditoryCalibrationTask_H

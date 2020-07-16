////////////////////////////////////////////////////////////////////////////////
// Authors: 
// Description: ReactionAuditoryTask header
////////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_ReactionAuditoryTask_H  // makes sure this header is not included more than once
#define INCLUDED_ReactionAuditoryTask_H

#include "ApplicationBase.h"
#include "TextStimulus.h"
#include "ImageStimulus.h"
#include "WavePlayer.h"
#include <string>

enum e_state {e_first_trial, e_soundcheck, e_cue, e_pre_stimuli,  e_stimuli, e_feedback, e_isi, e_rest, e_false_start};

class ReactionAuditoryTask : public ApplicationBase
{
 public:
  ReactionAuditoryTask();
  virtual ~ReactionAuditoryTask();
  virtual void Halt();
  virtual void Preflight( const SignalProperties& Input, SignalProperties& Output ) const;
  virtual void Initialize( const SignalProperties& Input, const SignalProperties& Output );
  virtual void StartRun();
  virtual void Process( const GenericSignal& Input, GenericSignal& Output );
  virtual void StopRun();


 private:
   ApplicationWindow& mrDisplay;
   e_state m_current_state;

    int           m_num_stimuli;
    unsigned int  m_runtime_counter;
    unsigned int  m_trial_counter;
	  unsigned int  m_push_button_channel;
	  unsigned int  m_push_button_threshold;
    unsigned int  m_sample_rate;
    unsigned int  m_block_size;
    unsigned int  m_isi_duration_blocks;
    unsigned int  m_rest_duration_blocks;
    unsigned int  m_cue_duration_blocks;
    unsigned int  m_soundcheck_duration_blocks;
    unsigned int  m_wait_min_blocks;
    unsigned int  m_wait_max_blocks;
    unsigned int  m_stimuli_duration_blocks;
    unsigned int  m_feedback_blocks;
    unsigned int  m_block_delta;
    unsigned int  m_num_trials;
    unsigned int  m_rand_blocks;
    unsigned int  m_num_sequence;
    unsigned int  m_num_sequence_col;
    unsigned int  m_perception_threshold_index;
    unsigned int  m_number_blocks;
    unsigned int  m_counter_blocks;
    unsigned int  m_total_number_blocks;
    unsigned int  m_total_number_zero_intensity;
    unsigned int  m_count_trials_rest;
    unsigned int  m_stimuli_intensity_pos_delta;
    unsigned int  m_stimuli_intensity_neg_delta;
    unsigned int  m_pushed_botton_flag;
    unsigned int  m_stimulus_onset_flag;
    unsigned int  m_num_unresponded_trial_counter;

    bool          m_is_first_trial;
    bool          m_do_pre_cue;
    bool          m_use_space_button;
    bool          m_use_Digital_input;

    float         m_text_height;
    float         m_attenuation_min;
    float         m_attenuation_max;
    float         m_perception_threshold;
    float         m_min_rt; 
    float         m_max_rt;  
    float         m_block_size_msec;
    float         m_zero_intensity_percentage;

    GUI::Rect     m_full_rectangle;
    GUI::Rect     m_cue_image_rectangle;
    GUI::Rect     m_marker_rectangle;
    RGBColor      m_text_feedback_pre_cue_color;
    RGBColor      m_text_feedback_positive_color;
    RGBColor      m_text_feedback_negative_color;
   
    TextStimulus *mp_text_stimulus;
    TextStimulus *mp_text_marker;
    ImageStimulus *mp_cue_image;

    WavePlayer *p_warning_tone;
    WavePlayer *p_sound_hearing_check_tone;

    const GenericSignal *mp_input;

    std::vector<WavePlayer*> m_list_auditory_stimuli;
    std::vector<float> m_reaction_time;
    std::vector<float> m_stimuli_sequence;
    std::vector<float> m_stimuli_intensity;
    std::vector<float> m_stimuli_intensity_trial;
    std::vector<float> m_subject_performance;
    std::vector<float> m_stimulus_Level_copy;
    std::vector<float> m_Pest_percentage_check;
    std::vector<float> m_list_attenuation; 
    std::vector<float> m_zero_intensity;
    std::vector<float> m_vector;

    std::stringstream m_text_stimulus_string;
    std::string       m_text_feedback_pre_cue;
    std::string       m_text_feedback_false_start;

    unsigned int CalculateStimulusIndex(float attenuation);
    void Randperm(int* matrix, int size);
    void StimulusOn( unsigned int idx );
    void StimulusOff( unsigned int idx );
    void CueImageOn();
	void CueImageOff();
    void SummarizeExperiment();
    void VisualMarkerOn();
    void VisualMarkerOff();
    float ReactionTime();
    int RandomNumber(int min, int max);
    bool IsButtonPressed();
    RandomGenerator m_random_generator;
};

#endif // INCLUDED_ReactionAuditoryTask_H

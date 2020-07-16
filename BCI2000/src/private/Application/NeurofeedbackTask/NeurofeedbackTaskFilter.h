////////////////////////////////////////////////////////////////////////////////
// $Id: $
// Authors: 
// Description: NeurofeedbackTaskFilter header
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

#ifndef INCLUDED_NeurofeedbackTaskFilter_H  // makes sure this header is not included more than once
#define INCLUDED_NeurofeedbackTaskFilter_H

#include "ApplicationBase.h"
#include "ApplicationWindow.h"
#include <Map>
#include <string>
#include <sstream>
#include <qthread.h>
#include "Stimulus.h"
#include "ImageStimulus.h"
#include "TextStimulus.h"
#include "WavePlayer.h"
#include "RandomGenerator.h"
#include "ImageStimulus.h"
#include "SpeechStimulus.h"
#include "TextToSpeech.h"
#include "BarGauge.h"
#include "Gauge.h"
#include "TextField.h"

enum e_state	{e_baseline=1, e_pain_rating, e_pain_exception, e_training, e_relax, e_stimulus_anticipation, e_stimulus_delivery, e_reward, e_randomize};
enum e_protocol {e_baseline_prot=1, e_training_prot, e_neurofeedback_prot};

 class CountDownThread : public QThread
 {

 private:

	TextToSpeech		m_speech_countdown;
    std::stringstream   m_text_speech_countdown;
	int                 m_counter;

 public:
	 void SetCounter(int counter) 
	 {
		 m_counter = counter;
	 }
     void run() 
	 {
		m_text_speech_countdown.str(std::string());
		m_text_speech_countdown << m_counter;
		m_speech_countdown.SetText(m_text_speech_countdown.str());
		m_speech_countdown.Speak();		
	 }

 };
 
class NeurofeedbackTaskFilter : public ApplicationBase
{
 public:
           NeurofeedbackTaskFilter();
  virtual ~NeurofeedbackTaskFilter();
  virtual void Halt();
  virtual void Preflight(  const SignalProperties& Input,       SignalProperties& Output ) const;
  virtual void Initialize( const SignalProperties& Input, const SignalProperties& Output );
  virtual void StartRun();
  virtual void Process(    const GenericSignal&    Input,       GenericSignal&    Output );
  virtual void StopRun();

 private:
   ApplicationWindow& mrDisplay;
   // Use this space to declare any APPWINDOW-specific methods and member variables you'll need
    e_state				m_current_state;

    unsigned int		m_sample_rate;
    unsigned int		m_block_size;
	float				m_block_size_msec;

	unsigned int		m_trial_counter;
    unsigned int		m_block_delta;
	/*unsigned int		m_relax_runtime_counter;
	unsigned int		m_feedback_runtime_counter;
	double				m_relax_power_sum;
	double				m_feedback_power_sum;
	double				m_trial_relax_power_sum;
	double				m_trial_feedback_power_sum;
	unsigned int		m_trial_relax_runtime_counter;
	unsigned int		m_trial_feedback_runtime_counter;*/

	unsigned int		m_counter;
	double				m_product_sum;
	double				m_period_sum;
	double				m_value_sum;
	double				m_period_squared_sum;
	double				m_value_squared_sum;

	unsigned int		m_counter_trial;
	double				m_product_sum_trial;
	double				m_period_sum_trial;
	double				m_value_sum_trial;
	double				m_period_squared_sum_trial;
	double				m_value_squared_sum_trial;

	unsigned int		m_protocol;
	unsigned int		m_signal;
	unsigned int		m_max_pain_rating_to_proceed;
	float				m_focus_threshold;
	double				m_min_threshold;
	double				m_max_threshold;
	float               m_green_threshold;
	float               m_red_threshold;
	float               m_mid_threshold;
	bool				m_baseline_pain_rating;
	unsigned int		m_display_feedback;
	unsigned int		m_test_session;
	bool				m_anticipation_NF;
	float				m_running_sum;

	
	unsigned int		m_num_trials;
	unsigned int		m_num_baseline_prot_trials;
	unsigned int		m_num_training_prot_trials;
	unsigned int		m_num_anticipation_prot_trials;
	unsigned int		m_num_stimulus_prot_trials;

	unsigned int		m_stim_anticipation_period_min_seconds;
	unsigned int		m_stim_anticipation_period_max_seconds;

	unsigned int		m_baseline_period_blocks;
	unsigned int		m_pain_exception_period_blocks;
	unsigned int		m_relax_period_blocks;
	unsigned int		m_training_period_blocks;
	unsigned int		m_stim_anticipation_period_blocks;
	unsigned int		m_stim_anticipation_period_min_blocks;
	unsigned int		m_stim_anticipation_period_max_blocks;
	unsigned int		m_stim_period_blocks;
	unsigned int		m_reward_period_blocks;

	int					m_pain_rating;
	int					m_pain_exception;
	int					m_blocks_to_wait;

	GUI::Rect			m_full_rectangle;

	TextStimulus		*mp_text_stimulus;

    std::string			m_text_baseline_string;
    std::string			m_text_pain_rating_string;
    std::string			m_text_pain_exception_string;
    std::string			m_text_relax_string;
	std::string			m_text_training_string;
    std::string			m_text_anti_string;
	std::string			m_text_stim_string;
	std::string			m_text_reward_string;

	RGBColor			m_text_baseline_color;
	RGBColor			m_text_pain_rating_color;
	RGBColor			m_text_pain_exception_color;
	RGBColor			m_text_relax_color;
	RGBColor			m_text_training_color;
	RGBColor			m_text_anti_color;
	RGBColor			m_text_stim_color;
	RGBColor			m_text_reward_color;

	RandomGenerator		m_random_generator;

	GUI::Rect			m_timeout_rectangle;
	GUI::Rect			m_image_baseline_rectangle;
	GUI::Rect			m_image_pain_rating_rectangle;
	GUI::Rect			m_image_pain_exception_rectangle;
	GUI::Rect			m_image_relax_rectangle;
	GUI::Rect			m_image_training_rectangle;
	GUI::Rect			m_image_anti_rectangle;
	GUI::Rect			m_image_stim_rectangle;
	GUI::Rect			m_image_reward_rectangle;

	ImageStimulus		*mp_image_baseline;
	ImageStimulus		*mp_image_pain_rating;
	ImageStimulus		*mp_image_pain_exception;
	ImageStimulus		*mp_image_relax;
	ImageStimulus		*mp_image_training;
	ImageStimulus		*mp_image_anti;
	ImageStimulus		*mp_image_stim;
	ImageStimulus		*mp_image_reward;
	RectangularShape	*mp_shaded_region;


	CountDownThread     m_count_down_thread;
	SpeechStimulus		m_speech_output;
    std::stringstream   m_text_speech_output;


	float				m_text_height;

	GUI::Rect			m_gauge_rectangle;
	GUI::Rect			m_gauge_hide_rectangle;
	GUI::Rect			m_gauge_target_rectangle;
	
	BarGauge			*mp_bar_gauge;

	TextField*         mpMessage;


	bool IsButtonPressed(); 
    int RandomNumber(int min, int max);
	int ReadKeyboardLoggerNumber();

	void VisualAnticipationImageOn(); 
	void VisualAnticipationImageOff();

	void VisualStimulusImageOn(); 
	void VisualStimulusImageOff();

	void VisualRewardImageOn(); 
	void VisualRewardImageOff();

	void Speak(std::string sz_text);
	void Feedback(std::string sz_text, RGBColor color, bool b_speak);
	void DisplayMessage(const std::string&);
	void ResetStates();
	void SummarizeExperiment();
	void GetPowerSums(const GenericSignal& sig);
	float RunningAverage();


 protected:
  bool InitDimension( Gauge* gauge, unsigned int gaugeIndex, unsigned int whichDim, unsigned int nChannels, bool force=false );

 private:
  void DeleteGauges();
  void HideGauges();
  void ShowGauges();
  std::vector< Gauge* > mpGauges;
  int mUpdateTime;
  int mTimeSinceLastUpdate;
  double mTheoreticalMilliseconds;

  bool        GaugeHasAttr( int inGaugeIndex, std::string inAttrName ) const;
  double      GetGaugeAttr( int inGaugeIndex, std::string inAttrName, double      inDefaultValue, bool inRequired=false ) const;
  std::string GetGaugeAttr( int inGaugeIndex, std::string inAttrName, std::string inDefaultValue, bool inRequired=false ) const;
  ParamRef    GetGaugeAttr( int inGaugeIndex, std::string inAttrName, bool &      outFound,       bool inRequired=false ) const;

};





#endif // INCLUDED_NeurofeedbackTaskFilter_H

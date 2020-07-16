////////////////////////////////////////////////////////////////////////////////
// $Id: gEstimFilter.cpp 4536 2013-08-05 14:30:13Z mellinger $
// Author: kaleb.goering@gmail.com
// Description: A filter to interact with gEstim device
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
#include "gEstimFilter.h"
#include "BCIEvent.h"
#include "Environment.h"

#include <vector>
#include <cstdio>
#include <sstream>
#include <cmath>

using namespace std;

// Helper function
void fieldsChangedCallback(PGDevice d, uint64_t changed_flags){}

RegisterFilter( gEstimFilter, 3.D );

gEstimFilter::gEstimFilter() : 
	mEstim(NULL), 
	mEstimActive(false), 
	mpgEstimThread(NULL), 
	mStimulating(false) {
}

gEstimFilter::~gEstimFilter() 
{
  Halt();
}

void gEstimFilter::Publish() 
{
	if (!OptionalParameter("ActivateEstim", 0)) return;

	// Define the gEstimFilter Parameters
	BEGIN_PARAMETER_DEFINITIONS

		"gEstim:gEstim int ActivateEstim= 1 0 0 1"
			" // allow use of g.Estim device (boolean)",

		"gEstim:Stimulation string StimulationExpression= % % % % "
			" // Expression to start g.Estim stimulation",

		"gEstim:Stimulation string AbortExpression= % % % % "
			" // Expression to abort g.Estim stimulation",

		"gEstim:Stimulation int Modularity= 1 1 0 1 "
			" // Modularity of pulses: 1 Biphasic, 0 Monophasic (enumeration)",

		"gEstim:Stimulation int Polarity= 1 1 0 1 "
			" // Polarity of pulses: 1 Alternating, 0 Steady (enumeration)",

		"gEstim:Stimulation int PulseDuration= 50 50 50 1000 "
			" // Length of pulse in microseconds [10us]",

		"gEstim:Stimulation int InterphaseDuration= 0 0 0 1000 "
			" // Length of interphase in microseconds [10us]",

		"gEstim:Stimulation int Amplitude= 50 50 50  15000 "
			"// Amplitude of the pulse in microamps [10uA]",

		"gEstim:Stimulation float Frequency= 0.1 0.1 0.1 500 "
			" // Frequency of the pulses in Hz [0.1Hz]",

		"gEstim:Stimulation int NumOfPulses= 1 1 1 30000 "
			" // Number of pulses",

		"gEstim:Stimulation float FrequencyOfTrains= 0.00001 0.00001 0.00001 5 "
			" // Frequency of trains [0.00001Hz]",

		"gEstim:Stimulation int NumOfTrains= 1 1 1 1000 "
			" // Number of trains",

		"gEstim:Stimulation int Jitter= 0 0 0 100 "
			" // Jitter of train [1%]",

		"gEstim:Electrode int ElectrodeType= 1 1 1 3 "
			" // Type of electrode being used: 1 Circle, 2 Depth, 3 Other (enumeration)",
		/*
		"gEstim:Electrode int UsingCircleElectrode= 1 0 0 1 "
			" // Is the electrode a circle electrode (boolean)",
		*/
		"gEstim:Electrode int CircleDiameter= 2000 2000 1000 90000 "
			" // Exposed diameter of the electrode in micrometers [1um]",
		/*
		"gEstim:Electrode int UsingDepthElectrode= 0 0 0 1 "
			" // Is the electrode a depth electrode (boolean)",
		*/
		"gEstim:Electrode int ContactDiameter= 2000 2000 1000 90000 "
			" // Contact diameter of the electrode in micrometers [1um]",

		"gEstim:Electrode int ContactLength= 1000 1000 1000 90000 "
			" // Contact length of the electrode in micrometers [1um]",
		/*
		"gEstim:Electrode int UsingOtherElectrode= 0 0 0 1 "
			" // Is the electrode neither circle nor depth (boolean)",
		*/
		"gEstim:Electrode int ExposedSurfaceArea= 1750000000 1750000000 1750000000 30000000000 "
			" // Exposed surface area of electrode in square micrometers [1um^2]",

	END_PARAMETER_DEFINITIONS

	// Define the gEstimFilter States
	BEGIN_STATE_DEFINITIONS

		"EstimStimulus 1 0 0 0",

	END_STATE_DEFINITIONS
}

void gEstimFilter::Preflight( const SignalProperties &Input, SignalProperties &Output ) const 
{
	Output = Input;
	if ((bool)Parameter("ActivateEstim")) 
	{
		// Test for devices
		openDevices(true);
		PGDevice *pd = gt_openDevices();
		int availableDevices = count_openDevices();
		closeDevices();
		if (availableDevices < 1)
			bcierr << "No g.Estim devices found" << endl;
		else if (availableDevices > 1)
			bcierr << "Multiple g.Estim devices found" << endl;

		// Test expressions
		GenericSignal preflightSignals(Input);
		Expression stimExpression = Parameter("StimulationExpression");
		stimExpression.Compile();
		stimExpression.Evaluate(&preflightSignals);
		Expression abortExpression = Parameter("AbortExpression");
		abortExpression.Compile();
		abortExpression.Evaluate(&preflightSignals);

		// Warn if Monophasic and steady
		if ((!Parameter("Modularity") && !Parameter("Polarity")))
			bciwarn << "Stimulation is set to steady monophasic pulses which will cause charge to accumulate" << endl;

		// Check if an electrode, and only one, was chosen (stress tested done)
		/*
		bool circ = Parameter("UsingCircleElectrode");
		bool depth = Parameter("UsingDepthElectrode");
		bool other = Parameter("UsingOtherElectrode");
		if((circ && depth) || (circ && other) || (depth && other) || !(circ || depth || other))
			bcierr << "One and only one electrode type must be selected" << endl;
		*/
	}
}

void gEstimFilter::Initialize( const SignalProperties &Input, const SignalProperties &Output ) 
{
	mEstimActive = (bool)Parameter("ActivateEstim");
	if (mEstimActive) 
	{
		bcidbg(0) << "This filter will NOT work in debug mode" << endl;
		
		mEstim = NULL;
		if (openDevice(&mEstim, NULL, true)) 
		{
			// Start box and self-diagnostics
			selftest(mEstim); // Must be run

			st_operationmode(mEstim, GESTIM_MODE_BASIC);
			wait_for_end_of_action(mEstim);
			register_with_polling(mEstim, fieldsChangedCallback);			

			// Get parameters
			mActivateExp         = (Expression)Parameter("StimulationExpression");
			mAbortExp            = (Expression)Parameter("AbortExpression");
			int modul            = Parameter("Modularity");
			int polar            = Parameter("Polarity");
			int pulseLength      = Parameter("PulseDuration");
			int interphaseLength = Parameter("InterphaseDuration");
			int pulseAmplitude   = Parameter("Amplitude");
			float pulseFrequency = Parameter("Frequency");
			int numberPulses     = Parameter("NumOfPulses");
			float freqOfTrains   = Parameter("FrequencyOfTrains");
			int numberTrains     = Parameter("NumOfTrains");
			int jitter           = Parameter("Jitter");

			switch ((int)Parameter("ElectrodeType"))
			{
			case 1:
				st_el_type(mEstim, ELECTRODE_CIRCULAR);
				st_el_circ_diameter(mEstim, (int)Parameter("CircleDiameter"));
				break;
			case 2:
				st_el_type(mEstim, ELECTRODE_DEPTH);
				st_el_depth_diameter(mEstim, (int)Parameter("ContactDiameter"));
				st_el_depth_length(mEstim, (int)Parameter("ContactLength"));
				break;
			case 3:
				st_el_type(mEstim, ELECTRODE_OTHER);
				st_el_other_surface(mEstim, (int)Parameter("ExposedSurfaceArea"));
				break;
			}

			/*
			if(Parameter("UsingCircleElectrode")) {
				st_el_type(mEstim, ELECTRODE_CIRCULAR);
				st_el_circ_diameter(mEstim, (int)Parameter("CircleDiameter"));
			} else if(Parameter("UsingDepthElectrode")) { // No need to reevaluate by using if since only one will be true
				st_el_type(mEstim, ELECTRODE_DEPTH);
				st_el_depth_diameter(mEstim, (int)Parameter("ContactDiameter"));
				st_el_depth_length(mEstim, (int)Parameter("ContactLength"));
			} else { // All ready checked for an electrode type to be choosen thus only one option left
				st_el_type(mEstim, ELECTRODE_OTHER);
				st_el_other_surface(mEstim, (int)Parameter("ExposedSurfaceArea"));
			} 
			*/

			// Convert values into ints
			if (pulseLength % 10 > 0)
				bciwarn << "PulseDuration was not an increment of 10 thus will be rounded down to nearest multiple of 10" << endl;
			pulseLength /= 10;
			if (interphaseLength % 10 > 0)
				bciwarn << "InterphaseDuration was not an increment of 10 thus will be rounded down to nearest multiple of 10" << endl;
			interphaseLength /= 10;
			if (pulseAmplitude % 10 > 0)
				bciwarn << "Amplitude was not an increment of 10 thus will be rounded down to nearest multiple of 10" << endl;
			pulseAmplitude /= 10;
			if (((pulseFrequency / 0.1) - (int)(pulseFrequency / 0.1)) > 0.0f) // good as mod
				bciwarn << "Frequency was not an increment of 0.1 thus will be rounded down to nearest multiple of 0.1" << endl;
			int frequency = (int)(pulseFrequency / 0.1);
			if (((freqOfTrains / 0.00001) - (int)(freqOfTrains / 0.00001)) > 0.0f) // buggy
				bciwarn << "Frequency was not an increment of 0.00001 thus will be rounded down to nearest multiple of 0.00001" << endl;
			freqOfTrains = floor(freqOfTrains * pow(10, 5)) / pow(10, 5);
			// numberPulses, numberTrains, jitter are ints with increments of 1 thus does not have to be checked

			// Extra check the values; unnecessary but a safeguard against 
			// issues since the device will be used on humans; adaptive to the individual device
			if (pulseLength > gt_phase_duration1_max(mEstim) || pulseLength < gt_phase_duration1_min(mEstim))
				bcierr << "PulseDuration must be between " << (gt_phase_duration1_min(mEstim) * 10) << "us and " << (gt_phase_duration1_max(mEstim) * 10) << "us" << endl;
			if (interphaseLength > gt_interphase_max(mEstim) || interphaseLength < gt_interphase_min(mEstim))
				bcierr << "InterphaseDuration must be between " << (gt_interphase_min(mEstim) * 10) << "us and " << (gt_interphase_max(mEstim) * 10) << "us" << endl;
			if (pulseAmplitude > gt_phase_current1_max(mEstim) || pulseAmplitude < gt_phase_current1_min(mEstim))
				bcierr << "Amplitude must be between " << (gt_phase_current1_min(mEstim) * 10) << "uA and "<< (gt_phase_current1_max(mEstim) * 10) << "uA" << endl;
			if (frequency > gt_pulse_rate_max(mEstim) || frequency < gt_pulse_rate_min(mEstim))
				bcierr << "Frequency must be between " << (gt_pulse_rate_min(mEstim) * 0.1) << "Hz and " << (gt_pulse_rate_max(mEstim) * 0.1) << "Hz" << endl;
			if (numberPulses > gt_pulses_max(mEstim) || numberPulses < gt_pulses_min(mEstim))
				bcierr << "NumOfPulses must be between " << gt_pulses_min(mEstim) << " and " << gt_pulses_max(mEstim) << endl;
			if (freqOfTrains > gt_train_rate_max(mEstim) || freqOfTrains < gt_train_rate_min(mEstim))
				bcierr << "FrequencyOfTrains must be between " << gt_train_rate_min(mEstim) << " and " << gt_train_rate_max(mEstim) << endl;
			if (numberTrains > gt_n_trains_max(mEstim) || numberTrains < gt_n_trains_min(mEstim)) 
				bcierr << "NumOfTrains must be between " << gt_n_trains_min(mEstim) << "Hz and " << gt_n_trains_max(mEstim) << "Hz" << endl;
			if (jitter > gt_jitter_max(mEstim) || jitter < gt_jitter_min(mEstim))
				bcierr << "Jitter must be between " << gt_jitter_min(mEstim) << "% and " << gt_jitter_max(mEstim) << "%" << endl;

			// Set parameters
			if (polar)
				st_alternate(mEstim, GALTERNATE_YES);
			else
				st_alternate(mEstim, GALTERNATE_NO);
			st_phase_duration1(mEstim, pulseLength);
			st_phase_duration2(mEstim, (pulseLength * modul));
			st_interphase(mEstim, interphaseLength);
			st_phase_current1(mEstim, pulseAmplitude);
			st_phase_current2(mEstim, (-1 * pulseAmplitude));
			st_pulse_rate(mEstim, frequency);
			st_pulses(mEstim, numberPulses);
			st_train_rate(mEstim, freqOfTrains);
			st_n_trains(mEstim, numberTrains);
			st_jitter(mEstim, jitter);

			// Send config
			activate(mEstim);
			wait_for_end_of_action(mEstim);
			if (gt_error(mEstim) != GERROR_SUCCESS)
				bcierr << "g.Estim error - " << gt_error_text(mEstim, gt_error(mEstim)) << endl;
			//bciwarn << "The charge of a sequence will be " << (gt_train_charge(mEstim) * numberTrains / 100) << "nC" << endl;	// 10 pC	
			bciwarn << "The charge of a sequence will be " << (gt_train_charge(mEstim) * numberTrains / 25000) << "uC" << endl;	 // FES * 40pC

			/*	
			bciwarn << "The resistance is " << gt_resistance(mEstim) << "O" << endl;
			bciwarn << "The resistance is " << measure_resistance(mEstim) << "O" << endl;
			bciwarn << "The resistance is " << gt_test_resistance(mEstim) << "O" << endl;
			*/

			// Create the communication thread
			mpgEstimThread = new gEstimThread(this);
			if (mpgEstimThread)
			{
				mpgEstimThread->Start();
			}
			else
			{
				bcierr << "Could not start g.Estim thread" << endl;
			}
		} 
		else 
		{
			bcierr << "Unable to connect to g.Estim device" << endl;
		}
	}
}

void gEstimFilter::StartRun() 
{

}

void gEstimFilter::StopRun() 
{
	if (mEstimActive)
	{
		abort_sequence(mEstim);
	}
}

void gEstimFilter::Halt() 
{
	// Kill thread
	if(mEstimActive) 
	{
		if (mpgEstimThread)
		{
			mpgEstimThread->Terminate();
		}
		delete mpgEstimThread;
		mpgEstimThread = NULL;
		unregister_with_polling(mEstim, fieldsChangedCallback);
		closeDevices();
	}
}

void gEstimFilter::Process( const GenericSignal &Input, GenericSignal &Output ) 
{
	if (mEstimActive) 
	{
		if (mpgEstimThread) 
		{
			GState state;
			state = gt_state(mEstim);
			
			if (state != GSTATE_ACTIVE && state != GSTATE_STIMULATION)
				bcierr << "Communication with g.Estim lost" << endl;
			if (state == GSTATE_ACTIVE && mActivateExp.Evaluate(&Input)) 
			{
				mStimulating = true;
				start_sequence(mEstim);
				bciout << "Stimulation Started" << endl;
			} 
			else if (state == GSTATE_STIMULATION && mAbortExp.Evaluate(&Input)) 
			{
				mStimulating = false;
				abort_sequence(mEstim);
				bciout << "Stimulation Aborted" << endl;
			} 
			else if (state == GSTATE_ACTIVE && mStimulating) // Stimulation stopped
			{
				mStimulating = false;
				bciout << "Stimulation Ended" << endl;
			}

			if (mStimulating)
				State("EstimStimulus") = 1;
			else
				State("EstimStimulus") = 0;
		} else 
			bcierr << "Lost connection to gEstimThread" << endl;
	}
}

int gEstimFilter::gEstimThread::OnExecute() 
{
	if (mpFilter -> mEstimActive) 
	{
		while (!Terminating()) 
		{
			rcv_state(mpFilter->mEstim);
			ThreadUtils::SleepForMs(100);
		}
	}
	return 0;
}

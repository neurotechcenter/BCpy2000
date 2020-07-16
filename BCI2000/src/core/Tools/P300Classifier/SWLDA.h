#ifndef _SWLDA_h
#define _SWLDA_h

#include <iostream>
#include <algorithm>
#include <vector>
#include "ap.h"
#include "blas.h"
#include "stepwisefit.h"
#include "remmean.h"

using namespace std;
///////////////////////////////////////////////////////////////////////////////
/// \struct parameters
/// This structure contains all the parameters needed for real time classification
/// using BCI2000
struct parameters
{
	/// MUD is BCI2000's moniker for the feature weight matrix
	ap::real_2d_array MUD;
	/// Contains only the channels with the predictor variables given by the SWLDA
	ap::real_1d_array channels;
	ap::real_1d_array chSet;
	/// Response window
	ap::real_1d_array windowlen;
	/// Filter order
	int MA;
	/// Decimation frequency
	int DF;
	/// Spatial filter
	int SF;
	/// Sampling rate
	int smprate;
	/// Software channel. Indicates how many channels are in the loaded BCI data file
	int softwarech;
};

bool SWLDA(ap::template_2d_array<float,true>& Responses,
		   ap::template_1d_array<unsigned short int, true>& Type,
		   ap::template_1d_array<short int,true>& trialnr,
		   ap::template_1d_array<double,true>& windowlen,
		   ap::template_1d_array<double, true>& channels,
		   int MAfilter,
		   int DecFact,
		   int SF,
		   int smprate,
		   double penter,
		   double premove,
		   int maxiter,
		   int SoftwareCh,
		   parameters &tMUD,
		   CALLBACK_STATUS callback_status=NULL);
#endif

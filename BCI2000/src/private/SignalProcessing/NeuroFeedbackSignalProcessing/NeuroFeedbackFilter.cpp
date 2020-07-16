////////////////////////////////////////////////////////////////////////////////
// $Id: $
// Authors:
// Description: NeuroFeedbackFilter implementation
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
#include "PCHIncludes.h"
#pragma hdrstop

#include "NeuroFeedbackFilter.h"
#include "BCIStream.h"


using namespace std;


//#error Change the location token 2.X in the line below as required. Then, remove this error line.
RegisterFilter( NeuroFeedbackFilter, 2.A );
     // Change the location as appropriate, to determine where your filter gets
     // sorted into the chain. By convention:
     //  - filter locations within SignalSource modules begin with "1."
     //  - filter locations witin SignalProcessing modules begin with "2."
     //       (NB: Filter() commands in a separate PipeDefinition.cpp file may override the default location set here with RegisterFilter)
     //  - filter locations within Application modules begin with "3."


NeuroFeedbackFilter::NeuroFeedbackFilter()
{

  // Declare any parameters that the filter needs....

 BEGIN_PARAMETER_DEFINITIONS

   "Filtering:NeuroFeedbackFilter int EnableNeuroFeedbackFilter= 0 0 0 1 // enable NeuroFeedbackFilter? (boolean)",                       // These are just examples:
   //"Filtering:NeuroFeedbackFilter float SomeParameter=  0.0 0.0 -1.0 1.0 // a useless NeuroFeedbackFilter parameter",   //  change them, or remove them.

 END_PARAMETER_DEFINITIONS


  // ...and likewise any state variables:

}


NeuroFeedbackFilter::~NeuroFeedbackFilter()
{
  Halt();
}

void
NeuroFeedbackFilter::Halt()
{
  // De-allocate any memory reserved in Initialize, stop any threads, etc.
  // Good practice is to write the Halt() method such that it is safe to call it even *before*
  // Initialize, and safe to call it twice (e.g. make sure you do not delete [] pointers that
  // have already been deleted:  set them to NULL after deletion).
}

void
NeuroFeedbackFilter::Preflight( const SignalProperties& Input, SignalProperties& Output ) const
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

  Output = Input; // this simply passes information through about SampleBlock dimensions, etc....

  // ... or alternatively, we could modify that info here:

  // Let's imagine this filter has only one output, namely the amount of stuff detected in the signal:
  // Output.SetChannels( 1 );
  // Output.ChannelLabels()[0] = "Stuff";

  // Imagine we want to output twice as many samples (or bins) as we receive from the input:
  // Output.SetElements( Input.Elements() * 2 );

  // Note that the NeuroFeedbackFilter instance itself, and its members, are read-only during
  // this phase, due to the "const" at the end of the Preflight prototype above.
  // Any methods called by Preflight must also be "const" in the same way.
}


void
NeuroFeedbackFilter::Initialize( const SignalProperties& Input, const SignalProperties& Output )
{
  // The user has pressed "Set Config" and all Preflight checks have passed.
  // The signal properties can no longer be modified, but the const limitation has gone, so
  // the NeuroFeedbackFilter instance itself can be modified. Allocate any memory you need, start any
  // threads, store any information you need in private member variables.
}

void
NeuroFeedbackFilter::StartRun()
{
  // The user has just pressed "Start" (or "Resume")
  //bciout << "Hello World!" << endl;
}


void
NeuroFeedbackFilter::Process( const GenericSignal& Input, GenericSignal& Output )
{

  // And now we're processing a single SampleBlock of data.
  // Remember not to take too much CPU time here, or you will break the real-time constraint.

  Output = Input; // Pass the signal through unmodified.
                  // ( Obviously this will no longer fly if we modified the shape of the
                  //   output SignalProperties during Preflight ).

  // Or we could do it one value at a time:
  /*
  for( ch = 0; ch < Output.Channels(); ch++ )
  {
    for( el = 0; el < Output.Elements(); el++ )
    {
      Output( ch, el ) = some_function( Input );
    }
  }
  */
}

void
NeuroFeedbackFilter::StopRun()
{
  // The Running state has been set to 0, either because the user has pressed "Suspend",
  // or because the run has reached its natural end.
  //bciwarn << "Goodbye World." << endl;
  // You know, you can delete methods if you're not using them.
  // Remove the corresponding declaration from NeuroFeedbackFilter.h too, if so.
}


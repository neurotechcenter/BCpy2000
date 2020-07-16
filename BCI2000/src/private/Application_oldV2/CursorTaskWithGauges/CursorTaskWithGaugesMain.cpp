////////////////////////////////////////////////////////////////////////////////
// $Id: GaugeAppMain.cpp 2236 2009-01-29 18:13:48Z mellinger $
// Authors: jhizver@wadsworth.org, schalk@wadsworth.org,
//   juergen.mellinger@uni-tuebingen.de, griffin.milsap@gmail.com
// Description: An application which displays various gauges given control
//              signals.
//
//              Gauges are defined by:
//                     caption
//                       This discription is displayed below the gauge (string)
//                     type
//                       Gauge type (string)
//                         BarGauge
//                         Histogram
//                         HistoryGauge
//                         X-Y, ...
//                     captionsize
//                       Size of the caption text (0 - 1)
//                     xpos, ypos
//                       X and Y coords of upper left corner (percent of screen) (0 - 1)
//                     width, height
//                       Width and height of gauge (percent of screen) (0 - 1)
//                     signal1
//                       Name of signal controlling this gauge (string)
//                     min1, max1
//                       Minimum and maximum value of the control signal (float)
//                     signal2
//                       Name of second signal (only applies to XY gauge)
//                     min2, max2
//                       Minimum and maximum value of the second signal (float)
//                     showscale
//                       1 = Show the scales, 0 = don't show
//                     precision
//                       integer number of sub-deliminations between min and max
//
// (C) 2000-2008, BCI2000 Project
// http://www.bci2000.org
////////////////////////////////////////////////////////////////////////////////
#include "PCHIncludes.h"
#pragma hdrstop

#include "CursorTaskWithGaugesMain.h"

#include "TextStimulus.h"
#include "ImageStimulus.h"
#include "AudioStimulus.h"
#include "Localization.h"
#include "MeasurementUnits.h"
#include "BarGauge.h"
#include "AnalogGauge.h"
#include "BarPlotGauge.h"
#include "ScrollingHistoryGauge.h"
#include "HistoryGauge.h"
#include "XYGauge.h"

#include <algorithm>

using namespace std;

RegisterFilter( GaugeApp, 3 );

GaugeApp::GaugeApp()
: mUpdateTime( 0 ),
  mTimeSinceLastUpdate( INT_MAX )
{
  // Gauges
  BEGIN_PARAMETER_DEFINITIONS
    "Application:Gauges matrix Gauges= "
    "5 " // row labels
    "{ caption type      xpos ypos width height other } " // columns
    "  Gauge1  bar       0    0    0.1   0.1    0 "
    "  Gauge2  histogram 0    0    0.1   0.1    0 "
    "  Gauge3  history   0    0    0.1   0.1    0 "
    "  Gauge4  xy        0    0    0.1   0.1    0 "
    "  Gauge5  bar       0    0    0.1   0.1    0 "
    " // Parameterization of Gauges ",

    "Application:Gauges float UpdateTime= 100ms 100ms 0 % "
    " // The time which elapses between gauge updates. ",
  END_PARAMETER_DEFINITIONS
}

GaugeApp::~GaugeApp()
{
  DeleteGauges();
}

void
GaugeApp::OnPreflight( const SignalProperties& input ) const
{
  // Do some error checking on the gauge parameters
  ParamRef gauges = Parameter( "Gauges" );
  for( int i = 0; i < gauges->NumRows(); i++ )
  {
    std::string type = gauges( i, "type" );
    ParamRef extraParameters = gauges( i, "other" );

    if( type == "" ) // We don't need to check anything, gauge doesn't exist
      continue;

    if( gauges( i, "xpos" ) < 0.0f || gauges( i, "xpos" ) > 1.0f )
      bcierr << "xpos on gauge " << i + 1 << " should be between 0.0 and 1.0." << endl;
    if( gauges( i, "ypos" ) < 0.0f || gauges( i, "ypos" ) > 1.0f )
      bcierr << "ypos on gauge " << i + 1 << " should be between 0.0 and 1.0." << endl;

    // Using default parameters
    if( extraParameters->NumRows() == 0 )
    {
      // Check default signals for validity
      parsedSignal ps1 = parseSignal( "Signal(0,0)" );
      parsedSignal ps2 = parseSignal( "Signal(1,0)" );
      if( ps1.success && ps2.success )
      {
        // Check input signals
        if( ps1.channel > input.Channels() || ps1.element > input.Elements() )
          bcierr << "Using default Signal(0,0) for gauge" << i + 1
                 << " but this signal is invalid.  "
                 << "Consider changing signal input to this gauge." << endl;
        if( ps2.channel > input.Channels() || ps2.element > input.Elements() )
          bcierr << "Using default Signal(1,0) for gauge" << i + 1
                 << " but this signal is invalid.  "
                 << "Consider changing signal input to this gauge." << endl;
      }
      continue;
    }

    for( int j = 0; j < extraParameters->NumRows(); j++ )
    {
      std::string paramName = extraParameters( j, 0 );
      ParamRef val = extraParameters( j, 1 );

      if( paramName == "captionsize" )
      {
        if( ( float )val <= 0.0f || ( float )val > 1.0f )
          bcierr << "captionsize on gauge " << i + 1 << " should be between 0.0 and 1.0." << endl;
      }
      else if( paramName == "hprecision" || paramName == "vprecision" || paramName == "precision" )
      {
        if( ( int )val < 0 )
          bcierr << "(h/v)precision on gauge " << i + 1 << " should be > 0." << endl;
      }
      else if( paramName == "signal1" )
      {
        parsedSignal ps = parseSignal( ( std::string )val );
        if( ps.success )
        {
          // Check input signal
          if( ps.channel > input.Channels() || ps.channel < 0 )
            bcierr << "signal1 on gauge " << i + 1 << " has an invalid channel index" << endl;
          if( ps.element > input.Elements() || ps.element < 0 )
            bcierr << "signal1 on gauge " << i + 1 << " has an invalid element index" << endl;
        }
        else
          State( ( std::string )val );
      }
      else if( paramName == "signal2" )
      {
        parsedSignal ps = parseSignal( ( std::string )val );
        if( ps.success )
        {
          // Check input signal
          if( ps.channel > input.Channels() || ps.channel < 0 )
            bcierr << "signal2 on gauge " << i + 1 << " has an invalid channel index" << endl;
          if( ps.element > input.Elements() || ps.element < 0 )
            bcierr << "signal2 on gauge " << i + 1 << " has an invalid element index" << endl;
        }
        else
          State( ( std::string )val );
      }
    }
  }

  // Check the update time parameter for existence
  Parameter( "UpdateTime" );
}

void
GaugeApp::OnInitialize( const SignalProperties& /*Input*/ )
{
  // Delete the Current Gauges
  DeleteGauges();

  // Create new Gauges
  ParamRef gauges = Parameter( "Gauges" );
  for( int i = 0; i < gauges->NumRows(); i++ )
  {
    // Make a display rect for this gauge
    GUI::Rect gaugeRect =
    {
      gauges( i, "xpos" ),
      gauges( i, "ypos" ),
      gauges( i, "xpos" ) + gauges( i, "width" ),
      gauges( i, "ypos" ) + gauges( i, "height" )
    };

    // Get a handle to the optional parameters
    std::string type = gauges( i, "type" );
    ParamRef extraParameters = gauges( i, "other" );

    // Creating a bar gauge
    if( type == "bar" )
    {
      // Set up defaults
      std::string signal1 = "Signal(0,0)";
      int vprecision = 0;
      float signalMin = -1.0f;
      float signalMax = 1.0f;
      float captionSize = 0.5f;
      bool showscale = true;

      // Get non-default variables
      for( int j = 0; j < extraParameters->NumRows(); j++ )
      {
        std::string paramName = extraParameters( j, 0 );
        ParamRef val = extraParameters( j, 1 );

        if( paramName == "signal1" )
          signal1 = ( std::string )val;
        else if( paramName == "vprecision" || paramName == "precision" )
          vprecision = ( int )val;
        else if( paramName == "min1" )
          signalMin = ( float )val;
        else if( paramName == "max1" )
          signalMax = ( float )val;
        else if( paramName == "captionsize" )
          captionSize = ( float )val;
        else if( paramName == "showscale" )
          showscale = ( int )val;
      }

      // Create the gauge using our values
      BarGauge *g = new BarGauge( Display(), 0 );
      g->SetCaption( gauges( i, "caption" ) );
      g->SetCaptionSize( captionSize );
      g->SetDisplayRect( gaugeRect );
      g->SetControlSignal1( signal1 );
      g->SetControlSignal1Bounds( signalMin, signalMax );
      g->SetVScalePrecision( vprecision );
      g->ShowScale( showscale );
      mpGauges.push_back( g );
    }

    // Creating an Analog Gauge
    else if( type == "analog" )
    {
      // Set up defaults
      std::string signal1 = "Signal(0,0)";
      int hprecision = 0;
      float signalMin = -1.0f;
      float signalMax = 1.0f;
      float captionSize = 0.5f;
      bool showscale = true;

      // Get non-default variables
      for( int j = 0; j < extraParameters->NumRows(); j++ )
      {
        std::string paramName = extraParameters( j, 0 );
        ParamRef val = extraParameters( j, 1 );

        if( paramName == "signal1" )
          signal1 = ( std::string )val;
        else if( paramName == "min1" )
          signalMin = ( float )val;
        else if( paramName == "max1" )
          signalMax = ( float )val;
        else if( paramName == "captionsize" )
          captionSize = ( float )val;
        else if( paramName == "showscale" )
          showscale = ( int )val;
      }

      // Create the gauge using our values
      AnalogGauge *g = new AnalogGauge( Display(), 0 );
      g->SetCaption( gauges( i, "caption" ) );
      g->SetCaptionSize( captionSize );
      g->SetDisplayRect( gaugeRect );
      g->SetControlSignal1( signal1 );
      g->SetControlSignal1Bounds( signalMin, signalMax );
      g->SetHScalePrecision( hprecision );
      g->ShowScale( showscale );
      g->Show();
      mpGauges.push_back( g );
    }

    // Creating a bar plot gauge
    else if( type == "histogram" || type == "bar-plot" )
    {
      // Set up defaults
      std::string signal1 = "Signal(0,0)";
      int vprecision = 0;
      int numBars = 5;
      float signalMin = -1.0f;
      float signalMax = 1.0f;
      float captionSize = 0.5f;
      bool showscale = true;

      // Get non-default variables
      for( int j = 0; j < extraParameters->NumRows(); j++ )
      {
        std::string paramName = extraParameters( j, 0 );
        ParamRef val = extraParameters( j, 1 );

        if( paramName == "signal1" )
          signal1 = ( std::string )val;
        else if( paramName == "vprecision" || paramName == "precision" )
          vprecision = ( int )val;
        else if( paramName == "bars" || paramName == "numbars" || paramName == "numbins" )
          numBars = ( int )val;
        else if( paramName == "min1" )
          signalMin = ( float )val;
        else if( paramName == "max1" )
          signalMax = ( float )val;
        else if( paramName == "captionsize" )
          captionSize = ( float )val;
        else if( paramName == "showscale" )
          showscale = ( int )val;
      }

      // Create the gauge using our values
      BarPlotGauge *g = new BarPlotGauge( Display(), 0 );
      g->SetCaption( gauges( i, "caption" ) );
      g->SetCaptionSize( captionSize );
      g->SetDisplayRect( gaugeRect );
      g->SetControlSignal1( signal1 );
      g->SetControlSignal1Bounds( signalMin, signalMax );
      g->SetControlSignal2Bounds( 0.0f, 1.0f );
      g->SetVScalePrecision( vprecision );
      g->SetHScalePrecision( numBars );
      g->ShowScale( showscale );
      mpGauges.push_back( g );
    }

    // Creating an history gauge
    else if( type == "history" )
    {
      // Set up defaults
      std::string signal1 = "Signal(0,0)";
      int hprecision = 0;
      int vprecision = 0;
      float historyLength = 10.0f;
      float signalMin = -1.0f;
      float signalMax = 1.0f;
      float captionSize = 0.5f;
      bool showscale = true;

      // Get non-default variables
      for( int j = 0; j < extraParameters->NumRows(); j++ )
      {
        std::string paramName = extraParameters( j, 0 );
        ParamRef val = extraParameters( j, 1 );

        if( paramName == "signal1" )
          signal1 = ( std::string )val;
        else if( paramName == "hprecision" )
          hprecision = ( int )val;
        else if( paramName == "vprecision" )
          vprecision = ( int )val;
        else if( paramName == "historylength" || paramName == "length" )
          historyLength = ( float )val;
        else if( paramName == "min1" )
          signalMin = ( float )val;
        else if( paramName == "max1" )
          signalMax = ( float )val;
        else if( paramName == "captionsize" )
          captionSize = ( float )val;
        else if( paramName == "showscale" )
          showscale = ( int )val;
      }

      // Create the gauge using our values
      HistoryGauge *g = new HistoryGauge( Display(), 0 );
      g->SetCaption( gauges( i, "caption" ) );
      g->SetCaptionSize( captionSize );
      g->SetDisplayRect( gaugeRect );
      g->SetControlSignal1( signal1 );
      g->SetControlSignal1Bounds( signalMin, signalMax );
      g->SetVScalePrecision( vprecision );
      g->SetHScalePrecision( hprecision );
      g->SetHistoryLength( historyLength );
      g->ShowScale( showscale );
      mpGauges.push_back( g );
    }

    // Creating a scrolling history gauge
    else if( type == "shistory" )
    {
      // Set up defaults
      std::string signal1 = "Signal(0,0)";
      int hprecision = 0;
      int vprecision = 0;
      float historyLength = 10.0f;
      float signalMin = -1.0f;
      float signalMax = 1.0f;
      float captionSize = 0.5f;
      bool showscale = true;

      // Get non-default variables
      for( int j = 0; j < extraParameters->NumRows(); j++ )
      {
        std::string paramName = extraParameters( j, 0 );
        ParamRef val = extraParameters( j, 1 );

        if( paramName == "signal1" )
          signal1 = ( std::string )val;
        else if( paramName == "hprecision" )
          hprecision = ( int )val;
        else if( paramName == "vprecision" )
          vprecision = ( int )val;
        else if( paramName == "historylength" || paramName == "length" )
          historyLength = ( float )val;
        else if( paramName == "min1" )
          signalMin = ( float )val;
        else if( paramName == "max1" )
          signalMax = ( float )val;
        else if( paramName == "captionsize" )
          captionSize = ( float )val;
        else if( paramName == "showscale" )
          showscale = ( int )val;
      }

      // Create the gauge using our values
      ScrollingHistoryGauge *g = new ScrollingHistoryGauge( Display(), 0 );
      g->SetCaption( gauges( i, "caption" ) );
      g->SetCaptionSize( captionSize );
      g->SetDisplayRect( gaugeRect );
      g->SetControlSignal1( signal1 );
      g->SetControlSignal1Bounds( signalMin, signalMax );
      g->SetVScalePrecision( vprecision );
      g->SetHScalePrecision( hprecision );
      g->SetHistoryLength( historyLength );
      g->ShowScale( showscale );
      mpGauges.push_back( g );
    }

    // Creating an XY Gauge
    else if( type == "xy" || type == "x-y" )
    {
      // Set up defaults
      std::string signal1 = "Signal(0,0)";
      std::string signal2 = "Signal(1,0)";
      int hprecision = 0;
      int vprecision = 0;
      float signal1Min = -1.0f;
      float signal1Max = 1.0f;
      float signal2Min = -1.0f;
      float signal2Max = 1.0f;
      float captionSize = 0.5f;
      bool showscale = true;

      // Get non-default variables
      for( int j = 0; j < extraParameters->NumRows(); j++ )
      {
        std::string paramName = extraParameters( j, 0 );
        ParamRef val = extraParameters( j, 1 );

        if( paramName == "signal1" )
          signal1 = ( std::string )val;
        else if( paramName == "signal2" )
          signal2 = ( std::string )val;
        else if( paramName == "hprecision" )
          hprecision = ( int )val;
        else if( paramName == "vprecision" )
          vprecision = ( int )val;
        else if( paramName == "min1" )
          signal1Min = ( float )val;
        else if( paramName == "max1" )
          signal1Max = ( float )val;
        else if( paramName == "min2" )
          signal2Min = ( float )val;
        else if( paramName == "max2" )
          signal2Max = ( float )val;
        else if( paramName == "captionsize" )
          captionSize = ( float )val;
        else if( paramName == "showscale" )
          showscale = ( int )val;
      }

      // Create the gauge using our values
      XYGauge *g = new XYGauge( Display(), 0 );
      g->SetCaption( gauges( i, "caption" ) );
      g->SetCaptionSize( captionSize );
      g->SetDisplayRect( gaugeRect );
      g->SetControlSignal1( signal1 );
      g->SetControlSignal1Bounds( signal1Min, signal1Max );
      g->SetControlSignal2( signal2 );
      g->SetControlSignal2Bounds( signal2Min, signal2Max );
      g->SetVScalePrecision( vprecision );
      g->SetHScalePrecision( hprecision );
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

  // Grab the update time
  mUpdateTime = MeasurementUnits::ReadAsTime( Parameter( "UpdateTime" ) );
}

void
GaugeApp::OnStartRun()
{
  // User has clicked the "Start" or "Resume" button
  mTimeSinceLastUpdate = INT_MAX;
}

void
GaugeApp::OnStopRun()
{
  // User has clicked the "Stop" button
}

void
GaugeApp::Process( const GenericSignal &input, GenericSignal &output )
{
   // Call Parent process function
   GenericStimulusTask::Process( input, output );

   // See if we need to update the gauges
   if( mTimeSinceLastUpdate < mUpdateTime )
   {
     mTimeSinceLastUpdate++;
     return;
   }
   else
     mTimeSinceLastUpdate = 0;

   // Update the Gauges
   for( size_t i = 0; i < mpGauges.size(); i++ )
   {
     if( mpGauges[i]->ControlSignal2() == "" )
     {
       parsedSignal sig = parseSignal( mpGauges[i]->ControlSignal1() );
       if( sig.success )
         mpGauges[i]->Update( input( sig.channel, sig.element ), 0.0f );
       else
         mpGauges[i]->Update( State( mpGauges[i]->ControlSignal1() ), 0.0f );
     }
     else
     {
       parsedSignal sig1 = parseSignal( mpGauges[i]->ControlSignal1() );
       parsedSignal sig2 = parseSignal( mpGauges[i]->ControlSignal2() );
       if( sig1.success && sig2.success )
         mpGauges[i]->Update( input( sig1.channel, sig1.element ),
                              input( sig2.channel, sig2.element ) );
       else if( sig1.success && !sig2.success )
         mpGauges[i]->Update( input( sig1.channel, sig1.element ),
                              State( mpGauges[i]->ControlSignal2() ) );
       else if( !sig1.success && sig2.success )
         mpGauges[i]->Update( State( mpGauges[i]->ControlSignal1() ),
                              input( sig2.channel, sig2.element ) );
       else
         mpGauges[i]->Update( State( mpGauges[i]->ControlSignal1() ),
                              State( mpGauges[i]->ControlSignal2() ) );
     }
  }
}

void
GaugeApp::DeleteGauges()
{
  for( size_t i = 0; i < mpGauges.size(); i++ )
  {
    if( mpGauges[i] )
    {
      mpGauges[i]->Hide();
      delete mpGauges[i];
      mpGauges[i] = NULL;
    }
  }
  mpGauges.clear();
}


parsedSignal GaugeApp::parseSignal( const std::string str ) const
{
  parsedSignal ret;
  if( str.substr( 0, 6 ) == "signal" || str.substr( 0, 6 ) == "Signal" )
  {
    std::string val = str.substr( 6 );
    int loc = val.find( ",", 0 );
    std::string strChannel = val.substr( 1, loc );
    std::string strElement = val.substr( loc + 1, val.size() - 2 - loc );
    std::stringstream ssChannel( strChannel );
    std::stringstream ssElement( strElement );
    int channel = 0;
    int element = 0;
    if( ( ssChannel >> channel ).fail() )
      return ret;
    if( ( ssElement >> element ).fail() )
      return ret;
    ret.channel = channel;
    ret.element = element;
    ret.success = true;
    return ret;
  }
  return ret;
}




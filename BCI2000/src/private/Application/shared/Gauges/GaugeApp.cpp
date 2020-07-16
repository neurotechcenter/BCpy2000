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

#include "GaugeApp.h"
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

RegisterFilter( GaugeApp, 3.A );

GaugeApp::GaugeApp()
: mUpdateTime( 0 ),
  mTimeSinceLastUpdate( INT_MAX ),
  mrDisplay( Window( "Gauges" ) )
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
GaugeApp::Preflight( const SignalProperties& Input, SignalProperties& Output ) const
{
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

bool
GaugeApp::GaugeHasAttr( int inGaugeIndex, string inAttrName ) const
{
  bool found = false;
  ParamRef p = GetGaugeAttr( inGaugeIndex, inAttrName, found, false );
  return found;
}
double
GaugeApp::GetGaugeAttr( int inGaugeIndex, string inAttrName, double inDefaultValue, bool inRequired ) const
{
  bool found = false;
  ParamRef p = GetGaugeAttr( inGaugeIndex, inAttrName, found, inRequired );
  if( (string)p == "" ) found = false;
  return ( found ? p : inDefaultValue );
}
string
GaugeApp::GetGaugeAttr( int inGaugeIndex, string inAttrName, string inDefaultValue, bool inRequired ) const
{
  bool found = false;
  ParamRef p = GetGaugeAttr( inGaugeIndex, inAttrName, found, inRequired );
  return ( found ? p : inDefaultValue );
}

ParamRef
GaugeApp::GetGaugeAttr( int inGaugeIndex, string inAttrName, bool & outFound, bool inRequired ) const
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
GaugeApp::InitDimension( Gauge* gauge, unsigned int gaugeIndex, unsigned int whichDim, unsigned int nChannels, bool force )
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
	// double qWait     = MeasurementUnits::TimeInSampleBlocks( GetGaugeAttr( gaugeIndex, "qwait"+suffix,   "250ms" ) ) / (double)mUpdateTime; // TODO:  a prospective way, not yet implemented, of automating the "hedge against ringing" and smoothing away the feedValueToQuantilizer argument to Gauge::Evaluate
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

void
GaugeApp::Initialize( const SignalProperties& Input, const SignalProperties& Output )
{
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
      g->SetCaption( caption );
      g->SetCaptionSize( captionSize );
      g->SetDisplayRect( gaugeRect );
      InitDimension( g, iGauge, 0, Input.Channels(), true );
      g->SetVScalePrecision( vprecision );
      g->SetVScaleFormat( vformat);
      g->ShowScale( showscale );
      mpGauges.push_back( g );
    }
    // Creating an Analog Gauge
    else if( type == "analog" )
    {
      // Create the gauge using our values
      AnalogGauge *g = new AnalogGauge( mrDisplay, 0 );
      g->SetCaption( caption );
      g->SetCaptionSize( captionSize );
      g->SetDisplayRect( gaugeRect );
      InitDimension( g, iGauge, 0, Input.Channels(), true );
      g->SetHScalePrecision( hprecision );
      g->SetHScaleFormat( hformat);
      g->ShowScale( showscale );
      g->Show(); // TODO:  why this here and nowhere else?
      mpGauges.push_back( g );
    }

    // Creating a bar plot gauge
    else if( type == "histogram" || type == "bar-plot" )
    {
      // Set up defaults
      int numBars = 5;
      numBars = (int)GetGaugeAttr( iGauge, "bars",    numBars );
      numBars = (int)GetGaugeAttr( iGauge, "numbars", numBars );
      numBars = (int)GetGaugeAttr( iGauge, "numbins", numBars );

      // Create the gauge using our values
      BarPlotGauge *g = new BarPlotGauge( mrDisplay, 0 );
      g->SetCaption( caption );
      g->SetCaptionSize( captionSize );
      g->SetDisplayRect( gaugeRect );
      InitDimension( g, iGauge, 0, Input.Channels(), true );
      g->SetVScalePrecision( vprecision );
      g->SetVScaleFormat( vformat);
      g->SetHScalePrecision( numBars ); // TODO: one of the few cases in which the copy-pasted block of gauge method calls actually varies?
      g->SetHScaleFormat( hformat);
      g->ShowScale( showscale );
      mpGauges.push_back( g );
    }

    // Creating an history gauge
    else if( type == "history" )
    {
      string historyLengthStr = "10s";
      historyLengthStr = GetGaugeAttr( iGauge, "length", historyLengthStr );
      historyLengthStr = GetGaugeAttr( iGauge, "historylength", historyLengthStr );
      double historyLength = MeasurementUnits::TimeInSeconds( historyLengthStr );

      // Create the gauge using our values
      HistoryGauge *g = new HistoryGauge( mrDisplay, 0 );
      g->SetCaption( caption );
      g->SetCaptionSize( captionSize );
      g->SetDisplayRect( gaugeRect );
      for( unsigned int iDim = 0; ; iDim++ ) 
        if( !InitDimension( g, iGauge, iDim, Input.Channels(), iDim==0 ) ) break;
      g->SetVScalePrecision( vprecision );
      g->SetVScaleFormat( vformat);
      g->SetHScalePrecision( hprecision );
      g->SetHScaleFormat( hformat);
      g->SetHistoryLength( historyLength );
      g->ShowScale( showscale );
      mpGauges.push_back( g );
    }

    // Creating a scrolling history gauge
    else if( type == "shistory" )
    {
      string historyLengthStr = "10s";
      historyLengthStr = GetGaugeAttr( iGauge, "length", historyLengthStr );
      historyLengthStr = GetGaugeAttr( iGauge, "historylength", historyLengthStr );
      double historyLength = MeasurementUnits::TimeInSeconds( historyLengthStr );

      // Create the gauge using our values
      ScrollingHistoryGauge *g = new ScrollingHistoryGauge( mrDisplay, 0 );
      g->SetCaption( caption );
      g->SetCaptionSize( captionSize );
      g->SetDisplayRect( gaugeRect );
      for( unsigned int iDim = 0; ; iDim++ ) 
        if( !InitDimension( g, iGauge, iDim, Input.Channels(), iDim==0 ) ) break;
      g->SetVScalePrecision( vprecision );
      g->SetVScaleFormat( vformat);
      g->SetHScalePrecision( hprecision );
      g->SetHScaleFormat( hformat);
      g->SetHistoryLength( historyLength );
      g->ShowScale( showscale );
      mpGauges.push_back( g );
    }

    // Creating an XY Gauge
    else if( type == "xy" || type == "x-y" )
    {
      // Create the gauge using our values
      XYGauge *g = new XYGauge( mrDisplay, 0 );
      g->SetCaption( caption );
      g->SetCaptionSize( captionSize );
      g->SetDisplayRect( gaugeRect );
      for( unsigned int iDim = 0; ; iDim+=2 )
	  {
        if( !InitDimension( g, iGauge, iDim,   Input.Channels(), iDim==0 ) ) break;
        if( !InitDimension( g, iGauge, iDim+1, Input.Channels(), iDim==0 ) ) break;
	  }
      g->SetVScalePrecision( vprecision );
      g->SetVScaleFormat( vformat);
      g->SetHScalePrecision( hprecision );
      g->SetHScaleFormat( hformat);
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
  if( mpGauges.size() ) 
	mrDisplay.Show();
  else
	mrDisplay.Hide();

}

void
GaugeApp::StartRun()
{
  // User has clicked the "Start" or "Resume" button
  mTimeSinceLastUpdate = INT_MAX;
  mTheoreticalMilliseconds = 0.0;
}

void
GaugeApp::StopRun()
{
  // User has clicked the "Stop" button
}

void
GaugeApp::Process( const GenericSignal &input, GenericSignal &output )
{
   // See if we need to update the gauges
   if( mTimeSinceLastUpdate++ >= mUpdateTime )
   {
     mTimeSinceLastUpdate = 0;

     // Update the Gauges
     for( size_t iGauge = 0; iGauge < mpGauges.size(); iGauge++ )
       mpGauges[iGauge]->Update( input, mTheoreticalMilliseconds );

     // Update the screen
     mrDisplay.Update();
   }
   output = input;
   mTheoreticalMilliseconds += MeasurementUnits::TimeInMilliseconds( "1" );
   //bciout << mTheoreticalMilliseconds << endl;
}

void
GaugeApp::DeleteGauges()
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




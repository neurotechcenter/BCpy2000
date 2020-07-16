/////////////////////////////////////////////////////////////////////////////
//
// File: MarkerView.cpp
//
// Date: Nov 8, 2001
//
// Author: Juergen Mellinger
//
// Description:
//
// Changes: Feb 16, 2003, jm: Introduced TGUIMarkerView for
//          Zero Bar / Fixation Cross display.
//          May 13, 2003, jm: Introduced multiple auditory markers
//          at arbitrary temporal offsets.
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
//////////////////////////////////////////////////////////////////////////////

#ifdef __BORLANDC__
#include "PCHIncludes.h"
#pragma hdrstop
#endif // __BORLANDC__

#include "MarkerView.h"
#include "PresParams.h"
#include "MidiPlayer.h"
#include "ParamList.h"
#include "Utils/Util.h"
#include "MeasurementUnits.h"

TMarkerView::TMarkerView( ParamList *inParamList )
: TPresView( inParamList ),
  visMarker( 0 ),
  currentMarker( audMarkers.begin() ),
  lastTimeOffset( 0.0 )
{
    PARAM_ENABLE( inParamList, PRVisMarker );
    PARAM_ENABLE( inParamList, PRAudMarkers );
    PARAM_ENABLE( inParamList, PRAudMarkerSoundFiles );
    PARAM_ENABLE( inParamList, PRGMMarkerInstruments );
    PARAM_ENABLE( inParamList, PRGMMarkerVolumes );
    PARAM_ENABLE( inParamList, PRGMMarkerNotes );
}

TMarkerView::~TMarkerView()
{
    PARAM_DISABLE( curParamList, PRVisMarker );
    PARAM_DISABLE( curParamList, PRAudMarkers );
    PARAM_DISABLE( curParamList, PRAudMarkerSoundFiles );
    PARAM_DISABLE( curParamList, PRGMMarkerInstruments );
    PARAM_DISABLE( curParamList, PRGMMarkerVolumes );
    PARAM_DISABLE( curParamList, PRGMMarkerNotes );
    ClearMarkers();
}

TPresError
TMarkerView::Initialize(       ParamList *inParamList,
                         const TGUIRect  &inRect )
{
    viewRect = inRect;
    TGUIView::Resized();

    PARAM_GET_NUM( inParamList, PRVisMarker, visMarker );
    TGUIMarkerView::SetType( visMarker );
    TGUIMarkerView::Show();

    ClearMarkers();
    Param* audMarkersParam;
    size_t numMarkers = 0;
    PARAM_GET_PTR( inParamList, PRAudMarkers, audMarkersParam );
    if( audMarkersParam != NULL )
      numMarkers = audMarkersParam->NumValues();
    int instrument,
        volume,
        note;
    const char* soundFile = NULL;
    float timeOffset;
    for( size_t i = 0; i < numMarkers; ++i )
    {
      const char* value = audMarkersParam->Value( i ).c_str();
      if( value == NULL )
        value = "";
      const Param* param = NULL;
      if( inParamList->Exists( value ) )
        param = &( *inParamList )[ value ];
      if( param != NULL )
        value = param->Value().c_str();
      timeOffset = MeasurementUnits::ReadAsTime( value );
      PARAM_GET_STRING_BY_INDEX( inParamList, PRAudMarkerSoundFiles, i, soundFile );
      PARAM_GET_NUM_BY_INDEX( inParamList, PRGMMarkerInstruments, i, instrument );
      PARAM_GET_NUM_BY_INDEX( inParamList, PRGMMarkerVolumes, i, volume );
      PARAM_GET_NUM_BY_INDEX( inParamList, PRGMMarkerNotes, i, note );
      audMarker marker;
      marker.timeOffset = timeOffset;
      if( instrument == 0 )
        marker.midiPlayer = NULL;
      else
        marker.midiPlayer = new MidiPlayer( instrument, volume, note );
      if( *soundFile == '\0' )
        marker.wavePlayer = NULL;
      else
      {
        marker.wavePlayer = new WavePlayer;
        marker.wavePlayer->SetFile( soundFile );
      }
      audMarkers.push_back( marker );
    }
    audMarkers.sort();
    lastTimeOffset = 0.0;
    currentMarker = audMarkers.begin();

    return presNoError;
}

void
TMarkerView::ProcessTrialActive( const TEventArgs& args )
{
  if( args.timeOffset < lastTimeOffset )
    currentMarker = audMarkers.begin();
  lastTimeOffset = args.timeOffset;
  
  while( currentMarker != audMarkers.end()
         && args.timeOffset >= currentMarker->timeOffset )
  {
    if( currentMarker->midiPlayer != NULL )
      currentMarker->midiPlayer->Play();
    if( currentMarker->wavePlayer != NULL )
      currentMarker->wavePlayer->Play();
    ++currentMarker;
  }
}

void
TMarkerView::ProcessBeginOfTrial( const TEventArgs& )
{
#ifdef ITI_BLANK_SCREEN
    TGUIMarkerView::Show();
#endif
}

void
TMarkerView::ProcessItiBegin( const TEventArgs& )
{
#ifdef ITI_BLANK_SCREEN
    TGUIMarkerView::Hide();
#endif
}

void
TMarkerView::ProcessStopBegin( const TEventArgs& )
{
  for( audMarkerContainer::iterator i = audMarkers.begin(); i != audMarkers.end(); ++i )
    if( i->midiPlayer != NULL )
      i->midiPlayer->StopSequence();
}

void
TMarkerView::ClearMarkers()
{
    for( audMarkerContainer::iterator i = audMarkers.begin(); i != audMarkers.end(); ++i )
    {
      delete i->midiPlayer;
      delete i->wavePlayer;
    }
    audMarkers.clear();
}


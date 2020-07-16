////////////////////////////////////////////////////////////////////////////////
// $Id: VisDisplayGraph.cpp 5417 2016-07-02 15:53:30Z mellinger $
// Authors: schalk@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: A class for graph type visualization windows.
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
#include "VisDisplayGraph.h"

#include "ColorListChooser.h"

#include <limits>

#include <QMenu>
#include <QAction>
#include <QActionGroup>
#include <QColorDialog>
#include <QPaintEvent>
#include <QPainter>
#include <QApplication>
#include <QDesktopWidget>
#include <QLayout>
#include <QStatusBar>
#include <QLabel>
#include <QStyle>

#define OFF "off"

using namespace std;

VisDisplayGraph::VisDisplayGraph( const std::string& inSourceID )
: VisDisplayLayer( inSourceID ),
  mpContextMenu( NULL ),
  mpHPMenu( NULL ),
  mpLPMenu( NULL ),
  mpNotchMenu( NULL ),
  mpActEnlargeSignal( NULL ),
  mpActReduceSignal( NULL ),
  mpActToggleAutoScale( NULL ),
  mpActFewerSamples( NULL ),
  mpActMoreSamples( NULL ),
  mpActMoreChannels( NULL ),
  mpActFewerChannels( NULL ),
  mpActToggleDisplayMode( NULL ),
  mpActToggleNumericValues( NULL ),
  mpActToggleBaselines( NULL ),
  mpActToggleValueUnit( NULL ),
  mpActToggleChannelLabels( NULL ),
  mpActToggleColor( NULL ),
  mpActInvertDisplay( NULL ),
  mpActChooseColors( NULL ),
  mpActSmallerFont( nullptr ),
  mpActLargerFont( nullptr ),
  mNumChannels( 0 ),
  mSignalElements( 0 ),
  mUserScaling( 0 ),
  mUserZoom( 0 ),
  mpCurrentHPItem( NULL ),
  mpCurrentLPItem( NULL ),
  mpCurrentNotchItem( NULL ),
  mpStatusBar( NULL ),
  mpStatusLabel( NULL ),
  mAutoScale( false ),
  mFixedScale( false ),
  mMinValue( -1 ),
  mMaxValue( 1 ),
  mElementGain( 0 )
{
  BuildStatusBar();
  UpdateStatusBar();
  BuildContextMenu();
  SyncDisplay();
  this->show();
}

VisDisplayGraph::~VisDisplayGraph()
{
  delete mpContextMenu;
}

void
VisDisplayGraph::OnSetConfig( ConfigSettings& inConfig )
{
  VisDisplayBase::OnSetConfig( inConfig );

  if( inConfig.Get( CfgID::MinValue, mMinValue ) )
    mDisplay.SetMinValue( mMinValue );
  if( inConfig.Get( CfgID::MaxValue, mMaxValue ) )
    mDisplay.SetMaxValue( mMaxValue );

  inConfig.Get( CfgID::FixedScale, mFixedScale );

  int userScaling = mUserScaling;
  mUserScaling = 0;
  if( !mFixedScale )
  {
    for( int i = 0; i < userScaling; ++i )
      EnlargeSignal();
    for( int i = 0; i > userScaling; --i )
      ReduceSignal();
  }
  int numSamples = NominalDisplaySamples();
  if( inConfig.Get( CfgID::NumSamples, numSamples ) )
    SetNominalDisplaySamples( numSamples );
  int userZoom = mUserZoom;
  mUserZoom = 0;
  for( int i = 0; i < userZoom; ++i )
    FewerSamples();
  for( int i = 0; i > userZoom; --i )
    MoreSamples();

  int channelGroupSize = mDisplay.ChannelGroupSize();
  if( inConfig.Get( CfgID::ChannelGroupSize, channelGroupSize ) )
  {
    if( channelGroupSize < 1 )
      channelGroupSize = numeric_limits<size_t>::max();
    mDisplay.SetChannelGroupSize( channelGroupSize );
  }
  int graphType = mDisplay.DisplayMode();
  if( inConfig.Get( CfgID::GraphType, graphType ) )
    switch( graphType )
    {
      case CfgID::Polyline:
        mDisplay.SetDisplayMode( SignalDisplay::polyline );
        break;
      case CfgID::Field2d:
        mDisplay.SetDisplayMode( SignalDisplay::field2d );
        break;
    }
  bool showNumericValues = mDisplay.NumericValuesVisible();
  if( inConfig.Get( CfgID::ShowNumericValues, showNumericValues ) )
    mDisplay.SetNumericValuesVisible( showNumericValues );

  bool showBaselines = mDisplay.BaselinesVisible();
  if( inConfig.Get( CfgID::ShowBaselines, showBaselines ) )
    mDisplay.SetBaselinesVisible( showBaselines );

  bool invertedDisplay = mDisplay.Inverted();
  if( inConfig.Get( CfgID::InvertedDisplay, invertedDisplay ) )
    mDisplay.SetInverted( invertedDisplay );

  int fontSize = mDisplay.FontSize();
  if( inConfig.Get( CfgID::FontSize, fontSize ) )
    mDisplay.SetFontSize( fontSize );

  ColorList channelColors = mDisplay.ChannelColors();
  if( inConfig.Get( CfgID::ChannelColors, channelColors ) )
    mDisplay.SetChannelColors( channelColors );

  std::string channelStyles = mDisplay.ChannelStyles();
  if( inConfig.Get( CfgID::ChannelStyles, channelStyles ) )
    mDisplay.SetChannelStyles( channelStyles );

  std::string valueColors;
  if( inConfig.Get( CfgID::ValueColors, valueColors ) )
    mDisplay.SetValueColors( valueColors );

  string unit;
  istringstream iss;
  if( inConfig.Get( CfgID::SampleUnit, unit ) )
  {
    double oldSampleUnit
     = NominalUnitsPerSample() * FilterUnitToValue( mDisplay.SampleUnit() );
    PhysicalUnit u;
    u.SetGainWithSymbol( unit );
    float unitsPerSample = u.Gain();
    string sampleUnit = u.Symbol();
    SetNominalUnitsPerSample( unitsPerSample );
    mDisplay.SetSampleUnit( u.Symbol() );
    if( oldSampleUnit != unitsPerSample * FilterUnitToValue( sampleUnit ) )
    {
      mDisplayFilter.HPCorner( 0 );
      mDisplayFilter.LPCorner( 0 );
      mDisplayFilter.NotchCenter( 0 );
    }
    mElementGain = unitsPerSample * FilterUnitToValue( sampleUnit );
  }
  float sampleOffset;
  if( inConfig.Get( CfgID::SampleOffset, sampleOffset ) )
    mDisplay.SetSampleOffset( -sampleOffset );
  
  if( mFixedScale )
    mAutoScale = false;
  else
  {
    float timeConstant;
    mAutoScale = inConfig.Get( CfgID::AutoScale, timeConstant );
    if( mAutoScale )
      mScaleObserver.TimeConstant( timeConstant );
  }
  if( inConfig.Get( CfgID::ChannelUnit, unit ) )
    mDisplay.SetChannelUnit( unit );

  if( inConfig.Get( CfgID::ValueUnit, unit ) )
    mDisplay.SetValueUnit( unit );
  bool showValueUnit = mDisplay.ValueUnitVisible();
  if( inConfig.Get( CfgID::ShowValueUnit, showValueUnit ) )
    mDisplay.SetValueUnitVisible( showValueUnit );

  LabelList labels = mDisplay.XAxisMarkers();
  if( inConfig.Get( CfgID::XAxisMarkers, labels ) )
    mDisplay.SetXAxisMarkers( labels );
  labels = mDisplay.ChannelLabels();
  if( inConfig.Get( CfgID::ChannelLabels, labels ) )
  {
    mDisplay.SetChannelLabels( labels );
    mDisplay.SetChannelLabelsVisible( !labels.empty() );
  }

  string hpFilter = OFF;
  if( inConfig.Get( CfgID::HPFilter, hpFilter ) )
    SetHP( hpFilter );

  string lpFilter = OFF;
  if( inConfig.Get( CfgID::LPFilter, lpFilter ) )
    SetLP( lpFilter );

  string notchFilter = OFF;
  if( inConfig.Get( CfgID::NotchFilter, notchFilter ) )
    SetNotch( notchFilter );

  ShowStatusBar( Filter_Enabled() );

  // Sanity checks.
  if( mDisplay.MinValue() == mDisplay.MaxValue() )
    mDisplay.SetMaxValue( mDisplay.MinValue() + 1 );
}

void
VisDisplayGraph::ShowStatusBar( bool inVisible )
{
  mpStatusBar->setVisible( inVisible );
  SyncDisplay();
}

void
VisDisplayGraph::OnSignal( const GenericSignal& s )
{
  if( s.Channels() < 1 || s.Elements() < 1 )
    return;

  int curChannels = mNumChannels,
      curSamples = mDisplay.NumSamples();

  // Apply the visualization filter.
  GenericSignal filteredSignal( s.Properties() );
  mDisplayFilter.Process( s, filteredSignal );
  if( mElementGain > 0 )
  {
    SignalProperties s = filteredSignal.Properties();
    s.ElementUnit().SetSymbol( "s" ).SetGain( mElementGain );
    filteredSignal.SetProperties( s );
  }
  
  if( mAutoScale )
  {
    mScaleObserver.Process( filteredSignal );
    float sigMax = mScaleObserver.Max(),
          sigMin = mScaleObserver.Min();
    if( sigMin > 0 )
      sigMin = 0;
    else if( sigMax < 0 )
      sigMax = 0;
    else
    {
      float absMax = max( abs( sigMax ), abs( sigMin ) );
      sigMax = absMax;
      sigMin = -absMax;
    }
    if( sigMax != sigMin )
    {
      mDisplay.SetMaxValue( sigMax );
      mDisplay.SetMinValue( sigMin );
    }
  }

  mNumChannels = filteredSignal.Channels();

  // Apply the decimation filter.
  GenericSignal decimatedSignal( 0, 0 );
  mDecimationFilter.Process( filteredSignal, decimatedSignal );

  mDisplay.WrapForward( decimatedSignal );
  mSignalElements = s.Elements();

  if( curChannels != mNumChannels || curSamples != mDisplay.NumSamples() )
    OnSetConfig( Visconfigs()[ mVisID ] );
}

void
VisDisplayGraph::BuildStatusBar()
{
  mpStatusBar = new QStatusBar( this );
  mpStatusBar->setPalette( mpStatusBar->style()->standardPalette() );
  mpStatusBar->setAutoFillBackground( true );
  mpStatusLabel = new QLabel( mpStatusBar );
  mpStatusBar->addPermanentWidget( mpStatusLabel );
  QVBoxLayout* pLayout = new QVBoxLayout( this );
  pLayout->setContentsMargins( 0, 0, 0, 0 );
  pLayout->insertStretch( 0 );
  pLayout->addWidget( mpStatusBar );
  this->setLayout( pLayout );
}

void
VisDisplayGraph::BuildContextMenu()
{
  mpContextMenu = new QMenu;
  mpActEnlargeSignal = mpContextMenu->addAction( tr("Enlarge Signal"), this, SLOT(EnlargeSignal()) );
  mpActReduceSignal = mpContextMenu->addAction( tr("Reduce Signal"), this, SLOT(ReduceSignal()) );
  mpActToggleAutoScale = mpContextMenu->addAction( tr("Auto Scale"), this, SLOT(ToggleAutoScale()) );
  mpActToggleAutoScale->setCheckable( true );
  mpContextMenu->addSeparator();

  mpActFewerSamples = mpContextMenu->addAction( tr("Fewer Samples"), this, SLOT(FewerSamples()) );
  mpActMoreSamples = mpContextMenu->addAction( tr("More Samples"), this, SLOT(MoreSamples()) );
  mpContextMenu->addSeparator();

  mpActMoreChannels = mpContextMenu->addAction( tr("More Channels"), this, SLOT(MoreChannels()) );
  mpActFewerChannels = mpContextMenu->addAction( tr("Fewer Channels"), this, SLOT(FewerChannels()) );
  mpContextMenu->addSeparator();

  mpActToggleDisplayMode = mpContextMenu->addAction( tr("Toggle Display Mode"), this, SLOT(ToggleDisplayMode()) );
  mpActToggleColor = mpContextMenu->addAction( tr("Color Display"), this, SLOT(ToggleColor()) );
  mpActToggleColor->setCheckable( true );
  mpActInvertDisplay = mpContextMenu->addAction( tr("Invert"), this, SLOT(InvertDisplay()) );
  mpActInvertDisplay->setCheckable( true );
  mpActChooseColors = mpContextMenu->addAction( tr("Choose Channel Colors..."), this, SLOT(ChooseColors()) );
  mpContextMenu->addSeparator();

  mpActToggleNumericValues = mpContextMenu->addAction( tr("Show Numeric Values"), this, SLOT(ToggleNumericValues()) );
  mpActToggleNumericValues->setCheckable( true );
  mpActToggleBaselines = mpContextMenu->addAction( tr("Show Baselines"), this, SLOT(ToggleBaselines()) );
  mpActToggleBaselines->setCheckable( true );
  mpActToggleValueUnit = mpContextMenu->addAction( tr("Show Unit"), this, SLOT(ToggleValueUnit()) );
  mpActToggleValueUnit->setCheckable( true );
  mpActToggleChannelLabels = mpContextMenu->addAction( tr("Show Legend"), this, SLOT(ToggleChannelLabels()) );
  mpActToggleChannelLabels->setCheckable( true );
  mpActLargerFont = mpContextMenu->addAction( tr("Larger Font"), this, SLOT(LargerFont()) );
  mpActSmallerFont = mpContextMenu->addAction( tr("Smaller Font"), this, SLOT(SmallerFont()) );
  mpContextMenu->addSeparator();

  mpHPMenu = new QMenu( tr("High Pass"), mpContextMenu );
  QActionGroup* pHPGroup = new QActionGroup( mpHPMenu );
  QAction* pAct = mpHPMenu->addAction( OFF, this, SLOT(SetHPOff()) );
  pAct->setCheckable( true );
  pHPGroup->addAction( pAct );
  pAct->setChecked( true );
  mpCurrentHPItem = pAct;
  pAct = mpHPMenu->addAction( "0.1Hz", this, SLOT(SetHP01()) );
  pAct->setCheckable( true );
  pHPGroup->addAction( pAct );
  pAct = mpHPMenu->addAction( "1Hz", this, SLOT(SetHP1()) );
  pAct->setCheckable( true );
  pHPGroup->addAction( pAct );
  pAct = mpHPMenu->addAction( "5Hz", this, SLOT(SetHP5()) );
  pAct->setCheckable( true );
  pHPGroup->addAction( pAct );
  mpContextMenu->addMenu( mpHPMenu );

  mpLPMenu = new QMenu( tr("Low Pass"), mpContextMenu );
  QActionGroup* pLPGroup = new QActionGroup( mpLPMenu );
  pAct = mpLPMenu->addAction( OFF, this, SLOT(SetLPOff()) );
  pAct->setCheckable( true );
  pAct->setChecked( true );
  mpCurrentLPItem = pAct;
  pLPGroup->addAction( pAct );
  pAct = mpLPMenu->addAction( "30Hz", this, SLOT(SetLP30()) );
  pAct->setCheckable( true );
  pLPGroup->addAction( pAct );
  pAct = mpLPMenu->addAction( "40Hz", this, SLOT(SetLP40()) );
  pAct->setCheckable( true );
  pLPGroup->addAction( pAct );
  pAct = mpLPMenu->addAction( "70Hz", this, SLOT(SetLP70()) );
  pAct->setCheckable( true );
  pLPGroup->addAction( pAct );
  mpContextMenu->addMenu( mpLPMenu );

  mpNotchMenu = new QMenu( tr("Notch"), mpContextMenu );
  QActionGroup* pNotchGroup = new QActionGroup( mpNotchMenu );
  pAct = mpNotchMenu->addAction( OFF, this, SLOT(SetNotchOff()) );
  pAct->setCheckable( true );
  pAct->setChecked( true );
  mpCurrentNotchItem = pAct;
  pNotchGroup->addAction( pAct );
  pAct = mpNotchMenu->addAction( "50Hz", this, SLOT(SetNotch50()) );
  pAct->setCheckable( true );
  pNotchGroup->addAction( pAct );
  pAct = mpNotchMenu->addAction( "60Hz", this, SLOT(SetNotch60()) );
  pAct->setCheckable( true );
  pNotchGroup->addAction( pAct );
  mpContextMenu->addMenu( mpNotchMenu );
}

void
VisDisplayGraph::OnContextMenu( const QPoint& inP )
{
  mpActEnlargeSignal->setEnabled( EnlargeSignal_Enabled() );
  mpActReduceSignal->setEnabled( ReduceSignal_Enabled() );
  mpActToggleAutoScale->setEnabled( ToggleAutoScale_Enabled() );
  mpActToggleAutoScale->setChecked( ToggleAutoScale_Checked() );

  mpActFewerSamples->setEnabled( FewerSamples_Enabled() );
  mpActMoreSamples->setEnabled( MoreSamples_Enabled() );

  mpActFewerChannels->setEnabled( FewerChannels_Enabled() );
  mpActMoreChannels->setEnabled( MoreChannels_Enabled() );

  mpActToggleColor->setEnabled( ToggleColor_Enabled() );
  mpActToggleColor->setChecked( ToggleColor_Checked() );

  mpActInvertDisplay->setChecked( InvertDisplay_Checked() );

  mpActChooseColors->setEnabled( ChooseColors_Enabled() );

  mpActToggleNumericValues->setEnabled( ToggleNumericValues_Enabled() );
  mpActToggleNumericValues->setChecked( ToggleNumericValues_Checked() );

  mpActToggleBaselines->setEnabled( ToggleBaselines_Enabled() );
  mpActToggleBaselines->setChecked( ToggleBaselines_Checked() );

  mpActToggleValueUnit->setEnabled( ToggleValueUnit_Enabled() );
  mpActToggleValueUnit->setChecked( ToggleValueUnit_Checked() );

  mpActToggleChannelLabels->setEnabled( ToggleChannelLabels_Enabled() );
  mpActToggleChannelLabels->setChecked( ToggleChannelLabels_Checked() );

  mpActSmallerFont->setEnabled( SmallerFont_Enabled() );

  mpHPMenu->setEnabled( Filter_Enabled() );
  mpLPMenu->setEnabled( Filter_Enabled() );
  mpNotchMenu->setEnabled( Filter_Enabled() );

  mpContextMenu->exec( this->mapToGlobal( inP ) );
}

void
VisDisplayGraph::ToggleAutoScale()
{
  mAutoScale = !mAutoScale;
  if( mAutoScale )
  {
    mScaleObserver.Reset();
    if( mScaleObserver.TimeConstant() == 0 )
      mScaleObserver.TimeConstant( 10 );
    Visconfigs()[ mVisID ].Put( CfgID::AutoScale, mScaleObserver.TimeConstant(), UserDefined );
  }
  else
  {
    mDisplay.SetMaxValue( mMaxValue );
    mDisplay.SetMinValue( mMinValue );
    Visconfigs()[ mVisID ].Put( CfgID::AutoScale, OFF, UserDefined );
  }    
}

bool
VisDisplayGraph::ToggleAutoScale_Enabled() const
{
  return !mFixedScale;
}

bool
VisDisplayGraph::ToggleAutoScale_Checked() const
{
  return mAutoScale;
}

void
VisDisplayGraph::ToggleDisplayMode()
{
  mDisplay.SetDisplayMode(
    ( mDisplay.DisplayMode() + 1 ) % SignalDisplay::numDisplayModes
  );
}

void
VisDisplayGraph::ToggleNumericValues()
{
  mDisplay.SetNumericValuesVisible( !mDisplay.NumericValuesVisible() );
  Visconfigs()[ mVisID ].Put( CfgID::ShowNumericValues, mDisplay.NumericValuesVisible(), UserDefined );
}

bool
VisDisplayGraph::ToggleNumericValues_Enabled() const
{
  return true;
}

bool
VisDisplayGraph::ToggleNumericValues_Checked() const
{
  return mDisplay.NumericValuesVisible();
}

void
VisDisplayGraph::ToggleBaselines()
{
  mDisplay.SetBaselinesVisible( !mDisplay.BaselinesVisible() );
  Visconfigs()[ mVisID ].Put( CfgID::ShowBaselines, mDisplay.BaselinesVisible(), UserDefined );
}

bool
VisDisplayGraph::ToggleBaselines_Enabled() const
{
  return mDisplay.DisplayMode() == SignalDisplay::polyline;
}

bool
VisDisplayGraph::ToggleBaselines_Checked() const
{
  return mDisplay.BaselinesVisible();
}

void
VisDisplayGraph::ToggleValueUnit()
{
  mDisplay.SetValueUnitVisible( !mDisplay.ValueUnitVisible() );
  Visconfigs()[ mVisID ].Put( CfgID::ShowValueUnit, mDisplay.ValueUnitVisible(), UserDefined );
}

bool
VisDisplayGraph::ToggleValueUnit_Enabled() const
{
  return true;
}

bool
VisDisplayGraph::ToggleValueUnit_Checked() const
{
  return mDisplay.ValueUnitVisible();
}

void
VisDisplayGraph::ToggleChannelLabels()
{
  mDisplay.SetChannelLabelsVisible( !mDisplay.ChannelLabelsVisible() );
}

bool
VisDisplayGraph::ToggleChannelLabels_Enabled() const
{
  return !mDisplay.ChannelLabels().empty();
}

bool
VisDisplayGraph::ToggleChannelLabels_Checked() const
{
  return mDisplay.ChannelLabelsVisible();
}

void
VisDisplayGraph::ToggleColor()
{
  mDisplay.SetColorDisplay( !mDisplay.ColorDisplay() );
}

bool
VisDisplayGraph::ToggleColor_Enabled() const
{
  return mDisplay.DisplayMode() == SignalDisplay::polyline
         || mDisplay.DisplayMode() == SignalDisplay::field2d;
}

bool
VisDisplayGraph::ToggleColor_Checked() const
{
  return mDisplay.ColorDisplay();
}

void
VisDisplayGraph::InvertDisplay()
{
  mDisplay.SetInverted( !mDisplay.Inverted() );
  Visconfigs()[ mVisID ].Put( CfgID::InvertedDisplay, mDisplay.Inverted(), UserDefined );
}

bool
VisDisplayGraph::InvertDisplay_Checked() const
{
  return mDisplay.Inverted();
}

void
VisDisplayGraph::ChooseColors()
{
  // The dialog's "custom colors" are used to hold channel colors.
  ColorList channelColors = mDisplay.ChannelColors();
  ColorListChooser().SetPrompt( "Choose channel colors" )
                    .Display( channelColors );
  mDisplay.SetChannelColors( channelColors );
  Visconfigs()[ mVisID ].Put( CfgID::ChannelColors, channelColors, UserDefined );
}

bool
VisDisplayGraph::ChooseColors_Enabled() const
{
  return mDisplay.ColorDisplay()
         && mDisplay.DisplayMode() == SignalDisplay::polyline;
}

void
VisDisplayGraph::EnlargeSignal()
{
  mDisplay.SetMinValue( mDisplay.MinValue() / 2 );
  mDisplay.SetMaxValue( mDisplay.MaxValue() / 2 );
  ++mUserScaling;
}

bool
VisDisplayGraph::EnlargeSignal_Enabled() const
{
  return !mFixedScale && mUserScaling < cMaxUserScaling;
}

void
VisDisplayGraph::ReduceSignal()
{
  mDisplay.SetMinValue( mDisplay.MinValue() * 2 );
  mDisplay.SetMaxValue( mDisplay.MaxValue() * 2 );
  --mUserScaling;
}

bool
VisDisplayGraph::ReduceSignal_Enabled() const
{
  return !mFixedScale && mUserScaling > -cMaxUserScaling;
}

void
VisDisplayGraph::FewerSamples()
{
  SetNominalDisplaySamples( NominalDisplaySamples() / 2 );
  ++mUserZoom;
}

bool
VisDisplayGraph::FewerSamples_Enabled() const
{
  return ( 2 * mSignalElements <= NominalDisplaySamples() ) && ( mUserZoom < cMaxUserScaling );
}

void
VisDisplayGraph::MoreSamples()
{
  SetNominalDisplaySamples( NominalDisplaySamples() * 2 );
  --mUserZoom;
}

bool
VisDisplayGraph::MoreSamples_Enabled() const
{
  return ( 2 * mSignalElements <= NominalDisplaySamples() ) && ( mUserZoom > -cMaxUserScaling );
}

void
VisDisplayGraph::FewerChannels()
{
  // Round down to the nearest power of 2.
  int n = mDisplay.DisplayGroups(),
      roundedN = 1;
  while( n >>= 1 )
    roundedN <<= 1;

  int newGroups = roundedN;
  if( newGroups == mDisplay.DisplayGroups() )
    newGroups /= 2;
  mDisplay.SetDisplayGroups( newGroups );
}

bool
VisDisplayGraph::FewerChannels_Enabled() const
{
  return mDisplay.DisplayGroups() > 1;
}

void
VisDisplayGraph::MoreChannels()
{
  int maxDisplayGroups = mNumChannels / mDisplay.ChannelGroupSize(),
      newDisplayGroups = min( maxDisplayGroups, mDisplay.DisplayGroups() * 2 );
  mDisplay.SetDisplayGroups( newDisplayGroups );
}

bool
VisDisplayGraph::MoreChannels_Enabled() const
{
  return mDisplay.DisplayGroups() < mNumChannels / mDisplay.ChannelGroupSize();
}

double
VisDisplayGraph::FilterCaptionToValue( const char* inCaption ) const
{
  const char* p = inCaption;
  while( *p && !::isdigit( *p ) )
    ++p;
  return ::atof( p ) * NominalUnitsPerSample()
                     * FilterUnitToValue( mDisplay.SampleUnit() );
}

double
VisDisplayGraph::FilterUnitToValue( const string& inUnit ) const
{
  PhysicalUnit u;
  u.SetGainWithSymbol( "1" + inUnit );
  return u.Symbol() == "s" ? u.Gain() : 0;
}

void
VisDisplayGraph::LargerFont()
{
  int size = Round( mDisplay.FontSize() * 1.1 );
  if( size == mDisplay.FontSize() )
    ++size;
  mDisplay.SetFontSize( size );
  Visconfigs()[mVisID].Put( CfgID::FontSize, mDisplay.FontSize(), UserDefined );
}

void
VisDisplayGraph::SmallerFont()
{
  int size = Round( mDisplay.FontSize() / 1.1 );
  if( size == mDisplay.FontSize() )
    --size;
  mDisplay.SetFontSize( size );
  Visconfigs()[mVisID].Put( CfgID::FontSize, mDisplay.FontSize(), UserDefined );
}

bool
VisDisplayGraph::SmallerFont_Enabled() const
{
  return mDisplay.FontSize() > 1;
}

int
VisDisplayGraph::NominalDisplaySamples() const
{
  return mDisplay.NumSamples() * mDecimationFilter.Decimation();
}

void
VisDisplayGraph::SetNominalDisplaySamples( int inSamples )
{
  if( inSamples != NominalDisplaySamples() )
  {
    double nominalUnitsPerSample = NominalUnitsPerSample();
    int actualSamples = inSamples,
        decimation = 1;
    int screenWidth = QApplication::desktop()->screenGeometry().width();
    if( screenWidth < inSamples )
    {
      decimation = ::ceil( static_cast<float>( inSamples ) / screenWidth / cDisplayOversampling );
      actualSamples = inSamples / decimation;
    }
    mDisplay.SetNumSamples( actualSamples );
    mDecimationFilter.Decimation( decimation );
    SetNominalUnitsPerSample( nominalUnitsPerSample );
  }
}

double
VisDisplayGraph::NominalUnitsPerSample() const
{
  return mDisplay.UnitsPerSample() / mDecimationFilter.Decimation();
}

void
VisDisplayGraph::SetNominalUnitsPerSample( double inUps )
{
  mDisplay.SetUnitsPerSample( inUps * mDecimationFilter.Decimation() );
}

bool
VisDisplayGraph::Filter_Enabled() const
{
  return !mFixedScale && NominalUnitsPerSample() * FilterUnitToValue( mDisplay.SampleUnit() ) != 0;
}

void
VisDisplayGraph::SetHP( const string& inCaption, bool inWriteToConfig )
{
  if( inCaption == OFF )
  {
    mDisplayFilter.HPCorner( 0 );
  }
  else
  {
    mDisplayFilter.HPCorner( FilterCaptionToValue( inCaption.c_str() ) );
    mLastHP = inCaption;
  }
  mpCurrentHPItem = SyncFilterMenu( inCaption, mpHPMenu );
  if( inWriteToConfig )
    Visconfigs()[mVisID].Put( CfgID::HPFilter, inCaption, UserDefined );
  UpdateStatusBar();
}

void
VisDisplayGraph::SetHPOff()
{
  SetHP( OFF, true );
}

void
VisDisplayGraph::SetHP01()
{
  SetHP( "0.1Hz", true );
}

void
VisDisplayGraph::SetHP1()
{
  SetHP( "1Hz", true );
}

void
VisDisplayGraph::SetHP5()
{
  SetHP( "5Hz", true );
}

void
VisDisplayGraph::SetLP( const string& inCaption, bool inWriteToConfig )
{
  if( inCaption == OFF )
  {
    mDisplayFilter.LPCorner( 0 );
  }
  else
  {
    mDisplayFilter.LPCorner( FilterCaptionToValue( inCaption.c_str() ) );
    mLastLP = inCaption;
  }
  mpCurrentLPItem = SyncFilterMenu( inCaption, mpLPMenu );
  if( inWriteToConfig )
    Visconfigs()[mVisID].Put( CfgID::LPFilter, inCaption, UserDefined );
  UpdateStatusBar();
}

void
VisDisplayGraph::SetLPOff()
{
  SetLP( OFF, true );
}

void
VisDisplayGraph::SetLP30()
{
  SetLP( "30Hz", true );
}

void
VisDisplayGraph::SetLP40()
{
  SetLP( "40Hz", true );
}

void
VisDisplayGraph::SetLP70()
{
  SetLP( "70Hz", true );
}

void
VisDisplayGraph::SetNotch( const string& inCaption, bool inWriteToConfig )
{
  if( inCaption == OFF )
  {
    mDisplayFilter.NotchCenter( 0 );
  }
  else
  {
    mDisplayFilter.NotchCenter( FilterCaptionToValue( inCaption.c_str() ) );
    mLastNotch = inCaption;
  }
  mpCurrentNotchItem = SyncFilterMenu( inCaption, mpNotchMenu );
  if( inWriteToConfig )
    Visconfigs()[mVisID].Put( CfgID::NotchFilter, inCaption, UserDefined );
  UpdateStatusBar();
}

void
VisDisplayGraph::SetNotchOff()
{
  SetNotch( OFF, true );
}

void
VisDisplayGraph::SetNotch50()
{
  SetNotch( "50Hz", true );
}

void
VisDisplayGraph::SetNotch60()
{
  SetNotch( "60Hz", true );
}

void
VisDisplayGraph::UpdateStatusBar()
{
  ostringstream oss;
  oss << "HP: ";
  if( mDisplayFilter.HPCorner() == 0.0 )
    oss << OFF;
  else
    oss << mLastHP;
  oss << "   LP: ";
  if( mDisplayFilter.LPCorner() == 0.0 )
    oss << OFF;
  else
    oss << mLastLP;
  oss << "   Notch: ";
  if( mDisplayFilter.NotchCenter() == 0.0 )
    oss << OFF;
  else
    oss << mLastNotch;
  mpStatusLabel->setText( oss.str().c_str() );
}

QAction*
VisDisplayGraph::SyncFilterMenu( const string& inCaption, QMenu* inpMenu )
{
  QAction* result = NULL;
  for( int i = 0; i < inpMenu->actions().size(); ++i )
  {
    QAction* pAction = inpMenu->actions()[i];
    bool isCurrent = ( inCaption == pAction->text().toLocal8Bit().constData() );
    if( isCurrent )
      result = pAction;
    pAction->setChecked( isCurrent );
  }
  return result;
}

QAction*
VisDisplayGraph::NextMenuItem( QMenu* inpMenu, QAction* inpItem )
{ // Beginning from the position of the input item, and wrapping around at the end,
  // get the next action item from the menu.
  if( inpMenu->actions().empty() )
    return NULL;

  int idx = 0;
  QAction* pAction = NULL;
  while( idx < inpMenu->actions().size() && pAction != inpItem )
    pAction = inpMenu->actions()[idx++];
  if( idx == inpMenu->actions().size() )
    idx = 0;
  return inpMenu->actions()[idx];
}

void
VisDisplayGraph::SyncDisplay()
{
  int heightReduction = 0;
  if( mpStatusBar->isVisible() )
    heightReduction = mpStatusBar->height();

  GUI::DrawContext dc =
  {
    { 0, 0, this->width(), this->height() - heightReduction },
    0
  };
  dc.device = this;
  mDisplay.SetContext( dc );
}

void
VisDisplayGraph::paintEvent( QPaintEvent* iopEvent )
{
  VisDisplayBase::paintEvent( iopEvent );
  mDisplay.Paint( &iopEvent->region() );
  iopEvent->accept();
}

void
VisDisplayGraph::moveEvent( QMoveEvent* iopEvent )
{
  SyncDisplay();
  VisDisplayBase::moveEvent( iopEvent );
  iopEvent->accept();
}

void
VisDisplayGraph::resizeEvent( QResizeEvent* iopEvent )
{
  SyncDisplay();
  VisDisplayBase::resizeEvent( iopEvent );
  iopEvent->accept();
}

void
VisDisplayGraph::keyReleaseEvent( QKeyEvent* iopEvent )
{
  static int acc = 0;
  int wipe_acc = 1;
  int key = iopEvent->key(),
      modkey = iopEvent->modifiers();

  if( key == Qt::Key_Space && ( modkey & Qt::ShiftModifier ) )
    key = Qt::Key_PageUp;
  if( acc == 0 && key == Qt::Key_G && ( modkey & Qt::ShiftModifier ) )
    key = Qt::Key_End;
  if( acc != 0 && key == Qt::Key_Return )
    key = Qt::Key_G;

  switch( key )
  {
    case Qt::Key_Up:
      mDisplay.SetTopGroup( mDisplay.TopGroup() - 1 );
      break;
    case Qt::Key_Down:
      mDisplay.SetTopGroup( mDisplay.TopGroup() + 1 );
      break;
    case Qt::Key_PageUp:
    case Qt::Key_B:
      mDisplay.SetTopGroup( mDisplay.TopGroup() - mDisplay.DisplayGroups() );
      break;
    case Qt::Key_PageDown:
    case Qt::Key_Space:
      mDisplay.SetTopGroup( mDisplay.TopGroup() + mDisplay.DisplayGroups() );
      break;
    case Qt::Key_Right:
      if( FewerSamples_Enabled() )
        FewerSamples();
      break;
    case Qt::Key_Left:
      if( MoreSamples_Enabled() )
        MoreSamples();
      break;
    case Qt::Key_Less:
    case Qt::Key_Comma:
      if( FewerChannels_Enabled() )
        FewerChannels();
      break;
    case Qt::Key_Greater:
    case Qt::Key_Period:
      if( MoreChannels_Enabled() )
        MoreChannels();
      break;
    case Qt::Key_Minus:
      if( ReduceSignal_Enabled() )
        ReduceSignal();
      break;
    case Qt::Key_Plus:
    case Qt::Key_Equal:
      if( EnlargeSignal_Enabled() )
        EnlargeSignal();
      break;
    case Qt::Key_Home:
      mDisplay.SetTopGroup( 0 );
      break;
    case Qt::Key_End:
      mDisplay.SetTopGroup( mNumChannels / mDisplay.ChannelGroupSize() );
      break;
    case Qt::Key_G:
      mDisplay.SetTopGroup( acc - 1 );
      break;
    case Qt::Key_H:
      if( modkey & Qt::ShiftModifier )
      {
        QAction* pItem = NextMenuItem( mpHPMenu, mpCurrentHPItem );
        if( pItem )
          pItem->trigger();
      }
      else
      {
        if( mDisplayFilter.HPCorner() == 0.0 )
          SetHP( mLastHP, true );
        else
          SetHP( OFF, true );
      }
      break;
    case Qt::Key_L:
      if( modkey & Qt::ShiftModifier )
      {
        QAction* pItem = NextMenuItem( mpLPMenu, mpCurrentLPItem );
        if( pItem )
          pItem->trigger();
      }
      else
      {
        if( mDisplayFilter.LPCorner() == 0.0 )
          SetLP( mLastLP, true );
        else
          SetLP( OFF, true );
      }
      break;
    case Qt::Key_N:
      if( modkey & Qt::ShiftModifier )
      {
        QAction* pItem = NextMenuItem( mpNotchMenu, mpCurrentNotchItem );
        if( pItem )
          pItem->trigger();
      }
      else
      {
        if( mDisplayFilter.NotchCenter() == 0.0 )
          SetNotch( mLastNotch, true );
        else
          SetNotch( OFF, true );
      }
      break;
    case Qt::Key_0:
    case Qt::Key_1:
    case Qt::Key_2:
    case Qt::Key_3:
    case Qt::Key_4:
    case Qt::Key_5:
    case Qt::Key_6:
    case Qt::Key_7:
    case Qt::Key_8:
    case Qt::Key_9:
      acc = acc * 10 + ( key - Qt::Key_0 );
      wipe_acc = 0;
      break;
    default:
      VisDisplayBase::keyReleaseEvent( iopEvent );
  }
  if( wipe_acc )
    acc = 0;
}

void
VisDisplayGraph::mousePressEvent( QMouseEvent* iopEvent )
{
  this->activateWindow();
  VisDisplayBase::mousePressEvent( iopEvent );
}

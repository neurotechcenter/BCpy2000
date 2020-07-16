////////////////////////////////////////////////////////////////////////////////
// $Id: SignalDisplay.cpp 5775 2018-06-04 13:03:53Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A SignalDisplay class that renders GenericSignal data into a
//   given DisplayContext.
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
#include "SignalDisplay.h"
#include "Exception.h"
#include "FastConv.h"
#include "Tree.h"

#include <cmath>
#include <sstream>
#include <iomanip>

#include <QWidget>
#include <QPainter>

#undef TEST_UPDATE_RGN

using namespace std;

////////////////////////////////////////////////////////////////////////////////
const RGBColor SignalDisplay::cAxisColorDefault = RGBColor::Aqua;
const RGBColor SignalDisplay::cChannelColorsDefault[] =
{
  RGBColor::White,
  RGBColor::White,
  RGBColor::White,
  RGBColor::White,
  RGBColor::Yellow,
  ColorList::End
};

SignalDisplay::SignalDisplay()
: mDataWidth( 0 ),
  mDataHeight( 0 ),
  mDataLeft( cLabelWidth ),
  mLabelWidth( cLabelWidth ),
  mMarkerHeight( 0 ),
  mDisplayMode( polyline ),
  mShowCursor( false ),
  mWrapAround( false ),
  mTimeLabels( false ),
  mShowNumericValues( false ),
  mShowBaselines( false ),
  mShowChannelLabels( false ),
  mShowValueUnit( false ),
  mDisplayColors( true ),
  mInverted( false ),
  mNumSamples( cNumSamplesDefault ),
  mSampleCursor( 0 ),
  mNumDisplayGroups( 0 ),
  mMaxDisplayGroups( 0 ),
  mNumDisplayChannels( 0 ),
  mTopGroup( 0 ),
  mChannelGroupSize( 1 ),
  mMarkerChannels( 0 ),
  mTickLength( mTickLength ),
  mFontSize( 11 ),
  mNumericValueWidth( 0 ),
  mMinValue( cMinValueDefault ),
  mMaxValue( cMaxValueDefault ),
  mAxisColor( cAxisColorDefault ),
  mChannelColors( cChannelColorsDefault ),
  mParent( NULL ),
  mpSignalPoints( NULL ),
  mSymbolWidth( cLabelWidth )
{
}

SignalDisplay::~SignalDisplay()
{
  delete[] mpSignalPoints;
}

namespace {

void SetUnitFromString( const std::string& s, PhysicalUnit& u )
{
  if( ::atof( s.c_str() ) )
    u.SetGainWithSymbol( s );
  else
    u.SetSymbol( s );
}

}

SignalDisplay&
SignalDisplay::SetChannelUnit( const std::string& s )
{
  SetUnitFromString( s, mChannelUnit );
  return Invalidate();
}

SignalDisplay&
SignalDisplay::SetSampleUnit( const std::string& s )
{
  SetUnitFromString( s, mSampleUnit );
  mTimeLabels = ( mSampleUnit.Symbol() == ":s" );
  return Invalidate();
}

SignalDisplay&
SignalDisplay::SetValueUnit( const std::string& s )
{
  SetUnitFromString( s, mValueUnit );
  return Invalidate();
}

std::string
SignalDisplay::ValueColors() const
{
  Tree tree;
  int i = 0;
  for( const auto& pair : mValueColors )
  {
    tree[i][0].Data() << pair.first;
    Tree& c = tree[i][1];
    c[0].Data() << pair.second.R();
    c[1].Data() << pair.second.G();
    c[2].Data() << pair.second.B();
    ++i;
  }
  return tree.ToString();
}

SignalDisplay&
SignalDisplay::SetValueColors( std::string s )
{
  mValueColors.clear();
  Tree tree;
  if( tree.Parse( s ) )
  {
    for( const auto& entry : tree.Children() )
    {
      double value;
      entry[0].Data() >> value;
      int rgb[3] = { 0 };
      for( int i = 0; i < 3; ++i )
        entry[1][i].Data() >> rgb[i];
      mValueColors.push_back( std::make_pair( value, RGBColor( rgb[0], rgb[1], rgb[2] ) ) );
    }
    std::sort( mValueColors.begin(), mValueColors.end() );
  }
  return Invalidate();
}

SignalDisplay&
SignalDisplay::SetContext( const GUI::DrawContext& dc )
{
  mDisplayRect = QRect(
    ToInt( dc.rect.left ),
    ToInt( dc.rect.top ),
    ToInt( dc.rect.right - dc.rect.left ),
    ToInt( dc.rect.bottom - dc.rect.top )
  );
  mDisplayRgn = mDisplayRect;
  mParent = dc.device;
  return Invalidate();
}

SignalDisplay&
SignalDisplay::Invalidate()
{
  QWidget* pWindow = dynamic_cast< QWidget* >( mParent );
  if( pWindow )
    pWindow->update( mDisplayRect );
  return *this;
}

void
SignalDisplay::AdaptTo( const GenericSignal& inSignal )
{
  // Any changes in signal size that we must react to?
  bool reconfigure = false;
  if( inSignal.Elements() > mNumSamples )
    WithLock( mDataLock )
    {
      SetNumSamples( inSignal.Elements() );
      reconfigure = true;
    }
  if( inSignal.Channels() != mData.Channels() )
    WithLock( mDataLock )
    {
      int newNumDisplayGroups = ( inSignal.Channels() - mMarkerChannels - 1 ) / mChannelGroupSize + 1;
      if( mNumDisplayGroups == 0 )
        mNumDisplayGroups = min<int>( newNumDisplayGroups, cInitialMaxDisplayGroups );
      else if( newNumDisplayGroups < mNumDisplayGroups )
        mNumDisplayGroups = newNumDisplayGroups;
      reconfigure = true;
    }
  if( reconfigure )
    WithLock( mDataLock )
    {
      mData = GenericSignal( SignalProperties( inSignal.Channels(), mNumSamples ), GenericSignal::NaN );
      SetDisplayGroups( DisplayGroups() );
      SyncLabelWidth();
      mSampleCursor = 0;
      Invalidate();
    }
}

SignalDisplay&
SignalDisplay::WrapForward( const GenericSignal& inSignal )
{
  if( mParent == NULL )
    return *this;

  AdaptTo( inSignal );
  ScopedLock( mDataLock );
  mShowCursor = ( inSignal.Elements() < mNumSamples );

  for( int i = 0; i < inSignal.Channels(); ++i )
    for( int j = 0; j < inSignal.Elements(); ++j )
      mData( i, ( mSampleCursor + j ) % mData.Elements() ) = inSignal( i, j );

  SyncGraphics();

  int firstInvalidSample = mSampleCursor,
      firstValidSample = mSampleCursor + inSignal.Elements();
  bool wrappingAround = false;
  if( mNumSamples > 0 )
  {
    wrappingAround = ( firstValidSample / mNumSamples > 0 );
    mWrapAround |= wrappingAround;
    wrappingAround |= bool( mSampleCursor == 0 );
    mSampleCursor = firstValidSample % mNumSamples;
  }

  long firstInvalidPixel = mDataRect.left(),
       firstValidPixel = mDataRect.right();

  switch( mDisplayMode )
  {
    case polyline:
      firstInvalidPixel = SampleLeft( firstInvalidSample - 1 );
      firstValidPixel = SampleLeft( firstValidSample + 1 );
      break;
    case field2d:
      firstInvalidPixel = SampleLeft( firstInvalidSample );
      firstValidPixel = SampleLeft( firstValidSample );
      break;
    default:
      Assert( false );
  }

  QWidget* pWindow = dynamic_cast< QWidget* >( mParent );
  if( pWindow )
  { // Our Paint() implementation is fast for rectangular update regions but
    // quite slow if the update region is wrapped around the right edge.
    // Thus, we use repaint() rather than update() in this case.
    if( wrappingAround )
      pWindow->repaint( mCursorRect );
    else
      pWindow->update( mCursorRect );

    QRect invalidRect = mDataRect;

    // The non-wrapped area.
    invalidRect.setLeft( max<int>( firstInvalidPixel, mDataRect.left() ) - mSymbolWidth / 2 );
    invalidRect.setRight( min<int>( firstValidPixel, mDataRect.right() ) );
    if( invalidRect.left() < invalidRect.right() )
    {
      if( wrappingAround )
        pWindow->repaint( invalidRect );
      else
        pWindow->update( invalidRect );
    }

    // The area wrapped around the left edge.
    invalidRect.setLeft( max<int>( firstInvalidPixel + mDataWidth, mDataRect.left() ) - mSymbolWidth / 2 );
    invalidRect.setRight( min<int>( firstValidPixel + mDataWidth, mDataRect.right() ) );
    if( invalidRect.left() < invalidRect.right() )
      pWindow->repaint( invalidRect );

    // The area wrapped around the right edge.
    invalidRect.setLeft( max<int>( firstInvalidPixel - mDataWidth, mDataRect.left() ) - mSymbolWidth / 2 );
    invalidRect.setRight( min<int>( firstValidPixel - mDataWidth, mDataRect.right() ) );
    if( invalidRect.left() < invalidRect.right() )
      pWindow->repaint( invalidRect );

    if( mShowNumericValues )
    {
      invalidRect.setLeft( NumericValuesLeft() );
      invalidRect.setRight( NumericValuesRight() );
      pWindow->repaint( invalidRect );
    }
  }
  return *this;
}

SignalDisplay&
SignalDisplay::ScrollForward( const GenericSignal& inSignal )
{
  if( mParent == NULL )
    return *this;

  if( inSignal.Elements() < 1 )
    return *this;

  AdaptTo( inSignal );
  ScopedLock( mDataLock );
  mShowCursor = false;
  // Shift data to the left, and then append the input signal.
  for( int i = 0; i < inSignal.Channels(); ++i )
  {
    int j = 0,
        k = inSignal.Elements();
    while( j < mData.Elements() - inSignal.Elements() )
      mData( i, j++ ) = mData( i, k++ );
    k = 0;
    while( j < mData.Elements() )
      mData( i, j++ ) = inSignal( i, k++ );
  }

  SyncGraphics();
  Invalidate();

  return *this;
}

SignalDisplay&
SignalDisplay::ScrollBack( const GenericSignal& inSignal )
{
  if( mParent == NULL )
    return *this;

  if( inSignal.Elements() < 1 )
    return *this;

  AdaptTo( inSignal );
  ScopedLock( mDataLock );
  mShowCursor = false;
  // Shift data to the right, and then prepend the input signal.
  for( int i = 0; i < inSignal.Channels(); ++i )
  {
    int j = mData.Elements(),
        k = mData.Elements() - inSignal.Elements();
    while( k > 0 )
      mData( i, --j ) = mData( i, --k );
    k = inSignal.Elements();
    while( j > 0 )
      mData( i, --j ) = inSignal( i, --k );
  }

  SyncGraphics();
  Invalidate();

  return *this;
}

QFont
SignalDisplay::AxisFont()
{
  QFont font;
  font.setPixelSize( mFontSize );
  font.setStyleHint( QFont::SansSerif, QFont::PreferBitmap );
  return font;
}

QFont
SignalDisplay::MonoFont()
{
  QFont font;
  font.setPixelSize( mFontSize );
  font.setStyleHint( QFont::Monospace, QFont::PreferBitmap );
  return font;
}

QFont
SignalDisplay::SymbolFont()
{
  QFont font;
  font.setPixelSize( mFontSize );
  font.setStyleHint( QFont::SansSerif, QFont::PreferBitmap );
  return font;
}

void
SignalDisplay::SyncLabelWidth()
{
  mChannelNameCache.clear();
  if( mParent == NULL )
  {
    mTickLength = cTickLength;
    mLabelWidth = cLabelWidth;
    mMarkerHeight = mLabelWidth;
  }
  else
  {
    QFontMetrics fontMetrics( AxisFont() );
    ostringstream oss;
    oss << mData.Channels();
    QSize size = fontMetrics.size( 0, oss.str().c_str() );
    mLabelWidth = size.width();
    mMarkerHeight = size.height();
    mTickLength = Round( size.height() / 3.0 );
    if( mShowChannelLabels )
    {
      for( size_t i = 0; i < mChannelLabels.size(); ++i )
      {
        int address = mChannelLabels[ i ].Address();
        if( address >= int( mChannelNameCache.size() ) )
          mChannelNameCache.resize( address + 1, "" );
        const string& label = mChannelLabels[ i ].Text();
        mChannelNameCache[ address ] = label;
        if( address >= mData.Channels() - mMarkerChannels )
          mChannelNameCache[ address ] += " ";
      }
    }
    for( int i = 0; i < mData.Channels(); ++i )
    {
      if( int( mChannelNameCache.size() ) <= i )
        mChannelNameCache.resize( i + 1 );
      if( mChannelNameCache[ i ].empty() )
      {
        ostringstream oss;
        if( mChannelUnit.Symbol().empty() )
          oss << i + cChannelBase;
        else
          oss << fixed << setprecision( 1 ) << mChannelUnit.RawToPhysical( i ).Utf8();
        mChannelNameCache[ i ] = oss.str();
      }
      const string& label = mChannelNameCache[i];
      size = fontMetrics.size( 0, label.c_str() );
      if( size.width() > mLabelWidth )
        mLabelWidth = size.width();
    }
    mLabelWidth += 3 * cAxisWidth + cTickWidth;
  }
  if( !mShowNumericValues )
    mNumericValueWidth = 0;
  mDataLeft = mLabelWidth + mNumericValueWidth + cAxisWidth + mTickLength;
  if( mShowNumericValues && mChannelGroupSize == 1 )
    mDataLeft += cAxisWidth;
}

inline
void
SignalDisplay::SyncGraphics()
{
  mDataRect.setLeft( mDataLeft );
  mDataRect.setTop( 0 );
  mDataRect.setRight( mDisplayRect.right() - mDisplayRect.left() );
  mDataRect.setBottom( mDisplayRect.bottom() - mDisplayRect.top() );
  mDataWidth = max<int>( 0, mDataRect.right() - mDataRect.left() );
  mDataHeight = max<int>( 0, mDataRect.bottom() - mDataRect.top()
                             - mMarkerHeight * mMarkerChannels
                             - mFontSize
                             - cAxisWidth
                             - 2 * mTickLength );
}

SignalDisplay&
SignalDisplay::SetDisplayGroups( int inDisplayGroups )
{
  mNumDisplayGroups = inDisplayGroups;
  int numSignalChannels = mData.Channels() - mMarkerChannels;
  if( mNumDisplayGroups * mChannelGroupSize > numSignalChannels )
    mNumDisplayGroups = ( numSignalChannels - 1 ) / mChannelGroupSize + 1;
  mNumDisplayChannels = min( numSignalChannels, mNumDisplayGroups * mChannelGroupSize );
  SetTopGroup( mTopGroup );
  Invalidate();
  return *this;
}

SignalDisplay&
SignalDisplay::SetTopGroup( int inTopGroup )
{
  int newTopGroup = inTopGroup,
         maxTopGroup = ChannelToGroup( mData.Channels() - mMarkerChannels ) - int( mNumDisplayGroups );
  if( newTopGroup > maxTopGroup )
    newTopGroup = maxTopGroup;
  if( newTopGroup < 0 )
    newTopGroup = 0;
  if( newTopGroup != mTopGroup )
  {
    mTopGroup = newTopGroup;
    Invalidate();
  }
  return *this;
}

SignalDisplay&
SignalDisplay::SetDisplayMode( int mode )
{
  if( mode != mDisplayMode )
  {
    mDisplayMode = eDisplayMode( mode );
    Invalidate();
  }
  return *this;
}

SignalDisplay&
SignalDisplay::SetNumSamples( int inNumSamples )
{
  int newNumSamples = inNumSamples;
  if( newNumSamples < 0 )
    newNumSamples = 0;
  if( newNumSamples != mNumSamples )
  {
    ScopedLock( mDataLock );
    GenericSignal newData( mData.Channels(), newNumSamples );
    int idx1 = mSampleCursor,
        idx2 = newNumSamples - mNumSamples;
    if( idx2 < 0 )
    {
      idx1 = ( idx1 - idx2 ) % mNumSamples;
      idx2 = 0;
    }
    for( int i = 0; i < idx2; ++i )
      for( int ch = 0; ch < newData.Channels(); ++ch )
        newData( ch, i ) = NaN( newData( ch, i ) );
    while( idx2 < newNumSamples )
    {
      for( int ch = 0; ch < newData.Channels(); ++ch )
        newData( ch, idx2 ) = mData( ch, idx1 );
      ++idx1 %= mNumSamples;
      ++idx2;
    }
    mData = newData;
    mSampleCursor = 0;
    Invalidate();
    mNumSamples = newNumSamples;
  }
  return *this;
}

SignalDisplay&
SignalDisplay::Paint( const void* inUpdateRgn )
{
  if( mParent == NULL )
    return *this;

  ScopedLock( mDataLock );
  PaintInfo p;
  SetupPainting( p, inUpdateRgn );
  ClearBackground( p );
  DrawXTicks( p );
  DrawAxes( p );
  switch( mDisplayMode )
  {
    case polyline:
      DrawSignalPolyline( p );
      DrawYLabels( p, true );
      DrawValueUnit( p );
      break;
    case field2d:
      DrawSignalField2d( p );
      DrawYLabels( p, false );
      break;
    default:
      Assert( false );
  }
  DrawMarkerChannels( p );
  DrawMarkers( p );
  DrawChannelLabels( p );
  DrawCursor( p );

#ifdef TEST_UPDATE_RGN
  p.painter->fillRect( mDisplayRect, QColor( ::rand() << 8 & 0x00ffff00 ) );
#endif // TEST_UPDATE_RGN

  CleanupPainting( p );
  return *this;
}

std::string
SignalDisplay::GetNumericValue( int ch )
{
  if( ch >= mData.Channels() || mData.Elements() < 1 )
    return " ";
  int sampleNumber = mSampleCursor - 1;
  if( sampleNumber < 0 )
    sampleNumber += mData.Elements();
  double value =  mData( ch, sampleNumber );

  ostringstream oss;
  oss.setf( ios::fixed );
  if( mShowValueUnit )
  {
    oss.precision( 2 );
    oss.width( 6 );
    if( value == Inf( value ) || value == -Inf( value ) || value == 0 || IsNaN( value ) )
      oss << Pretty( value );
    else
      oss << mValueUnit.RawToPhysical( value ).Utf8();
  }
  else
  {
    oss.precision( 10 );
    oss << Pretty( value );
  }
  return oss.str();
}


void
SignalDisplay::SetupPainting( PaintInfo& p, const void* inUpdateRgn )
{
  SyncGraphics();

  p.updateRgn = reinterpret_cast<const QRegion*>( inUpdateRgn );
  p.painter = new QPainter( mParent );
  p.painter->setRenderHint( QPainter::Antialiasing, false );
  p.painter->setRenderHint( QPainter::TextAntialiasing, false );
  p.signalPens.resize( mData.Channels() );
  p.signalBrushes.resize( mData.Channels() );
  p.signalStyles.resize( mData.Channels() );

  // Background properties
  RGBColor c = mInverted ? RGBColor::White : RGBColor::Black;
  p.backgroundColor = QColor( c.R(), c.G(), c.B() );
  p.backgroundBrush = QBrush( p.backgroundColor );

  // Cursor properties
  p.cursorWidth = 3;
  c = RGBColor::Yellow;
  p.cursorColor = QColor( c.R(), c.G(), c.B() );
  p.cursorBrush = QBrush( p.cursorColor );

  // Axis properties
  RGBColor axisColor = mAxisColor;
  if( mInverted && ( axisColor == RGBColor::White ) )
    axisColor = RGBColor::Black;
  QColor qAxisColor( axisColor.R(), axisColor.G(), axisColor.B() );
  p.axisBrush = QBrush( qAxisColor );
  p.axisY = GroupBottom( mNumDisplayGroups - 1 );

  p.markerWidth = 1;
  p.baselinePen = QPen( qAxisColor );
  c = RGBColor( mInverted ? RGBColor::Black : RGBColor::White );
  p.markerColor = QColor( c.R(), c.G(), c.B() );
  p.markerBrush = QBrush( p.markerColor );
  p.labelColor = qAxisColor;
  p.labelFont = AxisFont();
  p.monoFont = MonoFont();
  p.symbolFont = SymbolFont();
  mSymbolWidth = QFontMetrics( p.symbolFont ).maxWidth();

  // Signal properties
  if( mDisplayColors )
    for( int i = 0; i < mData.Channels(); ++i )
    {
      RGBColor channelColor = ChannelColor( i );
      if( mInverted && channelColor == RGBColor::White )
        channelColor = RGBColor::Black;
      QColor c( channelColor.R(), channelColor.G(), channelColor.B() );
      p.signalPens[ i ] = QPen( c );
      p.signalBrushes[ i ] = QBrush( c );
    }
  else
  {
    RGBColor channelColor = mInverted? RGBColor::Black : RGBColor::White;
    QColor c( channelColor.R(), channelColor.G(), channelColor.B() );
    QPen pen( c );
    for( int i = 0; i < mData.Channels(); ++i )
      p.signalPens[ i ] = pen;
  }
  p.signalStyles = QString::fromUtf8( mChannelStyles.c_str() );
  while( p.signalStyles.length() < mData.Channels() )
    p.signalStyles.append( '-' );
}

void
SignalDisplay::ClearBackground( const PaintInfo& p )
{
  p.painter->fillRect( mDisplayRect, p.backgroundColor );
}

static void DrawLine( QPainter* p, const QString& style, const QPoint* points, int pointCount )
{
  if( style == "-" )
    p->drawPolyline( points, pointCount );
  else
  {
    QPoint delta = p->fontMetrics().tightBoundingRect( style ).center();
    for( int i = 0; i < pointCount; ++i )
      p->drawText( points[i] - delta, style );
  }
}

void
SignalDisplay::DrawSignalPolyline( const PaintInfo& p )
{
  float baseInterval = mNumDisplayGroups > 0
                       ? mDataHeight / mNumDisplayGroups
                       : mDataHeight;
  double maxY = mDataRect.bottom() + 1,
         minY = mDataRect.top() - 1;
  // Draw the baselines.
  if( mShowBaselines )
  {
    QPoint baselinePoints[ 2 ];
    p.painter->setPen( p.baselinePen );
    baselinePoints[ 0 ].setX( SampleLeft( 0 ) );
    baselinePoints[ 1 ].setX( SampleRight( mNumSamples ) );
    for( int i = 0; i < mNumDisplayGroups; ++i )
    {
      baselinePoints[ 0 ].setY(
        ToInt( ChannelBottom( i ) + ( baseInterval * mMinValue ) / ( mMaxValue - mMinValue ) )
      );
      baselinePoints[ 1 ].setY( baselinePoints[ 0 ].y() );
      p.painter->drawPolyline( baselinePoints, 2 );
    }
  }

  int sampleBegin = 0,
      sampleEnd = mNumSamples;
  if( p.updateRgn )
  { // We restrict drawing to the actually requested update region.
    QRect clipRect = p.updateRgn->boundingRect();
    sampleBegin = PosToSample( clipRect.left() );
    sampleBegin = max( sampleBegin - 1, 0 );
    sampleEnd = PosToSample( clipRect.right() + 1 );
    sampleEnd = min( sampleEnd + 1, mNumSamples );
  }
  delete[] mpSignalPoints;
  try
  {
    mpSignalPoints = new QPoint[mNumSamples];
  }
  catch( const bad_alloc& )
  {
    throw std_bad_alloc << "Could not allocate memory for " << mNumSamples << " points";
  }
  p.painter->setFont( p.symbolFont );
  int numPens = static_cast<int>( p.signalPens.size() );

  for( int j = sampleBegin; j < sampleEnd; ++j )
    mpSignalPoints[j].setX( SampleLeft( j ) );
  for( int i = 0; i < mNumDisplayChannels; ++i )
  {
    int channelBottom = ChannelBottom( i ),
        channel = i + mTopGroup * mChannelGroupSize;
    QString style = p.signalStyles[channel % p.signalStyles.length()];
    p.painter->setPen( p.signalPens[ channel % numPens ] );
    int runEnd = sampleBegin;
    while( runEnd < sampleEnd )
    {
      int runBegin = runEnd;
      while( runBegin < sampleEnd && IsNaN( NormData( channel, runBegin ) ) )
        ++runBegin;
      double value;
      runEnd = runBegin;
      while( runEnd < sampleEnd && !IsNaN( value = NormData( channel, runEnd ) ) )
      {
        double y = channelBottom - 1 - baseInterval * value;
        if( y > maxY )
          y = maxY;
        else if( y < minY )
          y = minY;
        mpSignalPoints[runEnd++].setY( ToInt( y ) );
      }
      if( runBegin <= mSampleCursor && mSampleCursor < runEnd )
      {
        DrawLine( p.painter, style, mpSignalPoints + runBegin, mSampleCursor - runBegin );
        DrawLine( p.painter, style, mpSignalPoints + mSampleCursor, runEnd - mSampleCursor );
      }
      else
      {
        DrawLine( p.painter, style, mpSignalPoints + runBegin, runEnd - runBegin );
      }
    }
  }
}

void
SignalDisplay::DrawSignalField2d( const PaintInfo& p )
{
  int sampleBegin = 0,
      sampleEnd = mNumSamples;
  if( p.updateRgn )
  { // We restrict drawing to the actually requested update region.
    QRect clipRect = p.updateRgn->boundingRect();
    sampleBegin = PosToSample( clipRect.left() );
    sampleBegin = max( sampleBegin - 1, 0 );
    sampleEnd = PosToSample( clipRect.right() + 1 );
    sampleEnd = min( sampleEnd + 1, mNumSamples );
  }
  for( int i = 0; i < mNumDisplayChannels; ++i )
  {
    for( int j = sampleBegin; j < sampleEnd; ++j )
    {
      bool draw = true;
      double dataValue = NormData( i + mTopGroup * mChannelGroupSize, j );
      if( dataValue < 0.0 )
        dataValue = 0.0;
      else if( dataValue > 1.0 )
        dataValue = 1.0;

      QRect dotRect(
          SampleLeft( j ),
          ChannelTop( i ),
          SampleRight( j ) - SampleLeft( j ),
          ChannelBottom( i ) - ChannelTop( i )
      );
      RGBColor rgb;
      if( IsNaN( dataValue ) )
        rgb = RGBColor::Black;
      else if( !mValueColors.empty() )
      {
        typedef decltype( *mValueColors.begin() ) pair;
        auto i = std::lower_bound(
          mValueColors.begin(), mValueColors.end(),
          dataValue,
          []( const pair& a, double b ) { return a.first < b; }
        );
        if( i == mValueColors.begin() )
          rgb = i->second;
        else if( i == mValueColors.end() )
          rgb = mValueColors.back().second;
        else
        {
          auto j = i--;
          double p = dataValue / ( j->first - i->first );
          rgb = ( 1 - p ) * i->second + p * j->second;
        }
      }
      else if( mDisplayColors )
        rgb = RGBColor::FromHSV( dataValue - 1.0 / 3.0, 1.0, dataValue );
      else
        rgb = RGBColor::FromHSV( 0.0, 0.0, dataValue );
      p.painter->fillRect( dotRect, QColor( rgb.R(), rgb.G(), rgb.B() ) );
    }
  }
}

void
SignalDisplay::DrawMarkerChannels( const PaintInfo& p )
{
  for( int markerCh = 0; markerCh < mMarkerChannels; ++markerCh )
  {
    p.painter->setPen( p.labelColor );
    p.painter->setFont( p.labelFont );
    p.painter->setBackgroundMode( Qt::TransparentMode );
    int baseline = MarkerChannelBottom( markerCh );
    size_t channelNumber = mData.Channels() - mMarkerChannels + markerCh;
    if( mData.Elements() > 0 )
    {
      uint32_t prevVal = ToInt( mData( channelNumber, 0 ) );
      for( int sample = 0; sample < mData.Elements(); ++sample )
      {
        uint32_t curVal = ToInt( mData( channelNumber, sample ) );
        if( curVal != prevVal )
        {
          QRect posRect;
          posRect.setLeft( SampleLeft( sample ) - p.markerWidth );
          posRect.setTop( baseline );
          posRect.setRight( SampleLeft( sample ) );
          posRect.setBottom( baseline );
          ostringstream oss;
          oss << curVal;
          p.painter->drawText( posRect,
            Qt::AlignHCenter | Qt::TextSingleLine | Qt::AlignBottom | Qt::TextDontClip,
            oss.str().c_str() );
          QRect markerRect;
          markerRect.setLeft( SampleLeft( sample ) - p.markerWidth );
          markerRect.setTop( GroupTop( 0 ) );
          markerRect.setRight( SampleLeft( sample ) );
          markerRect.setBottom( p.axisY );
          p.painter->fillRect( markerRect, p.axisBrush );
          prevVal = curVal;
        }
      }
    }
    p.painter->setBackground( p.backgroundBrush );
    p.painter->setBackgroundMode( Qt::OpaqueMode );
    QRect labelRect( cTickWidth, baseline, 0, 0 );
    const char* labelText = "";
    if( mChannelNameCache.size() > channelNumber )
      labelText = mChannelNameCache[ channelNumber ].c_str();
    p.painter->drawText( labelRect,
      Qt::AlignLeft | Qt::TextSingleLine | Qt::AlignBottom | Qt::TextDontClip,
      labelText );
  }
}

void
SignalDisplay::DrawCursor( const PaintInfo& p )
{
  if( mShowCursor )
  {
    int cursorSample = mSampleCursor;

    QRect cursorRect(
      SampleLeft( cursorSample ) - p.cursorWidth,
      0,
      p.cursorWidth,
      GroupBottom( mNumDisplayGroups - 1 )
    );
    p.painter->fillRect( cursorRect, p.cursorBrush );
    // Remember the cursor rectangle for redrawing when the next
    // data packet arrives.
    mCursorRect = cursorRect;
  }
}

void
SignalDisplay::DrawXTicks( const PaintInfo& p )
{
  p.painter->setPen( p.labelColor );
  p.painter->setFont( p.labelFont );
  p.painter->setBackgroundMode( Qt::TransparentMode );
  float xStart = 0,
        xDivision = 1;
  if( mNumSamples > 0 )
  { // Are samples spaced too dense to allow for individual labels?
    float pixelsPerSample = ( SampleLeft( mNumSamples - 1 ) - SampleLeft( 0 ) ) / mNumSamples;
    if( pixelsPerSample < 10 )
    { // Samples are dense
      float displayLength = ::fabs( mNumSamples * mSampleUnit.Gain() ),
            scale = ::pow( 10.0, ::floor( ::log10( displayLength ) + 0.5 ) );
      xDivision = scale / mSampleUnit.Gain() / 5,
      xStart = xDivision - ::fmod( float( -mSampleUnit.Offset() ), xDivision );
    }
  }
  if( xDivision < 1 )
    xDivision = 1;

  int nextLabelPos = mDataRect.left();
  for( float j = xStart; j < float( mNumSamples ); j += xDivision )
  {
    int tickX = 0;
    switch( mDisplayMode )
    {
      case field2d:
        tickX = ( SampleRight( ToInt( j ) ) + SampleLeft( ToInt( j ) ) ) / 2;
        break;
      case polyline:
      default:
        tickX = SampleLeft( ToInt( j ) );
    }
    QRect tickRect(
        tickX - cTickWidth / 2,
        p.axisY + cAxisWidth,
        cTickWidth,
        mTickLength
    );
    p.painter->fillRect( tickRect, p.axisBrush );
    if( tickX > nextLabelPos )
    {
      tickRect.setTop( tickRect.top() + ( mTickLength + 3 ) / 2 );
      ostringstream label;
      if( mTimeLabels )
      {
        float val  = ( j - mSampleUnit.Offset() ) * mSampleUnit.Gain();
        label << ' ' << setfill( '0' )
              << setw( 2 ) << int( val ) / 60 << ':'
              << setw( 2 ) << setprecision( 2 ) << int( val ) % 60;
        if( ::fmod( xDivision * mSampleUnit.Gain(), 1.0 ) != 0.0 )
          label << '.' << setw( 2 ) << int( 100 * val + 0.5 ) % 100;
        label << ' ';
      }
      else
      {
        label << ' ' << setprecision( 6 ) << mSampleUnit.RawToPhysical( j, mNumSamples ).Utf8() << ' ';
      }

      p.painter->drawText( tickRect,
        Qt::AlignHCenter | Qt::TextSingleLine | Qt::AlignTop | Qt::TextDontClip,
        label.str().c_str() );
      nextLabelPos = tickX + p.painter->fontMetrics().width( label.str().c_str() );
    }
  }
}

void
SignalDisplay::DrawYLabels( const PaintInfo& p, bool inDrawTicks )
{
  p.painter->setPen( p.labelColor );
  p.painter->setBackgroundMode( Qt::TransparentMode );
  int nextLabelPos = mDataRect.top();
  for( int i = 0; i < mNumDisplayGroups; ++i )
  {
    size_t channelNumber = ( mTopGroup + i ) * mChannelGroupSize;
    int    tickY = ( GroupTop( i ) + GroupBottom( i ) ) / 2;
    if( inDrawTicks )
    {
      QRect tickRect(
          mDataLeft - cAxisWidth - mTickLength,
          tickY - cTickWidth / 2,
          mTickLength,
          cTickWidth
      );
      if( mDisplayColors && mChannelGroupSize == 1 )
      {
        tickRect.setTop( tickRect.top() - 1 );
        tickRect.setBottom( tickRect.bottom() + 1 );
        p.painter->fillRect( tickRect, p.signalBrushes[ channelNumber ] );
      }
      else
      {
        p.painter->fillRect( tickRect, p.axisBrush );
      }
    }
    if( tickY >= nextLabelPos )
    {
      const char* labelText = "";
      if( mChannelGroupSize == 1 && mChannelNameCache.size() > channelNumber )
        labelText = mChannelNameCache[channelNumber].c_str();
      if( mChannelGroupSize == 1 )
      {
        QRect r( 0, tickY - cTickWidth / 2, 0, 0 );
        if( mShowNumericValues )
          r.setRight( NumericValuesLeft() - 2 * cAxisWidth );
        else
          r.setRight( NumericValuesLeft() );
        p.painter->setFont( p.labelFont );
        p.painter->setPen( p.labelColor );
        p.painter->drawText( r, Qt::AlignVCenter | Qt::TextSingleLine | Qt::AlignRight | Qt::TextDontClip, labelText );
        if( mShowNumericValues )
        {
          p.painter->setPen( p.signalPens[channelNumber] );
          r.setRight( NumericValuesRight() );
          DrawNumericValue( p, r, GetNumericValue( channelNumber ).c_str() );
        }
      }
      nextLabelPos = tickY + p.painter->fontMetrics().height();
    }
  }
}

void
SignalDisplay::DrawAxes( const PaintInfo& p )
{
  QRect xAxis(
      0,
      p.axisY,
      mDisplayRect.right(),
      cAxisWidth
  );
  p.painter->fillRect( xAxis, p.axisBrush );
  QRect yAxis;
  yAxis.setLeft( mDataLeft - cAxisWidth );
  yAxis.setTop( mMarkerChannels > 0 ? GroupTop( 0 ) : 0 );
  yAxis.setWidth( cAxisWidth );
  yAxis.setBottom( mDisplayRect.bottom() );
  p.painter->fillRect( yAxis, p.axisBrush );
  if( mChannelGroupSize == 1 && mShowNumericValues )
  {
    QRect divider( NumericValuesLeft() - cAxisWidth, yAxis.top(), cAxisWidth, yAxis.height() );
    p.painter->fillRect( divider, p.axisBrush );
  }
  if( mMarkerChannels > 0 )
  {
    QRect divider;
    divider.setLeft( 0 );
    divider.setRight( mDisplayRect.right() );
    divider.setTop( GroupTop( 0 ) - cAxisWidth );
    divider.setBottom( GroupTop( 0 ) );
    p.painter->fillRect( divider, p.axisBrush );
  }
}

void
SignalDisplay::DrawMarkers( const PaintInfo& p )
{
  // Draw markers.
  for( size_t i = 0; i < mXAxisMarkers.size(); ++i )
  {
    int markerX = SampleRight( ToInt( mXAxisMarkers[ i ].Address() + mSampleUnit.Offset() ) );
    QRect markerBar(
        markerX - cAxisWidth / 2,
        p.axisY - 4 * cAxisWidth,
        cAxisWidth,
        4 * cAxisWidth - 1
    );
    p.painter->fillRect( markerBar, p.markerBrush );
  }
}

void
SignalDisplay::DrawChannelLabels( const PaintInfo& p )
{
  if( mChannelGroupSize > 1 )
  {  // Draw channel labels when channels don't coincide with groups.
    p.painter->setBackground( p.backgroundColor );
    p.painter->setBackgroundMode( Qt::OpaqueMode );
    int deltaY = QFontMetrics( p.labelFont ).height();
    QRect legendRect( 0, cAxisWidth, NumericValuesRight(), deltaY );
    for( size_t i = 0; i < mChannelLabels.size(); ++i )
    {
      p.painter->fillRect( legendRect, p.backgroundColor );
      RGBColor textColor = ChannelColor( mChannelLabels[ i ].Address() );
      if( mInverted && textColor == RGBColor::White )
        textColor = RGBColor::Black;
      p.painter->setPen( QColor( textColor.R(), textColor.G(), textColor.B() ) );
      if( mShowChannelLabels )
      {
        p.painter->setFont( p.labelFont );
        p.painter->drawText( legendRect.adjusted( cAxisWidth, 0, 0, 0 ),
          Qt::AlignVCenter | Qt::TextSingleLine | Qt::AlignLeft | Qt::TextDontClip,
          mChannelLabels[i].Text().c_str() );
      }
      if( mShowNumericValues )
        DrawNumericValue( p, legendRect, GetNumericValue( i ).c_str() );
      legendRect.translate( 0, deltaY );
    }
  }
}

void
SignalDisplay::DrawValueUnit( const PaintInfo& p )
{
  float baseInterval = mDataHeight;
  if( mNumDisplayGroups > 0 )
    baseInterval /= mNumDisplayGroups;

  if( mShowValueUnit && mNumDisplayGroups > 0 && baseInterval > 0 )
  {
    p.painter->setFont( p.labelFont );
    p.painter->setPen( p.backgroundColor );
    p.painter->setBackground( p.markerColor );
    p.painter->setBackgroundMode( Qt::OpaqueMode );
    // Find a rounded value that is close to display range.
    double unitsPerPixel = ::fabs( ( mMaxValue - mMinValue ) * mValueUnit.Gain() / baseInterval ),
           scale = ::pow( 10.0, ::ceil( ::log10( unitsPerPixel * 0.95 * baseInterval ) ) );
    int percent = 100;
    while( percent * scale / unitsPerPixel >= 95 * baseInterval
           && percent * scale / unitsPerPixel > 100 * mMarkerHeight )
      percent -= 10;
    double rulerLength = percent * scale / 100;
    int pixelLength = ToInt( rulerLength / unitsPerPixel );

    std::ostringstream oss;
    oss << mValueUnit.RawToPhysical( rulerLength / mValueUnit.Gain() ).Utf8();
    QString label( oss.str().c_str() );
    QRect labelRect;
    if( mMinValue == 0 )
    {
      int left = SampleLeft( 0 ) + mTickLength,
          top = ChannelBottom( 0 ) - pixelLength;
      labelRect = QRect( left, top, 0, 0 );
      p.painter->drawText( labelRect,
        Qt::AlignTop | Qt::TextSingleLine | Qt::AlignLeft,
        label, &labelRect );
      QRect lineRect;
      lineRect.setLeft( SampleLeft( 0 ) );
      lineRect.setTop( labelRect.top() );
      lineRect.setRight( left );
      lineRect.setHeight( 1 );
      p.painter->fillRect( lineRect, p.markerBrush );
    }
    else
    {
      int left = SampleLeft( 0 ),
          center = ( GroupBottom( mNumDisplayGroups - 1 ) + GroupTop( mNumDisplayGroups - 1 ) ) / 2;
      labelRect = QRect( left, center, 0, 0 );
      p.painter->drawText( labelRect,
        Qt::AlignVCenter | Qt::TextSingleLine | Qt::AlignLeft,
        label, &labelRect );
      QRect lineRect;
      lineRect.setLeft( labelRect.left() );
      lineRect.setTop( center - ( pixelLength + p.markerWidth ) / 2 + 1 );
      lineRect.setRight( labelRect.right() );
      lineRect.setBottom( center - ( pixelLength - p.markerWidth ) / 2 );
      p.painter->fillRect( lineRect, p.markerBrush );
      lineRect.setTop( lineRect.top() + pixelLength );
      lineRect.setBottom( lineRect.bottom() + pixelLength );
      p.painter->fillRect( lineRect, p.markerBrush );
    }
    labelRect.adjust( -1, 0, 1, 0 );
    p.painter->fillRect( labelRect, p.markerBrush );
    labelRect.adjust( 1, 0, -1, 0 );
    p.painter->drawText( labelRect,
      Qt::AlignVCenter | Qt::TextSingleLine | Qt::AlignLeft,
      label );
  }
}

void
SignalDisplay::DrawNumericValue( const PaintInfo& p, const QRect& inRect, const char* inValue )
{
  p.painter->setFont( p.monoFont );
  int w = p.painter->fontMetrics().width( inValue );
  if( mNumericValueWidth < w )
  {
    mNumericValueWidth = w;
    SyncLabelWidth();
    Invalidate();
  }
  p.painter->drawText( inRect,
    Qt::AlignVCenter | Qt::TextSingleLine | Qt::AlignRight | Qt::TextDontClip,
    inValue );
}

void
SignalDisplay::CleanupPainting( PaintInfo& p )
{
  delete p.painter;
}


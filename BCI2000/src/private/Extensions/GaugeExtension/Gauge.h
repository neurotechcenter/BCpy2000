////////////////////////////////////////////////////////////////////////////////
// $Id: Gauge.h 1782 2008-02-15 11:13:42Z milsapg $
// Author: griffin.milsap@gmail.com
// Description: Handles rendering and maintenance of a gauge
//
// (C) 2000-2008, BCI2000 Project
// http://www.bci2000.org
////////////////////////////////////////////////////////////////////////////////
#ifndef GAUGE_H
#define GAUGE_H

#include <string>
#include <vector>
#include "Scale.h"
#include "Shapes.h"
#include "TextField.h"
#include "GraphObject.h"
#include "Expression.h"
//#include "TPoint.h"
#include "Quantiles.h"

class Dimension
{
public:
  Dimension() : mControlExpression( "0" ), mColor( 0 ) { mValue = 0.0; mMin = 0.0; mMax = 1.0; mQMin = mQMax = -1.0; mExpansion = 0.0; }
  ~Dimension() {}

  void SetControlExpression( std::string cs ) { mControlExpression = Expression( cs.size() ? cs.c_str() : "0" ); }
  Expression& ControlExpression() { return mControlExpression; }
  void SetBounds( double min, double max ) { mMin = min; mMax = max; }
  double Min() { return mMin; }
  double Max() { return mMax; }
  void InitAutoscale( double qmin, double qmax, unsigned int capacity, unsigned int lifetime ) { mQMin = qmin; mQMax = qmax; mQuantilizer.Reset( capacity, lifetime ); }
  void SetColor( const RGBColor& color ) { mColor = color; }
  RGBColor& Color() { return mColor; }
  void SetExpansion( double val ) { mExpansion = val; }
  double Expansion() { return mExpansion; }
  bool Evaluate( const GenericSignal& input, bool feedValueToQuantilizer=false )
  {
    mValue = mControlExpression.Evaluate( &input );
    if( feedValueToQuantilizer )
    {
      double minVal = mMin;
      double maxVal = mMax;
      if( mQMin >= 0.0 || mQMax >= 0.0 ) mQuantilizer << mValue;
      if( mQMin >= 0.0 ) minVal = mQuantilizer.Quantile( mQMin );
      if( mQMax >= 0.0 ) maxVal = mQuantilizer.Quantile( mQMax );
      double midVal = ( minVal + maxVal ) / 2.0;
      if( mQMin >= 0.0 ) minVal = midVal - ( midVal - minVal ) * ( 1 + mExpansion );
      if( mQMax >= 0.0 ) maxVal = midVal + ( maxVal - midVal ) * ( 1 + mExpansion );
      if( maxVal == minVal ) { maxVal += 1.0; minVal -= 1.0; }
      if( minVal != mMin || maxVal != mMax ) { SetBounds( minVal, maxVal ); return true; }
    }
    return false;
  }
  double NormalizedValue() { return Normalize( mValue ); }
  double Normalize( double value )
  {
    double retval = ( value - mMin ) / ( mMax - mMin );
    if( retval <= 0.0f ) return 0.0f;
    if( retval >= 1.0f ) return 1.0f;
    return retval;
  }
  double RawValue() { return mValue; }

private:
  Expression mControlExpression;
  RGBColor mColor;
  double mValue;
  double mMin, mMax;
  double mQMin, mQMax;
  double mExpansion;
  Quantilizer<double> mQuantilizer;
};

// Gauge is the parent class for all types of gauges
class Gauge : public GUI::GraphObject
{
public:
  // Constructors
  Gauge( GUI::GraphDisplay& display, int zOrder );
  ~Gauge()
  {
    for( std::vector<Dimension*>::iterator it = mDimensions.begin(); it != mDimensions.end(); ++it )
      delete (*it);
    mDimensions.clear();
  }

  // Caption Accessors
  void SetCaption( std::string caption ) { mCaption = caption; UpdateStatics(); }
  std::string Caption() { return mCaption; }
  void SetCaptionSize( float size ) { mCaptionSize = size; UpdateStatics(); }
  float CaptionSize() { return mCaptionSize; }

  // Control Signal Accessors
  Dimension & GetDimension( unsigned int whichDim ) { while( whichDim >= mDimensions.size() ) mDimensions.push_back( new Dimension() ); return *mDimensions[ whichDim ]; }

  void Update( const GenericSignal& input, double millisecondsSinceRunStart );

  // Scale Accessors
  void ShowScale( bool show ) { mShowScale = show; UpdateStatics(); }
  bool ScaleShowing() { return mShowScale; }
  void SetHScaleFormat( std::string & fmt ) { mHScaleFormat = fmt; UpdateStatics(); }
  std::string HScaleFormat() { return mHScaleFormat; }
  void SetVScaleFormat( std::string & fmt ) { mVScaleFormat = fmt; UpdateStatics(); }
  std::string VScaleFormat() { return mVScaleFormat; }
  void SetHScalePrecision( int precision ) { mHScalePrecision = precision; UpdateStatics(); }
  int HScalePrecision() { return mHScalePrecision; }
  void SetVScalePrecision( int precision ) { mVScalePrecision = precision; UpdateStatics(); }
  int VScalePrecision() { return mVScalePrecision; }

  // Virtual Interface
  virtual void Update( ) = 0;
  virtual void OnChange( GUI::DrawContext& inDC );
  virtual void UpdateStatics() = 0;

protected:
  // Fake millisecond timestamp
  unsigned short mNow;

  // Display Context
  GUI::GraphDisplay* mpDisplay;

  // Caption properties
  std::string mCaption;
  float mCaptionSize;

  // Positioning is in percentage of screen width/height
  float mX, mY; // Position of upper left corner
  float mWidth, mHeight;

  // Control Signals and associated values
  std::vector<Dimension*> mDimensions;

  // Scale properties
  bool mShowScale;
  int mHScalePrecision;
  int mVScalePrecision;
  std::string mHScaleFormat;
  std::string mVScaleFormat;
  int mGaugeIndex;
};

// Helper Functions
float Normalize( float value, float min, float max );

#endif // GAUGE_H

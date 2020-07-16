////////////////////////////////////////////////////////////////////////////////
// $Id: VisDisplayGraph.h 5417 2016-07-02 15:53:30Z mellinger $
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
#ifndef VIS_DISPLAY_GRAPH_H
#define VIS_DISPLAY_GRAPH_H

#include "VisDisplayLayer.h"
#include "DisplayFilter.h"
#include "DecimationFilter.h"
#include "ScaleObservationFilter.h"
#include "SignalDisplay.h"

class QMenu;
class QAction;
class QMouseEvent;
class QLabel;
class QStatusBar;

class VisDisplayGraph : public VisDisplayLayer
{
  Q_OBJECT

  enum
  {
    cMaxUserScaling = 4, // The maximum number of scaling steps a user
                        // can take from the default.
    cDisplayOversampling = 10, // The number of data points drawn per pixel, if available.
    cStatusBarHeight = 12, // The height of the status bar, in pixels.
  };

 public:
  VisDisplayGraph( const std::string& visID );
  virtual ~VisDisplayGraph();

 protected:
  void OnSignal( const GenericSignal& ) override;
  void OnSetConfig( ConfigSettings& ) override;
  void OnContextMenu( const QPoint& ) override;

  // User interaction.
 private:
  void BuildStatusBar();
  void ShowStatusBar( bool );
  void BuildContextMenu();

  QMenu*   mpContextMenu,
       *   mpHPMenu,
       *   mpLPMenu,
       *   mpNotchMenu;
  QAction* mpActEnlargeSignal,
         * mpActReduceSignal,
         * mpActToggleAutoScale,
         * mpActFewerSamples,
         * mpActMoreSamples,
         * mpActMoreChannels,
         * mpActFewerChannels,
         * mpActToggleDisplayMode,
         * mpActToggleNumericValues,
         * mpActToggleBaselines,
         * mpActToggleValueUnit,
         * mpActToggleChannelLabels,
         * mpActToggleColor,
         * mpActInvertDisplay,
         * mpActChooseColors,
         * mpActLargerFont,
         * mpActSmallerFont;
  QStatusBar* mpStatusBar;
  QLabel* mpStatusLabel;

 private slots:
  // Menu action slots
  void EnlargeSignal();
  void ReduceSignal();
  void ToggleAutoScale();
  void FewerSamples();
  void MoreSamples();
  void MoreChannels();
  void FewerChannels();
  void ToggleDisplayMode();
  void ToggleNumericValues();
  void ToggleBaselines();
  void ToggleValueUnit();
  void ToggleChannelLabels();
  void LargerFont();
  void SmallerFont();
  void ToggleColor();
  void InvertDisplay();
  void ChooseColors();
  void SetHPOff();
  void SetHP01();
  void SetHP1();
  void SetHP5();
  void SetLPOff();
  void SetLP30();
  void SetLP40();
  void SetLP70();
  void SetNotchOff();
  void SetNotch50();
  void SetNotch60();

 private:
  bool EnlargeSignal_Enabled() const;
  bool ReduceSignal_Enabled() const;
  bool ToggleAutoScale_Enabled() const;
  bool ToggleAutoScale_Checked() const;
  bool FewerSamples_Enabled() const;
  bool MoreSamples_Enabled() const;
  bool MoreChannels_Enabled() const;
  bool FewerChannels_Enabled() const;
  bool ToggleNumericValues_Enabled() const;
  bool ToggleNumericValues_Checked() const;
  bool ToggleBaselines_Enabled() const;
  bool ToggleBaselines_Checked() const;
  bool ToggleValueUnit_Enabled() const;
  bool ToggleValueUnit_Checked() const;
  bool ToggleChannelLabels_Enabled() const;
  bool ToggleChannelLabels_Checked() const;
  bool SmallerFont_Enabled() const;
  bool ToggleColor_Enabled() const;
  bool ToggleColor_Checked() const;
  bool InvertDisplay_Checked() const;
  bool ChooseColors_Enabled() const;
  bool Filter_Enabled() const;
  double FilterUnitToValue( const std::string& inUnit ) const;
  double FilterCaptionToValue( const char* inCaption ) const;
  
  int    NominalDisplaySamples() const;
  void   SetNominalDisplaySamples( int );
  double NominalUnitsPerSample() const;
  void   SetNominalUnitsPerSample( double );

  void SetHP( const std::string&, bool writeToConfig = false );
  void SetLP( const std::string&, bool writeToConfig = false );
  void SetNotch( const std::string&, bool writeToConfig = false );
  void UpdateStatusBar();
  QAction* SyncFilterMenu( const std::string&, QMenu* );
  QAction* NextMenuItem( QMenu*, QAction* );

 private:
  int   mNumChannels,
        mSignalElements,
        mUserScaling,
        mUserZoom;
  std::string mLastHP,
              mLastLP,
              mLastNotch;
  QAction* mpCurrentHPItem,
         * mpCurrentLPItem,
         * mpCurrentNotchItem;
  DisplayFilter    mDisplayFilter;
  DecimationFilter mDecimationFilter;
  SignalDisplay mDisplay;
  bool mAutoScale, mFixedScale;
  float mMinValue, mMaxValue, mElementGain;
  ScaleObservationFilter mScaleObserver;

 private:
  void SyncDisplay();

 protected:
  // Qt event handlers
  void paintEvent( QPaintEvent* );
  void moveEvent( QMoveEvent* );
  void resizeEvent( QResizeEvent* );
  void keyReleaseEvent( QKeyEvent* );
  void mousePressEvent( QMouseEvent* );
};

#endif // VIS_DISPLAY_GRAPH_H

////////////////////////////////////////////////////////////////////////////////
// $Id: BCI2000Viewer.h 4607 2013-10-14 13:18:08Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: The main window of the BCI2000Viewer tool.
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
#ifndef BCI2000VIEWER_H
#define BCI2000VIEWER_H

#include <QMainWindow>
#include <QtWidgets>
#include <QtAV>
#include <QtAVWidgets>

#include "Color.h"
#include "GenericSignal.h"
#include "BCI2000FileReader.h"
#include "DisplayFilter.h"
#include "playerwindow.h"
#include "Clock.h"
//#include <QtAV.h>

//#include "VideoPlayer.h"

//#include <QtMultimedia\QMediaPlayer>
//#include <QtMultimedia\QMediaContent>
//#include <QtMultimedia\QMediaPlaylist>
//#include <QtMultimedia\QAbstractVideoSurface>
//#include <QtMultimedia\QVideoSurfaceFormat>


namespace Ui
{
    class BCI2000Viewer;
}

class UpdateThread;

class BCI2000Viewer : public QMainWindow
{
    Q_OBJECT

public:
    BCI2000Viewer(QWidget *parent = 0);
    ~BCI2000Viewer();

private:
    Ui::BCI2000Viewer *ui;

protected:
    // Qt event handlers
    void dragEnterEvent(QDragEnterEvent*);
    void dropEvent(QDropEvent*);
    void keyPressEvent(QKeyEvent*);

private slots:
    void on_signalDisplay_customContextMenuRequested(QPoint pos);
    void on_editPosition_textEdited(QString);
    void on_editPosition_editingFinished();
    void on_editPosition_returnPressed();
    void on_editPosition_selectionChanged();
    void on_channelList_itemChanged(QListWidgetItem*);
    void on_channelList_itemSelectionChanged();
    void on_channelList_currentRowChanged(int currentRow);
    void on_verticalScrollBar_valueChanged(int value);

private:
    static struct ActionEntry
    {
      typedef void (BCI2000Viewer::*Action)();
      typedef bool (BCI2000Viewer::*StateGetter)() const;
      Action      action;
      StateGetter enabled,
                  checked;
      const char* object,
                * group;
    } sActions[];
    void SetupActions();
    void UpdateActions();
private slots:
    void ExecuteAction();
	void updateSignalViewerSlot();

public:
    // User actions
    // Standard actions
    void FileOpen();
    void FileClose();
    bool FileClose_Enabled() const;
    void FileQuit();
    void EditCopy();
    bool EditCopy_Enabled() const;
    void HelpOpenHelp();
    void HelpAbout();
    // Temporal movement
    void ToFirstSample();
    void ToLastSample();
    void ToPrevBlock();
    void ToNextBlock();
	void ToPlayForward();
    void ToPrevPage();
    void ToNextPage();
    bool GoBack_Enabled() const;
    bool GoForward_Enabled() const;
    bool GoPlayForward_Enabled() const;
    // Temporal resolution
    void SampleZoomIn();
    bool SampleZoomIn_Enabled() const;
    void SampleZoomOut();
    bool SampleZoomOut_Enabled() const;
    // Number of displayed channels
    void FewerChannels();
    bool FewerChannels_Enabled() const;
    void MoreChannels();
    bool MoreChannels_Enabled() const;
    // Channel scrolling
    void ChannelUp();
    void ChannelDown();
    void ChannelPageNext();
    void ChannelPagePrev();
    void ChannelPageFirst();
    void ChannelPageLast();
    bool ChannelUp_Enabled();
    bool ChannelDown_Enabled();
    // Signal resolution
    void EnlargeSignal();
    void ReduceSignal();
    bool ChangeResolution_Enabled() const;
    // Display attributes
    void ChooseChannelColors();
    bool ChooseChannelColors_Enabled() const;
    void Invert();
    bool Invert_Checked() const;
    bool Invert_Enabled() const;
    void ToggleBaselines();
    bool ToggleBaselines_Checked() const;
    bool ToggleBaselines_Enabled() const;
    void ToggleUnit();
    bool ToggleUnit_Checked() const;
    bool ToggleUnit_Enabled() const;
    // Filters
    void HPOff();
    void HPRemoveMean();
    void HP1Hz();
    void HP5Hz();
    void LPOff();
    void LP30Hz();
    void LP40Hz();
    void LP70Hz();
    void NotchOff();
    void Notch50Hz();
    void Notch60Hz();
    bool Filter_Enabled() const;
    // Channel List
    void ShowSelectedChannels();
    void HideSelectedChannels();
    bool ChannelsSelected() const;
    void HelpOnState();
    bool StateSelected() const;

  private:
    // Internal functions
    void DoFileOpen( const QString& name );
    void FillChannelList();
    void SaveSettings() const;
    void ReadSettings();
    void UpdateSamplePos();
    void UpdateTimeField();
    void UpdateChannelLabels();
    void UpdateVerticalScroller();
    const GenericSignal& ConstructDisplaySignal( long samplePos, long length );
    void FilterChanged();
    bool FilterActive();

  protected:
	void SetSamplePos( long sampleIndex );
    void MoveSamplePos( long sampleIndexDiff );
    void WarpSamplePos( long sampleIndexDiff );

    static const RGBColor cAxisColor;
    static const RGBColor cChannelColorsDefault[];
    static const QColor   cHeadingColor;

    BCI2000FileReader  mFile;
    long long          mSamplePos;
    int                mNumSignalChannels;
    bool               mPositionEdited;
    float              mSamplingRate;
    DisplayFilter      mFilter;
    bool               mRemoveMean;
//	Clock			   mClock;

 //   PlayerWindow player;
	PlayerWindow      *mpPlayerWindow;
	UpdateThread	  *mpThread;

	// Qt5 Multimedia objects
//	QMediaPlayer* mpMediaPlayer;
//	QMediaPlaylist* mpMediaPlaylist;
//	QVideoWidget* mpVideoWidget;
//	QBoxLayout* mpBoxLayout;
};

class UpdateThread : public QThread 
{

 public: 

	 UpdateThread(BCI2000Viewer *pViewer)
	 {
		 this->mpViewer = pViewer;

//		mClock.SetIntervalMs( 1000 );
//		mClock.Start();
	 }

     Clock mClock;

	 BCI2000Viewer *mpViewer;

	 bool alive; 

     void run() 
	 {
		 while (alive) 
		 {

//			 mClock.Wait();

			 Sleep(1000);


//			 BCI2000Viewer* pviewer = dynamic_cast<BCI2000Viewer*>(this->parent);

//			 if (pviewer)
//				 pviewer->WarpSamplePos(1000);

			 // if file open and file playing

		 }
	 }
 };

#endif // BCI2000VIEWER_H

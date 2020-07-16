//////////////////////////////////////////////////////////////////////
// $Id: bci2000fileinfo.cpp 5414 2016-07-01 19:45:56Z mellinger $
// Authors: griffin.milsap@gmail.com
// Description: The main window of BCI2000FileInfo.
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
///////////////////////////////////////////////////////////////////////

#include "bci2000fileinfo.h"
#include "ui_bci2000fileinfo.h"
#include <QFileDialog>
#include <QtWidgets>
#include "Preferences.h"
#include "ConfigWindow.h"
#include "ShowStates.h"
#include "ExecutableHelp.h"
#include "AboutBox.h"
#include "GenericSignal.h"
#include "BitmapImage.h"
#include "Settings.h"
#include "defines.h"

#include <sstream>

using namespace std;

Preferences* gpPreferences = NULL;

#define EMPTY_FILENAME "<p align=center><i>&lt;drop a data file&gt;</i>" // using a macro allows translation

BCI2000FileInfo::BCI2000FileInfo(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::BCI2000FileInfo)
{
    if( gpPreferences == NULL )
        gpPreferences = new Preferences;
    ui->setupUi(this);
    ui->filename->setText( tr( EMPTY_FILENAME ) );
    setAcceptDrops( true );
    ReadSettings();
    if( qApp->arguments().size() > 1 )
      OpenFile( qApp->arguments().at( 1 ) );
}

BCI2000FileInfo::~BCI2000FileInfo()
{
    WriteSettings();
    delete ui;
    delete gpPreferences;
}

void BCI2000FileInfo::dragEnterEvent(QDragEnterEvent *event)
{
  // Only accept if the dragged object is a single BCI2000 file
  QList<QUrl> urlList = event->mimeData()->urls();
  if( urlList.size() == 1 )
  {
    QString file = urlList.first().toLocalFile();
    if( QFileInfo( file ).isFile() && BCI2000FileReader( file.toLocal8Bit() ).IsOpen() )
      event->acceptProposedAction();
  }
}

void BCI2000FileInfo::dropEvent(QDropEvent *event)
{
    QList<QUrl> urlList = event->mimeData()->urls();
    if( urlList.size() == 1 )
      OpenFile( urlList.first().toLocalFile() );
}

void BCI2000FileInfo::on_parmButton_clicked()
{
    // Open Populated Config Window
    if( gpConfig == NULL )
        gpConfig = new ConfigWindow( this );
    GetParameters();
    gpConfig->Initialize( &mParameters, gpPreferences );
    gpConfig->show();
}

void BCI2000FileInfo::on_actionOpen_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open Data File"), QDir::currentPath(), tr("BCI2000 Data Files (*.dat)"));
    if( !filename.isEmpty() )
    {
      QDir::setCurrent( QFileInfo( filename ).canonicalPath() );
      OpenFile( filename );
    }
}

void BCI2000FileInfo::on_actionAbout_triggered()
{
    //Do stuff for About
    AboutBox()
    .SetApplicationName( QApplication::applicationName().toLocal8Bit().constData() )
    .Display();
}

void BCI2000FileInfo::on_actionBCI2000_Help_triggered()
{
    //Do stuff for help
    ExecutableHelp()
    .Display();
}

void BCI2000FileInfo::OpenFile( QString filename )
{
    mFile.Open( filename.toLocal8Bit() );
    if( !mFile.IsOpen() )
    {
        QMessageBox msgBox;
        ostringstream oss;
        oss << "Could not open \"" << filename.toLocal8Bit().constData() << "\" as a BCI2000 data file.";
        msgBox.setText( QString::fromLocal8Bit( oss.str().c_str() ) );
        msgBox.setIcon( QMessageBox::Warning );
        msgBox.exec();
        ui->parmButton->setEnabled( false );
        ui->blockSize->setText( "" );
        ui->filename->setText( tr( EMPTY_FILENAME ) );
        ui->feedback->setText( "" );
        ui->samplingRate->setText( "" );
        ui->formatVer->setText( "" );
        ui->dataFormat->setText( "" );
    }
    else
    {
        ui->parmButton->setEnabled( true );
        ui->filename->setText( filename );
        float blockSize = mFile.SignalProperties().Elements();
        if( blockSize > 0 )
        {
            ui->blockSize->setText( mFile.Parameter( "SampleBlockSize" ).c_str() );
            ui->feedback->setText( QString::number( mFile.SamplingRate() / blockSize ).append( QString( " Hz" ) ) );
            ui->samplingRate->setText( QString::number( mFile.SamplingRate() ).append( QString( " Hz" ) ) );
            ui->formatVer->setText( mFile.FileFormatVersion().c_str() );
            ui->dataFormat->setText( mFile.SignalProperties().Type().Name() );
        }
    }
}

void BCI2000FileInfo::GetParameters()
{
    mParameters.Clear();
    if( mFile.IsOpen() )
        mParameters = *mFile.Parameters();
}

void BCI2000FileInfo::WriteSettings() const
{
  Settings settings;
  settings.beginGroup( KEY_CONFIG );
  settings.setValue( "Geometry", this->saveGeometry() );
  settings.setValue( "WorkingDirectory", QDir::currentPath() );
}

void BCI2000FileInfo::ReadSettings()
{
  Settings settings;
  settings.beginGroup( KEY_CONFIG );
  this->restoreGeometry( settings.value( "Geometry" ).toByteArray() );
  QDir::setCurrent( settings.value( "WorkingDirectory" ).toString() );
}

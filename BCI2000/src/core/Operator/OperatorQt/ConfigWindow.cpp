//////////////////////////////////////////////////////////////////////
// $Id: ConfigWindow.cpp 4625 2013-10-29 15:47:02Z mellinger $
// Authors: schalk@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: The Operator module's parameter configuration dialog.
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
#include "ConfigWindow.h"
#include "ui_ConfigWindow.h"

#include <vector>
#include <string>
#include <algorithm>
#include <QMessageBox>
#include <QLabel>
#include <QScrollArea>
#include <QScrollBar>
#include <QFileInfo>
#include <QFileDialog>
#include <QWhatsThis>
#include <QTemporaryFile>

#include "BCI2000FileReader.h"
#include "OperatorUtils.h"
#include "Preferences.h"
#include "ParamList.h"
#include "ShowParameters.h"
#include "FileUtils.h"
#include "ExecutableHelp.h"

using namespace std;

ConfigWindow* gpConfig = NULL;

ConfigWindow::ConfigWindow(QWidget *parent)
: QDialog(parent),
  m_ui(new Ui::ConfigWindow),
  mUserSwitchedTabs( false ),
  mpScrollArea( NULL ),
  mScrollPos( -1 )
{
  m_ui->setupUi(this);
  connect( this, SIGNAL(finished(int)), this, SLOT(OnClose()) );
  connect( m_ui->bConfigureLoad, SIGNAL(clicked()), this, SLOT(OnConfigureLoadFilterClick()) );
  connect( m_ui->bConfigureSave, SIGNAL(clicked()), this, SLOT(OnConfigureSaveFilterClick()) );
  connect( m_ui->bHelp, SIGNAL(clicked()), this, SLOT(OnHelpClick()) );
  connect( m_ui->bLoadParameters, SIGNAL(clicked()), this, SLOT(OnLoadParametersClick()) );
  connect( m_ui->bSaveParameters, SIGNAL(clicked()), this, SLOT(OnSaveParametersClick()) );
  connect( m_ui->bClose, SIGNAL(clicked()), this, SLOT(accept()) );
  connect( m_ui->cfgTabControl, SIGNAL(currentChanged(int)), this, SLOT(OnCfgTabControlChange()) );
  OperatorUtils::RestoreWidget( this );
  if( ExecutableHelp().ParamHelp().Empty() )
    m_ui->bHelp->setVisible( false );
  mOriginalTitle = this->windowTitle();
}

ConfigWindow::~ConfigWindow()
{
  OperatorUtils::SaveWidget( this );
  DisposeWidgets();
  delete m_ui;
}

void
ConfigWindow::changeEvent( QEvent* e )
{
  QDialog::changeEvent(e);
  switch (e->type()) {
  case QEvent::LanguageChange:
    m_ui->retranslateUi(this);
    break;
  default:
    break;
  }
}

static bool
CompareTabNames( const string& s1, const string& s2 )
{ // This comparison function makes sure that standard tabs have standard positions.
  static const char* leftTabs_[] =
  {
    "Visualize", "System", "Source", "Storage",
  };
  static vector<string> leftTabs(
    leftTabs_, leftTabs_ + sizeof( leftTabs_ ) / sizeof( *leftTabs_ )
  );
  vector<string>::const_iterator i1 = find( leftTabs.begin(), leftTabs.end(), s1 ),
                                 i2 = find( leftTabs.begin(), leftTabs.end(), s2 );
  return i1 < i2;
}

int
ConfigWindow::Initialize( ParamList* inParameters, Preferences* inPreferences )
{
  mpPreferences = inPreferences;
  mpParameters = inParameters;

  mpParameters->Sort();
  DeleteAllTabs();

  DisplayBase::InitMeasures( this->logicalDpiX() );

  vector<string> tabNames;
  for( int i = 0; i < mpParameters->Size(); ++i )
    if( OperatorUtils::GetUserLevel( ( *mpParameters )[ i ].Name().c_str() ) <= mpPreferences->mUserLevel )
    {
      string tabName = ( *mpParameters )[ i ].Section();
      if( std::find( tabNames.begin(), tabNames.end(), tabName ) == tabNames.end() )
        tabNames.push_back( tabName );
    }
  stable_sort( tabNames.begin(), tabNames.end(), CompareTabNames );

  mUserSwitchedTabs = false;
  for( vector<string>::const_iterator i = tabNames.begin(); i != tabNames.end(); ++i )
  {
    QScrollArea* pScrollArea = new QScrollArea;
    pScrollArea->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    m_ui->cfgTabControl->addTab( pScrollArea, i->c_str() );
    pScrollArea->setWidget( new QLabel );
  }
  mUserSwitchedTabs = true;

  if( m_ui->cfgTabControl->count() == 0 )
  {
    m_ui->cfgTabControl->insertTab( 0, new QWidget, tr("No parameter visible") );
    QMessageBox::information( this,
                              tr("Message"),
                              tr("No parameter visible! Increase user level"),
                              QMessageBox::Yes | QMessageBox::Default );
  }
  if( std::find( tabNames.begin(), tabNames.end(), mCurTab ) == tabNames.end() )
    mCurTab = "Storage";
  mUserSwitchedTabs = false;
  int curTabIdx = std::find( tabNames.begin(), tabNames.end(), mCurTab ) - tabNames.begin();
  if( curTabIdx != tabNames.size() )
    m_ui->cfgTabControl->setCurrentIndex( curTabIdx );
  mUserSwitchedTabs = true;
  mCurTab = m_ui->cfgTabControl->tabText( curTabIdx ).toLocal8Bit().constData();
  RenderParameters( mCurTab );
  if( mScrollPos >= 0 && mpScrollArea )
    mpScrollArea->verticalScrollBar()->setValue( mScrollPos );
  return 0;
}

void
ConfigWindow::DeleteAllTabs()
{
  mUserSwitchedTabs = false;
  // delete old Tabs, if present
  while( m_ui->cfgTabControl->count() > 0 )
    m_ui->cfgTabControl->removeTab( 0 );
  mUserSwitchedTabs = true;
}

// render all parameters in a particular section on the screen
void
ConfigWindow::RenderParameters( const string& inSection )
{
  if( inSection.empty() )
    return;

  DisposeWidgets();
  mParamDisplays.clear();

  int currentTop = DisplayBase::Measures[DisplayBase::buttonHeight_] / 2;
  string lastSubsection = "";
  QFrame* pLastBevel = NULL;
  QWidget* pTabWidget = m_ui->cfgTabControl->currentWidget();
  mpScrollArea = dynamic_cast<QScrollArea*>( pTabWidget );
  if( mpScrollArea == NULL )
    return;
  QWidget* pScrollingPane = new QWidget( mpScrollArea );
  delete mpScrollArea->widget();
  pScrollingPane->resize( DisplayBase::Measures[DisplayBase::totalWidth_] + 2 * DisplayBase::Measures[DisplayBase::leftMargin_], 1 );

  map<string, int>      subsectionIndex;
  vector<string>        subsectionTable;
  vector< vector<int> > subsectionGroups;

  for( int i = 0; i < mpParameters->Size(); ++i )
  {
    const Param& p = ( *mpParameters )[ i ];
    if( inSection == p.Section()
        && OperatorUtils::GetUserLevel( p.Name().c_str() ) <= mpPreferences->mUserLevel )
    {
      string subsection;
      if( p.Sections().size() > 1 )
        subsection = p.Sections()[ 1 ];
      if( subsectionIndex.find( subsection ) == subsectionIndex.end() )
      {
        subsectionIndex[ subsection ] = static_cast<int>( subsectionTable.size() );
        subsectionTable.push_back( subsection );
        subsectionGroups.resize( subsectionGroups.size() + 1 );
      }
      subsectionGroups[ subsectionIndex[ subsection ] ].push_back( i );
    }
  }

  for( size_t i = 0; i < subsectionTable.size(); ++i )
  {
    const string& subsection = subsectionTable[ i ];
    if( mpPreferences->mUserLevel > Preferences::Beginner )
    { // A bevel for each subsection.
      if( pLastBevel != NULL )
      {
        pLastBevel->resize( pLastBevel->size().width(), DisplayBase::Measures[DisplayBase::buttonHeight_] / 2 + currentTop - pLastBevel->y() );
        pLastBevel->show();
        pLastBevel = NULL;
        currentTop += DisplayBase::Measures[DisplayBase::buttonHeight_];
      }
      if( subsection != "" )
      {
        QFrame* pBevel = new QFrame( pScrollingPane );
        mWidgets.insert( pBevel );
        pBevel->setFrameShadow( QFrame::Sunken );
        pBevel->setFrameShape( QFrame::StyledPanel );
        pBevel->move( DisplayBase::Measures[DisplayBase::leftMargin_] / 2, currentTop );
        pBevel->resize( pScrollingPane->width() - DisplayBase::Measures[DisplayBase::leftMargin_], pBevel->height() );
        pLastBevel = pBevel;

        QLabel* pLabel = new QLabel( pScrollingPane );
        mWidgets.insert( pLabel );
        pLabel->setText( ( subsection + " " ).c_str() );
        QFont font = pLabel->font();
        font.setItalic( true );
        pLabel->setFont( font );
        pLabel->move( DisplayBase::Measures[DisplayBase::leftMargin_] / 2 + 4, currentTop + 2 );
        pLabel->show();

        currentTop += pLabel->height();
      }
    }
    int groupIndex = subsectionIndex[ subsection ];
    for( size_t j = 0; j < subsectionGroups[ groupIndex ].size(); ++j )
    {
      int paramIndex = subsectionGroups[ groupIndex ][ j ];
      const Param& p = ( *mpParameters )[ paramIndex ];
      ParamDisplay paramDisplay( p, pScrollingPane );
      paramDisplay.SetLeft( DisplayBase::Measures[DisplayBase::leftMargin_] );
      paramDisplay.SetTop( currentTop );
      paramDisplay.ReadValuesFrom( p );
      mParamDisplays[ p.Name() ] = paramDisplay;
      currentTop = paramDisplay.GetBottom();
    }
  }
  if( pLastBevel != NULL )
  {
    pLastBevel->resize( pLastBevel->width(), DisplayBase::Measures[DisplayBase::buttonHeight_] / 2 + currentTop - pLastBevel->y() );
    pLastBevel->show();
  }

  for( DisplayContainer::iterator i = mParamDisplays.begin(); i != mParamDisplays.end(); ++i )
    i->second.Show();

  pScrollingPane->resize( pScrollingPane->width(), currentTop + DisplayBase::Measures[DisplayBase::buttonHeight_] );
  mpScrollArea->setWidget( pScrollingPane );
}

// update one particular parameter on the screen
// useful, for example, if parameters change while stuff on screen
void
ConfigWindow::RenderParameter( Param *inParam )
{
  if( mParamDisplays.find( inParam->Name() ) != mParamDisplays.end() )
    mParamDisplays[ inParam->Name() ].ReadValuesFrom( *inParam );
}

// go through the parameters on the screen and update the parameters using the data on the screen
void
ConfigWindow::UpdateParameters()
{
  for( DisplayContainer::const_iterator i = mParamDisplays.begin(); i != mParamDisplays.end(); ++i )
    if( i->second.Modified() )
      i->second.WriteValuesTo( ( *mpParameters )[ i->first ] );
}

void
ConfigWindow::OnCfgTabControlChange()
{
  if( !mUserSwitchedTabs )
    return;

  UpdateParameters();
  int curTabIdx = m_ui->cfgTabControl->currentIndex();
  mCurTab = m_ui->cfgTabControl->tabText( curTabIdx ).toLocal8Bit().constData();
  RenderParameters( mCurTab );
}

void
ConfigWindow::OnClose()
{
 if( mpScrollArea )
   mScrollPos = mpScrollArea->verticalScrollBar()->value();
 UpdateParameters();
 mParamDisplays.clear();
 DisposeWidgets();
 DeleteAllTabs();
}

void
ConfigWindow::OnSaveParametersClick()
{
  QString fileName = QFileDialog::getSaveFileName(
    this, tr("Save Parameter File"), mpPreferences->mCurrentDir.canonicalPath(), tr("BCI2000 parameter files (*.prm)") );
  if( !fileName.isEmpty() )
  {
    mpPreferences->mCurrentDir = QFileInfo( fileName ).canonicalPath();
    UpdateParameters();

    ParamList paramsToSave;
    for( int i = 0; i < mpParameters->Size(); ++i )
      if( 0 == OperatorUtils::GetFilterStatus( (*mpParameters)[ i ].Name().c_str(), OperatorUtils::saveFilter ) )
        paramsToSave.Add( ( *mpParameters )[ i ] );

    bool result = paramsToSave.Save( fileName.toLocal8Bit().data() );
    if( result )
      this->setWindowTitle( mOriginalTitle + tr(" - wrote %1").arg( fileName ) );
    else
      QMessageBox::critical( this, tr("Error"), tr("Error writing parameter file") );
  }
}

bool
ConfigWindow::LoadParameters( const QString& inName )
{
  ParamList paramsFromFile = *mpParameters;
  paramsFromFile.Unchanged();
  bool result = false;
  QString fileExtension = QFileInfo( inName ).suffix();
  if( fileExtension == "dat" )
  {
    BCI2000FileReader file( inName.toLocal8Bit().constData() );
    if( file.IsOpen() )
    {
      FileUtils::TemporaryFile temp;
      file.Parameters()->Save( temp.Name() );
      result = paramsFromFile.Load( temp.Name(), false );
    }
  }
  else
    result = paramsFromFile.Load( inName.toLocal8Bit().constData(), false );

  for( int i = 0; i < paramsFromFile.Size(); ++i )
  {
    const Param& p = paramsFromFile[i];
    Param& q = (*mpParameters)[p.Name()];
    if( p.Changed() && 0 == OperatorUtils::GetFilterStatus( q.Name().c_str(), OperatorUtils::loadFilter ) )
      q.AssignValues( p, true );
  }
  return  result;
}

void
ConfigWindow::DisposeWidgets()
{
  for( WidgetContainer::iterator i = mWidgets.begin(); i != mWidgets.end(); ++i )
    delete *i;
  mWidgets.clear();
}

void
ConfigWindow::OnLoadParametersClick()
{
  QString fileName = QFileDialog::getOpenFileName(
    NULL, tr("Open Parameter File"), mpPreferences->mCurrentDir.canonicalPath(), tr("BCI2000 parameter file (*.prm);;Any file (*.*)") );
  if( !fileName.isEmpty() )
  {
    mpPreferences->mCurrentDir = QFileInfo( fileName ).canonicalPath();
    UpdateParameters();
    bool result = LoadParameters( fileName );
    if( !result )
    {
      QMessageBox::critical( this, tr("Error"), tr("Error reading parameter file") );
    }
    else
    {
      this->setWindowTitle( mOriginalTitle + tr(" - read %1").arg( fileName ) );
      RenderParameters( mCurTab );
    }
  }
}

void
ConfigWindow::OnConfigureSaveFilterClick()
{
  ShowParameters dialog( this, *mpParameters, OperatorUtils::saveFilter );
  dialog.exec();
}

void
ConfigWindow::OnConfigureLoadFilterClick()
{
  ShowParameters dialog( this, *mpParameters, OperatorUtils::loadFilter );
  dialog.exec();
}

void
ConfigWindow::OnHelpClick()
{
  QWhatsThis::enterWhatsThisMode();
}

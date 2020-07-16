#include "MainWindow.h"

#include "BCI2000FileReader.h"
#include "ExecutableHelp.h"
#include "AboutBox.h"
#include "Settings.h"
#include "BCIStream.h"
#include "BCIReader.h"
#include "Converters/ASCIIConverter.h"
#include "Converters/BrainVisionConverter.h"
#include "defines.h"
#include <QtGui>
#include <sstream>

using namespace std;

static struct
{
  const char* Name;
  BCIReader* (*CreateInstance)();
  BCIReader* ConverterInstance;
} sOutputFormats[] =
{
  { "BrainVisionAnalyzer",        BrainVisionGDRConverter::CreateInstance,     NULL },
  { "ASCII (standard precision)", ASCIIConverter::CreateInstance,              NULL },
  { "ASCII (high precision)",     ASCIIConverter::CreateInstanceHighPrecision, NULL },
};


MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent),
  mpImportStates( new KeyPressListWidget ),
  mpChannelNames( new QTextEdit ),
  mpDropArea( NULL ),
  mpOutputFormat( new QComboBox ),
  mpStatusLabel( NULL )
{
  QVBoxLayout* pLayout1 = new QVBoxLayout;
  pLayout1->addWidget( new QLabel( tr( "Channel Names" ) ) );
  pLayout1->addWidget( mpChannelNames );
  QFrame* pFrame1 = new QFrame;
  pFrame1->setLayout( pLayout1 );

  QVBoxLayout* pLayout2 = new QVBoxLayout;
  pLayout2->addWidget( new QLabel( tr( "Import States" ) ) );
  pLayout2->addWidget( mpImportStates );
  QFrame* pFrame2 = new QFrame;
  pFrame2->setLayout( pLayout2 );

  QVBoxLayout* pLayout3 = new QVBoxLayout;
  pLayout3->addWidget( new QLabel( tr( "Drop Area" ) ) );
  QFrame* pFrame4 = new QFrame;
  pLayout3->addWidget( pFrame4 );
  QFrame* pFrame3 = new QFrame;
  pFrame3->setLayout( pLayout3 );
  pFrame4->setFrameShadow( QFrame::Sunken );
  pFrame4->setFrameShape( QFrame::StyledPanel );
  pFrame4->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
  mpDropArea = new QLabel( tr( "Dropping files here will create converted files at the original files' location." ) );
  mpDropArea->setWordWrap( true );
  mpDropArea->setAlignment( Qt::AlignCenter | Qt::AlignHCenter );
  QVBoxLayout* pLayout4 = new QVBoxLayout;
  pLayout4->addWidget( mpDropArea );
  pFrame4->setLayout( pLayout4 );

  QSplitter* pSplitter = new QSplitter;
  pSplitter->setChildrenCollapsible( false );
  pSplitter->addWidget( pFrame1 );
  pSplitter->addWidget( pFrame2 );
  pSplitter->addWidget( pFrame3 );

  QFormLayout* pLayout5 = new QFormLayout;
  pLayout5->addRow( new QLabel( tr( "Output Format" ) ), mpOutputFormat );

  QFrame* pFrame5 = new QFrame;
  pFrame5->setFrameShadow( QFrame::Sunken );
  pFrame5->setFrameShape( QFrame::StyledPanel );
  QVBoxLayout* pLayout6 = new QVBoxLayout;
  pLayout6->addWidget( pSplitter );
  pLayout6->addLayout( pLayout5 );
  pLayout6->setStretch( 0, 1 );
  pLayout6->setStretch( 1, 0 );
  pFrame5->setLayout( pLayout6 );
  this->setCentralWidget( pFrame5 );

  mpImportStates->setToolTip( tr(
    "Drop BCI2000 files here to populate the list of import states.\n"
    "Delete selected items using the Delete key."
  ) );
  mpImportStates->setSelectionMode( QAbstractItemView::ExtendedSelection );
  mpChannelNames->setToolTip( tr(
    "Entering channel names in the usual notation allows\n"
    "creating topographic maps from the Analyzer program.\n"
    "If present, channel names from the BCI2000 file\n"
    "will override the ones defined here."
  ) );

  for( size_t i = 0; i < sizeof( sOutputFormats ) / sizeof( *sOutputFormats ); ++i )
  {
    mpOutputFormat->addItem( sOutputFormats[i].Name );
    sOutputFormats[ i ].ConverterInstance = sOutputFormats[ i ].CreateInstance();
  }
  mpOutputFormat->setCurrentIndex( 0 );

  QMenuBar* menuBar = new QMenuBar( 0 );
  QMenu* fileMenu = menuBar->addMenu( tr( "File" ) );
  fileMenu->addAction( tr( "Open..." ), this, SLOT(on_FileOpen()), Qt::CTRL | Qt::Key_O );
  fileMenu->addAction( tr( "Quit" ), this, SLOT(on_FileQuit()), Qt::CTRL | Qt::Key_Q );
  QMenu* editMenu = menuBar->addMenu( tr( "Edit" ) );
  editMenu->addAction( tr( "Cut" ), mpChannelNames, SLOT(cut()), Qt::CTRL | Qt::Key_X );
  editMenu->addAction( tr( "Copy" ), mpChannelNames, SLOT(copy()), Qt::CTRL | Qt::Key_C );
  editMenu->addAction( tr( "Paste" ), mpChannelNames, SLOT(paste()), Qt::CTRL | Qt::Key_V );
  QMenu* helpMenu = menuBar->addMenu( tr( "Help" ) );
  helpMenu->addAction( tr( "BCI2000 Help" ), this, SLOT(on_HelpBCI2000Help()) );
  helpMenu->addAction( tr( "About..." ), this, SLOT(on_HelpAbout()) );
  this->setMenuBar( menuBar );

  mpStatusLabel = new QLabel( tr( "Idle" ) );
  this->statusBar()->addPermanentWidget( mpStatusLabel, 1 );
  this->setAcceptDrops( true );
  this->setWindowTitle( QApplication::applicationName() );
  ReadSettings();

  if( QApplication::arguments().size() > 1 )
    DropArea_handleFiles( QApplication::arguments() );
}

MainWindow::~MainWindow()
{
  SaveSettings();
  for( size_t i = 0; i < sizeof( sOutputFormats ) / sizeof( *sOutputFormats ); ++i )
    delete sOutputFormats[ i ].ConverterInstance;
}

void
MainWindow::dragEnterEvent( QDragEnterEvent* inpEvent )
{ // Accept a single BCI2000 data file when dragged into one of
  // the ChannelNames, ImportStates, or DropArea widgets.
  QWidget* pChild = this->childAt( inpEvent->pos() );
  if( pChild == mpChannelNames->viewport()
      || pChild == mpImportStates->viewport()
      || pChild == mpDropArea || pChild == mpDropArea->parentWidget() )
  {
    QList<QUrl> urls = inpEvent->mimeData()->urls();
    if( urls.size() > 0 )
    {
      bool validFiles = true;
      foreach( QUrl url, urls )
      {
        QString fileName = url.toLocalFile();
        validFiles = validFiles && BCI2000FileReader( fileName.toLocal8Bit() ).IsOpen();
      }
      if( validFiles )
        inpEvent->acceptProposedAction();
    }
  }
}

void
MainWindow::dropEvent( QDropEvent* inpEvent )
{
  QWidget* pChild = this->childAt( inpEvent->pos() );
  QStringList files;
  foreach( QUrl url, inpEvent->mimeData()->urls() )
    files.append( url.toLocalFile() );
  if( !files.empty() )
  {
    if( pChild == mpChannelNames->viewport() )
      ChannelNames_handleFiles( files );
    else if( pChild == mpImportStates->viewport() )
      ImportStates_handleFiles( files );
    else if( pChild == mpDropArea || pChild == mpDropArea->parentWidget() )
      DropArea_handleFiles( files );
  }
}

void
MainWindow::ChannelNames_handleFiles( const QStringList& inFiles )
{
  BCI2000FileReader reader( inFiles.first().toLocal8Bit() );
  ParamRef ChannelNames = reader.Parameter( "ChannelNames" );
  if( ChannelNames != "" )
  {
    QString s;
    for( int i = 0; i < ChannelNames->NumValues(); ++i )
    {
      s += ChannelNames( i ).c_str();
      s += '\n';
    }
    mpChannelNames->setText( s );
  }
}

void
MainWindow::ImportStates_handleFiles( const QStringList& inFiles )
{
  foreach( QString file, inFiles )
  {
    BCI2000FileReader reader( file.toLocal8Bit() );
    if( reader.IsOpen() )
    {
      StringSet newStates;
      const StateList& states = *reader.States();
      for( const auto& state : states )
        newStates.insert( state.Name() );
      for( int j = 0; j < mpImportStates->count(); ++j )
        newStates.erase( mpImportStates->item( j )->text().toLocal8Bit().constData() );
      for( StringSet::const_iterator i = newStates.begin(); i != newStates.end(); ++i )
      {
        mpImportStates->insertItem( 0, i->c_str() );
        mpImportStates->item( 0 )->setCheckState( Qt::Checked );
      }
      mpImportStates->sortItems();
    }
  }
}

void
MainWindow::DropArea_handleFiles( const QStringList& inFiles )
{
  bcierr__.Clear();
  bciout__.Clear();
  DisableAll();
  mpStatusLabel->setText( tr( "Processing" ) );
  this->repaint();

  ImportStates_handleFiles( inFiles );

  StringSet statesToIgnore;
  for( int i = 0; i < mpImportStates->count(); ++i )
    if( mpImportStates->item( i )->checkState() != Qt::Checked )
      statesToIgnore.insert( mpImportStates->item( i )->text().toLocal8Bit().constData() );

  StringList channelNames;
  istringstream iss( mpChannelNames->document()->toPlainText().toLocal8Bit().constData() );
  string line;
  while( getline( iss, line ) )
    channelNames.push_back( line );

  int idx = mpOutputFormat->currentIndex();
  BCIReader& Converter = *sOutputFormats[ idx >= 0 ? idx : 0 ].ConverterInstance;
  int curFile = 0;
  foreach( QString file, inFiles )
  {
    ostringstream oss;
    oss << "Processing " << ++curFile << "/" << inFiles.size();
    mpStatusLabel->setText( oss.str().c_str() );
    mpStatusLabel->repaint();

    Converter.Open( file.toLocal8Bit() );
    if( bcierr__.Flushes() == 0 )
      Converter.Process( channelNames, statesToIgnore, false );
  }
  EnableAll();
  mpStatusLabel->setText( tr( "Idle" ) );
  this->statusBar()->clearMessage();
  this->repaint();
}

void
MainWindow::on_FileOpen()
{
  QStringList files = QFileDialog::getOpenFileNames( this,
    tr("Open Data Files"), QDir::currentPath(), tr("BCI2000 Data Files (*.dat)") );
  if( !files.empty() )
  {
    QDir::setCurrent( QFileInfo( files.first() ).canonicalPath() );
    DropArea_handleFiles( files );
  }
}

void
MainWindow::on_FileQuit()
{
  qApp->quit();
}

void
MainWindow::on_HelpBCI2000Help()
{
  ExecutableHelp().Display();
}

void
MainWindow::on_HelpAbout()
{
  AboutBox().SetApplicationName( QApplication::applicationName().toLocal8Bit().constData() )
            .Display();
}

void
MainWindow::EnableAll()
{
  foreach( QObject* pObject, this->children() )
  {
    QWidget* pWidget = dynamic_cast<QWidget*>( pObject );
    if( pWidget )
      pWidget->setEnabled( true );
  }
}

void
MainWindow::DisableAll()
{
  foreach( QObject* pObject, this->children() )
  {
    QWidget* pWidget = dynamic_cast<QWidget*>( pObject );
    if( pWidget )
      pWidget->setEnabled( false );
  }
  this->statusBar()->setEnabled( true );
  mpStatusLabel->setEnabled( true );
}

void
MainWindow::SaveSettings() const
{
  Settings settings;
  settings.beginGroup( KEY_CONFIG );
  settings.setValue( "Geometry", this->saveGeometry() );
  settings.setValue( "WorkingDirectory", QDir::currentPath() );
  settings.setValue( "ChannelNames", mpChannelNames->document()->toPlainText() );
  settings.setValue( "OutputFormat", mpOutputFormat->currentText() );

  settings.beginWriteArray( "States" );
  for( int i = 0; i < mpImportStates->count(); ++i )
  {
    settings.setArrayIndex( i );
    settings.setValue( "Name", mpImportStates->item( i )->text() );
    settings.setValue( "Checked", mpImportStates->item( i )->checkState() == Qt::Checked );
  }
  settings.endArray();

  settings.endGroup();
}

void
MainWindow::ReadSettings()
{
  Settings settings;
  settings.beginGroup( KEY_CONFIG );
  this->restoreGeometry( settings.value( "Geometry" ).toByteArray() );
  QDir::setCurrent( settings.value( "WorkingDirectory" ).toString() );
  mpChannelNames->setText( settings.value( "ChannelNames" ).toString() );

  for( int i = 0; i < mpOutputFormat->count(); ++i )
    if( mpOutputFormat->itemText( i ) == settings.value( "OutputFormat" ).toString() )
      mpOutputFormat->setCurrentIndex( i );

  int size = settings.beginReadArray( "States" );
  for( int i = 0; i < size; ++i )
  {
    settings.setArrayIndex( i );
    mpImportStates->insertItem( 0, settings.value( "Name" ).toString() );
    mpImportStates->item( 0 )->setCheckState( settings.value( "Checked" ).toBool() ? Qt::Checked : Qt::Unchecked );
  }
  settings.endArray();
  mpImportStates->sortItems();

  settings.endGroup();
}

void
MainWindow::KeyPressListWidget::keyPressEvent( QKeyEvent* inpEvent )
{
  switch( inpEvent->key() )
  {
    case Qt::Key_Delete:
    case Qt::Key_Clear:
      while( !this->selectedIndexes().empty() )
        delete this->takeItem( this->selectedIndexes().first().row() );
      break;
    default:
      QListWidget::keyPressEvent( inpEvent );
  }
}

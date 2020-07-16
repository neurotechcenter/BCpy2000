//////////////////////////////////////////////////////////////////////
// $Id: ParamDisplay.cpp 5550 2016-09-27 14:46:22Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A ParamDisplay class that handles a single parameter's
//       associated user interface, consisting of a group of user
//       interface widgets arranged inside a parent widget.
//       ParamDisplay is the interface to the outside world;
//       actual behavior is implemented by a class hierarchy
//       descending from a "DisplayBase" class.
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
#include "ParamDisplay.h"
#include "Param.h"
#include "Color.h"
#include "OperatorUtils.h"
#include "Preferences.h"
#include "EditMatrix.h"
#include "ExecutableHelp.h"
#include "ParsedComment.h"
#include "StringUtils.h"
#include "ProcessUtils.h"
#include "Thread.h"
#include "Blob.h"
#include "Debugging.h"
#include "BCIStream.h"

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QDialogButtonBox>
#include <QListWidget>
#include <QLayout>
#include <QMessageBox>
#include <QColorDialog>
#include <QDesktopServices>
#include <QHelpEvent>
#include <QWhatsThis>

#include <vector>
#include <string>
#include <sstream>

#define LOAD_MATRIX_FILTER tr("Matrix file (*.txt *." MATRIX_EXTENSION ")" \
                            ";;All files (*.*)")
#define SAVE_MATRIX_FILTER tr("BCI2000 matrix parameter (*." MATRIX_EXTENSION ")" \
                            ";;Tab delimited matrix file (*.txt)" \
                            ";;All files (*.*)")
#define ALLFILES_FILTER tr("All files (*.*)")

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// ParamDisplay definitions
////////////////////////////////////////////////////////////////////////////////
ParamDisplay::ParamDisplay()
: mpDisplay( NULL )
{
}

ParamDisplay::ParamDisplay( const Param& inParam, QWidget* inParent )
: mpDisplay( NULL )
{
  ParsedComment parsedComment( inParam );
  switch( parsedComment.Kind() & (ParsedComment::shapeMask | ParsedComment::typeMask) )
  {
  case ParsedComment::singleEntry | ParsedComment::enumeration:
    mpDisplay = new SingleEntryEnum( parsedComment, inParent );
    break;
  case ParsedComment::singleEntry | ParsedComment::boolean:
    mpDisplay = new SingleEntryBoolean( parsedComment, inParent );
    break;
  case ParsedComment::singleEntry | ParsedComment::inputfile:
    mpDisplay = new SingleEntryInputFile( parsedComment, inParent );
    break;
  case ParsedComment::singleEntry | ParsedComment::outputfile:
    mpDisplay = new SingleEntryOutputFile( parsedComment, inParent );
    break;
  case ParsedComment::singleEntry | ParsedComment::directory:
    mpDisplay = new SingleEntryDirectory( parsedComment, inParent );
    break;
  case ParsedComment::singleEntry | ParsedComment::color:
    mpDisplay = new SingleEntryColor( parsedComment, inParent );
    break;
  case ParsedComment::singleEntry | ParsedComment::blob:
    mpDisplay = new SingleEntryBlob( parsedComment, inParent );
    break;
  default:
    switch( parsedComment.Kind() & ParsedComment::shapeMask )
    {
    case ParsedComment::singleEntry:
      mpDisplay = new SingleEntryEdit( parsedComment, inParent );
      break;
    case ParsedComment::list:
      mpDisplay = new List( parsedComment, inParent );
      break;
    case ParsedComment::matrix:
      mpDisplay = new Matrix( parsedComment, inParent );
      break;
    default:
      Assert( false );
    }
  }
  ++mpDisplay->mRefCount;
}

ParamDisplay::ParamDisplay( const ParamDisplay& inOriginal )
: mpDisplay( inOriginal.mpDisplay )
{
  if( mpDisplay )
    ++mpDisplay->mRefCount;
}

const ParamDisplay&
ParamDisplay::operator=( const ParamDisplay& inOriginal )
{
  mpDisplay = inOriginal.mpDisplay;
  if( mpDisplay )
    ++mpDisplay->mRefCount;
  return *this;
}

ParamDisplay::~ParamDisplay()
{
  if( mpDisplay && --mpDisplay->mRefCount < 1 )
    delete mpDisplay;
}

void
ParamDisplay::SetTop( int top )
{
  if( mpDisplay )
    mpDisplay->SetTop( top );
}

void
ParamDisplay::SetLeft( int left )
{
  if( mpDisplay )
    mpDisplay->SetLeft( left );
}

int
ParamDisplay::GetBottom()
{
  return mpDisplay ? mpDisplay->GetBottom() : 0;
}

int
ParamDisplay::GetRight()
{
  return mpDisplay ? mpDisplay->GetRight() : 0;
}

void
ParamDisplay::Hide()
{
  if( mpDisplay )
    mpDisplay->Hide();
}

void
ParamDisplay::Show()
{
  if( mpDisplay )
    mpDisplay->Show();
}

void
ParamDisplay::WriteValuesTo( Param& p ) const
{
  if( mpDisplay )
    mpDisplay->WriteValuesTo( p );
}

void
ParamDisplay::ReadValuesFrom( const Param& p )
{
  if( mpDisplay )
    mpDisplay->ReadValuesFrom( p );
}

bool
ParamDisplay::Modified() const
{
  return mpDisplay && mpDisplay->Modified();
}

////////////////////////////////////////////////////////////////////////////////
// ParamLabel definitions
////////////////////////////////////////////////////////////////////////////////
ParamLabel::ParamLabel( QWidget* inParent, const ParsedComment& inParsedComment )
: QLabel( inParent ),
  mParamName( inParsedComment.Name() ),
  mHelpContext( inParsedComment.HelpContext() )
{
  setWhatsThis( "dummy" );
}

bool
ParamLabel::event( QEvent* inpEvent )
{
  bool result = false;
  switch( inpEvent->type() )
  {
    case QEvent::WhatsThis:
    {
      QWhatsThis::leaveWhatsThisMode();
      if( ExecutableHelp().ParamHelp().Exists( mParamName ) )
        ExecutableHelp().ParamHelp().Open( mParamName, mHelpContext );
      result = true;
    } break;
    default:
      result = QLabel::event( inpEvent );
      break;
  }
  return result;
}

////////////////////////////////////////////////////////////////////////////////
// DisplayBase definitions
////////////////////////////////////////////////////////////////////////////////
DisplayBase::DisplayBase( const ParsedComment& inParam, QWidget* inParent )
: mRefCount( 0 ),
  mModified( false ),
  mTop( 0 ),
  mLeft( 0 ),
  mpUserLevel( NULL )
{
  if( !*Measures )
    InitMeasures();
  // render the parameter's name
  ParamLabel* pLabel = new ParamLabel( inParent, inParam );
  pLabel->move( Measures[labelsOffsetX_], Measures[labelsOffsetY_] );
  pLabel->setText( QString::fromLocal8Bit( inParam.Name().c_str() ) );
  QFont font = pLabel->font();
  font.setWeight( QFont::Bold );
  pLabel->setFont( font );
  pLabel->setToolTip( QString::fromLocal8Bit( inParam.Comment().c_str() ) );
  AddWidget( pLabel );

  // render the parameter's User Level track bar
  // _only_ if the current user level is "advanced"
  if( OperatorUtils::UserLevel() == Preferences::Advanced )
  {
    mpUserLevel = new QSlider( inParent );
    mpUserLevel->setOrientation( Qt::Horizontal );
    mpUserLevel->move( Measures[userlevelOffsetX_], Measures[userlevelOffsetY_] );
    mpUserLevel->resize( Measures[userlevelWidth_], Measures[userlevelHeight_] );
    mpUserLevel->setRange( 1, 3 );
    mpUserLevel->setSingleStep( 1 );
    mpUserLevel->setPageStep( 1 );
    mpUserLevel->setTickInterval( 1 );
    mpUserLevel->setTickPosition( QSlider::TicksBelow );
    mpUserLevel->setTracking( false );
    connect( mpUserLevel, SIGNAL(valueChanged(int)), this, SLOT(OnContentChange()) );
    AddWidget( mpUserLevel );
  }
}

DisplayBase::~DisplayBase()
{
  for( WidgetIterator i = mWidgets.begin(); i != mWidgets.end(); ++i )
    delete *i;
}

void
DisplayBase::SetTop( int inTop )
{
  int delta = inTop - mTop;
  for( WidgetIterator i = mWidgets.begin(); i != mWidgets.end(); ++i )
    ( *i )->move( ( *i )->x(), ( *i )->y() + delta );
  mTop = inTop;
}

void
DisplayBase::SetLeft( int inLeft )
{
  int delta = inLeft - mLeft;
  for( WidgetIterator i = mWidgets.begin(); i != mWidgets.end(); ++i )
    ( *i )->move( ( *i )->x() + delta, ( *i )->y() );
  mLeft = inLeft;
}

int
DisplayBase::GetBottom()
{
  int bottom = 0;
  for( WidgetIterator i = mWidgets.begin(); i != mWidgets.end(); ++i )
    bottom = max( bottom, ( *i )->y() + ( *i )->size().height() );
  return bottom;
}

int
DisplayBase::GetRight()
{
  int right = 0;
  for( WidgetIterator i = mWidgets.begin(); i != mWidgets.end(); ++i )
    right = max( right, ( *i )->x() + ( *i )->size().width() );
  return right;
}

void
DisplayBase::Hide()
{
  for( WidgetIterator i = mWidgets.begin(); i != mWidgets.end(); ++i )
    ( *i )->hide();
}

void
DisplayBase::Show()
{
  for( WidgetIterator i = mWidgets.begin(); i != mWidgets.end(); ++i )
    ( *i )->show();
}

void
DisplayBase::WriteValuesTo( Param& inParam ) const
{
  if( mpUserLevel )
    OperatorUtils::SetUserLevel( inParam.Name().c_str(), mpUserLevel->value() );
}

void
DisplayBase::ReadValuesFrom( const Param& inParam )
{
  if( mpUserLevel )
    mpUserLevel->setValue( OperatorUtils::GetUserLevel( inParam.Name().c_str() ) );
  mModified = false;
}

int DisplayBase::Measures[DisplayBase::measuresCount] = { 0 };
static bool ignored = DisplayBase::InitMeasures();

bool
DisplayBase::InitMeasures( float inDpi )
{
  Measures[leftMargin_] =       30;
  Measures[topMargin_] =        50;

  Measures[labelsOffsetX_] =    0;
  Measures[labelsOffsetY_] =    18;
  Measures[commentOffsetX_] =   140;
  Measures[commentOffsetY_] =   0;
  Measures[valueOffsetX_] =     Measures[commentOffsetX_];
  Measures[valueOffsetY_] =     14;
  Measures[valueWidth_] =       220;
  Measures[valueHeight_] =      22;
  Measures[buttonWidth_] =      ( Measures[valueWidth_] - 20 ) / 3;
  Measures[buttonHeight_] =     Measures[valueHeight_];
  Measures[buttonSpacingX_] =   ( Measures[valueWidth_] - 3 * Measures[buttonWidth_] ) / 2;
  Measures[userlevelOffsetX_] = Measures[valueOffsetX_] + 260;
  Measures[userlevelWidth_] =   70;
  Measures[userlevelOffsetY_] = 17;
  Measures[userlevelHeight_] =  26;
  Measures[totalWidth_] =       Measures[userlevelOffsetX_] + Measures[userlevelWidth_];

  if( inDpi )
    for( size_t i = 0; i < sizeof( Measures ) / sizeof( *Measures ); ++i )
      Measures[i] *= inDpi / 96.0 /* design DPI */;

  return true;
}

////////////////////////////////////////////////////////////////////////////////
// SeparateComment definitions
////////////////////////////////////////////////////////////////////////////////
SeparateComment::SeparateComment( const ParsedComment& inParam, QWidget* inParent )
: DisplayBase( inParam, inParent )
{
  // render the parameter's comment
  QLabel* pComment = new QLabel( inParent );
  pComment->move( Measures[commentOffsetX_], Measures[commentOffsetY_] );
  pComment->setText( QString::fromLocal8Bit( inParam.Comment().c_str() ) );
  QFont font = pComment->font();
  font.setItalic( true );
  pComment->setFont( font );
  pComment->setToolTip( QString::fromLocal8Bit( inParam.Comment().c_str() ) );

  static const QString ellipsis = "...";
  while( pComment->text() != ellipsis
    && pComment->x() + pComment->minimumSizeHint().width() > Measures[userlevelOffsetX_] + Measures[userlevelWidth_] )
  {
    QString s = pComment->text();
    s.truncate( pComment->text().lastIndexOf( " " ) );
    pComment->setText( s + ellipsis );
  }
  pComment->setText( pComment->text() + ' ' );
  pComment->resize( pComment->minimumSizeHint() );
  AddWidget( pComment );
}

////////////////////////////////////////////////////////////////////////////////
// SingleEntryEdit definitions
////////////////////////////////////////////////////////////////////////////////
SingleEntryEdit::SingleEntryEdit( const ParsedComment& inParam, QWidget* inParent )
: SeparateComment( inParam, inParent ),
  mpEdit( NULL )
{
  mpEdit = new QLineEdit( inParent );
  mpEdit->move( Measures[valueOffsetX_], Measures[valueOffsetY_] );
  mpEdit->resize( Measures[valueWidth_], Measures[valueHeight_] );
  mpEdit->setReadOnly( inParam.Kind() & ParsedComment::noedit );
  mpEdit->setToolTip( "" );
  connect( mpEdit, SIGNAL(textChanged(QString)), this, SLOT(OnEditChange()) );
  AddWidget( mpEdit );
}

void
SingleEntryEdit::WriteValuesTo( Param& outParam ) const
{
  outParam.Value() = mpEdit->text().toLocal8Bit();
  DisplayBase::WriteValuesTo( outParam );
}

void
SingleEntryEdit::ReadValuesFrom( const Param& inParam )
{
  mpEdit->setText( QString::fromLocal8Bit( inParam.Value().c_str() ) );
  DisplayBase::ReadValuesFrom( inParam );
}

void
SingleEntryEdit::OnEditChange()
{
  mpEdit->setToolTip( mpEdit->text() );
  DisplayBase::OnContentChange();
}

////////////////////////////////////////////////////////////////////////////////
// List definitions
////////////////////////////////////////////////////////////////////////////////
List::List( const ParsedComment& inParam, QWidget* inParent )
: SingleEntryButton( inParam, inParent ), mName( inParam.Name() ), mKind( inParam.Kind() )
{
}

void
List::ButtonClick()
{
  Param param( mName, "", "list" );
  TextToParam( param );
  EditMatrix editor;
  editor.SetColumnKind( 0, mKind );
  editor.SetDisplayedParam( &param, mKind );
  editor.exec();
  ParamToText( param );
}

void
List::ParamToText( const Param& inParam )
{
  ostringstream oss;
  if( inParam.NumValues() > 0 )
  {
    oss << EncodedString( inParam.Value( 0 ) );
    for( int i = 1; i < inParam.NumValues(); ++i )
      oss << ' ' << EncodedString( inParam.Value( i ) );
  }
  mpEdit->setText( QString::fromLocal8Bit( oss.str().c_str() ) );
}

void
List::TextToParam( Param& outParam ) const
{
  istringstream is(  mpEdit->text().toLocal8Bit().constData() );
  EncodedString value;
  int index = 0;
  outParam.SetNumValues( 0 );
  while( is >> value )
  {
    outParam.SetNumValues( index + 1 );
    outParam.Value( index++ ) = value;
  }
}

void
List::WriteValuesTo( Param& outParam ) const
{
  if( !( mKind & ParsedComment::accessMask ) )
  {
    TextToParam( outParam );
    DisplayBase::WriteValuesTo( outParam );
  }
}

void
List::ReadValuesFrom( const Param& inParam )
{
  ParamToText( inParam );
  DisplayBase::ReadValuesFrom( inParam );
}

////////////////////////////////////////////////////////////////////////////////
// Matrix definitions
////////////////////////////////////////////////////////////////////////////////
Matrix::Matrix( const ParsedComment& inParam, QWidget* inParent )
: SeparateComment( inParam, inParent ),
  mKind( inParam.Kind() ),
  mMatrixWindowOpen( false )
{
  bool noedit = (inParam.Kind() & ParsedComment::accessMask);
  QPushButton* pEditButton = new QPushButton( inParent );
  pEditButton->setText( noedit ? "View..." : "Edit..." );
  pEditButton->move( Measures[valueOffsetX_], Measures[valueOffsetY_] );
  pEditButton->resize( Measures[buttonWidth_], Measures[buttonHeight_] );
  connect( pEditButton, SIGNAL(clicked()), this, SLOT(OnEditButtonClick()) );
  AddWidget( pEditButton );

  QPushButton* pLoadButton = new QPushButton( inParent );
  pLoadButton->setText( tr("Load...") );
  pLoadButton->setEnabled( !noedit );
  pLoadButton->move( Measures[valueOffsetX_] + Measures[buttonWidth_] + Measures[buttonSpacingX_], Measures[valueOffsetY_] );
  pLoadButton->resize( Measures[buttonWidth_], Measures[buttonHeight_] );
  connect( pLoadButton, SIGNAL(clicked()), this, SLOT(OnLoadButtonClick()) );
  AddWidget( pLoadButton );

  QPushButton* pSaveButton = new QPushButton( inParent );
  pSaveButton->setText( tr("Save...") );
  pSaveButton->move( Measures[valueOffsetX_] + 2 * ( Measures[buttonWidth_] + Measures[buttonSpacingX_] ), Measures[valueOffsetY_] );
  pSaveButton->resize( Measures[buttonWidth_], Measures[buttonHeight_] );
  connect( pSaveButton, SIGNAL(clicked()), this, SLOT(OnSaveButtonClick()) );
  AddWidget( pSaveButton );
}

void
Matrix::WriteValuesTo( Param& outParam ) const
{
  outParam = mParam;
  DisplayBase::WriteValuesTo( outParam );
}

void
Matrix::ReadValuesFrom( const Param& inParam )
{
  mParam = inParam;
  if( mMatrixWindowOpen )
    gpEditMatrix->SetDisplayedParam( &mParam, mKind );
  DisplayBase::ReadValuesFrom( inParam );
}

void
Matrix::OnEditButtonClick()
{
  mMatrixWindowOpen = true;
  if( gpEditMatrix == NULL )
    gpEditMatrix = new EditMatrix;
  gpEditMatrix->SetDisplayedParam( &mParam, mKind );
  gpEditMatrix->exec();
  mMatrixWindowOpen = false;
  DisplayBase::OnContentChange();
}

void
Matrix::OnLoadButtonClick()
{
  QDir& curDir = OperatorUtils::CurrentDir();
  QString fileName = QFileDialog::getOpenFileName( NULL, tr("Open Matrix File"), curDir.canonicalPath(), LOAD_MATRIX_FILTER );
  if( !fileName.isEmpty() )
  {
    curDir = QFileInfo( fileName ).canonicalPath();
    int result = OperatorUtils::LoadMatrix( fileName, mParam );
    switch( result )
    {
      case OperatorUtils::NoError:
        break;
      case OperatorUtils::MatLoadColsDiff:
        QMessageBox::critical( NULL, tr("Error"), tr("Number of columns differs across rows.") );
        break;
      case OperatorUtils::MatNotFound:
        QMessageBox::critical( NULL, tr("Error"), tr("Could not open \"%1\" as a matrix data file.") );
        break;
      default:
        QMessageBox::critical( NULL, tr("Error"), tr("Error loading \"%1\" as a matrix file.") );
    }
    DisplayBase::OnContentChange();
  }
}

void
Matrix::OnSaveButtonClick()
{
  QDir& curDir = OperatorUtils::CurrentDir();
  QString fileName = QFileDialog::getSaveFileName( NULL, tr("Save Matrix File"), curDir.canonicalPath(), SAVE_MATRIX_FILTER );
  if( !fileName.isEmpty() )
  {
    curDir = QFileInfo( fileName ).canonicalPath();
    int result = OperatorUtils::SaveMatrix( fileName, mParam );
    switch( result )
    {
      case OperatorUtils::NoError:
        break;
      case OperatorUtils::CannotWriteNestedMatrixAsText:
        QMessageBox::critical( NULL, tr("Error"), tr(
            "You are saving a \"nested\" matrix. Nested matrices cannot be written in text format.\n\n"
            "Please choose \"BCI2000 matrix parameter\" as a file type.\n"
            ) );
        break;
      case OperatorUtils::CouldNotWrite:
      default:
        QMessageBox::critical( NULL, tr("Error"), tr("Could not write to file \"%1\".").arg( fileName ) );
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
// SingleEntryBlob definitions
////////////////////////////////////////////////////////////////////////////////
SingleEntryBlob::SingleEntryBlob( const ParsedComment& inParam, QWidget* inParent )
: SeparateComment( inParam, inParent ),
  mpData( 0 ), mpParam( 0 ), mKind( inParam.Kind() )
{
  QPushButton* p = new QPushButton( inParent );
  p->setText( "View..." );
  p->move( Measures[valueOffsetX_], Measures[valueOffsetY_] );
  p->resize( Measures[buttonWidth_], Measures[buttonHeight_] );
  connect( p, SIGNAL(clicked()), this, SLOT(OnViewButtonClick()) );
  AddWidget( p );
  mpViewButton = p;

  p = new QPushButton( inParent );
  p->setText( "Load..." );
  p->setEnabled( (inParam.Kind() & ParsedComment::accessMask) == 0 );
  p->move( Measures[valueOffsetX_] + Measures[buttonWidth_] + Measures[buttonSpacingX_], Measures[valueOffsetY_] );
  p->resize( Measures[buttonWidth_], Measures[buttonHeight_] );
  connect( p, SIGNAL(clicked()), this, SLOT(OnLoadButtonClick()) );
  AddWidget( p );
  mpLoadButton = p;

  p = new QPushButton( inParent );
  p->setText( "Save..." );
  p->move( Measures[valueOffsetX_] + 2 * ( Measures[buttonWidth_] + Measures[buttonSpacingX_] ), Measures[valueOffsetY_] );
  p->resize( Measures[buttonWidth_], Measures[buttonHeight_] );
  connect( p, SIGNAL(clicked()), this, SLOT(OnSaveButtonClick()) );
  AddWidget( p );
  mpSaveButton = p;
}

SingleEntryBlob::~SingleEntryBlob()
{
  delete mpData;
}

void
SingleEntryBlob::WriteValuesTo( Param& outParam ) const
{
  if( (mKind & ParsedComment::noedit) == 0 )
  {
    if( mpData )
    {
      ostringstream oss;
      mpData->InsertInto( oss );
      outParam.Value() = oss.str();
    }
    bool notEmpty = *outParam.Value().c_str();
    mpViewButton->setEnabled( notEmpty );
    mpSaveButton->setEnabled( notEmpty );
    DisplayBase::WriteValuesTo( outParam );
  }
}

void
SingleEntryBlob::ReadValuesFrom( const Param& inParam )
{
  mpParam = &inParam;
  delete mpData;
  mpData = 0;
  bool notEmpty = *inParam.Value().c_str();
  mpViewButton->setEnabled( notEmpty );
  mpSaveButton->setEnabled( notEmpty );
  DisplayBase::ReadValuesFrom( inParam );
}

void
SingleEntryBlob::ReadData()
{
  Assert( mpParam );
  if( !mpData )
  {
    mpData = new Blob;
    istringstream iss( mpParam->Value().AsString() + " " );
    mpData->ExtractFrom( iss );
  }
}

void
SingleEntryBlob::OnViewButtonClick()
{
  ReadData();
  if( !mpData->SaveAsTemp() )
    bcierr__ << "Could not write temporary file";
  else
  {
    string file = FileUtils::NormalizedPath( mpData->FileName() );
    if( !QDesktopServices::openUrl( QUrl( ( "file:///" + file ).c_str(), QUrl::TolerantMode ) ) )
      bcierr__ << "Could not open file \"" << file << "\" in viewer, check OS configuration";
  }
}

void
SingleEntryBlob::OnLoadButtonClick()
{
  QDir& curDir = OperatorUtils::CurrentDir();
  QString fileName = QFileDialog::getOpenFileName( 0, "Open file", curDir.canonicalPath() );
  if( !fileName.isEmpty() )
  {
    curDir = QFileInfo( fileName ).canonicalPath();
    delete mpData;
    mpData = new Blob( fileName.toLocal8Bit().constData() );
    DisplayBase::OnContentChange();
  }
}

void
SingleEntryBlob::OnSaveButtonClick()
{
  QDir& curDir = OperatorUtils::CurrentDir();
  QString fileName = QFileDialog::getSaveFileName( 0, "Save as:", curDir.canonicalPath() );
  if( !fileName.isEmpty() )
  {
    curDir = QFileInfo( fileName ).canonicalPath();
    ReadData();
    if( !mpData->SaveAs( fileName.toLocal8Bit().constData() ) )
      QMessageBox::critical( 0, "Error", tr("Could not write to file \"%1\".").arg( fileName ) );
  }
}

////////////////////////////////////////////////////////////////////////////////
// SingleEntryButton definitions
////////////////////////////////////////////////////////////////////////////////
SingleEntryButton::SingleEntryButton( const ParsedComment& inParam, QWidget* inParent )
: SingleEntryEdit( inParam, inParent )
{
  QPushButton* pButton = new QPushButton( inParent );
  pButton->setText( "..." );
  pButton->setEnabled( (inParam.Kind() & ParsedComment::accessMask) == 0 );
  pButton->move( Measures[valueOffsetX_] + Measures[valueWidth_] + Measures[buttonHeight_] / 2, Measures[valueOffsetY_] );
  pButton->resize( Measures[buttonHeight_], Measures[buttonHeight_] );
  connect( pButton, SIGNAL(clicked()), this, SLOT(OnButtonClick()) );
  AddWidget( pButton );
  mComment = inParam.Comment();
}

void
SingleEntryButton::OnButtonClick()
{
  ButtonClick();
}

////////////////////////////////////////////////////////////////////////////////
// SingleEntryInputFile definitions
////////////////////////////////////////////////////////////////////////////////
SingleEntryInputFile::SingleEntryInputFile( const ParsedComment& inParam, QWidget* inParent )
: SingleEntryButton( inParam, inParent )
{
}

void
SingleEntryInputFile::ButtonClick()
{
  QString fileName = QFileDialog::getOpenFileName(
    mpEdit->parentWidget(),
    tr( "Choosing %1" ).arg( QString::fromLocal8Bit( mComment.c_str() ) ),
    OperatorUtils::AppRelativeToAbsolutePath( mpEdit->text() ).filePath(),
    ALLFILES_FILTER );
  if( !fileName.isEmpty() )
    mpEdit->setText( fileName );
}

////////////////////////////////////////////////////////////////////////////////
// SingleEntryOutputFile definitions
////////////////////////////////////////////////////////////////////////////////
SingleEntryOutputFile::SingleEntryOutputFile( const ParsedComment& inParam,
                                                            QWidget* inParent )
: SingleEntryButton( inParam, inParent )
{
}

void
SingleEntryOutputFile::ButtonClick()
{
  QString fileName = QFileDialog::getSaveFileName( 
    mpEdit->parentWidget(),
    tr( "Choosing %1" ).arg( QString::fromLocal8Bit( mComment.c_str() ) ),
    OperatorUtils::AppRelativeToAbsolutePath( mpEdit->text() ).filePath(),
    ALLFILES_FILTER );
  if( !fileName.isEmpty() )
    mpEdit->setText( fileName );
}

////////////////////////////////////////////////////////////////////////////////
// SingleEntryDirectory definitions
////////////////////////////////////////////////////////////////////////////////
SingleEntryDirectory::SingleEntryDirectory( const ParsedComment& inParam,
                                                          QWidget* inParent )
: SingleEntryButton( inParam, inParent )
{
}

void
SingleEntryDirectory::ButtonClick()
{
  QString dirName = QFileDialog::getExistingDirectory( 
    mpEdit->parentWidget(),
    tr( "Choosing %1" ).arg( QString::fromLocal8Bit( mComment.c_str() ) ),
    OperatorUtils::AppRelativeToAbsolutePath( mpEdit->text() ).filePath() 
  );
  if( !dirName.isEmpty() )
    mpEdit->setText( dirName );
}

////////////////////////////////////////////////////////////////////////////////
// SingleEntryColor definitions
////////////////////////////////////////////////////////////////////////////////
SingleEntryColor::SingleEntryColor( const ParsedComment& inParam,
                                                  QWidget* inParent )
: SingleEntryButton( inParam, inParent )
{
}

void
SingleEntryColor::ButtonClick()
{
  RGBColor color;
  istringstream iss( mpEdit->text().toLocal8Bit().constData() );
  if( !( iss >> color ) )
    color = RGBColor::Black;
  QColor c = QRgb( color );
  c = QColorDialog::getColor( c, NULL, tr( "Choosing %1" ).arg( QString::fromLocal8Bit( mComment.c_str() ) ) );
  if( c.isValid() )
  {
    color = c.rgb();
    ostringstream oss;
    oss << color;
    mpEdit->setText( QString::fromLocal8Bit( oss.str().c_str() ) );
  }
}
////////////////////////////////////////////////////////////////////////////////
// SingleEntryEnum definitions
////////////////////////////////////////////////////////////////////////////////
SingleEntryEnum::SingleEntryEnum( const ParsedComment& inParam,
                                                QWidget* inParent )
: SeparateComment( inParam, inParent ),
  mpComboBox( NULL ),
  mIndexBase( inParam.IndexBase() )
{
  mpComboBox = new QComboBox( inParent );
  mpComboBox->move( Measures[valueOffsetX_], Measures[valueOffsetY_] );
  mpComboBox->resize( Measures[valueWidth_], Measures[valueHeight_] );
  for( size_t i = 0; i < inParam.Values().size(); ++i )
    mpComboBox->addItem( QString::fromLocal8Bit( inParam.Values()[ i ].c_str() ) );
  mpComboBox->setToolTip( QString::fromLocal8Bit( inParam.Comment().c_str() ) );
  mpComboBox->setEnabled( !( inParam.Kind() & ParsedComment::accessMask ) );
  connect( mpComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(OnContentChange()) );
  AddWidget( mpComboBox );
}

void
SingleEntryEnum::WriteValuesTo( Param& outParam ) const
{
  if( mpComboBox->currentText() == "auto" )
    outParam.Value() = "auto";
  else
    outParam.Value() = String() << mpComboBox->currentIndex() + mIndexBase;
  SeparateComment::WriteValuesTo( outParam );
}

void
SingleEntryEnum::ReadValuesFrom( const Param& inParam )
{
  mpComboBox->setCurrentIndex( ::atoi( inParam.Value().c_str() ) - mIndexBase );
  SeparateComment::ReadValuesFrom( inParam );
}

////////////////////////////////////////////////////////////////////////////////
// SingleEntryBoolean definitions
////////////////////////////////////////////////////////////////////////////////
SingleEntryBoolean::SingleEntryBoolean( const ParsedComment& inParam,
                                                      QWidget* inParent )
: DisplayBase( inParam, inParent ),
  mpCheckBox( NULL )
{
  mpCheckBox = new QCheckBox( inParent );
  mpCheckBox->move( Measures[valueOffsetX_], Measures[valueOffsetY_] );
  mpCheckBox->resize( Measures[valueWidth_], Measures[valueHeight_] );
  mpCheckBox->setToolTip( QString::fromLocal8Bit( inParam.Comment().c_str() ) );
  mpCheckBox->setText( QString::fromLocal8Bit( inParam.Comment().c_str() ) );
  mpCheckBox->setEnabled( !( inParam.Kind() & ParsedComment::accessMask) );
  connect( mpCheckBox, SIGNAL(stateChanged(int)), this, SLOT(OnContentChange()) );
  AddWidget( mpCheckBox );
}

void
SingleEntryBoolean::WriteValuesTo( Param& outParam ) const
{
  outParam.Value() = ( mpCheckBox->isChecked() ? "1" : "0" );
  DisplayBase::WriteValuesTo( outParam );
}

void
SingleEntryBoolean::ReadValuesFrom( const Param& inParam )
{
  mpCheckBox->setChecked( ::atoi( inParam.Value().c_str() ) );
  DisplayBase::ReadValuesFrom( inParam );
}



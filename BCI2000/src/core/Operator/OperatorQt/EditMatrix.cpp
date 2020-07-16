//////////////////////////////////////////////////////////////////////
// $Id: EditMatrix.cpp 5595 2017-01-14 15:37:20Z mellinger $
// Authors: schalk@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: An editor for matrix-valued parameters.
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
#include "EditMatrix.h"
#include "ui_EditMatrix.h"

#include "Param.h"
#include "OperatorUtils.h"
#include "ParsedComment.h"
#include "ExecutableHelp.h"
#include "BCIStream.h"

#include <QtWidgets>

using namespace std;

EditMatrix *gpEditMatrix = NULL;

int EditMatrix::sNumInstances = 0;

EditMatrix::EditMatrix( QWidget* parent )
: QDialog( parent ),
  m_ui( new Ui::EditMatrix ),
  mLock( QMutex::Recursive ),
  mpSubEditor( NULL ),
  mContextRow( -1 ),
  mContextCol( -1 ),
  mMode( Matrix ),
  mEditable( false ),
  mpContextMenu( 0 )
{
  m_ui->setupUi(this);

  QDialogButtonBox* pButtonBox = new QDialogButtonBox( QDialogButtonBox::Close );
  this->layout()->addWidget( pButtonBox );
  connect( pButtonBox, SIGNAL(rejected()), this, SLOT(accept()) );

  connect( m_ui->pushButton, SIGNAL(clicked()), this, SLOT(OnChangeMatrixSizeClick()) );
  connect( this, SIGNAL(finished(int)), this, SLOT(OnClose()) );

  // In Qt 4.5.3, the spin box maximum appears not to be read from the ui file, so we need
  // to set it here.
  m_ui->rowsSpinBox->setMaximum( cSpinBoxMax );
  m_ui->colsSpinBox->setMaximum( cSpinBoxMax );

#if QT_IS4
  m_ui->tableWidget->verticalHeader()->setClickable( false );
  m_ui->tableWidget->horizontalHeader()->setClickable( false );
#else
  m_ui->tableWidget->verticalHeader()->setSectionsClickable( false );
  m_ui->tableWidget->horizontalHeader()->setSectionsClickable( false );
#endif
  connect( m_ui->tableWidget->verticalHeader(), SIGNAL(sectionDoubleClicked(int)), this, SLOT(EditVLabel(int)) );
  connect( m_ui->tableWidget->horizontalHeader(), SIGNAL(sectionDoubleClicked(int)), this, SLOT(EditHLabel(int)) );
  connect( m_ui->tableWidget, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), this, SLOT(EditItem(QTableWidgetItem*)) );

  ++sNumInstances;
  if( sNumInstances == 1 )
    OperatorUtils::RestoreWidget( this );
  BuildContextMenu();
  m_ui->tableWidget->setToolTip( tr("Right-click cells for a context menu.") );
}

EditMatrix::~EditMatrix()
{
  if( sNumInstances == 1 )
    OperatorUtils::SaveWidget( this );
  delete m_ui;
  --sNumInstances;
}

void
EditMatrix::SetDisplayedParam( Param* inParam, int inKind )
{
  Lock();
  mpMatrixParam = inParam;
  mMode = inKind & ParsedComment::shapeMask;
  mEditable = !(inKind & ParsedComment::noedit);
  mMatrixParamComment = QString::fromLocal8Bit( ParsedComment( *inParam ).Comment().c_str() );
  mMatrixParamName = QString::fromLocal8Bit( mpMatrixParam->Name().c_str() );
  m_ui->rowsSpinBox->setValue( mpMatrixParam->NumRows() );
  m_ui->colsSpinBox->setValue( mpMatrixParam->NumColumns() );

  switch( mMode )
  {
  case List:
    m_ui->colsSpinBox->setVisible( false );
    m_ui->label_2->setVisible( false );
    m_ui->label_3->setText( "# of entries" );
    m_ui->pushButton->setText( "Set new list size" );
    break;
  case Matrix:
  default:
    m_ui->colsSpinBox->setVisible( true );
    m_ui->label_2->setText( "# of columns" );
    m_ui->label_3->setText( "# of rows" );
    m_ui->pushButton->setText( "Set new matrix size" );
  }
  m_ui->colsSpinBox->setEnabled( mEditable );
  m_ui->rowsSpinBox->setEnabled( mEditable );
  m_ui->pushButton->setEnabled( mEditable );

  UpdateDisplay();
  Unlock();
}

QString
EditMatrix::GetDisplayedParamName() const
{
  return mMatrixParamName;
}

void
EditMatrix::SetColumnKind( int inCol, int inKind )
{
  if( inCol >= mColumnKinds.size() )
    mColumnKinds.resize( inCol + 1 );
  mColumnKinds[inCol] = inKind;
}

void
EditMatrix::UpdateDisplay()
{
  Lock();
  if( mpSubEditor != NULL )
    mpSubEditor->close();

  // set the window title and comment
  const char* obj = "Matrix editor";
  if( mMode == List )
    obj = "List editor";
  string title = string( obj ) + ": \"" + mpMatrixParam->Name() + "\"";
  this->setWindowTitle( QString::fromLocal8Bit( title.c_str() ) );
  m_ui->label->setText( mMatrixParamComment );
  m_ui->label->setToolTip( mMatrixParamComment );

  // given the pointer, write the parameter's data into the spread sheet
  QTableWidget* t = m_ui->tableWidget;
  t->setRowCount( m_ui->rowsSpinBox->value() );
  t->setColumnCount( m_ui->colsSpinBox->value() );
  t->horizontalHeader()->setVisible( mMode == Matrix );

  int col = 0;
  while( col < mpMatrixParam->NumColumns() )
  {
    QTableWidgetItem* item = new QTableWidgetItem;
    item->setText( QString::fromLocal8Bit( mpMatrixParam->ColumnLabels()[ col ].c_str() ) );
    t->setHorizontalHeaderItem( col, item );
    ++col;
  }
  while( col < t->columnCount() )
  {
    QTableWidgetItem* item = new QTableWidgetItem;
    item->setText( QString::fromLocal8Bit( LabelIndex::TrivialLabel( col ).c_str() ) );
    t->setHorizontalHeaderItem( col, item );
    ++col;
  }
  int row = 0;
  while( row < mpMatrixParam->NumRows() )
  {
    QTableWidgetItem* item = new QTableWidgetItem;
    item->setText( QString::fromLocal8Bit( mpMatrixParam->RowLabels()[ row ].c_str() ) );
    t->setVerticalHeaderItem( row, item );
    ++row;
  }
  while( row < t->rowCount() )
  {
    QTableWidgetItem* item = new QTableWidgetItem;
    item->setText( QString::fromLocal8Bit( LabelIndex::TrivialLabel( row ).c_str() ) );
    t->setVerticalHeaderItem( row, item );
    ++row;
  }

  // set the values in the spreadsheet
  for( int row = 0; row < mpMatrixParam->NumRows(); ++row )
    for( int col = 0; col < mpMatrixParam->NumColumns(); ++col )
    {
      QBrush subMatrixBrush( Qt::lightGray );
      QTableWidgetItem* item = new QTableWidgetItem;
      if( mpMatrixParam->Value( row, col ).Kind() == Param::ParamValue::Matrix )
        SetSubmatrixProperties( item, Matrix );
      else if(  mpMatrixParam->Value( row, col ).Kind() == Param::ParamValue::List )
        SetSubmatrixProperties( item, List );
      else
      {
        item->setText( QString::fromLocal8Bit( mpMatrixParam->Value( row, col ).c_str() ) );
        SetNormalEntryProperties( item, row, col );
      }
      t->setItem( row, col, item );
    }

  AdaptColumnWidths();
  m_ui->tableWidget->verticalHeader()->resizeSections( QHeaderView::ResizeToContents );
  m_ui->tableWidget->clearSelection();

  Unlock();
}

void
EditMatrix::UpdateParam()
{
  Lock();

  QTableWidget* t = m_ui->tableWidget;
  // set column and row labels
  for( int col = 0; col < mpMatrixParam->NumColumns(); ++col )
    mpMatrixParam->ColumnLabels()[ col ] = t->horizontalHeaderItem( col )->text().toLocal8Bit().constData();
  for( int row = 0; row < mpMatrixParam->NumRows(); ++row )
    mpMatrixParam->RowLabels()[ row ] = t->verticalHeaderItem( row )->text().toLocal8Bit().constData();

  // set the values in the parameter according to the values in the spreadsheet
  for( int row = 0; row < mpMatrixParam->NumRows(); ++row )
    for( int col = 0; col < mpMatrixParam->NumColumns(); ++col )
      if( !IsSubmatrix( t->item( row, col ) ) )
        mpMatrixParam->Value( row, col ) = t->item( row, col )->text().toLocal8Bit().constData();

  Unlock();
}

void
EditMatrix::EditHLabel( int inIdx )
{
  if( inIdx < 0 )
    return;

  QTableWidget* t = m_ui->tableWidget;
  QString text = HeaderEditor( t->horizontalHeaderItem( inIdx )->text() );
  if( !text.isEmpty() )
  {
    t->horizontalHeaderItem( inIdx )->setText( text );
    AdaptColumnWidths();
  }
}

void
EditMatrix::EditVLabel( int inIdx )
{
  if( inIdx < 0 )
    return;

  QTableWidget* t = m_ui->tableWidget;
  QString text = HeaderEditor( t->verticalHeaderItem( inIdx )->text() );
  if( !text.isEmpty() )
  {
    t->verticalHeaderItem( inIdx )->setText( text );
    t->verticalHeader()->resizeSections( QHeaderView::ResizeToContents );
    AdaptColumnWidths();
  }
}

QString
EditMatrix::HeaderEditor( const QString& inText )
{
  QString result = "";
  QDialog window( this );
  window.setWindowTitle( mEditable ? "Edit Header" : "View Header" );
  QLineEdit* pLineEdit = new QLineEdit( inText );
  pLineEdit->setReadOnly( !mEditable );
  QDialogButtonBox* pButtonBox = new QDialogButtonBox( mEditable ? QDialogButtonBox::Ok | QDialogButtonBox::Cancel : QDialogButtonBox::Ok );
  QVBoxLayout* pMainLayout = new QVBoxLayout;
  pMainLayout->addWidget( pLineEdit );
  pMainLayout->addWidget( pButtonBox );
  window.setLayout( pMainLayout );
  connect( pButtonBox, SIGNAL(accepted()), &window, SLOT(accept()) );
  connect( pButtonBox, SIGNAL(rejected()), &window, SLOT(reject()) );
  if( QDialog::Accepted == window.exec() && mEditable )
    result = pLineEdit->text();
  return result;
}

void
EditMatrix::AdaptColumnWidths()
{
  QTableWidget* t = m_ui->tableWidget;
  if( t->horizontalHeader()->count() > 0 )
    t->horizontalHeader()->resizeSection( 0, 0 );
  t->horizontalHeader()->resizeSections( QHeaderView::ResizeToContents );
}

void
EditMatrix::EditItem( QTableWidgetItem* inItem )
{
  if( IsSubmatrix( inItem ) )
  {
    int row = inItem->row(),
        col = inItem->column();
    mpSubEditor = new EditMatrix( this );
    mpSubEditor->move( this->x() + 20, this->y() + 20 );
    mpSubEditor->SetDisplayedParam( mpMatrixParam->Value( row, col ), mEditable ? mMode : mMode | ParsedComment::noedit );
    mpSubEditor->setWindowTitle( this->windowTitle() + "("
                           + QString::fromLocal8Bit( mpMatrixParam->RowLabels()[ row ].c_str() ) + ","
                           + QString::fromLocal8Bit( mpMatrixParam->ColumnLabels()[ col ].c_str() ) + ")"
                           );
    mpSubEditor->m_ui->label->setText( mMatrixParamComment );
    mpSubEditor->exec();
    delete mpSubEditor;
    mpSubEditor = NULL;
  }
}

void
EditMatrix::EditSubMatrix( int inRow, int inCol )
{
  EditItem( m_ui->tableWidget->item( inRow, inCol ) );
}


void
EditMatrix::PlainCellToMatrix( int inRow, int inCol )
{
  if( QMessageBox::Ok == QMessageBox::question( this, tr("Confirmation"),
    tr("You are about to change this cell into a 1x1-sub-matrix.\n"
       "Proceed?"),
      QMessageBox::Ok | QMessageBox::Cancel,
      QMessageBox::Cancel ) )
  {
    QTableWidgetItem* item = m_ui->tableWidget->item( inRow, inCol );
    Param p = Param::fromDefinition( "{ matrix 1 1 0 }" );
    p.Value( 0, 0 ) = item->text().toLocal8Bit().constData();
    mpMatrixParam->Value( inRow, inCol ) = p;
    SetSubmatrixProperties( item, Matrix );
    AdaptColumnWidths();
  }
}

void
EditMatrix::PlainCellToList( int inRow, int inCol )
{
  if( QMessageBox::Ok == QMessageBox::question( this, tr("Confirmation"),
    tr("You are about to change this cell into a list.\n"
       "Proceed?"),
      QMessageBox::Ok | QMessageBox::Cancel,
      QMessageBox::Cancel ) )
  {
    QTableWidgetItem* item = m_ui->tableWidget->item( inRow, inCol );
    Param p = Param::fromDefinition( "{ list 1 0 }" );
    p.Value( 0, 0 ) = item->text().toLocal8Bit().constData();
    mpMatrixParam->Value( inRow, inCol ) = p;
    SetSubmatrixProperties( item, List );
    AdaptColumnWidths();
  }
}

void
EditMatrix::SetSubmatrixProperties( QTableWidgetItem* inpItem, int inKind )
{
  inpItem->setFlags( inpItem->flags() & ~Qt::ItemIsEditable );
  inpItem->setText( (inKind & ParsedComment::shapeMask) == Matrix ? "Matrix..." : "List..." );
  inpItem->setBackground( QBrush( Qt::lightGray ) );
  inpItem->setData( Qt::UserRole, -1 );
}

void
EditMatrix::SetNormalEntryProperties( QTableWidgetItem* inpItem, int row, int col )
{
  if( mEditable )
    inpItem->setFlags( inpItem->flags() | Qt::ItemIsEditable );
  else
    inpItem->setFlags( inpItem->flags() & ~Qt::ItemIsEditable );
  inpItem->setBackground( QBrush( Qt::white ) );
  int kind = 0;
  if( col < mColumnKinds.size() )
    kind = mColumnKinds[col];
  inpItem->setData( Qt::UserRole, kind );
}

bool
EditMatrix::IsSubmatrix( QTableWidgetItem* inpItem )
{
  return inpItem && inpItem->data( Qt::UserRole ).toInt() == -1;
}


void
EditMatrix::MatrixToPlainCell( int inRow, int inCol )
{
  int subMatrixRows = mpMatrixParam->Value( inRow, inCol )->NumRows(),
      subMatrixCols = mpMatrixParam->Value( inRow, inCol )->NumColumns();
  QString message = tr("You are about to replace this sub-matrix with a single value.\n");
  if( subMatrixRows > 1 || subMatrixCols > 1 )
    message += tr("This will discard all sub-matrix entries except the top left one.\n");
  message += tr("Proceed?");

  if( QMessageBox::Ok == QMessageBox::question( this, tr("Confirmation"), message, QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel ) )
  {
    // Descend into sub-matrices until a single value is found.
    const Param::ParamValue* val = &mpMatrixParam->Value( inRow, inCol )->Value( 0, 0 );
    while( val->Kind() == Param::ParamValue::Matrix )
      val = &val->ToParam()->Value( 0, 0 );
    mpMatrixParam->Value( inRow, inCol ) = *val;
    QTableWidgetItem* item = m_ui->tableWidget->item( inRow, inCol );
    SetNormalEntryProperties( item, inRow, inCol );
    item->setText( QString::fromLocal8Bit( mpMatrixParam->Value( inRow, inCol ).c_str() ) );
    AdaptColumnWidths();
  }
}

void
EditMatrix::CopyToClipboard( int inRow, int inCol )
{
  QApplication::clipboard()->setText( QString::fromLocal8Bit( mpMatrixParam->Value( inRow, inCol ).AsString().c_str() ) );
}

void
EditMatrix::Help( int, int )
{
  ParsedComment p( *mpMatrixParam );
  if( ExecutableHelp().ParamHelp().Exists( p.Name() ) )
    ExecutableHelp().ParamHelp().Open( p.Name(), p.HelpContext() );
}

void
EditMatrix::BuildContextMenu()
{
  mpEditSubMatrix = new QAction( tr("Edit submatrix ..."), this );
  mpConvertToSubMatrix = new QAction( tr("Convert to submatrix"), this );
  mpConvertToList = new QAction( tr("Convert to list"), this );
  mpReplaceWithSingleCell = new QAction( tr("Replace with single cell"), this );
  mpCopyToClipboard = new QAction( "Copy to clipboard", this );
  mpHelp = new QAction( tr("Help ..."), this );

  mpContextMenu = new QMenu( this );
  mpContextMenu->addAction( mpCopyToClipboard );
  mpContextMenu->addSeparator();
  mpContextMenu->addAction( mpEditSubMatrix );
  mpContextMenu->addAction( mpConvertToSubMatrix );
  mpContextMenu->addAction( mpConvertToList );
  mpContextMenu->addAction( mpReplaceWithSingleCell );
  mpContextMenu->addSeparator();
  mpContextMenu->addAction( mpHelp );

  m_ui->tableWidget->setContextMenuPolicy( Qt::CustomContextMenu );
  connect( m_ui->tableWidget, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(ShowContextMenu(const QPoint&)) );
  connect( mpContextMenu, SIGNAL(triggered(QAction*)), this, SLOT(PerformContextAction(QAction*)) );
}

void
EditMatrix::ShowContextMenu( const QPoint& inP )
{
  QTableWidget* t = m_ui->tableWidget;
  QTableWidgetItem* item = t->itemAt( inP );
  if( item )
  {
    mContextRow = item->row();
    mContextCol = item->column();
  }
  else
  {
    mContextRow = -1;
    mContextCol = -1;
  }
  mpCopyToClipboard->setEnabled( item != 0 );
  mpEditSubMatrix->setText( mEditable ? "Edit submatrix..." : "View submatrix..." );
  mpEditSubMatrix->setEnabled( IsSubmatrix( item ) );
  mpConvertToSubMatrix->setEnabled( mEditable && !IsSubmatrix( item ) );
  mpConvertToList->setEnabled( mEditable && !IsSubmatrix( item ) );
  mpReplaceWithSingleCell->setEnabled( mEditable && IsSubmatrix( item ) );
  mpContextMenu->popup( t->mapToGlobal( inP ) );
}

void
EditMatrix::PerformContextAction( QAction* inpAction )
{
  if( inpAction == mpCopyToClipboard )
    CopyToClipboard( mContextRow, mContextCol );
  else if( inpAction == mpEditSubMatrix )
    EditSubMatrix( mContextRow, mContextCol );
  else if( inpAction == mpConvertToSubMatrix )
    PlainCellToMatrix( mContextRow, mContextCol );
  else if( inpAction == mpConvertToList )
    PlainCellToList( mContextRow, mContextCol );
  else if( inpAction == mpReplaceWithSingleCell )
    MatrixToPlainCell( mContextRow, mContextCol );
  else if( inpAction == mpHelp )
    Help( mContextRow, mContextCol );
}

void
EditMatrix::OnChangeMatrixSizeClick()
{
  const char* q = "Change size?";
  switch( mMode )
  {
  case Matrix:
    q = "Change the matrix size?";
    break;
  case List:
    q = "Change the list's size?";
    break;
  }
  if( QMessageBox::Ok == QMessageBox::question(
      this,
      "Confirmation", q,
      QMessageBox::Ok | QMessageBox::Cancel,
      QMessageBox::Cancel ) )
  {
    Lock();
    UpdateParam();
    mpMatrixParam->SetDimensions( m_ui->rowsSpinBox->value(), m_ui->colsSpinBox->value() );
    UpdateDisplay();
    Unlock();
  }
  else
  {
    m_ui->rowsSpinBox->setValue( m_ui->tableWidget->rowCount() );
    m_ui->colsSpinBox->setValue( m_ui->tableWidget->columnCount() );
  }
}

void
EditMatrix::OnClose()
{
  QTableWidget* w = m_ui->tableWidget;
  QWidget* c = w->cellWidget( w->currentRow(), w->currentColumn() );
  if( c )
    w->itemDelegate()->setModelData( c, w->model(), w->currentIndex() );
  UpdateParam();
}

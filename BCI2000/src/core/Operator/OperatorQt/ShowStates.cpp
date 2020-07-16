//////////////////////////////////////////////////////////////////////
// $Id: ShowStates.cpp 5209 2016-01-21 16:46:18Z mellinger $
// Authors: schalk@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: A dialog displaying system states.
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
#include "ShowStates.h"
#include "BCI_OperatorLib.h"
#include "WatchesWindow.h"

#include <QDialogButtonBox>
#include <QListWidget>
#include <QBoxLayout>
#include <QGroupBox>
#include <QApplication>
#include <QContextMenuEvent>
#include <QMenu>

namespace
{

struct StatesList : QListWidget
{
  StatesList( QWidget* parent )
  : QListWidget( parent )
  {
    setSelectionMode( QAbstractItemView::ExtendedSelection );
  }
  void Populate( const char* (STDCALL *Enumerate)( long ) )
  {
    int i = 0;
    for( const char* p = Enumerate( i ); p; p = Enumerate( ++i ) )
    {
      QListWidgetItem* item = new QListWidgetItem;
      item->setText( p );
      addItem( item );
      BCI_ReleaseObject( p );
    }
  }
  void contextMenuEvent( QContextMenuEvent* ev ) override
  {
    ev->accept();
    QString single = "...";
    QListWidgetItem* pSingle = itemAt( ev->pos() );
    if( pSingle )
      single = pSingle->text().split( " " ).first();
    QStringList group;
    auto selected = selectedItems();
    for( auto p : selected )
      group << p->text().split( " " ).first();
    QString multiple = "{ " + group.join( ", " ) + " }";
    if( !single.isEmpty() || !group.isEmpty() )
    {
      QMenu menu;
      QAction* addSingle = menu.addAction( "Add watch: " + single );
      addSingle->setEnabled( pSingle );
      QAction* addGroup = menu.addAction( "Add watch: " + multiple );
      addGroup->setEnabled( !group.isEmpty() );
      QAction* result = menu.exec( ev->globalPos() );
      if( result == addSingle )
        WatchesWindow::AddWatch( single );
      else if( result == addGroup )
        WatchesWindow::AddWatch( multiple );
    }
  }
};

}

ShowStates::ShowStates( QWidget* parent )
: QDialog( parent )
{
  QLayout* pLayout1 = new QVBoxLayout,
          *pLayout2 = new QHBoxLayout;

  QGroupBox* pGroupBox = new QGroupBox( "States", this );
  pGroupBox->setLayout( new QHBoxLayout );
  StatesList* pList = new StatesList( this );
  pList->Populate( &BCI_GetState );
  pGroupBox->layout()->addWidget( pList );
  pLayout2->addWidget( pGroupBox );

  pGroupBox = new QGroupBox( "Streams", this );
  pGroupBox->setLayout( new QHBoxLayout );
  pList = new StatesList( this );
  pList->Populate( &BCI_GetStream );
  pGroupBox->layout()->addWidget( pList );
  pLayout2->addWidget( pGroupBox );

  pGroupBox = new QGroupBox( "Events", this );
  pGroupBox->setLayout( new QHBoxLayout );
  pList = new StatesList( this );
  pList->Populate( &BCI_GetEvent );
  pGroupBox->layout()->addWidget( pList );
  pLayout2->addWidget( pGroupBox );

  pLayout1->addItem( pLayout2 );
  QDialogButtonBox* pButtonBox = new QDialogButtonBox( QDialogButtonBox::Ok, this );
  connect( pButtonBox, SIGNAL(accepted()), this, SLOT(accept()) );
  pLayout1->addWidget( pButtonBox );
  setLayout( pLayout1 );
  setWindowTitle( "BCI2000 States and Events" );

  resize( 600, 200 );
}

ShowStates::~ShowStates()
{
}


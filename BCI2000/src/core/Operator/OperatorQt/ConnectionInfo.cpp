//////////////////////////////////////////////////////////////////////
// $Id: ConnectionInfo.cpp 5291 2016-03-17 18:26:36Z mellinger $
// Authors: schalk@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: The Operator module's connection info dialog.
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
#include "ConnectionInfo.h"
#include "BCI_OperatorLib.h"
#include "Synchronized.h"
#include <QBoxLayout>
#include <QLabel>

struct ConnectionInfo::Private
{
  struct ConnectionDisplay : QWidget
  {
    ConnectionDisplay()
    : mpTitle( new QLabel( this ) ),
      mpText( new QLabel( this ) )
    {
      mpText->setMaximumWidth( 200 );
      QVBoxLayout* pLayout = new QVBoxLayout;
      pLayout->addWidget( mpTitle );
      pLayout->addWidget( mpText );
      setLayout( pLayout );
    };
    void SetTitle( const QString& s )
    {
      mpTitle->setText( "<b>" + s + "</b>" );
    }
    void SetText( const QString& s )
    {
      mpText->setText( s );
    }
    QLabel* mpTitle, *mpText;
  };
  std::vector<ConnectionDisplay*> mConnections;
  QWidget* mpParent;
  bool mNeverShown;
  Synchronized<int> mUpdatesPosted;
};

ConnectionInfo::ConnectionInfo( QWidget* parent )
: QDialog( parent, Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint ),
  p( new Private )
{
  p->mpParent = parent;
  p->mNeverShown = true;
  setLayout( new QHBoxLayout );
  setWindowTitle( "Connection Info" );
}

ConnectionInfo::~ConnectionInfo()
{
  for( auto pConnection : p->mConnections )
    delete pConnection;
  delete p;
}

void
ConnectionInfo::UpdateDisplay()
{
  if( isVisible() && ++p->mUpdatesPosted == 1 )
    QMetaObject::invokeMethod( this, "OnUpdateDisplay", Qt::QueuedConnection );
}

void
ConnectionInfo::OnUpdateDisplay()
{
  int i = 0;
  for( const char* pInfo = nullptr; pInfo = BCI_GetConnectionInfo( i ); ++i )
  {
    QString info( pInfo );
    BCI_ReleaseObject( pInfo );

    if( p->mConnections.size() <= i )
    {
      p->mConnections.push_back( new Private::ConnectionDisplay );
      QString tag = "Name: ";
      int pos1 = info.indexOf( tag ) + tag.length(), pos2 = info.indexOf( "\n", pos1 );
      p->mConnections.back()->SetTitle( info.mid( pos1, pos2 - pos1 ) );
      layout()->addWidget( p->mConnections.back() );
    }
    p->mConnections[i]->SetText( info );
  }
  while( p->mConnections.size() > i )
  {
    delete p->mConnections.back();
    p->mConnections.pop_back();
  }
  p->mUpdatesPosted = 0;
}

void
ConnectionInfo::showEvent( QShowEvent* e )
{
  if( p->mpParent && p->mNeverShown )
  {
    UpdateDisplay();
    p->mNeverShown = false;
    this->resize( p->mpParent->size() );
    this->move( p->mpParent->x(), p->mpParent->y() - frameGeometry().height() );
  }
}

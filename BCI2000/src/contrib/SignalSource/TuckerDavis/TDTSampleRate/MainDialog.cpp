////////////////////////////////////////////////////////////////////////////////
// $Id: MainDialog.cpp 4607 2013-10-14 13:18:08Z mellinger $
// Authors: jadamwilson2@gmail.com, juergen.mellinger@uni-tuebingen.de
// Description: The main dialog for the TDTSampleRate tool.
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
#include "MainDialog.h"
#include <QtWidgets>

MainDialog::MainDialog()
: mpLineEdit1( new QLineEdit( "512" ) ),
  mpLineEdit2( new QLineEdit ),
  mpLineEdit3( new QLineEdit ),
  mpButton( new QPushButton( tr("Calculate") ) )
{
  this->setWindowTitle( "TDTSampleRate" );
  QVBoxLayout* pMainLayout = new QVBoxLayout;
  pMainLayout->addWidget( new QLabel( tr("Enter desired sample rate here:") ) );
  pMainLayout->addWidget( mpLineEdit1 );
  pMainLayout->addWidget( mpButton );
  pMainLayout->addWidget( new QLabel( tr("Enter this sample rate in BCI2000:") ) );
  pMainLayout->addWidget( mpLineEdit2 );
  mpLineEdit2->setReadOnly( true );
  pMainLayout->addWidget( new QLabel( tr("Sample period:") ) );
  pMainLayout->addWidget( mpLineEdit3 );
  mpLineEdit3->setReadOnly( true );
  this->setLayout( pMainLayout );
  connect( mpButton, SIGNAL(clicked()), this, SLOT(compute()) );
}

void
MainDialog::compute()
{
  const float TDTsampleRate = 24414.0625;
  float sr = mpLineEdit1->text().toFloat();
  int per = static_cast<int>( ::floor( TDTsampleRate / sr ) );
  mpLineEdit2->setText( QString::number( TDTsampleRate / per, 'g', 15 ) );
  mpLineEdit3->setText( QString::number( per ) );
}

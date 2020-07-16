/***************************************************************************
 *   Copyright (C) 2007 by J. Adam Wilson   *
 *   jawilson@cae.wisc.edu   *
 *   juergen.mellinger@uni-tuebingen.de: fixed a parsing issue,
 *                                       added receiving thread.
 *                                                                         *
 * $BEGIN_BCI2000_LICENSE$
 *
 * This file is part of BCI2000, a platform for real-time bio-signal research.
 * [ Copyright (C) 2000-2012: BCI2000 team and many external contributors ]
 *
 * BCI2000 is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * BCI2000 is distributed in the hope that it will be useful, but
 *                         WITHOUT ANY WARRANTY
 * - without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * $END_BCI2000_LICENSE$
 ***************************************************************************/

#include "mainUI.h"
#include <sstream>

using namespace std;

mainUI::mainUI()
: mReceivingThread( *this )
{
  // setupUI is used to create the form and layouts
  setupUI();

  //in this program, the two major components are two group boxes
  //this first one handles the connection to BCI2000, and the second
  //provides info about the states being read.
  //Here we organize these in a vertical box layout, so that the connection
  //group appears above the state info group.
  mainLayout = new QVBoxLayout;
  mainLayout->addWidget(connectGroup,0);
  mainLayout->addWidget(statesGroup,0);
  setLayout(mainLayout);
  setWindowTitle("AppConnectorExample");
  resize(250,800);
}

mainUI::~mainUI()
{
  {
    QMutexLocker locker( &mMutex );
    recSocket.close();
  }
  mReceivingThread.wait();
}

//---------------------------------------------------------------
void mainUI::connectButCallback()
{
  //this SLOT is called when the connect button is clicked
  //the connections and sockets are first cleared and closed.
  //then the program attempts to connect to BCI2000 using the
  //IP and ports specified. For a more "robust" example, see
  //the ConnectorFilter.cpp file in the BCI2000/src/shared/modules/application folder
  bool sendconnected = true, recconnected = true;
  stringstream str;

  {
    QMutexLocker locker( &mMutex ); // Protect access to recSocket.
    recConnection.close();
    recConnection.clear();
    recSocket.close();
  }
  mReceivingThread.wait();

  sendConnection.close();
  sendConnection.clear();
  sendSocket.close();

  recSocket.open(receiveBox->text().toStdString().c_str());
  recConnection.open(recSocket);
  if (!recConnection.is_open())
  {
    //statusBox->setText("Could not connect to receiving address");
    sendconnected = false;
    str <<"Receive = false ";
  }
  else
  {
    mReceivingThread.start();
    str << "Reveive = true ";
  }

  sendSocket.open(sendBox->text().toStdString().c_str());
  sendConnection.open(sendSocket);
  if (!sendConnection.is_open())
  {
    //statusBox->setText("Could not connect to sending address");
    recconnected = false;
    str << "Send = false";
  }
  else
  {
    str << "Send = true";
  }

  statusBox->setText(str.str().c_str());
  if (!sendconnected || !recconnected)
    return;
  //else
  //	statusBox->setText("Connected!");

  //if we successfully connected, then start the timer
  timer->start();
}

void mainUI::timerUpdate()
{
  //this function is called every time the timer period is finished
  //updateVars();

  //update state display
  QMutexLocker locker( &mMutex );
  if( states.size() != stateTable->rowCount() )
  {
    stateTable->setRowCount( static_cast<int>( states.size( )) );
    for( int row = 0; row < stateTable->rowCount(); ++row )
    {
      stateTable->setItem( row, 0, new QTableWidgetItem );
      stateTable->setItem( row, 1, new QTableWidgetItem );
    }
  }
  int row = 0;
  for( map<string, string>::const_iterator i = states.begin(); i != states.end(); ++i )
  {
    stateTable->item( row, 0 )->setText( i->first.c_str() );
    stateTable->item( row, 1 )->setText( i->second.c_str() );
    ++row;
  }
}

// This function runs inside its own thread.
void mainUI::ReceivingThread::run()
{
  while( parent.recSocket.is_open() )
  {
    QMutexLocker locker( &parent.mMutex );
    if( parent.recSocket.wait_for_read( 10 ) )
    {
      string line;
      std::getline( parent.recConnection, line );
      istringstream iss( line );
      string name, value;
      //get the name and the value of the state
      iss >> name >> value;
      parent.states[name] = value;
    }
  }
}

//---------------------------------------------------------------
void mainUI::writeValue(const string& name, double value)
{
  //this function writes a value to a state in BCI2000
  //Isn't this easy?!
  sendConnection << name << ' ' << value << endl;
}

//----------------------------------------------
void mainUI::changeStateValue(int row, int column)
{
  //we only want to send a new value, not change the state name
  statusBox->setText("Changing value...");
  if (column == 0)
    return;
  bool ok1, ok2;

  if (!stateTable->item(row,0))
    return;

  string name = stateTable->item(row,0)->text().toStdString();

  stringstream str1;
  short value = stateTable->item(row, 1)->text().toShort(&ok1);

  str1 << "Enter a new value for " << name;

  short newValue = QInputDialog::getInt(this, "",str1.str().c_str(),value, -32767, 32767, 1, &ok2 );

  if (ok1 && ok2)
  {
    writeValue(name, newValue);
    stringstream str;
    str << "Wrote " << name << ": "<< newValue<<endl;
    statusBox->setText(str.str().c_str());
  }
  else
    statusBox->setText("There was an error...");

}

void mainUI::setupUI()
{
  //this function sets up both group boxes, and sets the timer interval
  setupConnectionGroup();
  setupStatesGroup();
  resize(minimumSizeHint());

  timer = new QTimer(this);
  //here, the timer SIGNAL is connected to the timerUpdate SLOT
  timer->connect(timer, SIGNAL(timeout()), this, SLOT(timerUpdate()));
  timer->setInterval(100);
  timer->stop();
}

void mainUI::setupConnectionGroup()
{
  //create the connection group box and setup the signals/slots
  connectGroup = new QGroupBox("Communication Ports");
  //connectGroup->setGeometry(QRect(10, 180, 150, 150));

  connectLayout = new QVBoxLayout;

  receiveLabel = new QLabel("Input IP:Port");
  receiveBox = new QLineEdit();
  receiveBox->setText("localhost:20321");
  sendLabel = new QLabel("Output IP:Port");
  sendBox = new QLineEdit();
  sendBox->setText("localhost:20320");
  connectBut = new QPushButton("Open Ports");
  statusBox = new QLineEdit;
  connect(connectBut, SIGNAL(clicked()), this, SLOT(connectButCallback()));

  connectLayout->addWidget(receiveLabel,0);
  connectLayout->addWidget(receiveBox,0);
  connectLayout->addWidget(sendLabel,0);
  connectLayout->addWidget(sendBox,0);
  connectLayout->addWidget(connectBut,0,Qt::Alignment(4));
  connectLayout->addWidget(statusBox);

  connectGroup->setLayout(connectLayout);
}

void mainUI::setupStatesGroup()
{
  statesGroup = new QGroupBox("States");
  //statesGroup->setGeometry(QRect(10, 10, 251, 171));

  statesLayout = new QGridLayout;
  statesLayout->setSpacing(6);
  statesLayout->setMargin(0);

  stateTable = new QTableWidget;
  stateTable->setColumnCount(2);
  stateTable->setRowCount(1);
  QStringList headers;
  headers << "State" << "Value";
  stateTable->setHorizontalHeaderLabels(headers);

  //add the signal/slot so that if a table value is double clicked, a dialog will appear
  stateTable->connect(stateTable, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(changeStateValue(int, int)));

  statesLayout->addWidget(stateTable,0,0);

  statesGroup->setLayout(statesLayout);
}

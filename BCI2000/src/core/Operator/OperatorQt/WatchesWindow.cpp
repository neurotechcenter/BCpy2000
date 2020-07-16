//////////////////////////////////////////////////////////////////////
// $Id$
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: A window displaying user-editable expressions.
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
#include "WatchesWindow.h"
#include "MainWindow.h"
#include "OperatorUtils.h"
#include "TimeUtils.h"
#include "Debugging.h"

#include <QMenuBar>
#include <QBoxLayout>
#include <QTableWidget>
#include <QStatusBar>
#include <QHeaderView>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QPainter>
#include <QInputDialog>

#include <deque>
#include <vector>

namespace
{
// https://www.w3.org/TR/SVG/types.html#ColorKeywords
const QString sPlotColors[] = { "black", "red", "green", "blue", "blueviolet", "gray", "lightgray", "fuchsia", "orangered" };
const int sNumPlotColors = sizeof(sPlotColors) / sizeof(*sPlotColors);

enum { expression, value, range, plot, numColumns };

struct RangeWidget : QWidget
{
  void SetRange( double min, double max )
  {
    mText = QString( "%1 ... %2" ).arg( min ).arg( max );
    update();
  }
  void paintEvent( QPaintEvent* ) override
  {
    QPainter( this ).drawText( rect(), Qt::AlignCenter, mText );
  }
  QString mText;
};

struct WatchPlot : QWidget
{
  WatchPlot( double maxTime, const double& now, RangeWidget* pRangeWidget )
  : mMaxTime( maxTime ), mrNow( now ), mpRangeWidget( pRangeWidget )
  {
  }
  void SetMaxTime( double maxTime )
  {
    mMaxTime = maxTime;
    double now = TimeUtils::MonotonicTime().Seconds();
    auto it = mHistory.begin();
    while( it != mHistory.end() && now - (*it)[0] > mMaxTime )
      ++it;
    if( it != mHistory.begin() )
      --it;
    while( it != mHistory.begin() )
      mHistory.pop_front();
    update();
  }
  void AddDataPoint( const QString& inData, double inTime )
  {
    auto values = inData.split( '\t' );
    if( !mHistory.empty() && mHistory.front().size() != values.size() + 1 )
      mHistory.clear();
    std::vector<float> data( 1, inTime );
    data.reserve( values.size() + 1 );
    for( const auto& value : values )
    {
      bool ok = false;
      double d = value.toFloat( &ok );
      if( !ok )
        d = NaN( d );
      data.push_back( d );
    }
    mHistory.push_back( data );
    SetMaxTime( mMaxTime );
  }
  void paintEvent( QPaintEvent* ) override
  {
    if( !mHistory.empty() )
    {
      QPainter painter( this );
      painter.setRenderHint( QPainter::Antialiasing );
      QRectF r = rect().adjusted( 1, 1, -1, -1 );
      QPointF origin( r.right(), r.top() + r.height() / 2 );
      float tscale = r.width() / mMaxTime,
            vmax = 0, vmin = 0;
      std::vector<QPointF> line( 2 * mHistory.size() + 1 );
      float t0 = mrNow;
      int i = 0;
      for( const auto& point : mHistory )
      {
        float x = tscale * ( point[0] - t0 ) + origin.x();
        line[i++].setX( x );
        line[i++].setX( x );
        for( int j = 1; j < point.size(); ++j )
        {
          vmax = std::max( vmax, point[j] );
          vmin = std::min( vmin, point[j] );
        }
      }
      line[i++].setX( tscale * t0 + origin.x() );
      float vscale = r.height() / ( vmax - vmin + 1 ), v0 = ( vmax + vmin ) / 2;
      for( int j = 1; j < mHistory.front().size(); ++j )
      {
        int i = 1;
        for( const auto& point : mHistory )
        {
          float y = origin.y() - vscale * ( point[j] - v0 );
          line[i++].setY( y );
          line[i++].setY( y );
        }
        painter.setPen( QPen( QColor( sPlotColors[(j - 1) % sNumPlotColors] ), 1 ) );
        painter.drawPolyline( line.data() + 1, line.size() - 1 );
      }
      if( mpRangeWidget )
        mpRangeWidget->SetRange( vmin, vmax );
    }
  }
  std::deque< std::vector<float> > mHistory;
  double mMaxTime;
  const double& mrNow;
  RangeWidget* mpRangeWidget;
};

struct WindowState
{
  double historyLength;
  int colWidth[numColumns];
};

WatchesWindow* spInstance = nullptr;

}

struct WatchesWindow::Watch : QLabel, bci::OperatorModule::ExpressionWatch
{
  Watch( WatchesWindow* pWindow, WatchPlot* pPlot )
  : mpWindow( pWindow ), mpPlot( pPlot )
  {
    setTextFormat( Qt::RichText );
    setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
  }
  void OnValueChanged( const char* inValue ) override
  {
    QMetaObject::invokeMethod(
      mpWindow, "OnValuesReceived",
      Qt::AutoConnection,
      Q_ARG( void*, this ),
      Q_ARG( QString, inValue ),
      Q_ARG( double, TimeUtils::MonotonicTime().Seconds() )
    );
  }
  WatchesWindow* mpWindow;
  WatchPlot* mpPlot;
};

void
WatchesWindow::Initialize( MainWindow* parent )
{
  Assert( !spInstance );
  spInstance = new WatchesWindow( parent );
}

void
WatchesWindow::Deinitialize()
{
  delete spInstance;
  spInstance = nullptr;
}

WatchesWindow::WatchesWindow( MainWindow* parent )
: mpMenuEntry( nullptr ),
  mpTable( nullptr ),
  mHistoryLengthSeconds( -1 ),
  mLastUpdate( -1 ),
  mUpdateInterval( 1.0 / 24 /* seconds */ )
{
  setWindowTitle( parent->windowTitle() + " Watches" );
  setWindowFlags( Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint );
  setAttribute( Qt::WA_QuitOnClose, false );
  mpMenuEntry = parent->GetWindowMenu()->addAction( "Watches" );
  mpMenuEntry->setShortcut( Qt::CTRL + Qt::ALT + Qt::Key_W );
  mpMenuEntry->setCheckable( true );
  mpMenuEntry->setChecked( this->isVisible() );
  connect( mpMenuEntry, SIGNAL(triggered(bool)), this, SLOT(setVisible(bool)) );

  QAction* pAction = new QAction("Save Watches", this);
  connect(pAction, SIGNAL(triggered(bool)), this, SLOT(SaveWatches()));
  addAction(pAction);
  pAction = new QAction("Restore Watches", this);
  connect(pAction, SIGNAL(triggered(bool)), this, SLOT(RestoreWatches()));
  addAction(pAction);
  setContextMenuPolicy(Qt::ActionsContextMenu);

  QLayout* pLayout = new QVBoxLayout;
  pLayout->setMargin( 0 );

  mpTable = new QTableWidget( this );
  mpTable->setColumnCount( numColumns );
  mpTable->horizontalHeader()->setStretchLastSection( true );
  mpTable->verticalHeader()->setVisible( false );
  connect( mpTable->horizontalHeader(), SIGNAL(sectionDoubleClicked(int)), SLOT(OnHeaderDoubleClick(int)) );
  connect( mpTable, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(OnItemChanged(QTableWidgetItem*)) );
  pLayout->addWidget( mpTable );

  QStatusBar* pStatusBar = new QStatusBar;
  pStatusBar->setSizeGripEnabled(true);
  pLayout->addWidget(pStatusBar);

  QPushButton* pButton = new QPushButton("Save Watches");
  connect(pButton, SIGNAL(clicked()), this, SLOT(SaveWatches()));
  pStatusBar->addWidget(pButton);
  pButton = new QPushButton("Restore Watches");
  connect(pButton, SIGNAL(clicked()), this, SLOT(RestoreWatches()));
  pStatusBar->addWidget(pButton);
  pButton = new QPushButton("Clear Watches");
  connect(pButton, SIGNAL(clicked()), this, SLOT(ClearWatches()));
  pStatusBar->addWidget(pButton);

  setLayout(pLayout);

  AddEmptyLine();
  QByteArray state;
  if( !OperatorUtils::RestoreWidget( this, &state ) )
    setGeometry( 100, 100, 400, 100 );
  if( state.isEmpty() )
  {
    mpTable->setColumnWidth( value, mpTable->columnWidth( value ) / 2 );
    mpTable->setColumnWidth( range, mpTable->columnWidth( range ) / 2 );
    SetHistoryLengthSeconds( 20 );
  }
  else
  {
    union { const char* c; WindowState* s; } pWindowState = { state.data() };
    for( int col = 0; col < numColumns; ++col )
      mpTable->setColumnWidth( col, pWindowState.s->colWidth[col] );
    SetHistoryLengthSeconds( pWindowState.s->historyLength );
  }
  startTimer( mUpdateInterval * 1e3 + 1 );
}

WatchesWindow::~WatchesWindow()
{
  union { WindowState s; char c[sizeof(WindowState)]; } windowState = { 0 };
  windowState.s.historyLength = mHistoryLengthSeconds;
  for( int col = 0; col < numColumns; ++col )
    windowState.s.colWidth[col] = mpTable->columnWidth( col );
  auto bytes = QByteArray( windowState.c, sizeof( windowState ) );
  OperatorUtils::SaveWidget( this, &bytes );
}

void
WatchesWindow::timerEvent( QTimerEvent* )
{
  Update();
}

void
WatchesWindow::Update()
{
  double now = TimeUtils::MonotonicTime().Seconds();
  if( now - mLastUpdate > 0.9 * mUpdateInterval )
  {
    mLastUpdate = now;
    for( int i = 0; i < mpTable->rowCount(); ++i )
    {
      QWidget* pWidget = mpTable->cellWidget( i, plot );
      if( pWidget )
        pWidget->update();
    }
  }
}

void
WatchesWindow::closeEvent( QCloseEvent* )
{
  if( mpMenuEntry )
    mpMenuEntry->setChecked( false );
}

void
WatchesWindow::OnHeaderDoubleClick( int idx )
{
  if( sender() == mpTable->horizontalHeader() && idx == plot )
  {
    bool ok = false;
    double length = QInputDialog::getDouble(
      this, "Edit history length", "New history length in seconds:",
      mHistoryLengthSeconds, 0, 3600, 1, &ok
    );
    if( ok )
      SetHistoryLengthSeconds( length );
  }
}

void
WatchesWindow::OnItemChanged( QTableWidgetItem* pItem )
{
	if( pItem->column() != expression )
		return;

	QString data = pItem->data(Qt::DisplayRole).toString().trimmed();
	if (data.startsWith('{') && data.endsWith('}'))
		data = data.mid(1, data.length() - 2).split(',').join('\t');

	int row = pItem->row();
	Watch* pWatch = dynamic_cast<Watch*>(mpTable->cellWidget(row, 1));
	if (pWatch)
	{
		pWatch->clear();
		pWatch->Register(data.toStdString().c_str());
	}
	if (data.isEmpty())
		mpTable->removeRow(row);
	if (mpTable->rowCount() < 1)
		AddEmptyLine();
	else
	{
		QTableWidgetItem* pLastLineItem = mpTable->item(mpTable->rowCount() - 1, expression);
		if (pLastLineItem && !pLastLineItem->data(Qt::DisplayRole).toString().trimmed().isEmpty())
			AddEmptyLine();
	}
}

void
WatchesWindow::OnValuesReceived( void* pData, QString values, double time )
{
	Watch* pWatch = static_cast<Watch*>(pData);
	// Watch may no longer exist by the time this is executed in the main thread
	bool found = false;
	for (int row = 0; row < mpTable->rowCount(); ++row)
	{
		if (pWatch == mpTable->cellWidget(row, value))
		{
			found = true;
			break;
		}
	}
	if (!found)
		return;

	QStringList valueList = values.split('\t');
	int i = 0;
	for (auto& s : valueList)
		s = QString("<font color='%1'>%2</font>").arg(sPlotColors[i++ % sNumPlotColors]).arg(s);
	bool wereUpdatesEnabled = updatesEnabled();
	setUpdatesEnabled(false);
	pWatch->setText(valueList.join(", ").trimmed());
	pWatch->mpPlot->AddDataPoint(values, time);
	setUpdatesEnabled(wereUpdatesEnabled);
    Update();
}

void
WatchesWindow::AddEmptyLine()
{
	int row = mpTable->rowCount();
	mpTable->setRowCount(mpTable->rowCount() + 1);
	RangeWidget* pRange = new RangeWidget;
	Watch* pWatch = new Watch(this, new WatchPlot(mHistoryLengthSeconds, mLastUpdate, pRange));
	mpTable->setCellWidget(row, value, pWatch);
	mpTable->setCellWidget(row, range, pRange);
	mpTable->setCellWidget(row, plot, pWatch->mpPlot);
	for (int col : { value, range, plot })
		mpTable->cellWidget(row, col)->setFocusPolicy(Qt::ClickFocus); // avoid Qt warning about edit failure
}

void
WatchesWindow::RemoveLastLine()
{
	if (mpTable->rowCount() > 0)
		mpTable->setRowCount(mpTable->rowCount() - 1);
}

void
WatchesWindow::SetHistoryLengthSeconds( double inSeconds )
{
  if( inSeconds != mHistoryLengthSeconds )
  {
    mHistoryLengthSeconds = inSeconds;
	for (int row = 0; row < mpTable->rowCount(); ++row)
	{
		Watch* pWatch = dynamic_cast<Watch*>(mpTable->cellWidget(row, 1));
		if (pWatch && pWatch->mpPlot)
			pWatch->mpPlot->SetMaxTime(inSeconds);
	}
	QStringList labels;
	labels << "Expression" << "Value" << "Range" << QString("Past %1s").arg(mHistoryLengthSeconds);
	mpTable->setHorizontalHeaderLabels(labels);
  }
}

void
WatchesWindow::AddWatch( const QString& inExpression )
{
  if( spInstance )
  {
    QTableWidget* pTable = spInstance->mpTable;
	pTable->setItem( pTable->rowCount() - 1, 0, new QTableWidgetItem( inExpression ) );
    spInstance->show();
  }
}

void
WatchesWindow::SaveWatches()
{
	QByteArray data;
	for (int row = 0; row < mpTable->rowCount(); ++row)
	{
		auto* pItem = mpTable->item(row, expression);
		if (pItem)
			data.append(pItem->text()).append(1, 0);
	}
	OperatorUtils::PutPersistentData("Watches", data);
}

void
WatchesWindow::RestoreWatches()
{
	ClearWatches();
	QByteArray data;
	OperatorUtils::GetPersistentData("Watches", data);
	QString expression;
	for (int i = 0; i < data.length(); ++i)
	{
		char c = data[i];
		if (c == 0)
		{
			mpTable->setItem(mpTable->rowCount() - 1, 0, new QTableWidgetItem(expression));
			expression.clear();
		}
		else
			expression += c;
	}
}

void
WatchesWindow::ClearWatches()
{
	mpTable->setRowCount(0);
	AddEmptyLine();
}

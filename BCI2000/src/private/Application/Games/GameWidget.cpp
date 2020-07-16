////////////////////////////////////////////////////////////////////////////////
// $Id: GameWidget.cpp 3293 2011-05-30 01:09:57Z jhill $
// Authors: griffin.milsap@gmail.com
// Description: A QGLWidget based class which updates independent of BCI2000
//
// (C) 2000-2010, BCI2000 Project
// http://www.bci2000.org
////////////////////////////////////////////////////////////////////////////////

#include "GameWidget.h"
#include "ThemeManager.h"
#include <qapplication.h>
#include <qnamespace.h>

using namespace std;

// Constructor - sets up the timer to call the "UpdateEvent"
GameWidget::GameWidget( QWidget *parent, char *name ) : QGLWidget( QGLFormat( QGL::SampleBuffers ), parent, NULL, Qt::FramelessWindowHint )
{
	mRunning = false;
	mGameTime = 0;
	mVolume = 1.0f;
}

// Deconstructor - deletes the timer object
GameWidget::~GameWidget()
{
}

void GameWidget::SetClearColor( QColor &color )
{
	mClearColor = color;
}

void GameWidget::SetVolume( float volume )
{
	mVolume = volume;
}

void GameWidget::initializeGL()
{
	// Created the widget
	qglClearColor( mClearColor );

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);

	OnCreate();

	// Apply any sort of theme
	ThemeManager::Instance().ApplyTheme( this );
}

void GameWidget::resizeGL( int width, int height )
{
	// Set the viewport size
    int side = qMin( width, height );
    glViewport( ( width - side ) / 2, ( height - side ) / 2, side, side);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

	// Allow the user to set his/her own frustum
	SetProjection();

	glMatrixMode(GL_MODELVIEW);

	// Alert subclasses that a resize occurred
	OnResize( width, height );
}

void GameWidget::paintGL()
{
	qglClearColor( mClearColor );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glLoadIdentity();

	// Allow subclasses to paint
	Update();
}

void GameWidget::SetProjection()
{
	// Load an orthographic view port (this is 2D)
    glOrtho(-1.0, +1.0, -1.0, +1.0, 4.0, 15.0);

	// We'll translate backward...
	glTranslatef( 0.0f, 0.0f, -10.0f );
}

void GameWidget::SetTheme( vector< pair< string, string > > &theme )
{
	// Set textures in the ThemeManager up
	for( unsigned int i = 0; i < theme.size(); i++ )
		ThemeManager::Instance().AddTexture( theme[i].first, theme[i].second );
}

void GameWidget::SetAudioEvents( vector< pair< string, string > > &events )
{
	// Set Audio Events in the ThemeManager up
	for( unsigned int i = 0; i < events.size(); i++ )
		ThemeManager::Instance().AddAudioEvent( events[i].first, events[i].second, mVolume );
}

void GameWidget::SaveGLState()
{
	// Save the OpenGL state
	glPushAttrib(GL_ALL_ATTRIB_BITS);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
}

void GameWidget::RestoreGLState()
{
	// Re-load the saved OpenGL state
	glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glPopAttrib();
}

double GameWidget::TextWidth( QString & msg, QFont & font )
{
	QFontMetrics metrics( font );
	double w = metrics.width( msg );
	double two = ( ( width() < height() ) ? width() : height() );
	return 2.0 * w / two;
}

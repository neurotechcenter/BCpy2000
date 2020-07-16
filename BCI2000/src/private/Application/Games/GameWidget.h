////////////////////////////////////////////////////////////////////////////////
// $Id: GameWidget.h 3293 2011-05-30 01:09:57Z jhill $
// Authors: griffin.milsap@gmail.com
// Description: A QGLWidget based class which updates independent of BCI2000
//
// (C) 2000-2010, BCI2000 Project
// http://www.bci2000.org
////////////////////////////////////////////////////////////////////////////////
#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <qgl.h>
#include <QGLWidget>
#include <vector>

class GameWidget : public QGLWidget
{
public:
	GameWidget( QWidget* parent = 0, char* name = 0 );
	~GameWidget();

	void SetTheme( std::vector< std::pair< std::string, std::string > > &theme );
	void SetAudioEvents( std::vector< std::pair< std::string, std::string > > &events );
	void SetClearColor( QColor &color );
	void SetVolume( float volume );
	void SetRunning( bool running ) { mRunning = running; }
	bool Running() { return mRunning; }
	void SetGameTime( long GameTime ) { mGameTime = GameTime; }
	long GameTime() { return mGameTime; }

	virtual void initializeGL();
	virtual void resizeGL( int width, int height );
	virtual void paintGL();

	void SaveGLState();
	void RestoreGLState();
	double TextWidth( QString & msg, QFont & font );

protected:
	virtual void OnCreate() { }
	virtual void OnResize( int width, int height ) { }
	virtual void Update() = 0;
	virtual void SetProjection();

private:
	QColor mClearColor;
	float mVolume;
	bool mRunning;
	long mGameTime;
};

#endif // GAMEWIDGET_H

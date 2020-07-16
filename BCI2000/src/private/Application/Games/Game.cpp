////////////////////////////////////////////////////////////////////////////////
// $Id: Game.cpp 3660 2011-11-18 16:36:38Z gmilsap $
// Authors: griffin.milsap@gmail.com
// Description: Manages some default game parameters and theming
//
// (C) 2000-2010, BCI2000 Project
// http://www.bci2000.org
////////////////////////////////////////////////////////////////////////////////
#include "PCHIncludes.h"
#pragma hdrstop

#include "Game.h"
#include "../ThemeManager.h"
#include <algorithm>

using namespace std;

Game::Game() :
  mpGameWidget( NULL ),
  mGameTime( 0 ),
  mLastTime( 0 )
{
  // Declare Parameters and States here
  BEGIN_PARAMETER_DEFINITIONS
    "Application:Game%20Appearance int WindowWidth=  800 800 1 % // width of game window ",
	"Application:Game%20Appearance int WindowHeight= 600 600 1 % // height of game window ",
	"Application:Game%20Appearance int WindowLeft=     0   0 % % // horizontal position of game window ",
	"Application:Game%20Appearance int WindowTop=      0   0 % % // vertical position of game window ",

    "Application:Game matrix VideoTheme= 1 { type texture } 0   0      %    %   %    // Graphical theme for the game",
    "Application:Game matrix AudioTheme= 1 { event sound  } 0   0      %    %   %    // Audio theme for the game",
    "Application:Game float  AudioVolume=                   1.0        1.0  0.0 1.0  // Game sound volume",
	"Application:Game string ClearColor= 0x000000 0x000000 0x000000 0xFFFFFF         // Color to clear the gl screen to (color) ",
  END_PARAMETER_DEFINITIONS
}

Game::~Game()
{
  // Clean up any allocated memory
  if( mpGameWidget ) delete mpGameWidget;
  mpGameWidget = NULL;
}

void Game::Preflight( const SignalProperties& Input, SignalProperties& Output ) const
{
  // Preflight checking
  GameWidget* game = NewGameWidget();

  // Check the Clear color
  RGBColor color = ( RGBColor )Parameter( "ClearColor" );
  QColor qcolor = QColor( color.R(), color.G(), color.B() );
  game->SetClearColor( qcolor );

  // Set the window bounds
  game->move( Parameter( "WindowLeft" ), Parameter( "WindowTop" ) );
  game->resize( Parameter( "WindowWidth" ), Parameter( "WindowHeight" ) );

  // Set a Theme for the Game
  ParamRef videotheme = Parameter( "VideoTheme" );
  ParamRef audiotheme = Parameter( "AudioTheme" );

  // Parse out the video theme
  if( videotheme->NumColumns() != 2 )
    bcierr << "Invalid VideoTheme parameter" << endl;
  vector< pair< string, string > > theme;
  for( int i = 0; i < videotheme->NumRows(); i++ )
    theme.push_back( pair<string,string>( ( string )videotheme( i, 0 ), ( string )videotheme( i, 1 ) ) );
  game->SetTheme( theme );

  // Parse out the audio theme
  if( audiotheme->NumColumns() != 2 )
    bcierr << "Invalid AudioTheme parameter" << endl;
  vector< pair< string, string > > events;
  for( int i = 0; i < audiotheme->NumRows(); i++ )
    events.push_back( pair<string,string>( ( string )audiotheme( i, 0 ), ( string )audiotheme( i, 1 ) ) );
  game->SetAudioEvents( events );

  // Allow subclasses to do preflight checking on 
  OnPreflight( Input, game );

  // Apply the theme after Game has added objects
  ThemeManager::Instance().ApplyTheme( game );

  // Flush the theme if we have added objects and themes
  ThemeManager::Instance().FlushObjects();
  ThemeManager::Instance().FlushTheme();

  // Delete the Game Widget
  delete game;
  game = NULL;

  Output = Input;
}

void Game::Initialize( const SignalProperties& Input, const SignalProperties& Output )
{
  // Program initialization
  mpGameWidget = NewGameWidget(); // We'll do 60 FPS
  mpGameWidget->SetRunning( true );

  // Check the Clear color
  RGBColor color = ( RGBColor )Parameter( "ClearColor" );
  QColor qcolor = QColor( color.R(), color.G(), color.B() );
  mpGameWidget->SetClearColor( qcolor ); 

  // Set the window bounds
  mpGameWidget->move( Parameter( "WindowLeft" ), Parameter( "WindowTop" ) );
  mpGameWidget->resize( Parameter( "WindowWidth" ), Parameter( "WindowHeight" ) );

  // Grab the theme data
  ParamRef videotheme = Parameter( "VideoTheme" );
  ParamRef audiotheme = Parameter( "AudioTheme" );

  // Parse out the video theme
  vector< pair< string, string > > theme;
  for( int i = 0; i < videotheme->NumRows(); i++ )
    theme.push_back( pair<string,string>( ( string )videotheme( i, 0 ), ( string )videotheme( i, 1 ) ) );
  mpGameWidget->SetTheme( theme );

  // Parse out the audio theme
  mpGameWidget->SetVolume(Parameter("AudioVolume"));
  vector< pair< string, string > > events;
  for( int i = 0; i < audiotheme->NumRows(); i++ )
    events.push_back( pair<string,string>( ( string )audiotheme( i, 0 ), ( string )audiotheme( i, 1 ) ) );
  mpGameWidget->SetAudioEvents( events );

  // Feed the current ...  uh.
  OnInitialize( Input );

  // Show the widget
  mpGameWidget->show();
  mpGameWidget->SetRunning( false );
  mGameTime = 0;
}

void Game::Process( const GenericSignal& Input, GenericSignal& Output )
{
  // Process things and send input to the GLWidget
  if( Widget()->Running() )
  {
	mGameTime += 1000 * SecondsPerPacket();
	//mGameTime += PrecisionTime::TimeDiff( mLastTime, PrecisionTime::Now() );
	mLastTime = PrecisionTime::Now();
  }
  mpGameWidget->SetGameTime( mGameTime );
  OnProcess( Input );
  mpGameWidget->updateGL();
  State( "StimulusTime" ) = PrecisionTime::Now();
}

void Game::StartRun()
{
  // Start the Game
  mLastTime = PrecisionTime::Now();
  Widget()->SetRunning( true );
  OnStartRun();
  mpGameWidget->updateGL();
}

void Game::StopRun()
{
  // Stop the Game
  OnStopRun();
  mpGameWidget->updateGL();
  Widget()->SetRunning( false );
}

void Game::Halt()
{
  // Halt the game
  OnHalt();
  if( mpGameWidget )
  {
	  mpGameWidget->hide();
	  delete mpGameWidget;
  }
  mpGameWidget = NULL;

  // Flush the theme if we have added objects and themes
  ThemeManager::Instance().FlushObjects();
  ThemeManager::Instance().FlushTheme();

  // Reset the Game timer
  mGameTime = 0;
}



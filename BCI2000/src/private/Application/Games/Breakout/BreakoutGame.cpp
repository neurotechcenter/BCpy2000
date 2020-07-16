////////////////////////////////////////////////////////////////////////////////
// $Id: BreakoutGame.cpp 3660 2011-11-18 16:36:38Z gmilsap $
// Authors: griffin.milsap@gmail.com
// Description: A breakout clone implemented as an application module
//
// (C) 2000-2010, BCI2000 Project
// http://www.bci2000.org
////////////////////////////////////////////////////////////////////////////////
#include "PCHIncludes.h"
#pragma hdrstop

#include "BreakoutGame.h"
#include <algorithm>

RegisterFilter( BreakoutGame, 3 );

using namespace std;

BreakoutGame::BreakoutGame()
{
  // Declare Parameters and States here
  BEGIN_PARAMETER_DEFINITIONS

	"Application:Game%20Appearance matrix Level= "
      "5 16 " // Rows, columns
      "1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 "
      "1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 "
      "1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 "
      "1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 "
      "1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 "
      "0 % % "
      "// Define block placement on breakout level ",

	"Application:Calibration matrix  Prompts= 2  { TargetCode String }  1  Relax     2 Imagine%20Moving  % % % // calibration prompt strings and corresponding TargetCode values",
	"Application:Calibration intlist PromptSequence= 0  % 1 % // sequence of stimulus codes to present as prompts for calibration",
	"Application:Calibration float   PromptDuration= 5s  % 0 % // duration of prompts",
	"Application:Calibration float   ISI= 1.5s  % 0 % // delay between prompts",
	"Application:Calibration float   FeedbackStateDelay= 500ms  % 0 % // delay between setting TargetCode state and Feedback state",
	"Application:Calibration int     FeedbackVisibleAfter= 4  4 0 % // number of trials to perform without visible feedback",

	"Application:Game float VariantProportion= 0.10 0.0 0.0 0.5 // proportion of each eligible sound type to vary",

	"Application:Game%20Appearance string TextColor= 0x000000 0x000000 0x000000 0xFFFFFF "
	  "// Color for in game scoring text (color) ",

	"Application:Game%20Appearance int TextSize= 13 13 6 % "
	  "// Size for in game scoring text ",

	"Application:Game%20Control string PaddleExpressionX= % "
      "// Expression for X position of the paddle (-1:1) ",

	"Application:Game%20Control int IntegratePaddleX= 1 1 0 1 "
      "// check to control x velocity, uncheck to control absolute x position (boolean)  ",

	"Application:Game%20Control string PaddleExpressionY= % "
      "// Expression for Y position of the paddle (-1:1) ",

	"Application:Game%20Control int IntegratePaddleY= 1 1 0 1 "
      "// check to control y velocity, uncheck to control absolute y position (boolean)  ",

	"Application:Game%20Challenge float MinSpawnTime=            1.0   1.0    0.0 %   // Minimum duration in seconds of a respawned path  ",
	"Application:Game%20Challenge float MaxSpawnTime=            1.0   1.0    0.0 %   // Maximum duration in seconds of a respawned path  ",

	"Application:Game%20Challenge float PaddleHeight=            0.01  0.01   0.0 2.0 // Height of the paddle rectangle (0-2) ",

    "Application:Game%20Challenge float PaddleWidth=             0.3   0.3    0.0 2.0 // Width of the paddle rectangle at GameDifficulty=0 (0-2) ",

	"Application:Game%20Challenge float PaddleWidthHitFactor=    0.90  0.90   0.0 %   // Multiplier for the paddle width corresponding to a unit increase in GameDifficulty ",

	"Application:Game%20Challenge float BallRadius=              0.05  0.05   0.0 2.0 // Radius of the ball in game at GameDifficulty=0 (0-2)",

	"Application:Game%20Challenge float BallRadiusHitFactor=     0.95  0.95   0.0 %   // Multiplier for the ball radius corresponding to a unit increase in GameDifficulty ",

	"Application:Game%20Challenge float BallSpeed=               1.5   1.5    0.0 %   // Ball speed (distance per second) at GameDifficulty=0 ",

	"Application:Game%20Challenge float BallSpeedHitFactor=      1.04  1.04   0.0 %   // Multiplier for the ball speed corresponding to a unit increase in GameDifficulty ",

	"Application:Game%20Challenge float InitialGameDifficulty=   0.0   0.0    %   %   // Starting point for staircase GameDifficulty ",
	"Application:Game%20Challenge float StepSizeOnHit=           1.0   1.0    0.0 %   // Increase in GameDifficulty per hit",
	"Application:Game%20Challenge float TargetAccuracy=          0.82  0.82   0.0 1.0 // Weighted-up-down staircase's target proportion of hits",

	"Application:Game%20Challenge int   ShowPredictor=           1     1      0   1   // Show a guide (boolean) ",

	"Application:Game%20Rewards   int PointsRequiredForReward=   4     4      1   %   // Number of eligible points required for a reward (gold coin)  ",
	"Application:Game%20Rewards   int MaxPointsPerStroke=        2     2      0   %   // Maximum number of points that can be earned from striking the ball once ",

	"Application:Game%20Rewards   matrix RewardStrings= 7 2  0 Better%20luck%20next%20time 1 Not%20bad... 2 Pretty%20good! 3 Great!! 4 Excellent!! 5 Amazing!! 6 Fantastic!!!   % % % // Mapping from number-of-rewards to feedback strings",

  END_PARAMETER_DEFINITIONS

  BEGIN_STATE_DEFINITIONS
   "Hits                          16 0 0 0",
   "Misses                        16 0 0 0",
   "Reversals                     16 0 0 0",
   "FutureBounces                 16 0 0 0",
   "JustHit                        1 0 0 0",
   "JustMissed                     1 0 0 0",
   "BallAngle                      9 0 0 0",
   "BallSpeedTimes10000           16 0 0 0",
   "BallRadiusTimes10000          14 0 0 0",
   "BallXPosTimes10000            14 0 0 0",
   "BallYPosTimes10000            14 0 0 0",
   "PaddleWidthTimes10000         14 0 0 0",
   "PaddleXPosTimes10000          14 0 0 0",
   "PaddleYPosTimes10000          14 0 0 0",
   "LeftMotionRequiredTimes10000  14 0 0 0",
   "RightMotionRequiredTimes10000 14 0 0 0",
   "BrickSound                     2 0 0 0",
   "PaddleSound                    2 0 0 0",

   "Feedback                       1 0 0 0",
   "TargetCode                     3 0 0 0",
  END_STATE_DEFINITIONS

}

BreakoutGame::~BreakoutGame()
{
  // TODO: Clean up any allocated memory
}


string BreakoutGame::ParamLookup(string paramName, string key) const
{
	ParamRef p = Parameter(paramName);
	if(p->NumRows()>0 && p->NumColumns() != 2) {
		bcierr << "Parameter " << paramName << " is expected to have 2 columns" << endl;
	}
	else {
		for(int i = 0; i < p->NumRows(); i++) {
			if(key == (string)p(i, 0)) return p(i, 1);
		}
		bcierr << "Parameter " << paramName << " has no \"" << key << "\" entry" << endl;
	}
	return string();
}

void BreakoutGame::OnPreflight( const SignalProperties& Input, GameWidget* widget ) const
{
  // Check the paddle expressions
  Expression pExpX, pExpY;
  GenericSignal preflightInput( Input );

  pExpX = Expression( Parameter( "PaddleExpressionX" ) );
  pExpX.Evaluate( &preflightInput );

  pExpY = Expression( Parameter( "PaddleExpressionY" ) );
  pExpY.Evaluate( &preflightInput );

  GameSetup(widget, 0.0f, -0.7f);

  State( "Running" );
  State( "JustHit" );
  State( "Hits" );
  State( "JustMissed" );
  State( "Misses" );
  State( "Reversals" );
  State( "FutureBounces" );
  State( "PaddleWidthTimes10000" );
  State( "PaddleXPosTimes10000" );
  State( "PaddleYPosTimes10000" );
  State( "BallXPosTimes10000" );
  State( "BallYPosTimes10000" );
  State( "BallAngle" );
  State( "BallSpeedTimes10000" );
  State( "BallRadiusTimes10000" );
  State( "LeftMotionRequiredTimes10000");
  State( "RightMotionRequiredTimes10000");
  State( "BrickSound" );
  State( "PaddleSound" );
  State( "StimulusTime" );

  ParamLookup("VideoTheme", "Block");
  ParamLookup("VideoTheme", "Ball");
  ParamLookup("VideoTheme", "Paddle");
  ParamLookup("VideoTheme", "Background");
  ParamLookup("VideoTheme", "Reward");
  ParamLookup("VideoTheme", "Ineligible");
  ParamLookup("AudioTheme", "HitBlock");
  ParamLookup("AudioTheme", "HitPaddle");
  ParamLookup("AudioTheme", "HitSideOfPaddle");
  ParamLookup("AudioTheme", "MissPaddle");
  ParamLookup("AudioTheme", "Reward");
  ParamLookup("AudioTheme", "HitBlockVariant");
  ParamLookup("AudioTheme", "HitPaddleVariant");

  ParamRef pRewardStrings = Parameter("RewardStrings");
  if(pRewardStrings->NumRows() > 0 && pRewardStrings->NumColumns() != 2)
	  bcierr << "Parameter \"RewardStrings\" must have 2 columns" << endl;
  for(int i = 0; i < pRewardStrings->NumRows(); i++)
  {
    double d = pRewardStrings(i, 0);
	if(d < 0.0 || d > 100.0)
		bcierr << "Parameter \"RewardStrings\" should contain values from 0 to 100 in the first column" << endl;
  }
}

void BreakoutGame::OnInitialize( const SignalProperties& Input )
{
  // Grab the expressions used later for paddle movement
  mPaddleExpX = Expression( Parameter( "PaddleExpressionX" ) );
  mPaddleExpY = Expression( Parameter( "PaddleExpressionY" ) );

  // Principle of least surprise: changing SampleBlockSize should not send ripples through your parameters.
  // For velocity control, the experimenter should feel free to calibrate the PaddleExpression magnitudes
  // once and for all in a way that is SampleBlockSize - independent,  i.e.  the PaddleExpression outputs
  // should be interpreted as metres per second (m/s).   The step we add to X on any given single Process()
  // call, let's call it DX,  is going to need to be in metres per packet (m/p). Therefore we will be
  // multiplying:  DX (m/p) = expression output (m/s)  *  packet duration (s/p)
  mDXmultiplier = (Parameter( "IntegratePaddleX" ) == 0) ? 0.0 : SecondsPerPacket();
  mDYmultiplier = (Parameter( "IntegratePaddleY" ) == 0) ? 0.0 : SecondsPerPacket();

  GenericSignal initialInput( Input );
  BreakoutWidget* w = GameSetup(Widget(),
    mDXmultiplier ? 0.0 : mPaddleExpX.Evaluate( &initialInput ),
    mDYmultiplier ? 0.0 : mPaddleExpY.Evaluate( &initialInput ) );

}

int BreakoutGame::ReadAsWholeNumberOfSampleBlocks( string paramName ) const
{
  double val_f = Parameter( paramName ).InSampleBlocks();
  int val_i = int( 0.5 + val_f );
  if( val_f != (double)val_i ) bciout << paramName << " was rounded from " << val_f << " to " << val_i << " SampleBlocks" << endl;
  return val_i;
}

BreakoutWidget* BreakoutGame::GameSetup(GameWidget *widget, float paddleX, float paddleY) const
{
  // Load the level and prepare things
  BreakoutWidget* w = dynamic_cast< BreakoutWidget* >( widget );

  // Game difficulty parameters
  w->InitSpawnTime(   Parameter( "MinSpawnTime" ) * PacketsPerSecond(),  Parameter( "MaxSpawnTime" ) * PacketsPerSecond() );
  w->SetPaddleHeight( Parameter( "PaddleHeight" ) );
  w->InitPaddleWidth( Parameter( "PaddleWidth" ), Parameter( "PaddleWidthHitFactor" ) );
  w->InitBallRadius(  Parameter( "BallRadius" ),  Parameter( "BallRadiusHitFactor" ) );
  w->InitBallSpeed(   Parameter( "BallSpeed" ) * SecondsPerPacket(),   Parameter( "BallSpeedHitFactor" ) );
  w->InitGameDifficulty( Parameter( "TargetAccuracy" ), Parameter( "InitialGameDifficulty" ), Parameter( "StepSizeOnHit" ) );
  w->InitRewards( Parameter("PointsRequiredForReward"), Parameter("MaxPointsPerStroke"));
  w->ShowPredictor( (int)Parameter( "ShowPredictor" ) );
  w->SetVariantProportion( (float)Parameter( "VariantProportion" ) );

  // Text color and size
  RGBColor color = ( RGBColor )Parameter( "TextColor" );
  w->SetTextColor( QColor( color.R(), color.G(), color.B() ) );
  w->SetTextSize( Parameter( "TextSize" ) );

  // Level loading
  vector< vector< double > > level;
  for( int i = 0; i < Parameter( "Level" )->NumRows(); i++ )
  {
	vector< double > row;
    for( int j = 0; j < Parameter( "Level" )->NumColumns(); j++ )
	{
	  ParamRef p = Parameter( "Level" )( i, j );
	  double pval = (((string)p).size() ? (double)p : 0);
	  row.push_back( pval );
	}
	level.push_back( row );
  }
  // Reward strings
  for( int i = 0; i < Parameter( "RewardStrings" )->NumRows(); i++ )
    w->SetRewardString(Parameter("RewardStrings")(i,0), Parameter("RewardStrings")(i,1));

  // Calibration prompts
  State( "TargetCode" );
  State( "Feedback" );
  vector<string> prompts;
  if( Parameter( "Prompts" )->NumRows() > 0 && Parameter( "Prompts" )->NumColumns() != 2 ) bcierr << "Prompts parameter should have 2 columns" << endl;
  for( int i = 0; i < Parameter( "Prompts" )->NumRows(); i++ )
  {
    ParamRef p = Parameter( "Prompts" )( i, 0 );
    double indf = p;
	if( indf < 1.0 || indf != floor( indf ) ) bcierr << "illegal item \"" << string( p ) << "\" in the first column of the Prompts parameter (values here should be integers > 0)" << endl;
	unsigned int ind = (unsigned int)( indf+0.5 );
	while( prompts.size() < ind ) prompts.push_back( "" );
	if( prompts[ ind-1 ].size() ) bcierr << "duplicate entry for TargetCode " << ind << " in Prompts parameter " << endl;
	prompts[ ind-1 ] = static_cast< string >( Parameter( "Prompts" )( i, 1 ) );
  }
  vector<unsigned int> promptSequence;
  for( int i = 0; i < Parameter( "PromptSequence" )->NumValues(); i++ )
  {
    int val = Parameter( "PromptSequence" )( i );
	if( val < 1 || val > int(prompts.size()) || prompts[val-1].size() == 0 ) bcierr << "entry " << val << " in the PromptSequence parameter is illegal because it does not have a corresponding TargetCode value in the Prompts parameter" << endl;
	promptSequence.push_back( val );
  }
  int promptDuration = ReadAsWholeNumberOfSampleBlocks( "PromptDuration" );
  int isi = ReadAsWholeNumberOfSampleBlocks( "ISI" );
  int feedbackStateDelay = ReadAsWholeNumberOfSampleBlocks( "FeedbackStateDelay" );
  int feedbacklessTrials = Parameter( "FeedbackVisibleAfter" );
  w->InitCalibration( prompts, promptSequence, promptDuration, isi, feedbackStateDelay, feedbacklessTrials );

  // get ready to start
  w->InitGame( level, paddleX, paddleY );

  return w;
}
void BreakoutGame::SetState(const char *name, int val)
{
	/*
	// argh argh argh, why am i being forced to do this??
	//  why is the error message accompanying the fatal error so uninformative in the first place, so that I have to wrap the assignment
	//  and value-check state value assignments manually??
	//  secondly, why isn't looking up a state's bit-precision by name easy/obvious?
	int bits = 0;
	for( int i = 0; i < States->Size(); ++i ) {
		if( ( *States )[ i ].Name() == name ) { bits = (*States)[i].Length(); break; }
	}
	long limit = (1L<<bits) - 1;
	if( val < 0 || val > limit )
		bciout << "illegal attempt to set " << bits << "-bit state " << name << " (legal range 0 to " << limit << ") to " << val << endl;

	if( val < 0) val = 0;
	if( val > limit) val = limit;
	/**/
	State(name) = val;
}
void BreakoutGame::OnProcess( const GenericSignal& Input )
{
  // Send new paddle position to the widget
  BreakoutWidget* w = dynamic_cast< BreakoutWidget* >( Widget() );

  float x,y;
  w->PaddlePosition(x,y);
  if (mDXmultiplier) x += mPaddleExpX.Evaluate( &Input ) * mDXmultiplier;
  else               x =  mPaddleExpX.Evaluate( &Input );
  if (mDYmultiplier) y += mPaddleExpY.Evaluate( &Input ) * mDYmultiplier;
  else               y =  mPaddleExpY.Evaluate( &Input );
  w->SetPaddlePosition( x, y );
  w->SetRandomNumber( mRandGen(10000) / 10000.0 ); // TODO: need more satisfactory solution

  // Grab game state
  SetState( "JustHit", (int)(State( "Hits" ) < w->Hits()) );
  SetState( "Hits", w->Hits() );
  SetState( "JustMissed", (int)(State( "Misses" ) < w->Misses()) );
  SetState( "Misses", w->Misses() );
  SetState( "Reversals", w->Reversals() );
  SetState( "FutureBounces", w->FutureBounces() );
  SetState( "BallAngle",             (unsigned int)( 0.5 + w->BallAngle() ) );


  // scale the horizontal extent of the board [-1, +1] between 0 and 10000
  // for x positions:            x -> (x+1) * 5000
  // for x velocities and sizes: x ->  x * 5000
  SetState( "PaddleWidthTimes10000", (unsigned int)( 0.5 + 5000 * w->PaddleWidth() ) );
  w->PaddlePosition(x, y);
  SetState( "PaddleXPosTimes10000",  (unsigned int)( 0.5 + 5000 * max(0.0,min(2.0,x+1.0)) ) );
  SetState( "PaddleYPosTimes10000",  (unsigned int)( 0.5 + 5000 * max(0.0,min(2.0,y+1.0)) ) );
  w->BallPosition(x, y);
  SetState( "BallXPosTimes10000",    (unsigned int)( 0.5 + 5000 * max(0.0,min(2.0,x+1.0)) ) );
  SetState( "BallYPosTimes10000",    (unsigned int)( 0.5 + 5000 * max(0.0,min(2.0,y+1.0)) ) );
  SetState( "BallSpeedTimes10000",   (unsigned int)( 0.5 + 5000 * w->BallSpeed() * PacketsPerSecond() ) );
  SetState( "BallRadiusTimes10000",  (unsigned int)( 0.5 + 5000 * w->BallRadius() ) );
  SetState( "LeftMotionRequiredTimes10000",  (unsigned int) (0.5 + 5000 * w->LeftMotionRequired() ) );
  SetState( "RightMotionRequiredTimes10000", (unsigned int) (0.5 + 5000 * w->RightMotionRequired() ) );
  SetState( "BrickSound", w->BrickSoundStatus() );
  SetState( "PaddleSound", w->PaddleSoundStatus() );

  SetState( "TargetCode", w->Calibration().TargetCode() );
  SetState( "Feedback",   w->Calibration().Feedback() );

  // If the game has overed, we need to stop the current run.
  if( w->GameOver() ) State( "Running" ) = 0;
}

void BreakoutGame::OnStartRun()
{
  // Start the Game
  //if( Widget() )
  //  Widget()->SetClearColor( QColor( 0, 255, 0 ) );
  BreakoutWidget* w = dynamic_cast< BreakoutWidget* >( Widget() );
  w->Calibration().Reset();
}

void BreakoutGame::OnStopRun()
{
  // Stop the Game
  //if( Widget() )
  //  Widget()->SetClearColor( QColor( 255, 0, 0 ) );
}

void BreakoutGame::OnHalt()
{
  // Halt the game
}



////////////////////////////////////////////////////////////////////////////////
// $Id: BreakoutWidget.cpp 4981 2015-08-26 17:04:38Z mellinger $
// Authors: griffin.milsap@gmail.com
// Description: A GameWidget which implements Breakout gameplay
//
// (C) 2000-2010, BCI2000 Project
// http://www.bci2000.org
////////////////////////////////////////////////////////////////////////////////
#include "MeasurementUnits.h"

#include "BreakoutWidget.h"
#include "../ThemeManager.h"
#include <math.h>

using namespace std;
#define report(x) #x<<"="<<x<<"  "

RewardState::RewardState()
{
	mPoints = 0.0;
	mPointsThisTime = 0.0;
	mMaxPointsPerStrike = 0.0;
	mQualifyingDifficulty = 0.0;
	mPointsRequired = 3.0;
	mAchieved = 0.0;
}
RewardState::~RewardState()
{
}

std::string CalibrationState::Prompt() const
{
	if( !Running() ) return "";
	if( mPostCalibrationPause ) return "Ready to play...";
	if( mTargetCode && mPrompts.size() ) return mPrompts[ (mTargetCode-1) % mPrompts.size() ];
	return "";
}
void CalibrationState::Update()
{
	//if( mTrialNumber == 0 ) { mTrialNumber = 1; mCount = 0; mTargetCode = 0; mFeedback = 0; mPostCalibrationPause = 0; }
	if( !Running() ) { mCount = 0; mTargetCode = 0; mFeedback = 0; mPostCalibrationPause = 0; return; }

	if( mTrialNumber == 0 )
	{
		mFeedback = 0;
		mTargetCode = 0;
		if( mCount >= MeasurementUnits::TimeInSampleBlocks( "4s" ) )
		{
			mCount = 0;
			if( ++mTrialNumber > mPromptSequence.size() )
				mPostCalibrationPause = 1;
		}
	}
	else if( mPostCalibrationPause )
	{
		mFeedback = 0;
		mTargetCode = 0;
		if( mCount >= MeasurementUnits::TimeInSampleBlocks( "2s" ) )
		{
			mCount = 0;
			mPostCalibrationPause = 0;
		}
	}
	else if( mTargetCode )
	{
		mFeedback = ( mCount >= mFeedbackStateDelay );
		if( mCount >= mPromptDuration )
		{
			mCount = 0;
			mTargetCode = 0;
			mFeedback = 0;
			if( ++mTrialNumber > mPromptSequence.size() )
				mPostCalibrationPause = 1;
		}
	}
	else
	{
		mFeedback = 0;
		if ( mCount >= mISI )
		{
			mCount = 0;
			mTargetCode = mPromptSequence[ (mTrialNumber - 1) % mPromptSequence.size() ];
		}
	}
	mCount++;
}




BreakoutWidget::BreakoutWidget( QWidget* parent, char* name )
: GameWidget( parent, name )
{
	// Set up game variables
	mVirgin = true;

	mHits = 0;
	mMisses = 0;
	mReversals = 0;
	mEnding = false;

	mPaddleWidthBaseValue  = 0.3;
	mPaddleWidthHitFactor  = 1.0;

	mBallSpeedBaseValue    = 1.5;
	mBallSpeedHitFactor    = 1.0;

	mBallRadiusBaseValue   = 0.05;
	mBallRadiusHitFactor   = 1.0;

	mTargetAccuracy = 0.75;
	mGameDifficulty = 0.0;
	mStepSize = 1.0;
	mLastStep = 0.0;

	mTextSize = 13;

	mRespawnX = 0; mRespawnDX = 0;
	mRespawnY = 0; mRespawnDY = 0;

	mMinSpawnTime = 20;
	mMaxSpawnTime = 20;

	// Add the background to the Theme Manager
	ThemeManager::Instance().AddObject( &mBackground, "Background" );
	ThemeManager::Instance().AddObject( &mRewardIcon, "Reward" );
	ThemeManager::Instance().AddObject( &mNoRewardIcon, "Ineligible" );

	mTriggeredPaddleSound = false;
	mTriggeredBrickSound = false;
	mVaryingPaddleSound = false;
	mVaryingBrickSound = false;
	mEligibleToVary = false;
	mVariantProportion = 0.0; // TODO: parameterize this

	setCursor(Qt::BlankCursor);
}

BreakoutWidget::~BreakoutWidget()
{
	// Delete any dynamically allocated memory
}

void BreakoutWidget::InitGame( vector< vector< double > > &level, float paddleX, float paddleY )
{
	// Create the theme and the level
	mBlockArray.LoadLevel( level );
	mBackground.SetExtent( -1.0f, -1.0f, 2.0f, 2.0f );
	mBackground.SetZ( -1.0f );
	mPaddle.SetPosition(paddleX, paddleY);
	mBall.SetPosition( 0.0f, -0.8f );
	// mBall.SetPosition( mPaddle.X(), mPaddle.Y() + mPaddle.GetHeight()/2 + mBall.Radius() + 0.001 );  // TODO:  ball never appears.... WHY??
	mBall.SetDirection( 0.707f, 0.707f );
	mVirgin = true;
}

void BreakoutWidget::InitGameDifficulty(float targetAccuracy, float gameDifficulty, float stepSize)
{
	mTargetAccuracy = targetAccuracy;
	mGameDifficulty = gameDifficulty;
	mStepSize = stepSize;
	AdjustGameDifficulty(0.0);
	mLastStep = 0.0;
}

void BreakoutWidget::InitRewards(float pointsRequired, float maxPointsPerStrike)
{
	mRewards.mPoints = 0.0;
	mRewards.mPointsThisTime = 0.0;
	mRewards.mMaxPointsPerStrike = maxPointsPerStrike;
	mRewards.mQualifyingDifficulty = mGameDifficulty;
	mRewards.mPointsRequired = pointsRequired;
	mRewards.mAchieved = 0.0;
}

void BreakoutWidget::InitCalibration( std::vector<std::string> prompts, std::vector<unsigned int> promptSequence, int promptDuration, int isi, int feedbackStateDelay, int feedbacklessTrials )
{
	mCalibration.Init( prompts, promptSequence, promptDuration, isi, feedbackStateDelay, feedbacklessTrials );
}

double BreakoutWidget::ProcessRewardPoints(double points)
{
	if( mGameDifficulty < mRewards.mQualifyingDifficulty ) // you don't earn points at all unless you're playing at least at the difficulty level of your last reward
		return 0.0;                                        //    (this is to prevent you beating the system by missing deliberately a few times to make things easier)

	if( points > mRewards.mMaxPointsPerStrike - mRewards.mPointsThisTime ) // you can only earn so many points per strike (bouncing around hitting multiple blocks *may* be a sign of skill, but more likely luck)
		points = mRewards.mMaxPointsPerStrike - mRewards.mPointsThisTime;  //    therefore it is possible to parameterize things so that >1 consecutive hits are definitely required to earn a reward
	                                                              //    (they would have to be consecutive, since points are also zeroed when you miss: again to prevent you beating the system)

	if( points > 0.0 && mBlockArray.BricksLeft() == 0 ) points = mRewards.mPointsRequired;  // bonus reward if you manage to finish the level at the difficulty level of your last reward or higher, irrespective of points accumulated

	mRewards.mPoints += points;
	mRewards.mPointsThisTime += points;
	if( mRewards.mPoints >= mRewards.mPointsRequired )
	{
		mRewards.mPoints = 0.0;
		mRewards.mQualifyingDifficulty = mGameDifficulty;
		mRewards.mAchieved += 1.0;
		return 1.0;
	}
	return 0.0;
}

int BreakoutWidget::AdjustGameDifficulty(double step)
{
	int result = 0;

	double newDifficulty  = mGameDifficulty + step;
	double newPaddleWidth = mPaddleWidthBaseValue * pow(mPaddleWidthHitFactor, newDifficulty);
	double newBallSpeed   = mBallSpeedBaseValue   * pow(mBallSpeedHitFactor,   newDifficulty);
	double newBallRadius  = mBallRadiusBaseValue  * pow(mBallRadiusHitFactor,  newDifficulty);

	if( newPaddleWidth < 0.01 || newPaddleWidth > 1.75 || newBallRadius < 0.02 || newBallRadius > 0.2 )
		return result; // avoid fragmentation of the staircase: only take a step in any dimension if all dimensions are achievable

	mPaddle.SetWidth( newPaddleWidth );
	mBall.SetSpeed( newBallSpeed );
	mBall.SetRadius( newBallRadius );
	mGameDifficulty = newDifficulty;

	//bciout << report(mGameDifficulty) << report(mBall.Radius()) << endl;
	if(mLastStep * step < 0.0) result = ++mReversals;
	mLastStep = step;
	return result;
}
void BreakoutWidget::Step(EventReport &events)
{
	int nhits = 0, previousHits = events.mHitBrick;
	bool already_hit_paddle = false;

	// Check if the ball and paddle are already colliding
	if( mPaddle.Collide( mBall ) ) already_hit_paddle = true; // prevents checking for strikes that have already happened

	// Move the ball in just the X direction and check for brick collisions
	mBall.MoveX();
	if( mBall.Solid() && (nhits = mBlockArray.Collide( mBall )) > 0 )
	{
		mBall.SetDirection( -1.0f * mBall.DX(), mBall.DY() );
		events.mHitBrick += nhits;
	}
	// Check for unsuccessful paddle strike (off the side)
	if( !already_hit_paddle && mPaddle.Collide( mBall ) )
	{
		mBall.SetDirection( -1.0f * mBall.DX(), mBall.DY() );
		already_hit_paddle = true; // prevents further checking for successful paddle strike
		events.mHitPaddleSide++;
	}

	// Check for wall bounce
	double xdir = (mBall.DX() < 0) ? -1.0 : (mBall.DX() > 0) ? +1.0 : 0.0;
	if( xdir && mBall.X() * xdir > 1.0f-mBall.Radius() )
	{
		mBall.SetDirection( -1.0f * mBall.DX(), mBall.DY() );
		events.mHitWall++;
	}

	// Move the ball in the Y direction and check again for brick collisions
	mBall.MoveY();
	if( mBall.Solid() && (nhits = mBlockArray.Collide( mBall )) > 0 )
	{
		mBall.SetDirection( mBall.DX(), -1.0f * mBall.DY() );
		events.mHitBrick += nhits;
	}

	// Check for ceiling bounce
	if( mBall.Y() > 1.0f-mBall.Radius() && mBall.DY() > 0 )
	{
		mBall.SetDirection( mBall.DX(), -1.0f * mBall.DY() );
		events.mHitCeiling++;
	}

	// Check for successful paddle strike
	if( !already_hit_paddle && mPaddle.Collide( mBall ) )
	{
		// Calculate and set a direction for the ball to go
		float dist = ( ( mBall.X() - mPaddle.X() ) * 2 ) / mPaddle.Width();
		if( dist > 0.9f ) dist = 0.9f;
		if( dist < -0.9f ) dist = -0.9f;
		float angle = asin( dist );
		//bcidbg( 0 ) << "Angle: " << angle << ", X = " << sin( angle ) << ", Y = " << cos( angle ) << std::endl;
		mBall.SetDirection( sin( angle ), cos( angle ) );
		already_hit_paddle = true;
		events.mHitPaddleTop++;
	}

	// Check for miss
	if( mBall.Y() < -1.0f && mBall.DY() < 0 ) events.mMiss++;

	int bricksHitThisTime = events.mHitBrick - previousHits;
	if( bricksHitThisTime ) events.mStepsWhereBricksWereHit++;
	events.mWinReward += ProcessRewardPoints( bricksHitThisTime );
	events.mSteps++;
}
#define Future(x) (max(0, mExpected.x - mSoFar.x))
int BreakoutWidget::FutureSteps() { return Future(mSteps); }
int BreakoutWidget::FutureBounces() { return Future(mStepsWhereBricksWereHit) + Future(mHitCeiling) + Future(mHitWall); }
int BreakoutWidget::FutureWallBounces() { return Future(mHitWall); }
int BreakoutWidget::FutureBrickBounces() { return Future(mStepsWhereBricksWereHit); }

#define NewEvents(x) (mSoFar.x-previous.x)
void BreakoutWidget::Update()
{
	// Draw the background
	mBackground.Update();

	if( Running() && mCalibration.Running() )
	{

		mCalibration.Update();
		if( mCalibration.FeedbackVisible() ) mPaddle.Update();
		string prompt = mCalibration.Prompt();
		if( prompt.size() )
		{
			QFont font( "Comic Sans MS", mTextSize*3 );
			QString msg( prompt.c_str() );
			qglColor( mTextColor );
			renderText( -0.5*TextWidth( msg, font ), -0.40, 0.0, msg, font );
		}
	}
	else if( Running() ) // Perform object updating if game is running
	{
		// Check for end condition
		if( mBlockArray.BricksLeft() == 0 )
			GameOver( true );


		mBlockArray.SaveStatus();
		EventReport previous = mSoFar;
		Step(mSoFar);

		if( NewEvents(mMiss) )
		{
			ThemeManager::Instance().AudioEvent( "MissPaddle" );
			mMisses++;
			mRewards.mPoints = mRewards.mPointsThisTime = 0.0; // lose all your points
			AdjustGameDifficulty( -mStepSize * mTargetAccuracy / (1.0-mTargetAccuracy) ); // WUD, Kaernbach 1991
			NewBall(false);
		}

		mTriggeredBrickSound = false;
		if( NewEvents(mHitBrick) )
		{
			mTriggeredBrickSound = true;
			if( mVaryingBrickSound )
				ThemeManager::Instance().AudioEvent( "HitBlockVariant" );
			else
				ThemeManager::Instance().AudioEvent( "HitBlock" );
			mBlockArray.Purge();
		}

		if( NewEvents(mHitBrick) ||
			NewEvents(mHitCeiling) ||
			(NewEvents(mHitWall) && FutureSteps() >= mMinSpawnTime) ||
			(mBall.Solid() && FutureBrickBounces() == 0 && FutureSteps() >= mMaxSpawnTime) )
		{
			SetBallSpawn();
		}

		if( NewEvents(mWinReward) )
		{
			ThemeManager::Instance().AudioEvent( "Reward" );
		}

		if( NewEvents(mHitPaddleSide) )
		{
			ThemeManager::Instance().AudioEvent( "HitSideOfPaddle" );
		}

		mTriggeredPaddleSound = false;
		if( NewEvents(mHitPaddleTop) )
		{
			mHits++;
			mBall.SetSolid(true);
			AdjustGameDifficulty( +mStepSize );
			mRewards.mPointsThisTime = 0.0;
			mTriggeredPaddleSound = true;
		}

		mPaddle.Update();
		mBall.Update();

		if( NewEvents(mHitPaddleTop) || NewEvents(mMiss) || mVirgin )
		{
			Predict();
		}
		if(mShowPredictor) renderText( mPredictedX, mPredictedY - mBall.Radius()*0.5, 0.0, QString("*" ), QFont("Comic Sans MS", 10) );
		// (the 0.5 * Radius is a fudge: it marks a point that it *looks* like you should aim for rather than a point the ball centre will actually pass through);
		// TODO: predictor mode in which it is only shown after the first miss (when ball is not solid)

		if( mTriggeredPaddleSound )
		{
			mEligibleToVary = mExpected.mStepsWhereBricksWereHit == 1 && mExpected.mWinReward == 0; // no extraneous sounds
			bool inhibitVariation = mVaryingBrickSound || mVaryingPaddleSound || mHits < 3;
			// ...even if the game conditions are eligible, only do it if we didn't vary last time, and only if enough hits have happened already
			mVaryingBrickSound = mVaryingPaddleSound = false;
			if( mEligibleToVary && !inhibitVariation )
			{
				double r = mRandomNumber;
				//bciout << r << endl;
				mVaryingBrickSound = r < mVariantProportion;
				mVaryingPaddleSound = mVariantProportion <= r && r < 2*mVariantProportion;
			}
			if( mVaryingPaddleSound )
				ThemeManager::Instance().AudioEvent( "HitPaddleVariant" );
			else
				ThemeManager::Instance().AudioEvent( "HitPaddle" );
		}

		mVirgin = false;
	}

	mBlockArray.Update();

	// Render the timer, hits, misses, blocks destroyed
	SaveGLState();
	qglColor( mTextColor );
	// Alright...  Who's responsible for this...
	// Vincent Connare, apparently :-P
	QFont font( "Comic Sans MS", mTextSize );
	int sec = (int)(GameTime() / 1000.0);
	int min = sec / 60; sec -= min * 60;
	double hitRate = 100 * (double)mHits / (double)(mHits + mMisses);
//	renderText( -1.0, -0.92, 0.0, QString( "Time: %1:%2" ).arg( min ).arg( sec, 2, 10, QLatin1Char('0') ),  font);
//	renderText( -1.0, -0.92, 0.0, QString( "Hits: %1" ).arg( mHits ), font );
//	renderText( -0.5, -0.92, 0.0, QString( "Misses: %1" ).arg( mMisses ), font );
//	renderText( -1.0, -0.92, 0.0, QString( "Points: %1" ).arg( mRewards.mPoints ), font );
//	renderText( -0.5, -0.92, 0.0, QString( "Rewards: %1" ).arg( mRewards.mAchieved ), font );
//	renderText( 0.4,  -0.92, 0.0, (mHits > 5) ? QString( "Accuracy: %1%" ).arg( (int)hitRate ) : QString("Accuracy: ---"), font );
//	renderText( 0,  -0.2, 0.0, QString( "%1" ).arg( FutureBounces() ), font );

	float lastx = 0.0f;
	float r = 0.08f, spacing = 1.2f;
	mRewardIcon.SetRadius(r);
	for(int i = mRewards.mAchieved-1; i >= 0; i--)
	{
		float x = i * r * spacing;
		if( lastx == 0.0 ) lastx = x + r;
		mRewardIcon.SetPosition(x - 1.0 + r, -1.0+r);
		mRewardIcon.Update();
	}
	mNoRewardIcon.SetRadius(r);
	int nIcons = (int)ceil((mRewards.mQualifyingDifficulty - mGameDifficulty)/mStepSize);
	spacing  = (2 - lastx) / ((nIcons+1) * r);
	if(spacing > 1.5f) spacing = 1.5f;
	for(int i = 0; (double)i < nIcons; i++)
	{
		float x = lastx + r + i * r * spacing;
		mNoRewardIcon.SetPosition(x - 1.0 + r, -1.0+r);
		mNoRewardIcon.Update();
	}

	// Render the game over screen
	if( mEnding )
	{
		// Render the gameover screen
		string grade; double maxSeen = -1;
		for( vector< pair< double, string > >::iterator i = mRewardStrings.begin(); i != mRewardStrings.end(); i++)
		{
			if((*i).first < maxSeen) continue;
			maxSeen = (*i).first;
			if(mRewards.mAchieved >= (*i).first) grade = (*i).second;
		}
		qglColor( mTextColor );
		QFont font( "Comic Sans MS", mTextSize*2 );

		renderText( -.75, 0.40, 0.0, QString( "You cleared 'em all..." ), font );
		renderText( -.00, 0.10, 0.0, QString( "...in %1 min %2 sec !" ).arg( min ).arg( sec ), font );
		renderText( -.75, -.20, 0.0, QString( "You collected %1 coin%2:" ).arg((int)mRewards.mAchieved).arg((mRewards.mAchieved==1)?"":"s"), font );
		renderText( -.50, -0.50, 0.0, QString( grade.c_str() ), font );
	}
	RestoreGLState();
}

// This method is called when the game has "overed."
void BreakoutWidget::GameOver( bool win )
{
	mEnding = true;
}

void BreakoutWidget::NewBall(bool solid)
{
	mBall.SetPosition( mRespawnX, mRespawnY );
	mBall.SetDirection( mRespawnDX, mRespawnDY );
	mBall.SetSolid(solid);
}

// Save the ball's current state for a potential respawn later
void BreakoutWidget::SetBallSpawn()
{
	mRespawnX = mBall.X();
	mRespawnY = mBall.Y();
	mRespawnDX = mBall.DX();
	mRespawnDY = mBall.DY();
}
void BreakoutWidget::SetPaddlePosition( float x, float y )
{
	float lim;
	lim = 1.0 - 0.5 * mPaddle.Width();
	x = (x < -lim) ? -lim : (x > +lim) ? +lim : x;

	lim = 1.0 - 0.5 * mPaddle.Height();
	y = (y < -lim) ? -lim : (y > +lim) ? +lim : y;

	mPaddle.SetPosition( x, y );
}
void BreakoutWidget::Predict()
{
	// save state
	bool wasSolid = mBall.Solid();
	SetBallSpawn();
	mBlockArray.SaveStatus();
	mPaddle.SetSolid(false);
	RewardState savedRewardState = mRewards;

	EventReport events;
	mSoFar = events;

	float xPre, yPre, xPost, yPost, xFinal, yFinal = mPaddle.Y() + mPaddle.Height() / 2.0 + mBall.Radius();
	while(mBall.DY() >= 0 || mBall.Y() > yFinal)
	{
		mBall.Position(xPre, yPre);
		Step(events);
		// mBall.Update(); // uncomment this to flash a prediction of the path
	}
	mBall.Position(xPost, yPost);
	xFinal = xPost - (xPost - xPre) * (yPost - yFinal) / (yPost - yPre);

	mExpected = events;
	mPredictedX = xFinal;
	mPredictedY = yFinal;

	// restore state
	NewBall(wasSolid);
	mPaddle.SetSolid(true);
	mBlockArray.RestoreStatus();
	mRewards = savedRewardState;
}

float BreakoutWidget::BallAngle()
{
	float x, y, a;
	mBall.Direction(x, y);
	a = atan2(y, x) * 180.0 / 3.1415926535897932;
	while(a < 0.0) a += 360.0;
	while(a >= 360.0) a -= 360.0;
	return a;
}

float BreakoutWidget::LeftMotionRequired()
{
	return max( 0.0, (double)mPaddle.X() - mPredictedX );
}

float BreakoutWidget::RightMotionRequired()
{
	return max( 0.0, mPredictedX - (double)mPaddle.X() );
}

int BreakoutWidget::BrickSoundStatus()
{
	if( !mTriggeredBrickSound ) return 0;
	if( !mEligibleToVary ) return 1;
	if( !mVaryingBrickSound ) return 2;
	return 3;
}

int BreakoutWidget::PaddleSoundStatus()
{
	if( !mTriggeredPaddleSound ) return 0;
	if( !mEligibleToVary ) return 1;
	if( !mVaryingPaddleSound ) return 2;
	return 3;
}

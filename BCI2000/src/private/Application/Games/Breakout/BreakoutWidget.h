////////////////////////////////////////////////////////////////////////////////
// $Id: BreakoutWidget.h 3401 2011-07-12 18:08:25Z gmilsap $
// Authors: griffin.milsap@gmail.com
// Description: A GameWidget which implements Breakout gameplay
//
// (C) 2000-2010, BCI2000 Project
// http://www.bci2000.org
////////////////////////////////////////////////////////////////////////////////
#ifndef BREAKOUTWIDGET_H
#define BREAKOUTWIDGET_H

#include "Ball.h"
#include "BlockArray.h"
#include "Paddle.h"
#include "../GameWidget.h"
#include <cmath>
#include <vector>
#include <utility>


class EventReport
{
public:
	EventReport()
	{
		mSteps = 0;
		mStepsWhereBricksWereHit = 0;
		mHitBrick = 0;
		mHitWall = 0;
		mHitCeiling = 0;
		mHitPaddleTop = 0;
		mHitPaddleSide = 0;
		mMiss = 0;
		mWinReward = 0;
	}
	~EventReport() {}

	int mSteps;
	int mStepsWhereBricksWereHit;
	int mHitBrick;
	int mHitWall;
	int mHitCeiling;
	int mHitPaddleTop;
	int mHitPaddleSide;
	int mMiss;
	int mWinReward;
};
class RewardState
{
public:
	RewardState();
	~RewardState();

	double mPoints;
	double mPointsThisTime;
	double mMaxPointsPerStrike;
	double mPointsRequired;
	double mQualifyingDifficulty;
	double mAchieved;
};

class CalibrationState
{
public:
	CalibrationState() {}
	~CalibrationState() {}

	int         TargetCode() const { return mTargetCode; }
	int         Feedback() const { return mFeedback; }
	int         FeedbackVisible() const { return ( mTrialNumber > mFeedbacklessTrials ); }
	int         Running() const { return (  mPostCalibrationPause || mTrialNumber <= mPromptSequence.size() ); }
	std::string Prompt() const;

	void Init( std::vector<std::string> prompts, std::vector<unsigned int> promptSequence, int promptDuration, int isi, int feedbackStateDelay, int feedbacklessTrials )
	{
		mPrompts = prompts; mPromptSequence = promptSequence; mPromptDuration = promptDuration; mISI = isi; mFeedbackStateDelay = feedbackStateDelay; mFeedbacklessTrials = feedbacklessTrials;
		Reset();
	}
	void Reset() { mCount = 0; mTrialNumber = 0; mTargetCode = 0; mFeedback = 0; mPostCalibrationPause = 0; }
	void Update();

private:
	int mCount;
	int mTargetCode;
	int mFeedback;
	int mPostCalibrationPause;
	unsigned int mTrialNumber;

	std::vector<std::string> mPrompts;
	std::vector<unsigned int> mPromptSequence;
	int mPromptDuration;
	int mISI;
	int mFeedbackStateDelay;
	unsigned int mFeedbacklessTrials;
};

class BreakoutWidget : public GameWidget
{
public:
	// Constructor/Destructor
    BreakoutWidget( QWidget* parent = 0, char* name = 0 );
    ~BreakoutWidget();

	// Game setup
	void InitGame( std::vector< std::vector< double > > &level, float paddleX, float paddleY );
	void InitPaddleWidth( float initialValue, float hitFactor ) { mPaddleWidthBaseValue = initialValue; mPaddleWidthHitFactor = hitFactor; }
	void InitBallRadius( float initialValue, float hitFactor ) { mBallRadiusBaseValue = initialValue; mBallRadiusHitFactor = hitFactor; }
	void InitBallSpeed( float initialValue, float hitFactor ) { mBallSpeedBaseValue = initialValue; mBallSpeedHitFactor = hitFactor; }
	void InitSpawnTime( int minSteps, int maxSteps ) { mMinSpawnTime = minSteps; mMaxSpawnTime = maxSteps; }
	void InitGameDifficulty(float targetAccuracy, float gameDifficulty, float stepSize);
	void InitRewards(float pointsRequired, float maxPointsPerHit);
	void InitCalibration( std::vector<std::string> prompts, std::vector<unsigned int> promptSequence, int promptDuration, int isi, int feedbackStateDelay, int feedbacklessTrials );

	void SetPaddlePosition( float x, float y );
	void SetPaddleHeight( float height ) { mPaddle.SetHeight( height ); }
	void SetPaddleWidth( float width ) { mPaddle.SetWidth( width ); }
	void SetBallRadius( float rad ) { mBall.SetRadius(rad); }
	void SetBallSpeed( float speed ) { mBall.SetSpeed(speed); }

	void ShowPredictor(bool setting) {mShowPredictor = setting;}
	void SetTextColor( QColor color ) { mTextColor = color; }
	void SetTextSize( int size ) { mTextSize = size; }
	void SetRewardString( float hitPercent, std::string str) { mRewardStrings.push_back( std::pair<double,std::string>( hitPercent, str ) ); }
	void SetVariantProportion( float p ) { mVariantProportion = p; }
	void SetRandomNumber( double r ) { mRandomNumber = r; } // TODO: need more satisfactory solution

	CalibrationState& Calibration() { return mCalibration; }

	// Queries
	bool GameOver() { return mEnding; }
	int Hits() { return mHits; }
	int Misses() { return mMisses; }
	int Reversals() { return mReversals; }
    void PaddlePosition(float &x, float &y) { mPaddle.Position(x,y); }
    void BallPosition(float &x, float &y) { mBall.Position(x,y); }
	float PaddleWidth() { return mPaddle.Width(); }
	float PaddleHeight() { return mPaddle.Height(); }
	float BallRadius() { return mBall.Radius(); }
	float BallSpeed() { return mBall.Speed(); }
	float BallAngle();
	float LeftMotionRequired();
	float RightMotionRequired();
	int FutureSteps();
	int FutureBounces();
	int FutureWallBounces();
	int FutureBrickBounces();
	int BrickSoundStatus();
	int PaddleSoundStatus();

protected:
	virtual int AdjustGameDifficulty(double step);
	virtual double ProcessRewardPoints(double points);
	virtual void Predict();

	virtual void Step(EventReport &events);
	virtual void Update();

private:
	// Game Events
	void GameOver( bool win );
	void NewBall(bool solid);
	void SetBallSpawn();

	// Member Variables
	Ball mBall;
	BlockArray mBlockArray;
	Paddle mPaddle;
	RectangleObject mBackground;
	QColor mTextColor;
	int mTextSize;

	// Game Variables
	int mHits;
	int mMisses;
	int mReversals;
	bool mVirgin;
	bool mEnding;
	bool mShowPredictor;

	double mTargetAccuracy;
	double mGameDifficulty;
	double mStepSize;
	double mLastStep;

	bool mTriggeredPaddleSound;
	bool mTriggeredBrickSound;
	bool mVaryingPaddleSound;
	bool mVaryingBrickSound;
	bool mEligibleToVary;
	double mVariantProportion;
	double mRandomNumber; // TODO: need more satisfactory solution

	RewardState mRewards;
	CalibrationState mCalibration;

	double mPaddleWidthBaseValue, mPaddleWidthHitFactor;
	double mBallRadiusBaseValue, mBallRadiusHitFactor;
	double mBallSpeedBaseValue, mBallSpeedHitFactor;

	float mRespawnX, mRespawnY, mRespawnDX, mRespawnDY;
	int mMinSpawnTime, mMaxSpawnTime;

	EventReport mSoFar;
	EventReport mExpected;
	float mPredictedX, mPredictedY;

	CircleObject mRewardIcon, mNoRewardIcon;

	std::vector< std::pair< double,std::string > > mRewardStrings;
};

#endif

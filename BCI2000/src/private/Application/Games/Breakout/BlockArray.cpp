////////////////////////////////////////////////////////////////////////////////
// $Id: BlockArray.cpp 4981 2015-08-26 17:04:38Z mellinger $
// Authors: griffin.milsap@gmail.com
// Description: Defines a collection of destructable blocks
//
// (C) 2000-2010, BCI2000 Project
// http://www.bci2000.org
////////////////////////////////////////////////////////////////////////////////
#include "BlockArray.h"
#include "../ThemeManager.h"
#include "BCIStream.h"
#include <iostream>
#include <math.h>

using namespace std;


// Constructor
BlockArray::BlockArray()  
{
}

// Deconstructor
BlockArray::~BlockArray()
{
	ClearBlockArray();
}

// Load a level from a bool matrix, allocating memory as necessary
void BlockArray::LoadLevel( vector< vector< double > > &level )
{
	// Clear the current level
	ClearBlockArray();

	// Create the level
	unsigned int LevelHeight = level.size();
	unsigned int LevelWidth = level[0].size();
	float BlockWidth = 2.0f / ( float )LevelWidth;
	float BlockHeight = 1.0f / ( float )LevelHeight;
	for( unsigned int i = 0; i < LevelHeight; i++ )
	{
		// Some minor error checking
		if( level[i].size() != LevelWidth )
			bcierr << "Level Array not sized correctly!" << endl;

		// Create the level
		for( unsigned int j = 0; j < LevelWidth; j++ )
		{
			if( level[i][j] )
			{
				// Create the object
				Brick* obj = new Brick();

				// Set its bounds
				obj->SetExtent( -1.0f + ( BlockWidth * ( float )j ), 1.0f - ( BlockHeight * ( ( float )i + 1.0f ) ), BlockWidth, BlockHeight );
				obj->Init( level[i][j] ); // registers it with the ThemeManager

				// Push it into the array
				mBricks.push_back( obj );
			}
		}
	}
}

// Clears the array and deallocates memory
void BlockArray::ClearBlockArray()
{
	for( unsigned int i = 0; i < mBricks.size(); i++ )
		delete mBricks[i];
	mBricks.clear();
}

void BlockArray::SaveStatus()
{
	for( vector< Brick* >::iterator itr = mBricks.begin(); itr != mBricks.end(); itr++ )
		(*itr)->SaveHitPoints();
}

void BlockArray::RestoreStatus()
{
	for( vector< Brick* >::iterator itr = mBricks.begin(); itr != mBricks.end(); itr++ )
		(*itr)->RestoreHitPoints();
}

void BlockArray::Purge()
{
	vector< Brick* >::iterator itr = mBricks.begin();
	while( itr != mBricks.end() )
	{
		if( (*itr)->Dead() )
		{
			delete *itr;
			itr = mBricks.erase( itr );
		}
		else
		{
			itr++;
		}
	}
	SaveStatus();
}

// Updates all of the blocks
void BlockArray::Update()
{
	for( vector< Brick* >::iterator itr = mBricks.begin(); itr != mBricks.end(); itr++ )
		(*itr)->Update();
}

int BlockArray::Collide(Ball &ball)
{
	int nHits = 0;	

	for( vector< Brick* >::iterator itr = mBricks.begin(); itr != mBricks.end(); itr++ )
	{
		nHits += (*itr)->Collide(ball);
	}
	return nHits;
}

int BlockArray::BricksLeft()
{
	int n = 0;	

	for( vector< Brick* >::iterator itr = mBricks.begin(); itr != mBricks.end(); itr++ )
	{
		if( !(*itr)->Dead() ) n++;
	}
	return n;
}





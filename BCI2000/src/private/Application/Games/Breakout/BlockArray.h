////////////////////////////////////////////////////////////////////////////////
// $Id: BlockArray.h 3126 2011-02-22 01:10:32Z jhill $
// Authors: griffin.milsap@gmail.com
// Description: Defines a collection of destructable blocks
//
// (C) 2000-2010, BCI2000 Project
// http://www.bci2000.org
////////////////////////////////////////////////////////////////////////////////
#ifndef BLOCKARRAY_H
#define BLOCKARRAY_H

#include "Brick.h"
#include "Ball.h"
#include <vector>

class BlockArray
{
public:
	BlockArray();
	~BlockArray();

	// Public Member Methods
	void LoadLevel( std::vector< std::vector< double > > &level );
	int Collide( Ball &ball );
	void Update();
	int BricksLeft();

	void SaveStatus();
	void RestoreStatus();
	void Purge();

private:
	// Private Helper Methods
	void ClearBlockArray();

	std::vector< Brick* > mBricks;
};

#endif // BLOCK_H
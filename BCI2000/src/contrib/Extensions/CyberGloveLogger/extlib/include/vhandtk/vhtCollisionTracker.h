#ifndef VHT_COLLISION_TRACKER_H
#define VHT_COLLISION_TRACKER_H


#if defined(BUILD_WIN32_VH_DLL)

#if defined __BUILD_VH_CORE_DLL__
#define VH_CORE_DLL_EXPORT __declspec(dllexport)
#else
#define VH_CORE_DLL_EXPORT __declspec(dllimport)
#endif

#else 

#define VH_CORE_DLL_EXPORT

#endif

#include <vhtVector3d.h>
#include <vhtShape3D.h>
#include "vhtWitnessPlane.h"

class vhtCollisionPair;

#define MAX_COLLISIONS 100

//: Internal Ghost Hand support class.
//!vhtmodule: Core
class VH_CORE_DLL_EXPORT vhtCollisionTracker
{
	public:
		vhtCollisionTracker(void);
		~vhtCollisionTracker(void);

		double getCurrentMTD(void) { return currMTD; }

		bool getCurrentOffset( vhtVector3d &offset, double &dist );

		enum PairOrder { handOrder, obj1Order, obj2Order };

		void setCurrentPair( vhtCollisionPair *currPair, PairOrder anOrder = handOrder );

		const vhtVector3d &getContactNormal( void );

		const vhtWitnessPlane &getWitnessPlane( void );

		virtual int getNumCollisions( void ) { return numCollisions; }
		virtual void resetCollisions( void );

		void reset( void );
    
		void print( void );

	private:

		double        currMTD;
		vhtVector3d   preContactWp;
		vhtVector3d   currentWp;

		// the contact plane
		vhtWitnessPlane   witnessPlane[MAX_COLLISIONS];
		unsigned int           numCollisions;
		unsigned int           bestWitness;

		vhtCollisionPair *lastPair;
};

#endif

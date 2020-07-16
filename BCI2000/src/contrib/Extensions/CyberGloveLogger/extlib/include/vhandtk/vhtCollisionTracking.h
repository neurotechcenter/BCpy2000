#ifndef _VHT_COLLISION_TRACKING_H
#define _VHT_COLLISION_TRACKING_H


#if defined(BUILD_WIN32_VH_DLL)

#if defined __BUILD_VH_CORE_DLL__
#define VH_CORE_DLL_EXPORT __declspec(dllexport)
#else
#define VH_CORE_DLL_EXPORT __declspec(dllimport)
#endif

#else 

#define VH_CORE_DLL_EXPORT

#endif

//: Collision tracking mechanism.
// Provides an automated mechanism for tracking collisions between two objects.  Collision
// tracking is an optimization techinique for calculating collision MTD's of objects that
// do not move significantly with respect to each other.  Below the unit tracking distance
// collisions are checked on every call.  Beyond the tracking distance, collisions are
// checked only every tracking length. In between, the collision call frequency is linearly 
// interpolated.  For most users, the defaults settings in this class are adequate.
//!vhtmodule: Core
class VH_CORE_DLL_EXPORT vhtCollisionTracking
{
	public:
		vhtCollisionTracking(void);
		//: Constructor.
		// Use default collision tracking parameters.

		~vhtCollisionTracking(void);
		//: Destructor.

		inline void setTrackingLength( int length ) 
		{ m_trackingLength = m_maxTrackingLength = length; }    
		//: Set maximum tracking length.
		// Sets the maximum number of calls before collisions are checked exactly. 
		inline int getTrackingLength(void) 
		{ return m_trackingLength; }
		// Get the maximum tracking length.

		void setMaxTrackingDistance( double dist );
		//: Set the maximum tracking distance.
		// The distance beyond which tracking length is used.
		inline double getMaxTrackingDistance( ) { return m_maxTrackingDistance; }
		//: Get the maximum tracking distance.

		void setUnitTrackingDistance( double dist );
		//: Set the minimum tracking distance.
		// Sets the distance (MTD) below which the tracking length is 1 call.
		inline double getUnitTrackingDistance(void) { return m_unitTrackingDistance; }
		//: Get the minimum tracking distance.


		bool exactRequired(void);
		//: Require exact collisions.
		// ExactRequired == true, implies that collisions should be checked exactly.
		// This is equivalent to setting the unit tracking distance to a huge value.

		void incrementTrackCount( double dist );
		//: Increment the track counter.
		// Should be called after each mtd calc.

		inline int getTrackCount( void ) { return m_trackCount; }

	private:
		// update the tracking length
		void updateTrackingLength( double dist );

		int             m_trackingLength;
		int             m_trackCount;

		int             m_maxTrackingLength;
		double          m_unitTrackingDistance;
		double          m_maxTrackingDistance;
		double          m_trackingSlope;
};

#endif



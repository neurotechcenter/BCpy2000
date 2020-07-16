////////////////////////////////////////////////////////////////////////////////
// $Id:  $
// Authors: jezhill@gmail.com
// Description: provides the Quantilizer template class 
// 
//              Example: 
//              	Quantilizer<double> buffer(CAPACITY);
//              	while(true)
//              	{
//              		buffer << NEW_INPUT_VALUE();
//              		double fifthPercentile = buffer.Quantile( 0.05 );
//              		double median = buffer.Quantile( 0.5 );
//              	}
// 
//              The quantilizer maintains a running buffer over the
//              last CAPACITY input values, and uses these to compute
//              quantiles. Update time is proportional to CAPACITY,
//              and subsequent lookups via .Quantile() or ::iterator
//              can be done in constant time.
// 
//              Variant constructor:
//              	Quantilizer<double> buffer(CAPACITY, LIFETIME);
// 
//              If LIFETIME > 0,  then only the first LIFETIME updates
//              will be considered. After that, the distribution will be
//              assumed to have stabilized, and the values returned by
//              the Quantile() method will not change.  Do a buffer.Reset()
//              in order to flush the buffer and reset this count.
// 
//              In Quantilizer<T>, any type T may be used provided it
//              supports operator<= 
//
//
// $BEGIN_BCI2000_LICENSE$
// 
// This file is part of BCI2000, a platform for real-time bio-signal research.
// [ Copyright (C) 2000-2012: BCI2000 team and many external contributors ]
// 
// BCI2000 is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
// 
// BCI2000 is distributed in the hope that it will be useful, but
//                         WITHOUT ANY WARRANTY
// - without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License along with
// this program.  If not, see <http://www.gnu.org/licenses/>.
// 
// $END_BCI2000_LICENSE$
////////////////////////////////////////////////////////////////////////////////
#ifndef QUANTILES_H
#define QUANTILES_H

template <typename T> class Sortable;
template <typename T> class Quantilizer
{
	public:
		Quantilizer( );
		Quantilizer( unsigned int inCapacity, unsigned int inLifetime=0 );
		~Quantilizer( );
		
		Quantilizer<T> & operator<<( const T & inValue );
		T& Quantile( double q );
		T& Quantile( double q, bool & valid ) { valid = IsValidQ( q ); return Quantile( q ); }
		T* operator[]( unsigned int index );

		void Reset( void ) { Init( mCapacity, mLifetime ); }    // empty the buffer, and resurrect the object if it has died
		void Reset( unsigned int inCapacity ) { Init( inCapacity, mLifetime ); }
		void Reset( unsigned int inCapacity, unsigned int inLifetime ) { Init( inCapacity, inLifetime ); }

		unsigned int Seen( void ) { return mSeen; }             // return the number of elements ever submitted, since reset
		unsigned int Lifetime( void ) { return mLifetime; }     // return the number of submissions after which the Quantilizer "dies" and no stores submitted values in the buffer.  0 = immortality
		unsigned int Capacity( void ) { return mCapacity; }     // return the size of the rolling buffer
		unsigned int Membership( void ) { return mMembership; } // return the number of elements currently in the buffer
		bool Empty( void ) { return mMembership == 0; }         // return true if the buffer is empty
		bool Full( void ) { return mMembership >= mCapacity; }  // return true if the buffer is full (if so, the oldest element will drop out on next submission, unless the object has died)
		bool Alive( void ) { return mLifetime == 0 || mSeen < mLifetime; } // return whether the object is alive
		
		double QMin( void ) { return 1.0 / double( mMembership + 1 ); }                    // What is the smallest valid input value to Quantile()? (smaller values than this will just return the minimum value of the buffer)
		double QMax( void ) { return double( mMembership ) / double( mMembership + 1 ); }  // What is the largest valid input value to Quantile()?  (larger values than this will just return the maximum value of the buffer)
		bool IsValidQ( double q ) { return q >= QMin() && q <= QMax(); }                   // Is the input q-value in the range [ QMin(), QMax() ] ?
		double i2q( unsigned int i ) { return double( i + 1 ) / double( mMembership + 1 ); }  // Convert from an integer index to a q-value.  obj[i] is equal to obj.Quantile( q )
		double q2i( double q ) { return q * double( mMembership + 1 ) - 1.0; }                // Convert from a q-value to an integer index.  obj[i] is equal to obj.Quantile( q )
		
		class iterator // for( Quantilizer<T>::iterator it = obj.begin(); it != obj.end(); it++ )
		{              // {   
			           //     T eachSortedValueInTheBuffer = *it;
			public:    //     double correspondingQValue = it.where();
			           // }
				bool operator==( const typename Quantilizer<T>::iterator & inOther ) { return &mParent == &inOther.mParent && mIndex == inOther.mIndex; }
				bool operator!=( const typename Quantilizer<T>::iterator & inOther ) { return !operator==( inOther ); }
				typename Quantilizer<T>::iterator & operator++() { mIndex++; return *this; }
				typename Quantilizer<T>::iterator & operator++(int) { mIndex++; return *this; }
				T& operator*() { return *mParent[mIndex]; }
				T* operator->() { return mParent[mIndex]; }
				double where() { return mParent.i2q( mIndex ); }
			
			private:
				friend class Quantilizer<T>;
				iterator( Quantilizer<T> & inParent, unsigned int inIndex ) : mParent( inParent ), mIndex( inIndex ) {}
				Quantilizer<T> & mParent;
				unsigned int mIndex;
		};
		typename Quantilizer<T>::iterator begin( void ) { return typename Quantilizer<T>::iterator( *this, 0 ); }
		typename Quantilizer<T>::iterator end( void ) { return typename Quantilizer<T>::iterator( *this, mMembership ); }
		
	private:
		void Init( unsigned int inCapacity, unsigned int inLifetime=0 );
		void Wipe( void );
		void Add( Sortable<T>* q );
		void Remove( Sortable<T>* q );
	
		unsigned int mSeen;
		unsigned int mLifetime;
		unsigned int mCapacity;
		unsigned int mMembership;
		unsigned int mCursor;
		Sortable<T>* mHead;
		Sortable<T>* mRing;
		T* mSorted;
};

#endif // QUANTILES_H

////////////////////////////////////////////////////////////////////////////////
// $Id:  $
// Authors: jezhill@gmail.com
// Description: implements the Quantilizer template class (see header for doc)
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

// #include "PCHIncludes.h"
// #pragma hdrstop

#include "Quantiles.h"
#include <cstddef> // for NULL

template <typename T> class Sortable
{
	public:
		Sortable( )                                  : mContent( ), mPrev( NULL ), mNext( NULL ) { }
		Sortable( const T & inContent )              : mContent( ), mPrev( NULL ), mNext( NULL ) { SetValue( inContent ); }
		Sortable( const Sortable<T> & inToBeCopied ) : mContent( ), mPrev( NULL ), mNext( NULL ) { SetValue( inToBeCopied.GetValue() ); }
		~Sortable( ) {}
		const Sortable<T> & operator=( const Sortable<T> & inToBeCopied ) { SetValue( inToBeCopied.GetValue() ); return *this; }
		const Sortable<T> & operator=( const T & inContent )              { SetValue(inContent);                 return *this; }
		void SetValue( const T & inContent ) { mContent = inContent; }
		const T & GetValue( void ) const { return mContent; }
		Sortable<T> * Previous( void ) { return mPrev; }
		Sortable<T> * Next( void ) { return mNext; }
		void InsertBefore( Sortable<T> & inOther );
		void InsertAfter( Sortable<T> & inOther );
		void Unlink( void );
		
	private:
		T mContent;
		Sortable<T> * mPrev;
		Sortable<T> * mNext;
};

template<typename T>
void
Sortable<T>::InsertBefore( Sortable<T> & inOther )
{
	mPrev = inOther.mPrev;
	inOther.mPrev = this;
	if( mPrev ) mPrev->mNext = this;
	mNext = &inOther;
}

template<typename T>
void
Sortable<T>::InsertAfter( Sortable<T> & inOther )
{
	mNext = inOther.mNext;
	inOther.mNext = this;
	if( mNext ) mNext->mPrev = this;
	mPrev = &inOther;
}

template<typename T>
void
Sortable<T>::Unlink( void )
{
	if( mPrev ) mPrev->mNext = mNext;
	if( mNext ) mNext->mPrev = mPrev;
	mPrev = NULL;
	mNext = NULL;
}


template<typename T> Quantilizer<T>::Quantilizer( )                                                  : mSeen( 0 ), mLifetime( 0 ), mCapacity( 0 ), mMembership( 0 ), mCursor( 0 ), mHead( NULL ), mRing( NULL ), mSorted( NULL )  { Wipe(); }
template<typename T> Quantilizer<T>::Quantilizer( unsigned int inCapacity, unsigned int inLifetime ) : mSeen( 0 ), mLifetime( 0 ), mCapacity( 0 ), mMembership( 0 ), mCursor( 0 ), mHead( NULL ), mRing( NULL ), mSorted( NULL )  { Init( inCapacity, inLifetime ); }

template<typename T> Quantilizer<T>::~Quantilizer( ) { Wipe(); }

template<typename T>
void
Quantilizer<T>::Wipe( void )
{
	mSeen = 0;
	mLifetime = 0;
	mMembership = 0;
	mCursor = 0;
	mHead = NULL;
	delete [] mRing; mRing = NULL;
	delete [] mSorted; mSorted = NULL;
}

template<typename T>
void
Quantilizer<T>::Init( unsigned int inCapacity, unsigned int inLifetime )
{
	Wipe();
	mLifetime = inLifetime;
	mCapacity = inCapacity;
	if( mCapacity )
	{
		mRing = new Sortable<T>[ mCapacity ];
		mSorted = new T[ mCapacity ];
	}
}

template<typename T>
Quantilizer<T> &
Quantilizer<T>::operator<<( const T & inValue )
{
	if( mCapacity > 0 && Alive() )
	{
		mCursor = ( mCursor + 1 ) % mCapacity;
		Sortable<T> * q = mRing + mCursor;
		if( Full() ) Remove( q );
		q->SetValue( inValue );
		Add( q );
		mSeen++;
	}
	return *this;
}


template<typename T>
void
Quantilizer<T>::Remove( Sortable<T>* q )
{
	if( q == NULL || mMembership == 0 ) return;
	if( q == mHead ) mHead = q->Next();
	q->Unlink();
	mMembership--;
}

template<typename T>
void
Quantilizer<T>::Add( Sortable<T>* q )
{
	if( q == NULL || mMembership >= mCapacity ) return;
	const T & qv = q->GetValue();
	bool done = false;
	if( mHead == NULL )
	{
		mHead = q;
		mSorted[0] = qv;
		done = true;
	}
	else
	{
		Sortable<T>* p = mHead;
		for( unsigned int i = 0; p != NULL; p = p->Next(), i++ )
		{
			if( !done && qv <= p->GetValue() )
			{
				q->InsertBefore( *p );
				if( p == mHead ) mHead = q;
				p = q;
				done = true;
			}
			else if ( !done && p->Next() == NULL )
			{
				q->InsertAfter( *p );
				done = true;
			}
			mSorted[ i ] = p->GetValue();
		}
	}
	mMembership++;
}

template<typename T>
T*
Quantilizer<T>::operator[]( unsigned int index )
{	
	if( mMembership == 0.0 ) return NULL;
	if( index >= mMembership - 1 ) index = mMembership - 1;
	return mSorted + index;
}
template<typename T>
T&
Quantilizer<T>::Quantile( double q )
{
	q = q2i( q );
	if( q < 0.0 ) q = 0.0;
	unsigned int index = (unsigned int)( q + 0.5 );
	return *operator[]( index );
}


// The following lines avoid linker errors: for classes not explicitly pre-concretized in this way,
// you would have to #include the entire .cpp file before instantiating the class, rather than just
// the header (then when you do: beware of symbols now potentially defined in multiple places -
// Quantiles.cpp itself, and the source file with the #include "Quantiles.cpp" )

template class Quantilizer<double>;

#include <utility>
template class Quantilizer< std::pair<double,double> >;

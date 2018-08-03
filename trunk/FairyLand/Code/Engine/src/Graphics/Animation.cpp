//============================================================================
// Animation.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandGraphics.h"

//----------------------------------------------------------------------------
// Animation Implementation
//----------------------------------------------------------------------------

Animation::Animation( StringPtr name )
{
	mName			= name;
	mDuration		= 0.0f;
	mCurrent		= 0.0f;
	mSpeed			= 1.0f;
	mCircular		= _true;
	mBackward		= _false;
	mAutoClearEvent = _true;
	mUserData1		= 0;
	mUserData2		= 0;
}

_bool Animation::InsertEvent( _float time, StringPtr name )
{
	if ( time > mDuration )
		return _false;

	_bool added = _false;

	for ( _dword i = 0; i < mEventArray.Number( ); i ++ )
	{
		EventArray::Iterator iterator = mEventArray.GetIterator( i );

		if ( mEventArray[i].mObject1 > time )
		{
			mEventArray.Insert( EventPair( time, String( name ) ), iterator );		
			added = _true;

			break;
		}
	}

	if ( added == _false )
		mEventArray.Append( EventPair( time, String( name ) ) );
	
	return _true;
}

_bool Animation::RemoveEvent( _dword index )
{
	if ( index < mEventArray.Number( ) )
	{
		mEventArray.Remove( index ); 
		return _true;
	}
	
	return _false;
}

_bool Animation::ClearEvent( )
{
	mEventArray.Clear( );

	return _true;
}

_dword Animation::GetTotalEventNumber( ) const
{
	return mEventArray.Number( );
}

_float Animation::GetTotalEventTime( _dword index ) const
{
	return mEventArray[index].mObject1;
}

StringPtr Animation::GetTotalEventName( _dword index ) const
{
	return mEventArray[index].mObject2;
}

_dword Animation::GetHappenedEventNumber( ) const
{
	return mIndexArray.Number( );
}

_float Animation::GetHappenedEventTime( _dword index ) const
{
	return mEventArray[mIndexArray[index]].mObject1;
}

StringPtr Animation::GetHappenedEventName( _dword index ) const
{
	return mEventArray[mIndexArray[index]].mObject2;
}

_void Animation::ClearEventHappened( )
{
	mIndexArray.Clear( );
}

_void Animation::Update( _float last, _float current )
{
	if ( mEventArray.Number( ) == 0 )
		return;

	if ( mAutoClearEvent )
		mIndexArray.Clear( );
	
	_float millisecond = 0.001f;

	// Loop Animation
	if ( mCircular )
	{
		//_dword loopbegin = 0;
		//for ( _float temp = mDuration; temp < last; temp += mDuration )
			//loopbegin ++;
		
		//_dword loopend = 0;
		//for ( _float temp = mDuration; temp < current; temp += mDuration )
			//loopend ++;

		_dword loopbegin = _dword( last / mDuration );
		_dword loopend	 = _dword( current / mDuration );
		
		for ( _dword i = loopbegin; i <= loopend; i ++ )
		{
			for ( _dword j = 0; j < mEventArray.Number( ); j ++ )
			{
				_float oritime = mEventArray[j].mObject1;
				
				// For float error, event should NOT at 0.0f or duration
				if ( oritime < millisecond )
					oritime = millisecond;
				if ( mDuration - oritime < millisecond )
					oritime = mDuration - millisecond;

				_float eventtime = oritime + ( i * mDuration );
				
				if ( last < eventtime && eventtime <= current )
				{
					mIndexArray.Append( j );
				}
			}
		}
	}
	// Once Animation
	else
	{
		// For every event
		for ( _dword i = 0; i < mEventArray.Number( ); i ++ )
		{
			_float oritime = mEventArray[i].mObject1;
				
			// For float error, event should NOT at 0.0f or duration
			if ( oritime < millisecond )
				oritime = millisecond;
			if ( mDuration - oritime < millisecond )
				oritime = mDuration - millisecond;

			_float eventtime = oritime;

			// For float error, event should NOT at 0.0f or duration
			if ( last < eventtime && eventtime <= current )
			{
				mIndexArray.Append( i );
			}
		}
	}
}
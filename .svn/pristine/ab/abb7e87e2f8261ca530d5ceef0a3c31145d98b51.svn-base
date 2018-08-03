//============================================================================
// Animation.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// Animation
//----------------------------------------------------------------------------

class Animation : public ReferencedObject
{
protected:
	typedef Pair< _float, String >	EventPair;
	typedef Array< EventPair >		EventArray;
	typedef Array< _dword >			IndexArray;

	String		mName;
	_float		mDuration;
	_float		mCurrent;
	_float		mSpeed;
	_bool		mCircular;
	_bool		mBackward;
	_bool		mAutoClearEvent;
	_dword		mUserData1;
	_dword		mUserData2;

	EventArray	mEventArray;
	IndexArray	mIndexArray;

protected:
	Animation( StringPtr name );

protected:
	_bool		InsertEvent( _float time, StringPtr name );
	_bool		RemoveEvent( _dword index );
	_bool		ClearEvent( );

	_dword		GetTotalEventNumber( ) const;
	_float		GetTotalEventTime( _dword index ) const;
	StringPtr	GetTotalEventName( _dword index ) const;

	_dword		GetHappenedEventNumber( ) const;
	_float		GetHappenedEventTime( _dword index ) const;
	StringPtr	GetHappenedEventName( _dword index ) const;

	_void		ClearEventHappened( );
	_void		Update( _float last, _float current );
};		  

};
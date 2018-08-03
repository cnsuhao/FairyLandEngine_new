//============================================================================
// IAnimation.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// IAnimation
//----------------------------------------------------------------------------

class IAnimation
{
public:
	virtual StringPtr	GetName( ) const = 0;
	virtual _float		GetDuration( ) const = 0;

	virtual _void		SetCurrent( _float current ) = 0;
	virtual _float		GetCurrent( ) const = 0;
	virtual _void		SetSpeed( _float speed ) = 0;
	virtual _float		GetSpeed( ) const = 0;
	virtual _void		SetCircular( _bool circular ) = 0;
	virtual _bool		GetCircular( ) const = 0;
	virtual _void		SetBackward( _bool backward ) = 0;
	virtual _bool		GetBackward( ) const = 0;
	virtual _void		SetAutoClearEvent( _bool autoclear ) = 0;
	virtual _bool		GetAutoClearEvent( ) const = 0;
	virtual _void		SetUserData( _dword userdata ) = 0;
	virtual _dword		GetUserData( ) const = 0;
	virtual _void		SetUserData2( _dword userdata ) = 0;
	virtual _dword		GetUserData2( ) const = 0;

	virtual _dword		GetTotalEventNumber( ) const = 0;
	virtual _float		GetTotalEventTime( _dword index ) const = 0;
	virtual StringPtr	GetTotalEventName( _dword index ) const = 0;

	virtual _dword		GetHappenedEventNumber( ) const = 0;
	virtual _float		GetHappenedEventTime( _dword index ) const = 0;
	virtual StringPtr	GetHappenedEventName( _dword index ) const = 0;

	virtual _bool		InsertEvent( _float time, StringPtr name ) = 0;
	virtual _bool		RemoveEvent( _dword index ) = 0;
	virtual _bool		ClearEvent( ) = 0;

	virtual _void		Update( _dword elapse, _float weight = 1.0f, _bool onlyroot = _false ) = 0;
	virtual _void		Reset( ) = 0;
};

};
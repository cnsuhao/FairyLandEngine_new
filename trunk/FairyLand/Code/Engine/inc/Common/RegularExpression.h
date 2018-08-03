//============================================================================
// RegularExpression.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// RegularExpression
//----------------------------------------------------------------------------

class RegularExpression
{
public:
	enum _MATCH_OPTION
	{
		_MATCH_CASELESS			= 0x0001,
		_MATCH_SEQUENCE			= 0x0002,

		_MATCH_PRE_EXCLUDE		= 0x0010,
		_MATCH_POST_EXCLUDE		= 0x0020,

		_MATCH_PRE_SPACE		= 0x0100,
		_MATCH_POST_SPACE		= 0x0200,
		_MATCH_INNER_SPACE		= 0x0400,

		_MATCH_PRE_NONEWORD		= 0x1000,
		_MATCH_POST_NONEWORD	= 0x2000,
		_MATCH_INNER_NONEWORD	= 0x4000,
	};

private:
	_void*	mPCREContext;
	_dword	mMatchOption;

	RegularExpression( const RegularExpression& );
	RegularExpression& operator = ( const RegularExpression& );

public:
	RegularExpression( );
	~RegularExpression( );

	_bool SetRegularExpression( StringPtr pattern, _dword option = _MATCH_CASELESS );

	_dword Match( const _charw* subject, _dword* startindex, _dword* endindex, _dword length, _bool matchall = _true );
	_dword Match( const _chara* subject, _dword* startindex, _dword* endindex, _dword length, _bool matchall = _true );
};

}
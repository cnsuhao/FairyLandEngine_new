//============================================================================
// Expression.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// Expression
//----------------------------------------------------------------------------

class Expression
{
public:
	typedef _void (*OnError)( const Expression* expression, StringPtr errormsg, _void* parameter1, _void* parameter2 );

	typedef _void (*OnMethod)( const Expression* expression, _dword level, _dword argindex, StringPtr methodname, _dword parameternumber, _char* parameterstring[], const TinyVariable parametervalue[], TinyVariable& result, _void* parameter1, _void* parameter2 );
	typedef _void (*OnVariable)( const Expression* expression, StringPtr variablename, TinyVariable& result, _void* parameter1, _void* parameter2 );

	typedef _void (*OnCheckMethod)( const Expression* expression, StringPtr methodname, _dword parameternumber, _char* parameterstring[], _void* parameter1, _void* parameter2 );
	typedef _void (*OnCheckVariable)( const Expression* expression, StringPtr variablename, _void* parameter1, _void* parameter2 );
	typedef _void (*OnCheckType)( const Expression* expression, StringPtr opstring1, _char operatorcharacter, StringPtr opstring2, _void* parameter1, _void* parameter2 );

public:
	inline _void 		SetErrorCallback( OnError funcpointer );

	inline _void 		SetMethodCallback( OnMethod funcpointer );
	inline _void 		SetVariableCallback( OnVariable funcpointer );

	inline _void 		SetCheckMethodCallback( OnCheckMethod funcpointer );
	inline _void 		SetCheckVariableCallback( OnCheckVariable funcpointer );
	inline _void 		SetCheckTypeCallback( OnCheckType funcpointer );

	inline _void 		SetUserData1( _dword userdata );
	inline _void 		SetUserData2( _dword userdata );
	inline _void 		SetUserData3( _dword userdata );
	inline _void 		SetUserData4( _dword userdata );

	inline _dword		GetUserData1( ) const;
	inline _dword		GetUserData2( ) const;
	inline _dword		GetUserData3( ) const;
	inline _dword		GetUserData4( ) const;

	inline _dword		GetCalledMethodNumber( ) const;

	inline StringPtr	GetExpression( ) const;

	inline _dword		GetSectionNumber( ) const;

public:
	_bool 	Load( StringPtr expression, _void* parameter1 = _null, _void* parameter2 = _null );
	_void 	Unload( );

	_void	Calculate( TinyVariable& result, _void* parameter1 = _null, _void* parameter2 = _null );
	_void	Check( _void* parameter1 = _null, _void* parameter2 = _null );

public:
	Expression( );
	~Expression( );

private:
	enum _OPERATOR
	{
		// Unknown Type
		_OPERATOR_UNKNOWN		= 0,	// ?

		// Common's Operator	
		_OPERATOR_ADD			= 1, 	// +
		_OPERATOR_SUB			= 2, 	// -
		_OPERATOR_MUL			= 3, 	// /
		_OPERATOR_DIV			= 4, 	// *

		_OPERATOR_BIG			= 5,	// >
		_OPERATOR_BIG_EQUAL		= 6,	// >=
		_OPERATOR_NOT_EQUAL		= 7,	// !=
		_OPERATOR_EQUAL			= 8,	// ==
		_OPERATOR_LESS			= 9,	// <
		_OPERATOR_LESS_EQUAL	= 10,	// <=

		_OPERATOR_AND			= 11,	// &&
		_OPERATOR_OR			= 12,	// ||

		_OPERATOR_NUMBER		= 13,
	};

private:
	struct Section
	{
		_bool			mIsMethod;
		_dword			mLevel;
		_dword			mArgIndex;
		TinyVariable	mValue;

		String			mExpression;
	};

private:
	enum
	{
		_MAX_SECTION_NUMBER = 24
	};

	_bool				mIsCheckMode;

	_dword				mMaxLevel;
	String				mExpression;

	_dword				mUserData1;
	_dword				mUserData2;
	_dword				mUserData3;
	_dword				mUserData4;

	_dword				mCalledMethodNumber;

	OnError				mErrorFunc;
	OnMethod			mMethodFunc;
	OnVariable			mVariableFunc;
	OnCheckMethod		mCheckMethodFunc;
	OnCheckVariable		mCheckVariableFunc;
	OnCheckType			mCheckTypeFunc;

	Section				mSectionArray[ _MAX_SECTION_NUMBER ];
	_dword				mSectionNumber;

	TinyVariable		mTempTinyVariableArray[ 8 ];

private:
	static _chara sOperatorPriorityTable[ _OPERATOR_NUMBER ][ _OPERATOR_NUMBER ];

private:
	inline _bool	IsEnglishCharacter( _char character ) const;
	inline _bool	IsMethodOrVariableName( StringPtr name ) const;

	inline _dword	GetOperatorFlag( _char character ) const;
	inline _char	GetOperatorCode( _dword operatorflag ) const;
	inline _long	CompareOperator( _dword op1, _dword op2 ) const;

private:
	_bool	BuildSection( _dword& index, _dword level, Section* section, _bool splited = _false, _bool wholesection = _false );

	_void	CalculateSection( Section& section, TinyVariable& result, _void* parameter1, _void* parameter2 );
	_void	CalculateExpression( const Section& section, StringPtr op1string, _dword operatorflag, StringPtr op2string, TinyVariable& result, _void* parameter1, _void* parameter2 );
	_void	Calculate( _dword startindex, _dword number, TinyVariable& result, _void* parameter1, _void* parameter2 );

	_void	GetValue( const Section& section, StringPtr expression, TinyVariable& result, _void* parameter1, _void* parameter2 );

	_dword 	GetSectionNumber( StringPtr expression ) const;
};

//----------------------------------------------------------------------------
// Expression Implementation
//----------------------------------------------------------------------------

_void Expression::SetErrorCallback( OnError funcpointer )
{
	mErrorFunc = funcpointer;
}

_void Expression::SetMethodCallback( OnMethod funcpointer )
{
	mMethodFunc = funcpointer;
}

_void Expression::SetVariableCallback( OnVariable funcpointer )
{
	mVariableFunc = funcpointer;
}

_void Expression::SetCheckMethodCallback( OnCheckMethod funcpointer )
{
	mCheckMethodFunc = funcpointer;
}

_void Expression::SetCheckVariableCallback( OnCheckVariable funcpointer )
{
	mCheckVariableFunc = funcpointer;
}

_void Expression::SetCheckTypeCallback( OnCheckType funcpointer )
{
	mCheckTypeFunc = funcpointer;
}

_void Expression::SetUserData1( _dword userdata )
{
	mUserData1 = userdata;
}

_void Expression::SetUserData2( _dword userdata )
{
	mUserData2 = userdata;
}

_void Expression::SetUserData3( _dword userdata )
{
	mUserData3 = userdata;
}

_void Expression::SetUserData4( _dword userdata )
{
	mUserData4 = userdata;
}

_dword Expression::GetUserData1( ) const
{
	return mUserData1;
}

_dword Expression::GetUserData2( ) const
{
	return mUserData2;
}

_dword Expression::GetUserData3( ) const
{
	return mUserData3;
}

_dword Expression::GetUserData4( ) const
{
	return mUserData4;
}

_dword Expression::GetCalledMethodNumber( ) const
{
	return mCalledMethodNumber;
}

StringPtr Expression::GetExpression( ) const
{
	return mExpression;
}

_dword Expression::GetSectionNumber( ) const
{
	return mSectionNumber;
}

_bool Expression::IsEnglishCharacter( _char character ) const
{
	return ( character >= 'a' && character <= 'z' ) || ( character >= 'A' && character <= 'Z' );
}

_bool Expression::IsMethodOrVariableName( StringPtr name ) const
{
	const _char* address = ( const _char* ) name;

	// Thr first character can't be a string start tag (")
	if ( *address == '"' )
		return _false;

	// The end character can't be a operator 
	if ( GetOperatorFlag( name[ name.Length( ) - 1 ] ) != _OPERATOR_UNKNOWN )
		return _false;

	// Continue to search, if name contains any english code then determine it could be a variable or method name
	while ( *address != 0 )
	{
		if ( IsEnglishCharacter( *address ++ ) )
			return _true;
	}

	return _false;
}

_dword Expression::GetOperatorFlag( _char character ) const
{
	if ( character == '+' )
		return _OPERATOR_ADD;
	else if ( character == '-' )
		return _OPERATOR_SUB;
	else if ( character == '*' )
		return _OPERATOR_MUL;
	else if ( character == '/' )
		return _OPERATOR_DIV;
	else if ( character >= _OPERATOR_BIG && character <= _OPERATOR_OR )
		return character;

	return _OPERATOR_UNKNOWN;
}

_char Expression::GetOperatorCode( _dword operatorflag ) const
{
	if ( operatorflag == _OPERATOR_ADD )
		return '+';
	else if ( operatorflag == _OPERATOR_SUB )
		return '-';
	else if ( operatorflag == _OPERATOR_MUL )
		return '*';
	else if ( operatorflag == _OPERATOR_DIV )
		return '/';
	else if ( operatorflag >= _OPERATOR_BIG && operatorflag <= _OPERATOR_OR )
		return _char( operatorflag );

	return 0;
}

_long Expression::CompareOperator( _dword op1, _dword op2 ) const
{
	_chara symbol = sOperatorPriorityTable[ op1 ][ op2 ];

	if ( symbol == '>' )
		return 1;
	else if ( symbol == '<' )
		return -1;
	else
		return 0;
}

}
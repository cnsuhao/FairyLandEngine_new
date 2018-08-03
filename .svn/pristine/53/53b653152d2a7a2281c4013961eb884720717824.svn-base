//============================================================================
// Variable.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// Variable
//----------------------------------------------------------------------------

class Variable
{
public:
	enum _VALUE_TYPE
	{
		_TYPE_UNKNOWN	= 0x0000,
		_TYPE_CHAR		= 0x0001,
		_TYPE_TINY		= 0x0002,
		_TYPE_SHORT		= 0x0003,
		_TYPE_LONG		= 0x0004,
		_TYPE_BYTE		= 0x0005,
		_TYPE_WORD		= 0x0006,
		_TYPE_DWORD		= 0x0007,
		_TYPE_FLOAT		= 0x0008,
		_TYPE_STRING	= 0x0009,
		_TYPE_STRUCT	= 0x000A,

		_TYPE_MASK		= 0x00FF,

		_TYPE_ARRAY		= 0x0100,
	};

	static Variable cInvalidVariable;

private:
	typedef Array< Variable* > VariableArray;

	String			mName;
	_dword			mType;

	union
	{
		_char		mChar;
		_tiny		mTiny;
		_short		mShort;
		_long		mLong;
		_byte		mByte;
		_word		mWord;
		_dword		mDword;
		_float		mFloat;
		_char*		mString;
	};

	Variable*		mVariableTemplate;
	Variable*		mVariableNumber;
	VariableArray	mVariableArray;

	Variable( const Variable& );
	Variable& operator = ( const Variable& );

	_void CopyString( StringPtr string );
	_void SplitNameL2R( const _char* name, _char* buffer, _char*& prefix, _char*& postfix ) const;
	_void SplitNameR2L( const _char* name, _char* buffer, _char*& prefix, _char*& postfix ) const;
	Variable* CloneVariable( StringPtr name, const Variable& variabletemplate );

public:
	Variable( StringPtr name = L"", _dword type = _TYPE_UNKNOWN );
	~Variable( );

	_bool operator == ( const Variable& variable ) const
		{ return mName == variable.mName; }
	_bool operator > ( const Variable& variable ) const
		{ return mName > variable.mName; }
	_bool operator < ( const Variable& variable ) const
		{ return mName < variable.mName; }

	static StringPtr Type2String( _dword type );
	static _dword String2Type( StringPtr type );

	// Get variable name.
	inline StringPtr GetName( ) const
		{ return mName; }

	// Get variable type.
	inline _dword GetType( ) const
		{ return mType; }

	// Get if the variable is valid.
	inline _bool IsValid( ) const
		{ return mType != _TYPE_UNKNOWN; }

	// Get if the variable value has not been set.
	inline _bool IsInitial( ) const
		{ return mDword == 0; }

	// Get variable template.
	inline const Variable* GetTemplate( ) const
		{ return mVariableTemplate; }

	// Get variable element number.
	inline _dword GetElementNumber( ) const
		{ return mVariableArray.Number( ); }

	// Get variable element.
	inline const Variable* GetElement( _dword index ) const
		{ return mVariableArray[ index ]; }

	// Convert variable to _char.
	inline operator _char ( ) const
		{ return mType == _TYPE_CHAR ? mChar : 0; }

	// Convert variable to _tiny.
	inline operator _tiny ( ) const
		{ return mType == _TYPE_TINY || mType == _TYPE_BYTE ? mTiny : 0; }

	// Convert variable to _short.
	inline operator _short ( ) const
		{ return mType == _TYPE_SHORT || mType == _TYPE_WORD ? mShort : 0; }

	// Convert variable to _long.
	inline operator _long ( ) const
		{ return mType == _TYPE_LONG || mType == _TYPE_DWORD ? mLong : 0; }

	// Convert variable to _byte.
	inline operator _byte ( ) const
		{ return mType == _TYPE_TINY || mType == _TYPE_BYTE ? mByte : 0; }

	// Convert variable to _word.
	inline operator _word ( ) const
		{ return mType == _TYPE_SHORT || mType == _TYPE_WORD ? mWord : 0; }

	// Convert variable to _dword.
	inline operator _dword ( ) const
		{ return mType == _TYPE_LONG || mType == _TYPE_DWORD ? mDword : 0; }

	// Convert variable to _float.
	inline operator _float ( ) const
		{ return mType == _TYPE_FLOAT ? mFloat : 0.0f; }

	// Convert variable to string.
	inline operator const _char* ( ) const
		{ return mType == _TYPE_STRING && mString != _null ? mString : L""; }

	// Convert variable to string.
	inline operator StringPtr ( ) const
		{ return mType == _TYPE_STRING && mString != _null ? mString : L""; }

	// Copy _char value to variable.
	inline Variable& operator = ( _char value )
		{ if ( mType == _TYPE_CHAR ) mChar = value; return *this; }

	// Copy _tiny value to variable.
	inline Variable& operator = ( _tiny value )
		{ if ( mType == _TYPE_TINY || mType == _TYPE_BYTE ) mTiny = value; return *this; }

	// Copy _short value to variable.
	inline Variable& operator = ( _short value )
		{ if ( mType == _TYPE_SHORT || mType == _TYPE_WORD ) mShort = value; return *this; }

	// Copy _long value to variable.
	inline Variable& operator = ( _long value )
		{ if ( mType == _TYPE_LONG || mType == _TYPE_DWORD ) mLong = value; return *this; }

	// Copy _byte value to variable.
	inline Variable& operator = ( _byte value )
		{ if ( mType == _TYPE_TINY || mType == _TYPE_BYTE ) mByte = value; return *this; }

	// Copy _word value to variable.
	inline Variable& operator = ( _word value )
		{ if ( mType == _TYPE_SHORT || mType == _TYPE_WORD ) mWord = value; return *this; }

	// Copy _dword value to variable.
	inline Variable& operator = ( _dword value )
		{ if ( mType == _TYPE_LONG || mType == _TYPE_DWORD ) mDword = value; return *this; }

	// Copy _float value to variable.
	inline Variable& operator = ( _float value )
		{ if ( mType == _TYPE_FLOAT ) mFloat = value; return *this; }

	// Copy string to variable.
	inline Variable& operator = ( StringPtr string )
		{ if ( mType == _TYPE_STRING ) CopyString( string ); return *this; }

	// Get sub variable by name.
	const Variable& operator [] ( const _char* name ) const;
	const Variable& operator () ( const _char* name, _dword i ) const;
	const Variable& operator () ( const _char* name, _dword i, _dword j ) const;
	const Variable& operator () ( const _char* name, _dword i, _dword j, _dword k ) const;

	// Get sub variable by name.
	Variable& operator [] ( const _char* name );
	Variable& operator () ( const _char* name, _dword i );
	Variable& operator () ( const _char* name, _dword i, _dword j );
	Variable& operator () ( const _char* name, _dword i, _dword j, _dword k );

	// Redefine the variable.
	Variable& Redeclare( StringPtr name, _dword type );

	// Create variable member or array, just like declaration.
	Variable& DefineMember( StringPtr name, _dword type );

	// Create variable element in array.
	Variable& CreateElement( StringPtr name, _dword number );

	// Delete variable element in array.
	_bool DeleteElement( StringPtr name, _dword index );

	// Clear variable element in array.
	_bool ClearElement( StringPtr name );

	// Clear variable itself.
	_void Clear( );
};

//----------------------------------------------------------------------------
// VariableOperation
//----------------------------------------------------------------------------

class VariableOperation
{
public:
	enum _OPERATION_TYPE
	{
		_OPERATION_NONE				= 0,
		_OPERATION_REDECLARE		= 1,
		_OPERATION_DEFINE_MEMBER	= 2,
		_OPERATION_CREATE_ELEMENT	= 3,
		_OPERATION_DELETE_ELEMENT	= 4,
		_OPERATION_UPDATE_VARIABLE	= 5,
		_OPERATION_QUERY_VARIABLE	= 6,
	};

private:
	typedef Triple< _dword, String, String >	Operation;
	typedef Array< Operation >					OperationArray;

	_dword			mUserData1;
	_dword			mUserData2;
	_dword			mUserData3;
	_dword			mUserData4;

	_dword			mRedOptNumber;
	_dword			mDefOptNumber;
	_dword			mApdOptNumber;
	_dword			mDelOptNumber;
	_dword			mUpdOptNumber;
	_dword			mQueOptNumber;

	OperationArray	mOperationArray;

	_bool GetVariableType( _char* buffer, _char*& value, const _char* string ) const;
	_void AppendUpdateOperation( StringPtr name, StringPtr variable );

public:
	VariableOperation( );
	~VariableOperation( );

	static StringPtr Type2String( _dword type );
	static _dword String2Type( StringPtr type );

	_void  SetUserData1( _dword userdata );
	_dword GetUserData1( ) const;
	_void  SetUserData2( _dword userdata );
	_dword GetUserData2( ) const;
	_void  SetUserData3( _dword userdata );
	_dword GetUserData3( ) const;
	_void  SetUserData4( _dword userdata );
	_dword GetUserData4( ) const;

	_dword    GetOperationNumber( ) const;
	_dword    GetRedeclareOperationNumber( ) const;
	_dword    GetDefineOperationNumber( ) const;
	_dword    GetCreateOperationNumber( ) const;
	_dword    GetDeleteOperationNumber( ) const;
	_dword    GetUpdateOperationNumber( ) const;
	_dword    GetQueryOperationNumber( ) const;

	_dword    GetOperationType( _dword index ) const;
	StringPtr GetOperationParam1( _dword index ) const;
	StringPtr GetOperationParam2( _dword index ) const;

	_void Redeclare( StringPtr name, _dword type );

	_void DefineMember( StringPtr name, _dword type );
	_void DefineMember( StringPtr name, _dword type, _char value );
	_void DefineMember( StringPtr name, _dword type, _tiny value );
	_void DefineMember( StringPtr name, _dword type, _short value );
	_void DefineMember( StringPtr name, _dword type, _long value );
	_void DefineMember( StringPtr name, _dword type, _byte value );
	_void DefineMember( StringPtr name, _dword type, _word value );
	_void DefineMember( StringPtr name, _dword type, _dword value );
	_void DefineMember( StringPtr name, _dword type, _float value );
	_void DefineMember( StringPtr name, _dword type, StringPtr string );

	_void CreateElement( StringPtr name, _dword number );
	_void DeleteElement( StringPtr name, _dword index );

	_void UpdateVariable( StringPtr name, _char value );
	_void UpdateVariable( StringPtr name, _tiny value );
	_void UpdateVariable( StringPtr name, _short value );
	_void UpdateVariable( StringPtr name, _long value );
	_void UpdateVariable( StringPtr name, _byte value );
	_void UpdateVariable( StringPtr name, _word value );
	_void UpdateVariable( StringPtr name, _dword value );
	_void UpdateVariable( StringPtr name, _float value );
	_void UpdateVariable( StringPtr name, StringPtr string );
	_void UpdateVariableWithoutType( StringPtr name, StringPtr variable );

	_void QueryVariable( StringPtr name, StringPtr filter = L"" );

	_bool Operate( Variable& variable ) const;
	_bool Query( Variable& variable, VariableOperation& result ) const;

	_bool FromBuffer( const _byte* buffer, _dword length );
	_bool ToBinBuffer( MemFile& buffer ) const;
	_bool ToTxtBuffer( MemFile& buffer ) const;

	_void Clear( );
};

//----------------------------------------------------------------------------
// VariableSaver
//----------------------------------------------------------------------------

class VariableSaver
{
public:
	static _bool SaveVariable( StringPtr prefix, StringPtr filter, const Variable& variable, VariableOperation& operation, _bool subelement = _false );
	static _bool SaveVariable( StringPtr filename, const Variable& variable, _bool bin = _false );
};

//----------------------------------------------------------------------------
// VariableLoader
//----------------------------------------------------------------------------

class VariableLoader
{
public:
	static _bool LoadVariable( StringPtr filename, Variable& variable );
};

};
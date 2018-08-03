//! @file     TinyVariable.h
//! @author   Deen
//! @version  1.0
//! @date     2007.10
//! Copyright 2006-2009 ??? Incorporated.

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// TinyVariable
//----------------------------------------------------------------------------

class TinyVariable
{
public:
	static const TinyVariable cNull;
	static const TinyVariable cTrue;
	static const TinyVariable cFalse;

public:
	enum _TYPE
	{
		_TYPE_INTEGER_OP_FLOAT	= 0x00000001, // 0001
		_TYPE_STRING			= 0x00000002, // 0010
		_TYPE_FLOAT				= 0x00000005, // 0101
		_TYPE_INTEGER			= 0x00000009, // 1001

		_TYPE_MASK				= 0x000000FF,

		_TYPE_SIGNED			= 0x00010000,
	};

	enum _TYPE_ID
	{
		_TYPE_ID_UNKNOWN	= 0,											// Unknown
		_TYPE_ID_INT8		= 0x00100000 | _TYPE_INTEGER | _TYPE_SIGNED,	// Signed 8-bits integer
		_TYPE_ID_INT16		= 0x00200000 | _TYPE_INTEGER | _TYPE_SIGNED,	// Signed 16-bits integer
		_TYPE_ID_INT32		= 0x00400000 | _TYPE_INTEGER | _TYPE_SIGNED,	// Signed 32-bits integer
		_TYPE_ID_UINT8		= 0x00100000 | _TYPE_INTEGER,					// Unsigned 8-bits integer
		_TYPE_ID_UINT16		= 0x00200000 | _TYPE_INTEGER,					// Unsigned 16-bits integer
		_TYPE_ID_UINT32		= 0x00400000 | _TYPE_INTEGER,					// Unsigned 32-bits integer
		_TYPE_ID_FLOAT		= 0x00800000 | _TYPE_FLOAT,						// Float
		_TYPE_ID_DOUBLE		= 0x01000000 | _TYPE_FLOAT,						// Double
		_TYPE_ID_STRING		= 0x02000000 | _TYPE_STRING,					// String
		_TYPE_ID_STRING_PTR	= 0x04000000 | _TYPE_STRING,					// StringPtr
		_TYPE_ID_POINTER	= 0x08000000,									// Pointer
		_TYPE_ID_USERDATA	= 0x10000000,									// Userdata
	};

public:
	inline operator _tiny( ) const;
	inline operator _byte( ) const;
	inline operator _short( ) const;
	inline operator _word( ) const;
	inline operator _long( ) const;
	inline operator _dword( ) const;
	inline operator StringPtr( ) const;
	inline operator _float( ) const;
	inline operator _double( ) const;
	inline operator _void*( ) const;
	inline operator const _char*( ) const;

	inline const TinyVariable& operator = ( _tiny value );
	inline const TinyVariable& operator = ( _byte value );
	inline const TinyVariable& operator = ( _short value );
	inline const TinyVariable& operator = ( _word value );
	inline const TinyVariable& operator = ( _long value );
	inline const TinyVariable& operator = ( _dword value );
	inline const TinyVariable& operator = ( const _char* string );
	inline const TinyVariable& operator = ( StringPtr string );
	inline const TinyVariable& operator = ( _float value );
	inline const TinyVariable& operator = ( _double value );
	inline const TinyVariable& operator = ( _void* pointer );
	inline const TinyVariable& operator = ( const _void* pointer );
	inline const TinyVariable& operator = ( const TinyVariable& variable );

	inline const TinyVariable& operator ++ ( _long );
	inline const TinyVariable& operator -- ( _long );

	inline const TinyVariable& operator += ( const TinyVariable& variable );
	inline const TinyVariable& operator -= ( const TinyVariable& variable );
	inline const TinyVariable& operator *= ( const TinyVariable& variable );
	inline const TinyVariable& operator /= ( const TinyVariable& variable );
	inline const TinyVariable& operator &= ( const TinyVariable& variable );
	inline const TinyVariable& operator |= ( const TinyVariable& variable );

	inline TinyVariable 		operator - ( ) const;

	inline _bool 				operator <  ( const TinyVariable& variable ) const;
	inline _bool 				operator <= ( const TinyVariable& variable ) const;
	inline _bool 				operator == ( const TinyVariable& variable ) const;
	inline _bool 				operator != ( const TinyVariable& variable ) const;
	inline _bool 				operator >  ( const TinyVariable& variable ) const;
	inline _bool 				operator >= ( const TinyVariable& variable ) const;

	inline _bool 				operator && ( const TinyVariable& variable ) const;
	inline _bool 				operator || ( const TinyVariable& variable ) const;

	inline _bool				IsPointer( ) const;
	inline _bool				IsStringType( ) const;
	inline _bool				IsFloatType( ) const;
	inline _bool				IsZero( ) const;
	inline _bool				IsSigned( ) const;

	inline _dword				GetHiDword( ) const;
	inline _void				SetHiDword( _dword value );

	inline String				ToString( ) const;

	inline _long				CompareValue( const TinyVariable& variable ) const;

	inline _dword				GetType( ) const;
	inline _void				Clear( _bool freememory = _false );

	inline _void				CloneString( const _char* string, _dword stringlength );
	inline _void				CloneString( const TinyVariable& variable );

	inline _dword				GetStringLength( ) const;
	inline StringPtr			GetStringPtr( ) const;

public:
	inline TinyVariable( );
	inline TinyVariable( _dword type, _qword value );
	inline TinyVariable( _tiny value );
	inline TinyVariable( _byte value );
	inline TinyVariable( _short value );
	inline TinyVariable( _word value );
	inline TinyVariable( _long value );
	inline TinyVariable( _dword value );
	inline TinyVariable( const _char* string );
	inline TinyVariable( StringPtr string );
	inline TinyVariable( _float value );
	inline TinyVariable( _double value );
	inline TinyVariable( _void* pointer );
	inline TinyVariable( const _void* pointer );
	inline TinyVariable( const TinyVariable& variable );
	inline ~TinyVariable( );

private:
	union
	{
		// String: High 64bits( High 16 bits - size, Low 16 bits - string length )
		struct
		{
			_qword	mValue;
		};

		struct
		{
			_double mDoubleValue;
		};

		struct
		{
			_dword	mLowValue;
			_dword	mHighValue;
		};
	};

	union
	{
		struct
		{
			_dword	mType;
		};

		struct
		{
			_word	mLowType;
			_word	mHighType;
		};
	};

private:
	inline _dword Type( const TinyVariable& variable ) const;
	inline _dword Size( ) const;

	inline _void CleanString( );

	_void CopyString( const _char* string, _dword stringlength );
	_void AppendString( const _char* string, _dword stringlength );
};

//----------------------------------------------------------------------------
// TinyVariable Implementation
//----------------------------------------------------------------------------

TinyVariable::TinyVariable( )
{
	mType	= _TYPE_ID_UNKNOWN;
	mValue	= 0;
}

TinyVariable::TinyVariable( _dword type, _qword value )
{
	mType	= type;
	mValue	= value;
}

TinyVariable::TinyVariable( _tiny value )
{
	mType	= _TYPE_ID_INT8;
	mValue	= value;
}

TinyVariable::TinyVariable( _byte value )
{
	mType	= _TYPE_ID_UINT8;
	mValue	= value;
}

TinyVariable::TinyVariable( _short value )
{
	mType	= _TYPE_ID_INT16;
	mValue	= value;
}

TinyVariable::TinyVariable( _word value )
{
	mType	= _TYPE_ID_UINT16;
	mValue	= value;
}

TinyVariable::TinyVariable( _long value )
{
	mType	= _TYPE_ID_INT32;
	mValue	= value;
}

TinyVariable::TinyVariable( _dword value )
{
	mType	= _TYPE_ID_UINT32;
	mValue	= value;
}

TinyVariable::TinyVariable( const _char* string )
{
	mType	= _TYPE_ID_UNKNOWN;
	mValue	= 0;

	CopyString( string, StringFormatter::StringLength( string ) );
}

TinyVariable::TinyVariable( StringPtr string )
{
	mType	= _TYPE_ID_UNKNOWN;
	mValue	= 0;

	CopyString( string, string.Length( ) );
}

TinyVariable::TinyVariable( _float value )
{
	mType			= _TYPE_ID_FLOAT;
	mDoubleValue	= (_double) value;
}

TinyVariable::TinyVariable( _double value )
{
	mType			= _TYPE_ID_DOUBLE;
	mDoubleValue	= value;
}

TinyVariable::TinyVariable( _void* pointer )
{
	mType	= _TYPE_ID_POINTER;
	mValue	= _dword( pointer );
}

TinyVariable::TinyVariable( const _void* pointer )
{
	mType	= _TYPE_ID_POINTER;
	mValue	= _dword( pointer );
}

TinyVariable::TinyVariable( const TinyVariable& variable )
{
	switch ( Type( variable ) )
	{
		case _TYPE_STRING:
		{
			CopyString( (const _char*) (_void*) variable, variable.GetStringLength( ) );
		}
		break;

		case _TYPE_FLOAT:
		{
			mDoubleValue = variable.mDoubleValue;
		}
		break;

		default:
		{
			mType	= variable.mType;
			mValue	= variable.mValue;
		}
		break;
	}
}

TinyVariable::~TinyVariable( )
{
	if ( mType == _TYPE_ID_STRING )
		CleanString( );
}

TinyVariable::operator _tiny( ) const
{
	if ( IsFloatType( ) )
		return (_tiny) mDoubleValue;
	else
		return (_tiny) mLowValue;
}

TinyVariable::operator _byte( ) const
{
	if ( IsFloatType( ) )
		return (_byte) mDoubleValue;
	else
		return (_byte) mLowValue;
}

TinyVariable::operator _short( ) const
{
	if ( IsFloatType( ) )
		return (_short) mDoubleValue;
	else
		return (_short) mLowValue;
}

TinyVariable::operator _word( ) const
{
	if ( IsFloatType( ) )
		return (_word) mDoubleValue;
	else
		return (_word) mLowValue;
}

TinyVariable::operator _long( ) const
{
	if ( IsFloatType( ) )
		return (_long) mDoubleValue;
	else
		return (_long) mLowValue;
}

TinyVariable::operator _dword( ) const
{
	if ( IsFloatType( ) )
		return (_dword) mDoubleValue;
	else
		return mLowValue;
}

TinyVariable::operator StringPtr( ) const
{
	if ( IsStringType( ) )
		return (const _char*) mLowValue;
	else
		return L"";
}

TinyVariable::operator _float( ) const
{
	if ( IsFloatType( ) )
		return (_float) mDoubleValue;

	if ( IsSigned( ) )
		return (_float) (_long) mLowValue;
	else
		return (_float) mLowValue;
}

TinyVariable::operator _double( ) const
{
	if ( IsFloatType( ) )
		return mDoubleValue;

	if ( IsSigned( ) )
		return (_double) (_long) mLowValue;
	else
		return (_double) mLowValue;
}

TinyVariable::operator _void*( ) const
{
	if ( IsStringType( ) )
		return (_void*) (const _char*) mLowValue;
	else
		return (_void*) mLowValue;
}

TinyVariable::operator const _char*( ) const
{
	if ( IsStringType( ) )
		return (const _char*) mLowValue;
	else
		return L"";
}

const TinyVariable& TinyVariable::operator = ( _tiny value )
{
	if ( mType == _TYPE_ID_STRING )
		CleanString( );

	mType		= _TYPE_ID_INT8;
	mLowValue	= value;

	return *this;
}

const TinyVariable& TinyVariable::operator = ( _byte value )
{
	if ( mType == _TYPE_ID_STRING )
		CleanString( );

	mType		= _TYPE_ID_UINT8;
	mLowValue	= value;

	return *this;
}

const TinyVariable& TinyVariable::operator = ( _short value )
{
	if ( mType == _TYPE_ID_STRING )
		CleanString( );

	mType		= _TYPE_ID_INT16;
	mLowValue	= value;

	return *this;
}

const TinyVariable& TinyVariable::operator = ( _word value )
{
	if ( mType == _TYPE_ID_STRING )
		CleanString( );

	mType		= _TYPE_ID_UINT16;
	mLowValue	= value;

	return *this;
}

const TinyVariable& TinyVariable::operator = ( _long value )
{
	if ( mType == _TYPE_ID_STRING )
		CleanString( );

	mType		= _TYPE_ID_INT32;
	mLowValue	= value;

	return *this;
}

const TinyVariable& TinyVariable::operator = ( _dword value )
{
	if ( mType == _TYPE_ID_STRING )
		CleanString( );

	mType		= _TYPE_ID_UINT32;
	mLowValue	= value;

	return *this;
}

const TinyVariable& TinyVariable::operator = ( const _char* string )
{
	CopyString( string, StringFormatter::StringLength( string ) );

	return *this;
}

const TinyVariable& TinyVariable::operator = ( StringPtr string )
{
	CopyString( string, string.Length( ) );

	return *this;
}

const TinyVariable& TinyVariable::operator = ( _float value )
{
	if ( mType == _TYPE_ID_STRING )
		CleanString( );

	mType			= _TYPE_ID_FLOAT;
	mDoubleValue	= (_double) value;

	return *this;
}

const TinyVariable& TinyVariable::operator = ( _double value )
{
	if ( mType == _TYPE_ID_STRING )
		CleanString( );

	mType			= _TYPE_ID_DOUBLE;
	mDoubleValue	= value;

	return *this;
}

const TinyVariable& TinyVariable::operator = ( _void* pointer )
{
	if ( mType == _TYPE_ID_STRING )
		CleanString( );

	// Don't overlap high 32 bits
	mType		= _TYPE_ID_POINTER;
	mLowValue	= _dword( pointer ); 

	return *this;
}

const TinyVariable& TinyVariable::operator = ( const _void* pointer )
{
	return operator = ( (_void*) pointer );
}

const TinyVariable& TinyVariable::operator = ( const TinyVariable& variable )
{
	switch ( Type( variable ) )
	{
		case _TYPE_INTEGER_OP_FLOAT:
		{
			// Float = Integer
			if ( IsFloatType( ) )
			{
				mType			= variable.mType;
				mLowValue		= variable.mLowValue;
			}
			// Integer = Float
			else
			{
				mType			= _TYPE_ID_FLOAT;
				mDoubleValue	= variable.mDoubleValue;
			}
		}
		break;

		case _TYPE_STRING:
		{
			CopyString( (const _char*) (_void*) variable, variable.GetStringLength( ) );
		}
		break;

		case _TYPE_FLOAT:
		{
			mDoubleValue = variable.mDoubleValue;
		}
		break;

		case _TYPE_INTEGER:
		{
			mType		= variable.mType;
			mLowValue	= variable.mLowValue;
		}
		break;

		default:
		{
			if ( mType == _TYPE_ID_STRING )
				CleanString( );

			if ( variable.IsStringType( ) )
			{
				CopyString( (const _char*) (_void*) variable, variable.GetStringLength( ) );
			}
			else
			{
				mType	= variable.mType;
				mValue	= variable.mValue;
			}
		}
		break;
	}

	return *this;
}

const TinyVariable& TinyVariable::operator ++ ( _long )
{
	switch ( mType & _TYPE_MASK ) 
	{
		case _TYPE_FLOAT:
		{
			mDoubleValue = mDoubleValue + 1.0;
		}
		break;

		case _TYPE_INTEGER:
		{
			mLowValue ++;
		}
		break;
	}

	return *this;
}

const TinyVariable& TinyVariable::operator -- ( _long )
{
	switch ( mType & _TYPE_MASK ) 
	{
		case _TYPE_FLOAT:
		{
			mDoubleValue = mDoubleValue - 1.0;
		}
		break;

		case _TYPE_INTEGER:
		{
			mLowValue --;
		}
		break;
	}

	return *this;
}

const TinyVariable& TinyVariable::operator += ( const TinyVariable& variable )
{
	switch ( Type( variable ) )
	{
		case _TYPE_INTEGER_OP_FLOAT:
		{
			// Float + Integer
			if ( IsFloatType( ) )
			{
				if ( variable.IsSigned( ) )
					mDoubleValue = mDoubleValue + (_double) (_long) variable.mLowValue;
				else
					mDoubleValue = mDoubleValue + (_double) variable.mLowValue;
			}
			// Integer + Float 
			else
			{
				if ( IsSigned( ) )
					mDoubleValue = ( (_double) (_long) mLowValue ) + variable.mDoubleValue;
				else
					mDoubleValue = ( (_double) mLowValue ) + variable.mDoubleValue;

				mType = _TYPE_ID_FLOAT;
			}
		}
		break;

		case _TYPE_STRING:
		{
			AppendString( (const _char*) (_void*) variable, variable.GetStringLength( ) );
		}
		break;

		case _TYPE_FLOAT:
		{
			mDoubleValue = mDoubleValue + variable.mDoubleValue;
		}
		break;

		case _TYPE_INTEGER:
		{
			mLowValue = mLowValue + variable.mLowValue;

			if ( _long( mLowValue ) < 0 )
				mType |= _TYPE_SIGNED;
			else
				mType &= ~_TYPE_SIGNED;
		}
		break;
	}

	return *this;
}

const TinyVariable& TinyVariable::operator -= ( const TinyVariable& variable )
{
	switch ( Type( variable ) )
	{
		case _TYPE_INTEGER_OP_FLOAT:
		{	
			// Float - Integer
			if ( IsFloatType( ) )
			{
				if ( variable.IsSigned( ) )
					mDoubleValue = mDoubleValue - (_double) (_long) variable.mLowValue;
				else
					mDoubleValue = mDoubleValue - (_double) variable.mLowValue;
			}
			// Integer - Float 
			else
			{
				if ( IsSigned( ) )
					mDoubleValue = ( (_double) (_long) mLowValue ) - variable.mDoubleValue;
				else
					mDoubleValue = ( (_double) mLowValue ) - variable.mDoubleValue;

				mType = _TYPE_ID_FLOAT;
			}
		}
		break;

		case _TYPE_STRING:
		{
		}
		break;

		case _TYPE_FLOAT:
		{
			mDoubleValue = mDoubleValue - variable.mDoubleValue;
		}
		break;

		case _TYPE_INTEGER:
		{
			mLowValue = mLowValue - variable.mLowValue;

			if ( _long( mLowValue ) < 0 )
				mType |= _TYPE_SIGNED;
			else
				mType &= ~_TYPE_SIGNED;
		}
		break;
	}

	return *this;
}

const TinyVariable& TinyVariable::operator *= ( const TinyVariable& variable )
{
	switch ( Type( variable ) )
	{
		case _TYPE_INTEGER_OP_FLOAT:
		{
			// Float * Integer
			if ( IsFloatType( ) )
			{
				if ( variable.IsSigned( ) )
					mDoubleValue = mDoubleValue * (_double) (_long) variable.mLowValue;
				else
					mDoubleValue = mDoubleValue * (_double) variable.mLowValue;
			}
			// Integer * Float 
			else
			{
				if ( IsSigned( ) )
					mDoubleValue = ( (_double) (_long) mLowValue ) * variable.mDoubleValue;
				else
					mDoubleValue = ( (_double) mLowValue ) * variable.mDoubleValue;

				mType = _TYPE_ID_FLOAT;
			}
		}
		break;

		case _TYPE_STRING:
		{
		}
		break;

		case _TYPE_FLOAT:
		{
			mDoubleValue = mDoubleValue * variable.mDoubleValue;
		}
		break;

		case _TYPE_INTEGER:
		{
			mLowValue = mLowValue * variable.mLowValue;

			if ( _long( mLowValue ) < 0 )
				mType |= _TYPE_SIGNED;
			else
				mType &= ~_TYPE_SIGNED;
		}
		break;
	}

	return *this;
}

const TinyVariable& TinyVariable::operator /= ( const TinyVariable& variable )
{
	switch ( Type( variable ) )
	{
		case _TYPE_INTEGER_OP_FLOAT:
		{
			// Float / Integer
			if ( IsFloatType( ) )
			{
				if ( variable.IsSigned( ) )
					mDoubleValue = mDoubleValue / (_double) (_long) variable.mLowValue;
				else
					mDoubleValue = mDoubleValue / (_double) variable.mLowValue;
			}
			// Integer / Float 
			else
			{
				if ( IsSigned( ) )
					mDoubleValue = ( (_double) (_long) mLowValue ) / variable.mDoubleValue;
				else
					mDoubleValue = ( (_double) mLowValue ) / variable.mDoubleValue;

				mType = _TYPE_ID_FLOAT;
			}
		}
		break;

		case _TYPE_STRING:
		{
		}
		break;

		case _TYPE_FLOAT:
		{
			mDoubleValue = mDoubleValue / variable.mDoubleValue;
		}
		break;

		case _TYPE_INTEGER:
		{
			mLowValue = mLowValue / variable.mLowValue;

			if ( _long( mValue ) < 0 )
				mType |= _TYPE_SIGNED;
			else
				mType &= ~_TYPE_SIGNED;
		}
		break;
	}

	return *this;
}

const TinyVariable& TinyVariable::operator &= ( const TinyVariable& variable )
{
	if ( Type( variable ) == _TYPE_INTEGER )
		mLowValue &= variable.mLowValue;

	return *this;
}

const TinyVariable& TinyVariable::operator |= ( const TinyVariable& variable )
{
	if ( Type( variable ) == _TYPE_INTEGER )
		mLowValue |= variable.mLowValue;

	return *this;
}

TinyVariable TinyVariable::operator - ( ) const
{
	if ( IsStringType( ) )
		return 0;

	if ( IsFloatType( ) )
		return - mDoubleValue;

	return - _long( mLowValue );
}

_bool TinyVariable::operator < ( const TinyVariable& variable ) const
{
	switch ( Type( variable ) )
	{
		case _TYPE_INTEGER_OP_FLOAT:
		{
			// Float < Integer
			if ( IsFloatType( ) )
			{
				if ( variable.IsSigned( ) )
					return (_long) mDoubleValue < (_long) variable.mLowValue;
				else
					return (_dword) mDoubleValue < variable.mLowValue;
			}
			// Integer < Float 
			else
			{
				if ( IsSigned( ) )
					return (_long) mLowValue < (_long) variable.mDoubleValue;
				else
					return mLowValue < (_dword) variable.mDoubleValue;
			}
		}
		break;

		case _TYPE_STRING:
		{
			return StringPtr( (const _char*) mLowValue ) < StringPtr( (const _char*) variable.mLowValue );
		}
		break;

		case _TYPE_FLOAT:
		{
			return mDoubleValue < variable.mDoubleValue;
		}
		break;

		case _TYPE_INTEGER:
		{
			return mLowValue < variable.mLowValue;
		}
		break;
	}

	// Compare value
	return mLowValue < variable.mLowValue;
}

_bool TinyVariable::operator <= ( const TinyVariable& variable ) const
{
	switch ( Type( variable ) )
	{
		case _TYPE_INTEGER_OP_FLOAT:
		{
			// Float <= Integer
			if ( IsFloatType( ) )
			{
				if ( variable.IsSigned( ) )
					return (_long) mDoubleValue <= (_long) variable.mLowValue;
				else
					return (_dword) mDoubleValue <= variable.mLowValue;
			}
			// Integer <= Float 
			else
			{
				if ( IsSigned( ) )
					return (_long) mLowValue <= (_long) variable.mDoubleValue;
				else
					return mLowValue <= (_dword) variable.mDoubleValue;
			}
		}
		break;

		case _TYPE_STRING:
		{
			return StringPtr( (const _char*) mLowValue ) <= StringPtr( (const _char*) variable.mLowValue );
		}
		break;

		case _TYPE_FLOAT:
		{
			return mDoubleValue <= variable.mDoubleValue;
		}
		break;

		case _TYPE_INTEGER:
		{
			return mLowValue <= variable.mLowValue;
		}
		break;
	}

	// Compare value
	return mLowValue <= variable.mLowValue;
}

_bool TinyVariable::operator == ( const TinyVariable& variable ) const
{
	switch ( Type( variable ) )
	{
		case _TYPE_INTEGER_OP_FLOAT:
		{
			// Float == Integer
			if ( IsFloatType( ) )
			{
				if ( variable.IsSigned( ) )
					return (_long) mDoubleValue == (_long) variable.mLowValue;
				else
					return (_dword) mDoubleValue == variable.mLowValue;
			}
			// Integer == Float 
			else
			{
				if ( IsSigned( ) )
					return (_long) mLowValue == (_long) variable.mDoubleValue;
				else
					return mLowValue == (_dword) variable.mDoubleValue;
			}
		}
		break;

		case _TYPE_STRING:
		{
			return StringPtr( (const _char*) mLowValue ) == StringPtr( (const _char*) variable.mLowValue );
		}
		break;

		case _TYPE_FLOAT:
		{
			if ( mDoubleValue > variable.mDoubleValue )
				return mDoubleValue - variable.mDoubleValue < Math::cEpsilon;
			else
				return variable.mDoubleValue - mDoubleValue < Math::cEpsilon;
		}
		break;

		case _TYPE_INTEGER:
		{
			return mLowValue == variable.mLowValue;
		}
		break;
	}

	// Compare value
	return mLowValue == variable.mLowValue;
}

_bool TinyVariable::operator != ( const TinyVariable& variable ) const
{
	switch ( Type( variable ) )
	{
		case _TYPE_INTEGER_OP_FLOAT:
		{
			// Float != Integer
			if ( IsFloatType( ) )
			{
				if ( variable.IsSigned( ) )
					return (_long) mDoubleValue != (_long) variable.mLowValue;
				else
					return (_dword) mDoubleValue != variable.mLowValue;
			}
			// Integer != Float 
			else
			{
				if ( IsSigned( ) )
					return (_long) mLowValue != (_long) variable.mDoubleValue;
				else
					return mLowValue != (_dword) variable.mDoubleValue;
			}
		}
		break;

		case _TYPE_STRING:
		{
			return StringPtr( (const _char*) mLowValue ) != StringPtr( (const _char*) variable.mLowValue );
		}
		break;

		case _TYPE_FLOAT:
		{
			if ( mDoubleValue > variable.mDoubleValue )
				return mDoubleValue - variable.mDoubleValue > Math::cEpsilon;
			else
				return variable.mDoubleValue - mDoubleValue > Math::cEpsilon;
		}
		break;

		case _TYPE_INTEGER:
		{
			return mLowValue != variable.mLowValue;
		}
		break;
	}

	// Compare value
	return mLowValue != variable.mLowValue;
}

_bool TinyVariable::operator > ( const TinyVariable& variable ) const
{
	switch ( Type( variable ) )
	{
		case _TYPE_INTEGER_OP_FLOAT:
		{
			// Float > Integer
			if ( IsFloatType( ) )
			{
				if ( variable.IsSigned( ) )
					return (_long) mDoubleValue > (_long) variable.mLowValue;
				else
					return (_dword) mDoubleValue > variable.mLowValue;
			}
			// Integer > Float 
			else
			{
				if ( IsSigned( ) )
					return (_long) mLowValue > (_long) variable.mDoubleValue;
				else
					return mLowValue > (_dword) variable.mDoubleValue;
			}
		}
		break;

		case _TYPE_STRING:
		{
			return StringPtr( (const _char*) mLowValue ) > StringPtr( (const _char*) variable.mLowValue );
		}
		break;

		case _TYPE_FLOAT:
		{
			return mDoubleValue > variable.mDoubleValue;
		}
		break;

		case _TYPE_INTEGER:
		{
			return mLowValue > variable.mLowValue;
		}
		break;
	}

	// Compare value
	return mLowValue > variable.mLowValue;
}

_bool TinyVariable::operator >= ( const TinyVariable& variable ) const
{
	switch ( Type( variable ) )
	{
		case _TYPE_INTEGER_OP_FLOAT:
		{
			// Float >= Integer
			if ( IsFloatType( ) )
			{
				if ( variable.IsSigned( ) )
					return (_long) mDoubleValue >= (_long) variable.mLowValue;
				else
					return (_dword) mDoubleValue >= variable.mLowValue;
			}
			// Integer > Float 
			else
			{
				if ( IsSigned( ) )
					return (_long) mLowValue >= (_long) variable.mDoubleValue;
				else
					return mLowValue >= (_dword) variable.mDoubleValue;
			}
		}
		break;

		case _TYPE_STRING:
		{
			return StringPtr( (const _char*) mLowValue ) >= StringPtr( (const _char*) variable.mLowValue );
		}
		break;

		case _TYPE_FLOAT:
		{
			return mDoubleValue >= variable.mDoubleValue;
		}
		break;

		case _TYPE_INTEGER:
		{
			return mLowValue >= variable.mLowValue;
		}
		break;
	}

	// Compare value
	return mLowValue >= variable.mLowValue;
}

_bool TinyVariable::operator && ( const TinyVariable& variable ) const
{
	return mLowValue && variable.mLowValue;
}

_bool TinyVariable::operator || ( const TinyVariable& variable ) const
{
	return mLowValue || variable.mLowValue;
}

_bool TinyVariable::IsPointer( ) const
{
	return mType == _TYPE_ID_POINTER;
}

_bool TinyVariable::IsStringType( ) const
{
	return mType & _TYPE_STRING;
}

_bool TinyVariable::IsFloatType( ) const
{
	return mLowType == _TYPE_FLOAT;
}

_bool TinyVariable::IsZero( ) const
{
	return mValue == 0;
}

_bool TinyVariable::IsSigned( ) const
{
	return mType & _TYPE_SIGNED;
}

_dword TinyVariable::GetHiDword( ) const
{
	return mHighValue;
}

_void TinyVariable::SetHiDword( _dword value )
{
	if ( IsStringType( ) )
		return;

	mHighValue = value;
}

String TinyVariable::ToString( ) const
{
	if ( IsStringType( ) )
		return (const _char*) mValue;

	if ( IsFloatType( ) )
		return Double( mDoubleValue ).ToString( );

	if ( IsSigned( ) )
		return Long( (_long) mLowValue ).ToString( );
	else
		return Long( mLowValue ).ToString( );
}

_dword TinyVariable::GetType( ) const
{
	return mType;
}

_void TinyVariable::Clear( _bool freememory )
{
	if ( mType == _TYPE_ID_STRING )
	{
		if ( freememory == _true )
		{
			CleanString( );
		}
		else if ( mValue != 0 )
		{
			*( (_char*) mLowValue ) = 0;
		}
	}
	else
	{
		mValue	= 0;
		mType	= _TYPE_ID_UNKNOWN;
	}
}

_void TinyVariable::CloneString( const _char* string, _dword stringlength )
{
	// Clean string
	if ( mType == _TYPE_ID_STRING )
		CleanString( );

	// String, now set to StringPtr mode ( not release memory buffer, more faster )
	mType = _TYPE_ID_STRING_PTR;

	// Copy string address
	mLowValue	= _dword( string );
	mHighValue	= stringlength;
}

_void TinyVariable::CloneString( const TinyVariable& variable )
{
	CloneString( (const _char*) variable.mLowValue, variable.GetStringLength( ) );
}

_dword TinyVariable::GetStringLength( ) const
{	
	return mHighValue & 0x0000FFFF;
}

StringPtr TinyVariable::GetStringPtr( ) const
{
	if ( IsStringType( ) == _false )
		return L"";

	return (const _char*) mLowValue;
}

_dword TinyVariable::Type( const TinyVariable& variable ) const
{
	return mLowType & variable.mLowType;
}

_dword TinyVariable::Size( ) const
{
	return mHighValue >> 16;
}

_void TinyVariable::CleanString( )
{
	FL_ASSERT( mType == _TYPE_ID_STRING );

	delete[] (_char*) mLowValue;

	mType	= _TYPE_ID_UNKNOWN;
	mValue	= 0;
}

}
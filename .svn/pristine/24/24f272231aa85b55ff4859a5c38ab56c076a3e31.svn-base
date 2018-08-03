//============================================================================
// DataType.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// Tiny Implementation
//----------------------------------------------------------------------------

Tiny::Tiny( _tiny value ) : mValue( value )
{
}

Tiny::Tiny( StringPtr string )
{
	mValue = ::_wtoi( string );
}

Tiny::operator _tiny& ( )
{
	return mValue;
}

String Tiny::ToString( ) const
{
	return StringFormatter::FormatString( L"%d", mValue );
}

StringPtr Tiny::ToString( _char* buffer, _dword size ) const
{
	return StringFormatter::FormatBuffer( buffer, size, L"%d", mValue );
}

//----------------------------------------------------------------------------
// Short Implementation
//----------------------------------------------------------------------------

Short::Short( _short value ) : mValue( value )
{
}

Short::Short( StringPtr string )
{
	mValue = ::_wtoi( string );
}

Short::operator _short& ( )
{
	return mValue;
}

String Short::ToString( ) const
{
	return StringFormatter::FormatString( L"%d", mValue );
}

StringPtr Short::ToString( _char* buffer, _dword size ) const
{
	return StringFormatter::FormatBuffer( buffer, size, L"%d", mValue );
}

//----------------------------------------------------------------------------
// Long Implementation
//----------------------------------------------------------------------------

Long::Long( _long value ) : mValue( value )
{
}

Long::Long( StringPtr string )
{
	mValue = ::_wtoi( string );
}

Long::operator _long& ( )
{
	return mValue;
}

String Long::ToString( ) const
{
	return StringFormatter::FormatString( L"%d", mValue );
}

StringPtr Long::ToString( _char* buffer, _dword size ) const
{
	return StringFormatter::FormatBuffer( buffer, size, L"%d", mValue );
}

//----------------------------------------------------------------------------
// Large Implementation
//----------------------------------------------------------------------------

Large::Large( _large value ) : mValue( value )
{
}

Large::Large( StringPtr string )
{
	mValue = ::_wtoi64( string );
}

Large::operator _large& ( )
{
	return mValue;
}

String Large::ToString( ) const
{
	return StringFormatter::FormatString( L"%d", (_long) mValue );
}

StringPtr Large::ToString( _char* buffer, _dword size ) const
{
	return StringFormatter::FormatBuffer( buffer, size, L"%d", (_long) mValue );
}

//----------------------------------------------------------------------------
// Byte Implementation
//----------------------------------------------------------------------------

Byte::Byte( _byte value ) : mValue( value )
{
}

Byte::Byte( StringPtr string )
{
	_char* endpointer = _null;

	mValue = _byte( ::wcstoul( string, &endpointer, 16 ) );
}

Byte::operator _byte& ( )
{
	return mValue;
}

String Byte::ToString( ) const
{
	return StringFormatter::FormatString( L"%.2x", mValue );
}

StringPtr Byte::ToString( _char* buffer, _dword size ) const
{
	return StringFormatter::FormatBuffer( buffer, size, L"%.2x", mValue );
}

//----------------------------------------------------------------------------
// Word Implementation
//----------------------------------------------------------------------------

Word::Word( _word value ) : mValue( value )
{
}

Word::Word( _byte lobyte, _byte hibyte )
{
	( (_byte*) this )[0] = lobyte;
	( (_byte*) this )[1] = hibyte;
}

Word::Word( StringPtr string )
{
	_char* endpointer = _null;

	mValue = _word( ::wcstoul( string, &endpointer, 16 ) );
}

Word::operator _word& ( )
{
	return mValue;
}

Word Word::InverseByteOrder( ) const
{
	return Word( ( (_byte*) this )[1], ( (_byte*) this )[0] );
}

String Word::ToString( ) const
{
	return StringFormatter::FormatString( L"%.4x", mValue );
}

StringPtr Word::ToString( _char* buffer, _dword size ) const
{
	return StringFormatter::FormatBuffer( buffer, size, L"%.4x", mValue );
}

_byte Word::LoByte( ) const
{
	return ( (_byte*) this )[0];
}

_byte Word::HiByte( ) const
{
	return ( (_byte*) this )[1];
}

//----------------------------------------------------------------------------
// Dword Implementation
//----------------------------------------------------------------------------

Dword::Dword( _dword value ) : mValue( value )
{
}

Dword::Dword( _word loword, _word hiword )
{
	( (_word*) this )[0] = loword;
	( (_word*) this )[1] = hiword;
}

Dword::Dword( _byte byte1, _byte byte2, _byte byte3, _byte byte4 )
{
	( (_byte*) this )[0] = byte1;
	( (_byte*) this )[1] = byte2;
	( (_byte*) this )[2] = byte3;
	( (_byte*) this )[3] = byte4;
}

Dword::Dword( StringPtr string )
{
	_char* endpointer = _null;

	mValue = ::wcstoul( string, &endpointer, 16 );
}

Dword::operator _dword& ( )
{
	return mValue;
}

Dword Dword::InverseByteOrder( ) const
{
	return Dword( ( (_byte*) this )[3], ( (_byte*) this )[2], ( (_byte*) this )[1], ( (_byte*) this )[0] );
}

Dword Dword::CircleShift1( ) const
{
	return Dword( ( (_byte*) this )[3], ( (_byte*) this )[0], ( (_byte*) this )[1], ( (_byte*) this )[2] );
}

Dword Dword::CircleShift2( ) const
{
	return Dword( ( (_byte*) this )[2], ( (_byte*) this )[3], ( (_byte*) this )[0], ( (_byte*) this )[1] );
}

Dword Dword::CircleShift3( ) const
{
	return Dword( ( (_byte*) this )[1], ( (_byte*) this )[2], ( (_byte*) this )[3], ( (_byte*) this )[0] );
}

String Dword::ToString( ) const
{
	return StringFormatter::FormatString( L"%.8x", mValue );
}

StringPtr Dword::ToString( _char* buffer, _dword size ) const
{
	return StringFormatter::FormatBuffer( buffer, size, L"%.8x", mValue );
}

_byte Dword::Byte1( ) const
{
	return ( (_byte*) this )[0];
}

_byte Dword::Byte2( ) const
{
	return ( (_byte*) this )[1];
}

_byte Dword::Byte3( ) const
{
	return ( (_byte*) this )[2];
}

_byte Dword::Byte4( ) const
{
	return ( (_byte*) this )[3];
}

_word Dword::LoWord( ) const
{
	return ( (_word*) this )[0];
}

_word Dword::HiWord( ) const
{
	return ( (_word*) this )[1];
}

//----------------------------------------------------------------------------
// Qword Implementation
//----------------------------------------------------------------------------

Qword::Qword( _qword value ) : mValue( value )
{
}

Qword::Qword( _dword lodword, _dword hidword )
{
	( (_dword*) this )[0] = lodword;
	( (_dword*) this )[1] = hidword;
}

Qword::Qword( StringPtr string )
{
	_dword length = string.Length( );

	if ( length > 8 )
	{
		( (_dword*) this )[0] = Dword( string.SubString( length - 8 ) );

		_char buffer[ 16 ];

		if ( length >= 16 )
			( (_dword*) this )[1] = Dword( StringFormatter::CopyString( buffer, string.SubString( length - 16 ), 8 ) );
		else
			( (_dword*) this )[1] = Dword( StringFormatter::CopyString( buffer, string, length - 8 ) );
	}
	else
	{
		mValue = Dword( string );
	}
}

Qword::operator _qword& ( )
{
	return mValue;
}

Qword Qword::InverseByteOrder( ) const
{
	return Qword( Dword( ( (_dword*) this )[1] ).InverseByteOrder( ), Dword( ( (_dword*) this )[0] ).InverseByteOrder( ) );
}

String Qword::ToString( ) const
{
	return StringFormatter::FormatString( L"%.8x%.8x", ( (_dword*) this )[1], ( (_dword*) this )[0] );
}

StringPtr Qword::ToString( _char* buffer, _dword size ) const
{
	return StringFormatter::FormatBuffer( buffer, size, L"%.8x%.8x", ( (_dword*) this )[1], ( (_dword*) this )[0] );
}

_dword Qword::LoDword( ) const
{
	return ( (_dword*) this )[0];
}

_dword Qword::HiDword( ) const
{
	return ( (_dword*) this )[1];
}

//----------------------------------------------------------------------------
// Oword Implementation
//----------------------------------------------------------------------------

Oword::Oword( )
{
	mDword1	= 0;
	mDword2	= 0;
	mDword3	= 0;
	mDword4	= 0;
}

Oword::Oword( _dword dword1, _dword dword2, _dword dword3, _dword dword4 )
{
	mDword1	= dword1;
	mDword2	= dword2;
	mDword3	= dword3;
	mDword4	= dword4;
}

Oword::Oword( StringPtr string )
{
	// String buffer
	_char buffer[ 16 ];

	// Get string length
	_dword length = string.Length( );

	// Set value
	if ( length >= 32 )
	{
		mDword1 = Dword( string.SubString( length - 8 ) );
		mDword2	= Dword( StringFormatter::CopyString( buffer, string.SubString( length - 16 ), 8 ) );
		mDword3	= Dword( StringFormatter::CopyString( buffer, string.SubString( length - 24 ), 8 ) );
		mDword4	= Dword( StringFormatter::CopyString( buffer, string.SubString( length - 32 ), 8 ) );
	}
	else if ( length >= 24 )
	{
		mDword1 = Dword( string.SubString( length - 8 ) );
		mDword2	= Dword( StringFormatter::CopyString( buffer, string.SubString( length - 16 ), 8 ) );
		mDword3	= Dword( StringFormatter::CopyString( buffer, string.SubString( length - 24 ), 8 ) );
		mDword4	= Dword( StringFormatter::CopyString( buffer, string, length - 24 ) );
	}
	else if ( length >= 16 )
	{
		mDword1 = Dword( string.SubString( length - 8 ) );
		mDword2	= Dword( StringFormatter::CopyString( buffer, string.SubString( length - 16 ), 8 ) );
		mDword3	= Dword( StringFormatter::CopyString( buffer, string, length - 16 ) );
		mDword4	= 0;
	}
	else if ( length >= 8 )
	{
		mDword1 = Dword( string.SubString( length - 8 ) );
		mDword2	= Dword( StringFormatter::CopyString( buffer, string, length - 8 ) );
		mDword3	= 0;
		mDword4	= 0;
	}
	else
	{
		mDword1 = Dword( string );
		mDword2	= 0;
		mDword3	= 0;
		mDword4	= 0;
	}
}

Oword::operator _dword ( ) const
{
	return mDword1;
}

Oword Oword::InverseByteOrder( ) const
{
	return Oword( Dword( mDword4 ).InverseByteOrder( ), Dword( mDword3 ).InverseByteOrder( ), Dword( mDword2 ).InverseByteOrder( ), Dword( mDword1 ).InverseByteOrder( ) );
}

String Oword::ToString( ) const
{
	return StringFormatter::FormatString( L"%.8x%.8x%.8x%.8x", mDword4, mDword3, mDword2, mDword1 );
}

StringPtr Oword::ToString( _char* buffer, _dword size ) const
{
	return StringFormatter::FormatBuffer( buffer, size, L"%.8x%.8x%.8x%.8x", mDword4, mDword3, mDword2, mDword1 );
}

_dword Oword::Dword1( ) const
{
	return mDword1;
}

_dword Oword::Dword2( ) const
{
	return mDword2;
}

_dword Oword::Dword3( ) const
{
	return mDword3;
}

_dword Oword::Dword4( ) const
{
	return mDword4;
}

_bool Oword::operator == ( const Oword& oword ) const
{
	return mDword1 == oword.mDword1 && mDword2 == oword.mDword2 && mDword3 == oword.mDword3 && mDword4 == oword.mDword4;
}

_bool Oword::operator != ( const Oword& oword ) const
{
	return mDword1 != oword.mDword1 || mDword2 != oword.mDword2 || mDword3 != oword.mDword3 || mDword4 != oword.mDword4;
}

//----------------------------------------------------------------------------
// Float Implementation
//----------------------------------------------------------------------------

Float::Float( _float value ) : mValue( value )
{
}

Float::Float( StringPtr string )
{
	mValue = _float( ::_wtof( string ) );
}

Float::operator _float& ( )
{
	return mValue;
}

String Float::ToString( ) const
{
	return StringFormatter::FormatString( L"%5.3f", mValue );
}

StringPtr Float::ToString( _char* buffer, _dword size ) const
{
	return StringFormatter::FormatBuffer( buffer, size, L"%5.3f", mValue );
}

//----------------------------------------------------------------------------
// Double Implementation
//----------------------------------------------------------------------------

Double::Double( _double value ) : mValue( value )
{
}

Double::Double( StringPtr string )
{
	mValue = ::_wtof( string );
}

Double::operator _double& ( )
{
	return mValue;
}

String Double::ToString( ) const
{
	return StringFormatter::FormatString( L"%10.5f", (_float) mValue );
}

StringPtr Double::ToString( _char* buffer, _dword size ) const
{
	return StringFormatter::FormatBuffer( buffer, size, L"%10.5f", (_float) mValue );
}
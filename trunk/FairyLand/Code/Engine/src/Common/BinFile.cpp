//============================================================================
// BinFile.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// BinFile Implementation
//----------------------------------------------------------------------------

BinFile::BinFile( )
{
}

_bool BinFile::WriteBuffer( const _void* buffer, _dword size )
{
	if ( RemainBytes( ) < size )
		return _false;

	Memory::MemCpy( mPointer, buffer, size );
	mPointer += size;

	return _true;
}

_bool BinFile::WriteTiny( _tiny data )
{
	if ( RemainBytes( ) < sizeof( _tiny ) )
		return _false;

	*( (_tiny*) mPointer ) = data;
	mPointer += sizeof( _tiny );

	return _true;
}

_bool BinFile::WriteShort( _short data )
{
	if ( RemainBytes( ) < sizeof( _short ) )
		return _false;

	*( (_short*) mPointer ) = data;
	mPointer += sizeof( _short );

	return _true;
}

_bool BinFile::WriteLong( _long data )
{
	if ( RemainBytes( ) < sizeof( _long ) )
		return _false;

	*( (_long*) mPointer ) = data;
	mPointer += sizeof( _long );

	return _true;
}

_bool BinFile::WriteByte( _byte data )
{
	if ( RemainBytes( ) < sizeof( _byte ) )
		return _false;

	*( (_byte*) mPointer ) = data;
	mPointer += sizeof( _byte );

	return _true;
}

_bool BinFile::WriteWord( _word data )
{
	if ( RemainBytes( ) < sizeof( _word ) )
		return _false;

	*( (_word*) mPointer ) = data;
	mPointer += sizeof( _word );

	return _true;
}

_bool BinFile::WriteDword( _dword data )
{
	if ( RemainBytes( ) < sizeof( _dword ) )
		return _false;

	*( (_dword*) mPointer ) = data;
	mPointer += sizeof( _dword );

	return _true;
}

_bool BinFile::WriteFloat( _float data )
{
	if ( RemainBytes( ) < sizeof( _float ) )
		return _false;

	*( (_float*) mPointer ) = data;
	mPointer += sizeof( _float );

	return _true;
}

_bool BinFile::WriteDouble( _double data )
{
	if ( RemainBytes( ) < sizeof( _double ) )
		return _false;

	*( (_double*) mPointer ) = data;
	mPointer += sizeof( _double );

	return _true;
}

_bool BinFile::WriteBool( _bool data )
{
	if ( RemainBytes( ) < sizeof( _bool ) )
		return _false;

	*( (_bool*) mPointer ) = data;
	mPointer += sizeof( _bool );

	return _true;
}

_bool BinFile::WriteString( StringPtr string )
{
	_dword size = string.SizeOfBytes( );

	if ( RemainBytes( ) < size )
		return _false;

	Memory::MemCpy( mPointer, (const _char*) string, size );
	mPointer += size;

	return _true;
}

_bool BinFile::WriteString( StringPtr string, _dword length )
{
	FL_ASSERT( string.Length( ) >= length )

	_dword size = length * sizeof( _char );

	if ( RemainBytes( ) < size )
		return _false;

	Memory::MemCpy( mPointer, (const _char*) string, size );
	mPointer += size;

	return _true;
}

_bool BinFile::ReadBuffer( _void* buffer, _dword size )
{
	if ( RemainBytes( ) < size )
		return _false;

	Memory::MemCpy( buffer, mPointer, size );
	mPointer += size;

	return _true;
}

_bool BinFile::ReadTiny( _tiny& data )
{
	if ( RemainBytes( ) < sizeof( _tiny ) )
		return _false;

	data = *( (_tiny*) mPointer );
	mPointer += sizeof( _tiny );

	return _true;
}

_bool BinFile::ReadShort( _short& data )
{
	if ( RemainBytes( ) < sizeof( _short ) )
		return _false;

	data = *( (_short*) mPointer );
	mPointer += sizeof( _short );

	return _true;
}

_bool BinFile::ReadLong( _long& data )
{
	if ( RemainBytes( ) < sizeof( _long ) )
		return _false;

	data = *( (_long*) mPointer );
	mPointer += sizeof( _long );

	return _true;
}

_bool BinFile::ReadByte( _byte& data )
{
	if ( RemainBytes( ) < sizeof( _byte ) )
		return _false;

	data = *( (_byte*) mPointer );
	mPointer += sizeof( _byte );

	return _true;
}

_bool BinFile::ReadWord( _word& data )
{
	if ( RemainBytes( ) < sizeof( _word ) )
		return _false;

	data = *( (_word*) mPointer );
	mPointer += sizeof( _word );

	return _true;
}

_bool BinFile::ReadDword( _dword& data )
{
	if ( RemainBytes( ) < sizeof( _dword ) )
		return _false;

	data = *( (_dword*) mPointer );
	mPointer += sizeof( _dword );

	return _true;
}

_bool BinFile::ReadFloat( _float& data )
{
	if ( RemainBytes( ) < sizeof( _float ) )
		return _false;

	data = *( (_float*) mPointer );
	mPointer += sizeof( _float );

	return _true;
}

_bool BinFile::ReadDouble( _double& data )
{
	if ( RemainBytes( ) < sizeof( _double ) )
		return _false;

	data = *( (_double*) mPointer );
	mPointer += sizeof( _double );

	return _true;
}

_bool BinFile::ReadBool( _bool& data )
{
	if ( RemainBytes( ) < sizeof( _bool ) )
		return _false;

	data = *( (_bool*) mPointer );
	mPointer += sizeof( _bool );

	return _true;
}

_bool BinFile::ReadString( String& string )
{
	if ( ReachEnd( ) )
		return _false;

	_char* temp = (_char*) mPointer;

	while ( *temp != 0 )
	{
		if ( ( (_byte*) ( temp + 1 ) ) >= mBuffer + mLength )
			return _false;

		temp ++;
	}

	string = (_char*) mPointer;
	mPointer += (_byte*) temp - mPointer + sizeof( _char );

	return _true;
}

_bool BinFile::ReadString( String& string, _dword length )
{
	if ( RemainBytes( ) < length * sizeof( _char ) )
		return _false;

	string = String( (_char*) mPointer, length );
	mPointer += length * sizeof( _char );

	return _true;
}

_bool BinFile::OffsetFromBegin( _long offset )
{
	if ( offset < 0 )
		return _false;

	if ( offset > (_long) mLength )
		return _false;

	mPointer = mBuffer + offset;

	return _true;
}

_bool BinFile::OffsetFromEnd( _long offset )
{
	if ( offset > 0 )
		return _false;

	offset = -offset;

	if ( offset > (_long) mLength )
		return _false;

	mPointer = mBuffer + (_long) mLength - offset;

	return _true;
}

_bool BinFile::OffsetFromCurrent( _long offset )
{
	_byte* temp = mPointer + offset;

	if ( temp < mBuffer || temp > mBuffer + mLength )
		return _false;

	mPointer = temp;

	return _true;
}
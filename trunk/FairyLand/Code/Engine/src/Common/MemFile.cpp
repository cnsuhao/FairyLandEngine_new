//============================================================================
// MemFile.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// MemFile Implementation
//----------------------------------------------------------------------------

MemFile::MemFile( ) : mAutoDeleteBuffer( _true ), mLength( 0 ), mBuffer( _null ), mPointer( _null ), mBackup( _null )
{
}

MemFile::MemFile( const MemFile& memfile )
{
	Open( memfile );
}

MemFile& MemFile::operator = ( const MemFile& memfile )
{
	Close( );

	Open( memfile );

	return *this;
}

MemFile::~MemFile( )
{
	Close( );
}

_bool MemFile::Create( _dword length )
{
	Close( );

	if ( length > 0 )
		mBuffer = new _byte[ length ];

	mLength  = length;
	mPointer = mBuffer;

	return _true;
}

_bool MemFile::Open( StringPtr filename )
{
	File file;
	if ( file.Open( filename, File::_OPEN_EXISTING, File::_OPERATION_READ, File::_SHARE_READ | File::_SHARE_WRITE ) == _false )
		return _false;

	_dword length = file.GetSize( );

	if ( length == 0 || length == -1 )
		return Open( _null, 0, _false );

	_byte* buffer = new _byte[ length ];

	if ( file.Read( buffer, length ) == _false )
	{
		delete[] buffer;
		return _false;
	}

	return Open( buffer, length, _false );
}

_bool MemFile::Open( const _byte* buffer, _dword length, _bool copy )
{
	Close( );

	if ( length > 0 )
	{
		if ( copy == _true )
		{
			mBuffer = new _byte[ length ];
			Memory::MemCpy( mBuffer, buffer, length );
		}
		else
		{
			mBuffer = (_byte*) buffer;
		}
	}

	mLength  = length;
	mPointer = mBuffer;

	return _true;
}

_bool MemFile::Open( const MemFile& memfile )
{
	return Open( memfile.mBuffer, memfile.mLength, _true );
}

_bool MemFile::Append( const _byte* buffer, _dword length )
{
	if ( length == 0 )
		return _false;

	_byte* temp = mBuffer;

	mBuffer = new _byte[ mLength + length ];
	Memory::MemCpy( mBuffer, temp, mLength );
	Memory::MemCpy( mBuffer + mLength, buffer, length );

	if ( mAutoDeleteBuffer )
		delete[] temp;

	mLength += length;
	mPointer = mBuffer;
	mAutoDeleteBuffer = _true;

	return _true;
}

_void MemFile::Resize( _dword newsize )
{
	if ( mLength == newsize )
		return;

	_byte* newbuffer = new _byte[ newsize ];
	Memory::MemCpy( newbuffer, mBuffer, Math::Min( newsize, mLength ) );

	if ( mAutoDeleteBuffer )
		delete[] mBuffer;

	mAutoDeleteBuffer	= _true;
	mLength				= newsize;
	mPointer			= newbuffer + ( (_dword) mPointer - (_dword) mBuffer );
	mBackup				= mBackup == _null ? _null : newbuffer + ( (_dword) mBackup - (_dword) mBuffer );
	mBuffer				= newbuffer;

	if ( mPointer > mBuffer + mLength )
		mPointer = mBuffer + mLength;

	if ( mBackup > mBuffer + mLength )
		mBackup = mBuffer + mLength;
}

_void MemFile::Close( )
{
	if ( mAutoDeleteBuffer )
		delete[] mBuffer;

	mLength  = 0;
	mBuffer  = _null;
	mPointer = _null;
}
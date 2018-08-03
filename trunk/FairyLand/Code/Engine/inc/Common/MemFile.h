//============================================================================
// MemFile.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// MemFile
//----------------------------------------------------------------------------

class MemFile
{
protected:
	_bool	mAutoDeleteBuffer;
	_dword	mLength;
	_byte*	mBuffer;
	_byte*	mPointer;
	_byte*	mBackup;
	String	mFileName;

public:
	MemFile( );
	MemFile( const MemFile& memfile );
	~MemFile( );

	MemFile& operator = ( const MemFile& memfile );

	inline _void  EnableAutoDeleteBuffer( _bool enable );
	inline _dword GetLength( ) const;
	inline _byte* GetBuffer( ) const;
	inline _byte* GetPointer( ) const;

	inline _void SeekToBegin( );
	inline _void SeekToEnd( );

	inline _void SavePosition( );
	inline _void LoadPosition( );

	_bool Create( _dword length );
	_bool Open( StringPtr filename );
	_bool Open( const _byte* buffer, _dword length, _bool copy = _true );
	_bool Open( const MemFile& memfile );
	_bool Append( const _byte* buffer, _dword length );
	_void Resize( _dword newsize );
	_void Close( );

	inline _void SetFileName( StringPtr filename );
	inline StringPtr GetFileName( ) const;
};

//----------------------------------------------------------------------------
// MemFile Implementation
//----------------------------------------------------------------------------

_void MemFile::EnableAutoDeleteBuffer( _bool enable )
{
	mAutoDeleteBuffer = enable;
}

_dword MemFile::GetLength( ) const
{
	return mLength;
}

_byte* MemFile::GetBuffer( ) const
{
	return mBuffer;
}

_byte* MemFile::GetPointer( ) const
{
	return mPointer;
}

_void MemFile::SeekToBegin( )
{
	mPointer = mBuffer;
}

_void MemFile::SeekToEnd( )
{
	mPointer = mBuffer + mLength;
}

_void MemFile::SavePosition( )
{
	mBackup = mPointer;
}

_void MemFile::LoadPosition( )
{
	mPointer = mBackup;
}

_void MemFile::SetFileName( StringPtr filename )
{
	mFileName = filename;
}

StringPtr MemFile::GetFileName( ) const
{
	return mFileName;
}

};
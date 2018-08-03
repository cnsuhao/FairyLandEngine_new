//============================================================================
// MemStream.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// MemStream
//----------------------------------------------------------------------------

class MemStream : public IDataStream
{
private:
	String		mName;
	Oword		mChecksum;
	_byte*		mBuffer;
	_dword		mSize;
	_dword		mOffset;
	_bool		mBufferOwner;

public:
	MemStream( StringPtr name, const Oword& checksum, _byte* buffer, _dword size, _bool bufferowner );
	~MemStream( );

	virtual _void		Release( );

	virtual _dword		GetStreamType( ) const;
	virtual StringPtr	GetName( ) const;
	virtual Oword		GetChecksum( ) const;

	virtual _byte*		GetBuffer( ) const;
	virtual _dword		GetSize( ) const;

	virtual _bool		Read( _void* buffer, _dword length, _dword offset, _dword* finish = _null );
	virtual _bool		Write( const _void* buffer, _dword length, _dword offset, _dword* finish = _null );

	virtual _bool		IsCompressed( );
	virtual _bool		Mapping( MemFile& memfile, _bool decompress );
	virtual _bool		Unmapping( MemFile& memfile, _bool compress );

	virtual _bool		Check( const Oword& checksum, _bool withname, _dword* finish );
};

};
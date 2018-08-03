//============================================================================
// HashStream.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// HashStream
//----------------------------------------------------------------------------

class HashStream : public IDataStream
{
private:
	String		mName;
	Oword		mChecksum;
	File&		mFile;
	_dword		mSize;
	_dword		mBase;

public:
	HashStream( StringPtr name, Oword& checksum, File& file, _dword size, _dword base );
	~HashStream( );

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
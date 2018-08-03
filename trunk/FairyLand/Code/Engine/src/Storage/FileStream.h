//============================================================================
// FileStream.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// FileStream
//----------------------------------------------------------------------------

class FileStream : public IDataStream
{
private:
	String		mName;
	File		mFile;

public:
	FileStream( StringPtr name, File& file );
	~FileStream( );

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
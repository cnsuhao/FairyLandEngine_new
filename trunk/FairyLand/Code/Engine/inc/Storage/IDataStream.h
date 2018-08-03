//============================================================================
// IDataStream.h
//
// Copyright 2006-2007 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

class IDataArchive;

//----------------------------------------------------------------------------
// IDataStream
//----------------------------------------------------------------------------

class IDataStream
{
public:
	virtual _void		Release( )																			= 0;

	virtual _dword		GetStreamType( ) const																= 0;
	virtual StringPtr	GetName( ) const																	= 0;
	virtual Oword		GetChecksum( ) const																= 0;

	virtual _byte*		GetBuffer( ) const																	= 0;
	virtual _dword		GetSize( ) const																	= 0;

	virtual _bool		Read( _void* buffer, _dword length, _dword offset, _dword* finish = _null )			= 0;
	virtual _bool		Write( const _void* buffer, _dword length, _dword offset, _dword* finish = _null )	= 0;

	virtual _bool		IsCompressed( )																		= 0;
	virtual _bool		Mapping( MemFile& memfile, _bool decompress = _true )								= 0;
	virtual _bool		Unmapping( MemFile& memfile, _bool compress = _false )								= 0;

	virtual _bool		Check( const Oword& checksum, _bool withname, _dword* finish = _null )				= 0;
};

};
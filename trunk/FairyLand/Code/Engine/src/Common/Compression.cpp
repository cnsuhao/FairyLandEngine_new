//============================================================================
// Compression.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// Compression Implementation
//----------------------------------------------------------------------------

_bool Compression::IsCompressed( MemFile& memfile )
{
	if ( memfile.GetLength( ) < 20 )
		return _false;

	return Memory::MemCmp( FileSystem::cCompressedFileHeader, memfile.GetBuffer( ), 16 );
}

_bool Compression::Compress( MemFile& memfile, _float compressratio )
{
	// Already compressed.
	if ( IsCompressed( memfile ) == _true )
		return _true;

	// Calculate file size after compress.
	_dword compressedsize = (_dword)( memfile.GetLength( ) * compressratio );

	// Create buffer to compress.
	_byte* newbuffer = new byte[ compressedsize + 20 ];

	// Compress it.
	if ( Compress( newbuffer + 20, compressedsize, memfile.GetBuffer( ), memfile.GetLength( ) ) == _false )
	{
		delete[] newbuffer;
		return _false;
	}

	// Set file header.
	Memory::MemCpy( newbuffer, FileSystem::cCompressedFileHeader, 16 );

	// Set original file size.
	*( (_dword*) ( newbuffer + 16 ) ) = memfile.GetLength( );

	// Set new buffer.
	memfile.Open( newbuffer, compressedsize + 20, _false );
	memfile.EnableAutoDeleteBuffer( _true );

	return _true;
}

_bool Compression::Decompress( MemFile& memfile )
{
	// Not compressed.
	if ( IsCompressed( memfile ) == _false )
		return _true;

	// Get original file size.
	_dword originalsize = *( (_dword*) ( memfile.GetBuffer( ) + 16 ) );

	// Create buffer to decompress.
	_byte* newbuffer = new byte[ originalsize ];

	if ( newbuffer == _null )
		return _false;

	// Decompress it.
	if ( Decompress( newbuffer, originalsize, memfile.GetBuffer( ) + 20, memfile.GetLength( ) - 20 ) == _false )
	{
		delete[] newbuffer;
		return _false;
	}

	// Set new buffer.
	memfile.Open( newbuffer, originalsize, _false );
	memfile.EnableAutoDeleteBuffer( _true );

	return _true;
}

_bool Compression::IsCompressed( const _void* buffer, _dword length )
{
	if ( length < 20 )
		return _false;

	return Memory::MemCmp( FileSystem::cCompressedFileHeader, buffer, 16 );
}

_bool Compression::Compress( _void* desbuffer, _dword& deslength, const _void* srcbuffer, _dword srclength )
{
	return ::compress( (_byte*) desbuffer, &deslength, (const _byte*) srcbuffer, srclength ) == 0;
}

_bool Compression::Decompress( _void* desbuffer, _dword& deslength, const _void* srcbuffer, _dword srclength )
{
	return ::uncompress( (_byte*) desbuffer, &deslength, (const _byte*) srcbuffer, srclength ) == 0;
}

_bool Compression::BeginCompress( CompressionStream& stream, _dword level )
{
	z_stream* zstream = (z_stream*) stream.mReserved;

	// Allocate inflate state.
	zstream->zalloc		= 0;
	zstream->zfree		= 0;
	zstream->opaque		= 0;
	zstream->avail_in	= 0;
	zstream->next_in	= 0;

	stream.mInitalized			= _true;
	stream.mTotalInputLength	= 0;
	stream.mTotalOutputLength	= 0;

	if ( level == _COMPRESS_NOCOMPRESSION )
		level = Z_NO_COMPRESSION;
	else if ( level == _COMPRESS_BEST_SPEED )
		level = Z_BEST_SPEED;
	else if ( level == _COMPRESS_BEST_COMPRESSION )
		level = Z_BEST_COMPRESSION;
	else
		level = Z_DEFAULT_COMPRESSION;

	return ::deflateInit( zstream, level ) == 0;
}

_void Compression::FinishCompress( CompressionStream& stream )
{
	if ( stream.mInitalized == _false )
		return;

	z_stream* zstream = (z_stream*) stream.mReserved;

	::deflateEnd( zstream );
}

_void Compression::PrepareCompress( CompressionStream& stream, const _void* srcbuffer, _dword srclength )
{
	FL_ASSERT( srclength != 0 )
	FL_ASSERT( srcbuffer != _null )

	z_stream* zstream = (z_stream*) stream.mReserved;

	zstream->next_in	= (Bytef*) srcbuffer;
	zstream->avail_in	= srclength;
}

_void Compression::ProcessCompress( CompressionStream& stream, _void* desbuffer, _dword& deslength, _dword flushflag )
{
	FL_ASSERT( deslength != 0 )
	FL_ASSERT( desbuffer != _null )

	z_stream* zstream = (z_stream*) stream.mReserved;

	zstream->next_out	= (Bytef*) desbuffer;
	zstream->avail_out	= deslength;

	_dword zflag = Z_NO_FLUSH;

	if ( flushflag == _FLUSH_FINISH )
		zflag = Z_FINISH;
	else if ( flushflag == _FLUSH_SYNC )
		zflag = Z_SYNC_FLUSH;
	else if ( flushflag == _FLUSH_FULL )
		zflag = Z_FULL_FLUSH;

	::deflate( zstream, zflag );

	deslength = deslength - zstream->avail_out;

	stream.mTotalInputLength	= zstream->total_in;
	stream.mTotalOutputLength	= zstream->total_out;
}

_bool Compression::BeginDecompress( CompressionStream& stream )
{
	z_stream* zstream = (z_stream*) stream.mReserved;

	// Allocate inflate state.
	zstream->zalloc		= 0;
	zstream->zfree		= 0;
	zstream->opaque		= 0;
	zstream->avail_in	= 0;
	zstream->next_in	= 0;

	stream.mInitalized			= _true;
	stream.mTotalInputLength	= 0;
	stream.mTotalOutputLength	= 0;

	return ::inflateInit( zstream ) == 0;
}

_void Compression::FinishDecompress( CompressionStream& stream )
{
	if ( stream.mInitalized == _false )
		return;

	z_stream* zstream = (z_stream*) stream.mReserved;

	::inflateEnd( zstream );
}

_void Compression::PrepareDecompress( CompressionStream& stream, const _void* srcbuffer, _dword srclength )
{
	FL_ASSERT( srclength != 0 )
	FL_ASSERT( srcbuffer != _null )

	z_stream* zstream = (z_stream*) stream.mReserved;

	zstream->next_in	= (Bytef*) srcbuffer;
	zstream->avail_in	= srclength;
}

_bool Compression::ProcessDecompress( CompressionStream& stream, _void* desbuffer, _dword& deslength )
{
	FL_ASSERT( deslength != 0 )
	FL_ASSERT( desbuffer != _null )

	z_stream* zstream = (z_stream*) stream.mReserved;

	zstream->next_out	= (Bytef*) desbuffer;
	zstream->avail_out	= deslength;

	_bool result = ::inflate( zstream, Z_NO_FLUSH ) == Z_STREAM_END;

	deslength = deslength - zstream->avail_out;

	stream.mTotalInputLength	= zstream->total_in;
	stream.mTotalOutputLength	= zstream->total_out;

	return result;
}
//============================================================================
// Compression.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// CompressionStream
//----------------------------------------------------------------------------

struct CompressionStream
{
	//! Reserved data used by zip lib.
	_byte	mReserved[60];

	_bool	mInitalized;
	_dword	mTotalInputLength;
	_dword	mTotalOutputLength;

	CompressionStream( )
		: mInitalized( _false ), mTotalInputLength( 0 ), mTotalOutputLength( 0 ) { }
};

//----------------------------------------------------------------------------
// Compression
//----------------------------------------------------------------------------

class Compression
{
public:
	enum _COMPRESS_FLAG
	{
		_COMPRESS_UNKNOWN	= 0,
		_COMPRESS_NO		= 1,
		_COMPRESS_YES		= 2,
	};

	enum _COMPRESS_LEVEL
	{
		_COMPRESS_DEFAULT			= 0,
		_COMPRESS_NOCOMPRESSION		= 1,
		_COMPRESS_BEST_SPEED		= 2,
		_COMPRESS_BEST_COMPRESSION	= 3,
	};

	enum _FLUSH_FLAG
	{
		_FLUSH_NO		= 0,
		_FLUSH_FINISH	= 1,
		_FLUSH_SYNC		= 2,
		_FLUSH_FULL		= 3,
	};

public:
	static _bool IsCompressed( MemFile& memfile );
	static _bool Compress( MemFile& memfile, _float compressratio = 1.0f );
	static _bool Decompress( MemFile& memfile );

	static _bool IsCompressed( const _void* buffer, _dword length );
	static _bool Compress( _void* desbuffer, _dword& deslength, const _void* srcbuffer, _dword srclength );
	static _bool Decompress( _void* desbuffer, _dword& deslength, const _void* srcbuffer, _dword srclength );

	static _bool BeginCompress( CompressionStream& stream, _dword level = _COMPRESS_DEFAULT );
	static _void FinishCompress( CompressionStream& stream );
	static _void PrepareCompress( CompressionStream& stream, const _void* srcbuffer, _dword srclength );
	static _void ProcessCompress( CompressionStream& stream, _void* desbuffer, _dword& deslength, _dword flushflag );

	static _bool BeginDecompress( CompressionStream& stream );
	static _void FinishDecompress( CompressionStream& stream );
	static _void PrepareDecompress( CompressionStream& stream, const _void* srcbuffer, _dword srclength );
	static _bool ProcessDecompress( CompressionStream& stream, _void* desbuffer, _dword& deslength );
};

};
//============================================================================
// AsyncIOManager.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// AsyncIOManager
//----------------------------------------------------------------------------

class AsyncIOManager
{
private:
	struct AsyncCheck
	{
		IDataStream*	mStream;
		_dword*			mFinish;
		Oword			mChecksum;
		_bool			mWithName;
	};

	struct AsyncWrite
	{
		File*			mFile;
		_dword*			mFinish;
		_dword			mOffset;
		_byte*			mHeaderBuffer;
		_dword			mHeaderLength;
		_byte*			mResourceBuffer;
		_dword			mResourceLength;
	};

	struct AsyncRead
	{
		File*			mFile;
		_dword*			mFinish;
		_dword			mOffset;
		_byte*			mBuffer;
		_dword			mLength;
	};

	typedef Queue< AsyncCheck >	AsyncCheckQueue;
	typedef Queue< AsyncRead >	AsyncReadQueue;
	typedef Queue< AsyncWrite >	AsyncWriteQueue;

	Lock				mAsyncLock;
	Thread				mAsyncThread;

	AsyncCheckQueue		mAsyncCheckQueue;
	AsyncReadQueue		mAsyncReadQueue;
	AsyncWriteQueue		mAsyncWriteQueue;

	static _bool		OnAsyncIOThread( _void* parameter1, _void* parameter2 );

public:
	AsyncIOManager( );
	~AsyncIOManager( );

	_void	PostAsyncCheck( IDataStream* stream, const Oword& checksum, _bool withname, _dword* finish );
	_void	ProcessCheck( IDataStream* stream, const Oword& checksum, _bool withname, _dword* finish );
	_void	ProcessAllAsyncCheck( );

	_void	PostAsyncRead( File& file, _dword offset, _byte* buffer, _dword length, _dword* finish );
	_void	ProcessRead( File& file, _dword offset, _byte* buffer, _dword length, _dword* finish );
	_void	ProcessAllAsyncRead( );

	_void	PostAsyncWrite( File& file, _dword offset, _byte* headerbuffer, _dword headerlength, _byte* resourcebuffer, _dword resourcelength, _dword* finish );
	_void	ProcessWrite( File& file, _dword offset, _byte* headerbuffer, _dword headerlength, _byte* resourcebuffer, _dword resourcelength, _dword* finish );
	_void	ProcessAllAsyncWrite( );
};

};
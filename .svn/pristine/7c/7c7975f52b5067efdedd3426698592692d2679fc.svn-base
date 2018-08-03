//============================================================================
// AsyncIOManager.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandStorage.h"

//----------------------------------------------------------------------------
// AsyncIOManager Implementation
//----------------------------------------------------------------------------

AsyncIOManager::AsyncIOManager( )
{
}

AsyncIOManager::~AsyncIOManager( )
{
	mAsyncThread.Close( );

	// Process all unfinished write operation.
	ProcessAllAsyncWrite( );

	FL_ASSERT( mAsyncWriteQueue.Number( ) == 0 )
}

_bool AsyncIOManager::OnAsyncIOThread( _void* parameter1, _void* parameter2 )
{
	AsyncIOManager* asynciomanager = (AsyncIOManager*) parameter1;

	asynciomanager->ProcessAllAsyncCheck( );
	asynciomanager->ProcessAllAsyncRead( );
	asynciomanager->ProcessAllAsyncWrite( );

	return _true;
}

_void AsyncIOManager::PostAsyncCheck( IDataStream* stream, const Oword& checksum, _bool withname, _dword* finish )
{
	AsyncCheck asynccheck;
	asynccheck.mStream			= stream;
	asynccheck.mFinish			= finish;
	asynccheck.mChecksum		= checksum;
	asynccheck.mWithName		= withname;

	mAsyncLock.Enter( );
	mAsyncCheckQueue.Append( asynccheck );
	mAsyncLock.Leave( );

	if ( mAsyncThread.GetThreadID( ) == 0 )
	{
		mAsyncThread.Create( 10 );
		mAsyncThread.RegisterUpdateFunc( OnAsyncIOThread, this, _null );
	}
}

_void AsyncIOManager::ProcessCheck( IDataStream* stream, const Oword& checksum, _bool withname, _dword* finish )
{
	MemFile memfile;

	// Try to map this file.
	if ( stream->Mapping( memfile, withname ) == _false )
	{
		// Set finish flag.
		if ( finish != _null )
			Lock::SetValue( (_long*) finish, IResourceManager::_ASYNC_DONE_FALSE );

		return;
	}

	Oword candidatechecksum;

	// Great, we can check only against data checksum, much faster.
	if ( withname == _false )
	{
		// Build checksum with file data.
		Encryption::MD5Checksum( memfile.GetBuffer( ), memfile.GetLength( ), candidatechecksum );
	}
	else
	{
		_dword namelength = stream->GetName( ).SizeOfBytes( );
		_byte* buffer = new _byte[ namelength + memfile.GetLength( ) ];

		// Make file name lowercased.
		_char lowercasedfilename[1024];
		StringFormatter::CopyString( lowercasedfilename, stream->GetName( ), 1023 );
		StringFormatter::LowercaseString( lowercasedfilename );

		// Prepare buffer to make checksum.
		Memory::MemCpy( buffer, lowercasedfilename, namelength );
		Memory::MemCpy( buffer + namelength, memfile.GetBuffer( ), memfile.GetLength( ) );

		// Build checksum with full file name and file data after compress.
		Encryption::MD5Checksum( buffer, namelength + memfile.GetLength( ), candidatechecksum );

		delete[] buffer;
	}

	// Check against checksum.
	if ( candidatechecksum != checksum )
	{
		PS_ERROR1( L"[RM] Check Resource Failed At Checksum : %s\r\n", stream->GetName( ) );
		PS_ERROR8( L"\tWantted checksum: %08X-%08X-%08X-%08X\r\n\tReally checksum: %08X-%08X-%08X-%08X\r\n",
			candidatechecksum.Dword1( ), candidatechecksum.Dword2( ), candidatechecksum.Dword3( ), candidatechecksum.Dword4( ),
			checksum.Dword1( ), checksum.Dword2( ), checksum.Dword3( ), checksum.Dword4( ) );

		if ( finish != _null )
			Lock::SetValue( (_long*) finish, IResourceManager::_ASYNC_DONE_FALSE );
	}
	else
	{
		if ( finish != _null )
			Lock::SetValue( (_long*) finish, IResourceManager::_ASYNC_DONE_TRUE );
	}
}

_void AsyncIOManager::ProcessAllAsyncCheck( )
{
	mAsyncLock.Enter( );
	_dword number = mAsyncCheckQueue.Number( );
	mAsyncLock.Leave( );

	for ( _dword i = 0; i < number; i ++ )
	{
		mAsyncLock.Enter( );
		AsyncCheck asynccheck = mAsyncCheckQueue.Front( );
		mAsyncLock.Leave( );

		//  Process check operation.
		ProcessCheck( asynccheck.mStream, asynccheck.mChecksum, asynccheck.mWithName, asynccheck.mFinish );

		mAsyncLock.Enter( );
		mAsyncCheckQueue.Remove( );
		mAsyncLock.Leave( );
	}
}

_void AsyncIOManager::PostAsyncRead( File& file, _dword offset, _byte* buffer, _dword length, _dword* finish )
{
	FL_ASSERT( buffer != _null )

	AsyncRead asyncread;
	asyncread.mFile		= &file;
	asyncread.mFinish	= finish;
	asyncread.mOffset	= offset;
	asyncread.mBuffer	= buffer;
	asyncread.mLength	= length;

	mAsyncLock.Enter( );
	mAsyncReadQueue.Append( asyncread );
	mAsyncLock.Leave( );

	if ( mAsyncThread.GetThreadID( ) == 0 )
	{
		mAsyncThread.Create( 10 );
		mAsyncThread.RegisterUpdateFunc( OnAsyncIOThread, this, _null );
	}
}

_void AsyncIOManager::ProcessRead( File& file, _dword offset, _byte* buffer, _dword length, _dword* finish )
{
	file.SeekFromBegin( offset );

	_bool done = _true;

	// Read into buffer.
	if ( buffer != _null )
		done = file.Read( buffer, length );

	// Set finish flag.
	if ( finish != _null )
		Lock::SetValue( (_long*) finish, done ? IResourceManager::_ASYNC_DONE_TRUE : IResourceManager::_ASYNC_DONE_FALSE );
}

_void AsyncIOManager::ProcessAllAsyncRead( )
{
	mAsyncLock.Enter( );
	_dword number = mAsyncReadQueue.Number( );
	mAsyncLock.Leave( );

	for ( _dword i = 0; i < number; i ++ )
	{
		mAsyncLock.Enter( );
		AsyncRead asyncread = mAsyncReadQueue.Front( );
		mAsyncLock.Leave( );

		//  Process read operation.
		ProcessRead( *asyncread.mFile, asyncread.mOffset, asyncread.mBuffer, asyncread.mLength, asyncread.mFinish );

		mAsyncLock.Enter( );
		mAsyncReadQueue.Remove( );
		mAsyncLock.Leave( );
	}
}

_void AsyncIOManager::PostAsyncWrite( File& file, _dword offset, _byte* headerbuffer, _dword headerlength, _byte* resourcebuffer, _dword resourcelength, _dword* finish )
{
	AsyncWrite asyncwrite;
	asyncwrite.mFile			= &file;
	asyncwrite.mFinish			= finish;
	asyncwrite.mOffset			= offset;
	asyncwrite.mHeaderBuffer	= headerbuffer;
	asyncwrite.mHeaderLength	= headerlength;
	asyncwrite.mResourceBuffer	= resourcebuffer;
	asyncwrite.mResourceLength	= resourcelength;

	if ( asyncwrite.mHeaderBuffer != _null )
		Memory::ReserveReferencedBuffer( asyncwrite.mHeaderBuffer );

	if ( asyncwrite.mResourceBuffer != _null )
		Memory::ReserveReferencedBuffer( asyncwrite.mResourceBuffer );

	mAsyncLock.Enter( );
	mAsyncWriteQueue.Append( asyncwrite );
	mAsyncLock.Leave( );

	if ( mAsyncThread.GetThreadID( ) == 0 )
	{
		mAsyncThread.Create( 10 );
		mAsyncThread.RegisterUpdateFunc( OnAsyncIOThread, this, _null );
	}
}

_void AsyncIOManager::ProcessWrite( File& file, _dword offset, _byte* headerbuffer, _dword headerlength, _byte* resourcebuffer, _dword resourcelength, _dword* finish )
{
	file.SeekFromBegin( offset );

	// Write header buffer.
	if ( headerbuffer != _null )
	{
		file.Write( headerbuffer, headerlength );
		Memory::ReleaseReferencedBuffer( headerbuffer );
	}

	// Write resource buffer.
	if ( resourcebuffer != _null )
	{
		file.Write( resourcebuffer, resourcelength );
		Memory::ReleaseReferencedBuffer( resourcebuffer );
	}

	// Set finish flag.
	if ( finish != _null )
		Lock::SetValue( (_long*) finish, IResourceManager::_ASYNC_DONE_TRUE );
}

_void AsyncIOManager::ProcessAllAsyncWrite( )
{
	mAsyncLock.Enter( );
	_dword number = mAsyncWriteQueue.Number( );
	mAsyncLock.Leave( );

	for ( _dword i = 0; i < number; i ++ )
	{
		mAsyncLock.Enter( );
		AsyncWrite asyncwirte = mAsyncWriteQueue.Front( );
		mAsyncLock.Leave( );

		//  Process write operation.
		ProcessWrite( *asyncwirte.mFile, asyncwirte.mOffset, asyncwirte.mHeaderBuffer,
			asyncwirte.mHeaderLength, asyncwirte.mResourceBuffer, asyncwirte.mResourceLength, asyncwirte.mFinish );

		mAsyncLock.Enter( );
		mAsyncWriteQueue.Remove( );
		mAsyncLock.Leave( );
	}
}
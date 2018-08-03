//============================================================================
// Pipe.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// Pipe Implementation
//----------------------------------------------------------------------------

Pipe::Pipe( ) : mPipeHandle( _null )
{
}

Pipe::~Pipe( )
{
	Close( );
}

_bool Pipe::Create( StringPtr pipename, _dword size )
{
	Close( );

	FL_ASSERT( pipename != L"" )

	String tempname = L"\\\\.\\pipe\\";
	tempname += pipename;

	_dword flag = PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT;

	mPipeHandle = ::CreateNamedPipe( tempname, PIPE_ACCESS_DUPLEX, flag, 32, size, size, -1, _null );
	if ( mPipeHandle == INVALID_HANDLE_VALUE )
		mPipeHandle = _null;

	return mPipeHandle != _null;
}

_bool Pipe::WaitConnection( )
{
	if ( mPipeHandle == _null )
		return _false;

	return ::ConnectNamedPipe( mPipeHandle, _null );
}

_bool Pipe::Open( StringPtr pipename )
{
	Close( );

	FL_ASSERT( pipename != L"" )

	String tempname = L"\\\\.\\pipe\\";
	tempname += pipename;

	mPipeHandle = ::CreateFile( tempname, GENERIC_READ | GENERIC_WRITE, 0, _null, OPEN_EXISTING, 0, _null );
	if ( mPipeHandle == INVALID_HANDLE_VALUE )
		mPipeHandle = _null;

	return mPipeHandle != _null;
}

_bool Pipe::Peek( _dword& bytesavail )
{
	bytesavail = 0;

	if ( mPipeHandle == _null )
		return _false;

	_dword bytesread = 0, totalbytesavail = 0, bytesleft = 0;
	if ( ::PeekNamedPipe( mPipeHandle, _null, 0, &bytesread, &totalbytesavail, &bytesleft ) == _false )
		return _false;

	bytesavail = totalbytesavail;

	return _true;
}

_bool Pipe::Read( _void* buffer, _dword length, _dword* bytesread )
{
	if ( bytesread != _null )
		*bytesread = 0;

	if ( mPipeHandle == _null )
		return _false;

	_dword bytes = 0;
	_bool retval = ::ReadFile( mPipeHandle, buffer, length, &bytes, _null );

	if ( bytesread != _null )
		*bytesread = bytes;

	return retval;
}

_bool Pipe::Write( const _void* buffer, _dword length, _dword* byteswritten )
{
	if ( byteswritten != _null )
		*byteswritten = 0;

	if ( mPipeHandle == _null )
		return _false;

	_dword bytes = 0;
	_bool retval = ::WriteFile( mPipeHandle, buffer, length, &bytes, _null );

	if ( byteswritten != _null )
		*byteswritten = bytes;

	return retval;
}

_void Pipe::Close( )
{
	if ( mPipeHandle != _null )
		::CloseHandle( mPipeHandle );

	mPipeHandle = _null;
}
//============================================================================
// Pipe.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// Pipe
//----------------------------------------------------------------------------

class Pipe
{
private:
	_handle	mPipeHandle;

	Pipe( const Pipe& );
	Pipe& operator = ( const Pipe& );

public:
	Pipe( );
	~Pipe( );

	_bool Create( StringPtr pipename, _dword size = 1024 * 1024 );
	_bool WaitConnection( );
	_bool Open( StringPtr pipename );

	_bool Peek( _dword& bytesavail );
	_bool Read( _void* buffer, _dword length, _dword* bytesread = _null );
	_bool Write( const _void* buffer, _dword length, _dword* byteswritten = _null );

	_void Close( );
};

};
//============================================================================
// HTTPSession.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// HTTPSession
//----------------------------------------------------------------------------

class HTTPSession
{
public:
	enum _HTTP_TYPE
	{
		_TYPE_UNKNOWN	= 0,
		_TYPE_BINARY	= 1,
		_TYPE_TEXT		= 2,
		_TYPE_IMAGE		= 3,
	};

private:
	String		mHostURL;
	String		mHostFolder;
	String		mHostFile;
	_handle		mSession;
	_handle		mRequest;

public:
	HTTPSession( _handle internet, StringPtr hosturl );
	~HTTPSession( );

	inline StringPtr GetHostURL( );

	_bool	StartRequest( StringPtr url, _dword start, _dword end );
	_bool	ReadResponse( _void* buffer, _dword length, _dword* bytesread = _null );
	_dword	GetResponseType( );
	_dword	GetResponseSize( );
	_void	StopRequest( );
};

//----------------------------------------------------------------------------
// HTTPDownloader Implementation
//----------------------------------------------------------------------------

StringPtr HTTPSession::GetHostURL( )
{
	return mHostURL;
}

};
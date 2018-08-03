//============================================================================
// HTTPSession.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandNetwork.h"

//----------------------------------------------------------------------------
// HTTPSession Implementation
//----------------------------------------------------------------------------

HTTPSession::HTTPSession( _handle internet, StringPtr hosturl )
	: mSession( _null ), mRequest( _null )
{
	String lowercaseurl = hosturl;
	lowercaseurl.Lowercase( );
	lowercaseurl.Replace( L"http://", L"" );

	mHostURL = StringFormatter::GetPathName( lowercaseurl, _false );

	if ( mHostURL == L"" )
	{
		mHostURL = lowercaseurl;
	}
	else
	{
		mHostFolder	= lowercaseurl;
		mHostFolder.Remove( 0, mHostURL.Length( ) );
		mHostFolder.TrimBoth( L"/\\" );

		_dword length = mHostFolder.Length( );

		if ( length > 0 && mHostFolder[ length - 1 ] == '?' )
		{
			mHostFile = mHostFolder;
			mHostFile.Remove( length - 1, 1 );

			mHostFolder = L"";
		}
	}

	_word port = INTERNET_DEFAULT_HTTP_PORT;

	_dword index = mHostURL.SearchR2L( ':' );
	if ( index != -1 )
	{
		port = Short( (const _char*) mHostURL + index + 1 );
		mHostURL.Remove( index, mHostURL.Length( ) - index );
	}

	// Connect to the address, open a session.
	mSession = ::InternetConnect( internet, mHostURL, port, L"", L"", INTERNET_SERVICE_HTTP, 0, _null );
}

HTTPSession::~HTTPSession( )
{
	if ( mRequest != _null )
		::InternetCloseHandle( mRequest );

	if ( mSession != _null )
		::InternetCloseHandle( mSession );
}

_bool HTTPSession::StartRequest( StringPtr url, _dword start, _dword end )
{
	String requrl = url;

	if ( mHostFile != L"" )
	{
		requrl = mHostFile;

		if ( start == -1 || end == -1 )
			return _false;
	}
	else if ( mHostFolder != L"" )
	{
		requrl.Insert( 0, '/' );
		requrl.Insert( 0, mHostFolder );
	}

	if ( mSession == _null )
		return _false;

	if ( mRequest != _null )
	{
		::InternetCloseHandle( mRequest );
		mRequest = _null;
	}

	const _char* accepttype[] = { L"*/*", _null };

	// Creates a HTTP GET request handle.
	mRequest = ::HttpOpenRequest( mSession, L"GET", requrl, L"HTTP/1.1", L"", accepttype, INTERNET_FLAG_KEEP_CONNECTION, _null );
	if ( mRequest == _null )
		return _false;

	const _char* acceptheader = L"Accept: */*\r\n";

	// Add request headers.
	if ( ::HttpAddRequestHeaders( mRequest, acceptheader, -1, HTTP_ADDREQ_FLAG_ADD ) == 0 )
	{
		::InternetCloseHandle( mRequest );
		mRequest = _null;

		return _false;
	}

	if ( mHostFile != L"" && start != -1 && end != -1 )
	{
		_char rangeheader[1024];
		StringFormatter::FormatBuffer( rangeheader, 1024, L"Range: bytes=%d-%d\r\n", start, end );

		// Add range headers.
		if ( ::HttpAddRequestHeaders( mRequest, rangeheader, -1, HTTP_ADDREQ_FLAG_ADD ) == 0 )
		{
			::InternetCloseHandle( mRequest );
			mRequest = _null;

			return _false;
		}
	}

	// Send Request.
	if ( ::HttpSendRequest( mRequest, _null, 0, _null, 0 ) == 0 )
	{
		::InternetCloseHandle( mRequest );
		mRequest = _null;

		return _false;
	}

	return _true;
}

_bool HTTPSession::ReadResponse( _void* buffer, _dword length, _dword* bytesread )
{
	if ( mRequest == _null )
		return _false;

	_dword bytes = 0;

	_bool retval = ::InternetReadFile( mRequest, buffer, length, &bytes ) != 0;

	if ( bytesread != _null )
		*bytesread = bytes;

	return retval;
}

_dword HTTPSession::GetResponseType( )
{
	if ( mRequest == _null )
		return -1;

	_char buffer[1024]; _dword length = 1024, index = 0;
	if ( ::HttpQueryInfo( mRequest, HTTP_QUERY_CONTENT_TYPE, buffer, &length, &index ) == _false )
		return -1;

	if ( length < 1024 )
		buffer[ length ] = 0;

	if ( StringPtr( buffer ).SearchL2RLowercase( L"application" ) != -1 )
		return _TYPE_BINARY;

	if ( StringPtr( buffer ).SearchL2RLowercase( L"binary" ) != -1 )
		return _TYPE_BINARY;

	if ( StringPtr( buffer ).SearchL2RLowercase( L"text" ) != -1 )
		return _TYPE_TEXT;

	if ( StringPtr( buffer ).SearchL2RLowercase( L"image" ) != -1 )
		return _TYPE_IMAGE;

	return _TYPE_UNKNOWN;
}

_dword HTTPSession::GetResponseSize( )
{
	if ( mRequest == _null )
		return -1;

	_char buffer[1024]; _dword length = 1024, index = 0;
	if ( ::HttpQueryInfo( mRequest, HTTP_QUERY_CONTENT_LENGTH, buffer, &length, &index ) == _false )
		return -1;

	if ( length < 1024 )
		buffer[ length ] = 0;

	return Long( buffer );
}

_void HTTPSession::StopRequest( )
{
	if ( mRequest != _null )
	{
		::InternetCloseHandle( mRequest );
		mRequest = _null;
	}
}
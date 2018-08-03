//============================================================================
// Clipboard.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// Clipboard Implementation
//----------------------------------------------------------------------------

_bool Clipboard::Copy( StringPtr string )
{
	if ( ::OpenClipboard( _null ) == 0 )
		return _false;

	_dword size = string.SizeOfBytes( );

	HGLOBAL globalmem = ::GlobalAlloc( GHND | GMEM_DDESHARE, size );
	if ( globalmem == _null )
	{
		::CloseClipboard( );
		return _false;
	}

	Memory::MemCpy( ::GlobalLock( globalmem ), (const _char*) string, size );
	::GlobalUnlock( globalmem );

	::EmptyClipboard( );
	::SetClipboardData( CF_UNICODETEXT, globalmem );

	::GlobalFree( globalmem );
	::CloseClipboard( );

	return _true;
}

_bool Clipboard::Paste( String& string )
{
	if ( ::OpenClipboard( _null ) == 0 )
		return _false;

	HGLOBAL globalmem = _null;

	if ( ( globalmem = ::GetClipboardData( CF_UNICODETEXT ) ) != _null )
	{
		_char* unicodetext = (_char*) ::GlobalLock( globalmem );

		if ( unicodetext != _null )
			string = unicodetext;

		::GlobalUnlock( globalmem );
	}
	else if ( ( globalmem = ::GetClipboardData( CF_TEXT ) ) != _null )
	{
		_chara* asciitext = (_chara*) ::GlobalLock( globalmem );

		if ( asciitext != _null )
		{
			_char* unicodetext = StringFormatter::AnsiToUnicode( _null, 0, asciitext );
			string = unicodetext;
			delete[] unicodetext;
		}

		::GlobalUnlock( globalmem );
	}
	else
	{
		::CloseClipboard( );

		return _false;
	}

	::CloseClipboard( );

	return _true;
}
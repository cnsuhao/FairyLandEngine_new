//============================================================================
// TxtFile.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// TxtFile Implementation
//----------------------------------------------------------------------------

TxtFile::TxtFile( ) : mUnicode( _UNICODE_UNKNOWN ), mDivision( L",;|\t " )
{
	mLineNumber			= 1;
	mLinePointerBackup	= _null;
	mTextBeginOffset	= 0;
}

_void TxtFile::CheckUnicode( )
{
	mTextBeginOffset = 0;

	if ( mLength < 2 )
	{
		mUnicode = _UNICODE_NO;
	}
	else if ( *( (_word*) mBuffer ) == 0xFEFF )
	{
		mUnicode = _UNICODE_YES;

		// Auto adjust buffer.
		if ( mPointer == mBuffer )
			mPointer = mBuffer + 2;

		mTextBeginOffset = 2;
	}
	else if ( mLength > 2 && ( (_byte*) mBuffer )[0] != 0 && ( (_byte*) mBuffer )[1] == 0 )
	{
		mUnicode = _UNICODE_YES;
	}
	else
	{
		mUnicode = _UNICODE_NO;
	}
}

_void TxtFile::TrimLine( )
{
	if ( mPointer < mBuffer + mTextBeginOffset )
		mPointer = mBuffer + mTextBeginOffset;

	// Check unicode flag.
	if ( mUnicode == _UNICODE_UNKNOWN )
		CheckUnicode( );

	// Calculate line number.
	if ( mPointer != mLinePointerBackup )
		UpdateLineNumber( );

	mLinePointerBackup = mPointer;

	if ( mPointer - mBuffer == mLength )
		return;

	// Unicode.
	if ( mUnicode == _UNICODE_YES )
	{
		_charw* temp = (_charw*) mPointer;

		while ( (_byte*) temp - mBuffer < (_long) mLength && ( *temp == '\r' || *temp == '\n' || mDivision.SearchL2R( (_char) *temp ) != -1 ) )
		{
			if ( *temp == '\n' )
				mLineNumber ++;

			temp ++;
		}

		mPointer = (_byte*) temp;
	}
	// Ansii.
	else
	{
		_chara* temp = (_chara*) mPointer;

		while ( (_byte*) temp - mBuffer < (_long) mLength && ( *temp == '\r' || *temp == '\n' || mDivision.SearchL2R( (_char) *temp ) != -1 ) )
		{
			if ( *temp == '\n' )
				mLineNumber ++;

			temp ++;
		}

		mPointer = (_byte*) temp;
	}

	mLinePointerBackup = mPointer;
}

_void TxtFile::TrimWord( )
{
	if ( mPointer < mBuffer + mTextBeginOffset )
		mPointer = mBuffer + mTextBeginOffset;

	// Check unicode flag.
	if ( mUnicode == _UNICODE_UNKNOWN )
		CheckUnicode( );

	// Calculate line number.
	if ( mPointer != mLinePointerBackup )
		UpdateLineNumber( );

	mLinePointerBackup = mPointer;

	if ( mPointer - mBuffer == mLength )
		return;

	// Unicode.
	if ( mUnicode == _UNICODE_YES )
	{
		_charw* temp = (_charw*) mPointer;

		while ( (_byte*) temp - mBuffer < (_long) mLength && *temp != '\r' && *temp != '\n' && mDivision.SearchL2R( (_char) *temp ) != -1 )
			temp ++;

		mPointer = (_byte*) temp;
	}
	// Ansii.
	else
	{
		_chara* temp = (_chara*) mPointer;

		while ( (_byte*) temp - mBuffer < (_long) mLength && *temp != '\r' && *temp != '\n' && mDivision.SearchL2R( (_char) *temp ) != -1 )
			temp ++;

		mPointer = (_byte*) temp;
	}

	mLinePointerBackup = mPointer;
}

_void TxtFile::UpdateLineNumber( )
{
	// Calculate line number
	mLineNumber			= 1;
	mLinePointerBackup	= mBuffer;

	while ( mLinePointerBackup < mPointer )
	{
		if ( *mLinePointerBackup == '\n' )
			mLineNumber ++;

		mLinePointerBackup ++;
	}
}

_void TxtFile::CopyString( String& string, const _byte* pointer, _dword length ) const
{
	if ( length >= 2048 )
	{
		_char* tempstring = StringFormatter::AnsiToUnicode( _null, 0, (_chara*) pointer, length );
		string = tempstring;
		delete[] tempstring;
	}
	else
	{
		_char localtempstring[ 2048 ];
		string.CopyString( StringFormatter::AnsiToUnicode( localtempstring, 2048, (_chara*) pointer, length ), length );
	}
}

_bool TxtFile::ReachEnd( )
{
	if ( mPointer < mBuffer + mTextBeginOffset )
		mPointer = mBuffer + mTextBeginOffset;

	// Check unicode flag.
	if ( mUnicode == _UNICODE_UNKNOWN )
		CheckUnicode( );

	// Calculate line number.
	if ( mPointer != mLinePointerBackup )
		UpdateLineNumber( );

	if ( mPointer - mBuffer == mLength )
		return _true;

	// Unicode.
	if ( mUnicode == _UNICODE_YES )
	{
		for ( _charw* temp = (_charw*) mPointer; (_byte*) temp - mBuffer != mLength; temp ++ )
		{
			if ( *temp == '\r' )
				continue;

			if ( *temp == '\n' )
				continue;

			if ( mDivision.SearchL2R( (_char) *temp ) == -1 )
				return _false;
		}
	}
	// Ansii.
	else
	{
		for ( _chara* temp = (_chara*) mPointer; (_byte*) temp - mBuffer != mLength; temp ++ )
		{
			if ( *temp == '\r' )
				continue;

			if ( *temp == '\n' )
				continue;

			if ( mDivision.SearchL2R( (_char) *temp ) == -1 )
				return _false;
		}
	}

	return _true;
}

_bool TxtFile::ReadText( String& string )
{
	if ( mPointer < mBuffer + mTextBeginOffset )
		mPointer = mBuffer + mTextBeginOffset;

	// Check unicode flag.
	if ( mUnicode == _UNICODE_UNKNOWN )
		CheckUnicode( );

	_dword length = _dword( mLength - ( mPointer - mBuffer ) );

	// Unicode.
	if ( mUnicode == _UNICODE_YES )
	{
		string.CopyString( (_charw*) mPointer, length );
	}
	// Ansii.
	else
	{
		CopyString( string, mPointer, length );
	}

	return _true;
}

_bool TxtFile::ReadLine( String& string )
{
	TrimLine( );

	if ( mPointer - mBuffer == mLength )
	{
		string = L"";

		return _false;
	}

	// Unicode.
	if ( mUnicode == _UNICODE_YES )
	{
		_charw* temp = (_charw*) mPointer;

		while ( (_byte*) temp - mBuffer < (_long) mLength && *temp != '\r' && *temp != '\n' )
			temp ++;

		string.CopyString( (_charw*) mPointer, (_long) ( temp - (_charw*) mPointer ) );

		mLinePointerBackup = mPointer;

		if ( temp == (_charw*) mPointer )
			return _false;

		mPointer			= (_byte*) temp;
		mLinePointerBackup	= mPointer;

		return _true;
	}
	// Ansii.
	else
	{
		_chara* temp = (_chara*) mPointer;

		while ( (_byte*) temp - mBuffer < (_long) mLength && *temp != '\r' && *temp != '\n' )
			temp ++;

		CopyString( string, mPointer, _dword( temp - (_chara*) mPointer ) );

		mLinePointerBackup = mPointer;

		if ( temp == (_chara*) mPointer )
			return _false;

		mPointer			= (_byte*) temp;
		mLinePointerBackup	= mPointer;

		return _true;
	}
}

_bool TxtFile::ReadWord( String& string )
{
	TrimLine( );

	if ( mPointer - mBuffer == mLength )
	{
		string = L"";

		return _false;
	}

	// Unicode.
	if ( mUnicode == _UNICODE_YES )
	{
		_charw* temp = (_charw*) mPointer;

		while ( (_byte*) temp - mBuffer < (_long) mLength && *temp != '\r' && *temp != '\n' && mDivision.SearchL2R( (_char) *temp ) == -1 )
			temp ++;

		string.CopyString( (_charw*) mPointer, (_long) ( temp - (_charw*) mPointer ) );

		mLinePointerBackup = mPointer;

		if ( temp == (_charw*) mPointer )
			return _false;

		mPointer			= (_byte*) temp;
		mLinePointerBackup	= mPointer;

		return _true;
	}
	// Ansii.
	else
	{
		_chara* temp = (_chara*) mPointer;

		while ( (_byte*) temp - mBuffer < (_long) mLength && *temp != '\r' && *temp != '\n' && mDivision.SearchL2R( (_char) *temp ) == -1 )
			temp ++;

		CopyString( string, mPointer, _dword( temp - (_chara*) mPointer ) );

		mLinePointerBackup = mPointer;

		if ( temp == (_chara*) mPointer )
			return _false;

		mPointer			= (_byte*) temp;
		mLinePointerBackup	= mPointer;

		return _true;
	}
}

_bool TxtFile::ReadWordInLine( String& string )
{
	TrimWord( );

	if ( mPointer - mBuffer == mLength )
	{
		string = L"";

		return _false;
	}

	// Unicode.
	if ( mUnicode == _UNICODE_YES )
	{
		_charw* temp = (_charw*) mPointer;

		while ( (_byte*) temp - mBuffer < (_long) mLength && *temp != '\r' && *temp != '\n' && mDivision.SearchL2R( (_char) *temp ) == -1 )
			temp ++;

		string.CopyString( (_charw*) mPointer, (_long) ( temp - (_charw*) mPointer ) );

		mLinePointerBackup = mPointer;

		if ( temp == (_charw*) mPointer )
			return _false;

		mPointer			= (_byte*) temp;
		mLinePointerBackup	= mPointer;

		return _true;
	}
	// Ansii.
	else
	{
		_chara* temp = (_chara*) mPointer;

		while ( (_byte*) temp - mBuffer < (_long) mLength && *temp != '\r' && *temp != '\n' && mDivision.SearchL2R( (_char) *temp ) == -1 )
			temp ++;

		CopyString( string, mPointer, _dword( temp - (_chara*) mPointer ) );

		mLinePointerBackup = mPointer;

		if ( temp == (_chara*) mPointer )
			return _false;

		mPointer			= (_byte*) temp;
		mLinePointerBackup	= mPointer;

		return _true;
	}
}
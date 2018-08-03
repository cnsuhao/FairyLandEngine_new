//============================================================================
// RegularExpression.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

// '/', '\', '.', ',', ':', ';', '"', '''
// '~', '!', '@', '#', '$', '%', '^', '*'
// '?', '+', '-', '=', '|', '\t', ' '
// '<', '>', '(', ')', '[', ']', '{', '}'

const _chara* gMatchSpace_Inner =
"[ \t]";

const _chara* gMatchSpace_Pre =
"(^|[ \t])";

const _chara* gMatchSpace_Post =
"($|[ \t])";

const _chara* gMatchNoneWord_Inner =
"["
"/\\\\.,:;\"'"
"~!@#\\$%\\^\\*"
"\\?\\+-=\\|\t "
"<>\\(\\)\\[\\]\\{\\}"
"0-9"
"]";

const _chara* gMatchNoneWord_Pre =
"(^|["
"/\\\\.,:;\"'"
"~!@#\\$%\\^\\*"
"\\?\\+-=\\|\t "
"<>\\(\\)\\[\\]\\{\\}"
"0-9"
"])";

const _chara* gMatchNoneWord_Post =
"($|["
"/\\\\.,:;\"'"
"~!@#\\$%\\^\\*"
"\\?\\+-=\\|\t "
"<>\\(\\)\\[\\]\\{\\}"
"0-9"
"])";

//----------------------------------------------------------------------------
// Helper Function
//----------------------------------------------------------------------------

inline _void PutFixHelper( _chara*& temp, const _chara* fix, _chara more )
{
	StringFormatter::CopyString( temp, fix );
	temp += StringFormatter::StringLength( fix );

	temp[0] = more;
	temp ++;
	temp[0] = 0;
}

inline _bool IsNoneWordHelper( _chara x )
{
	switch ( x )
	{
		case '/':
		case '\\':
		case '.':
		case ',':
		case ':':
		case ';':
		case '\"':
		case '\'':
		case '~':
		case '!':
		case '@':
		case '#':
		case '$':
		case '%':
		case '^':
		case '*':
		case '?':
		case '+':
		case '-':
		case '=':
		case '|':
		case '\t':
		case ' ':
		case '<':
		case '>':
		case '(':
		case ')':
		case '[':
		case ']':
		case '{':
		case '}':
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			return _true;
	}

	return _false;
}

//----------------------------------------------------------------------------
// RegularExpression Implementation
//----------------------------------------------------------------------------

RegularExpression::RegularExpression( )
	: mPCREContext( _null ), mMatchOption( 0 )
{
}

RegularExpression::~RegularExpression( )
{
	// Release pcre result data
	if ( mPCREContext != _null )
		::free( mPCREContext );
}

_bool RegularExpression::SetRegularExpression( StringPtr pattern, _dword option )
{
	// Release pcre result data
	if ( mPCREContext != _null )
	{
		::free( mPCREContext );
		mPCREContext = _null;
	}

	_dword pcreoption = PCRE_UTF8;

	if ( option & _MATCH_CASELESS )
		pcreoption |= PCRE_CASELESS;

	_chara patternstring[4096];
	_chara* temp = patternstring;

	// Put prefix none word.
	if ( option & _MATCH_PRE_NONEWORD )
	{
		PutFixHelper( temp, gMatchNoneWord_Pre, '+' );
	}
	// Put prefix space.
	else if ( option & _MATCH_PRE_SPACE )
	{
		PutFixHelper( temp, gMatchSpace_Pre, '+' );
	}

	if ( option & _MATCH_SEQUENCE )
	{
		temp[0] = '(';
		temp ++;
		temp[0] = 0;
	}

	// Put inner none word.
	if ( option & _MATCH_INNER_NONEWORD )
	{
		_char charbuffer[2] = { 0 };

		for ( _dword i = 0; pattern[i] != 0; i ++ )
		{
			charbuffer[0] = pattern[i];

			_dword charcopy = 0;
			StringFormatter::UnicodeToUTF8( temp, 4000 - (_dword) ( temp - patternstring ), charbuffer, &charcopy );
			temp += charcopy;

			if ( pattern[i+1] != 0 )
				PutFixHelper( temp, gMatchNoneWord_Inner, '*' );
		}
	}
	// Put inner space.
	else if ( option & _MATCH_INNER_SPACE )
	{
		_char charbuffer[2] = { 0 };

		for ( _dword i = 0; pattern[i] != 0; i ++ )
		{
			charbuffer[0] = pattern[i];

			_dword charcopy = 0;
			StringFormatter::UnicodeToUTF8( temp, 2048 - (_dword) ( temp - patternstring ), charbuffer, &charcopy );
			temp += charcopy;

			if ( pattern[i+1] != 0 )
				PutFixHelper( temp, gMatchSpace_Inner, '*' );
		}
	}
	// Convert pattern string to UTF-8.
	else
	{
		_dword charcopy = 0;
		StringFormatter::UnicodeToUTF8( temp, 2048 - (_dword) ( temp - patternstring ), pattern, &charcopy );
		temp += charcopy;
	}

	if ( option & _MATCH_SEQUENCE )
	{
		temp[0] = ')';
		temp ++;
		temp[0] = '+';
		temp ++;
		temp[0] = 0;
	}

	// Put post none word.
	if ( option & _MATCH_POST_NONEWORD )
	{
		PutFixHelper( temp, gMatchNoneWord_Post, '+' );
	}
	// Put post space.
	else if ( option & _MATCH_POST_SPACE )
	{
		PutFixHelper( temp, gMatchSpace_Post, '+' );
	}

	// Pcre error string
	const _chara*	errorstring	= _null;
	_long			erroroffset	= 0;

	// Try to compile the pattern string
	mPCREContext = ::pcre_compile( patternstring, pcreoption, &errorstring, &erroroffset, _null );
	if ( mPCREContext == _null )
		return _false;

	mMatchOption = option;

	return _true;
}

_dword RegularExpression::Match( const _charw* subject, _dword* startindex, _dword* endindex, _dword length, _bool matchall )
{
	FL_ASSERT( subject != _null )

	if ( mPCREContext == _null )
		return 0;

	// Convert subject string to UTF-8
	_chara tempbuffer[4096];
	StringFormatter::UnicodeToUTF8( tempbuffer, 4096, subject );

	return Match( tempbuffer, startindex, endindex, length, matchall );
}

_dword RegularExpression::Match( const _chara* subject, _dword* startindex, _dword* endindex, _dword length, _bool matchall )
{
	FL_ASSERT( subject != _null )

	if ( mPCREContext == _null )
		return 0;

	_long matchindex = 0, strindex = 0, substringnumber = 0, matchbuffer[1024];

	const _chara* strinput = subject;

	do
	{
		// Execute subject string with pattern
		substringnumber = ::pcre_exec( (const pcre*) mPCREContext, _null, strinput, StringFormatter::StringLength( strinput ), 0, 0, matchbuffer, 1024 );

		if ( substringnumber > 0 )
		{
			_long a = matchbuffer[0];
			_long b = matchbuffer[1];

			if ( a == -1 || b == -1 )
				break;

			// Exclude head space.
			if ( ( mMatchOption & _MATCH_PRE_SPACE ) || ( mMatchOption & _MATCH_PRE_NONEWORD ) )
			{
				while ( a < b )
				{
					if ( IsNoneWordHelper( strinput[a] ) == _false )
						break;

					a ++;
				}
			}

			// Exclude back space.
			if ( ( mMatchOption & _MATCH_POST_SPACE ) || ( mMatchOption & _MATCH_POST_NONEWORD ) )
			{
				while ( a < b )
				{
					if ( IsNoneWordHelper( strinput[b-1] ) == _false )
						break;

					b --;
				}
			}

			// Save start and end index.
			startindex[matchindex] = strindex + a;
			endindex[matchindex]   = strindex + b;

			strinput += matchbuffer[1];
			strindex += matchbuffer[1];

			matchindex ++;

			if ( matchindex == length )
				break;
		}

		if ( matchall == _false )
			break;
	}
	while ( substringnumber > 0 );

	// Temp buffer.
	_char tempbuffer[4096]; _dword charcopy = 0;

	// Check index by change string back to UCS-2.
	for ( _long i = 0; i < matchindex; i ++ )
	{
		if ( startindex[i] != 0 )
		{
			StringFormatter::UTF8ToUnicode( tempbuffer, 4096, subject, startindex[i], &charcopy );
			startindex[i] = charcopy;
		}

		if ( endindex[i] != 0 )
		{
			StringFormatter::UTF8ToUnicode( tempbuffer, 4096, subject, endindex[i], &charcopy );
			endindex[i] = charcopy;
		}
	}

	return matchindex;
}
//============================================================================
// HyperTextParser.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// HyperTextParser Implementation
//----------------------------------------------------------------------------

HyperTextParser::HyperTextParser( StringPtr textsource ) : mTextSource( textsource )
{
}

_dword HyperTextParser::ParseTokenStart( StringPtr textsource, String& token, _dword& formatstart )
{
	_dword tokenstart = textsource.SearchL2R( '>' );
	if ( tokenstart == -1 || tokenstart < 2 )
		return 0;

	_bool validcode = _false;

	_dword i = 0;

	// Set token name.
	for ( i = 1; i < tokenstart; i ++ )
	{
		// Found some valid code.
		if ( textsource[i] != ' ' && textsource[i] != '\t' )
		{
			validcode = _true;
			continue;
		}

		if ( validcode && ( textsource[i] == ' ' || textsource[i] == '\t' ) )
		{
			formatstart = i + 1;

			token = String( (const _char*) textsource + 1, i - 1 );
			break;
		}
	}

	if ( token == L"" )
		token = String( (const _char*) textsource + 1, i - 1 );

	return tokenstart + 1;
}

_dword HyperTextParser::ParseTokenEnd( StringPtr textsource, StringPtr token, _dword& textlength )
{
	textlength = 0;

	_char tokenbuffer[1024];
	StringFormatter::FormatBuffer( tokenbuffer, 1024, L"</%s>", token );

	_dword tokenend = textsource.SearchL2RLowercase( tokenbuffer );
	if ( tokenend == -1 )
		return 0;

	textlength = tokenend;

	return tokenend + StringPtr( tokenbuffer ).Length( );
}

_void HyperTextParser::ParseTextFormat( StringPtr textsource )
{
	_char buffer1[1024]; buffer1[0] = 0; _dword index1 = 0;
	_char buffer2[1024]; buffer2[0] = 0; _dword index2 = 0;

	_dword type = 0;

	for ( _dword i = 0; textsource[i] != 0; i ++ )
	{
		if ( textsource[i] == '>' )
		{
			if ( index1 != 0 )
			{
				buffer1[ index1 ] = 0;
				buffer2[ index2 ] = 0;

				HyperTextFormat newformat;
				newformat.mKey	 = buffer1;
				newformat.mValue = buffer2;

				mCurrentFormatSet.mFormatArray.Append( newformat );
			}

			return;
		}

		// Found some space, we can save this format.
		if ( textsource[i] == ' ' || textsource[i] == '\t' )
		{
			if ( index1 != 0 )
			{
				buffer1[ index1 ] = 0;
				buffer2[ index2 ] = 0;

				HyperTextFormat newformat;
				newformat.mKey	 = buffer1;
				newformat.mValue = buffer2;

				mCurrentFormatSet.mFormatArray.Append( newformat );
			}

			index1 = 0;
			index2 = 0;
			type   = 0;
		}
		else
		{
			// Found some not empty character, save as key or value.
			if ( textsource[i] == '=' )
			{
				type = 1;
			}
			else if ( type == 0 )
			{
				buffer1[ index1 ++ ] = textsource[i];
			}
			else
			{
				buffer2[ index2 ++ ] = textsource[i];
			}
		}
	}
}

_bool HyperTextParser::Parse( )
{
	mCurrentText = L"";
	
	mCurrentFormatSet.mFormatToken = L"";
	mCurrentFormatSet.mFormatArray.Clear( );

	// Parse finished.
	if ( mTextSource == L"" )
		return _false;

	const _char* startpointer = mTextSource;

	while ( mTextSource[0] != 0 )
	{
		// Found a format start.
		if ( mTextSource[0] == '<' )
		{
			// Already has some text formated.
			if ( startpointer != (const _char*) mTextSource )
			{
				// Set current text.
				mCurrentText = String( startpointer, (_dword) ( (const _char*) mTextSource - startpointer ) );

				return _true;
			}

			String token;

			_dword formatstart = 0;

			// Try to find token start.
			_dword tokenstart = ParseTokenStart( mTextSource, token, formatstart );

			// It is valid token start, we need find token end.
			if ( tokenstart != 0 )
			{
				_dword textlength = 0;

				// Try to find token end.
				_dword tokenend = ParseTokenEnd( (const _char*) mTextSource + tokenstart, token, textlength );

				// Found valid token end.
				if ( tokenend != 0 )
				{
					// Set current text.
					mCurrentText = String( (const _char*) mTextSource + tokenstart, textlength );

					// Set format token.
					mCurrentFormatSet.mFormatToken = token;
					mCurrentFormatSet.mFormatToken.TrimBoth( L" \t" );

					// Parse text format.
					if ( formatstart != 0 )
						ParseTextFormat( (const _char*) mTextSource + formatstart );

					// Adjust text source.
					mTextSource = (const _char*) mTextSource + tokenstart + tokenend;

					return _true;
				}
			}
		}

		mTextSource = (const _char*) mTextSource + 1;
	}

	// Set current text.
	mCurrentText = String( startpointer, (_dword) ( (const _char*) mTextSource - startpointer ) );

	return _true;
}
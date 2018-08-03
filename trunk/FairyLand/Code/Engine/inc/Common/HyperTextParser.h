//============================================================================
// HyperTextParser.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// HyperTextFormat
//----------------------------------------------------------------------------

struct HyperTextFormat
{
	String	mKey;
	String	mValue;
};

//----------------------------------------------------------------------------
// HyperTextFormatSet
//----------------------------------------------------------------------------

struct HyperTextFormatSet
{
	typedef Array< HyperTextFormat > FormatArray;

	String		mFormatToken;
	FormatArray	mFormatArray;
};

//----------------------------------------------------------------------------
// HyperTextParser
//----------------------------------------------------------------------------

class HyperTextParser
{
private:
	StringPtr			mTextSource;

	String				mCurrentText;
	HyperTextFormatSet	mCurrentFormatSet;

	_dword ParseTokenStart( StringPtr textsource, String& token, _dword& formatstart );
	_dword ParseTokenEnd( StringPtr textsource, StringPtr token, _dword& textlength );

	_void ParseTextFormat( StringPtr textsource );

public:
	HyperTextParser( StringPtr textsource );

	inline StringPtr GetCurrentText( ) const;
	inline const HyperTextFormatSet& GetCurrentFormatSet( ) const;

	_bool Parse( );
};

//----------------------------------------------------------------------------
// HyperTextParser Implementation
//----------------------------------------------------------------------------

StringPtr HyperTextParser::GetCurrentText( ) const
{
	return mCurrentText;
}

const HyperTextFormatSet& HyperTextParser::GetCurrentFormatSet( ) const
{
	return mCurrentFormatSet;
}

};
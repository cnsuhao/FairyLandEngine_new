//============================================================================
// TxtFile.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// TxtFile
//----------------------------------------------------------------------------

class TxtFile : public MemFile
{
protected:
	enum
	{
		_UNICODE_UNKNOWN	= 0,
		_UNICODE_YES		= 1,
		_UNICODE_NO			= 2,
	};

	_dword	mUnicode;
	_dword	mLineNumber;
	_byte*	mLinePointerBackup;
	_dword	mTextBeginOffset;

	String	mDivision;

	_void CheckUnicode( );
	_void TrimLine( );
	_void TrimWord( );

	_void UpdateLineNumber( );

	_void CopyString( String& string, const _byte* pointer, _dword length ) const;

public:
	TxtFile( );

	inline _void  SetDivision( StringPtr division );
	inline _bool  IsUnicode( );
	inline _dword GetLineNumber( ) const;

	_bool ReachEnd( );

	_bool ReadText( String& string );
	_bool ReadLine( String& string );
	_bool ReadWord( String& string );
	_bool ReadWordInLine( String& string );
};

//----------------------------------------------------------------------------
// TxtFile Implementation
//----------------------------------------------------------------------------

_void TxtFile::SetDivision( StringPtr division )
{
	mDivision = division;
}

_bool TxtFile::IsUnicode( )
{
	// Check unicode flag.
	if ( mUnicode == _UNICODE_UNKNOWN )
		CheckUnicode( );

	return mUnicode == _UNICODE_YES;
}

_dword TxtFile::GetLineNumber( ) const 
{
	return mLineNumber;
}

};
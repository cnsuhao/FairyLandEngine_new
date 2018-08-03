//============================================================================
// TxtParser.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// TxtParser
//----------------------------------------------------------------------------

class TxtParser
{
public:
	enum _TYPE
	{
		_TYPE_STRING	= 1, // String %s
		_TYPE_INTEGER	= 2, // Integer %d
		_TYPE_FLOAT		= 3, // Float %f
		_TYPE_HEX		= 4, // Hex %x
		_TYPE_PERCENT	= 5, // Percent %p
	};

	enum _CONTENT_FLAG
	{
		_CONTENT_FLAG_CONFIG		= 1,
		_CONTENT_FLAG_MAPPING_TEXT	= 2,
	};

public:
	typedef String (*OnText)( const TxtParser* txtparser, StringPtr keyname, StringPtr content, StringPtr text );
	typedef String (*OnContent)( const TxtParser* txtparser, _dword type, StringPtr content );

public:
	inline _void SetTextCallback( OnText funcpointer );
	inline _void SetContentCallback( OnContent funcpointer );

public:
	_bool		CreateSection( StringPtr name, _bool settocurrentsection = _false );
	_bool		SearchSection( StringPtr name ) const;
	_void		RemoveSection ( StringPtr name );
	_void		RemoveAllSection( );

	_bool		SetCurrentSection( StringPtr name );
	_void		CleanCurrentSection( );
	StringPtr	GetCurrentSectionName( ) const;

	_dword		GetSectionNumber( ) const;
	StringPtr	GetSectionName( _dword index ) const;

	_bool		AddText( StringPtr keyname, StringPtr content, _dword flag = _CONTENT_FLAG_CONFIG );
	_bool		UpdateText( StringPtr keyname, StringPtr content, _dword flag = _CONTENT_FLAG_CONFIG );
	_void		RemoveText( StringPtr keyname, _dword flag = _CONTENT_FLAG_CONFIG );
	_void		RemoveAllText( _dword flag = _CONTENT_FLAG_CONFIG );
	StringPtr	SearchText( StringPtr keyname, _dword flag = _CONTENT_FLAG_CONFIG ) const;

	_bool		Save( MemFile& memfile ) const;
	_bool		Load( TxtFile& txtfile );

	inline String GetText( StringPtr keyname ) const;

	template < typename T1 >
	inline String GetText( StringPtr keyname, T1 type1 ) const;

	template < typename T1, typename T2 >
	inline String GetText( StringPtr keyname, T1 type1, T2 type2 ) const;

	template < typename T1, typename T2, typename T3 >
	inline String GetText( StringPtr keyname, T1 type1, T2 type2, T3 type3 ) const;

	template < typename T1, typename T2, typename T3, typename T4 >
	inline String GetText( StringPtr keyname, T1 type1, T2 type2, T3 type3, T4 type4 ) const;

	template < typename T1, typename T2, typename T3, typename T4, typename T5 >
	inline String GetText( StringPtr keyname, T1 type1, T2 type2, T3 type3, T4 type4, T5 type5 ) const;

	template < typename T1, typename T2, typename T3, typename T4, typename T5, typename T6 >
	inline String GetText( StringPtr keyname, T1 type1, T2 type2, T3 type3, T4 type4, T5 type5, T6 type6 ) const;

	template < typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7 >
	inline String GetText( StringPtr keyname, T1 type1, T2 type2, T3 type3, T4 type4, T5 type5, T6 type6, T7 type7 ) const;

	template < typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8 >
	inline String GetText( StringPtr keyname, T1 type1, T2 type2, T3 type3, T4 type4, T5 type5, T6 type6, T7 type7, T8 type8 ) const;

public:
	TxtParser( );
	~TxtParser( );

private:
	//!	Config
	struct Config
	{
		_dword	mKeyID;
		String	mKeyName;
		String	mContent;

		operator _dword( ) const
		{
			return mKeyID;
		}

		Config( _dword keyid = 0, StringPtr keyname = L"", StringPtr content = L"" ) : mKeyID( keyid ), mKeyName( keyname ), mContent( content )
		{}
	};
	typedef Array< Config, _dword > ConfigArray;

	//!	Section
	struct Section
	{
		String		mName;

		ConfigArray	mConfigArray;
		ConfigArray	mMappingTextDataArray;

		operator StringPtr( ) const
		{
			return mName;
		}

		Section( StringPtr name = L"" ) : mName( name )
		{}
	};
	typedef Array< Section, StringPtr > SectionArray;

private:
	_dword			mCurrentSectionIndex;
	SectionArray	mSectionArray;

	OnText			mTextFunc;
	OnContent		mContentFunc;

private:
	inline ConfigArray* GetConfigArray( _dword flag ) const;

	_bool		AddText( ConfigArray* configarray, StringPtr keyname, StringPtr content );
	_bool		UpdateText( ConfigArray* configarray, StringPtr keyname, StringPtr content );
	_void		RemoveText( ConfigArray* configarray, StringPtr keyname );
	_void		RemoveAllText( ConfigArray* configarray );
	StringPtr	SearchText( const ConfigArray* configarray, _dword keyid ) const;
	StringPtr	SearchText( const ConfigArray* configarray, StringPtr keyname ) const;

	_bool 		Parse( StringPtr keyname, _dword& startindex, _dword& number, _dword& type, _dword& commandstringlength, _char buffer[] ) const;

	_void 		Insert( _char buffer[], _dword type, _long content ) const;
	_void 		Insert( _char buffer[], _dword type, _float content ) const;
	_void 		Insert( _char buffer[], _dword type, StringPtr content ) const;

	_bool		AddTextFromFile( const String& string, _dword flag );

	StringPtr	AppendText( _char buffer[], _dword length, StringPtr name, StringPtr text ) const;

	template < typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8 >
	String		CoreGetText( StringPtr keyname, _dword maxnumber, T1 type1, T2 type2, T3 type3, T4 type4, T5 type5, T6 type6, T7 type7, T8 type8 ) const;
};

//----------------------------------------------------------------------------
// TxtParser Implementation
//----------------------------------------------------------------------------

_void TxtParser::SetTextCallback( OnText funcpointer )
{
	mTextFunc = funcpointer;
}

_void TxtParser::SetContentCallback( OnContent funcpointer )
{
	mContentFunc = funcpointer;
}

TxtParser::ConfigArray* TxtParser::GetConfigArray( _dword flag ) const
{
	if ( flag == _CONTENT_FLAG_CONFIG )
		return (ConfigArray*) &mSectionArray[ mCurrentSectionIndex ].mConfigArray;
	else if ( flag == _CONTENT_FLAG_MAPPING_TEXT )
		return (ConfigArray*) &mSectionArray[ mCurrentSectionIndex ].mMappingTextDataArray;
	else
		return _null;
}

String TxtParser::GetText( StringPtr keyname ) const
{
	return CoreGetText( keyname, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
}

template < typename T1 >
String TxtParser::GetText( StringPtr keyname, T1 type1 ) const
{
	return CoreGetText( keyname, 1, type1, 0, 0, 0, 0, 0, 0, 0 );
}

template < typename T1, typename T2 >
String TxtParser::GetText( StringPtr keyname, T1 type1, T2 type2 ) const
{
	return CoreGetText( keyname, 2, type1, type2, 0, 0, 0, 0, 0, 0 );
}

template < typename T1, typename T2, typename T3 >
String TxtParser::GetText( StringPtr keyname, T1 type1, T2 type2, T3 type3 ) const
{
	return CoreGetText( keyname, 3, type1, type2, type3, 0, 0, 0, 0, 0 );
}

template < typename T1, typename T2, typename T3, typename T4 >
String TxtParser::GetText( StringPtr keyname, T1 type1, T2 type2, T3 type3, T4 type4 ) const
{
	return CoreGetText( keyname, 4, type1, type2, type3, type4, 0, 0, 0, 0 );
}

template < typename T1, typename T2, typename T3, typename T4, typename T5 >
String TxtParser::GetText( StringPtr keyname, T1 type1, T2 type2, T3 type3, T4 type4, T5 type5 ) const
{
	return CoreGetText( keyname, 5, type1, type2, type3, type4, type5, 0, 0, 0 );
}

template < typename T1, typename T2, typename T3, typename T4, typename T5, typename T6 >
String TxtParser::GetText( StringPtr keyname, T1 type1, T2 type2, T3 type3, T4 type4, T5 type5, T6 type6 ) const
{
	return CoreGetText( keyname, 6, type1, type2, type3, type4, type5, type6, 0, 0 );
}

template < typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7 >
String TxtParser::GetText( StringPtr keyname, T1 type1, T2 type2, T3 type3, T4 type4, T5 type5, T6 type6, T7 type7 ) const
{
	return CoreGetText( keyname, 7, type1, type2, type3, type4, type5, type6, type7, 0 );
}

template < typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8 >
String TxtParser::GetText( StringPtr keyname, T1 type1, T2 type2, T3 type3, T4 type4, T5 type5, T6 type6, T7 type7, T8 type8 ) const
{
	return CoreGetText( keyname, 8, type1, type2, type3, type4, type5, type6, type7, type8 );
}

template < typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8 >
String TxtParser::CoreGetText( StringPtr keyname, _dword maxnumber, T1 type1, T2 type2, T3 type3, T4 type4, T5 type5, T6 type6, T7 type7, T8 type8 ) const
{
	FL_ASSERT( maxnumber <= 8 );

	// Search content by keyname
	StringPtr content = SearchText( keyname );
	if ( content == L"" )
		return L"";

	// Feedback buffer
	_char buffer[ 2048 ]; buffer[ 0 ] = 0;

	// Start index
	_dword startindex = 0;

	// Start to parse
	if ( maxnumber == 0 )
	{
		StringFormatter::CopyString( buffer, content, 2047 );
	}
	else
	{
		while ( _true )
		{
			// command string length
			_dword commandstringlength = 0;

			// parse string
			_dword number	= 0;
			_dword type		= 0;
			if ( Parse( content, startindex, number, type, commandstringlength, buffer ) == _false )
				return _false;

			// command is over 
			if ( commandstringlength == 0 )
				break;

			// check number bound
			if ( number > maxnumber )
				return _false;

			// insert content
			if ( number == 1 )
				Insert( buffer, type, type1 );
			else if ( number == 2 )
				Insert( buffer, type, type2 );
			else if ( number == 3 )
				Insert( buffer, type, type3 );
			else if ( number == 4 )
				Insert( buffer, type, type4 );
			else if ( number == 5 )
				Insert( buffer, type, type5 );
			else if ( number == 6 )
				Insert( buffer, type, type6 );
			else if ( number == 7 )
				Insert( buffer, type, type7 );
			else if ( number == 8 )
				Insert( buffer, type, type8 );

			// jump to next command string
			startindex += commandstringlength;
		}
	}

	// Get current section
	const Section& currentsection = mSectionArray[ mCurrentSectionIndex ];

	// Try to replace mapping text string
	for ( _dword i = 0; i < currentsection.mMappingTextDataArray.Number( ); i ++ )
	{
		const Config& mappingtextdata = currentsection.mMappingTextDataArray[i];

		StringFormatter::Replace( buffer, 2047, mappingtextdata.mKeyName, mappingtextdata.mContent );
	}

	// Run text callback function
	if ( mTextFunc != _null )
		return (*mTextFunc)( this, keyname, content, buffer );

	return buffer;
}

}
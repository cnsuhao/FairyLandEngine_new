//============================================================================
// TxtParser.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

const static StringPtr cMappingTextSeparatorString = L"--- Mapping Text ---";

//----------------------------------------------------------------------------
// TxtParser Implementation
//----------------------------------------------------------------------------

TxtParser::TxtParser( )
{
	mTextFunc				= _null;
	mContentFunc			= _null;

	mCurrentSectionIndex	= -1;
}

TxtParser::~TxtParser( )
{
}

_bool TxtParser::CreateSection( StringPtr name, _bool settocurrentsection )
{
	// Search existing sections
	SectionArray::Iterator it = mSectionArray.SearchAscending( name );
	if ( it.Valid( ) == _true )
	{
		// Update the current section
		if ( settocurrentsection == _true )
			mCurrentSectionIndex = it.mIndex;

		// Can't overlap it
		return _true; 
	}

	// Insert section
	_dword sectionindex = mSectionArray.InsertAscending( Section( name ) );

	// Does need to set to current section ?
	if ( settocurrentsection == _true )
		mCurrentSectionIndex = sectionindex;

	return _true;
}

_bool TxtParser::SearchSection( StringPtr name ) const
{
	return mSectionArray.SearchAscending( name ).Valid( );
}

_void TxtParser::RemoveSection( StringPtr name )
{
	// Search section what to be removed
	SectionArray::Iterator it = mSectionArray.SearchAscending( name );
	if ( it.Valid( ) == _false )
		return;

	// Clean current section index if it's need
	if ( it.mIndex == mCurrentSectionIndex )
		mCurrentSectionIndex = -1;

	// Remove section
	mSectionArray.Remove( it );
}

_void TxtParser::RemoveAllSection( )
{
	mCurrentSectionIndex = -1;

	mSectionArray.Clear( );
}

_bool TxtParser::SetCurrentSection( StringPtr name )
{
	// Search current by name
	SectionArray::Iterator it = mSectionArray.SearchAscending( name );
	if ( it.Valid( ) == _false )
		return _false;

	// Update current section index
	mCurrentSectionIndex = it.mIndex;

	return _true;
}

_void TxtParser::CleanCurrentSection( )
{
	mCurrentSectionIndex = -1;
}

StringPtr TxtParser::GetCurrentSectionName( ) const
{
	if ( mCurrentSectionIndex == -1 )
		return L"";

	if ( mCurrentSectionIndex >= mSectionArray.Number( ) )
		return L"";

	return mSectionArray[ mCurrentSectionIndex ].mName;
}

_dword TxtParser::GetSectionNumber( ) const
{
	return mSectionArray.Number( );
}

StringPtr TxtParser::GetSectionName( _dword index ) const
{
	if ( index >= mSectionArray.Number( ) )
		return L"";

	return mSectionArray[ index ].mName;
}

_bool TxtParser::AddText( StringPtr keyname, StringPtr content, _dword flag )
{
	if ( mCurrentSectionIndex == -1 )
		return _false;

	return AddText( GetConfigArray( flag ), keyname, content );
}

_bool TxtParser::UpdateText( StringPtr keyname, StringPtr content, _dword flag )
{
	if ( mCurrentSectionIndex == -1 )
		return _false;

	return UpdateText( GetConfigArray( flag ), keyname, content );
}

_void TxtParser::RemoveText( StringPtr keyname, _dword flag )
{
	if ( mCurrentSectionIndex == -1 )
		return;

	RemoveText( GetConfigArray( flag ), keyname );
}

_void TxtParser::RemoveAllText( _dword flag )
{
	if ( mCurrentSectionIndex == -1 )
		return;

	RemoveAllText( GetConfigArray( flag ) );
}

StringPtr TxtParser::SearchText( StringPtr keyname, _dword flag ) const
{
	if ( mCurrentSectionIndex == -1 )
		return L"";

	return SearchText( GetConfigArray( flag ), keyname );
}

_bool TxtParser::Save( MemFile& memfile ) const
{
	String stringbuffer;

	// Write config array into string buffer
	for ( _dword i = 0; i < mSectionArray.Number( ); i ++ )
	{
		const Section& section = mSectionArray[i];

		// Write section start tag
		_char buffer[ 1024 ];
		stringbuffer += StringFormatter::FormatBuffer( buffer, 1024, L"%s\r\n{\r\n", ( const _char* ) section.mName );

		// Write config array into string buffer
		for ( _dword j = 0; j < section.mConfigArray.Number( ); j ++ )
		{
			// Get keyname and content
			StringPtr keyname	= section.mConfigArray[j].mKeyName;
			StringPtr content	= section.mConfigArray[j].mContent;

			// Write string 
			stringbuffer += AppendText( buffer, 1024, keyname, content );
		}

		// Write mapping text data
		if ( section.mMappingTextDataArray.Number( ) > 0 )
		{
			// Write a separator symbol
			stringbuffer += cMappingTextSeparatorString;
			stringbuffer += L"\r\n";

			// Write mapping text data into string buffer
			for ( _dword j = 0; j < section.mMappingTextDataArray.Number( ); j ++ )
			{
				// Get name and text
				StringPtr name = section.mMappingTextDataArray[j].mKeyName;
				StringPtr text = section.mMappingTextDataArray[j].mContent;

				// Write string 
				stringbuffer += AppendText( buffer, 1024, name, text );
			}
		}

		// Write section end tag
		stringbuffer += L"}\r\n\r\n";
	}

	// Calculate the size of buffer ( Unicode header + Data size )
	_dword size = sizeof( _char ) + ( stringbuffer.Length( ) * sizeof( _char ) );

	// Create new buffer 
	BinFile& binfile = (BinFile&) memfile;
	binfile.Create( size );

	// Write unicode code header
	binfile.WriteWord( 0xFEFF );

	// Write config array into new buffer
	return binfile.WriteBuffer( stringbuffer.GetString( ), stringbuffer.Length( ) * sizeof( _char ) );
}

_bool TxtParser::Load( TxtFile& txtfile )
{
	// Remove all config array
	RemoveAllSection( );

	// Clean division symbol
	txtfile.SetDivision( L"" );

	// String buffer for loading
	String string;
	String sectionname;

	// Flag
	_dword flag = _CONTENT_FLAG_CONFIG;

	// Load config data
	while ( txtfile.ReachEnd( ) == _false )
	{
		// Read line
		txtfile.ReadLine( string );

		// Is a section data ?
		if ( string == L"{" && sectionname != L"" )
		{
			while ( txtfile.ReachEnd( ) == _false )
			{
				// Read content
				txtfile.ReadLine( string );

				// Is end tag ?
				if ( string == L"}" )
				{
					// Clean section name
					sectionname = L"";

					break;
				}

				// Enter into mapping text section ?
				if ( flag != _CONTENT_FLAG_MAPPING_TEXT && string == cMappingTextSeparatorString )
				{
					flag = _CONTENT_FLAG_MAPPING_TEXT;

					continue;
				}

				// Add content
				if ( AddTextFromFile( string, flag ) == _false )
					return _false;
			}
		}
		// New section ?
		else if ( sectionname == L"" )
		{
			// Set section name
			sectionname = string;

			// Set current section index
			mCurrentSectionIndex = mSectionArray.InsertAscending( Section( sectionname ) );

			// Reset flag
			flag = _CONTENT_FLAG_CONFIG;
		}
	}

	// Clean current section index
	mCurrentSectionIndex = -1;

	return _true;
}

_bool TxtParser::AddText( ConfigArray* configarray, StringPtr keyname, StringPtr content )
{
	// Check config array
	if ( configarray == _null )
		return _false;

	// Can't create a blank keyname
	if ( keyname.IsBlank( ) )
		return _false;

	// Get key id
	_dword keyid = keyname;

	// Search existing text by key name
	ConfigArray::Iterator it = configarray->Search( keyid );
	if ( it.Valid( ) == _true )
	{
		// Update content
		( ( Config& ) it ).mContent = content;
	}
	else
	{
		// Insert an new key and content
		configarray->Append( Config( keyid, keyname, content ) );
	}

	return _true;
}

_bool TxtParser::UpdateText( ConfigArray* configarray, StringPtr keyname, StringPtr content )
{
	// Check config array
	if ( configarray == _null )
		return _false;

	// Search config array
	ConfigArray::Iterator it = configarray->Search( keyname );
	if ( it.Valid( ) == _false )
		return _false;

	// Update content
	( ( Config& ) it ).mContent = content;

	return _true;
}

_void TxtParser::RemoveText( ConfigArray* configarray, StringPtr keyname )
{
	// Check config array
	if ( configarray == _null )
		return;

	// Search config array
	ConfigArray::Iterator it = configarray->Search( keyname );
	if ( it.Valid( ) == _false )
		return;

	// Remove it
	configarray->Remove( it );
}

_void TxtParser::RemoveAllText( ConfigArray* configarray )
{
	if ( configarray != _null )
		configarray->Clear( );
}

StringPtr TxtParser::SearchText( const ConfigArray* configarray, _dword keyid ) const
{
	// Check config array
	if ( configarray == _null )
		return _false;

	// Search config array
	ConfigArray::Iterator it = configarray->Search( keyid );
	if ( it.Valid( ) == _false )
		return L"";

	return ( ( const Config& ) it ).mContent;
}

StringPtr TxtParser::SearchText( const ConfigArray* configarray, StringPtr keyname ) const
{
	// Check config array
	if ( configarray == _null )
		return _false;

	// Search config array
	ConfigArray::Iterator it = configarray->Search( keyname );
	if ( it.Valid( ) == _false )
		return L"";

	return ( ( const Config& ) it ).mContent;
}

_bool TxtParser::Parse( StringPtr keyname, _dword& startindex, _dword& number, _dword& type, _dword& commandstringlength, _char buffer[] ) const
{
	_char	stringbuffer[ 2048 ]; stringbuffer[ 0 ] = 0;
	_char*	address = stringbuffer;

	// Start to parse the string
	while ( keyname[ startindex ] != 0 )
	{
		_char character		= keyname[ startindex ];
		_char nextcharacter = keyname[ startindex + 1 ];

		// Check character style
		if ( character == '%' )
		{
			// Update command string length
			commandstringlength = 2;

			// Get next character
			if ( nextcharacter == 0 )
				return _false;

			// Feedback command type
			if ( nextcharacter == 's' || nextcharacter == 'S' )
			{
				// String 
				type = _TYPE_STRING;
			}
			else if ( nextcharacter == 'd' || nextcharacter == 'D' )
			{
				// Integer number
				type = _TYPE_INTEGER;
			}
			else if ( nextcharacter == 'f' || nextcharacter == 'F' )
			{
				// Float number
				type = _TYPE_FLOAT;
			}
			else if ( nextcharacter == 'x' || nextcharacter == 'X' )
			{
				// Hex number
				type = _TYPE_HEX;
			}
			else if ( nextcharacter == 'p' || nextcharacter == 'P' )
			{
				// Percent number
				type = _TYPE_PERCENT;
			}
			else
			{
				// Unknown command type
				return _false;
			}

			// Get number character
			_char numbercharacter = keyname[ startindex + 2 ];
			if ( numbercharacter == 0 )
				return _false;

			// Update command string length
			commandstringlength ++;

			// Feedback number
			if ( numbercharacter > '9' || numbercharacter < '0' )
				return _false;

			number = numbercharacter - '0';

			// Parse completed
			break;
		}

		// Insert character to buffer
		*address ++ = character;

		// Next character
		startindex ++;		
	}

	// Write 'Null' end tag
	*address = 0;

	// Combine string
	StringFormatter::AppendString( buffer, stringbuffer );

	return _true;
}

_void TxtParser::Insert( _char buffer[], _dword type, _long content ) const
{
	_char valuestring[ 1024 ]; valuestring[ 0 ] = 0;

	if ( type == _TYPE_INTEGER )
	{
		// Integer
		StringFormatter::CopyString( valuestring, Long( content ).ToString( ), 1023 );
	}
	else if ( type == _TYPE_HEX )
	{
		// Hex
		StringFormatter::AppendString( valuestring, L"0x" );
		StringFormatter::AppendString( valuestring, Dword( content ).ToString( ) );
	}
	else if ( type == _TYPE_FLOAT )
	{
		// Float
		StringFormatter::CopyString( valuestring, Float( (_float) content ).ToString( ), 1023 );
	}
	else if ( type == _TYPE_PERCENT )
	{
		// Percent
		StringFormatter::CopyString( valuestring, Long( content ).ToString( ), 1023 );
		StringFormatter::AppendString( valuestring, L"%%" );
	}
	else
	{
		// Unknown type for this content
		StringFormatter::CopyString( valuestring, L"?", 1023 );
	}

	// Run content callback function
	if ( mContentFunc != _null )
	{
		String string = (*mContentFunc)( this, type, valuestring );
		if ( string != L"" )
			StringFormatter::CopyString( valuestring, string, 1023 );
	}

	// Combine string
	StringFormatter::AppendString( buffer, valuestring );
}

_void TxtParser::Insert( _char buffer[], _dword type, _float content ) const
{
	_char valuestring[ 1024 ]; valuestring[ 0 ] = 0;

	if ( type == _TYPE_INTEGER || type == _TYPE_FLOAT )
	{
		// Integer or Float
		StringFormatter::CopyString( valuestring, Float( content ).ToString( ), 1023 );
	}
	else if ( type == _TYPE_PERCENT )
	{
		// Percent
		StringFormatter::CopyString( valuestring, Float( content ).ToString( ), 1023 );
		StringFormatter::AppendString( valuestring, L"%%" );
	}
	else
	{
		// Unknown type for this content
		StringFormatter::CopyString( valuestring, L"?", 1023 );
	}

	// Run content callback function
	if ( mContentFunc != _null )
	{
		String string = (*mContentFunc)( this, type, valuestring );
		if ( string != L"" )
			StringFormatter::CopyString( valuestring, string, 1023 );
	}

	// Combine string
	StringFormatter::AppendString( buffer, valuestring );
}

_void TxtParser::Insert( _char buffer[], _dword type, StringPtr content ) const
{
	_char valuestring[ 1024 ]; valuestring[ 0 ] = 0;

	if ( type == _TYPE_STRING )
	{
		// String
		StringFormatter::CopyString( valuestring, content, 1023 );
	}
	else
	{
		// Unknown type for this content
		StringFormatter::CopyString( valuestring, L"?", 1023 );
	}

	// Run content callback function
	if ( mContentFunc != _null )
	{
		String string = (*mContentFunc)( this, type, valuestring );
		if ( string != L"" )
			StringFormatter::CopyString( valuestring, string, 1023 );
	}

	// Combine string
	StringFormatter::AppendString( buffer, valuestring );
}

_bool TxtParser::AddTextFromFile( const String& string, _dword flag )
{
	// Locate equal symbol position
	_dword equalindex = string.SearchL2R( '=' );
	if ( equalindex == -1 )
		return _false;

	// Read keyname
	String keyname = string.SubString( 0, equalindex );
	keyname.TrimBoth( L" \t" );
	if ( keyname.Length( ) <= 2 )
		return _false; // keyname is invalid

	// Read value
	String content = StringPtr( string ).SubString( equalindex + 1 );
	content.TrimBoth( L" \t" );
	if ( content.Length( ) <= 2 )
		return _false; // value is invalid

	// Remove [] symbol
	keyname.Remove( 0, 1 );
	keyname.Remove( keyname.Length( ) - 1, 1 );
	content.Remove( 0, 1 );
	content.Remove( content.Length( ) - 1, 1 );

	// Replace real command text string to command string
	while ( content.Replace( L"\\r", L"\r" ) ) {}
	while ( content.Replace( L"\\n", L"\n" ) ) {}
	while ( content.Replace( L"\\t", L"\t" ) ) {}

	// Add config data
	return AddText( keyname, content, flag );
}

StringPtr TxtParser::AppendText( _char buffer[], _dword length, StringPtr name, StringPtr text ) const
{
	// Format string for writing
	StringFormatter::FormatBuffer( buffer, length, L"\t[%s]\t=\t[%s]", name, text );

	// Replace command string to real command text string
	StringFormatter::Replace( buffer, length, L"\r", L"\\r" );
	StringFormatter::Replace( buffer, length, L"\n", L"\\n" );
	StringFormatter::Replace( buffer, length, L"\t", L"\\t" );

	// Insert new line tag
	StringFormatter::AppendString( buffer, L"\r\n" );

	return buffer;
}

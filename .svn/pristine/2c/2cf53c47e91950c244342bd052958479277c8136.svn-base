//============================================================================
// CfgFile.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// CfgFile Implementation
//----------------------------------------------------------------------------

CfgFile::CfgFile( )
{
	mEnableErrorCallback				= _true;
	mEnableReadSectionStringCallback	= _true;

	mCurrentSection						= _null;
	mNextSection						= _null;

	mErrorFunc							= _null;
	mReadSectionStringFunc				= _null;

	// Clear division
	SetDivision( L"" );
}

CfgFile::~CfgFile( )
{
	Finalize( );
}

_bool CfgFile::Initialize( )
{
	Finalize( );

	// Create the first section
	SectionData* section	= new SectionData;
	section->mID			= 0;
	section->mFatherID		= -1;
	section->mLevel			= 0;
	section->mLineNumber	= 0;
	section->mUserData1		= 0;
	section->mUserData2		= 0;

	// Add into section array
	mSectionDataArray.Append( section );

	// Start to build section
	if ( BuildSection( section ) == _false )
	{
		if ( mErrorFunc != _null )
		{
			_char buffer[ 1024 ];
			StringFormatter::FormatBuffer( buffer, 1024, L"Build '%s' failed\r\n", ( const _char* ) mFileName );

			(*mErrorFunc)( this, L"", buffer, mFileName );
		}

		return _false;
	}

	// Check status
	if ( ReachEnd( ) == _false )
	{
		if ( mErrorFunc != _null )
		{
			_char buffer[ 1024 ];
			StringFormatter::FormatBuffer( buffer, 1024, L"Read '%s' failed, exception at line %d\r\n", ( const _char* ) mFileName, GetLineNumber( ) );

			(*mErrorFunc)( this, L"", buffer, mFileName );
		}

		return _false;
	}

	// Initialize the next section to the first section data ( not root section data )
	if ( mSectionDataArray.Number( ) > 1 )
		mNextSection = mSectionDataArray[1];

	return _true;
}

_void CfgFile::Finalize( )
{
	mPointer			= mBuffer;
	mBackup				= _null;

	mLineNumber			= 1;
	mLinePointerBackup	= _null;

	mCurrentSection		= _null;
	mNextSection		= _null;

	// Remove all sections
	for ( _dword i = 0; i < mSectionDataArray.Number( ); i ++ )
		delete mSectionDataArray[i];

	mSectionDataArray.Clear( );

	// Remove all read key callback functions
	RemoveAllReadKeyCallback( );
}

_bool CfgFile::Save( StringPtr filename ) const
{
	RootDataArray	rootdataarray;
	DwordArray		rootdataindexarray;

	// Build root data
	for ( _dword i = 0; i < mSectionDataArray.Number( ); i ++ )
	{
		const SectionData* sectiondata = mSectionDataArray[i];

		// Append it into array
		rootdataarray.Append( RootData( mSectionDataArray[ sectiondata->mID ] ) );

		// If it's a root section then record index
		if ( sectiondata->mLevel == 1 )
			rootdataindexarray.Append( sectiondata->mID );

		// Update child id array
		if ( sectiondata->mFatherID != -1 && sectiondata->mFatherID != 0 )
			rootdataarray[ sectiondata->mFatherID ].mChildIDArray.Append( sectiondata->mID );
	}

	// Create file
	File file;
	if ( file.Open( filename, File::_CREATE_ALWAYS, File::_OPERATION_WRITE, File::_SHARE_READ ) == _false )
		return _false;

	// Write unicode flag
	if ( file.WriteUnicodeFlag( ) == _false )
		return _false;

	// Write section data
	for ( _dword i = 0; i < rootdataindexarray.Number( ); i ++ )
	{
		// Get root data
		const RootData& rootdata = rootdataarray[ rootdataindexarray[i] ];

		// Save root data
		if ( Save( file, rootdata, rootdataarray ) == _false )
			return _false;
	}

	return _true;
}

_dword CfgFile::GetSectionNumber( ) const
{
	return mSectionDataArray.Number( );
}

_dword CfgFile::GetSectionID( _dword index ) const
{
	if ( index >= mSectionDataArray.Number( ) )
		return -1;

	return mSectionDataArray[index]->mID;
}

_dword CfgFile::GetSectionID( StringPtr sectionname, StringPtr defname, StringPtr parentsectionname, StringPtr parentdefname ) const
{
	_char lowercasedsectionname[ 1024 ];
	StringFormatter::CopyString( lowercasedsectionname, sectionname, 1023 );
	StringFormatter::LowercaseString( lowercasedsectionname );

	_char lowercasedparentsectionname[ 1024 ];
	StringFormatter::CopyString( lowercasedparentsectionname, parentsectionname, 1023 );
	StringFormatter::LowercaseString( lowercasedparentsectionname );

	// Search section by name
	for ( _dword i = 0; i < mSectionDataArray.Number( ); i ++ )
	{
		const SectionData* section = mSectionDataArray[i];

		// Compare section name
		if ( section->mName != lowercasedsectionname )
			continue;

		// Compare define name
		if ( defname != L"" && section->mDefName.CompareLowercase( defname ) == _false )
			continue;

		if ( section->mFatherID != -1 )
		{
			// Get father section
			const SectionData* fathersection = mSectionDataArray[ section->mFatherID ];

			// Compare parent section name
			if ( parentsectionname != L"" && fathersection->mName != lowercasedparentsectionname )
				continue;

			// Compare parent define name
			if ( parentdefname != L"" && fathersection->mDefName.CompareLowercase( parentdefname ) == _false )
				continue;
		}

		return section->mID;
	}

	return -1;
}

_bool CfgFile::SetSectionUserData1( _dword userdata, _dword sectionid )
{
	// Search section
	SectionData* section = (SectionData*) GetSection( sectionid );
	if ( section == _null )
		return _false;

	// Set user data
	section->mUserData1 = userdata;

	return _true;
}

_bool CfgFile::SetSectionUserData2( _dword userdata, _dword sectionid )
{
	// Search section
	SectionData* section = (SectionData*) GetSection( sectionid );
	if ( section == _null )
		return _false;

	// Set user data
	section->mUserData2 = userdata;

	return _true;
}

_dword CfgFile::GetSectionUserData1( _dword sectionid ) const
{
	// Search section
	const SectionData* section = GetSection( sectionid );
	if ( section == _null )
		return _false;

	// Get user data
	return section->mUserData1;
}

_dword CfgFile::GetSectionUserData2( _dword sectionid ) const
{
	// Search section
	const SectionData* section = GetSection( sectionid );
	if ( section == _null )
		return _false;

	// Get user data
	return section->mUserData2;
}

_dword CfgFile::GetParentSectionID( _dword sectionid ) const
{
	// Get section
	const SectionData* section = GetSection( sectionid );
	if ( section == _null )
		return 0;

	return section->mFatherID;
}

StringPtr CfgFile::GetSectionName( _dword sectionid ) const
{
	// Get section
	const SectionData* section = GetSection( sectionid );
	if ( section == _null )
		return L"";

	return section->mName;
}

StringPtr CfgFile::GetSectionDefName( _dword sectionid ) const
{
	// Get section
	const SectionData* section = GetSection( sectionid );
	if ( section == _null )
		return L"";

	return section->mDefName;
}

StringPtr CfgFile::GetParentSectionName( _dword sectionid ) const
{
	// Get section
	const SectionData* section = GetSection( sectionid );
	if ( section == _null )
		return L"";

	// Check father id
	if ( section->mFatherID == -1 )
		return L"";

	// Get father section data
	const SectionData* fathersection = SearchSection( section->mFatherID );
	if ( fathersection == _null )
		return L"";

	return fathersection->mName;
}

_bool CfgFile::Parse( StringPtr& sectionname, StringPtr* defname, _dword* level, _dword* linenumber )
{
	if ( mNextSection == _null )
		return _false;

	// Update current section
	mCurrentSection = mNextSection;

	// Get section data information
	ParseSectionData( mCurrentSection, sectionname, defname, level, linenumber );

	// Get the next section id
	_dword nextsectionid = mCurrentSection->mID + 1;

	// Update next section
	if ( nextsectionid >= mSectionDataArray.Number( ) )
		mNextSection = _null;
	else
		mNextSection = mSectionDataArray[ nextsectionid ];

	return _true;
}

_bool CfgFile::ReadSection( _dword sectionid, StringPtr& sectionname, StringPtr* defname, _dword* level, _dword* linenumber ) const
{
	// Search section
	const SectionData* section = GetSection( sectionid );
	if ( section == _null )
		return _false;

	ParseSectionData( section, sectionname, defname, level, linenumber );

	return _true;
}

_bool CfgFile::ReadCurrentSection( StringPtr& sectionname, StringPtr* defname, _dword* level, _dword* linenumber ) const
{
	if ( mCurrentSection == _null )
		return _false;

	ParseSectionData( mCurrentSection, sectionname, defname, level, linenumber );

	return _true;
}

_bool CfgFile::ReadSectionString( String& stringbuffer, _dword sectionid )  const
{
	// Get section
	const SectionData* section = GetSection( sectionid );
	if ( section == _null )
		return _false;

	return CoreReadSectionString( section, stringbuffer );
}

_bool CfgFile::ReadSectionString( Array< String >& stringarray, _dword sectionid )  const
{
	// Get section
	const SectionData* section = GetSection( sectionid );
	if ( section == _null )
		return _false;

	return CoreReadSectionString( section, stringarray );
}

_bool CfgFile::ReadSectionString( Array< String >* stringarray, Array< String >* commentarray, Array< String >* keynamearray, _dword sectionid ) const
{
	// Get section
	const SectionData* section = GetSection( sectionid );
	if ( section == _null )
		return _false;

	return CoreReadSectionString( section, stringarray, commentarray, keynamearray );
}

_bool CfgFile::ReadKey( StringPtr keyname, _byte& value, String* comment, _dword* linenumber ) const
{
	return ReadKey< Tiny, _byte >( _TYPE_INTEGER, keyname, value, comment, linenumber );
}

_bool CfgFile::ReadKey( StringPtr keyname, _tiny& value, String* comment, _dword* linenumber ) const
{
	return ReadKey< Tiny, _tiny >( _TYPE_INTEGER, keyname, value, comment, linenumber );
}

_bool CfgFile::ReadKey( StringPtr keyname, _short& value, String* comment, _dword* linenumber ) const
{
	return ReadKey< Short, _short >( _TYPE_INTEGER, keyname, value, comment, linenumber );
}

_bool CfgFile::ReadKey( StringPtr keyname, _word& value, String* comment, _dword* linenumber ) const
{
	return ReadKey< Short, _word >( _TYPE_INTEGER, keyname, value, comment, linenumber );
}

_bool CfgFile::ReadKey( StringPtr keyname, _long& value, String* comment, _dword* linenumber ) const
{
	return ReadKey< Long, _long >( _TYPE_INTEGER, keyname, value, comment, linenumber );
}

_bool CfgFile::ReadKey( StringPtr keyname, _dword& value, String* comment, _dword* linenumber ) const
{
	return ReadKey< Long, _dword >( _TYPE_INTEGER, keyname, value, comment, linenumber );
}

_bool CfgFile::ReadKey( StringPtr keyname, _float& value, String* comment, _dword* linenumber ) const
{
	return ReadKey< Float, _float >( _TYPE_FLOAT, keyname, value, comment, linenumber );
}

_bool CfgFile::ReadKey( StringPtr keyname, _double& value, String* comment, _dword* linenumber ) const
{
	return ReadKey< Double, _double >( _TYPE_FLOAT, keyname, value, comment, linenumber );
}

_bool CfgFile::ReadKey( StringPtr keyname, String& value, String* comment, _dword* linenumber ) const
{
	if ( mCurrentSection == _null )
		return _false;

	// Find key section
	return ReadKey( keyname, mCurrentSection->mBufferArray, value, comment, linenumber );
}

_bool CfgFile::ReadKey( StringPtr keyname, Color& color, String* comment, _dword* linenumber ) const
{
	if ( mCurrentSection == _null )
		return _false;

	// Find key section
	String valuestring;
	if ( ReadKey( keyname, mCurrentSection->mBufferArray, valuestring, comment, linenumber ) == _false )
		return _false;

	// To lowercase
	valuestring.Lowercase( );

	// Trim prefix name and brackets
	if ( valuestring.SearchL2R( L"rgba" ) != -1 ) // RGBA 
		valuestring.TrimLeft( L"rgba" );
	else if ( valuestring.SearchL2R( L"rgb" ) != -1 ) // RGB
		valuestring.TrimLeft( L"rgb" );	
	else
		return _false; // Style wrong

	valuestring.TrimBoth( L"()" );

	// Copy value string 
	_char valuebuffer[ 32 ];
	StringFormatter::CopyString( valuebuffer, valuestring, 31 );

	// Get RGBA data
	_char* colortable[4];
	_dword colornumber = StringFormatter::FormatCommandLine( valuebuffer, colortable, 4, L"," );
	if ( colornumber < 3 )
		return _false;

	// Set RGB color
	color.r = Float( colortable[0] ) / 255.0f;
	color.g = Float( colortable[1] ) / 255.0f;
	color.b = Float( colortable[2] ) / 255.0f;

	// Set alpha color
	if ( colornumber > 3 )
		color.a = Float( colortable[3] ) / 255.0f; 
	else
		color.a = 1.0f;

	return _true;
}

_bool CfgFile::ReadKey( StringPtr keyname, _long& minvalue, _long& maxvalue, String* comment, _dword* linenumber ) const
{
	return ReadKey< Long, _long >( _TYPE_INTEGER, keyname, minvalue, maxvalue, comment, linenumber );
}

_bool CfgFile::ReadKey( StringPtr keyname, _dword& minvalue, _dword& maxvalue, String* comment, _dword* linenumber ) const
{
	return ReadKey< Long, _dword >( _TYPE_INTEGER, keyname, minvalue, maxvalue, comment, linenumber );
}

_bool CfgFile::ReadKey( StringPtr keyname, _float& minvalue, _float& maxvalue, String* comment, _dword* linenumber ) const
{
	return ReadKey< Float, _float >( _TYPE_FLOAT, keyname, minvalue, maxvalue, comment, linenumber );
}

_bool CfgFile::WriteKey( StringPtr keyname, _long value )
{
	return WriteKey< _long, Long >( keyname, value );
}

_bool CfgFile::WriteKey( StringPtr keyname, _float value )
{
	return WriteKey< _float, Float >( keyname, value );
}

_bool CfgFile::WriteKey( StringPtr keyname, _double value )
{
	return WriteKey< _double, Double >( keyname, value );
}

_bool CfgFile::WriteKey( StringPtr keyname, StringPtr value )
{
	// Check current section
	if ( mCurrentSection == _null )
		return _false;

	// Search buffer data
	BufferData* bufferdata = SearchBufferData( mCurrentSection, keyname );
	if ( bufferdata == _null )
		return _false;

	// Update content
	bufferdata->mContent = value;

	return _true;
}

_bool CfgFile::WriteKey( const Array< StringPtr >& stringarray )
{
	return WriteKey< Array< StringPtr > >( stringarray );
}

_bool CfgFile::WriteKey( const Array< String >& stringarray )
{
	return WriteKey< Array< String > >( stringarray );
}

_dword CfgFile::CreateSection( StringPtr name, StringPtr defname, _dword parentsectionid )
{
	// Locate at parent section
	SectionData* parentsection = _null;
	if ( parentsectionid == -1 )
		parentsection = mSectionDataArray[0];
	else
		parentsection = SearchSection( parentsectionid );

	if ( parentsection == _null )
		return -1;

	// Create section data
	SectionData* section	= new SectionData( );
	section->mID			= mSectionDataArray.Number( );
	section->mFatherID		= parentsection->mID;
	section->mLevel			= parentsection->mLevel + 1;
	section->mLineNumber	= 0;
	section->mUserData1		= 0;
	section->mUserData2		= 0;
	section->mDefName		= defname;
	section->mName			= name;
	section->mName.Lowercase( );

	// Add into section array
	mSectionDataArray.Append( section );

	return section->mID;
}

_bool CfgFile::CreateKey( _dword sectionid, StringPtr name, StringPtr value )
{
	// Search section
	SectionData* section = SearchSection( sectionid );
	if ( section == _null )
		return _false;

	// Create key in section
	BufferData bufferdata;
	bufferdata.mLineNumber	= 0;
	bufferdata.mContent		= value;
	bufferdata.mKeyName		= name;
	bufferdata.mKeyName.Lowercase( );

	// Append it into array
	section->mBufferArray.Append( bufferdata );

	return _true;
}

_bool CfgFile::SetReadKeyCallback( StringPtr keyname, OnReadKeyName funcpointer, _dword sectionid )
{
	// Get section
	const SectionData* section = GetSection( sectionid );
	if ( section == _null )
		return _false;

	return SetReadKeyCallback( section, keyname, funcpointer );
}

_void CfgFile::RemoveReadKeyCallback( StringPtr keyname, _dword sectionid )
{
	_char lowercasedkeyname[ 1024 ];
	StringFormatter::CopyString( lowercasedkeyname, keyname, 1023 );
	StringFormatter::LowercaseString( lowercasedkeyname );

	// Get section
	const SectionData* section = GetSection( sectionid );
	if ( section == _null )
		return;

	// Search section callback block
	ReadKeyCallbackData* data = (ReadKeyCallbackData*) SearchReadKeyCallbackData( section->mID );
	if ( data == _null )
		return;

	// Search keyname callback block
	KeyNameDataArray::Iterator it = data->mKeyNameDataArray.SearchAscending( lowercasedkeyname );
	if ( it.Valid( ) == _false )
		return;

	// Remove it
	data->mKeyNameDataArray.Remove( it );
}

_void CfgFile::RemoveAllReadKeyCallback( _dword sectionid )
{
	// Remove all read key callback functions data
	if ( sectionid == -1 )
	{
		mReadKeyCallbackDataArray.Clear( );

		return;
	}

	// Search section callback block
	ReadKeyCallbackData* data = (ReadKeyCallbackData*) SearchReadKeyCallbackData( sectionid );
	if ( data == _null )
		return;

	// Remove all keyname callback 
	data->mKeyNameDataArray.Clear( );
}

_bool CfgFile::BuildSection( SectionData* section, _dword level )
{
	_bool incommentsection = _false;

	String string1, string2;
	String keyname, commentstring;

	// Read section buffer data
	while ( ReachEnd( ) == _false )
	{
		// Read the first string
		if ( string2 != L"" && string2 != L"{" )
		{
			// The next line string has been loaded so use string2 immediately
			string1 = string2;
		}
		else
		{
			// Read next line string from file
			ReadLine( string1 ); string1.TrimBoth( L" \t\"" );
		}

		if ( string1 == L"" )
			continue;

		// Get string length
		_dword length = string1.Length( );

		// Is comment line 
		if ( length >= 2 && string1[0] == '/' && string1[1] == '/' )
		{
			string2 = L"";
			continue;
		}

		// Is comment section start tag ?
		if ( incommentsection == _false && length >= 2 && string1[0] == '/' && string1[1] == '*' )
			incommentsection = _true;

		// Do not read anything when in the comment section
		if ( incommentsection == _true )
		{
			// Is comment section end tag ?
			if ( length >= 2 && string1[ length - 2 ] == '*' && string1[ length - 1 ] == '/' )
				incommentsection = _false;

			string2 = L"";
			continue;
		}

		// If string1 equal to '}' character then analyst finished
		if ( string1 == L"}" )
			break;

		// Read the second string
		ReadLine( string2 ); string2.TrimBoth( L" \t\"" );

		// Get line number 
		_dword linenumber = GetLineNumber( );

		// Is sub section start tag
		if ( string2 == L"{" )
		{
			// Build sub section ( Decrease the '{' of calculating line number )
			SectionData* subsection = CreateSection( string1, section->mID, level, linenumber - 1 );
			if ( subsection == _null )
				return _false;

			// Recall sub section
			if ( BuildSection( subsection, level + 1 ) == _false )
				return _false;

			continue;
		}
		
		// Is [ keyname = value ] style 
		_dword index = string1.SearchL2R( '=' );
		if ( index != -1 )
		{
			// Get keyname
			keyname.CopyString( string1, index );
			keyname.TrimRight( L" \t" );
			keyname.Lowercase( );

			// Update content ( remove keyname )
			string1.Remove( 0, index + 1 );
			string1.TrimLeft( L" \t" );
		}

		// Get comment
		_dword commentindex = string1.SearchR2L( L"//" );
		if ( commentindex != -1 )
		{
			// Get comment string
			commentstring = StringPtr( string1 ).SubString( commentindex );
			commentstring.TrimLeft( L" \t//" );

			// Update content ( remove comment string )
			string1.Remove( commentindex, string1.Length( ) - commentindex );
			string1.TrimRight( L" \t" );
		}

		// Trim some unused character
		string1.TrimBoth( L" \t\"" );

		// Get content string length
		length = string1.Length( );

		// Cleanup content string
		if ( length >= 4 )
		{
			// Trim '[[' and ']]'
			if ( string1[0] == '[' && string1[1] == '[' && string1[ length - 1 ] == ']' && string1[ length - 2 ] == ']' )
			{
				string1.Remove( length - 2, 2 );
				string1.Remove( 0, 2 );
			}
		}

		// Create buffer data
		BufferData bufferdata;
		bufferdata.mLineNumber	= linenumber;
		bufferdata.mKeyName		= keyname;
		bufferdata.mContent 	= string1;
		bufferdata.mComment 	= commentstring;

		// Append the first buffer data into array
		section->mBufferArray.Append( bufferdata );
	}

	return _true;
}

CfgFile::SectionData* CfgFile::CreateSection( const String& string, _dword fatherid, _dword level, _dword linenumber )
{
	String sectionname, defname;

	// Read section name and define name
	_dword index = string.SearchL2R( ':' );
	if ( index != -1 )
	{
		// Section has section name and define name

		// Read section name
		sectionname.CopyString( string, index );
		sectionname.TrimRight( L" \t" );

		// Read def name ( jump ':' symbol )
		defname	= StringPtr( string ).SubString( index + 1 );

		// Remove command string
		_dword commandindex = defname.SearchL2R( L"//" );
		if ( commandindex != -1 )
			defname.Remove( commandindex, defname.Length( ) - commandindex );

		defname.TrimLeft( L" \t" );
	}
	else
	{
		// Section has section name only
		sectionname = string;
	}

	// Create the new section
	SectionData* section	= new SectionData;
	section->mID			= mSectionDataArray.Number( );
	section->mFatherID		= fatherid;
	section->mLevel			= level;
	section->mLineNumber	= linenumber;
	section->mUserData1		= 0;
	section->mUserData2		= 0;
	section->mName			= sectionname.Lowercase( );
	section->mDefName		= defname;

	// Add into section array
	mSectionDataArray.Append( section );

	return section;
}

CfgFile::SectionData* CfgFile::GetSection( _dword sectionid ) const
{
	if ( sectionid == -1 )
		return mCurrentSection;

	return SearchSection( sectionid );
}

CfgFile::SectionData* CfgFile::SearchSection( _dword sectionid ) const
{
	// Check section bounds
	if ( sectionid >= mSectionDataArray.Number( ) )
		return _null;

	return mSectionDataArray[ sectionid ];
}

_bool CfgFile::ReadKey( StringPtr keyname, const BufferDataArray& bufferarray, String& valuestring, String* comment, _dword* linenumber ) const
{
	// Copy key name 
	_char keynamebuffer[ 256 ];
	StringFormatter::CopyString( keynamebuffer, keyname, 255 );
	StringFormatter::LowercaseString( keynamebuffer );

	// Split key name by '|' symbol
	_char* keynametable[ 32 ];
	_dword keynamenumber = StringFormatter::FormatCommandLine( keynamebuffer, keynametable, 32, L"|" );

	// Search key name
	for ( _dword i = 0; i < bufferarray.Number( ); i ++ )
	{
		const BufferData& bufferdata = bufferarray[i];

		// Compare key name
		for ( _dword j = 0; j < keynamenumber; j ++ )
		{
			if ( bufferdata.mKeyName == keynametable[j] )
			{
				// Get value string
				valuestring = bufferdata.mContent;

				// Get comment
				if ( comment != _null )
					*comment = bufferdata.mComment;

				// Feedback line number
				if ( linenumber != _null )
					*linenumber = bufferdata.mLineNumber;

				// Read value 
				String resultstring;
				if ( ReadKeyValue( mCurrentSection, keyname, valuestring, resultstring ) == _true )
					valuestring = resultstring;

				return _true;
			}
		}
	}

	// Does need to call error function pointer ?
	if ( mErrorFunc != _null && IsEnableErrorCallback( ) )
		(*mErrorFunc)( this, keyname, L"doesn't has this keyname", GetFileName( ) );

	return _false;
}

const CfgFile::ReadKeyCallbackData* CfgFile::SearchReadKeyCallbackData( _dword sectionid ) const
{
	// Search section callback block
	ReadKeyCallbackDataArray::Iterator it = mReadKeyCallbackDataArray.SearchAscending( sectionid );
	if ( it.Valid( ) == _false )
		return _null;

	return &( ReadKeyCallbackData& ) it;
}

const CfgFile::KeyNameData* CfgFile::SearchKeyNameData( _dword sectionid, StringPtr keyname ) const
{
	_char lowercasedkeyname[ 1024 ];
	StringFormatter::CopyString( lowercasedkeyname, keyname, 1023 );
	StringFormatter::LowercaseString( lowercasedkeyname );

	// Search section callback block
	const ReadKeyCallbackData* data = SearchReadKeyCallbackData( sectionid );
	if ( data == _null )
		return _null;

	// Search keyname callback block
	KeyNameDataArray::Iterator it = data->mKeyNameDataArray.SearchAscending( lowercasedkeyname );
	if ( it.Valid( ) == _false )
		return _null;

	return &( KeyNameData& ) it;
}

CfgFile::BufferData* CfgFile::SearchBufferData( SectionData* sectiondata, StringPtr keyname )
{
	FL_ASSERT( sectiondata != _null );

	_char lowercasedkeyname[ 1024 ];
	StringFormatter::CopyString( lowercasedkeyname, keyname, 1023 );
	StringFormatter::LowercaseString( lowercasedkeyname );

	// Search key name
	for ( _dword i = 0; i < sectiondata->mBufferArray.Number( ); i ++ )
	{
		BufferData& bufferdata = sectiondata->mBufferArray[i];
		
		// Compare key name
		if ( bufferdata.mKeyName != lowercasedkeyname )
			continue;

		return &bufferdata;
	}

	return _null;
}

_bool CfgFile::SetReadKeyCallback( const SectionData* section, StringPtr keyname, OnReadKeyName funcpointer )
{
	// Search section callback block
	ReadKeyCallbackData* readkeydata = (ReadKeyCallbackData*) SearchReadKeyCallbackData( section->mID );
	if ( readkeydata == _null )
	{
		// Create the read key callback data
		ReadKeyCallbackData callbackdata;
		callbackdata.mSectionData = (SectionData*) section;

		// Insert it into read key callback data array
		_dword index = mReadKeyCallbackDataArray.InsertAscending( callbackdata );

		// Get the read key data
		readkeydata = &mReadKeyCallbackDataArray[ index ];
	}

	// The string buffer what to store lowecased key name string
	_char stringbuffer[1024];

	// Initialize keyname buffer
	_char* keynametable[32]; 
	_dword number = 0;

	// Does need to lock all keys ?
	if ( keyname == L"" )
	{
		// Lock all keys
		number = GetSectionKeyName( section, keynametable );
	}
	else
	{
		// Convert to lowercase 
		StringFormatter::CopyString( stringbuffer, keyname, 1023 );
		StringFormatter::LowercaseString( stringbuffer );

		// Split key name by ',' symbol
		number = StringFormatter::FormatCommandLine( stringbuffer, keynametable, 32, L"," );

		// Try to match key name in buffer data
		for ( _dword i = 0; i < number; i ++ )
			MatchKeyName( section, keynametable[i] );
	}

	// Set key name callback function
	for ( _dword i = 0; i < number; i ++ )
	{
		if ( keynametable[i] == _null )
			continue;

		// Search for read key data by keyname
		if ( SearchKeyNameData( section->mID, keynametable[i] ) != _null )
			continue;

		// Create key name data
		KeyNameData keynamedata;
		keynamedata.mKeyName			= keynametable[i]; // keynametable[i]'s address is the same as BufferData::mKeyName
		keynamedata.mReadKeyNameFunc	= funcpointer;

		// Insert key data into array
		readkeydata->mKeyNameDataArray.InsertAscending( keynamedata );
	}

	return _true;
}

_bool CfgFile::CoreReadSectionString( const SectionData* section, String& stringbuffer ) const
{
	String newcontent;

	for ( _dword i = 0; i < section->mBufferArray.Number( ); i ++ )
	{
		StringPtr content = section->mBufferArray[i].mContent;

		// Feedback section buffer from callback function
		if ( mReadSectionStringFunc != _null && IsEnableReadSectionStringCallback( ) )
		{
			newcontent = (*mReadSectionStringFunc)( this, i, section->mFatherID, section->mName, section->mDefName, content, GetFileName( ) );
			if ( newcontent != L"" )
			{
				stringbuffer += newcontent;
				stringbuffer += L"\r\n";

				continue;
			}
		}

		// Feedback section buffer from config file
		stringbuffer += content;
		stringbuffer += L"\r\n";
	}

	return _true;
}

_dword CfgFile::GetSectionKeyName( const SectionData* section, _char* keynametable[] ) const
{
	for ( _dword i = 0; i < section->mBufferArray.Number( ); i ++ )
		keynametable[i] = (_char*) (const _char*) section->mBufferArray[i].mKeyName;

	return section->mBufferArray.Number( );
}

_void CfgFile::MatchKeyName( const SectionData* section, _char*& keyname ) const
{
	for ( _dword i = 0; i < section->mBufferArray.Number( ); i ++ )
	{
		const BufferData& bufferdata = section->mBufferArray[i];

		if ( bufferdata.mKeyName == keyname )
		{
			keyname = (_char*) (const _char*) bufferdata.mKeyName;

			return;
		}
	}

	// Keyname is not existing in buffer data
	keyname = _null;
}

_bool CfgFile::CoreReadSectionString( const SectionData* section, Array< String >& stringarray ) const
{
	String newcontent;

	for ( _dword i = 0; i < section->mBufferArray.Number( ); i ++ )
	{
		StringPtr content = section->mBufferArray[i].mContent;

		// Feedback section buffer from callback function
		if ( mReadSectionStringFunc != _null && IsEnableReadSectionStringCallback( ) )
		{
			newcontent = (*mReadSectionStringFunc)( this, i, section->mFatherID, section->mName, section->mDefName, content, GetFileName( ) );
			if ( newcontent != L"" )
			{
				stringarray.Append( newcontent );

				continue;
			}
		}

		// Feedback section buffer from config file
		stringarray.Append( ( const _char* ) content );
	}

	return _true;
}

_bool CfgFile::CoreReadSectionString( const SectionData* section, Array< String >* stringarray, Array< String >* commentarray, Array< String >* keynamearray ) const
{
	// Only output content without comment and keyname
	if ( stringarray != _null && commentarray == _null && keynamearray == _null )
	{
		for ( _dword i = 0; i < section->mBufferArray.Number( ); i ++ )
			stringarray->Append( section->mBufferArray[i].mContent );

		return _true;
	}

	String contentstring;

	// Output content with comment or keyname
	for ( _dword i = 0; i < section->mBufferArray.Number( ); i ++ )
	{
		const BufferData& bufferdata = section->mBufferArray[i];

		// Get content string
		contentstring = bufferdata.mContent;

		// Append comment string
		if ( commentarray != _null )
			commentarray->Append( bufferdata.mComment );

		// Append keyname
		if ( keynamearray != _null )
			keynamearray->Append( bufferdata.mKeyName );

		// Feedback section buffer from callback function
		if ( mReadSectionStringFunc != _null && IsEnableReadSectionStringCallback( ) )
		{
			String newcontent = (*mReadSectionStringFunc)( this, i, section->mFatherID, section->mName, section->mDefName, contentstring, GetFileName( ) );
			if ( newcontent != L"" )
				contentstring = newcontent;
		}

		// Feedback value from callback function
		if ( stringarray != _null )
		{
			// Feedback section buffer from config file
			String resultstring;
			if ( ReadKeyValue( section, bufferdata.mKeyName, contentstring, resultstring ) == _true )
				stringarray->Append( resultstring );
			else
				stringarray->Append( contentstring );
		}
	}

	return _true;
}

_bool CfgFile::ReadKeyValue( const SectionData* section, StringPtr keyname, StringPtr valuestring, String& resultstring ) const
{
	const KeyNameData* keynamedata = SearchKeyNameData( section->mID, keyname );
	if ( keynamedata != _null && keynamedata->mReadKeyNameFunc != _null )
	{
		resultstring = (*keynamedata->mReadKeyNameFunc)( this, section->mFatherID, section->mName, section->mDefName, keyname, valuestring, GetFileName( ) );
		if ( resultstring != L"" )
			return _true; // use result string
	}

	return _false; // use value string
}

template < typename Type1, typename Type2 >
_bool CfgFile::ReadKey( _dword type, StringPtr keyname, Type2& value, String* comment, _dword* linenumber ) const
{
	if ( mCurrentSection == _null )
		return _false;

	// Find key section
	String valuestring;
	if ( ReadKey( keyname, mCurrentSection->mBufferArray, valuestring, comment, linenumber ) == _false )
		return _false;

	// Does need to analyse boolean value style ?
	if ( type == _TYPE_INTEGER )
	{
		// Convert to lowercase
		valuestring.Lowercase( );

		// True
		if ( valuestring == L"true" )
		{
			value = _true;

			return _true;
		}
		// False
		else if ( valuestring == L"false" )
		{
			value = _false;

			return _true;
		}
	}

	// Analyse value string
	if ( valuestring != L"" )
	{
		// Get the last character
		_char lastcharacter = valuestring[ valuestring.Length( ) - 1 ];

		// Is the percent style string ?
		if ( lastcharacter == '%' )
		{
			// Remove '%' (percent) character
			valuestring.TrimRight( '%' );

			// Get value
			value = Type1( valuestring );

			return _true;
		}
		// Example : 100.0 ms, 99 s
		else if ( valuestring.Length( ) > 3 && valuestring[ valuestring.Length( ) - 3 ] == ' ' )
		{
			// Check symbol
			if ( lastcharacter == 's' )
			{
				if ( valuestring[ valuestring.Length( ) - 2 ] == 'm' )
				{
					// Remove 'ms' (millisecond) symbol
					valuestring.TrimRight( L"ms" );

					// Get value
					value = Type1( valuestring );

					return _true;
				}
				else
				{
					// Remove 's' (second) symbol
					valuestring.TrimRight( 's' );

					// Get value
					value = Type1( valuestring );

					return _true;
				}
			}
		}
	}

	// Convert value to specific type
	value = Type1( valuestring );

	return _true;
}

template < typename Type1, typename Type2 >
_bool CfgFile::ReadKey( _dword type, StringPtr keyname, Type2& minvalue, Type2& maxvalue, String* comment, _dword* linenumber ) const
{
	if ( mCurrentSection == _null )
		return _false;

	// Find key section
	String valuestring;
	if ( ReadKey( keyname, mCurrentSection->mBufferArray, valuestring, comment, linenumber ) == _false )
		return _false;

	// Check value string length
	if ( valuestring == L"" )
	{
		// no value string inforamtion
		maxvalue = 0;
		minvalue = 0;

		return _true;
	}

	// Find value range
	_dword index = valuestring.SearchL2R( '~' );

	// Check value range style
	if ( index == -1 )
	{
		// No max value information
		// Exp : 10
		minvalue = Type1( valuestring );
		maxvalue = 0;
	}
	else if ( index == 0 )
	{
		// No min value information
		// Exp : ~10
		minvalue = 0;
		maxvalue = Type1( valuestring );
	}
	else if ( index == valuestring.Length( ) - 1 )
	{
		// Remove '~' symbol
		valuestring.TrimRight( '~' );
		if ( valuestring == L"" )
		{
			// Only has '~' symbol, doesn't have min and max value at all
			// Exp : ~
			maxvalue = 0;
			minvalue = 0;
		}
		else
		{
			// No max value information
			// Exp : 10~
			minvalue = 0;
			maxvalue = Type1( valuestring );
		}
	}
	else
	{
		// Get min and max value information
		minvalue = Type1( valuestring.SubString( 0, index ) );
		maxvalue = Type1( StringPtr( valuestring ).SubString( index + 1 ) );
	}

	return _true;
}

template < typename Type1, typename Type2 >
_bool CfgFile::WriteKey( StringPtr keyname, Type1 value )
{
	// Check current section
	if ( mCurrentSection == _null )
		return _false;

	// Search buffer data
	BufferData* bufferdata = SearchBufferData( mCurrentSection, keyname );
	if ( bufferdata == _null )
		return _false;

	// Update content
	bufferdata->mContent = Type2( value ).ToString( );

	return _true;
}

template < typename Type >
_bool CfgFile::WriteKey( const Type& stringarray )
{
	// Check current section
	if ( mCurrentSection == _null )
		return _false;

	// Clear buffer data
	mCurrentSection->mBufferArray.Clear( );

	// Update buffer data ( The line number info will lost )
	for ( _dword i = 0; i < stringarray.Number( ); i ++ )
	{
		BufferData bufferdata;
		bufferdata.mContent = stringarray[i];

		mCurrentSection->mBufferArray.Append( bufferdata );
	}

	return _true;
}

_void CfgFile::WriteTabs( File& file, _dword number ) const
{
	if ( number == 0 )
		return;

	while ( -- number > 0 )
		file.WriteString( L"\t" );
}

_void CfgFile::WriteLeftBracket( File& file, const SectionData* section ) const
{
	WriteTabs( file, section->mLevel ); file.WriteString( L"{" ); 

	file.WriteEndline( );
}

_void CfgFile::WriteRightBracket( File& file, const SectionData* section ) const
{
	WriteTabs( file, section->mLevel ); file.WriteString( L"}" ); 

	file.WriteEndline( );
	file.WriteEndline( );
}

_void CfgFile::WriteSectionName( File& file, const SectionData* section ) const
{
	// Write section name
	WriteTabs( file, section->mLevel ); file.WriteString( section->mName );

	// Write define name
	if ( section->mDefName != L"" )
	{
		file.WriteString( L" : " );
		file.WriteString( section->mDefName );
	}

	file.WriteEndline( );
}

_bool CfgFile::Save( File& file, const RootData& rootdata, const RootDataArray& rootdataarray ) const
{
	// Get parent section
	const SectionData* section = rootdata.mSectionData;

	// Write section name
	WriteSectionName( file, section );

	// Write '{'
	WriteLeftBracket( file, section );

	// Write buffer data
	for ( _dword j = 0; j < section->mBufferArray.Number( ); j ++ )
		WriteBufferData( file, section->mBufferArray[j], section );

	// Write child section
	for ( _dword i = 0; i < rootdata.mChildIDArray.Number( ); i ++ )
	{
		// Get child section
		const SectionData* childsection = mSectionDataArray[ rootdata.mChildIDArray[i] ];

		// Get child root data
		const RootData& childrootdata = rootdataarray[ childsection->mID ];

		// Save section recursive
		if ( Save( file, childrootdata, rootdataarray ) == _false )
			return _false;
	}

	// Write '}'
	WriteRightBracket( file, section );

	return _true;
}

_void CfgFile::WriteBufferData( File& file, const BufferData& bufferdata, const SectionData* section ) const
{
	WriteTabs( file, section->mLevel + 1 );

	if ( bufferdata.mKeyName == L"" )
	{
		// Write value
		file.WriteString( bufferdata.mContent );
	}
	else
	{
		// Write key name
		file.WriteString( bufferdata.mKeyName );

		// Write '='
		file.WriteString( L" = " );

		// Write value
		file.WriteString( bufferdata.mContent );
	}

	file.WriteEndline( );
}

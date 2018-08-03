//============================================================================
//Camera.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// IniFile Implementation
//----------------------------------------------------------------------------

IniFile::IniFile( )
{
	mSortFlag					= _false;

	mCurrentSectionDataIndex	= -1;

	mCurrentBufferDataIndex		= -1;
	mBackupBufferDataIndex		= -1;

	// Clear division
	SetDivision( L"" );
}

IniFile::~IniFile( )
{
	Finalize( );
}

_bool IniFile::Initialize( _bool skipemptysection, _bool sort )
{
	Finalize( );

	String 		string;
	String 		keyname;
	SectionData sectiondata;

	// Start to analyst *.ini file
	while ( ReachEnd( ) == _false )
	{
		// Read string line
		ReadLine( string ); 

		// Skip empty line
		string.TrimBoth( L" \t" );
		if ( string == L"" )
			continue;

		// Get string length
		_dword length = string.Length( );

		// Check if it's a section string
		if ( string[0] == '[' || string[length - 1] == ']' )
		{
			string.TrimBoth( L" \t[]" );

			// Insert the section data into array
			if ( skipemptysection == _false || sectiondata.mBufferDataArray.Number( ) > 0 )
				mSectionDataArray.Append( sectiondata );

			// Initialize section data
			sectiondata.mName = string.Lowercase( );
			sectiondata.mBufferDataArray.Clear( );

			continue;
		}

		// Search for sperator character of key name
		_dword index = string.SearchL2R( ':' );
		if ( index == -1 )
			index = string.SearchL2R( '=' );

		// Get key name
		if ( index != - 1 )
		{
			// Get key name
			keyname.CopyString( string, index );
			keyname.TrimRight( L" \t" );

			// Key name can't contain any space and table character
			if ( keyname.SearchL2R( ' ' ) == -1 && keyname.SearchL2R( '\t' ) == -1 )
			{
				// Trim key name and sperator character
				string.Remove( 0, index + 1 );
				string.TrimBoth( L" \t\"" );
			}
		}
		else
		{
			// Clear key name
			keyname = L"";
		}

		// Create buffer data
		BufferData bufferdata;
		bufferdata.mKeyName = keyname.Lowercase( );
		bufferdata.mContent	= string;

		// Append it into array
		sectiondata.mBufferDataArray.Append( bufferdata );
	}

	// Insert the last section data into array
	if ( skipemptysection == _false || sectiondata.mBufferDataArray.Number( ) > 0 )
		mSectionDataArray.Append( sectiondata );

	// Sort buffer data by key name
	if ( sort )
	{
		// Update sort flag
		mSortFlag = sort;

		// Sort buffer data of each section
		for ( _dword i = 0; i < mSectionDataArray.Number( ); i ++ )
			mSectionDataArray[i].mBufferDataArray.SortAscending( );
	}

	return _true;
}

_void IniFile::Finalize( )
{
	mPointer					= mBuffer;
	mBackup						= _null;

	mLineNumber					= 1;
	mLinePointerBackup			= _null;

	mSortFlag					= _false;

	mCurrentSectionDataIndex	= -1;

	mCurrentBufferDataIndex		= -1;
	mBackupBufferDataIndex		= -1;

	mIndexDataStack.Clear( );
	mSectionDataArray.Clear( );
}

_bool IniFile::SetCurrentSectionByName( StringPtr sectionname )
{
	_char lowercasedname[ 1024 ];
	StringFormatter::CopyString( lowercasedname, sectionname, 1023 );
	StringFormatter::LowercaseString( lowercasedname );

	SectionDataArray::Iterator it = mSectionDataArray.Search( lowercasedname );
	if ( it.Valid( ) == _false )
		return _null;

	mCurrentSectionDataIndex	= it.mIndex;

	mCurrentBufferDataIndex		= ( (const SectionData&) it ).mBufferDataArray.Number( ) > 0 ? 0 : -1;
	mBackupBufferDataIndex		= mCurrentBufferDataIndex;

	return _true;
}

_bool IniFile::SetCurrentSectionByIndex( _dword index )
{
	if ( index >= mSectionDataArray.Number( ) )
		return _false;

	mCurrentSectionDataIndex	= index;

	mCurrentBufferDataIndex		= mSectionDataArray[ index ].mBufferDataArray.Number( ) > 0 ? 0 : -1;
	mBackupBufferDataIndex		= mCurrentBufferDataIndex;

	return _true;
}

_void IniFile::PushCurrentSection( )
{
	if ( mCurrentSectionDataIndex == -1 )
		return;

	mIndexDataStack.Push( IndexData( mCurrentSectionDataIndex, mCurrentBufferDataIndex ) );
}

_void IniFile::PopCurrentSection( )
{
	if ( mIndexDataStack.Number( ) == 0 )
		return;

	mCurrentSectionDataIndex	= mIndexDataStack.Top( ).mSectionDataIndex;

	mCurrentBufferDataIndex		= mIndexDataStack.Top( ).mBufferDataIndex;
	mBackupBufferDataIndex		= mCurrentBufferDataIndex;

	mIndexDataStack.Pop( );
}

_bool IniFile::Parse( )
{
	if ( mCurrentBufferDataIndex == -1 || mCurrentSectionDataIndex == -1 )
		return _false;

	// Backup buffer data index
	mBackupBufferDataIndex = mCurrentBufferDataIndex;

	// Get current section data
	const SectionData& sectiondata = mSectionDataArray[ mCurrentSectionDataIndex ];

	// Update current buffer data index
	mCurrentBufferDataIndex ++;
	if ( mCurrentBufferDataIndex >= sectiondata.mBufferDataArray.Number( ) )
		mCurrentBufferDataIndex = -1;

	return _true;
}

StringPtr IniFile::GetKeyName( ) const
{
	if ( mBackupBufferDataIndex == -1 || mCurrentSectionDataIndex == -1 )
		return L"";

	// Get current section data
	const SectionData& sectiondata = mSectionDataArray[ mCurrentSectionDataIndex ];

	// Get key name
	return sectiondata.mBufferDataArray[ mBackupBufferDataIndex ].mKeyName;
}

StringPtr IniFile::GetValueString( ) const
{
	if ( mBackupBufferDataIndex == -1 || mCurrentSectionDataIndex == -1 )
		return L"";

	// Get current section data
	const SectionData& sectiondata = mSectionDataArray[ mCurrentSectionDataIndex ];

	// Get value string
	return sectiondata.mBufferDataArray[ mBackupBufferDataIndex ].mContent;
}

_dword IniFile::GetValueDword( ) const
{
	if ( mBackupBufferDataIndex == -1 || mCurrentSectionDataIndex == -1 )
		return 0;

	// Get current section data
	const SectionData& sectiondata = mSectionDataArray[ mCurrentSectionDataIndex ];

	// Get value
	return Long( sectiondata.mBufferDataArray[ mBackupBufferDataIndex ].mContent );
}

_float IniFile::GetValueFloat( ) const
{
	if ( mBackupBufferDataIndex == -1 || mCurrentSectionDataIndex == -1 )
		return 0.0f;

	// Get current section data
	const SectionData& sectiondata = mSectionDataArray[ mCurrentSectionDataIndex ];

	// Get value
	return Float( sectiondata.mBufferDataArray[ mBackupBufferDataIndex ].mContent );
}

_double IniFile::GetValueDouble( ) const
{
	if ( mBackupBufferDataIndex == -1 || mCurrentSectionDataIndex == -1 )
		return 0.0;

	// Get current section data
	const SectionData& sectiondata = mSectionDataArray[ mCurrentSectionDataIndex ];

	// Get value
	return Double( sectiondata.mBufferDataArray[ mBackupBufferDataIndex ].mContent );
}

_dword IniFile::GetElementNumber( ) const
{
	if ( mCurrentSectionDataIndex == -1 )
		return 0;

	return mSectionDataArray[ mCurrentSectionDataIndex ].mBufferDataArray.Number( );
}

StringPtr IniFile::GetElementString( _dword index, StringPtr* keyname ) const
{
	if ( mCurrentSectionDataIndex == -1 )
		return L"";

	if ( index >= mSectionDataArray[ mCurrentSectionDataIndex ].mBufferDataArray.Number( ) )
		return L"";

	const BufferData& bufferdata = mSectionDataArray[ mCurrentSectionDataIndex ].mBufferDataArray[ index ];

	// Feedback key name
	if ( keyname != _null )
		*keyname = bufferdata.mKeyName;

	return bufferdata.mContent;
}

_dword IniFile::GetElementString( _dword index, _char* stringbuffer, _dword number, _char* stringtable[], _dword stringtablenumber, StringPtr delimiter, StringPtr* keyname ) const
{
	StringPtr string = GetElementString( index, keyname );
	if ( string == L"" )
		return 0;

	StringFormatter::CopyString( stringbuffer, string, number - 1 );

	return StringFormatter::FormatCommandLine( stringbuffer, stringtable, stringtablenumber, delimiter );
}

StringPtr IniFile::ReadKey( StringPtr keyname ) const
{
	if ( keyname == L"" || mCurrentSectionDataIndex == -1 )
		return L"";

	return GetBufferDataContent( keyname, &mSectionDataArray[ mCurrentSectionDataIndex ] );
}

StringPtr IniFile::ReadKey( StringPtr keyname, StringPtr sectionname ) const
{
	if ( keyname == L"" )
		return L"";

	// Get section data
	const SectionData* sectiondata = SearchSection( sectionname );
	if ( sectiondata == _null )
		return L"";

	return GetBufferDataContent( keyname, sectiondata );
}

const IniFile::SectionData* IniFile::SearchSection( StringPtr sectionname ) const
{
	_char lowercasedname[ 1024 ];
	StringFormatter::CopyString( lowercasedname, sectionname, 1023 );
	StringFormatter::LowercaseString( lowercasedname );

	SectionDataArray::Iterator it = mSectionDataArray.Search( lowercasedname );
	if ( it.Valid( ) == _false )
		return _null;

	return &( (const SectionData&) it );
}

StringPtr IniFile::GetBufferDataContent( StringPtr keyname, const SectionData* sectiondata ) const
{
	// Conver keyname to lowercase
	_char lowercasedkeyname[ 1024 ];
	StringFormatter::CopyString( lowercasedkeyname, keyname, 1023 );
	StringFormatter::LowercaseString( lowercasedkeyname );

	// Search element by key name ( Sorted )
	if ( mSortFlag )
	{
		BufferDataArray::Iterator it = sectiondata->mBufferDataArray.SearchAscending( lowercasedkeyname );
		if ( it.Valid( ) == _false )
			return L"";

		return ( (const BufferData&) it ).mContent;
	}

	// Search element by key name ( Unsort )
	BufferDataArray::Iterator it = sectiondata->mBufferDataArray.Search( lowercasedkeyname );
	if ( it.Valid( ) == _false )
		return L"";

	return ( (const BufferData&) it ).mContent;
}


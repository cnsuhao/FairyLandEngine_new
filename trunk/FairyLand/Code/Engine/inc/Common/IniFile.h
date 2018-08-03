//============================================================================
// IniFile.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// IniFile
//----------------------------------------------------------------------------

class IniFile : public TxtFile
{
public:
	//!	Get the number of sections.
	//!	@param		none.
	//!	@return		The number of sections.
	inline _dword GetSectionNumber( ) const;

	//!	Get the current section name.
	//!	@param		none.
	//!	@return		The current section name string.
	inline StringPtr GetCurrentSectionName( ) const;

	//!	Initialize.
	//! @param		skipemptysection	True indicates skip for the empty section.
	//! @param		sort				True indicates sort section's buffer data ascending.
	//! @return		True indicates success, false indicates failure.
	_bool Initialize( _bool skipemptysection = _false, _bool sort = _false );
	//!	Finalize.
	//! @param		none.
	//! @return		none.
	_void Finalize( );

	//!	Set the current section.
	//!	@param		sectionname			The section name.
	//! @return		True indicates success, false indicates failure.
	_bool SetCurrentSectionByName( StringPtr sectionname );
	//!	Set the current section.
	//!	@param		index				The section index.
	//! @return		True indicates success, false indicates failure.
	_bool SetCurrentSectionByIndex( _dword index );

	//!	Push the current section.
	//!	@param		none.
	//!	@return		none.
	_void PushCurrentSection( );
	//!	Pop the current section.
	//!	@param		none.
	//!	@return		none.
	_void PopCurrentSection( );

	//!	Begin to parse the current section.
	//! @param		none.
	//! @return		False indicates parse finished, true indicates still in procession.
	_bool Parse( );

	//!	Get key name after parse.
	//!	@param		none.
	//!	@return		The key name string.
	StringPtr GetKeyName( ) const;
	//!	Get value string after parse.
	//!	@param		none.
	//!	@return		The value string.
	StringPtr GetValueString( ) const;
	//!	Get value after parse.
	//!	@param		none.
	//!	@return		The value.
	_dword GetValueDword( ) const;
	//!	Get value after parse.
	//!	@param		none.
	//!	@return		The value.
	_float GetValueFloat( ) const;
	//!	Get value after parse.
	//!	@param		none.
	//!	@return		The value.
	_double GetValueDouble( ) const;

	//!	Get the number of elements in the current section data.
	//!	@param		none.
	//!	@return		The number of elements in section data.
	_dword GetElementNumber( ) const;
	//!	Get the string of element in the current section data.
	//!	@param		index				The index of element in section data.
	//!	@param		keyname				The key name of element.
	//!	@return		The content of element.
	StringPtr GetElementString( _dword index, StringPtr* keyname = _null ) const;
	//!	Get the splited string of element in the current section data.
	//!	@param		index				The index of element in section data.
	//!	@param		keyname				The key name of element.
	//!	@param		stringbuffer		The string buffer of splited content.
	//!	@param		number				The number of string buffer.
	//!	@param		stringtable			The Splited content string table.
	//!	@param		stringtablenumber	The number of string table.
	//!	@param		delimiter			The stirng of delimiter.
	//!	@return		The number of splited content string.
	_dword GetElementString( _dword index, _char* stringbuffer, _dword number, _char* stringtable[], _dword stringtablenumber, StringPtr delimiter = L" \t", StringPtr* keyname = _null ) const;

	//!	Get key value string in the current section.
	//!	@param		keyname				The key name.
	//!	@return		The key value string.
	StringPtr ReadKey( StringPtr keyname ) const;
	//!	Get key value string.
	//!	@param		keyname				The key name.
	//!	@param		sectionname			The section name.
	//!	@return		The key value string.
	StringPtr ReadKey( StringPtr keyname, StringPtr sectionname ) const;

public:
	IniFile( );
	~IniFile( );

public:
	//!	BufferData
	struct BufferData
	{
		String	mKeyName;
		String	mContent;

		operator StringPtr ( ) const
			{ return mKeyName; }

		_bool operator < ( const BufferData& bufferdata ) const
			{ return mKeyName < bufferdata.mKeyName; }

		_bool operator > ( const BufferData& bufferdata ) const
			{ return mKeyName > bufferdata.mKeyName; }
	};
	typedef Array< BufferData, StringPtr > BufferDataArray;

	//!	SectionData
	struct SectionData
	{
		String			mName;
		BufferDataArray	mBufferDataArray;

		operator StringPtr ( ) const
			{ return mName; }
	};
	typedef Array< SectionData, StringPtr > SectionDataArray;

	//! IndexData
	struct IndexData
	{
		_dword	mSectionDataIndex;
		_dword	mBufferDataIndex;

		IndexData( _dword sectiondataindex = -1, _dword bufferdataindex = -1 ) : mSectionDataIndex( sectiondataindex ), mBufferDataIndex( bufferdataindex ) { }
	};
	typedef Stack< IndexData > IndexDataStack;

private:
	_bool				mSortFlag;

	_dword				mCurrentSectionDataIndex;

	_dword				mCurrentBufferDataIndex;
	_dword				mBackupBufferDataIndex;

	IndexDataStack		mIndexDataStack;
	SectionDataArray	mSectionDataArray;

private:
	//!	Search for section data.
	//!	@param		sectionname		The section name.
	//! @return		The section data pointer.
	const SectionData* SearchSection( StringPtr sectionname ) const;

	//!	Search buffer data's content by key name
	//!	@param		keyname			The key name.
	//!	@param		sectiondata		The section data.
	//!	@return		The content string.
	StringPtr GetBufferDataContent( StringPtr keyname, const SectionData* sectiondata ) const;
};

//----------------------------------------------------------------------------
// IniFile Implementation
//----------------------------------------------------------------------------

_dword IniFile::GetSectionNumber( ) const
{
	return mSectionDataArray.Number( );
}

StringPtr IniFile::GetCurrentSectionName( ) const
{
	if ( mCurrentSectionDataIndex == -1 )
		return L"";

	return mSectionDataArray[ mCurrentSectionDataIndex ].mName;
}

}
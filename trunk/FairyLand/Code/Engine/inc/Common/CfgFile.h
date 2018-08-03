//============================================================================
// CfgFile.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// CfgFile
//----------------------------------------------------------------------------

class CfgFile : public TxtFile
{
public:
	//!	Error callback function.
	//!	@param		cfgfile				The config file.
	//!	@param		keyname				The key name.
	//!	@param		errormessage		The error message.
	//!	@param		filename			The file name.
	//!	@return		none.
	typedef _void	(*OnError)( const CfgFile* cfgfile, StringPtr keyname, StringPtr errormessage, StringPtr filename );

	//!	Read key name callback function.
	//!	@param		cfgfile				The config file.
	//!	@param		fathersectionid		The fahter section id.
	//!	@param		sectionname			The section name.
	//!	@param		defname				The define name.
	//!	@param		keyname				The key name.
	//!	@param		value				The key value.
	//!	@param		filename			The file name.
	//!	@return		If returns L"" then use the 'value' arugment as a final string or else use the returned string.
	typedef String	(*OnReadKeyName)( const CfgFile* cfgfile, _dword fathersectionid, StringPtr sectionname, StringPtr defname, StringPtr keyname, StringPtr value, StringPtr filename );
	//!	Read whole string buffer of section callback function.
	//!	@param		cfgfile				The config file.
	//!	@param		index				The index of sub-string.
	//!	@param		fathersectionid		The fahter section id.
	//!	@param		sectionname			The section name.
	//!	@param		defname				The define name.
	//!	@param		content				The string of content.
	//!	@param		filename			The file name.
	//!	@return		If returns L"" then use the 'content' arugment as a final string or else use the returned string.
	typedef String	(*OnReadSectionString)( const CfgFile* cfgfile, _dword index, _dword fathersectionid, StringPtr sectionname, StringPtr defname, StringPtr content, StringPtr filename );

public:
	//!	Set error callback function.
	//!	@param		funcpointer			The function pointer.
	//!	@return		none.
	inline _void SetErrorCallback( OnError funcpointer );
	//!	Set read section buffer callback function.
	//!	@param		funcpointer			The function pointer.
	//!	@return		none.
	inline _void SetReadSectionStringCallback( OnReadSectionString funcpointer );

	//!	Set current section data.
	//!	@param		sectionid			The section id.
	//! @return		True indicates success, false indicates failure.
	inline _bool SetCurrentSection( _dword sectionid );

	//!	Enable/Disable error callback.
	//! @param		enable				True indicates enable the error callback.
	//! @return		none.
	inline _void EnableErrorCallback( _bool enable );
	//!	Is enable error callback.
	//! @param		none.
	//! @return		True indicates enable error callback.
	inline _bool IsEnableErrorCallback( ) const;

	//!	Enable/Disable read section string callback.
	//! @param		enable				True indicates enable the read section string callback.
	//! @return		none.
	inline _void EnableReadSectionStringCallback( _bool enable );
	//!	Is enable read section string callback.
	//! @param		none.
	//! @return		True indicates enable read section string callback.
	inline _bool IsEnableReadSectionStringCallback( ) const;

public:
	//!	Initialize.
	//! @param		none.
	//! @return		True indicates success, false indicates failure.
	_bool Initialize( );
	//!	Finalize.
	//! @param		none.
	//! @return		none.
	_void Finalize( );

	//!	Save to file.
	//!	@param		filename			The file name.
	//! @return		True indicates success, false indicates failure.
	_bool Save( StringPtr filename ) const;

	//!	Get section number.
	//! @param		none.
	//! @return		The number of sections.
	_dword GetSectionNumber( ) const;
	//!	Get section id.
	//! @param		index				The index of section.
	//! @return		The id of section, -1 indicates failure.
	_dword GetSectionID( _dword index ) const;
	//!	Get section id.
	//! @param		sectionname			The name of section.
	//! @param		defname				The define name of section ( case sensitive ).
	//! @param		parentsectionname	The name of parent section.
	//! @param		parentdefname		The define name of parent section ( case sensitive ).
	//! @return		The id of section, -1 indicates failure.
	_dword GetSectionID( StringPtr sectionname, StringPtr defname = L"", StringPtr parentsectionname = L"", StringPtr parentdefname = L"" ) const;

	//!	Set section user data 1.
	//! @param		userdata			The user data.
	//! @param		sectionid			The section id ( -1 means use the current section ).
	//! @return		True indicates success, false indicates failure.
	_bool	SetSectionUserData1( _dword userdata, _dword sectionid = -1 );
	//!	Set section user data 2.
	//! @param		userdata			The user data.
	//! @param		sectionid			The section id ( -1 means use the current section ).
	//! @return		True indicates success, false indicates failure.
	_bool	SetSectionUserData2( _dword userdata, _dword sectionid = -1 );
	//!	Get section user data 1.
	//! @param		sectionid			The section id ( -1 means use the current section ).
	//! @return		The user data.
	_dword	GetSectionUserData1( _dword sectionid = -1 ) const;
	//!	Get section user data 2.
	//! @param		sectionid			The section id ( -1 means use the current section ).
	//! @return		The user data.
	_dword	GetSectionUserData2( _dword sectionid = -1 ) const;

	//!	Get parent section id.
	//! @param		sectionid			The section id ( -1 means use the current section ).
	//! @return		0 indicates hasn't parent section.
	_dword		GetParentSectionID( _dword sectionid = -1 ) const;
	//!	Get parent section name.
	//! @param		sectionid			The section id ( -1 means use the current section ).
	//! @return		The parent section name.
	StringPtr	GetParentSectionName( _dword sectionid = -1 ) const;

	//!	Get section name.
	//! @param		sectionid			The section id ( -1 means use the current section ).
	//! @return		The section name.
	StringPtr GetSectionName( _dword sectionid = -1 ) const;
	//!	Get section define name.
	//! @param		sectionid			The section id ( -1 means use the current section ).
	//! @return		The section define name.
	StringPtr GetSectionDefName( _dword sectionid = -1 ) const;

	//!	Begin to parse config file.
	//! @param		sectionname			The section name.
	//! @param		defname				The section define name.
	//! @param		level				The section level.
	//! @param		linenumber			The section line number.
	//! @return		False indicates parse finished, true indicates still in procession.
	_bool Parse( StringPtr& sectionname, StringPtr* defname = _null, _dword* level = _null, _dword* linenumber = _null );

	//!	Read section data.
	//! @param		sectionid			The section id ( -1 indicates use the current section data ).
	//! @param		sectionname			The section name.
	//! @param		defname				The section define name.
	//! @param		level				The section level.
	//! @param		linenumber			The section line number.
	//! @return		True indicates success, false indicates failure.
	_bool ReadSection( _dword sectionid, StringPtr& sectionname, StringPtr* defname = _null, _dword* level = _null, _dword* linenumber = _null ) const;
	//!	Read the current section data.
	//! @param		sectionname			The section name.
	//! @param		defname				The section define name.
	//! @param		level				The section level.
	//! @param		linenumber			The section line number.
	//! @return		True indicates success, false indicates failure.
	_bool ReadCurrentSection( StringPtr& sectionname, StringPtr* defname = _null, _dword* level = _null, _dword* linenumber = _null ) const;

	//!	Read section string.
	//! @param		stringbuffer		The string buffer.
	//! @param		sectionid			The section id ( -1 means use the current section ).
	//! @return		True indicates success, false indicates failure.
	_bool ReadSectionString( String& stringbuffer, _dword sectionid = -1 ) const;
	//!	Read section string.
	//! @param		stringarray			The string buffer array ( one line one element ).
	//! @param		sectionid			The section id ( -1 means use the current section ).
	//! @return		True indicates success, false indicates failure.
	_bool ReadSectionString( Array< String >& stringarray, _dword sectionid = -1 ) const;
	//!	Read section string.
	//! @param		stringarray			The content string buffer array ( one line one element ).
	//! @param		commentarray		The comment string buffer array ( one line one element ).
	//! @param		keynamearray		The key name string buffer array ( one line one element ).
	//! @param		sectionid			The section id ( -1 means use the current section ).
	//! @return		True indicates success, false indicates failure.
	_bool ReadSectionString( Array< String >* stringarray, Array< String >* commentarray, Array< String >* keynamearray, _dword sectionid = -1 ) const;

	//!	Read key value.
	//! @param		keyname				The key name.
	//! @param		value				The key value.
	//! @param		comment				The comment of this element.
	//! @param		linenumber			The line number of this element.
	//! @return		True indicates success, false indicates failure.
	_bool ReadKey( StringPtr keyname, _byte& value, String* comment = _null, _dword* linenumber = _null ) const;
	//!	Read key value.
	//! @param		keyname				The key name.
	//! @param		value				The key value.
	//! @param		comment				The comment of this element.
	//! @param		linenumber			The line number of this element.
	//! @return		True indicates success, false indicates failure.
	_bool ReadKey( StringPtr keyname, _tiny& value, String* comment = _null, _dword* linenumber = _null ) const;
	//!	Read key value.
	//! @param		keyname				The key name.
	//! @param		value				The key value.
	//! @param		comment				The comment of this element.
	//! @param		linenumber			The line number of this element.
	//! @return		True indicates success, false indicates failure.
	_bool ReadKey( StringPtr keyname, _short& value, String* comment = _null, _dword* linenumber = _null ) const;
	//!	Read key value.
	//! @param		keyname				The key name.
	//! @param		value				The key value.
	//! @param		comment				The comment of this element.
	//! @param		linenumber			The line number of this element.
	//! @return		True indicates success, false indicates failure.
	_bool ReadKey( StringPtr keyname, _word& value, String* comment = _null, _dword* linenumber = _null ) const;
	//!	Read key value.
	//! @param		keyname				The key name.
	//! @param		value				The key value.
	//! @param		comment				The comment of this element.
	//! @param		linenumber			The line number of this element.
	//! @return		True indicates success, false indicates failure.
	_bool ReadKey( StringPtr keyname, _long& value, String* comment = _null, _dword* linenumber = _null ) const;
	//!	Read key value.
	//! @param		keyname				The key name.
	//! @param		value				The key value.
	//! @param		comment				The comment of this element.
	//! @param		linenumber			The line number of this element.
	//! @return		True indicates success, false indicates failure.
	_bool ReadKey( StringPtr keyname, _dword& value, String* comment = _null, _dword* linenumber = _null ) const;
	//!	Read key value.
	//! @param		keyname				The key name.
	//! @param		value				The key value.
	//! @param		comment				The comment of this element.
	//! @param		linenumber			The line number of this element.
	//! @return		True indicates success, false indicates failure.
	_bool ReadKey( StringPtr keyname, _float& value, String* comment = _null, _dword* linenumber = _null ) const;
	//!	Read key value.
	//! @param		keyname				The key name.
	//! @param		value				The key value.
	//! @param		comment				The comment of this element.
	//! @param		linenumber			The line number of this element.
	//! @return		True indicates success, false indicates failure.
	_bool ReadKey( StringPtr keyname, _double& value, String* comment = _null, _dword* linenumber = _null ) const;
	//!	Read key value.
	//! @param		keyname				The key name.
	//! @param		value				The key value.
	//! @param		comment				The comment of this element.
	//! @param		linenumber			The line number of this element.
	//! @return		True indicates success, false indicates failure.
	_bool ReadKey( StringPtr keyname, String& value, String* comment = _null, _dword* linenumber = _null ) const;
	//!	Read key value.
	//! @param		keyname				The key name.
	//! @param		value				The key value.
	//! @param		comment				The comment of this element.
	//! @param		linenumber			The line number of this element.
	//! @return		True indicates success, false indicates failure.
	_bool ReadKey( StringPtr keyname, Color& color, String* comment = _null, _dword* linenumber = _null ) const;

	//!	Read key value ( min, max ).
	//! @param		keyname				The key name.
	//! @param		minvalue			The min key value.
	//! @param		maxvalue			The max key value.
	//! @param		comment				The comment of this element.
	//! @param		linenumber			The line number of this element.
	//! @return		True indicates success, false indicates failure.
	_bool ReadKey( StringPtr keyname, _long& minvalue, _long& maxvalue, String* comment = _null, _dword* linenumber = _null ) const;
	//!	Read key value ( min, max ).
	//! @param		keyname				The key name.
	//! @param		minvalue			The min key value.
	//! @param		maxvalue			The max key value.
	//! @param		comment				The comment of this element.
	//! @param		linenumber			The line number of this element.
	//! @return		True indicates success, false indicates failure.
	_bool ReadKey( StringPtr keyname, _dword& minvalue, _dword& maxvalue, String* comment = _null, _dword* linenumber = _null ) const;
	//!	Read key value ( min, max ).
	//! @param		keyname				The key name.
	//! @param		minvalue			The min key value.
	//! @param		maxvalue			The max key value.
	//! @param		comment				The comment of this element.
	//! @param		linenumber			The line number of this element.
	//! @return		True indicates success, false indicates failure.
	_bool ReadKey( StringPtr keyname, _float& minvalue, _float& maxvalue, String* comment = _null, _dword* linenumber = _null ) const;

	//!	Write key value.
	//! @param		keyname				The key name.
	//! @param		value				The key value.
	//! @return		True indicates success, false indicates failure.
	_bool WriteKey( StringPtr keyname, _long value );
	//!	Write key value.
	//! @param		keyname				The key name.
	//! @param		value				The key value.
	//! @return		True indicates success, false indicates failure.
	_bool WriteKey( StringPtr keyname, _float value );
	//!	Write key value.
	//! @param		keyname				The key name.
	//! @param		value				The key value.
	//! @return		True indicates success, false indicates failure.
	_bool WriteKey( StringPtr keyname, _double value );
	//!	Write key value.
	//! @param		keyname				The key name.
	//! @param		value				The key value.
	//! @return		True indicates success, false indicates failure.
	_bool WriteKey( StringPtr keyname, StringPtr value );

	//!	Write string list.
	//! @param		stringarray			The string array.
	//! @return		True indicates success, false indicates failure.
	_bool WriteKey( const Array< StringPtr >& stringarray );
	//!	Write string list.
	//! @param		stringarray			The string array.
	//! @return		True indicates success, false indicates failure.
	_bool WriteKey( const Array< String >& stringarray );

	//!	Create section.
	//!	@param		name				The section name.
	//!	@param		defname				The section define name.
	//!	@param		parentsectionid		The parent section id, -1 indicates no parent section.
	//! @return		The section id, -1 indicates failure.
	_dword CreateSection( StringPtr name, StringPtr defname = L"", _dword parentsectionid = -1 );
	//!	Create key in section.
	//!	@param		sectionid			The section id.
	//!	@param		name				The key name.
	//!	@param		value				The key value.
	//! @return		True indicates success, false indicates failure.
	_bool CreateKey( _dword sectionid, StringPtr name, StringPtr value );

	//!	Set read key callback function.
	//! @param		keyname				The key name.
	//! @param		funcpointer			The function pointer.
	//! @param		sectionid			The section id ( -1 means use the current section ).
	//! @return		True indicates success, false indicates failure.
	_bool SetReadKeyCallback( StringPtr keyname, OnReadKeyName funcpointer, _dword sectionid = -1 );
	//!	Remove read key callback function.
	//! @param		keyname				The key name.
	//! @param		sectionid			The section id ( -1 means use the current section ).
	//! @return		none.
	_void RemoveReadKeyCallback( StringPtr keyname, _dword sectionid = -1 );
	//!	Remove all read keys callback function.
	//! @param		sectionid			The section id ( -1 means remove all read key callback function in all sections  ).
	//! @return		none.
	_void RemoveAllReadKeyCallback( _dword sectionid = -1 );

public:
	CfgFile( );
	~CfgFile( );

private:
	//!	DwordArray
	typedef Array< _dword > DwordArray;

	//!	BufferData
	struct BufferData
	{
		_dword	mLineNumber;
		String	mKeyName;
		String	mContent;
		String	mComment;
	};
	typedef Array< BufferData > BufferDataArray;

	//! SectionData
	struct SectionData
	{
		_dword			mID;
		_dword			mFatherID;
		_dword			mLevel;

		_dword			mLineNumber;

		_dword			mUserData1;
		_dword			mUserData2;

		String			mName;
		String			mDefName;

		BufferDataArray	mBufferArray;
	};
	typedef Array< SectionData* > SectionDataArray;

	//! KeyNameData ( Callback function )
	struct KeyNameData
	{
		StringPtr		mKeyName;
		OnReadKeyName	mReadKeyNameFunc;

		operator StringPtr ( ) const
			{ return mKeyName; }
	};
	typedef Array< KeyNameData, StringPtr > KeyNameDataArray;

	//! ReadKeyCallbackData
	struct ReadKeyCallbackData
	{
		SectionData*		mSectionData;
		KeyNameDataArray	mKeyNameDataArray;

		operator _dword ( ) const
			{ return mSectionData->mID; }
	};
	typedef Array< ReadKeyCallbackData, _dword > ReadKeyCallbackDataArray;

	//!	RootData
	struct RootData
	{
		DwordArray			mChildIDArray;
		const SectionData*	mSectionData;

		operator _dword( ) const 
			{ return mSectionData->mID; }

		RootData( const SectionData* sectiondata = _null ) : mSectionData( sectiondata ) { }
	};
	typedef Array< RootData, _dword > RootDataArray;

private:
	enum _TYPE
	{
		_TYPE_FLOAT		= 1, // Float
		_TYPE_INTEGER	= 2, // Integer
	};

private:
	_bool						mEnableErrorCallback;
	_bool						mEnableReadSectionStringCallback;

	SectionData*				mCurrentSection;
	SectionData*				mNextSection;

	SectionDataArray			mSectionDataArray;

	ReadKeyCallbackDataArray	mReadKeyCallbackDataArray;

	OnError						mErrorFunc;
	OnReadSectionString			mReadSectionStringFunc;

private:
	//!	Parse section.
	//! @param		section				The section data.
	//! @param		sectionname			The section name.
	//! @param		defname				The section define name.
	//! @param		level				The section level.
	//! @param		linenumber			The section line number.
	//!	@return		none.
	inline _void	ParseSectionData( const SectionData* section, StringPtr& sectionname, StringPtr* defname, _dword* level, _dword* linenumber ) const;

	//!	Build section.
	//! @param		section				The section data.
	//! @param		level				The section level.
	//! @return		True indicates success, false indicates failure.
	_bool			BuildSection( SectionData* section, _dword level = 1 );
	//!	Create section.
	//! @param		string				The section name and define name ( maybe doesn't contain define name ).
	//! @param		fatherid			The section father id.
	//! @param		level				The section level.
	//! @param		llinenumber			The section line number.
	//! @return		_null indicates creating failed.
	SectionData*	CreateSection( const String& string, _dword fatherid, _dword level, _dword linenumber );

	//!	Get section.
	//! @param		sectionid			The section id ( -1 means use the current section ).
	//! @return		_null indicates getting failed.
	SectionData*	GetSection( _dword sectionid ) const;
	//!	Search section.
	//! @param		sectionid			The section id.
	//! @return		_null indicates getting failed.
	SectionData*	SearchSection( _dword sectionid ) const;

	//!	Read key value.
	//! @param		keyname				The key name.
	//! @param		bufferarray			The key&content pair array.
	//! @param		valuestring			The key value.
	//! @param		comment				The comment of this element.
	//! @param		linenumber			The line number of this element.
	//! @return		True indicates success, false indicates failure.
	_bool ReadKey( StringPtr keyname, const BufferDataArray& bufferarray, String& valuestring, String* comment, _dword* linenumber ) const;
	//!	Read key value ( main access point ).
	//! @param		section				The section.
	//! @param		keyname				The key name.
	//! @param		valuestring			The key value ( input ).
	//! @param		resultstring		The key value ( output/result ).
	//! @return		True indicates use 'result' argument as key value string, false indicates use 'value' argument as key value string.
	_bool ReadKeyValue( const SectionData* section, StringPtr keyname, StringPtr valuestring, String& resultstring ) const;

	//!	Set read key value callback function.
	//! @param		sectionid			The section id.
	//! @param		keyname				The key name.
	//! @param		funcpointer			The function pointer.
	//! @return		True indicates success, false indicates failure.
	_bool						SetReadKeyCallback( const SectionData* section, StringPtr keyname, OnReadKeyName funcpointer );
	//!	Search read key value callback data.
	//! @param		sectionid			The section id.
	//! @return		_null indicates this section hasn't any read key value callback function at all.
	const ReadKeyCallbackData*	SearchReadKeyCallbackData( _dword sectionid ) const;

	//!	Search key name callback data.
	//! @param		sectionid			The section id.
	//! @param		keyname				The key name.
	//! @return		_null indicates this section hasn't any key name callback function at all.
	const KeyNameData* SearchKeyNameData( _dword sectionid, StringPtr keyname ) const;

	//!	Search buffer data.
	//! @param		section				The section.
	//!	@param		keyname				The key name.
	//!	@return		The BufferData pointer.
	BufferData* SearchBufferData( SectionData* sectiondata, StringPtr keyname );

	//!	Read section string.
	//! @param		section				The section.
	//! @param		stringbuffer		The section string buffer.
	//! @return		True indicates success, false indicates failure.
	_bool CoreReadSectionString( const SectionData* section, String& stringbuffer ) const;
	//!	Read section string.
	//! @param		section				The section.
	//! @param		stringarray			The string buffer array ( one line one element ).
	//! @return		True indicates success, false indicates failure.
	_bool CoreReadSectionString( const SectionData* section, Array< String >& stringarray ) const;
	//!	Read section string.
	//! @param		section				The section.
	//! @param		stringarray			The content string buffer array ( one line one element ).
	//! @param		commentarray		The comment string buffer array ( one line one element ).
	//! @param		keynamearray		The key name string buffer array ( one line one element ).
	//! @return		True indicates success, false indicates failure.
	_bool CoreReadSectionString( const SectionData* section, Array< String >* stringarray, Array< String >* commentarray, Array< String >* keynamearray ) const;

	//!	Get all section keyname.
	//! @param		section				The section.
	//! @param		keynametable		The key name string table.
	//! @return		The number of keyname buffer.
	_dword GetSectionKeyName( const SectionData* section, _char* keynametable[] ) const;

	//!	Match keyname in buffer data.
	//! @param		section				The section.
	//! @param		keyname				The key name.
	//! @return		none.
	_void MatchKeyName( const SectionData* section, _char*& keyname ) const;

	//!	Read key value.
	//! @param		valuetype			The key value type.
	//! @param		keyname				The key name.
	//! @param		value				The feedback value.
	//! @param		comment				The comment of this element.
	//! @param		linenumber			The line number of this element.
	//! @return		True indicates success, false indicates failure.
	template < typename Type1, typename Type2 >
	_bool ReadKey( _dword valuetype, StringPtr keyname, Type2& value, String* comment, _dword* linenumber ) const;
	//!	Read key value ( min, max ).
	//! @param		valuetype			The key value type.
	//! @param		keyname				The key name.
	//! @param		minvalue			The feedback min key value.
	//! @param		maxvalue			The feedback max key value.
	//! @param		comment				The comment of this element.
	//! @param		linenumber			The line number of this element.
	//! @return		True indicates success, false indicates failure.
	template < typename Type1, typename Type2 >
	_bool ReadKey( _dword valuetype, StringPtr keyname, Type2& minvalue, Type2& maxvalue, String* comment, _dword* linenumber ) const;

	//!	Write key value.
	//! @param		keyname				The key name.
	//! @param		value				The key value.
	//! @return		True indicates success, false indicates failure.
	template < typename Type1, typename Type2 >
	_bool WriteKey( StringPtr keyname, Type1 value );
	//!	Write string list.
	//! @param		stringarray			The string array.
	//! @return		True indicates success, false indicates failure.
	template < typename Type >
	_bool WriteKey( const Type& stringarray );

	//!	Write tabs.
	//!	@param		file				The file.
	//!	@param		number				The number of tabs.
	//! @return		none.
	_void WriteTabs( File& file, _dword number ) const;
	//!	Write left bracket.
	//!	@param		file				The file.
	//! @param		section				The section.
	//! @return		none.
	_void WriteLeftBracket( File& file, const SectionData* section ) const;
	//!	Write right bracket.
	//!	@param		file				The file.
	//! @param		section				The section.
	//! @return		none.
	_void WriteRightBracket( File& file, const SectionData* section ) const;
	//!	Write section name.
	//!	@param		file				The file.
	//! @param		section				The section.
	//! @return		none.
	_void WriteSectionName( File& file, const SectionData* section ) const;
	//!	Write buffer data.
	//!	@param		file				The file.
	//! @param		bufferdata			The buffer data.
	//! @param		section				The section.
	//! @return		none.
	_void WriteBufferData( File& file, const BufferData& bufferdata, const SectionData* section ) const;

	//!	Save section data.
	//!	@param		file				The file
	//! @param		rootdata			The root data.
	//! @param		rootdataarray		The root data array.
	//! @return		True indicates success, false indicates failure.
	_bool Save( File& file, const RootData& rootdata, const RootDataArray& rootdataarray ) const;
};

//----------------------------------------------------------------------------
// CfgFile Implementation
//----------------------------------------------------------------------------

_void CfgFile::SetErrorCallback( OnError funcpointer )
{
	mErrorFunc = funcpointer;
}

_void CfgFile::SetReadSectionStringCallback( OnReadSectionString funcpointer )
{
	mReadSectionStringFunc = funcpointer;
}

_bool CfgFile::SetCurrentSection( _dword sectionid )
{
	if ( sectionid > mSectionDataArray.Number( ) )
		return _false;

	mCurrentSection	= mSectionDataArray[ sectionid ];
	mNextSection	= mCurrentSection;

	return _true;
}

_void CfgFile::EnableErrorCallback( _bool enable )
{
	mEnableErrorCallback = enable;
}

_bool CfgFile::IsEnableErrorCallback( ) const
{
	return mEnableErrorCallback;
}

_void CfgFile::EnableReadSectionStringCallback( _bool enable )
{
	mEnableReadSectionStringCallback = enable;
}

_bool CfgFile::IsEnableReadSectionStringCallback( ) const
{
	return mEnableReadSectionStringCallback;
}

_void CfgFile::ParseSectionData( const SectionData* section, StringPtr& sectionname, StringPtr* defname, _dword* level, _dword* linenumber ) const
{
	// Feedback section name
	sectionname = section->mName;

	// Feedback def name
	if ( defname != _null )
		*defname = section->mDefName;

	// Feedback level ( indent )
	if ( level != _null )
		*level = section->mLevel;

	// Feedback line number
	if ( linenumber != _null )
		*linenumber = section->mLineNumber;
}

}
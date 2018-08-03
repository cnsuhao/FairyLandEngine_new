//============================================================================
// Variable.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

Variable Variable::cInvalidVariable;

//----------------------------------------------------------------------------
// Variable Implementation
//----------------------------------------------------------------------------

Variable::Variable( StringPtr name, _dword type ) : mName( name ), mType( type )
{
	mDword				= 0;
	mVariableNumber		= _null;
	mVariableTemplate	= _null;

	// Create variable number.
	if ( ( mType & ~_TYPE_MASK ) == _TYPE_ARRAY )
		mVariableNumber = new Variable( L"#", _TYPE_DWORD );

	// Create variable template for struct array.
	if ( ( mType & ~_TYPE_MASK ) == _TYPE_ARRAY && ( mType & _TYPE_MASK ) == _TYPE_STRUCT )
		mVariableTemplate = new Variable( L"", _TYPE_STRUCT );
}

Variable::~Variable( )
{
	if ( mType == _TYPE_STRING && mString != _null )
		delete mString;

	if ( mVariableNumber != _null )
		delete mVariableNumber;

	if ( mVariableTemplate != _null )
		delete mVariableTemplate;

	for ( _dword i = 0; i < mVariableArray.Number( ); i ++ )
		delete mVariableArray[i];
}

_void Variable::CopyString( StringPtr string )
{
	if ( mType != _TYPE_STRING )
		return;

	if ( mString != _null )
		delete mString;

	if ( string == L"" )
	{
		mString = _null;
	}
	else
	{
		mString = new _char[ string.Length( ) + 1 ];
		StringFormatter::CopyString( mString, string );
	}
}

_void Variable::SplitNameL2R( const _char* name, _char* buffer, _char*& prefix, _char*& postfix ) const
{
	prefix = buffer;

	_dword length = 0;

	// Copy prefix name to buffer.
	for ( _dword i = 0; name[i] != 0; i ++ )
	{
		buffer[ length ++ ] = name[i];

		// Skip first character, even it is delimiter.
		if ( i == 0 )
			continue;

		// Found delimiter '.' .
		if ( name[i] == '.' )
		{
			buffer[ length - 1 ] = 0;

			postfix = (_char*) name + i + 1;

			return;
		}
		// Found delimiter '[' .
		else if ( name[i] == '[' )
		{
			buffer[ length - 1 ] = 0;

			postfix = (_char*) name + i;

			return;
		}
		// Found delimiter '#' .
		else if ( name[i] == '#' )
		{
			buffer[ length - 1 ] = 0;

			postfix = (_char*) name + i;

			return;
		}
	}

	buffer[ length ] = 0;
}

_void Variable::SplitNameR2L( const _char* name, _char* buffer, _char*& prefix, _char*& postfix ) const
{
	postfix = buffer;

	_dword length = 0, index = 0;

	// Copy prefix name to buffer.
	for ( _dword i = 0; name[i] != 0; i ++ )
	{
		buffer[ length ++ ] = name[i];

		// Found delimiter '.', save the index to split postfix.
		if ( name[i] == '.' )
			index = i;
	}

	// We can split name at index.
	if ( index > 0 )
	{
		buffer[ index ] = 0;

		prefix  = buffer;
		postfix = buffer + index + 1;
	}

	buffer[ length ] = 0;
}

Variable* Variable::CloneVariable( StringPtr name, const Variable& variabletemplate )
{
	Variable* newvariable = new Variable( name, variabletemplate.mType );

	// Clone struct template.
	if ( variabletemplate.mVariableTemplate != _null )
	{
		if ( newvariable->mVariableTemplate != _null )
			delete newvariable->mVariableTemplate;

		newvariable->mVariableTemplate = CloneVariable( L"", *variabletemplate.mVariableTemplate );
	}

	// Set variable number.
	if ( ( variabletemplate.mType & ~_TYPE_MASK ) == _TYPE_ARRAY )
	{
		if ( newvariable->mVariableNumber != _null )
			newvariable->mVariableNumber->mDword = variabletemplate.mVariableArray.Number( );
	}

	// Clone each element in array.
	if ( variabletemplate.mVariableArray.Number( ) > 0 )
	{
		newvariable->mVariableArray.Clear( variabletemplate.mVariableArray.Number( ), 10 );

		for ( _dword i = 0; i < variabletemplate.mVariableArray.Number( ); i ++ )
		{
			const Variable& element = *variabletemplate.mVariableArray[i];

			newvariable->mVariableArray.Append( CloneVariable( element.mName, element ) );
		}
	}

	return newvariable;
}

StringPtr Variable::Type2String( _dword type )
{
	_dword subtype = type & _TYPE_MASK;

	// Variable string.
	if ( ( type & ~_TYPE_MASK ) == _TYPE_ARRAY )
	{
		switch ( subtype )
		{
			case _TYPE_CHAR:	return L"#CHR";
			case _TYPE_TINY:	return L"#TNY";
			case _TYPE_SHORT:	return L"#SHT";
			case _TYPE_LONG:	return L"#LNG";
			case _TYPE_BYTE:	return L"#BYT";
			case _TYPE_WORD:	return L"#WOD";
			case _TYPE_DWORD:	return L"#DWD";
			case _TYPE_FLOAT:	return L"#FLT";
			case _TYPE_STRING:	return L"#STR";
			case _TYPE_STRUCT:	return L"#STT";
		}
	}
	else
	{
		switch ( subtype )
		{
			case _TYPE_CHAR:	return L"CHR";
			case _TYPE_TINY:	return L"TNY";
			case _TYPE_SHORT:	return L"SHT";
			case _TYPE_LONG:	return L"LNG";
			case _TYPE_BYTE:	return L"BYT";
			case _TYPE_WORD:	return L"WOD";
			case _TYPE_DWORD:	return L"DWD";
			case _TYPE_FLOAT:	return L"FLT";
			case _TYPE_STRING:	return L"STR";
			case _TYPE_STRUCT:	return L"STT";
		}
	}

	return L"";
}

_dword Variable::String2Type( StringPtr type )
{
	_dword vartype = 0;

	if ( type[0] == '#' )
	{
		type = (const _char*) type + 1;
		vartype = Variable::_TYPE_ARRAY;
	}

	if ( type == L"CHR" )		vartype |= _TYPE_CHAR;
	else if ( type == L"TNY" )	vartype |= _TYPE_TINY;
	else if ( type == L"SHT" )	vartype |= _TYPE_SHORT;
	else if ( type == L"LNG" )	vartype |= _TYPE_LONG;
	else if ( type == L"BYT" )	vartype |= _TYPE_BYTE;
	else if ( type == L"WOD" )	vartype |= _TYPE_WORD;
	else if ( type == L"DWD" )	vartype |= _TYPE_DWORD;
	else if ( type == L"FLT" )	vartype |= _TYPE_FLOAT;
	else if ( type == L"STR" )	vartype |= _TYPE_STRING;
	else if ( type == L"STT" )	vartype |= _TYPE_STRUCT;
	else						vartype = 0;

	return vartype;
}

const Variable& Variable::operator [] ( const _char* name ) const
{
	if ( StringPtr( name ) == L"" )
		return *this;

	_char buffer[1024], *prefix = L"", *postfix = L"";
	SplitNameL2R( name, buffer, prefix, postfix );

	// Search in array, using faster method.
	if ( ( mType & _TYPE_ARRAY ) != 0 && name[0] == '[' )
	{
		_dword index = Long( name + 1 );

		if ( index < mVariableArray.Number( ) )
			return mVariableArray[ index ]->operator []( postfix );
	}
	else
	{
		//Variable targetvariable;
		//targetvariable.mSearchName = prefix;

		//// Search in sub variable array.
		//if ( mVariableArray.Number( ) > 0 )
		//{
		//	VariableArray::Iterator it = mVariableArray.SearchAscending( &targetvariable );

		//	if ( it.Valid( ) )
		//		return ( (Variable*) it )->operator []( postfix );
		//}

		// Search sub variable in array.
		for ( _dword i = 0; i < mVariableArray.Number( ); i ++ )
		{
			if ( mVariableArray[i]->mName == prefix )
				return mVariableArray[i]->operator []( postfix );
		}
	}

	// Const version should not search in template, just try to search in variable number.
	if ( mVariableNumber != _null )
	{
		if ( mVariableNumber->mName == prefix )
			return *mVariableNumber;
	}

	return cInvalidVariable;
}

const Variable& Variable::operator () ( const _char* name, _dword i ) const
{
	_char buffer[1024];
	StringFormatter::FormatBuffer( buffer, 1024, name, i );

	return operator []( buffer );
}

const Variable& Variable::operator () ( const _char* name, _dword i, _dword j ) const
{
	_char buffer[1024];
	StringFormatter::FormatBuffer( buffer, 1024, name, i, j );

	return operator []( buffer );
}

const Variable& Variable::operator () ( const _char* name, _dword i, _dword j, _dword k ) const
{
	_char buffer[1024];
	StringFormatter::FormatBuffer( buffer, 1024, name, i, j, k );

	return operator []( buffer );
}

Variable& Variable::operator [] ( const _char* name )
{
	if ( StringPtr( name ) == L"" )
		return *this;

	_char buffer[1024], *prefix = L"", *postfix = L"";
	SplitNameL2R( name, buffer, prefix, postfix );

	// Search in array, using faster method.
	if ( ( mType & _TYPE_ARRAY ) != 0 && name[0] == '[' )
	{
		_dword index = Long( name + 1 );

		if ( index < mVariableArray.Number( ) )
			return mVariableArray[ index ]->operator []( postfix );
	}
	else
	{
		// Search sub variable in array.
		for ( _dword i = 0; i < mVariableArray.Number( ); i ++ )
		{
			if ( mVariableArray[i]->mName == prefix )
				return mVariableArray[i]->operator []( postfix );
		}
	}

	// Search in template variable array.
	if ( mVariableTemplate != _null && mVariableTemplate->mVariableArray.Number( ) > 0 )
	{
		//Variable targetvariable;
		//targetvariable.mSearchName = prefix;

		//VariableArray::Iterator it = mVariableTemplate->mVariableArray.SearchAscending( &targetvariable );

		//if ( it.Valid( ) )
		//	return ( (Variable*) it )->operator []( postfix );

		for ( _dword i = 0; i < mVariableTemplate->mVariableArray.Number( ); i ++ )
		{
			if ( mVariableTemplate->mVariableArray[i]->mName == prefix )
				return mVariableTemplate->mVariableArray[i]->operator []( postfix );
		}
	}

	// Search in variable number.
	if ( mVariableNumber != _null )
	{
		if ( mVariableNumber->mName == prefix )
			return *mVariableNumber;
	}

	return cInvalidVariable;
}

Variable& Variable::operator () ( const _char* name, _dword i )
{
	_char buffer[1024];
	StringFormatter::FormatBuffer( buffer, 1024, name, i );

	return operator []( buffer );
}

Variable& Variable::operator () ( const _char* name, _dword i, _dword j )
{
	_char buffer[1024];
	StringFormatter::FormatBuffer( buffer, 1024, name, i, j );

	return operator []( buffer );
}

Variable& Variable::operator () ( const _char* name, _dword i, _dword j, _dword k )
{
	_char buffer[1024];
	StringFormatter::FormatBuffer( buffer, 1024, name, i, j, k );

	return operator []( buffer );
}

Variable& Variable::Redeclare( StringPtr name, _dword type )
{
	if ( mType == _TYPE_STRING && mString != _null )
		delete mString;

	if ( mVariableNumber != _null )
		delete mVariableNumber;

	if ( mVariableTemplate != _null )
		delete mVariableTemplate;

	for ( _dword i = 0; i < mVariableArray.Number( ); i ++ )
		delete mVariableArray[i];

	mVariableTemplate	= _null;
	mVariableNumber		= _null;
	mVariableArray.Clear( );

	mName = name;
	mType = type;

	// Create variable number.
	if ( ( mType & ~_TYPE_MASK ) == _TYPE_ARRAY )
		mVariableNumber = new Variable( L"#", _TYPE_DWORD );

	// Create variable template for struct array.
	if ( ( mType & ~_TYPE_MASK ) == _TYPE_ARRAY && ( mType & _TYPE_MASK ) == _TYPE_STRUCT )
		mVariableTemplate = new Variable( L"", _TYPE_STRUCT );

	return *this;
}

Variable& Variable::DefineMember( StringPtr name, _dword type )
{
	// Split variable name to prefix and postfix.
	_char buffer[1024], *prefix = L"", *postfix = L"";
	SplitNameR2L( name, buffer, prefix, postfix );

	if ( StringPtr( postfix ) == L"" )
		return cInvalidVariable;

	// Search sub variable by prefix.
	Variable& subvariable = operator [] ( prefix );

	// Auto create sub variable if needed.
	if ( subvariable.mType == _TYPE_UNKNOWN && prefix != L"" )
	{
		Variable& submember = DefineMember( prefix, Variable::_TYPE_STRUCT );

		return submember.DefineMember( postfix, type );
	}

	// The parent variable to be create a member in.
	Variable* parentvariable = _null;

	// Create member for template.
	if ( ( subvariable.mType & ~_TYPE_MASK ) == _TYPE_ARRAY )
	{
		FL_ASSERT( subvariable.mVariableTemplate != _null )

		Variable& templatevariable = *subvariable.mVariableTemplate;

		// Only struct can create sub variable.
		if ( templatevariable.mType != _TYPE_STRUCT )
			return cInvalidVariable;

		parentvariable = &templatevariable;
	}
	else
	{
		// Only struct can create sub variable.
		if ( subvariable.mType != _TYPE_STRUCT )
			return cInvalidVariable;

		parentvariable = &subvariable;
	}

	if ( parentvariable == _null )
		return cInvalidVariable;

	Array< Variable* >& variablearray = parentvariable->mVariableArray;

	// Try to find the existing member.
	for ( _dword i = 0; i < variablearray.Number( ); i ++ )
	{
		if ( variablearray[i]->mName == postfix )
			return *variablearray[i];
	}

	// Create an new variable member.
	Variable* member = new Variable( postfix, type );

	// Insert into parent variable array.
	parentvariable->mVariableArray.Append( member );

	// Create member for all existing elements.
	if ( ( subvariable.mType & ~_TYPE_MASK ) == _TYPE_ARRAY )
	{
		for ( _dword i = 0; i < subvariable.mVariableArray.Number( ); i ++ )
			subvariable.mVariableArray[i]->DefineMember( postfix, type );
	}

	return *member;
}

Variable& Variable::CreateElement( StringPtr name, _dword number )
{
	// Cant create element.
	if ( number == 0 || number == -1 )
		return cInvalidVariable;

	Variable& subvariable = operator [] ( name );

	// Only array can create elelment.
	if ( ( subvariable.mType & ~_TYPE_MASK ) != _TYPE_ARRAY )
		return cInvalidVariable;

	Variable* element = _null;

	for ( _dword i = 0; i < number; i ++ )
	{
		_char elementname[1024];
		StringFormatter::FormatBuffer( elementname, 1024, L"[%d]", subvariable.mVariableArray.Number( ) );

		// This variable is a struct array, so create new element by clone template.
		if ( subvariable.mVariableTemplate != _null )
		{
			element = CloneVariable( elementname, *subvariable.mVariableTemplate );
		}
		// Create new element, type is same as parent variable.
		else
		{
			element = new Variable( elementname, subvariable.mType & _TYPE_MASK );
		}

		// Put the new created element into array.
		subvariable.mVariableArray.Append( element );

		// Set variable number.
		if ( subvariable.mVariableNumber != _null )
			*subvariable.mVariableNumber = subvariable.mVariableArray.Number( );
	}

	if ( element == _null )
		return cInvalidVariable;

	return *element;
}

_bool Variable::DeleteElement( StringPtr name, _dword index )
{
	Variable& subvariable = operator [] ( name );

	// Only array can create elelment.
	if ( ( subvariable.mType & ~_TYPE_MASK ) != _TYPE_ARRAY )
		return _false;

	if ( index >= subvariable.mVariableArray.Number( ) )
		return _false;

	delete subvariable.mVariableArray[ index ];

	// Remove element from variable array.
	subvariable.mVariableArray.Remove( index );

	// Change all elements name.
	for ( _dword i = 0; i < subvariable.mVariableArray.Number( ); i ++ )
		subvariable.mVariableArray[i]->mName.Format( L"[%d]", i );

	// Set variable number.
	if ( subvariable.mVariableNumber != _null )
		*subvariable.mVariableNumber = subvariable.mVariableArray.Number( );

	return _true;
}

_bool Variable::ClearElement( StringPtr name )
{
	Variable& subvariable = operator [] ( name );

	// Only array can clear elelment.
	if ( ( subvariable.mType & ~_TYPE_MASK ) != _TYPE_ARRAY )
		return _false;

	for ( _dword i = 0; i < subvariable.mVariableArray.Number( ); i ++ )
		delete subvariable.mVariableArray[i];

	subvariable.mVariableArray.Clear( _true );

	// Set variable number.
	if ( subvariable.mVariableNumber != _null )
		*subvariable.mVariableNumber = (_dword) 0;

	return _true;
}

_void Variable::Clear( )
{
	mName = L"";
	mType = _TYPE_UNKNOWN;

	if ( mType == _TYPE_STRING && mString != _null )
		delete mString;

	if ( mVariableNumber != _null )
		delete mVariableNumber;

	if ( mVariableTemplate != _null )
		delete mVariableTemplate;

	for ( _dword i = 0; i < mVariableArray.Number( ); i ++ )
		delete mVariableArray[i];

	mVariableArray.Clear( _true );
}

//----------------------------------------------------------------------------
// VariableOperation
//----------------------------------------------------------------------------

VariableOperation::VariableOperation( )
{
	mUserData1		= 0;
	mUserData2		= 0;
	mUserData3		= 0;
	mUserData4		= 0;
	mRedOptNumber	= 0;
	mDefOptNumber	= 0;
	mApdOptNumber	= 0;
	mDelOptNumber	= 0;
	mUpdOptNumber	= 0;
	mQueOptNumber	= 0;
}

VariableOperation::~VariableOperation( )
{
	Clear( );
}

_bool VariableOperation::GetVariableType( _char* buffer, _char*& value, const _char* string ) const
{
	if ( string[0] == '#' )
	{
		buffer[0] = '#';
		buffer ++;
		string ++;
	}

	// Copy variable type to temp buffer.
	for ( _dword i = 0; i < 4; i ++ )
	{
		buffer[i] = 0;

		if ( string[i] == 0 )
			return _false;

		buffer[i] = string[i];
	}

	if ( buffer[3] != ':' )
		return _false;

	// Now we found delimiter, slipt to type and value.
	buffer[3] = 0;
	value	  = (_char*) string + 4;

	return _true;
}

_void VariableOperation::AppendUpdateOperation( StringPtr name, StringPtr variable )
{
	// AllocaterSwitcher switcher( &mAllocater );

	// Try to combine with previous create operation.
	if ( mDefOptNumber > 0 )
	{
		Operation& prevoperation = mOperationArray[ mRedOptNumber + mDefOptNumber - 1 ];

		// We can combine this update operation with previous create operation.
		if ( prevoperation.mObject1 == _OPERATION_DEFINE_MEMBER && prevoperation.mObject2 == name )
		{
			prevoperation.mObject3 = variable;

			return;
		}
	}

	mOperationArray.Append( Operation( _OPERATION_UPDATE_VARIABLE, String( name ), String( variable ) ) );
}

StringPtr VariableOperation::Type2String( _dword type )
{
	switch ( type )
	{
		case _OPERATION_REDECLARE:			return L"RED";
		case _OPERATION_DEFINE_MEMBER:		return L"DEF";
		case _OPERATION_CREATE_ELEMENT:		return L"APD";
		case _OPERATION_DELETE_ELEMENT:		return L"DEL";
		case _OPERATION_UPDATE_VARIABLE:	return L"UPD";
		case _OPERATION_QUERY_VARIABLE:		return L"QUE";
	}

	return L"";
}

_dword VariableOperation::String2Type( StringPtr type )
{
	if ( type == L"RED" )		return _OPERATION_REDECLARE;
	else if ( type == L"DEF" )	return _OPERATION_DEFINE_MEMBER;
	else if ( type == L"APD" )	return _OPERATION_CREATE_ELEMENT;
	else if ( type == L"DEL" )	return _OPERATION_DELETE_ELEMENT;
	else if ( type == L"UPD" )	return _OPERATION_UPDATE_VARIABLE;
	else if ( type == L"QUE" )	return _OPERATION_QUERY_VARIABLE;

	return 0;
}

_void VariableOperation::SetUserData1( _dword userdata )
{
	mUserData1 = userdata;
}

_dword VariableOperation::GetUserData1( ) const
{
	return mUserData1;
}

_void VariableOperation::SetUserData2( _dword userdata )
{
	mUserData2 = userdata;
}

_dword VariableOperation::GetUserData2( ) const
{
	return mUserData2;
}

_void VariableOperation::SetUserData3( _dword userdata )
{
	mUserData3 = userdata;
}

_dword VariableOperation::GetUserData3( ) const
{
	return mUserData3;
}

_void VariableOperation::SetUserData4( _dword userdata )
{
	mUserData4 = userdata;
}

_dword VariableOperation::GetUserData4( ) const
{
	return mUserData4;
}

_dword VariableOperation::GetOperationNumber( ) const
{
	return mOperationArray.Number( );
}

_dword VariableOperation::GetRedeclareOperationNumber( ) const
{
	return mRedOptNumber;
}

_dword VariableOperation::GetDefineOperationNumber( ) const
{
	return mDefOptNumber;
}

_dword VariableOperation::GetCreateOperationNumber( ) const
{
	return mApdOptNumber;
}

_dword VariableOperation::GetDeleteOperationNumber( ) const
{
	return mDelOptNumber;
}

_dword VariableOperation::GetUpdateOperationNumber( ) const
{
	return mUpdOptNumber;
}

_dword VariableOperation::GetQueryOperationNumber( ) const
{
	return mQueOptNumber;
}

_dword VariableOperation::GetOperationType( _dword index ) const
{
	return mOperationArray[ index ].mObject1;
}

StringPtr VariableOperation::GetOperationParam1( _dword index ) const
{
	return mOperationArray[ index ].mObject2;
}

StringPtr VariableOperation::GetOperationParam2( _dword index ) const
{
	return mOperationArray[ index ].mObject3;
}

_void VariableOperation::Redeclare( StringPtr name, _dword type )
{
	// AllocaterSwitcher switcher( &mAllocater );

	// Try to replace existing redeclare operation.
	for ( _dword i = 0; i < mRedOptNumber; i ++ )
	{
		if ( mOperationArray[i].mObject2 == name )
		{
			mOperationArray[i].mObject3 = Variable::Type2String( type );

			return;
		}
	}

	// Create new redeclare operation.
	Operation redopt( _OPERATION_REDECLARE, String( name ), String( Variable::Type2String( type ) ) );

	_dword insertindex = mRedOptNumber; mRedOptNumber ++;

	mOperationArray.Insert( redopt, insertindex );
}

_void VariableOperation::DefineMember( StringPtr name, _dword type )
{
	// AllocaterSwitcher switcher( &mAllocater );

	// Try to replace existing define operation.
	for ( _dword i = mRedOptNumber; i < mRedOptNumber + mDefOptNumber; i ++ )
	{
		if ( mOperationArray[i].mObject2 == name )
		{
			mOperationArray[i].mObject3 = Variable::Type2String( type );

			return;
		}
	}

	Operation defopt( _OPERATION_DEFINE_MEMBER, String( name ), String( Variable::Type2String( type ) ) );

	_dword insertindex = mRedOptNumber + mDefOptNumber; mDefOptNumber ++;

	mOperationArray.Insert( defopt, insertindex );
}

_void VariableOperation::DefineMember( StringPtr name, _dword type, _char value )
{
	DefineMember( name, type );
	UpdateVariable( name, value );
}

_void VariableOperation::DefineMember( StringPtr name, _dword type, _tiny value )
{
	DefineMember( name, type );
	UpdateVariable( name, value );
}

_void VariableOperation::DefineMember( StringPtr name, _dword type, _short value )
{
	DefineMember( name, type );
	UpdateVariable( name, value );
}

_void VariableOperation::DefineMember( StringPtr name, _dword type, _long value )
{
	DefineMember( name, type );
	UpdateVariable( name, value );
}

_void VariableOperation::DefineMember( StringPtr name, _dword type, _byte value )
{
	DefineMember( name, type );
	UpdateVariable( name, value );
}

_void VariableOperation::DefineMember( StringPtr name, _dword type, _word value )
{
	DefineMember( name, type );
	UpdateVariable( name, value );
}

_void VariableOperation::DefineMember( StringPtr name, _dword type, _dword value )
{
	DefineMember( name, type );
	UpdateVariable( name, value );
}

_void VariableOperation::DefineMember( StringPtr name, _dword type, _float value )
{
	DefineMember( name, type );
	UpdateVariable( name, value );
}

_void VariableOperation::DefineMember( StringPtr name, _dword type, StringPtr string )
{
	DefineMember( name, type );
	UpdateVariable( name, string );
}

_void VariableOperation::CreateElement( StringPtr name, _dword number )
{
	// AllocaterSwitcher switcher( &mAllocater );

	_dword position = -1;

	// Try to replace existing append operation.
	for ( _dword i = mRedOptNumber + mDefOptNumber; i < mRedOptNumber + mDefOptNumber + mApdOptNumber; i ++ )
	{
		if ( mOperationArray[i].mObject2 == name )
		{
			mOperationArray[i].mObject3 = Long( number ).ToString( );

			return;
		}

		// Sub element must create before parent, so we adjust the order.
		if ( name.SearchL2R( mOperationArray[i].mObject2 ) == 0 )
			position = i;
	}

	Operation apdopt( _OPERATION_CREATE_ELEMENT, String( name ), Long( number ).ToString( ) );

	_dword insertindex = mRedOptNumber + mDefOptNumber + mApdOptNumber; mApdOptNumber ++;

	mOperationArray.Insert( apdopt, position == -1 ? insertindex : position );
}

_void VariableOperation::DeleteElement( StringPtr name, _dword index )
{
	// AllocaterSwitcher switcher( &mAllocater );

	// Try to replace existing append operation.
	for ( _dword i = mRedOptNumber + mDefOptNumber + mApdOptNumber; i < mRedOptNumber + mDefOptNumber + mApdOptNumber + mDelOptNumber; i ++ )
	{
		if ( mOperationArray[i].mObject2 == name )
		{
			mOperationArray[i].mObject3 = Long( index ).ToString( );

			return;
		}
	}

	Operation delopt( _OPERATION_DELETE_ELEMENT, String( name ), Long( index ).ToString( ) );

	_dword insertindex = mRedOptNumber + mDefOptNumber + mApdOptNumber + mDelOptNumber; mDelOptNumber ++;

	mOperationArray.Insert( delopt, insertindex );
}

_void VariableOperation::UpdateVariable( StringPtr name, _char value )
{
	_char buffer[1024];
	StringFormatter::FormatBuffer( buffer, 1024, L"CHR:%c", value );

	AppendUpdateOperation( name, buffer );
}

_void VariableOperation::UpdateVariable( StringPtr name, _tiny value )
{
	_char buffer[1024];
	StringFormatter::FormatBuffer( buffer, 1024, L"TNY:%d", value );

	AppendUpdateOperation( name, buffer );
}

_void VariableOperation::UpdateVariable( StringPtr name, _short value )
{
	_char buffer[1024];
	StringFormatter::FormatBuffer( buffer, 1024, L"SHT:%d", value );

	AppendUpdateOperation( name, buffer );
}

_void VariableOperation::UpdateVariable( StringPtr name, _long value )
{
	_char buffer[1024];
	StringFormatter::FormatBuffer( buffer, 1024, L"LNG:%d", value );

	AppendUpdateOperation( name, buffer );
}

_void VariableOperation::UpdateVariable( StringPtr name, _byte value )
{
	_char buffer[1024];
	StringFormatter::FormatBuffer( buffer, 1024, L"BYT:%d", value );

	AppendUpdateOperation( name, buffer );
}

_void VariableOperation::UpdateVariable( StringPtr name, _word value )
{
	_char buffer[1024];
	StringFormatter::FormatBuffer( buffer, 1024, L"WOD:%d", value );

	AppendUpdateOperation( name, buffer );
}

_void VariableOperation::UpdateVariable( StringPtr name, _dword value )
{
	_char buffer[1024];
	StringFormatter::FormatBuffer( buffer, 1024, L"DWD:%d", value );

	AppendUpdateOperation( name, buffer );
}

_void VariableOperation::UpdateVariable( StringPtr name, _float value )
{
	_char buffer[1024];
	StringFormatter::FormatBuffer( buffer, 1024, L"FLT:%4.2f", value );

	AppendUpdateOperation( name, buffer );
}

_void VariableOperation::UpdateVariable( StringPtr name, StringPtr string )
{
	_char buffer[1024];
	StringFormatter::FormatBuffer( buffer, 1024, L"STR:%s", string );

	AppendUpdateOperation( name, buffer );
}

_void VariableOperation::UpdateVariableWithoutType( StringPtr name, StringPtr variable )
{
	AppendUpdateOperation( name, variable );
}

_void VariableOperation::QueryVariable( StringPtr name, StringPtr filter )
{
	// AllocaterSwitcher switcher( &mAllocater );

	mQueOptNumber ++;

	mOperationArray.Append( Operation( _OPERATION_QUERY_VARIABLE, String( name ), String( filter ) ) );
}

_bool VariableOperation::Operate( Variable& variable ) const
{
	for ( _dword i = 0; i < mOperationArray.Number( ); i ++ )
	{
		const Operation& operation = mOperationArray[i];

		switch ( operation.mObject1 )
		{
			case _OPERATION_REDECLARE:
			{
				variable.Redeclare( operation.mObject2, Variable::String2Type( operation.mObject3 ) );

				break;
			}

			case _OPERATION_DEFINE_MEMBER:
			{
				_char buffer1[1024], *buffer2 = L"";
				GetVariableType( buffer1, buffer2, operation.mObject3 );

				StringPtr type = buffer1, value = buffer2;

				// Auto change the variable to struct if it is empty.
				if ( variable.GetType( ) == 0 )
					variable.Redeclare( L"", Variable::_TYPE_STRUCT );

				// Create an new variable member.
				Variable& newvariable = variable.DefineMember( operation.mObject2, Variable::String2Type( type ) );

				if ( newvariable.IsValid( ) == _false )
					return _false;

				// Update the variable if initial value is given.
				if ( value != L"" )
				{
					if ( type == L"CHR" )		newvariable = value[0];
					else if ( type == L"TNY" )	newvariable = (_tiny) Tiny( value );
					else if ( type == L"SHT" )	newvariable = (_short) Short( value );
					else if ( type == L"LNG" )	newvariable = (_long) Long( value );
					else if ( type == L"BYT" )	newvariable = (_byte) Tiny( value );
					else if ( type == L"WOD" )	newvariable = (_word) Short( value );
					else if ( type == L"DWD" )	newvariable = (_dword) Long( value );
					else if ( type == L"FLT" )	newvariable = (_float) Float( value );
					else if ( type == L"STR" )	newvariable = value;
				}

				break;
			}

			case _OPERATION_CREATE_ELEMENT:
			{
				_dword number = Long( operation.mObject3 );

				if ( number == 0 )
					break;

				if ( variable.CreateElement( operation.mObject2, number ).IsValid( ) == _false )
					return _false;

				break;
			}

			case _OPERATION_DELETE_ELEMENT:
			{
				_dword index = Long( operation.mObject3 );

				if ( index == -1 )
					break;

				if ( variable.DeleteElement( operation.mObject2, index ) == _false )
					return _false;

				break;
			}

			case _OPERATION_UPDATE_VARIABLE:
			{
				_char buffer1[1024], *buffer2 = L"";
				if ( GetVariableType( buffer1, buffer2, operation.mObject3 ) == _false )
					return _false;

				// Parse variable type to update it.
				StringPtr type = buffer1, value = buffer2;

				// Get the variable to be updated by name.
				Variable& updvariable = variable[ operation.mObject2 ];

				if ( updvariable.IsValid( ) == _false )
					return _false;

				if ( type == L"CHR" )		updvariable = value[0];
				else if ( type == L"TNY" )	updvariable = (_tiny) Tiny( value );
				else if ( type == L"SHT" )	updvariable = (_short) Short( value );
				else if ( type == L"LNG" )	updvariable = (_long) Long( value );
				else if ( type == L"BYT" )	updvariable = (_byte) Tiny( value );
				else if ( type == L"WOD" )	updvariable = (_word) Short( value );
				else if ( type == L"DWD" )	updvariable = (_dword) Long( value );
				else if ( type == L"FLT" )	updvariable = (_float) Float( value );
				else if ( type == L"STR" )	updvariable = value;

				break;
			}
		}
	}

	return _true;
}

_bool VariableOperation::Query( Variable& variable, VariableOperation& result ) const
{
	for ( _dword i = 0; i < mOperationArray.Number( ); i ++ )
	{
		const Operation& operation = mOperationArray[i];

		if ( operation.mObject1 != _OPERATION_QUERY_VARIABLE )
			continue;

		const Variable& subvariable = variable[ operation.mObject2 ];

		if ( subvariable.IsValid( ) == _false )
			return _false;

		result.DefineMember( operation.mObject2, subvariable.GetType( ) );
		VariableSaver::SaveVariable( operation.mObject2, operation.mObject3, subvariable, result );
	}

	return result.GetOperationNumber( ) > 0;
}

_bool VariableOperation::FromBuffer( const _byte* buffer, _dword length )
{
	// AllocaterSwitcher switcher( &mAllocater );

	// Clear current operation array.
	mOperationArray.Clear( );

	TxtFile varfile;

	varfile.Open( buffer, length, _false );
	varfile.EnableAutoDeleteBuffer( _false );

	Compression::Decompress( varfile );

	// Read each line in variable file, and build operations.
	while ( varfile.ReachEnd( ) == _false )
	{
		String line;
		varfile.ReadLine( line );

		Array< String > keywords;
		StringFormatter::SplitString( line, keywords, L"|" );

		if ( keywords.Number( ) == 3 )
		{
			if ( keywords[0] == L"USD" )
			{
				mUserData1 = Dword( keywords[1] );
				mUserData2 = Dword( keywords[2] );
			}
			else if ( keywords[0] == L"USDEX" )
			{
				mUserData3 = Dword( keywords[1] );
				mUserData4 = Dword( keywords[2] );
			}
			else
			{
				_dword type = String2Type( keywords[0] );

				switch ( type )
				{
					case _OPERATION_REDECLARE:			mRedOptNumber ++; break;
					case _OPERATION_DEFINE_MEMBER:		mDefOptNumber ++; break;
					case _OPERATION_CREATE_ELEMENT:		mApdOptNumber ++; break;
					case _OPERATION_DELETE_ELEMENT:		mDelOptNumber ++; break;
					case _OPERATION_UPDATE_VARIABLE:	mUpdOptNumber ++; break;
				}
	
				mOperationArray.Append( Operation( type, keywords[1], keywords[2] ) );
			}
		}
		else if ( keywords.Number( ) == 2 )
		{
			_dword type = String2Type( keywords[0] );

			if ( type == _OPERATION_QUERY_VARIABLE )
			{
				mOperationArray.Append( Operation( type, keywords[1], L"" ) );

				mQueOptNumber ++;
			}
		}
	}

	return mOperationArray.Number( ) > 0;
}

_bool VariableOperation::ToBinBuffer( MemFile& buffer ) const
{
	if ( ToTxtBuffer( buffer ) == _false )
		return _false;

	Compression::Compress( buffer );

	return _true;
}

_bool VariableOperation::ToTxtBuffer( MemFile& buffer ) const
{
	// AllocaterSwitcher switcher( &mAllocater );

	Array< String > optarray; _dword totalsize = 2;

	if ( mUserData1 != 0 || mUserData2 != 0 )
	{
		String newopt;
		newopt.Format( L"USD|%X|%X\r\n", mUserData1, mUserData2 );

		totalsize += newopt.Length( ) * sizeof( _char );

		optarray.Append( newopt );
	}

	if ( mUserData3 != 0 || mUserData4 != 0 )
	{
		String newopt;
		newopt.Format( L"USDEX|%X|%X\r\n", mUserData3, mUserData4 );

		totalsize += newopt.Length( ) * sizeof( _char );

		optarray.Append( newopt );
	}

	for ( _dword i = 0; i < GetOperationNumber( ); i ++ )
	{
		String newopt;
		newopt.Format( L"%s|%s|%s\r\n", Type2String( GetOperationType( i ) ), GetOperationParam1( i ), GetOperationParam2( i ) );

		totalsize += newopt.Length( ) * sizeof( _char );

		optarray.Append( newopt );
	}

	if ( optarray.Number( ) == 0 )
		return _false;

	BinFile& varfile = (BinFile&) buffer;

	//// Switcher memory managed of out this function, so we turn of memory allocater.
	//switcher.TurnOff( );

	// Create enough buffer for this operation.
	varfile.Create( totalsize );

	//// Now turn back memory allocater.
	//switcher.TurnOn( );

	// Write unicode flag.
	varfile.WriteWord( 0xFEFF );

	// Write all operations.
	for ( _dword i = 0; i < optarray.Number( ); i ++ )
		varfile.WriteBuffer( (const _char*) optarray[i], optarray[i].Length( ) * sizeof( _char ) );

	return _true;
}

_void VariableOperation::Clear( )
{
	// AllocaterSwitcher switcher( &mAllocater );

	mOperationArray.Clear( _true );

	mUserData1		= 0;
	mUserData2		= 0;
	mUserData3		= 0;
	mUserData4		= 0;
	mRedOptNumber	= 0;
	mDefOptNumber	= 0;
	mApdOptNumber	= 0;
	mDelOptNumber	= 0;
	mUpdOptNumber	= 0;
	mQueOptNumber	= 0;
}

//----------------------------------------------------------------------------
// VariableSaver Implementation
//----------------------------------------------------------------------------

_bool VariableSaver::SaveVariable( StringPtr prefix, StringPtr filter, const Variable& variable, VariableOperation& operation, _bool subelement )
{
	_char fullname[1024];

	const Variable* structvariable = _null;

	if ( ( variable.GetType( ) & Variable::_TYPE_MASK ) == Variable::_TYPE_STRUCT )
	{
		structvariable = variable.GetTemplate( );

		if ( structvariable == _null )
			structvariable = &variable;
	}

	// Save declaration for struct.
	if ( structvariable != _null )
	{
		for ( _dword i = 0; i < structvariable->GetElementNumber( ); i ++ )
		{
			const Variable* element = structvariable->GetElement( i );
			FL_ASSERT( element != _null )

			if ( filter != L"" && filter != element->GetName( ) )
				continue;

			if ( prefix == L"" )
				StringFormatter::FormatBuffer( fullname, 1024, L"%s", element->GetName( ) );
			else
				StringFormatter::FormatBuffer( fullname, 1024, L"%s.%s", prefix, element->GetName( ) );

			// Append create operation for this member.
			if ( subelement == _false )
				operation.DefineMember( fullname, element->GetType( ) );

			// Save declaration for elements too, with name prefix.
			SaveVariable( fullname, L"", *element, operation, subelement );
		}
	}

	// Save elements for array.
	if ( ( variable.GetType( ) & ~Variable::_TYPE_MASK ) == Variable::_TYPE_ARRAY )
	{
		if ( subelement == _false && variable.GetElementNumber( ) > 0 )
			operation.CreateElement( prefix, variable.GetElementNumber( ) );

		// Save variable value for elements.
		for ( _dword i = 0; i < variable.GetElementNumber( ); i ++ )
		{
			const Variable* element = variable.GetElement( i );
			FL_ASSERT( element != _null )

			// Build full name for array elements.
			StringFormatter::FormatBuffer( fullname, 1024, L"%s%s", prefix, element->GetName( ) );

			// Save variable value for elements.
			SaveVariable( fullname, filter, *element, operation, _true );
		}
	}
	// Save variable value for elements.
	else if ( variable.IsInitial( ) == _false )
	{
		switch ( variable.GetType( ) )
		{
			case Variable::_TYPE_CHAR:		operation.UpdateVariable( prefix, (_char) variable ); break;
			case Variable::_TYPE_TINY:		operation.UpdateVariable( prefix, (_tiny) variable ); break;
			case Variable::_TYPE_SHORT:		operation.UpdateVariable( prefix, (_short) variable ); break;
			case Variable::_TYPE_LONG:		operation.UpdateVariable( prefix, (_long) variable ); break;
			case Variable::_TYPE_BYTE:		operation.UpdateVariable( prefix, (_byte) variable ); break;
			case Variable::_TYPE_WORD:		operation.UpdateVariable( prefix, (_word) variable ); break;
			case Variable::_TYPE_DWORD:		operation.UpdateVariable( prefix, (_dword) variable ); break;
			case Variable::_TYPE_FLOAT:		operation.UpdateVariable( prefix, (_float) variable ); break;
			case Variable::_TYPE_STRING:	operation.UpdateVariable( prefix, (const _char*) variable ); break;
		}
	}

	return _true;
}

_bool VariableSaver::SaveVariable( StringPtr filename, const Variable& variable, _bool bin )
{
	VariableOperation operation;

	// First create a redeclare operation.
	operation.Redeclare( variable.GetName( ), variable.GetType( ) );

	SaveVariable( L"", L"", variable, operation );

	MemFile varfile;

	if ( bin )
	{
		if ( operation.ToBinBuffer( varfile ) == _false )
			return _false;
	}
	else
	{
		if ( operation.ToTxtBuffer( varfile ) == _false )
			return _false;
	}

	return FileSystem::Create( filename, varfile.GetBuffer( ), varfile.GetLength( ) );
}

//----------------------------------------------------------------------------
// VariableLoader Implementation
//----------------------------------------------------------------------------

_bool VariableLoader::LoadVariable( StringPtr filename, Variable& variable )
{
	MemFile varfile;

	if ( varfile.Open( filename ) == _false )
		return _false;

	VariableOperation operation;

	if ( operation.FromBuffer( varfile.GetBuffer( ), varfile.GetLength( ) ) == _false )
		return _false;

	return operation.Operate( variable );
}
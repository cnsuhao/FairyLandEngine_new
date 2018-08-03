//============================================================================
// TinyVariable.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// TinyVariable Implementation
//----------------------------------------------------------------------------

const TinyVariable TinyVariable::cNull;
const TinyVariable TinyVariable::cTrue	= _true;
const TinyVariable TinyVariable::cFalse = _false;

_void TinyVariable::CopyString( const _char* string, _dword stringlength )
{
	_dword stringlength_plus_1 = stringlength + 1;

	// It's string to string copy
	if ( mType == _TYPE_ID_STRING )
	{
		// Check if it has enough space to store this string 
		if ( Size( ) >= stringlength_plus_1 )
		{
			// Clear string length
			mHighValue &= 0xFFFF0000;

			if ( string == L"" )
			{
				*( (_char*) mLowValue ) = 0;
			}
			else
			{
				Memory::MemCpy( (_char*) mLowValue, string, stringlength_plus_1 * sizeof( _char ) );

				mHighValue |= stringlength;
			}

			return;
		}

		// Delete old buffer
		delete[] (_char*) mLowValue;
	}

	// If it doesn't contain any characters then convert it to "StringPtr"
	if ( string == _null || *string == 0 || stringlength_plus_1 > 0xFFFF )
	{
		mValue	= 0;
		mType	= _TYPE_ID_UNKNOWN;

		return;
	}

	// Create string buffer
	_char* buffer = new _char[ stringlength_plus_1 ];

	// Copy string
	Memory::MemCpy( buffer, string, stringlength_plus_1 * sizeof( _char ) );

	// Set to "String" type
	mType = _TYPE_ID_STRING;

	// Update value
	mLowValue	= _dword( buffer );
	mHighValue	= stringlength | ( ( stringlength + 1 ) << 16 );
}

_void TinyVariable::AppendString( const _char* string, _dword stringlength )
{
	_dword stringlength_plus_1 = GetStringLength( ) + stringlength + 1;

	// Has enough space to store string
	if ( Size( ) >= stringlength_plus_1 )
	{
		// Update string length
		mHighValue += stringlength;

		// Append string
		StringFormatter::AppendString( (_char*) mValue, string );
	}
	else
	{
		// Copy string
		_char buffer[ 2048 ];
		StringFormatter::CopyString( buffer, (const _char*) mValue, 2047 );

		// Append string
		StringFormatter::AppendString( buffer, string );

		// Copy new string
		CopyString( buffer, GetStringLength( ) + stringlength );
	}
}
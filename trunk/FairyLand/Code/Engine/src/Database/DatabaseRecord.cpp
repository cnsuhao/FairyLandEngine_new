//============================================================================
// DatabaseRecord.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandDatabase.h"

//----------------------------------------------------------------------------
// DatabaseRecord Implementation
//----------------------------------------------------------------------------

DatabaseRecord::DatabaseRecord( _handle statement, _bool randomaccess )
	: mSQLStatement( statement ), mRandomAccess( randomaccess ), mColumnNumber( 0 )
{
	::SQLNumResultCols( statement, (SQLSMALLINT*) &mColumnNumber );

	FL_ASSERT( mColumnNumber <= _MAX_COLUMN_NUMBER )

	for ( _dword i = 0; i < mColumnNumber; i ++ )
		mColumnDesc[i].mName[0] = 0;
}

DatabaseRecord::~DatabaseRecord( )
{
}

_bool DatabaseRecord::GetColumnHelper( _dword index )
{
	RecordColumnDesc* columndesc = &mColumnDesc[ index ];

	if ( columndesc->mName[0] != 0 )
		return _true;

	_dword namelength = 0, datatype = 0, decimaldigits = 0;

	// Get column from record.
	::SQLDescribeCol( mSQLStatement, (SQLSMALLINT) index + 1, columndesc->mName, RecordColumnDesc::_MAX_STRING_LENGTH,
		(SQLSMALLINT*) &namelength, (SQLSMALLINT*) &datatype, &columndesc->mSize, (SQLSMALLINT*) &decimaldigits, (SQLSMALLINT*) &columndesc->mNullable );

	if ( namelength == 0 )
		return _null;

	switch ( datatype )
	{
		case SQL_CHAR:			datatype = RecordColumnDesc::_TYPE_CHAR; break;
		case SQL_NUMERIC:		datatype = RecordColumnDesc::_TYPE_NUMERIC; break;
		case SQL_DECIMAL:		datatype = RecordColumnDesc::_TYPE_DECIMAL; break;
		case SQL_INTEGER:		datatype = RecordColumnDesc::_TYPE_INTEGER; break;
		case SQL_SMALLINT:		datatype = RecordColumnDesc::_TYPE_SMALLINT; break;
		case SQL_FLOAT:			datatype = RecordColumnDesc::_TYPE_FLOAT; break;
		case SQL_REAL:			datatype = RecordColumnDesc::_TYPE_REAL; break;
		case SQL_DOUBLE:		datatype = RecordColumnDesc::_TYPE_DOUBLE; break;
		case SQL_DATETIME:		datatype = RecordColumnDesc::_TYPE_DATETIME; break;
		case SQL_TIME:			datatype = RecordColumnDesc::_TYPE_TIME; break;
		case SQL_TIMESTAMP:		datatype = RecordColumnDesc::_TYPE_TIMESTAMP; break;
		case SQL_VARCHAR:		datatype = RecordColumnDesc::_TYPE_VARCHAR; break;
		case SQL_LONGVARCHAR:	datatype = RecordColumnDesc::_TYPE_LONGVARCHAR; break;
		case SQL_BINARY:		datatype = RecordColumnDesc::_TYPE_BINARY; break;
		case SQL_VARBINARY:		datatype = RecordColumnDesc::_TYPE_VARBINARY; break;
		case SQL_LONGVARBINARY:	datatype = RecordColumnDesc::_TYPE_LONGVARBINARY; break;
		case SQL_BIGINT:		datatype = RecordColumnDesc::_TYPE_BIGINT; break;
		case SQL_TINYINT:		datatype = RecordColumnDesc::_TYPE_TINYINT; break;
		case SQL_BIT:			datatype = RecordColumnDesc::_TYPE_BINARY; break;
	}

	columndesc->mType = datatype;

	return columndesc->mName[0] != 0;
}

_bool DatabaseRecord::GetValueHelper( _dword index, _dword type, _void* buffer, _dword length, _dword* outlength, _bool* nullvalue )
{
	if ( outlength != _null )
		*outlength = 0;

	if ( nullvalue != _null )
		*nullvalue = _false;

	// We need random access, so we must scroll fetch cursor.
	if ( mRandomAccess )
		::SQLFetchScroll( mSQLStatement, SQL_FETCH_RELATIVE, 0 );

	_dword tempout = 0;
	if ( SQL_SUCCEEDED( ::SQLGetData( mSQLStatement, (SQLUSMALLINT) index + 1, (SQLSMALLINT) type, buffer, length, (SQLINTEGER*) &tempout ) ) == _false )
	{
		if ( nullvalue != _null )
			*nullvalue = _true;
	}
	else
	{
		if ( tempout > length )
		{
			if ( nullvalue != _null )
				*nullvalue = _true;

			tempout = 0;
		}

		if ( outlength != _null )
			*outlength = tempout;
	}

	return _true;
}

_bool DatabaseRecord::MoveFirst( )
{
	FL_ASSERT( mRandomAccess )

	return SQL_SUCCEEDED( ::SQLFetchScroll( mSQLStatement, SQL_FETCH_FIRST, 0 ) );
}

_bool DatabaseRecord::MoveLast( )
{
	return SQL_SUCCEEDED( ::SQLFetchScroll( mSQLStatement, SQL_FETCH_LAST, 0 ) );
}

_bool DatabaseRecord::MovePrior( )
{
	FL_ASSERT( mRandomAccess )

	return SQL_SUCCEEDED( ::SQLFetchScroll( mSQLStatement, SQL_FETCH_PRIOR, 0 ) );
}

_bool DatabaseRecord::MoveNext( )
{
	return SQL_SUCCEEDED( ::SQLFetchScroll( mSQLStatement, SQL_FETCH_NEXT, 0 ) );
}

_dword DatabaseRecord::GetColumnNumber( )
{
	return mColumnNumber;
}

_dword DatabaseRecord::GetColumnIndex( StringPtr filed )
{
	_bool needgetcolumn = _false;

	// Get in existing column desc.
	for ( _dword i = 0; i < mColumnNumber; i ++ )
	{
		if ( mColumnDesc[i].mName[0] == 0 )
		{
			needgetcolumn = _true;
			continue;
		}

		if ( filed.CompareLowercase( mColumnDesc[i].mName ) )
			return i;
	}

	// There is some column desc need to get.
	if ( needgetcolumn )
	{
		for ( _dword i = 0; i < mColumnNumber; i ++ )
		{
			if ( mColumnDesc[i].mName[0] != 0 )
				continue;

			// Get this column desc.
			if ( GetColumnHelper( i ) == _false )
				continue;

			if ( filed.CompareLowercase( mColumnDesc[i].mName ) )
				return i;
		}
	}

	return -1;
}

RecordColumnDesc* DatabaseRecord::GetColumnDesc( _dword index )
{
	if ( index >= mColumnNumber )
		return _null;

	if ( GetColumnHelper( index ) == _null )
		return _null;

	return &mColumnDesc[ index ];
}

_tiny DatabaseRecord::GetTiny( _dword index, _bool* nullvalue )
{
	_tiny value = 0;
	PS_VERIFY( GetValueHelper( index, SQL_C_STINYINT, &value, sizeof( value ), _null, nullvalue ) );

	return value;
}

_short DatabaseRecord::GetShort( _dword index, _bool* nullvalue )
{
	_short value = 0;
	PS_VERIFY( GetValueHelper( index, SQL_C_SSHORT, &value, sizeof( value ), _null, nullvalue ) );

	return value;
}

_long DatabaseRecord::GetLong( _dword index, _bool* nullvalue )
{
	_long value = 0;
	PS_VERIFY( GetValueHelper( index, SQL_C_SLONG, &value, sizeof( value ), _null, nullvalue ) );

	return value;
}

_byte DatabaseRecord::GetByte( _dword index, _bool* nullvalue )
{
	_long value = 0;
	PS_VERIFY( GetValueHelper( index, SQL_C_UTINYINT, &value, sizeof( value ), _null, nullvalue ) );

	return value;
}

_word DatabaseRecord::GetWord( _dword index, _bool* nullvalue )
{
	_long value = 0;
	PS_VERIFY( GetValueHelper( index, SQL_C_USHORT, &value, sizeof( value ), _null, nullvalue ) );

	return value;
}

_dword DatabaseRecord::GetDword( _dword index, _bool* nullvalue )
{
	_long value = 0;
	PS_VERIFY( GetValueHelper( index, SQL_C_ULONG, &value, sizeof( value ), _null, nullvalue ) );

	return value;
}

String DatabaseRecord::GetString( _dword index, _bool* nullvalue )
{
	_char buffer[1024]; _dword outlength = 0;
	PS_VERIFY( GetValueHelper( index, SQL_WCHAR, buffer, sizeof( buffer ), &outlength, nullvalue ) );

	buffer[ outlength ] = 0;

	return String( buffer );
}

Time DatabaseRecord::GetTime( _dword index, _bool* nullvalue )
{
	SQL_TIMESTAMP_STRUCT timestamp = { 0 };
	PS_VERIFY( GetValueHelper( index, SQL_C_TIMESTAMP, &timestamp, sizeof( timestamp ), _null, nullvalue ) );

	Time time;
	time.mYear			= timestamp.year;
	time.mMonth			= timestamp.month;
	time.mDay			= timestamp.day;
	time.mHour			= timestamp.hour;
	time.mMinute		= timestamp.minute;
	time.mSecond		= timestamp.second;
	time.mMilliseconds	= (_word) timestamp.fraction;

	return time;
}

_dword DatabaseRecord::GetCharBuffer( _dword index, _char* buffer, _dword length, _bool* nullvalue )
{
	_dword outlength = 0;
	PS_VERIFY( GetValueHelper( index, SQL_WCHAR, buffer, length, &outlength, nullvalue ) );

	return outlength;
}

_dword DatabaseRecord::GetBinaryBuffer( _dword index, _byte* buffer, _dword length, _bool* nullvalue )
{
	_dword outlength = 0;
	PS_VERIFY( GetValueHelper( index, SQL_C_BINARY, buffer, length, &outlength, nullvalue ) );

	return outlength;
}
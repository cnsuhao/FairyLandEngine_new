//============================================================================
// IDatabaseRecord.h
//
// Copyright 2006-2007 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

struct RecordColumnDesc
{
	enum { _MAX_STRING_LENGTH = 128 };

	enum _COLUMN_TYPE
	{
		_TYPE_CHAR			=  1,
		_TYPE_NUMERIC		=  2,
		_TYPE_DECIMAL		=  3,
		_TYPE_INTEGER		=  4,
		_TYPE_SMALLINT		=  5,
		_TYPE_FLOAT			=  6,
		_TYPE_REAL			=  7,
		_TYPE_DOUBLE		=  8,
		_TYPE_DATETIME		=  9,
		_TYPE_TIME			= 10,
		_TYPE_TIMESTAMP		= 11,
		_TYPE_VARCHAR		= 12,
		_TYPE_LONGVARCHAR	= 13,
		_TYPE_BINARY		= 14,
		_TYPE_VARBINARY		= 15,
		_TYPE_LONGVARBINARY	= 16,
		_TYPE_BIGINT		= 17,
		_TYPE_TINYINT		= 18,
		_TYPE_BIT			= 19,
	};

	_char		mName[ _MAX_STRING_LENGTH ];
	_dword		mType;
	_dword		mSize;
	_dword		mDecimalDigits;
	_bool		mNullable;
};

//----------------------------------------------------------------------------
// IDatabaseRecord
//----------------------------------------------------------------------------

class IDatabaseRecord
{
public:
	virtual _bool	MoveFirst( )																			= 0;
	virtual _bool	MoveLast( )																				= 0;

	virtual _bool	MovePrior( )																			= 0;
	virtual _bool	MoveNext( )																				= 0;

	virtual _dword	GetColumnNumber( )																		= 0;
	virtual _dword	GetColumnIndex( StringPtr filed )														= 0;
	virtual RecordColumnDesc* GetColumnDesc( _dword index )													= 0;

	virtual _tiny	GetTiny( _dword index, _bool* nullvalue = _null )										= 0;
	virtual _short	GetShort( _dword index, _bool* nullvalue = _null )										= 0;
	virtual _long	GetLong( _dword index, _bool* nullvalue = _null )										= 0;
	virtual _byte	GetByte( _dword index, _bool* nullvalue = _null )										= 0;
	virtual _word	GetWord( _dword index, _bool* nullvalue = _null )										= 0;
	virtual _dword	GetDword( _dword index, _bool* nullvalue = _null )										= 0;

	virtual String	GetString( _dword index, _bool* nullvalue = _null )										= 0;
	virtual Time	GetTime( _dword index, _bool* nullvalue = _null )										= 0;

	virtual _dword	GetCharBuffer( _dword index, _char* buffer, _dword length, _bool* nullvalue = _null )	= 0;
	virtual _dword	GetBinaryBuffer( _dword index, _byte* buffer, _dword length, _bool* nullvalue = _null )	= 0;
};

};
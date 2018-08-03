//============================================================================
// DatabaseRecord.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// DatabaseRecord
//----------------------------------------------------------------------------

class DatabaseRecord : public IDatabaseRecord
{
private:
	enum { _MAX_COLUMN_NUMBER = 64 };

	_handle				mSQLStatement;
	_bool				mRandomAccess;
	_dword				mColumnNumber;
	RecordColumnDesc	mColumnDesc[ _MAX_COLUMN_NUMBER ];

	_bool GetColumnHelper( _dword index );
	_bool GetValueHelper( _dword index, _dword type, _void* buffer, _dword length, _dword* outlength, _bool* nullvalue );

public:
	DatabaseRecord( _handle statement, _bool randomaccess );
	~DatabaseRecord( );

	virtual _bool	MoveFirst( );
	virtual _bool	MoveLast( );

	virtual _bool	MovePrior( );
	virtual _bool	MoveNext( );

	virtual _dword	GetColumnNumber( );
	virtual _dword	GetColumnIndex( StringPtr filed );
	virtual RecordColumnDesc* GetColumnDesc( _dword index );

	virtual _tiny	GetTiny( _dword index, _bool* nullvalue );
	virtual _short	GetShort( _dword index, _bool* nullvalue );
	virtual _long	GetLong( _dword index, _bool* nullvalue );
	virtual _byte	GetByte( _dword index, _bool* nullvalue );
	virtual _word	GetWord( _dword index, _bool* nullvalue );
	virtual _dword	GetDword( _dword index, _bool* nullvalue );

	virtual String	GetString( _dword index, _bool* nullvalue );
	virtual Time	GetTime( _dword index, _bool* nullvalue );

	virtual _dword	GetCharBuffer( _dword index, _char* buffer, _dword length, _bool* nullvalue );
	virtual _dword	GetBinaryBuffer( _dword index, _byte* buffer, _dword length, _bool* nullvalue );
};

};
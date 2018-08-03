//============================================================================
// IDatabaseConnection.h
//
// Copyright 2006-2007 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// IDatabaseConnection
//----------------------------------------------------------------------------

class IDatabaseConnection
{
public:
	virtual _bool Connect( StringPtr connectionstring )								= 0;
	virtual _bool Connect( StringPtr dsn, StringPtr username, StringPtr password )	= 0;
	virtual _bool Disconnect( )														= 0;
	virtual _bool Reconnect( _bool alwaysretry = _false )							= 0;

	virtual _bool Execute( const _char* sql, ... )									= 0;
	virtual _bool ExecuteAsync( const _char* sql, ... )								= 0;
	virtual _void FlushAsyncCommand( )												= 0;

	virtual IDatabaseRecord* Query( _bool randomaccess, const _char* sql, ... )		= 0;
	virtual IDatabaseRecord* QueryAndFetch( const _char* sql, ... )					= 0;
	virtual _void ReleaseDatabaseRecord( IDatabaseRecord*& record )					= 0;

	virtual _bool IsConnected( ) const												= 0;

	// Error
	virtual String GetLastConnectionError( )										= 0;
	virtual String GetLastCommandError( )											= 0;
};

};
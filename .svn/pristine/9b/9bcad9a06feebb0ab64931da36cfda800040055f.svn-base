//============================================================================
// DatabaseConnection.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// DatabaseConnection
//----------------------------------------------------------------------------

class DatabaseConnection : public IDatabaseConnection
{
private:
	typedef Queue< String > StringQueue;

	String		mConnectionString;
	String		mConnectionDSN;
	String		mConnectionUsername;
	String		mConnectionPassword;

	_handle		mSQLConnection;
	_handle		mSQLStatement;
	_dword		mRecordNumber;

	Lock		mDeferCommandLock;
	Thread		mDeferCommandThread;
	StringQueue	mDeferCommandQueue;

	static _bool OnProcessDeferCommand( _void* parameter1, _void* parameter2 );

	_bool IsConnectionLost( );
	_bool ExecuteHelper( StringPtr sqlcommand, _bool clearresult );

public:
	DatabaseConnection( _handle connection );
	~DatabaseConnection( );

	virtual _bool Connect( StringPtr connectionstring );
	virtual _bool Connect( StringPtr dsn, StringPtr username, StringPtr password );
	virtual _bool Disconnect( );
	virtual _bool Reconnect( _bool alwaysretry );

	virtual _bool Execute( const _char* sql, ... );
	virtual _bool ExecuteAsync( const _char* sql, ... );
	virtual _void FlushAsyncCommand( );

	virtual IDatabaseRecord* Query( _bool randomaccess, const _char* sql, ... );
	virtual IDatabaseRecord* QueryAndFetch( const _char* sql, ... );
	virtual _void ReleaseDatabaseRecord( IDatabaseRecord*& record );

	virtual _bool IsConnected( ) const;

	// Error
	virtual String GetLastConnectionError( );
	virtual String GetLastCommandError( );
};

};
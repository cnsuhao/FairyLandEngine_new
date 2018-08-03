//============================================================================
// DatabaseConnection.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandDatabase.h"

//----------------------------------------------------------------------------
// DatabaseConnection Implementation
//----------------------------------------------------------------------------

DatabaseConnection::DatabaseConnection( _handle connection )
	: mSQLConnection( connection ), mSQLStatement( _null ), mRecordNumber( 0 )
{
}

DatabaseConnection::~DatabaseConnection( )
{
	FlushAsyncCommand( );

	FL_ASSERT( mRecordNumber == 0 )

	if ( mSQLStatement != _null )
	{
		::SQLFreeHandle( SQL_HANDLE_STMT, mSQLStatement );
		::SQLDisconnect( mSQLConnection );
	}

	::SQLFreeHandle( SQL_HANDLE_DBC, mSQLConnection );
}

_bool DatabaseConnection::OnProcessDeferCommand( _void* parameter1, _void* parameter2 )
{
	DatabaseConnection* connection = (DatabaseConnection*) parameter1;
	FL_ASSERT( connection != _null )

	connection->FlushAsyncCommand( );

	return _true;
}

_bool DatabaseConnection::IsConnectionLost( )
{
	_char sqlstate[6], error[1024]; _short size = 0;
	::SQLGetDiagRec( (_short) SQL_HANDLE_STMT, mSQLStatement, 1, sqlstate, _null, error, 1024, &size );

	if ( size == 0 )
		return _false;

	return StringPtr( sqlstate ) == L"01000";
}

_bool DatabaseConnection::ExecuteHelper( StringPtr sqlcommand, _bool clearresult )
{
	_long result = ::SQLExecDirect( mSQLStatement, (SQLWCHAR*) (const _char*) sqlcommand, SQL_NTS );

	if ( SQL_SUCCEEDED( result ) == _false )
	{
		if ( result == SQL_NO_DATA )
			return _true;

		// Connection lost, we try to reconnect.
		if ( IsConnectionLost( ) )
		{
			Reconnect( _true );

			return ExecuteHelper( sqlcommand, _true );
		}

		PS_ERROR1( L"[DB] Execute SQL command error : %s\r\n", (const _char*) GetLastCommandError( ) );

		return _false;
	}

	if ( clearresult )
	{
		while ( SQL_SUCCEEDED( ::SQLMoreResults( mSQLStatement ) ) );
	}

	return _true;
}

_bool DatabaseConnection::Connect( StringPtr connectionstring )
{
	_char buffer[1024]; _dword length = 0;

	if ( SQL_SUCCEEDED( ::SQLDriverConnect( mSQLConnection, _null, (SQLWCHAR*) (const _char*) connectionstring, (SQLSMALLINT) connectionstring.Length( ), buffer, 1024, (SQLSMALLINT*) &length, SQL_DRIVER_NOPROMPT ) ) == _false )
		return _false;

	if ( SQL_SUCCEEDED( ::SQLSetConnectAttr( mSQLConnection, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER) _true, SQL_IS_POINTER ) ) == _false )
		return _false;

	if ( SQL_SUCCEEDED( ::SQLAllocHandle( SQL_HANDLE_STMT, mSQLConnection, &mSQLStatement ) ) == _false )
		return _false;

	mConnectionString = connectionstring;

	return _true;
}

_bool DatabaseConnection::Connect( StringPtr dsn, StringPtr username, StringPtr password )
{
	if ( SQL_SUCCEEDED( ::SQLConnect( mSQLConnection, (SQLWCHAR*) (const _char*) dsn, SQL_NTS, (SQLWCHAR*) (const _char*) username, SQL_NTS, (SQLWCHAR*) (const _char*) password, SQL_NTS ) ) == _false )
		return _false;

	if ( SQL_SUCCEEDED( ::SQLSetConnectAttr( mSQLConnection, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER) _true, SQL_IS_POINTER ) ) == _false )
		return _false;

	if ( SQL_SUCCEEDED( ::SQLAllocHandle( SQL_HANDLE_STMT, mSQLConnection, &mSQLStatement ) ) == _false )
		return _false;

	mConnectionDSN		= dsn;
	mConnectionUsername	= username;
	mConnectionPassword	= password;

	return _true;
}

_bool DatabaseConnection::Disconnect( )
{
	if ( mSQLStatement != _null )
	{
		if ( SQL_SUCCEEDED( ::SQLFreeHandle( SQL_HANDLE_STMT, mSQLStatement ) ) == _false )
			return _false;
	}

	if ( SQL_SUCCEEDED( ::SQLDisconnect( mSQLConnection ) ) == _false )
		return _false;

	mConnectionString	= L"";
	mConnectionDSN		= L"";
	mConnectionUsername	= L"";
	mConnectionPassword	= L"";

	return _true;
}

_bool DatabaseConnection::Reconnect( _bool alwaysretry )
{
	String connectionstring = mConnectionString;
	String dsn				= mConnectionDSN;
	String username			= mConnectionUsername;
	String password			= mConnectionPassword;

	// Connect use connection string.
	if ( connectionstring != L"" )
	{
		Disconnect( );

		// Always retry if connect failed.
		if ( alwaysretry )
		{
			while ( Connect( connectionstring ) == _false );
		}
		// Connect just once.
		else
		{
			if ( Connect( connectionstring ) )
				return _true;
		}
	}
	// Connect use DNS.
	else if ( dsn != L"" )
	{
		Disconnect( );

		// Always retry if connect failed.
		if ( alwaysretry )
		{
			while ( Connect( dsn, username, password ) == _false );
		}
		// Connect just once.
		else
		{
			if ( Connect( dsn, username, password ) )
				return _true;
		}
	}

	mConnectionString	= connectionstring;
	mConnectionDSN		= dsn;
	mConnectionUsername	= username;
	mConnectionPassword	= password;

	return _false;
}

_bool DatabaseConnection::Execute( const _char* sql, ... )
{
	FL_ASSERT( mRecordNumber == 0 )

	// Build SQL command.
	_char buffer[4096];
	StringFormatter::FormatBufferV( buffer, 4096, sql, (_chara*)( &sql ) + sizeof( sql ) );

	return ExecuteHelper( buffer, _true );
}

_bool DatabaseConnection::ExecuteAsync( const _char* sql, ... )
{
	// Build SQL command.
	_char buffer[4096];
	StringFormatter::FormatBufferV( buffer, 4096, sql, (_chara*)( &sql ) + sizeof( sql ) );

	mDeferCommandLock.Enter( );
	mDeferCommandQueue.Append( String( buffer ) );
	mDeferCommandLock.Leave( );

	if ( mDeferCommandThread.GetThreadID( ) == 0 )
	{
		PS_VERIFY( mDeferCommandThread.Create( 10 ) )

		mDeferCommandThread.RegisterUpdateFunc( OnProcessDeferCommand, this, _null );
	}

	return _true;
}

_void DatabaseConnection::FlushAsyncCommand( )
{
	while ( mDeferCommandQueue.Number( ) > 0 )
	{
		mDeferCommandLock.Enter( );

		// Get command from defer queue.
		String command = mDeferCommandQueue.Front( );
		mDeferCommandQueue.Remove( );

		mDeferCommandLock.Leave( );

		// Execute this defer command.
		ExecuteHelper( command, _true );
	}
}

IDatabaseRecord* DatabaseConnection::Query( _bool randomaccess, const _char* sql, ... )
{
	FL_ASSERT( mRecordNumber == 0 )

	// Build SQL command.
	_char buffer[4096];
	StringFormatter::FormatBufferV( buffer, 4096, sql, (_chara*)( &sql ) + sizeof( sql ) );

	if ( randomaccess )
	{
		if ( SQL_SUCCEEDED( ::SQLSetStmtAttr( mSQLStatement, SQL_ATTR_CURSOR_TYPE, (SQLPOINTER) SQL_CURSOR_DYNAMIC, SQL_IS_INTEGER ) ) == _false )
			return _null;
	}

	if ( ExecuteHelper( buffer, _false ) == _false )
		return _null;

	mRecordNumber ++;

	return new DatabaseRecord( mSQLStatement, randomaccess );
}

IDatabaseRecord* DatabaseConnection::QueryAndFetch( const _char* sql, ... )
{
	return _null;
}

_void DatabaseConnection::ReleaseDatabaseRecord( IDatabaseRecord*& record )
{
	if ( record == _null )
		return;

	::SQLCloseCursor( mSQLStatement );

	delete (DatabaseRecord*) record;

	FL_ASSERT( mRecordNumber > 0 )
	mRecordNumber --;

	record = _null;
}

_bool DatabaseConnection::IsConnected( ) const
{
	return mConnectionString != L"" || mConnectionDSN != L"";
}

String DatabaseConnection::GetLastConnectionError( )
{
	_char sqlstate[6], error[1024]; _short size = 0;

	if ( mSQLConnection != _null )
		::SQLGetDiagRec( SQL_HANDLE_DBC, mSQLConnection, 1, sqlstate, _null, error, 1024, &size );

	return size == 0 ? String( L"" ) : String( error );
}

String DatabaseConnection::GetLastCommandError( )
{
	_char sqlstate[6], error[1024]; _short size = 0;

	if ( mSQLStatement != _null )
		::SQLGetDiagRec( SQL_HANDLE_STMT, mSQLStatement, 1, sqlstate, _null, error, 1024, &size );

	return size == 0 ? String( L"" ) : String( error );
}
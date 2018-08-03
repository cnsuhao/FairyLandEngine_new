//============================================================================
// DatabaseManager.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandDatabase.h"

//----------------------------------------------------------------------------
// DatabaseManager Implementation
//----------------------------------------------------------------------------

DatabaseManager::DatabaseManager( )
{
	mSQLEnvironment		= _null;
	mConnectionNumber	= 0;
}

DatabaseManager::~DatabaseManager( )
{
	FL_ASSERT( mConnectionNumber == 0 )

	if ( mSQLEnvironment != _null )
		::SQLFreeHandle( SQL_HANDLE_ENV, mSQLEnvironment );

	GetFairyLandGlobal( ).mDatabaseManager = _null;
}

_void DatabaseManager::Release( )
{
	delete this;
}

_dword DatabaseManager::GetDatabaseConnectionNumber( ) const
{
	return mConnectionNumber;
}

IDatabaseConnection* DatabaseManager::CreateDatabaseConnection( )
{
	if ( mSQLEnvironment == _null )
	{
		if ( SQL_SUCCEEDED( ::SQLAllocHandle( SQL_HANDLE_ENV, SQL_NULL_HANDLE, &mSQLEnvironment ) ) == _false )
			return _null;

		if ( SQL_SUCCEEDED( ::SQLSetEnvAttr( mSQLEnvironment, SQL_ATTR_ODBC_VERSION, (SQLPOINTER) SQL_OV_ODBC3, SQL_IS_INTEGER ) ) == _false )
			return _null;
	}

	_handle connection = _null;
	if ( SQL_SUCCEEDED( ::SQLAllocHandle( SQL_HANDLE_DBC, mSQLEnvironment, &connection ) ) == _false )
		return _null;

	mConnectionNumber ++;

	return new DatabaseConnection( connection );
}

_void DatabaseManager::ReleaseDatabaseConnection( IDatabaseConnection*& connection )
{
	if ( connection == _null )
		return;

	delete (DatabaseConnection*) connection;

	FL_ASSERT( mConnectionNumber > 0 )
	mConnectionNumber --;

	connection = _null;
}

String DatabaseManager::GetLastError( )
{
	_char sqlstate[6], error[1024]; _short size = 0;

	if ( mSQLEnvironment != _null )
		::SQLGetDiagRec( SQL_HANDLE_ENV, mSQLEnvironment, 1, sqlstate, _null, error, 1024, &size );

	return size == 0 ? String( L"" ) : String( error );
}
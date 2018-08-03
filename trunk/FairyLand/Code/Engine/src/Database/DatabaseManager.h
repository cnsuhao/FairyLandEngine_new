//============================================================================
// DatabaseManager.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// DatabaseManager
//----------------------------------------------------------------------------

class DatabaseManager : public IDatabaseManager
{
private:
	_handle		mSQLEnvironment;
	_dword		mConnectionNumber;

public:
	DatabaseManager( );
	~DatabaseManager( );

	virtual _void Release( );

	virtual _dword GetDatabaseConnectionNumber( ) const;

	virtual IDatabaseConnection* CreateDatabaseConnection( );
	virtual _void ReleaseDatabaseConnection( IDatabaseConnection*& connection );

	// Error
	virtual String GetLastError( );
};

};
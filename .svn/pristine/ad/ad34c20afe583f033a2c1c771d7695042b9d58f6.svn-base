//============================================================================
// IDatabaseManager.h
//
// Copyright 2006-2007 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// IDatabaseManager
//----------------------------------------------------------------------------

class IDatabaseManager
{
public:
	virtual _void Release( )													= 0;

	virtual _dword GetDatabaseConnectionNumber( ) const							= 0;

	virtual IDatabaseConnection* CreateDatabaseConnection( )					= 0;
	virtual _void ReleaseDatabaseConnection( IDatabaseConnection*& connection )	= 0;

	// Error
	virtual String GetLastError( )												= 0;
};

};
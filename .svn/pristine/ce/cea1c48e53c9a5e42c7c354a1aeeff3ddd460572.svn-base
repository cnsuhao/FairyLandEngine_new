//============================================================================
// FairyLandDatabase.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandDatabase.h"

//----------------------------------------------------------------------------
// CreateDatabaseManager Implementation
//----------------------------------------------------------------------------

IDatabaseManager* CreateDatabaseManager( )
{
	IDatabaseManager* databasemanager = GetDatabaseManagerPointer( );

	if ( databasemanager != _null )
		return databasemanager;

	databasemanager = new DatabaseManager( );
	GetFairyLandGlobal( ).mDatabaseManager = databasemanager;

	PS_TRACE( L"[KL] DatabaseManager Created\r\n" )

	return databasemanager;
}
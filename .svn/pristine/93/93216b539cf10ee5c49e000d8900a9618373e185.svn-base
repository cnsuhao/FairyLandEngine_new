//============================================================================
// AsyncNetworkManager.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandNetwork.h"

//----------------------------------------------------------------------------
// AsyncNetworkManager Implementation
//----------------------------------------------------------------------------

AsyncNetworkManager::AsyncNetworkManager( )
{
	mCurrentArray = &mOperationArray1;
	mProcessArray = &mOperationArray2;
}

AsyncNetworkManager::~AsyncNetworkManager( )
{
	mManagerLock.Enter( );

	for ( _dword i = 0; i < mCurrentArray->Number( ); i ++ )
	{
		AsyncOperation& operation = (*mCurrentArray)[i];

		if ( operation.mBuffer != _null )
			delete[] operation.mBuffer;
	}

	for ( _dword i = 0; i < mProcessArray->Number( ); i ++ )
	{
		AsyncOperation& operation = (*mProcessArray)[i];

		if ( operation.mBuffer != _null )
			delete[] operation.mBuffer;
	}

	mManagerLock.Leave( );
}

_void AsyncNetworkManager::PostAsyncOperation( const AsyncOperation& asyncoperation )
{
	LockOwner lockowner( mManagerLock );

	mCurrentArray->Append( asyncoperation );
}

_void AsyncNetworkManager::BeginProcess( )
{
	LockOwner lockowner( mManagerLock );

	AsyncOperationArray* temp = mCurrentArray;

	mCurrentArray = mProcessArray;
	mProcessArray = temp;
}

_void AsyncNetworkManager::EndProcess( )
{
	for ( _dword i = 0; i < mProcessArray->Number( ); i ++ )
	{
		AsyncOperation& operation = (*mProcessArray)[i];

		if ( operation.mBuffer != _null )
			delete[] operation.mBuffer;
	}

	mProcessArray->Clear( );
}
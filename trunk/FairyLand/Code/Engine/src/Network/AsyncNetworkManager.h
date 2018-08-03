//============================================================================
// AsyncOperation.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// AsyncOperation
//----------------------------------------------------------------------------

struct AsyncOperation
{
	enum
	{
		_ASYNC_SENDPACKET					=  1,
		_ASYNC_SENDDEFERPACKET				=  2,
		_ASYNC_FLUSHDEFERPACKET				=  3,

		_ASYNC_SENDGROUPPACKET				=  4,
		_ASYNC_SENDGROUPDEFERPACKET			=  5,
		_ASYNC_FLUSHGROUPDEFERPACKET		=  6,

		_ASYNC_SENDSESSIONPACKET			=  7,
		_ASYNC_SENDSESSIONDEFERPACKET		=  8,
		_ASYNC_FLUSHSESSIONDEFERPACKET		=  9,
	};

	_dword	mOperationType;
	_dword	mSessionID;
	_dword	mParameter1;
	_dword	mParameter2;
	_dword	mParameter3;
	_dword	mParameter4;
	_byte*	mBuffer;
	_dword	mLength;

	AsyncOperation( _dword type = 0, _dword sessionid = 0, _dword param1 = 0, _dword param2 = 0, _dword param3 = 0, _dword param4 = 0, _byte* buffer = _null, _dword length = 0 )
	{
		mOperationType	= type;
		mSessionID		= sessionid;
		mParameter1		= param1;
		mParameter2		= param2;
		mParameter3		= param3;
		mParameter4		= param4;
		mBuffer			= _null;
		mLength			= length;

		if ( mLength != 0 )
		{
			FL_ASSERT( buffer != _null )

			mBuffer = new _byte[ length ];
			Memory::MemCpy( mBuffer, buffer, length );
		}
	}
};

typedef Array< AsyncOperation > AsyncOperationArray;

//----------------------------------------------------------------------------
// AsyncNetworkManager
//----------------------------------------------------------------------------

class AsyncNetworkManager : public AsyncManager
{
private:
	AsyncOperationArray		mOperationArray1;
	AsyncOperationArray		mOperationArray2;

	AsyncOperationArray*	mCurrentArray;
	AsyncOperationArray*	mProcessArray;

public:
	AsyncNetworkManager( );
	~AsyncNetworkManager( );

	inline AsyncOperationArray& GetCurrentOperationArray( );
	inline AsyncOperationArray& GetProcessOperationArray( );

	_void PostAsyncOperation( const AsyncOperation& asyncoperation );
	_void BeginProcess( );
	_void EndProcess( );
};

AsyncOperationArray& AsyncNetworkManager::GetCurrentOperationArray( )
{
	return *mCurrentArray;
}

AsyncOperationArray& AsyncNetworkManager::GetProcessOperationArray( )
{
	return *mProcessArray;
}

};
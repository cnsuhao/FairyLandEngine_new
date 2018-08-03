//============================================================================
// Registry.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// Registry
//----------------------------------------------------------------------------

class Registry
{
private:
	_handle		mKey;
	_dword		mRight;

public:
	enum _ACCESS_RIGHT
	{
		_ACCESS_ALL			= 0,
		_ACCESS_READ		= 1,
		_ACCESS_WRITE		= 2,
		_ACCESS_READWRITE	= 3,
	};

	enum _ROOT_KEY
	{
		_KEY_CLASSES_ROOT		= 0x80000000,
		_KEY_CURRENT_USER		= 0x80000001,
		_KEY_LOCAL_MACHINE		= 0x80000002,
		_KEY_USERS				= 0x80000003,
		_KEY_PERFORMANCE_DATA	= 0x80000004,
		_KEY_CURRENT_CONFIG		= 0x80000005,
		_KEY_DYN_DATA			= 0x80000006,
	};

	enum _KEY_TYPE
	{
		_TYPE_NONE				= 0,
		_TYPE_STRING			= 1,
		_TYPE_BINARY			= 3,
		_TYPE_DWORD				= 4,
		_TYPE_LINK				= 6,
		_TYPE_QWORD				= 11,
	};

	Registry( );
	~Registry( );

	_bool IsKeyOpened( ) const;
	_void SetAccessRight( _dword right );

	_bool Open( _dword root, StringPtr keyname );
	_void Close( );

	_bool DoesKeyExist( StringPtr keyname );
	_bool DoesValueExist( StringPtr valuename );

	_bool CreateKey( StringPtr keyname );
	_bool DeleteKey( StringPtr keyname );

	_bool ReadValue( StringPtr valuename, _dword& type, _void* buffer, _dword& length );
	_bool WriteValue( StringPtr valuename, _dword type, _void* buffer, _dword length );
	_bool DeleteValue( StringPtr valuename );

	_bool SaveKey( StringPtr filename );
	_bool LoadKey( StringPtr filename );
};

};
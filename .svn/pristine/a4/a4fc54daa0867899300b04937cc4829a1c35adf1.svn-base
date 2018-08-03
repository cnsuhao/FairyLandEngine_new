//============================================================================
// Registry.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// Registry Implementation
//----------------------------------------------------------------------------

Registry::Registry( ) : mKey( _null ), mRight( _ACCESS_ALL )
{
}

Registry::~Registry( )
{
	Close( );
}

_bool Registry::IsKeyOpened( ) const
{
	return mKey != _null;
}

_void Registry::SetAccessRight( _dword right )
{
	if ( right == _ACCESS_ALL )
		mRight = KEY_ALL_ACCESS;
	else if ( right == _ACCESS_READ )
		mRight = KEY_READ;
	else if ( right == _ACCESS_WRITE )
		mRight = KEY_WRITE | KEY_CREATE_SUB_KEY;
	else if ( right == _ACCESS_READWRITE )
		mRight = KEY_READ | KEY_WRITE | KEY_CREATE_SUB_KEY;
}

_bool Registry::Open( _dword root, StringPtr keyname )
{
	return ::RegOpenKeyEx( (HKEY) root, keyname, 0, mRight, (HKEY*) &mKey ) == 0;
}

_void Registry::Close( )
{
	if ( mKey != _null )
		::RegCloseKey( (HKEY) mKey );

	mKey = _null;
}

_bool Registry::DoesKeyExist( StringPtr keyname )
{
	_handle subkey = _null;

	if ( ::RegOpenKeyEx( (HKEY) mKey, keyname, 0, mRight, (HKEY*) &subkey ) != 0 )
		return _false;

	::RegCloseKey( (HKEY) subkey );

	return _true;
}

_bool Registry::DoesValueExist( StringPtr valuename )
{
	_dword length;
	return ::RegQueryValueEx( (HKEY) mKey, valuename, _null, _null, _null, &length ) == 0;
}

_bool Registry::CreateKey( StringPtr keyname )
{
	_dword disposition = 0;
	return ::RegCreateKeyEx( (HKEY) mKey, keyname, 0, _null, REG_OPTION_NON_VOLATILE, mRight, _null, (HKEY*) &mKey, &disposition ) == 0;
}

_bool Registry::DeleteKey( StringPtr keyname )
{
	return ::RegDeleteKey( (HKEY) mKey, keyname ) == 0;
}

_bool Registry::ReadValue( StringPtr valuename, _dword& type, _void* buffer, _dword& length )
{
	return ::RegQueryValueEx( (HKEY) mKey, valuename, _null, &type, (_byte*) buffer, &length ) == 0;
}

_bool Registry::WriteValue( StringPtr valuename, _dword type, _void* buffer, _dword length )
{
	return ::RegSetValueEx( (HKEY) mKey, valuename, 0, type, (_byte*) buffer, length ) == 0;
}

_bool Registry::DeleteValue( StringPtr valuename )
{
	return ::RegDeleteValue( (HKEY) mKey, valuename ) == 0;
}

_bool Registry::SaveKey( StringPtr filename )
{
	return ::RegSaveKey( (HKEY) mKey, filename, _null ) == 0;
}

_bool Registry::LoadKey( StringPtr filename )
{
	return ::RegRestoreKey( (HKEY) mKey, filename, REG_FORCE_RESTORE ) == 0;
}
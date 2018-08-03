//============================================================================
// PEResArchive.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandStorage.h"

//----------------------------------------------------------------------------
// PEResArchive Implementation
//----------------------------------------------------------------------------

PEResArchive::PEResArchive( ) : mModule( _null )
{
}

PEResArchive::~PEResArchive( )
{
	if ( mModule != _null )
		::FreeLibrary( (HMODULE) mModule );
}

_bool PEResArchive::OnFoundPERes( _handle module, StringPtr type, StringPtr name, _void* parameter )
{
	typedef Quadruple< FileSystem::OnFoundFile, _void*, _void*, _dword > EnumParameter;

	EnumParameter* enumparameter = (EnumParameter*) parameter;
	FL_ASSERT( enumparameter != _null )

	enumparameter->mObject4 ++;

	if ( enumparameter->mObject1 != _null )
		return (*enumparameter->mObject1)( name, enumparameter->mObject4, enumparameter->mObject2, enumparameter->mObject3 );

	return _true;
}

_bool PEResArchive::CreateArchive( StringPtr archivename, StringPtr mountpath )
{
	return _false;
}

_bool PEResArchive::OpenArchive( StringPtr archivename, StringPtr mountpath, _handle module )
{
	mName = archivename;
	mName.Lowercase( );

	mPath = mountpath;
	mPath.Lowercase( );
	mPath.TrimBoth( L"\\/ " );
	mPath.ReplaceAll( '/', '\\' );

	if ( mPath == L"." )
		mPath = L"";

	if ( archivename.CompareLowercase( L".exe" ) )
		archivename = L"";

	if ( module == _null )
	{
		if ( archivename == L"" )
			return _true;

		// Set the path of library of current directory.
		DirectoryRecorder( StringFormatter::GetPathName( archivename ) );
		mModule = ::LoadLibrary( StringFormatter::GetFileName( archivename ) );

		return mModule != _null;
	}
	else
	{
		if ( System::GetDllFileName( module ) == L"" )
			return _false;

		mModule = module;
		return _true;
	}
}

_void PEResArchive::Release( )
{
	delete this;
}

StringPtr PEResArchive::GetName( ) const
{
	return mName;
}

StringPtr PEResArchive::GetPath( ) const
{
	return mPath;
}

_dword PEResArchive::GetArchiveType( ) const
{
	return IResourceManager::_ARCHIVE_PERES;
}

_dword PEResArchive::GetResourceNumber( ) const
{
	ResourceManager* manager = (ResourceManager*) GetResourceManagerPointer( );
	FL_ASSERT( manager != _null )

	LockOwner lockowner( manager->GetResourceLock( ) );

	typedef Quadruple< FileSystem::OnFoundFile, _void*, _void*, _dword > EnumParameter;

	EnumParameter enumparameter( _null, 0, 0, 0 );

	if ( ::EnumResourceNames( (HMODULE) mModule, L"FAIRYLANDRES", (ENUMRESNAMEPROC) OnFoundPERes, (LONG_PTR) &enumparameter ) == _false )
		return 0;

	return enumparameter.mObject4;
}

IDataStream* PEResArchive::OpenResource( StringPtr resname )
{
	ResourceManager* manager = (ResourceManager*) GetResourceManagerPointer( );
	FL_ASSERT( manager != _null )

	LockOwner lockowner( manager->GetResourceLock( ) );

	// Find resource by name & type.
	_handle resinfo = ::FindResource( (HMODULE) mModule, resname, L"FAIRYLANDRES" );
	if ( resinfo == _null )
		return _null;

	// Get the size of the resource.
	_dword size = ::SizeofResource( (HMODULE) mModule, (HRSRC) resinfo );
	if ( size == 0 )
		return _null;

	// Load the resource into memory.
	_handle resource = ::LoadResource( (HMODULE) mModule, (HRSRC) resinfo );
	if ( resource == _null )
		return _null;

	_char fullname[1024];

	if ( mPath == L"." || mPath == L"" )
		StringFormatter::CopyString( fullname, resname );
	else
		StringFormatter::FormatBuffer( fullname, 1024, L"%s\\%s", (const _char*) mPath, (const _char*) resname );

	return new PEResStream( fullname, resource, size );
}

_bool PEResArchive::CreateResource( StringPtr resname, const _void* buffer, _dword size, const Oword* checksum, _dword* finish )
{
	return _false;
}

_bool PEResArchive::DeleteResource( StringPtr resname )
{
	return _false;
}

_bool PEResArchive::SearchResource( StringPtr resname, _dword* size, Oword* checksum )
{
	ResourceManager* manager = (ResourceManager*) GetResourceManagerPointer( );
	FL_ASSERT( manager != _null )

	LockOwner lockowner( manager->GetResourceLock( ) );

	// Find resource by name & type.
	_handle resinfo = ::FindResource( (HMODULE) mModule, resname, L"FAIRYLANDRES" );
	if ( resinfo == _null )
		return _false;

	// Get the size of the resource.
	_dword ressize = ::SizeofResource( (HMODULE) mModule, (HRSRC) resinfo );
	if ( ressize == 0 )
		return _false;

	if ( size != _null )
		*size = ressize;

	return _true;
}

_dword PEResArchive::EnumArchive( FileSystem::OnFoundFile funcpointer, _void* parameter1, _void* parameter2, _float* percentage )
{
	ResourceManager* manager = (ResourceManager*) GetResourceManagerPointer( );
	FL_ASSERT( manager != _null )

	LockOwner lockowner( manager->GetResourceLock( ) );

	// Set percentage to 0.0 at begin.
	if ( percentage != _null )
		*percentage = 0.0f;

	typedef Quadruple< FileSystem::OnFoundFile, _void*, _void*, _dword > EnumParameter;

	EnumParameter enumparameter( funcpointer, parameter1, parameter2, 0 );

	::EnumResourceNames( (HMODULE) mModule, L"FAIRYLANDRES", (ENUMRESNAMEPROC) OnFoundPERes, (LONG_PTR) &enumparameter );

	// Set percentage to 1.0 at end.
	if ( percentage != _null )
		*percentage = 1.0f;

	return enumparameter.mObject4;
}
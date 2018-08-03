//============================================================================
// FileArchive.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// FileArchive
//----------------------------------------------------------------------------

class FileArchive : public IDataArchive
{
private:
	String		mName;
	String		mPath;
	String		mAbsolutePath;

public:
	FileArchive( );
	~FileArchive( );

	_bool	CreateArchive( StringPtr archivename, StringPtr mountpath );
	_bool	OpenArchive( StringPtr archivename, StringPtr mountpath );
	_void	BuildFileName( _char* buffer, _dword length, StringPtr resname );

	virtual _void			Release( );

	virtual StringPtr		GetName( ) const;
	virtual StringPtr		GetPath( ) const;

	virtual _dword			GetArchiveType( ) const;
	virtual _dword			GetResourceNumber( ) const;

	virtual IDataStream*	OpenResource( StringPtr resname );
	virtual _bool			CreateResource( StringPtr resname, const _void* buffer, _dword size, const Oword* checksum, _dword* finish );
	virtual _bool			DeleteResource( StringPtr resname );
	virtual _bool			SearchResource( StringPtr resname, _dword* size, Oword* checksum );

	virtual _dword			EnumArchive( FileSystem::OnFoundFile funcpointer, _void* parameter1, _void* parameter2, _float* percentage );
};

};
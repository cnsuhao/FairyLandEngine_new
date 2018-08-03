//============================================================================
// PEResArchive.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// PEResArchive
//----------------------------------------------------------------------------

class PEResArchive : public IDataArchive
{
private:
	_handle		mModule;
	String		mName;
	String		mPath;

	static _bool OnFoundPERes( _handle module, StringPtr type, StringPtr name, _void* parameter );

public:
	PEResArchive( );
	~PEResArchive( );

	_bool	CreateArchive( StringPtr archivename, StringPtr mountpath );
	_bool	OpenArchive( StringPtr archivename, StringPtr mountpath, _handle module );

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
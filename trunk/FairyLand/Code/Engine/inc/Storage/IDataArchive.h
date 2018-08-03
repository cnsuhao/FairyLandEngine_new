//============================================================================
// IDataArchive.h
//
// Copyright 2006-2007 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// IDataArchive
//----------------------------------------------------------------------------

class IDataArchive
{
public:
	virtual _void			Release( )																												= 0;

	virtual StringPtr		GetName( ) const																										= 0;
	virtual StringPtr		GetPath( ) const																										= 0;

	virtual _dword			GetArchiveType( ) const																									= 0;
	virtual _dword			GetResourceNumber( ) const																								= 0;

	virtual IDataStream*	OpenResource( StringPtr resname )																						= 0;
	virtual _bool			CreateResource( StringPtr resname, const _void* buffer, _dword size, const Oword* checksum, _dword* finish = _null )	= 0;
	virtual _bool			DeleteResource( StringPtr resname )																						= 0;
	virtual _bool			SearchResource( StringPtr resname, _dword* size, Oword* checksum )														= 0;

	virtual _dword			EnumArchive( FileSystem::OnFoundFile funcpointer, _void* parameter1, _void* parameter2, _float* percentage = _null )	= 0;
};

};
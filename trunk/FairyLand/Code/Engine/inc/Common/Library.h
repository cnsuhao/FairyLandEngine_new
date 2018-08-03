//============================================================================
// Library.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// Library
//----------------------------------------------------------------------------

class Library
{
private:
	struct MemoryModule
	{
		_byte*		mNTHeaders;
		_byte*		mCodeBase;
		_void**		mModuleList;
		_long		mModuleNumber;
	};

	_handle		mModule;
	_bool		mLibInMemory;

	static _void CopySections( const _byte* data, _void* header, MemoryModule* module );
	static _void BaseRelocation( MemoryModule* module, _dword delta );
	static _bool BuildImportTable( MemoryModule* module );
	static _void FinalizeSections( MemoryModule* module );

public:
	Library( );
	~Library( );

	_bool	Load( StringPtr libraryname );
	_bool	Load( const _byte* memory );
	_void	Free( );

	_handle	GetModule( ) const;
	_void*	GetFunction( StringPtr functionname ) const;
};

};
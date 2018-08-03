//============================================================================
// Library.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// Library Implementation
//----------------------------------------------------------------------------

Library::Library( ) : mModule( _null ), mLibInMemory( _false )
{
}

Library::~Library( )
{
	Free( );
}

_void Library::CopySections( const _byte* data, _void* header, MemoryModule* module )
{
	// Get code base address.
	_byte* code = module->mCodeBase;

	// Get image header.
	IMAGE_NT_HEADERS* ntheader	= (IMAGE_NT_HEADERS*) header;
	IMAGE_NT_HEADERS* memheader	= (IMAGE_NT_HEADERS*) module->mNTHeaders;

	// Get first section header.
	IMAGE_SECTION_HEADER* section = IMAGE_FIRST_SECTION( memheader );

	// Copy each section.
	for ( _word i = 0; i < memheader->FileHeader.NumberOfSections; i ++ )
	{
		// Section doesn't contain data in the dll itself, but may define uninitialized data.
		if ( section[i].SizeOfRawData == 0 )
		{
			_dword size = ntheader->OptionalHeader.SectionAlignment;

			if ( size > 0 )
			{
				// Commit memory for section.
				_byte* des = (_byte*) ::VirtualAlloc( code + section[i].VirtualAddress, size, MEM_COMMIT, PAGE_READWRITE );
				section[i].Misc.PhysicalAddress = (_dword) des;

				// Initialize all data to zero.
				Memory::MemSet( des, 0, size );
			}
		}
		else
		{
			_dword size = section[i].SizeOfRawData;

			if ( size > 0 )
			{
				// Commit memory for section.
				_byte* des = (_byte*) ::VirtualAlloc( code + section[i].VirtualAddress,
					size, MEM_COMMIT, PAGE_READWRITE );
				section[i].Misc.PhysicalAddress = (_dword) des;

				// Copy section data.
				Memory::MemCpy( des, (_byte*) data + section[i].PointerToRawData, size );
			}
		}
	}
}

_void Library::FinalizeSections( MemoryModule* module )
{
	// Get image header.
	IMAGE_NT_HEADERS* memheader	= (IMAGE_NT_HEADERS*) module->mNTHeaders;

	// Get first section header.
	IMAGE_SECTION_HEADER* section = IMAGE_FIRST_SECTION( memheader );

	// Loop through all sections and change access flags.
	for ( _word i = 0; i < memheader->FileHeader.NumberOfSections; i ++ )
	{
		// The section is discardable and can safely be freed.
		if ( section[i].Characteristics & IMAGE_SCN_MEM_DISCARDABLE )
		{
			::VirtualFree( (_void*) section[i].Misc.PhysicalAddress, section[i].SizeOfRawData, MEM_DECOMMIT );
			continue;
		}

		_dword executable	= ( section[i].Characteristics & IMAGE_SCN_MEM_EXECUTE ) != 0;
		_dword readable		= ( section[i].Characteristics & IMAGE_SCN_MEM_READ ) != 0;
		_dword writeable	= ( section[i].Characteristics & IMAGE_SCN_MEM_WRITE ) != 0;

		const static _dword protectionflags[2][2][2] =
		{
			{
				{ PAGE_NOACCESS, PAGE_WRITECOPY },
				{ PAGE_READONLY, PAGE_READWRITE },
			},
			{
				{ PAGE_EXECUTE, PAGE_EXECUTE_WRITECOPY },
				{ PAGE_EXECUTE_READ, PAGE_EXECUTE_READWRITE },
			},
		};

		// Determine protection flags based on characteristics.
		_dword protect = protectionflags[ executable ][ readable ][ writeable ];
		if ( section[i].Characteristics & IMAGE_SCN_MEM_NOT_CACHED )
			protect |= PAGE_NOCACHE;

		// Determine size of region
		_dword size = section[i].SizeOfRawData;

		if ( size == 0 )
		{
			if ( section[i].Characteristics & IMAGE_SCN_CNT_INITIALIZED_DATA )
				size = memheader->OptionalHeader.SizeOfInitializedData;
			else if ( section[i].Characteristics & IMAGE_SCN_CNT_UNINITIALIZED_DATA )
				size = memheader->OptionalHeader.SizeOfUninitializedData;
		}

		if ( size > 0 )
		{
			_dword oldProtect = 0;

			// Change memory access flags.
			::VirtualProtect( (_void*) section[i].Misc.PhysicalAddress, section[i].SizeOfRawData, protect, &oldProtect );
		}
	}
}

_void Library::BaseRelocation( MemoryModule* module, _dword delta )
{
	if ( delta == 0 )
		return;

	// Get code base address.
	_byte* code = module->mCodeBase;

	// Get image header.
	IMAGE_NT_HEADERS* memheader	= (IMAGE_NT_HEADERS*) module->mNTHeaders;

	// Get image relocation directory.
	IMAGE_DATA_DIRECTORY* directory = &( memheader->OptionalHeader.DataDirectory[ IMAGE_DIRECTORY_ENTRY_BASERELOC ] );

	if ( directory->Size == 0 )
		return;

	// Get relocation information.
	IMAGE_BASE_RELOCATION* relocation = (IMAGE_BASE_RELOCATION*)( code + directory->VirtualAddress );

	while ( relocation->VirtualAddress > 0 )
	{
		_byte* des = (_byte*) ( code + relocation->VirtualAddress );
		_word* rel = (_word*) ( (_byte*) relocation + sizeof( IMAGE_BASE_RELOCATION ) );

		for ( _dword i = 0; i < ( relocation->SizeOfBlock - sizeof( IMAGE_BASE_RELOCATION ) ) / 2; i ++ )
		{
			// The upper 4 bits define the type of relocation.
			_word type = rel[i] >> 12;
			// The lower 12 bits define the offset.
			_word offset = rel[i] & 0x0FFF;

			switch ( type )
			{
				// Skip relocation.
				case IMAGE_REL_BASED_ABSOLUTE:
					break;

				// Change complete 32 bit address.
				case IMAGE_REL_BASED_HIGHLOW:
				{
					_dword* address = (_dword*) ( des + offset );
					*address += delta;

					break;
				}
			}
		}

		// Advance to next relocation block.
		relocation = (IMAGE_BASE_RELOCATION*) ( (_dword) relocation + relocation->SizeOfBlock );
	}
}

_bool Library::BuildImportTable( MemoryModule* module )
{
	// Get code base address.
	_byte* code = module->mCodeBase;

	// Get image header.
	IMAGE_NT_HEADERS* memheader	= (IMAGE_NT_HEADERS*) module->mNTHeaders;

	// Get image import directory.
	IMAGE_DATA_DIRECTORY* directory = &( memheader->OptionalHeader.DataDirectory[ IMAGE_DIRECTORY_ENTRY_IMPORT ] );

	if ( directory->Size == 0 )
		return _true;

	// Get import information.
	IMAGE_IMPORT_DESCRIPTOR* import = (IMAGE_IMPORT_DESCRIPTOR*)( code + directory->VirtualAddress );

	while ( ::IsBadReadPtr( import, sizeof( IMAGE_IMPORT_DESCRIPTOR ) ) == _false && import->Name != 0 )
	{
		_dword *thunkref = _null, *funcref = _null;

		// Load import library.
		HMODULE library = ::LoadLibraryA( (const _chara*) ( code + import->Name ) );

		// Can't load library.
		if ( library == _null )
			return _false;

		// Reallocate memory for module list.
		if ( module->mModuleList == _null )
			module->mModuleList = (_void**) ::HeapAlloc( ::GetProcessHeap( ), 0, sizeof( _void** ) );
		else
			module->mModuleList = (_void**) ::HeapReAlloc( ::GetProcessHeap( ), 0, module->mModuleList, ( module->mModuleNumber + 1 ) * sizeof( _void** ) );

		// Can't allocater memory.
		if ( module->mModuleList == _null )
			return _false;

		// Set library module address.
		module->mModuleList[ module->mModuleNumber ++ ] = library;

		// Has hint table.
		if ( import->OriginalFirstThunk != 0 )
		{
			thunkref	= (_dword*) ( code + import->OriginalFirstThunk );
			funcref		= (_dword*) ( code + import->FirstThunk );
		}
		// No hint table.
		else
		{
			thunkref	= (_dword*) ( code + import->FirstThunk );
			funcref		= (_dword*) ( code + import->FirstThunk );
		}

		// Import all functions.
		while ( *thunkref != 0 )
		{
			if ( IMAGE_SNAP_BY_ORDINAL( *thunkref ) == _true )
			{
				*funcref = (_dword) ::GetProcAddress( library, (const _chara*) IMAGE_ORDINAL( *thunkref ) );
			}
			else
			{
				IMAGE_IMPORT_BY_NAME* thunkdata = (IMAGE_IMPORT_BY_NAME*) ( code + *thunkref );
				*funcref = (_dword) ::GetProcAddress( library, (const _chara*) &thunkdata->Name );
			}

			if ( *funcref == 0 )
				return _false;

			thunkref ++;
			funcref  ++;
		}

		// Advance to next import block.
		import ++;
	}

	return _true;
}

_bool Library::Load( StringPtr libraryname )
{
	if ( mModule != _null )
		Free( );

	String path = StringFormatter::GetPathName( libraryname );

	if ( path != L"" )
	{
		if ( FileSystem::DoesDirExist( path ) == _false )
			return _false;
	}

	// Set the path of library of current directory.
	DirectoryRecorder dir( path );

	mModule			= ::LoadLibrary( StringFormatter::GetFileName( libraryname ) );
	mLibInMemory	= _false;

	return mModule != _null;
}

_bool Library::Load( const _byte* memory )
{
	if ( mModule != _null )
		Free( );

	FL_ASSERT( memory != _null )

	IMAGE_DOS_HEADER* dosheader = (IMAGE_DOS_HEADER*) memory;

	// Not a valid executable file.
	if ( dosheader->e_magic != IMAGE_DOS_SIGNATURE )
		return _false;

	IMAGE_NT_HEADERS* ntheader = (IMAGE_NT_HEADERS*) ( (_byte*) memory + dosheader->e_lfanew );

	// Not a valid pe file.
	if ( ntheader->Signature != IMAGE_NT_SIGNATURE )
		return _false;

	// Reserve memory for image of library.
	_byte* code = (_byte*) ::VirtualAlloc( (_void*) ntheader->OptionalHeader.ImageBase,
		ntheader->OptionalHeader.SizeOfImage, MEM_RESERVE, PAGE_READWRITE );

	// First try failed, then try to allocate memory at arbitrary position.
	if ( code == _null )
		code = (_byte*) ::VirtualAlloc( _null, ntheader->OptionalHeader.SizeOfImage, MEM_RESERVE, PAGE_READWRITE );

	// Can't reserve memory.
	if ( code == _null )
		return _false;

	// Allocate memory for module header.
	MemoryModule* module = (MemoryModule*) ::HeapAlloc( ::GetProcessHeap( ), 0, sizeof( MemoryModule ) );
	module->mCodeBase		= code;
	module->mModuleList		= _null;
	module->mModuleNumber	= 0;

	// Commit a complete memory region for library image.
	::VirtualAlloc( code, ntheader->OptionalHeader.SizeOfImage, MEM_COMMIT, PAGE_READWRITE );

	// Commit memory for headers.
	_byte* headers = (_byte*) ::VirtualAlloc( code, ntheader->OptionalHeader.SizeOfHeaders, MEM_COMMIT, PAGE_READWRITE );

	// Copy PE header to code.
	Memory::MemCpy( headers, dosheader, dosheader->e_lfanew + ntheader->OptionalHeader.SizeOfHeaders );

	// Set memory module header.
	module->mNTHeaders = ( (_byte*) headers + dosheader->e_lfanew );
	IMAGE_NT_HEADERS* memheader = (IMAGE_NT_HEADERS*) module->mNTHeaders;

	// Update position
	memheader->OptionalHeader.ImageBase = (_dword) code;

	// copy sections from dll file block to new memory location.
	CopySections( memory, ntheader, module );

	// Adjust base address of imported data.
	BaseRelocation( module, (_dword)( code - ntheader->OptionalHeader.ImageBase ) );

	// Load required dlls and adjust function table of imports
	if ( BuildImportTable( module ) == _false )
		return _false;

	// Mark memory pages depending on section headers and release sections that are marked as "discardable".
	FinalizeSections( module );

	// Get entry point of loaded library
	if ( memheader->OptionalHeader.AddressOfEntryPoint != 0 )
	{
		typedef _bool (__stdcall *DllEntryProc) ( _handle, _dword, _void* );
		DllEntryProc dllentry = (DllEntryProc) ( code + memheader->OptionalHeader.AddressOfEntryPoint );

		if ( dllentry != _null )
		{
			// Notify library about attaching to process.
			if ( (*dllentry) ( code, DLL_PROCESS_ATTACH, 0 ) == _false )
				return _false;
		}
	}

	mModule			= module;
	mLibInMemory	= _true;

	return _true;
}

_void Library::Free( )
{
	if ( mModule == _null )
		return;

	// Free library in memory.
	if ( mLibInMemory == _true )
	{
		MemoryModule* module = (MemoryModule*) mModule;

		// Get code base address.
		_byte* code = module->mCodeBase;

		// Get image header.
		IMAGE_NT_HEADERS* memheader	= (IMAGE_NT_HEADERS*) module->mNTHeaders;

		typedef _bool (__stdcall *DllEntryProc) ( _handle, _dword, _void* );
		DllEntryProc dllentry = (DllEntryProc) ( code + memheader->OptionalHeader.AddressOfEntryPoint );

		if ( dllentry != _null )
		{
			// Notify library about detaching from process.
			(*dllentry) ( code, DLL_PROCESS_DETACH, 0 );
		}

		if ( module->mModuleList != _null )
		{
			// Free previously opened libraries.
			for ( _long i = 0; i < module->mModuleNumber; i ++ )
			{
				if ( module->mModuleList[i] != _null )
					::FreeLibrary( (HMODULE) module->mModuleList[i] );
			}

			::HeapFree( ::GetProcessHeap( ), 0, module->mModuleList );
		}

		// Release memory of library.
		if ( code != _null )
			::VirtualFree( code, 0, MEM_RELEASE );

		::HeapFree( ::GetProcessHeap( ), 0, module );
	}
	// Free library using win32 api.
	else
	{
		::FreeLibrary( (HMODULE) mModule );
	}

	mModule			= _null;
	mLibInMemory	= _false;
}

_handle	Library::GetModule( ) const
{
	return mModule;
}

_void* Library::GetFunction( StringPtr functionname ) const
{
	if ( mModule == _null )
		return _null;

	_chara ansiname[1024];
	StringFormatter::UnicodeToAnsi( ansiname, 1024, functionname );

	// Get function from library in memory.
	if ( mLibInMemory == _true )
	{
		MemoryModule* module = (MemoryModule*) mModule;

		// Get code base address.
		_byte* code = module->mCodeBase;

		// Get image header.
		IMAGE_NT_HEADERS* memheader	= (IMAGE_NT_HEADERS*) module->mNTHeaders;

		// Get image relocation directory.
		IMAGE_DATA_DIRECTORY* directory = &( memheader->OptionalHeader.DataDirectory[ IMAGE_DIRECTORY_ENTRY_EXPORT ] );

		// no export table found
		if ( directory->Size == 0 )
			return _null;

		// Get relocation information.
		IMAGE_EXPORT_DIRECTORY* exports = (IMAGE_EXPORT_DIRECTORY*)( code + directory->VirtualAddress );

		// DLL doesn't export anything.
		if ( exports->NumberOfNames == 0 || exports->NumberOfFunctions == 0 )
			return _null;

		// Search function name in list of exported names.
		_dword* nameref = (DWORD*) ( code + exports->AddressOfNames );
		_word*  ordinal = (WORD*) ( code + exports->AddressOfNameOrdinals );

		_dword index = -1;

		// Search function by name.
		for ( _dword i = 0; i < exports->NumberOfNames; i ++ )
		{
			if ( ::strcmp( ansiname, (const _chara*) ( code + *nameref ) ) != 0 )
			{
				nameref ++;
				ordinal ++;

				continue;
			}

			index = *ordinal;
			break;
		}

		// Exported symbol not found.
		if ( index == -1 )
			return _null;

		// Name <-> ordinal number don't match.
		if ( index > exports->NumberOfFunctions )
			return _null;

		// AddressOfFunctions contains the RVAs to the "real" functions
		return ( code + * ( (DWORD*) ( code + exports->AddressOfFunctions + index * sizeof( _void* ) ) ) );
	}
	else
	{
		return ::GetProcAddress( (HMODULE) mModule, ansiname );
	}
}
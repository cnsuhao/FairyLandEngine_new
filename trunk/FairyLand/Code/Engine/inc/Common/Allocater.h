//============================================================================
// Allocater.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// Allocater
//----------------------------------------------------------------------------

//! This class represents a pre-allocated buffer, user can allocate from this buffer instead of from system heap ( much faster ).
class Allocater
{
private:
	enum _CHUNK_FLAG
	{
		_USED_CHUNK = 0x80706050,
		_FREE_CHUNK = 0xA0B0C0E0,
	};

	//! The allocate chunk header, linked in a chain.
	struct ChunkHeader
	{
		//! The page it belongs to.
		_void*		mPage;
		//! Chunk flag.
		_dword		mFlag;
		//! The size of the chunk, without the header.
		_dword		mSize;
	};

	//! The allocate chunk end, used as chunk marker.
	struct ChunkEnd
	{
		//! The chunk node pointer.
		_void*		mNode;
		//! Chunk flag.
		_dword		mFlag;
	};

	typedef Link< ChunkHeader > ChunkLink;

	//! The pre-allocated page from system heap, all pages linked in a chain.
	struct MemPage
	{
		//! The size of the page, without the header.
		_dword		mSize;

		//! The used space of the page, in number of bytes.
		_dword		mUsedSpace;
		//! The free space of the page, in number of bytes.
		_dword		mFreeSpace;
		//! The bound of the page.
		_byte*		mPageBound;

		//! The free chunk link of the page.
		ChunkLink	mFreeChunkLink;
	};

	typedef Array< MemPage* > PageArray;

	//! Enable free memory or not.
	_bool		mEnableFreeMemory;
	//! The size per page in number of bytes, not include page header.
	_dword		mPageSize;
	//! The array of page.
	PageArray	mPageArray;

	_void  CreatePage( MemPage* page, _dword size );

	_void  CreateChunk( MemPage* page, ChunkLink::Node* freenode, _dword size, _bool newnode );
	_void  ReleaseChunk( MemPage* page, ChunkLink::Node* freenode, _dword size );

	_void* CreateBuffer( MemPage* page, _dword size );

public:
	//! Constructor, create an empty allocater with specified page size.
	//! @param		pagesize	The size per page in number of bytes.
	Allocater( _dword pagesize = 1024 * 1024 );
	//! Destructor, delete all pre-allocated pages.
	//! @param		none
	~Allocater( );

	//! Enable free memory or not.
	//! @param		enable		Enable free memory or not.
	_void EnableFreeMemory( _bool enable );

	//! Allocates memory block in the allocater.
	//! @param		size		Bytes to be allocated from the buffer.
	//! @return		The allocated memory pointer.
	_void* Allocate( _dword size );
	_void* Allocate( const _void* pointer, _dword size );

	//! Deallocates memory block in the allocater, decrease the reference count of the block, when it reachs zero, then will be released.
	//! @param		pointer		Previously allocated memory pointer to be freed. 
	_void Free( const _void* pointer );

	//! Clear all allocated memory block.
	//! @param		none.
	_void Clear( );

	//! Get the space information of the allocater.
	//! @param		totalspace	The total allocated space, in number of bytes.
	//! @param		usedspace	The used space, in number of bytes.
	//! @param		freespace	The free space, in number of bytes.
	_void GetAllocationInfo( _dword& totalspace, _dword& usedspace, _dword& freespace ) const;
};

//----------------------------------------------------------------------------
// AllocaterSwitcher
//----------------------------------------------------------------------------

class AllocaterSwitcher
{
private:
	const Allocater*	mOldAllocater;
	const Allocater*	mNewAllocater;

public:
	AllocaterSwitcher( const Allocater* newallocater );
	~AllocaterSwitcher( );

	_void TurnOn( );
	_void TurnOff( );
};

};
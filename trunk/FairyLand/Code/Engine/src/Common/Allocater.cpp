//============================================================================
// Allocater.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// Allocater Implementation
//----------------------------------------------------------------------------

Allocater::Allocater( _dword pagesize )
{
	mPageSize			= pagesize;
	mEnableFreeMemory	= _true;
}

Allocater::~Allocater( )
{
	_void* tempallocater = Memory::GetAllocater( );
	Memory::SetAllocater( _null );

	for ( _dword i = 0; i < mPageArray.Number( ); i ++ )
		delete[] (_byte*) mPageArray[i];

	mPageArray.Clear( _true );

	Memory::SetAllocater( tempallocater );
}

_void Allocater::CreatePage( MemPage* page, _dword size )
{
	// Make sure page size is big enough.
	if ( size < sizeof( ChunkLink::Node ) + sizeof( ChunkEnd ) )
		size = sizeof( ChunkLink::Node ) + sizeof( ChunkEnd );

	page->mSize		 = size;
	page->mUsedSpace = 0;
	page->mFreeSpace = size;
	page->mPageBound = (_byte*) page + sizeof( MemPage ) + size;

	page->mFreeChunkLink.Clear( );

	ChunkLink::Node* freenode = (ChunkLink::Node*) ( (_byte*) page + sizeof( MemPage ) );

	// Create default free chunk.
	CreateChunk( page, freenode, size - sizeof( ChunkLink::Node ) - sizeof( ChunkEnd ), _true );
}

_void Allocater::CreateChunk( MemPage* page, ChunkLink::Node* freenode, _dword size, _bool newnode )
{
	// Set chunk header for this node.
	ChunkHeader* header = &freenode->mElement;
	header->mPage = page;
	header->mFlag = _FREE_CHUNK;
	header->mSize = size;

	// Set chunk end for this node.
	ChunkEnd* end = (ChunkEnd*)( (_byte*) freenode + sizeof( ChunkLink::Node ) + size );
	end->mNode = freenode;
	end->mFlag = _FREE_CHUNK;

	if ( newnode )
	{
		freenode->mNext = _null;
		freenode->mPrev = _null;

		// Adjust space.
		page->mUsedSpace += sizeof( ChunkLink::Node ) + sizeof( ChunkEnd );
		page->mFreeSpace -= sizeof( ChunkLink::Node ) + sizeof( ChunkEnd );

		// Put the free node into free chunk list.
		page->mFreeChunkLink.InsertTail( freenode );
	}
}

_void Allocater::ReleaseChunk( MemPage* page, ChunkLink::Node* freenode, _dword size )
{
	// Get next node.
	ChunkLink::Node* nextnode = (ChunkLink::Node*) ( (_byte*) freenode + size + sizeof( ChunkLink::Node ) + sizeof( ChunkEnd ) );

	// Try to combine with next page.
	if ( (_byte*) nextnode < (_byte*) page->mPageBound )
	{
		// Next chunk is free.
		if ( nextnode->mElement.mFlag == _FREE_CHUNK )
		{
			_dword deltasize = nextnode->mElement.mSize + sizeof( ChunkLink::Node ) + sizeof( ChunkEnd );

			size += deltasize;

			page->mUsedSpace += deltasize;
			page->mFreeSpace -= deltasize;

			page->mFreeChunkLink.Remove( nextnode );
		}
		// Previous chunk is used.
		else
		{
			FL_ASSERT( nextnode->mElement.mFlag == _USED_CHUNK )
		}
	}
	else
	{
		FL_ASSERT( (_byte*) nextnode == (_byte*) page->mPageBound )
	}

	page->mUsedSpace -= size + sizeof( ChunkLink::Node ) + sizeof( ChunkEnd );
	page->mFreeSpace += size + sizeof( ChunkLink::Node ) + sizeof( ChunkEnd );

	_bool recycled = _false;

	// Try to combine with previous page.
	if ( (_byte*) freenode > (_byte*) page + sizeof( MemPage ) )
	{
		// Get previous chunk end.
		ChunkEnd* end = (ChunkEnd*)( (_byte*) freenode - sizeof( ChunkEnd ) );

		// Previous chunk is free.
		if ( end->mFlag == _FREE_CHUNK )
		{
			// Get previous node.
			ChunkLink::Node* prevnode = (ChunkLink::Node*) end->mNode;
			FL_ASSERT( prevnode->mElement.mFlag == _FREE_CHUNK )

			_dword newsize = prevnode->mElement.mSize + size + sizeof( ChunkLink::Node ) + sizeof( ChunkEnd );

			// Adjust the previous node.
			CreateChunk( page, prevnode, newsize, _false );

			recycled = _true;
		}
		// Previous chunk is used.
		else
		{
			FL_ASSERT( end->mFlag == _USED_CHUNK )
		}
	}
	else
	{
		FL_ASSERT( (_byte*) freenode == (_byte*) page + sizeof( MemPage ) )
	}

	// We must recycle this chunk.
	if ( recycled == _false )
		CreateChunk( page, freenode, size, _true );
}

_void* Allocater::CreateBuffer( MemPage* page, _dword size )
{
	// Try to find a node big enough.
	ChunkLink::Node* freenode = page->mFreeChunkLink.GetHeadNode( );

	while ( freenode != _null )
	{
		// We found a free node is big enough.
		if ( freenode->mElement.mSize >= size )
			break;

		freenode = freenode->mNext;
	}

	// There is no candidate node to create chunk.
	if ( freenode == _null )
		return _null;

	FL_ASSERT( freenode->mElement.mFlag == _FREE_CHUNK )

	_dword freenodesize = freenode->mElement.mSize;

	page->mUsedSpace += size;
	page->mFreeSpace -= size;

	// Remove the node from free chunk link.
	page->mFreeChunkLink.Remove( freenode );

	// This free node is big enough, we can split it into two node.
	if ( freenodesize > size + sizeof( ChunkLink::Node ) + sizeof( ChunkEnd ) + 24 )
	{
		// Adjust node size.
		freenode->mElement.mSize = size;

		// Find the next node position.
		ChunkLink::Node* newnode = (ChunkLink::Node*)( (_byte*) freenode + size + sizeof( ChunkLink::Node ) + sizeof( ChunkEnd ) );

		CreateChunk( page, newnode, freenodesize - size - sizeof( ChunkLink::Node ) - sizeof( ChunkEnd ), _true );
	}

	// We use the free node as used node.
	freenode->mNext = _null;
	freenode->mPrev = _null;

	// Set chunk header for this node.
	ChunkHeader* header = &freenode->mElement;
	header->mFlag = _USED_CHUNK;

	// Set chunk end for this node.
	ChunkEnd* end = (ChunkEnd*)( (_byte*) freenode + sizeof( ChunkLink::Node ) + freenode->mElement.mSize );
	end->mNode = freenode;
	end->mFlag = _USED_CHUNK;

	return (_byte*) freenode + sizeof( ChunkLink::Node );
}

_void Allocater::EnableFreeMemory( _bool enable )
{
	mEnableFreeMemory = enable;
}

_void* Allocater::Allocate( _dword size )
{
	while ( mPageSize < size )
		mPageSize *= 2;

	for ( _dword i = 0; i < mPageArray.Number( ); i ++ )
	{
		MemPage* page = mPageArray[ mPageArray.Number( ) - i - 1 ];

		// This page has enough space to allocate memory block, but maybe cant because of fragment.
		if ( page->mFreeSpace >= size )
		{
			_void* pointer = CreateBuffer( page, size );
			if ( pointer != _null )
				return pointer;
		}
	}

	_void* tempallocater = Memory::GetAllocater( );
	Memory::SetAllocater( _null );

	// Create memory block to hold one page.
	_byte* buffer = new _byte[ mPageSize + sizeof( MemPage ) ];

	// Create new page to allocate memory block.
	MemPage* page = (MemPage*) buffer;
	CreatePage( page, mPageSize );

	mPageArray.Append( page );

	Memory::SetAllocater( tempallocater );

	// Create chunk in the page.
	return CreateBuffer( page, size );
}

_void* Allocater::Allocate( const _void* pointer, _dword size )
{
	_void* buffer = Allocate( size );

	if ( buffer == _null )
		return _null;

	Memory::MemCpy( buffer, pointer, size );

	return buffer;
}

_void Allocater::Free( const _void* pointer )
{
	if ( mEnableFreeMemory == _false )
		return;

	if ( pointer == _null )
		return;

	// Get the node pointer from memory pointer.
	ChunkLink::Node* node = (ChunkLink::Node*) ( (_byte*) pointer - sizeof( ChunkLink::Node ) );
	FL_ASSERT( node->mElement.mFlag == _USED_CHUNK )

	MemPage* page = (MemPage*) node->mElement.mPage;
	FL_ASSERT( page != _null )

	// Release the chunk in the page.
	ReleaseChunk( page, node, node->mElement.mSize );
}

_void Allocater::Clear( )
{
	for ( _dword i = 0; i < mPageArray.Number( ); i ++ )
		CreatePage( mPageArray[i], mPageArray[i]->mSize );
}

_void Allocater::GetAllocationInfo( _dword& totalspace, _dword& usedspace, _dword& freespace ) const
{
	totalspace	= 0;
	usedspace	= 0;
	freespace	= 0;

	for ( _dword i = 0; i < mPageArray.Number( ); i ++ )
	{
		totalspace	+= mPageArray[i]->mSize;
		usedspace	+= mPageArray[i]->mUsedSpace;
		freespace	+= mPageArray[i]->mFreeSpace;
	}
}

//----------------------------------------------------------------------------
// AllocaterSwitcher
//----------------------------------------------------------------------------

AllocaterSwitcher::AllocaterSwitcher( const Allocater* newallocater )
{
	mOldAllocater = (const Allocater*) Memory::GetAllocater( );
	mNewAllocater = newallocater;

	Memory::SetAllocater( (_void*) newallocater );
}

AllocaterSwitcher::~AllocaterSwitcher( )
{
	Memory::SetAllocater( (_void*) mOldAllocater );
}

_void AllocaterSwitcher::TurnOn( )
{
	Memory::SetAllocater( (_void*) mNewAllocater );
}

_void AllocaterSwitcher::TurnOff( )
{
	Memory::SetAllocater( (_void*) mOldAllocater );
}
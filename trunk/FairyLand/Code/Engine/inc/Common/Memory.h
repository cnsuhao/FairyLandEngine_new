//============================================================================
// Memory.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

class MonitorWindow;

//----------------------------------------------------------------------------
// MemoryAllocation
//----------------------------------------------------------------------------

//! This class represents memory allocation information, used for memory track.
struct MemoryAllocation
{
	//! Memory block base address.
	_void*		mAddress;
	//! Memory block size.
	_dword		mSize;
	//! The function call stack where the allocation occur.
	CallStack	mCallStack;
};

//----------------------------------------------------------------------------
// Memory
//----------------------------------------------------------------------------

//! This class trace memory allocation to detect memory leak and allocate times etc.
class Memory : public Link< MemoryAllocation >
{
public:
	enum _DEBUG_MODE
	{
		_TRACE_NONE		= 0,
		_TRACE_SIZE		= 1,
		_TRACE_LEAK		= 2,
		_TRACE_STACK	= 3,
	};

private:
	//! The critical section used internal.
	Lock			mMemoryLock;

	//! Debug mode of memory.
	_dword			mDebugMode;

	//! Allocation happen times.
	_dword			mHappenAllocTimes;

	//! The current allocation times.
	_dword			mCurAllocTimes;
	//! The current allocation total size in number of bytes.
	_dword			mCurAllocSize;
	//! The maximal allocation times.
	_dword			mMaxAllocTimes;
	//! The maximal allocation total size in number of bytes.
	_dword			mMaxAllocSize;

	//! Monitor window of memory allocation.
	MonitorWindow*	mMemoryMonitorWindow;
	//! Monitor ID of memory allocation.
	_dword			mMemoryMonitorID;

	//! Callback function for update memory monitor.
	//! @param		monitorid	The monitor to update.
	static _void OnFreshMemoryMonitor( _dword monitorid );

public:
	//! The name of the source file that will allocate memory.
	static _char*	sOwnerFileName;
	//! The line number of the source file where the allocation will occur.
	static _dword	sOwnerLineNumber;
	//! Allocater use to create memory.
	static _void*	sAllocater;

	//! Initialize memory system.
	//! @param		debugmode	Debug mode of memory system.
	static _void Initialize( _dword debugmode );

	//! Finalize memory system.
	//! @param		none.
	static _void Finalize( );

	//! Set debug mode.
	//! @param		debugmode	Debug mode of memory system.
	static _void SetDebugMode( _dword debugmode );
	//! Get debug mode.
	//! @param		none
	//! @return		Debug mode of memory system.
	static _dword GetDebugMode( );

	//! Set allocater to memory system.
	//! @param		allocater	Allocater to be used.
	static _void SetAllocater( _void* allocater );
	//! Get allocater of memory system.
	//! @return		Allocater using.
	static _void* GetAllocater( );

	//! Sets buffers to a specified value.
	//! @param		desbuffer	Pointer to destination.
	//! @param		value		Value to set.
	//! @param		length		Number of bytes to set.
	static _void MemSet( _void* desbuffer, _byte value, _dword length );
	//! Compare values in two buffers.
	//! @param		buffer1		First buffer.
	//! @param		buffer2		Second buffer.
	//! @param		length		Number of bytes to compare.
	//! @param		True if buffer1 identical to buffer2, false otherwise.
	static _bool MemCmp( const _void* buffer1, const _void* buffer2, _dword length );
	//! Copies values between buffers.
	//! @param		desbuffer	Allocater to copy into.
	//! @param		srcbuffer	Allocater to copy from.
	//! @param		length		Number of bytes to copy.
	static _void MemCpy( _void* desbuffer, const _void* srcbuffer, _dword length );
	//! Moves one buffer to another.
	//! @param		desbuffer	Allocater to move into.
	//! @param		srcbuffer	Allocater to move from.
	//! @param		length		Number of bytes to move.
	static _void MemMove( _void* desbuffer, const _void* srcbuffer, _dword length );

	//! Allocate memory from heap.
	//! @param		size		The size of memory will be allocated in number of bytes.
	//! @return		The pointer to the allocated memory block.
	static _void* HeapAlloc( _dword size );
	//! Frees a memory block allocated.
	//! @param		pointer		The pointer to the memory block to be freed.
	static _void HeapFree( _void* pointer );

	//! Overload new operator, allocate memory from heap and trace the allocation information.
	//! @param		size		The size of memory will be allocated in number of bytes.
	//! @return		The pointer to the allocated memory block.
	static _void* OperatorNew( _dword size );
	//! Overload delete operator, frees a memory block allocated and remove allocation information.
	//! @param		pointer		The pointer to the memory block to be freed.
	static _void OperatorDelete( _void* pointer );

	//! Get current allocation times.
	//! @param		none
	//! @return		The current allocation times.
	static _dword GetCurAllocTimes( );
	//! Get current allocation total size in number of bytes.
	//! @param		none
	//! @return		The current allocation total size in number of bytes.
	static _dword GetCurAllocSize( );
	//! Get maximal allocation times.
	//! @param		none
	//! @return		The maximal allocation times.
	static _dword GetMaxAllocTimes( );
	//! Get maximal allocation total size in number of bytes.
	//! @param		none
	//! @return		The maximal allocation total size in number of bytes.
	static _dword GetMaxAllocSize( );

	//! Create referenced buffer.
	//! @param		size		The size of memory will be allocated in number of bytes.
	//! @return		The pointer to the allocated memory block.
	static _void* CreateReferencedBuffer( _dword size );
	//! Reserve referenced buffer.
	//! @param		pointer		The pointer to the memory block to be reserved.
	static _void  ReserveReferencedBuffer( _void* buffer );
	//! Release referenced buffer.
	//! @param		pointer		The pointer to the memory block to be released.
	//! @return		True if the buffer is really released, or false if the buffer is just dereferenced.
	static _bool  ReleaseReferencedBuffer( _void* buffer );
};

};

//----------------------------------------------------------------------------
// Operator new / delete
//----------------------------------------------------------------------------

#ifndef _DISABLE_NEW_REDIRECTION

#ifdef _MEMORY_ENABLE

inline void* operator new ( size_t size )
{
	return FL::Memory::OperatorNew( (FL::_dword) size );
}

inline void* operator new[] ( size_t size )
{
	return FL::Memory::OperatorNew( (FL::_dword) size );
}

inline void operator delete ( void* pointer )
{
	return FL::Memory::OperatorDelete( (FL::_void*) pointer );
}

inline void operator delete[] ( void* pointer )
{
	return FL::Memory::OperatorDelete( (FL::_void*) pointer );
}

#define new ( FL::Memory::sOwnerFileName = _FILENAME, FL::Memory::sOwnerLineNumber = _LINENUMBER, 0 ) ? 0 : new

#else

inline void* operator new ( size_t size )
{
	return FL::Memory::HeapAlloc( (FL::_dword) size );
}

inline void* operator new[] ( size_t size )
{
	return FL::Memory::HeapAlloc( (FL::_dword) size );
}

inline void operator delete ( void* pointer )
{
	return FL::Memory::HeapFree( (FL::_void*) pointer );
}

inline void operator delete[] ( void* pointer )
{
	return FL::Memory::HeapFree( (FL::_void*) pointer );
}

#endif

#endif
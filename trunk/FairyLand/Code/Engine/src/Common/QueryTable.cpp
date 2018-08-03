//============================================================================
// QueryTable.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

QueryTable::QueryTable( ) : mFuncPointer( _null )
{	
}

QueryTable::~QueryTable( )
{
}

_void QueryTable::InsertItem( StringPtr key, StringPtr param, StringPtr data )
{
	mTable.InsertAscending( TableItem( mAllocater, key, param, data ) );
}

_void QueryTable::SetItem( StringPtr key, StringPtr param, StringPtr data )
{
	TableItemArray::Iterator iterator = mTable.SearchAscending( key );

	// Search forward
	for ( TableItemArray::Iterator it = iterator; it.Valid( ); it -- )
	{
		TableItem& item = it;

		if ( key != item.mKey )
			break;

		if ( item.mParam == param )
		{
			mAllocater.Free( (const _char*) item.mData );
			_char* allocaterdata = (_char*)(mAllocater.Allocate( data.SizeOfBytes( ) ));
			StringFormatter::CopyString( allocaterdata, data, -1 );
			item.mData = allocaterdata;
			return;
		}
	}

	// Search backward
	for ( TableItemArray::Iterator it = iterator; it.Valid( ); it ++ )
	{
		TableItem& item = it;

		if ( key != item.mKey )
			break;

		if ( item.mParam == param )
		{
			mAllocater.Free( (const _char*) item.mData );
			_char* allocaterdata = (_char*)(mAllocater.Allocate( data.SizeOfBytes( ) ));
			StringFormatter::CopyString( allocaterdata, data, -1 );
			item.mData = allocaterdata;
			return;
		}
	}
}

_void QueryTable::RemoveItem( StringPtr key )
{
	TableItemArray::Iterator iterator = mTable.SearchAscending( key );

	// Search forward
	for ( TableItemArray::Iterator it = iterator; it.Valid( ); it --)
	{
		const TableItem& item = it;

		if ( key != item.mKey )
			break;

		mAllocater.Free( (const _char*) item.mKey );
		mAllocater.Free( (const _char*) item.mParam );
		mAllocater.Free( (const _char*) item.mData );
		mTable.Remove( it );	
	}

	// Search backward
	for ( TableItemArray::Iterator it = iterator; it.Valid( ); )
	{
		const TableItem& item = it;

		if ( key != item.mKey )
			break;

		mAllocater.Free( (const _char*) item.mKey );
		mAllocater.Free( (const _char*) item.mParam );
		mAllocater.Free( (const _char*) item.mData );
		mTable.Remove( it );
	}
}

_void QueryTable::RemoveItem( StringPtr key, StringPtr param )
{
	TableItemArray::Iterator iterator = mTable.SearchAscending( key );

	// Search forward
	for ( TableItemArray::Iterator it = iterator; it.Valid( ); it -- )
	{
		const TableItem& item = it;

		if ( key != item.mKey )
			break;

		if ( item.mParam == param )
		{
			mAllocater.Free( (const _char*) item.mKey );
			mAllocater.Free( (const _char*) item.mParam );
			mAllocater.Free( (const _char*) item.mData );
			mTable.Remove( it );
			return;
		}
	}

	// Search backward
	for ( TableItemArray::Iterator it = iterator; it.Valid( ); it ++ )
	{
		const TableItem& item = it;

		if ( key != item.mKey )
			break;

		if ( item.mParam == param )
		{
			mAllocater.Free( (const _char*) item.mKey );
			mAllocater.Free( (const _char*) item.mParam );
			mAllocater.Free( (const _char*) item.mData );
			mTable.Remove( it );
			return;
		}
	}
}

_void QueryTable::Clear( )
{
	mTable.Clear( );
	mAllocater.Clear( );
}

StringPtr QueryTable::SearchItem( StringPtr key, StringPtr param, _bool callback )
{
	if ( mTable.Number( ) == 0 )
		return L"";

	TableItemArray::Iterator iterator = mTable.SearchAscending( key );

	// Search forward
	for ( TableItemArray::Iterator it = iterator; it.Valid( ); it -- )
	{
		const TableItem& item = it;

		if ( key != item.mKey )
			break;

		if ( item.mParam == param )
		{
			if ( callback )
				RunCallback( item.mData );

			return item.mData;
		}
	}

	// Search backward
	for ( TableItemArray::Iterator it = iterator; it.Valid( ); it ++ )
	{
		const TableItem& item = it;

		if ( key != item.mKey )
			break;

		if ( item.mParam == param )
		{
			if ( callback )
				RunCallback( item.mData );

			return item.mData;
		}
	}

	return L"";
}

_void QueryTable::SetCallback( OnFoundResult funcpointer, _dword param1, _dword param2 )
{
	mFuncPointer	= funcpointer;
	mParameter1		= param1;
	mParameter2		= param2;
}

_void QueryTable::RunCallback( StringPtr data )
{
	if ( mFuncPointer != _null )
		(*mFuncPointer)( data, mParameter1, mParameter2 );
}

_dword QueryTable::GetItemNum( )
{
	return mTable.Number( );
}

_void QueryTable::GetItem( _dword index, String& key, String& param, String& data )
{
	key   = mTable[index].mKey;
	param = mTable[index].mParam;
	data  = mTable[index].mData;
}
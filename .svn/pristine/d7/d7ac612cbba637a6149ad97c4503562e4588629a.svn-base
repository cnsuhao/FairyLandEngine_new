//============================================================================
// QueryTable.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

// Query table
class QueryTable
{
public:
	typedef _void (*OnFoundResult)( StringPtr data, _dword param1, _dword param2 );

private:
	// Query Item
	class TableItem
	{
	public:
		StringPtr	mKey;
		StringPtr	mData;
		StringPtr	mParam;

		TableItem( )
		{
			mKey   = _null;
			mData  = _null;
			mParam = _null;
		}

		TableItem( Allocater& allocater, StringPtr key, StringPtr param, StringPtr data )
		{
			_char* allocaterkey = (_char*)(allocater.Allocate( key.SizeOfBytes( ) ));
			StringFormatter::CopyString( allocaterkey, key, -1 );
			mKey = allocaterkey;

			_char* allocaterparam = (_char*)(allocater.Allocate( param.SizeOfBytes( ) ));
			StringFormatter::CopyString( allocaterparam, param, -1 );
			mParam = allocaterparam;

			_char* allocaterdata = (_char*)(allocater.Allocate( data.SizeOfBytes( ) ));
			StringFormatter::CopyString( allocaterdata, data, -1 );
			mData = allocaterdata;
		}

		operator StringPtr ( ) const
		{ return mKey;}
	};

	typedef Array< TableItem, StringPtr > TableItemArray;

	TableItemArray	mTable;
	OnFoundResult	mFuncPointer;
	_dword			mParameter1;
	_dword			mParameter2;

	Allocater		mAllocater;

public:
	QueryTable( );
	~QueryTable( );

public:
	_void		InsertItem( StringPtr key, StringPtr param, StringPtr data );
	_void		SetItem( StringPtr key, StringPtr param, StringPtr data );
	_void		RemoveItem( StringPtr key );
	_void		RemoveItem( StringPtr key, StringPtr param );
	_void		Clear( );

	StringPtr	SearchItem( StringPtr key, StringPtr param, _bool callback = _true );

	_void		SetCallback( OnFoundResult funcpointer, _dword param1, _dword param2 );
	_void		RunCallback( StringPtr data );

	_dword		GetItemNum( );
	_void		GetItem( _dword index, String& key, String& param, String& data );
};



};
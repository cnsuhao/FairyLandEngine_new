//============================================================================
// MonitorWindow.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// MonitorWindow
//----------------------------------------------------------------------------

class MonitorWindow : public Singleton< MonitorWindow >
{
public:
	enum
	{
		_RENDER_STYLE_LINE		= 0,
		_RENDER_STYLE_COLUMN	= 1,

		_COLUMN_TYPE_STRING		= 0,
		_COLUMN_TYPE_LONG		= 1,
		_COLUMN_TYPE_FLOAT		= 2,

		_COLUMN_SORT_ASCENDING	= 0,
		_COLUMN_SORT_DESCENDING	= 1,
		_COLUMN_SORT_REVERSE	= 2,
		_COLUMN_SORT_SUBSORT	= 8,
	};

	typedef _void (*OnNotice)( StringPtr name );
	typedef _bool (*OnCommand)( _char* command[], _dword number );
	typedef _void (*OnFreshMonitor)( _dword monitorid );

private:
	enum
	{
		_MONITOR_DIALOG_WINDOW	= 1,
		_MONITOR_OUTPUT_WINDOW	= 2,
		_MONITOR_GRAPH_RECORD	= 3,
		_MONITOR_LIST_RECORD	= 4,

		_MAX_GRAPH_ITEM_NUMBER	= 8,

		_DEFAULT_OUTPUT_MAXLINE	= 1024,
		_DEFAULT_RECORD_PERIOD	= 60,

		_FRESH_MONITOR_INTERVAL	= 2000,

		_DELAY_MESSAGE_CLEAR	= 0x421,
		_DELAY_MESSAGE_STATE	= 0x422,
		_DELAY_MESSAGE_PRINT	= 0x423,
		_DELAY_MESSAGE_FRESH	= 0x424,
	};

	struct Monitor
	{
		String			mName;
		String			mTitle;
		_handle			mMonitorWindow;
		_dword			mMonitorID;
		_dword			mMonitorType;
		_dword			mUserData;

		OnFreshMonitor	mFreshFunc;
	};

	typedef Pool< Monitor* > MonitorPool;

	// Dialog control.
	struct Control
	{
		_handle			mControlWindow;
		String			mControlName;
		OnNotice		mNoticeFunc;
	};

	typedef Array< Control > ControlArray;

	// Dialog window.
	struct DialogWindow : public Monitor
	{
		ControlArray	mControlArray;
	};

	// Output window.
	struct OutputWindow : public Monitor
	{
		_dword			mMaxLine;
		_dword			mBackColor;
	};

	// Graph record monitor.
	struct GraphRecord : public Monitor
	{
		_dword			mRenderStyle;
		_dword			mRecordPeriod;

		String			mItemName[ _MAX_GRAPH_ITEM_NUMBER ];
		_dword			mPenWidth[ _MAX_GRAPH_ITEM_NUMBER ];
		_dword			mPenColor[ _MAX_GRAPH_ITEM_NUMBER ];
		_dword			mValueRange[ _MAX_GRAPH_ITEM_NUMBER ];
		_dword*			mValueList[ _MAX_GRAPH_ITEM_NUMBER ];
	};

	typedef Quadruple< String, _dword, _dword, _dword > TitleItem;
	typedef Pair< _dword, String >						RecordItem;
	typedef Array< TitleItem >							TitleArray;
	typedef Array< RecordItem, _dword >					RecordArray;

	// List record monitor.
	struct ListRecord : public Monitor
	{
		_char			mDelimiter;
		_dword			mMainSortColumn;
		_dword			mSubSortColumn;
		_dword			mFreshTickcount;

		TitleArray		mTitleArray;
		RecordArray		mRecordArray;
	};

	// Delay print structure.
	struct DelayPrintStruct
	{
		_dword			mMonitorID;
		_dword			mColor;
		_char			mString[1];
	};

	_handle				mWindow;
	_handle				mInputWindow;
	_handle				mStateWindow;
	_handle				mSaveWindow;
	_handle				mClearWindow;
	_handle				mFont;

	_bool				mMainWindow;
	_bool				mShowing;
	_bool				mTopmost;

	_dword				mActiveColumn;
	_dword				mActiveSortMode;
	_dword				mActiveMonitor;

	OnCommand			mCommandFunc;
	_dword				mTimerHandle;

	MonitorPool			mMonitorPool;

	static _long _stdcall MonitorWndProc( _handle window, _dword msgid, _dword wparam, _dword lparam );
	static _long _stdcall DialogWndProc( _handle window, _dword msgid, _dword wparam, _dword lparam );
	static _long _stdcall GraphWndProc( _handle window, _dword msgid, _dword wparam, _dword lparam );
	static _long _stdcall ListCompareFunc( _dword param1, _dword param2, _dword paramsort );

	Monitor*	GetMonitor( _dword monitorid, _dword type ) const;
	Control*	GetControl( _dword monitorid, StringPtr name ) const;
	_void		RenderRange( const GraphRecord& monitor, _handle hdc, _dword width, _dword height );
	_void		RenderValue( const GraphRecord& monitor, _handle hdc, _dword width, _dword height );
	_void		FreshList( ListRecord& monitor, _handle list );

public:
	MonitorWindow( );
	~MonitorWindow( );

	// Output record window.
	_dword		CreateDialogWindow( StringPtr name );
	_bool		CreateDialogStatic( _dword monitorid, StringPtr name, StringPtr title, const Point& position, const Point& size, OnNotice funcpointer = _null );
	_bool		CreateDialogButton( _dword monitorid, StringPtr name, StringPtr title, const Point& position, const Point& size, OnNotice funcpointer = _null );
	_bool		CreateDialogEdit( _dword monitorid, StringPtr name, StringPtr title, const Point& position, const Point& size, OnNotice funcpointer = _null );
	_bool		EnableDialogControl( _dword monitorid, StringPtr name, _bool enable );
	_bool		ShowDialogControl( _dword monitorid, StringPtr name, _bool show );
	_bool		SetDialogControlTitle( _dword monitorid, StringPtr name, StringPtr title );
	String		GetDialogControlTitle( _dword monitorid, StringPtr name ) const;

	// Output record window.
	_dword		CreateOutputWindow( StringPtr name );
	_bool		SetOutputWindowStyle( _dword monitorid, _dword maxline, _dword backcolor );
	_bool		PrintString( _dword monitorid, _dword color, const _char* format, ... );

	// Graph record window.
	_dword		CreateGraphRecord( StringPtr name );
	_bool		SetGraphRecordStyle( _dword monitorid, _dword style );
	_bool		SetGraphRecordPeriod( _dword monitorid, _dword period );
	_bool		SetGraphRecordItem( _dword monitorid, _dword subid, StringPtr name, _dword range, _dword width, _dword color );
	_bool		AddGraphRecordSample( _dword monitorid, _dword subid, _dword value );

	// List record window.
	_dword		CreateListRecord( StringPtr name );
	_bool		SetListRecordDelimiter( _dword monitorid, _char delimiter );
	_bool		SetListRecordColumn( _dword monitorid, StringPtr title, _dword width, _dword type );
	_bool		AddListRecordSample( _dword monitorid, _dword key, StringPtr value );
	_bool		DelListRecordSample( _dword monitorid, _dword key );
	_bool		SortListRecord( _dword monitorid, _dword column, _dword sortmode = _COLUMN_SORT_REVERSE );

	// Common function.
	_dword		GetActiveMonitor( ) const;
	_dword		SearchMonitor( StringPtr name ) const;

	_void		SetMonitorUserData( _dword monitorid, _dword userdata );
	_dword		GetMonitorUserData( _dword monitorid ) const;

	_bool		RemoveMonitor( _dword monitorid );
	_bool		ActiveMonitor( _dword monitorid );
	_bool		ClearMonitor( _dword monitorid );
	_bool		SetFreshMonitorCallback( _dword monitorid, OnFreshMonitor funcpointer );
	_bool		FreshMonitor( _dword monitorid );
	_void		ClearAllMonitor( );

	// Save & load monitor.
	_bool		SaveMonitor( _dword monitorid, StringPtr filename ) const;
	_bool		LoadMonitor( StringPtr filename );

	// Parent Window.
	_void		SetCommandCallback( OnCommand funcpointer );
	_void		SetState( const _char* format, ... );
	_void		SetTitle( StringPtr title );

	_bool		IsWindowShowing( ) const;
	_void		RunWindow( );
	_void		ShowWindow( );
	_void		HideWindow( );
};

};
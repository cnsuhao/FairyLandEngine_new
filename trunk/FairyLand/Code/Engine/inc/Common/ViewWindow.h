//============================================================================
// ViewWindow.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// ViewWindow
//----------------------------------------------------------------------------

class ViewWindow : public MainWindow
{
public:
	typedef _bool (*OnCommand)( _char* command[], _dword number );
	typedef _void (*OnControlPaint)( _handle window, _dword userdata, _handle hdc, const Rect& rect );
	typedef _void (*OnControlMessage)( _handle window, _dword userdata, _dword msgid, _dword wparam, _dword lparam );

private:
	struct Control
	{
		String				mTitle;
		_handle				mWindow;
		_dword				mControlID;
		_dword				mUserData;
		OnControlPaint		mPaintFunc;
		OnControlMessage	mMessageFunc;
	};

	typedef Array< Control > ControlArray;

	_handle			mInputWindow;
	_handle			mStateWindow;
	_handle			mFont;
	_dword			mFontWidth;
	_dword			mFontHeight;
	OnCommand		mCommandFunc;

	_dword			mNextControlID;
	_dword			mActiveControlID;
	ControlArray	mControlArray;

	static _long __stdcall ViewWndProc( _handle window, _dword msgid, _dword wparam, _dword lparam );

	static _bool OnViewMessage( _dword msgid, _dword wparam, _dword lparam );

	Control* GetControl( _dword controlid );
	_void CreateControl( Control& control );

public:
	ViewWindow( );
	~ViewWindow( );

	inline _void SetCommandCallback( OnCommand funcpointer );

	_void SetFont( StringPtr fontname, _dword size, _dword weight = 400 );

	_dword CreateEditControl( StringPtr title, _dword maxlinenumber, _dword contorlid = -1 );
	_dword CreateUserControl( StringPtr title, _dword userdata, OnControlPaint paintfunc, OnControlMessage messagefunc, _dword contorlid = -1 );
	_bool ActiveControl( _dword controlid );
	_bool DeleteControl( _dword controlid );

	_dword GetActiveControlID( ) const;

	_void ChangeState( const _char* format, ... );
	_void PrintString( _dword controlid, _dword color, const _char* format, ... );
	_void ClearString( _dword controlid );
};

//----------------------------------------------------------------------------
// ViewWindow Implementation
//----------------------------------------------------------------------------

_void ViewWindow::SetCommandCallback( OnCommand funcpointer )
{
	mCommandFunc = funcpointer;
}

};
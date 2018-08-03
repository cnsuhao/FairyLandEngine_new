//============================================================================
// MainWindow.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// MainWindow
//----------------------------------------------------------------------------

class MainWindow
{
public:
	enum _WINDOW_FLAG
	{
		_WINDOW_OVERLAPPED	= 0x00000000,
		_WINDOW_POPUP		= 0x00000001,
		_WINDOW_TOOLBAR		= 0x00000002,
		_WINDOW_TOPMOST		= 0x00000004,
	};

	typedef _bool (*OnInitialize)( );
	typedef _void (*OnFinalize)( );
	typedef _bool (*OnMessage)( _dword msgid, _dword wparam, _dword lparam );
	typedef _bool (*OnIdle)( _dword elapse );
	typedef _bool (*OnCloseConfirm)( );

private:
	MainWindow( const MainWindow& mainwindow );
	MainWindow& operator = ( const MainWindow& mainwindow );

protected:
	String			mTitle;
	_handle			mWindow;
	_float			mAlpha;
	_bool			mUserFrame;

	_bool			mPrevMaximized;
	_dword			mProcessMessageResult;

	OnInitialize	mInitializeFunc;
	OnFinalize		mFinalizeFunc;
	OnMessage		mPreMessageFunc;
	OnMessage		mMessageFunc;
	OnIdle			mIdleFunc;
	OnCloseConfirm	mCloseConfirmFunc;

	static _long __stdcall WndProc( _handle window, _dword msgid, _dword wparam, _dword lparam );

public:
	MainWindow( );
	~MainWindow( );

	static MainWindow* GetMainWindowPointer( );
	static MainWindow& GetMainWindow( );

	inline operator _handle ( ) const;

	inline _void SetInitializeCallback( OnInitialize funcpointer );
	inline _void SetFinalizeCallback( OnFinalize funcpointer );
	inline _void SetMessageCallback( OnMessage funcpointer );
	inline _void SetIdleCallback( OnIdle funcpointer );
	inline _void SetCloseConfirmCallback( OnCloseConfirm funcpointer );

	_bool Create( StringPtr title, const Point& position, const Point& size, _dword flag = 0 );
	_bool Create( StringPtr title, const Point& size, _dword flag = 0 );
	_void Close( );

	_void  SetAlpha( _float alpha );
	_float GetAlpha( ) const;

	_bool Show( );
	_bool Hide( );
	_void Run( _bool sleep = _true );

	_void CenterWindow( );
	_void MoveWindow( _long x, _long y, _long w, _long h );

	_void SetTitle( StringPtr title );
	StringPtr GetTitle( ) const;

	_void EnableUserFrame( _bool frame );

	_void SetProcessMessageResult( _dword result );
	_dword GetProcessMessageResult( ) const;

	_void Maximize( );
	_void Minimize( );
	_void Restore( );

	_bool IsWindowMaximized( ) const;
	_bool IsWindowMinimized( ) const;

	Rect GetClientRect( ) const;
	Rect GetWindowRect( ) const;
};

//----------------------------------------------------------------------------
// MainWindow Implementation
//----------------------------------------------------------------------------

MainWindow::operator _handle ( ) const
{
	return mWindow;
}

_void MainWindow::SetInitializeCallback( OnInitialize funcpointer )
{
	mInitializeFunc = funcpointer;
}

_void MainWindow::SetFinalizeCallback( OnFinalize funcpointer )
{
	mFinalizeFunc = funcpointer;
}

_void MainWindow::SetMessageCallback( OnMessage funcpointer )
{
	mMessageFunc = funcpointer;
}

_void MainWindow::SetIdleCallback( OnIdle funcpointer )
{
	mIdleFunc = funcpointer;
}

_void MainWindow::SetCloseConfirmCallback( OnCloseConfirm funcpointer )
{
	mCloseConfirmFunc = funcpointer;
}

};
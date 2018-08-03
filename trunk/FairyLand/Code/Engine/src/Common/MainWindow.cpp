//============================================================================
// MainWindow.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// MainWindow Implementation
//----------------------------------------------------------------------------

MainWindow::MainWindow( ) : mWindow( _null )
{
	mAlpha					= 1.0f;
	mUserFrame				= _false;
	mPrevMaximized			= _false;
	mProcessMessageResult	= -1;

	mInitializeFunc			= _null;
	mFinalizeFunc			= _null;
	mPreMessageFunc			= _null;
	mMessageFunc			= _null;
	mIdleFunc				= _null;
	mCloseConfirmFunc		= _null;
}

MainWindow::~MainWindow( )
{
	Close( );
}

MainWindow* MainWindow::GetMainWindowPointer( )
{
	return GetFairyLandGlobal( ).mMainWindow;
}

MainWindow& MainWindow::GetMainWindow( )
{
	return *( GetFairyLandGlobal( ).mMainWindow );
}

_long MainWindow::WndProc( _handle window, _dword msgid, _dword wparam, _dword lparam )
{
	MainWindow* mainwindow = (MainWindow*) ::GetWindowLong( (HWND) window, GWL_USERDATA );

	if ( mainwindow == _null )
	{
		mainwindow = &MainWindow::GetMainWindow( );

		if ( mainwindow != _null )
			mainwindow->mWindow = window;
	}

	if ( mainwindow == _null )
		return (_long) ::DefWindowProc( (HWND) window, msgid, wparam, lparam );

	_dword idbackup = msgid;

	if ( msgid == WM_CLOSE )
	{
		if ( mainwindow->mCloseConfirmFunc != _null )
		{
			if ( (*mainwindow->mCloseConfirmFunc)( ) == _false )
				return 0;
		}

		::PostQuitMessage( 0 );
	}
	else
	{
		if ( mainwindow->mPreMessageFunc != _null )
		{
			if ( (*mainwindow->mPreMessageFunc)( msgid, wparam, lparam ) == _false )
				return 0;
		}
		else
		{
			if ( msgid == WM_PAINT )
			{
				PAINTSTRUCT paintstruct;

				::BeginPaint( (HWND) window, &paintstruct );
				::EndPaint( (HWND) window, &paintstruct );
			}
			else if ( mainwindow->mMessageFunc != _null )
			{
				if ( (*mainwindow->mMessageFunc)( msgid, wparam, lparam ) == _false )
				{
					_dword result = mainwindow->mProcessMessageResult;
					mainwindow->mProcessMessageResult = 0;

					return result;
				}

				if ( WM_SETCURSOR == msgid && mainwindow->mProcessMessageResult == -1 )
				{
					mainwindow->mProcessMessageResult = 0;
					return 0;
				}
			}
		}
	}

	return (_long) ::DefWindowProc( (HWND) window, idbackup, wparam, lparam );
}

_bool MainWindow::Create( StringPtr title, const Point& position, const Point& size, _dword flag )
{
	WNDCLASSEX windowclass		= { sizeof( WNDCLASSEX ) };
	windowclass.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	windowclass.lpfnWndProc		= (WNDPROC) WndProc;
	windowclass.hInstance		= ::GetModuleHandle( _null );
	windowclass.hIcon			= ::LoadIcon( _null, IDI_WINLOGO );
	windowclass.hCursor			= ::LoadCursor( _null, IDC_ARROW );
	windowclass.lpszClassName	= L"FairyLand Main Window";

	::RegisterClassEx( &windowclass );

	_dword style = 0, styleex = 0;

	if ( flag & _WINDOW_POPUP )
		style = WS_POPUP | WS_SYSMENU | WS_MINIMIZEBOX;
	else
		style = WS_OVERLAPPEDWINDOW;

	if ( flag & _WINDOW_TOOLBAR )
		styleex |= WS_EX_TOOLWINDOW | WS_EX_APPWINDOW;

	if ( flag & _WINDOW_TOPMOST )
		styleex |= WS_EX_TOPMOST;

	RECT windowrect = { 0, 0, size.x, size.y };

	// Convert window rect to client rect.
	if ( ::AdjustWindowRectEx( &windowrect, style, _false, styleex ) == 0 )
		return _false;

	mWindow = ::CreateWindowEx( styleex, windowclass.lpszClassName, title, style, position.x, position.y,
		windowrect.right - windowrect.left, windowrect.bottom - windowrect.top, _null, _null, _null, _null );

	if ( mWindow == _null )
		return _false;

	GetFairyLandGlobal( ).mMainWindow = this;

	::SetWindowLong( (HWND) mWindow, GWL_USERDATA, (_long) this );

	if ( mPreMessageFunc != _null )
		(*mPreMessageFunc)( WM_CREATE, 0, 0 );

	if ( mInitializeFunc != _null )
	{
		if ( (*mInitializeFunc)( ) == _false )
			return _false;
	}

	mTitle = title;

	return _true;
}

_bool MainWindow::Create( StringPtr title, const Point& size, _dword flag )
{
	_long x = ( ::GetSystemMetrics( SM_CXSCREEN ) - size.x ) / 2;
	_long y = ( ::GetSystemMetrics( SM_CYSCREEN ) - size.y ) / 2;

	return Create( title, Point( x, y ), size, flag );
}

_void MainWindow::Close( )
{
	if ( mWindow != _null )
	{
		if ( mFinalizeFunc != _null )
			(*mFinalizeFunc)( );

		::DestroyWindow( (HWND) mWindow );
	}

	mWindow = _null;
}

_void MainWindow::SetAlpha( _float alpha )
{
	alpha = Math::Clamp( alpha, 0.0f, 1.0f );

	if ( mAlpha == alpha )
		return;

	if ( alpha == 1.0f )
	{
		::SetWindowLong( (HWND) mWindow, GWL_EXSTYLE , ::GetWindowLong( (HWND) mWindow, GWL_EXSTYLE ) & ~WS_EX_LAYERED );
	}
	else
	{
		if ( mAlpha == 1.0f )
			::SetWindowLong( (HWND) mWindow, GWL_EXSTYLE , ::GetWindowLong( (HWND) mWindow, GWL_EXSTYLE ) | WS_EX_LAYERED );

		::SetLayeredWindowAttributes( (HWND) mWindow, RGB( 255, 0, 255 ), (_byte) ( 255 * alpha ), LWA_COLORKEY | LWA_ALPHA );
	}

	mAlpha = alpha;
}

_float MainWindow::GetAlpha( ) const
{
	return mAlpha;
}

_bool MainWindow::Show( )
{
	if ( ::ShowWindow( (HWND) mWindow, SW_SHOW ) == 0 )
		return _false;

	if ( ::UpdateWindow( (HWND) mWindow ) == 0 )
		return _false;

	return _true;
}

_bool MainWindow::Hide( )
{
	if ( ::ShowWindow( (HWND) mWindow, SW_HIDE ) == 0 )
		return _false;

	return _true;
}

_void MainWindow::Run( _bool sleep )
{
	// Get start tickcount.
	_dword lasttickcount = System::GetCurrentTickcount( );

	MSG message = { 0 };
	while ( message.message != WM_QUIT )
	{
		if ( ::PeekMessage( &message, _null, 0, 0, PM_REMOVE ) )
		{
			if ( mPreMessageFunc != _null && message.message == WM_KEYDOWN && message.wParam == VK_RETURN )
			{
				(*mPreMessageFunc)( WM_USER + 24, 0, 0 );
			}
			else
			{
				::TranslateMessage( &message );
				::DispatchMessage( &message );
			}
		}
		else
		{
			_dword currtickcount = System::GetCurrentTickcount( );

			if ( mIdleFunc != _null )
			{
				_dword dwordelapse = currtickcount - lasttickcount;

				if ( (*mIdleFunc)( dwordelapse ) == _false )
					break;
			}

			lasttickcount = currtickcount;

			if ( mPreMessageFunc == _null )
			{
				if ( ::IsIconic( (HWND) mWindow ) )
					System::Sleep( 200 );
				else if ( ::GetForegroundWindow( ) != (HWND) mWindow )
					System::Sleep( 20 );
				else if ( mAlpha != 1.0f )
					System::Sleep( 20 );
				else if ( sleep )
					System::Sleep( 10 );
				else
					System::Sleep( 0 );
			}
			else
			{
				System::Sleep( 1 );
			}
		}
	}
}

_void MainWindow::CenterWindow( )
{
	Rect windowrect = GetWindowRect( );

	_long x = ( ::GetSystemMetrics( SM_CXSCREEN ) - windowrect.Width( ) ) / 2;
	_long y = ( ::GetSystemMetrics( SM_CYSCREEN ) - windowrect.Height( ) ) / 2;

	::MoveWindow( (HWND) mWindow, x, y, windowrect.Width( ), windowrect.Height( ), _true );
}

_void MainWindow::MoveWindow( _long x, _long y, _long w, _long h )
{
	::MoveWindow( (HWND) mWindow, x, y, w, h, _true );
}

_void MainWindow::SetTitle( StringPtr title )
{
	if ( mTitle == title )
		return;

	mTitle = title;

	if ( mUserFrame == _false )
	{
		::SetWindowText( (HWND) mWindow, title );
	}
	else
	{
		::SendMessage( (HWND) mWindow, WM_USER + 100, 0, 0 );
	}
}

StringPtr MainWindow::GetTitle( ) const
{
	return mTitle;
}

_void MainWindow::EnableUserFrame( _bool frame )
{
	mUserFrame = frame;
}

_void MainWindow::SetProcessMessageResult( _dword result )
{
	mProcessMessageResult = result;
}

_dword MainWindow::GetProcessMessageResult( ) const
{
	return mProcessMessageResult;
}

_void MainWindow::Maximize( )
{
	::ShowWindow( (HWND) mWindow, SW_SHOWMAXIMIZED );
}

_void MainWindow::Minimize( )
{
	::ShowWindow( (HWND) mWindow, SW_SHOWMINIMIZED );
}

_void MainWindow::Restore( )
{
	::ShowWindow( (HWND) mWindow, SW_RESTORE );
}

_bool MainWindow::IsWindowMaximized( ) const
{
	WINDOWPLACEMENT windowplacement = { 0 };

	::GetWindowPlacement( (HWND) mWindow, &windowplacement );

	return windowplacement.showCmd == SW_SHOWMAXIMIZED;
}

_bool MainWindow::IsWindowMinimized( ) const
{
	WINDOWPLACEMENT windowplacement = { 0 };

	::GetWindowPlacement( (HWND) mWindow, &windowplacement );

	return windowplacement.showCmd == SW_SHOWMINIMIZED;
}

Rect MainWindow::GetClientRect( ) const
{
	Rect rect;
	::GetClientRect( (HWND) mWindow, (RECT*) &rect );

	return rect;
}

Rect MainWindow::GetWindowRect( ) const
{
	Rect rect;
	::GetWindowRect( (HWND) mWindow, (RECT*) &rect );

	return rect;
}
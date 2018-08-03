//============================================================================
// ViewWindow.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// ViewWindow Implementation
//----------------------------------------------------------------------------

ViewWindow::ViewWindow( )
{
	mInputWindow		= _null;
	mStateWindow		= _null;
	mFont				= _null;
	mFontWidth			= 0;
	mFontHeight			= 0;
	mNextControlID		= 1;
	mActiveControlID	= 0;

	mPreMessageFunc = OnViewMessage;
	mCommandFunc	= _null;
}

ViewWindow::~ViewWindow( )
{
	if ( mFont != _null )
		::DeleteObject( mFont );
}

_long ViewWindow::ViewWndProc( _handle window, _dword msgid, _dword wparam, _dword lparam )
{
	ViewWindow* viewwindow = (ViewWindow*) ::GetWindowLong( (HWND) window, GWL_USERDATA );

	if ( viewwindow == _null )
		return (_long) ::DefWindowProc( (HWND) window, msgid, wparam, lparam );

	if ( msgid == WM_PAINT )
	{
		PAINTSTRUCT paintstruct;
		::BeginPaint( (HWND) window, &paintstruct );

		for ( _dword i = 0; i < viewwindow->mControlArray.Number( ); i ++ )
		{
			Control& control = viewwindow->mControlArray[i];

			// Found a control to call paint callback function.
			if ( control.mWindow == window )
			{
				Rect rect;
				::GetClientRect( (HWND) control.mWindow, (RECT*) &rect );

				if ( viewwindow->mFont != _null )
					::SelectObject( paintstruct.hdc, (HFONT) viewwindow->mFont );

				if ( control.mPaintFunc != _null )
					(*control.mPaintFunc)( window, control.mUserData, paintstruct.hdc, rect );

				break;
			}
		}

		::EndPaint( (HWND) window, &paintstruct );
	}
	else
	{
		if ( msgid == WM_TIMER ||
			 msgid == WM_MOUSEWHEEL ||
			 msgid == WM_MOUSEMOVE ||
			 msgid == WM_LBUTTONDOWN ||
			 msgid == WM_LBUTTONUP ||
			 msgid == WM_LBUTTONDBLCLK ||
			 msgid == WM_RBUTTONDOWN ||
			 msgid == WM_RBUTTONUP ||
			 msgid == WM_RBUTTONDBLCLK ||
			 msgid == WM_MBUTTONDOWN ||
			 msgid == WM_MBUTTONUP ||
			 msgid == WM_MBUTTONDBLCLK )
		{
			for ( _dword i = 0; i < viewwindow->mControlArray.Number( ); i ++ )
			{
				Control& control = viewwindow->mControlArray[i];

				// Found a control to call mouse message callback function.
				if ( control.mWindow == window )
				{
					if ( control.mMessageFunc != _null )
						(*control.mMessageFunc)( window, control.mUserData, msgid, wparam, lparam );

					break;
				}
			}
		}
	}

	return (_long) ::DefWindowProc( (HWND) window, msgid, wparam, lparam );
}

_bool ViewWindow::OnViewMessage( _dword msgid, _dword wparam, _dword lparam )
{
	// Get global window handle.
	_handle window = MainWindow::GetMainWindow( );

	// Get view window from the handle.
	ViewWindow* viewwindow = (ViewWindow*) ::GetWindowLong( (HWND) window, GWL_USERDATA );

	_dword tabheight	= viewwindow->mFontHeight + 6;
	_dword tabwidth		= viewwindow->mFontWidth * 2 + 20;
	_dword inputheight	= viewwindow->mFontHeight + 8;
	_dword stateheight	= viewwindow->mFontHeight + 2;

	switch ( msgid )
	{
		case WM_CREATE:
		{
			// Force load richedit control.
			::LoadLibrary( L"riched32.dll" );

			// Create input window.
			viewwindow->mInputWindow = ::CreateWindowEx( WS_EX_CLIENTEDGE, L"edit", L"", WS_VISIBLE | WS_CHILD,
				0, 0, 0, 0, (HWND) window, (HMENU) 101, _null, _null );

			// Create state window.
			viewwindow->mStateWindow = ::CreateWindowEx( 0, L"static", L"", WS_VISIBLE | WS_CHILD | SS_CENTER,
				0, 0, 0, 0, (HWND) window, (HMENU) 102, _null, _null );

			if ( viewwindow->mFontWidth == 0 || viewwindow->mFontHeight == 0 )
			{
				HDC hdc = ::GetDC( (HWND) window );

				TEXTMETRIC textmetric;
				::GetTextMetrics( hdc, &textmetric );

				// Get font size.
				viewwindow->mFontWidth	= textmetric.tmAveCharWidth;
				viewwindow->mFontHeight	= textmetric.tmHeight;

				::ReleaseDC( (HWND) window, hdc );

				tabheight	= viewwindow->mFontHeight + 6;
				tabwidth	= viewwindow->mFontWidth * 2 + 20;
				inputheight	= viewwindow->mFontHeight + 8;
				stateheight	= viewwindow->mFontHeight + 2;
			}

			HMENU sysmenu = ::GetSystemMenu( (HWND) window, _false );
			::AppendMenu( sysmenu, MF_STRING | MF_UNCHECKED, 200, L"Top Most" );

			break;
		}

		case WM_SIZE:
		{
			_long w = (_short) Dword( lparam ).LoWord( ), h = (_short) Dword( lparam ).HiWord( );

			// Move input window.
			::MoveWindow( (HWND) viewwindow->mInputWindow, 0, h - inputheight - stateheight, w, inputheight, _true );
			// Move state window.
			::MoveWindow( (HWND) viewwindow->mStateWindow, 0, h - stateheight, w, stateheight, _true );

			// Move all control windows.
			for ( _dword i = 0; i < viewwindow->mControlArray.Number( ); i ++ )
				::MoveWindow( (HWND) viewwindow->mControlArray[i].mWindow, 0, 0, w, h - tabheight - inputheight - stateheight, _true );

			break;
		}

		case WM_SETFOCUS:
		{
			::SetFocus( (HWND) viewwindow->mInputWindow );

			break;
		}

		case WM_PAINT:
		{
			Rect clientrect;
			::GetClientRect( (HWND) window, (RECT*) &clientrect );

			PAINTSTRUCT paintstruct;

			// Ready to paint view window.
			::BeginPaint( (HWND) window, &paintstruct );

			::SetBkMode( paintstruct.hdc, TRANSPARENT );

			HFONT oldfont = _null;

			if ( viewwindow->mFont != _null )
				oldfont = (HFONT) ::SelectObject( paintstruct.hdc, (HFONT) viewwindow->mFont );

			// Create brush to paint background.
			HBRUSH backgroundbrush = ::CreateSolidBrush( ::GetSysColor( COLOR_BTNFACE ) );

			HPEN	oldpen   = (HPEN)   ::SelectObject( paintstruct.hdc, ::GetStockObject( NULL_PEN ) );
			HBRUSH	oldbrush = (HBRUSH) ::SelectObject( paintstruct.hdc, backgroundbrush );

			// Paint background of input and state window.
			::Rectangle( paintstruct.hdc, 0, clientrect.b - tabheight - inputheight - stateheight,
				clientrect.r + 1, clientrect.b - inputheight - stateheight + 1 );

			if ( oldpen != _null )
				::SelectObject( paintstruct.hdc, oldpen );

			if ( oldbrush != _null )
				::SelectObject( paintstruct.hdc, oldbrush );

			// Delete brush.
			::DeleteObject( backgroundbrush );

			// Create brush to paint control title.
			backgroundbrush = ::CreateSolidBrush( ::GetSysColor( COLOR_BTNHILIGHT ) );

			oldpen   = (HPEN)   ::SelectObject( paintstruct.hdc, ::GetStockObject( BLACK_PEN ) );
			oldbrush = (HBRUSH) ::SelectObject( paintstruct.hdc, backgroundbrush );

			const Control* activecontrol = _null; Rect activerect;

			// Paint all control title.
			for ( _dword i = 0, l = 4, r = 4; i < viewwindow->mControlArray.Number( ); i ++, l = r )
			{
				const Control& control = viewwindow->mControlArray[i];

				r = control.mTitle.Length( ) * viewwindow->mFontWidth + l + tabwidth;

				Rect titlerect( l, clientrect.b - tabheight - inputheight - stateheight,
					r, clientrect.b - inputheight - stateheight - 3 );

				// Skip actived control title.
				if ( control.mControlID == viewwindow->mActiveControlID )
				{
					activecontrol	= &control;
					activerect		= titlerect;

					continue;
				}

				// Set point list to paint title polygon.
				Point tabpointlist[4] = 
				{
					Point( titlerect.l - 4, titlerect.t ),
					Point( titlerect.r + 4, titlerect.t ),
					Point( titlerect.r - 4, titlerect.b ),
					Point( titlerect.l + 4, titlerect.b ),
				};

				// Paint title polygon.
				::Polygon( paintstruct.hdc, (POINT*) tabpointlist, 4 );
				// Draw title.
				::DrawText( paintstruct.hdc, control.mTitle, -1, (RECT*) &titlerect, DT_CENTER | DT_VCENTER | DT_SINGLELINE );
			}

			if ( oldpen != _null )
				::SelectObject( paintstruct.hdc, oldpen );

			if ( oldbrush != _null )
				::SelectObject( paintstruct.hdc, oldbrush );

			// Delete brush.
			::DeleteObject( backgroundbrush );

			oldbrush = (HBRUSH) ::SelectObject( paintstruct.hdc, ::GetStockObject( WHITE_BRUSH ) );

			// Paint actived control title.
			if ( activecontrol != _null )
			{
				// Set point list to paint title polygon.
				Point tabpointlist[4] = 
				{
					Point( activerect.l - 4, activerect.t ),
					Point( activerect.r + 4, activerect.t ),
					Point( activerect.r - 4, activerect.b ),
					Point( activerect.l + 4, activerect.b ),
				};

				// Paint title polygon.
				::Polygon( paintstruct.hdc, (POINT*) tabpointlist, 4 );

				if ( oldpen != _null )
					::SelectObject( paintstruct.hdc, oldpen );

				oldpen = (HPEN) ::SelectObject( paintstruct.hdc, ::GetStockObject( WHITE_PEN ) );

				// Erase top line of the polygon.
				::MoveToEx( paintstruct.hdc, tabpointlist[0].x + 1, tabpointlist[0].y, _null );
				::LineTo( paintstruct.hdc, tabpointlist[1].x, tabpointlist[1].y );

				// Draw title.
				::DrawText( paintstruct.hdc, activecontrol->mTitle, -1, (RECT*) &activerect, DT_CENTER | DT_VCENTER | DT_SINGLELINE );
			}

			if ( oldfont != _null )
				::SelectObject( paintstruct.hdc, (HFONT) oldfont );

			if ( oldpen != _null )
				::SelectObject( paintstruct.hdc, oldpen );

			if ( oldbrush != _null )
				::SelectObject( paintstruct.hdc, oldbrush );

			::EndPaint( (HWND) window, &paintstruct );

			break;
		}

		case WM_LBUTTONDOWN:
		{
			_long x = (_short) Dword( lparam ).LoWord( ), y = (_short) Dword( lparam ).HiWord( );

			Rect clientrect;
			::GetClientRect( (HWND) window, (RECT*) &clientrect );

			for ( _dword i = 0, l = 4, r = 4; i < viewwindow->mControlArray.Number( ); i ++, l = r )
			{
				const Control& control = viewwindow->mControlArray[i];

				r = control.mTitle.Length( ) * viewwindow->mFontWidth + l + tabwidth;

				Rect titlerect( l, clientrect.b - tabheight - inputheight - stateheight,
					r, clientrect.b - inputheight - stateheight - 3 );

				// Active control window.
				if ( titlerect.PointInRect( Point( x, y ) ) )
				{
					viewwindow->ActiveControl( control.mControlID );
					break;
				}
			}

			break;
		}

		case WM_SYSCOMMAND:
		{
			if ( Dword( wparam ).LoWord( ) == 200 )
			{
				static _bool checked = _false; checked = !checked;

				HMENU sysmenu = ::GetSystemMenu( (HWND) window, _false );

				if ( checked )
				{
					::ModifyMenu( sysmenu, ::GetMenuItemCount( sysmenu ) - 1, MF_CHECKED | MF_BYPOSITION | MF_STRING, 200, L"Top Most" );
					::SetWindowPos( (HWND) window, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
				}
				else
				{
					::ModifyMenu( sysmenu, ::GetMenuItemCount( sysmenu ) - 1, MF_UNCHECKED | MF_BYPOSITION | MF_STRING, 200, L"Top Most" );
					::SetWindowPos( (HWND) window, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
				}
			}

			break;
		}

		case WM_USER + 20:
		{
			struct DelayCreateStruct
			{
				_bool	mUserControl;
				_dword	mControlID;
				_dword	mUserData;
				_void*	mPaintFunc;
				_void*	mMessageFunc;
				_char	mString[1];
			};

			DelayCreateStruct* delaycreate = (DelayCreateStruct*) lparam;

			if ( delaycreate->mUserControl )
				viewwindow->CreateUserControl( delaycreate->mString, delaycreate->mUserData, (OnControlPaint) delaycreate->mPaintFunc, (OnControlMessage) delaycreate->mMessageFunc, delaycreate->mControlID );
			else
				viewwindow->CreateEditControl( delaycreate->mString, delaycreate->mUserData, delaycreate->mControlID );

			delete delaycreate;

			break;
		}

		case WM_USER + 21:
		{
			_char* delayprint = (_char*) lparam;
			viewwindow->ChangeState( L"%s", delayprint );
			delete delayprint;

			break;
		}

		case WM_USER + 22:
		{
			struct DelayPrintStruct
			{
				_dword	mControlID;
				_dword	mColor;
				_char	mString[1];
			};

			DelayPrintStruct* delayprint = (DelayPrintStruct*) lparam;
			viewwindow->PrintString( delayprint->mControlID, delayprint->mColor, L"%s", delayprint->mString );

			delete delayprint;

			break;
		}

		case WM_USER + 23:
		{
			viewwindow->ClearString( wparam );

			break;
		}

		case WM_USER + 24:
		{
			if ( ::GetFocus( ) != viewwindow->mInputWindow )
				break;

			_char buffer[1024];

			::GetWindowText( (HWND) viewwindow->mInputWindow, buffer, 1024 );
			::SetWindowText( (HWND) viewwindow->mInputWindow, L"" );

			if ( buffer[0] != 0 && viewwindow->mCommandFunc != _null )
			{
				_char* commands[80];

				for ( _dword i = 0; i < 80; i ++ )
					commands[i] = L"";

				_dword number = StringFormatter::FormatCommandLine( buffer, commands, 80 );

				if ( number == 0 )
					break;

				if ( (*viewwindow->mCommandFunc)( commands, number ) == _false )
					::PostQuitMessage( 0 );
			}

			break;
		}

		case WM_USER + 25:
		{
			::CloseWindow( (HWND) wparam );
			::DestroyWindow( (HWND) wparam );

			break;
		}
	}

	return _true;
}

ViewWindow::Control* ViewWindow::GetControl( _dword controlid )
{
	if ( controlid == 0 )
		return _null;

	for ( _dword i = 0; i < mControlArray.Number( ); i ++ )
	{
		if ( mControlArray[i].mControlID == controlid )
			return &mControlArray[i];
	}

	return _null;
}

_void ViewWindow::CreateControl( Control& control )
{
	Rect clientrect;
	::GetClientRect( (HWND) mWindow, (RECT*) &clientrect );

	_dword tabheight	= mFontHeight + 6;
	_dword inputheight	= mFontHeight + 8;
	_dword stateheight	= mFontHeight + 2;

	::MoveWindow( (HWND) control.mWindow, 0, 0, clientrect.r, clientrect.b - tabheight - inputheight - stateheight, _true );

	// Add control to array.
	mControlArray.Append( control );

	// Active first control.
	if ( mActiveControlID == 0 )
		ActiveControl( control.mControlID );

	::InvalidateRect( (HWND) mWindow, _null, _true );
	::UpdateWindow( (HWND) mWindow );
}

_void ViewWindow::SetFont( StringPtr fontname, _dword size, _dword weight )
{
	if ( mFont != _null )
		::DeleteObject( mFont );

	HDC hdc = ::GetDC( (HWND) mWindow );

	// Create gdi font.
	mFont = ::CreateFont( - MulDiv( size, ::GetDeviceCaps( hdc, LOGPIXELSY ), 72 ),
		0, 0, 0, weight, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, fontname );

	// Set input and state font.
	::SendMessage( (HWND) mInputWindow, WM_SETFONT, (WPARAM) mFont, 0 );
	::SendMessage( (HWND) mStateWindow, WM_SETFONT, (WPARAM) mFont, 0 );

	// Set control font.
	for ( _dword i = 0; i < mControlArray.Number( ); i ++ )
		::SendMessage( (HWND) mControlArray[i].mWindow, WM_SETFONT, (WPARAM) mFont, 0 );

	::SelectObject( hdc, (HFONT) mFont );

	TEXTMETRIC textmetric;
	::GetTextMetrics( hdc, &textmetric );

	// Get font size.
	mFontWidth	= textmetric.tmAveCharWidth;
	mFontHeight	= textmetric.tmHeight;

	::ReleaseDC( (HWND) mWindow, hdc );
}

_dword ViewWindow::CreateEditControl( StringPtr title, _dword maxlinenumber, _dword controlid )
{
	if ( controlid == -1 )
		controlid = mNextControlID ++;

	// If current in window thread, just set create control directly.
	if ( ::GetWindowThreadProcessId( (HWND) mWindow, _null ) == ::GetCurrentThreadId( ) )
	{
		Control control;

		// Create richedit window.
		control.mWindow = ::CreateWindowEx( WS_EX_CLIENTEDGE, L"richedit", L"", WS_HSCROLL | WS_VSCROLL | WS_CHILD | ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_READONLY,
			0, 0, 0, 0, (HWND) mWindow, _null, _null, _null );

		// Set richedit background color to white.
		::SendMessage( (HWND) control.mWindow, EM_SETBKGNDCOLOR, 0, 0x00FFFFFF );

		if ( mFont != _null )
			::SendMessage( (HWND) control.mWindow, WM_SETFONT, (WPARAM) mFont, 0 );

		// Set control title.
		control.mTitle			= title;
		// Set control id.
		control.mControlID		= controlid;
		// Set control userdata.
		control.mUserData		= maxlinenumber;
		// Set callback function.
		control.mPaintFunc		= _null;
		control.mMessageFunc	= _null;

		CreateControl( control );
	}
	// Otherwise send an async message to create control.
	else
	{
		struct DelayCreateStruct
		{
			_bool	mUserControl;
			_dword	mControlID;
			_dword	mUserData;
			_void*	mPaintFunc;
			_void*	mMessageFunc;
			_char	mString[1];
		};

		DelayCreateStruct* delaycreate = (DelayCreateStruct*) ( new _byte[ sizeof( DelayCreateStruct ) + title.SizeOfBytes( ) ] );
		delaycreate->mUserControl	= _false;
		delaycreate->mControlID		= controlid;
		delaycreate->mUserData		= maxlinenumber;
		delaycreate->mPaintFunc		= _null;
		delaycreate->mMessageFunc	= _null;
		StringFormatter::CopyString( delaycreate->mString, title );

		::SendNotifyMessage( (HWND) mWindow, WM_USER + 20, 0, (DWORD) delaycreate );
	}

	return controlid;
}

_dword ViewWindow::CreateUserControl( StringPtr title, _dword userdata, OnControlPaint paintfunc, OnControlMessage messagefunc, _dword controlid )
{
	if ( controlid == -1 )
		controlid = mNextControlID ++;

	// If current in window thread, just set create control directly.
	if ( ::GetWindowThreadProcessId( (HWND) mWindow, _null ) == ::GetCurrentThreadId( ) )
	{
		Control control;

		WNDCLASSEX windowclass		= { sizeof( WNDCLASSEX ) };
		windowclass.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
		windowclass.lpfnWndProc		= (WNDPROC) ViewWndProc;
		windowclass.hInstance		= ::GetModuleHandle( _null );
		windowclass.hIcon			= ::LoadIcon( _null, IDI_WINLOGO );
		windowclass.hCursor			= ::LoadCursor( _null, IDC_ARROW );
		windowclass.lpszClassName	= L"FairyLand View Window";

		::RegisterClassEx( &windowclass );

		// Create user control window.
		control.mWindow = ::CreateWindowEx( 0, windowclass.lpszClassName, L"", WS_CHILD, 0, 0, 0, 0, (HWND) mWindow, _null, _null, _null );

		::SetWindowLong( (HWND) control.mWindow, GWL_USERDATA, (_long) this );

		// Set control title.
		control.mTitle			= title;
		// Set control id.
		control.mControlID		= controlid;
		// Set control userdata.
		control.mUserData		= userdata;
		// Set callback function.
		control.mPaintFunc		= paintfunc;
		control.mMessageFunc	= messagefunc;

		CreateControl( control );

		if ( messagefunc != _null )
			(*messagefunc)( control.mWindow, userdata, WM_CREATE, 0, 0 );
	}
	// Otherwise send an async message to create control.
	else
	{
		struct DelayCreateStruct
		{
			_bool	mUserControl;
			_dword	mControlID;
			_dword	mUserData;
			_void*	mPaintFunc;
			_void*	mMessageFunc;
			_char	mString[1];
		};

		DelayCreateStruct* delaycreate = (DelayCreateStruct*) ( new _byte[ sizeof( DelayCreateStruct ) + title.SizeOfBytes( ) ] );
		delaycreate->mUserControl	= _true;
		delaycreate->mControlID		= controlid;
		delaycreate->mUserData		= userdata;
		delaycreate->mPaintFunc		= paintfunc;
		delaycreate->mMessageFunc	= messagefunc;
		StringFormatter::CopyString( delaycreate->mString, title );

		::SendNotifyMessage( (HWND) mWindow, WM_USER + 20, 0, (DWORD) delaycreate );
	}

	return controlid;
}

_bool ViewWindow::ActiveControl( _dword controlid )
{
	Control* newactivecontrol = GetControl( controlid );

	if ( newactivecontrol == _null )
		return _false;

	Control* oldactivecontrol = GetControl( mActiveControlID );

	// Hide old control.
	if ( oldactivecontrol != _null )
		::ShowWindow( (HWND) oldactivecontrol->mWindow, SW_HIDE );

	mActiveControlID = controlid;

	// Show new control.
	::ShowWindow( (HWND) newactivecontrol->mWindow, SW_SHOW );

	::InvalidateRect( (HWND) mWindow, _null, _true );
	::UpdateWindow( (HWND) mWindow );

	return _true;
}

_bool ViewWindow::DeleteControl( _dword controlid )
{
	for ( _dword i = 0; i < mControlArray.Number( ); i ++ )
	{
		if ( mControlArray[i].mControlID != controlid )
			continue;

		::PostMessage( (HWND) mWindow, WM_USER + 25, (_dword) mControlArray[i].mWindow, 0 );

		// Remove it from control array.
		mControlArray.Remove( i );

		// Active the first control, control id must be 1.
		ActiveControl( 1 );

		return _true;
	}

	return _false;
}

_dword ViewWindow::GetActiveControlID( ) const
{
	return mActiveControlID;
}

_void ViewWindow::ChangeState( const _char* format, ... )
{
	_char buffer[2048];
	StringFormatter::FormatBufferV( buffer, 2048, format, (_chara*)( &format ) + sizeof( format ) );

	// If current in window thread, just set state window text directly.
	if ( ::GetWindowThreadProcessId( (HWND) mWindow, _null ) == ::GetCurrentThreadId( ) )
	{
		::SetWindowText( (HWND) mStateWindow, buffer );
	}
	// Otherwise send an async message to set state window text.
	else
	{
		_char* delaystate = new _char[2048];
		Memory::MemCpy( delaystate, buffer, sizeof( buffer ) );

		::SendNotifyMessage( (HWND) mWindow, WM_USER + 21, 0, (DWORD) delaystate );
	}
}

_void ViewWindow::PrintString( _dword controlid, _dword color, const _char* format, ... )
{
	// Convert from d3d color to gdi color.
	color = Dword( color ).InverseByteOrder( ) >> 8;

	_char buffer[2048];
	StringFormatter::FormatBufferV( buffer, 2048, format, (_chara*)( &format ) + sizeof( format ) );

	// If current in window thread, just set control window text directly.
	if ( ::GetWindowThreadProcessId( (HWND) mWindow, _null ) == ::GetCurrentThreadId( ) )
	{
		Control* control = GetControl( controlid );

		if ( control == _null )
			return;

		// So many line, just clear it first.
		if ( (_dword) ::SendMessage( (HWND) control->mWindow, EM_GETLINECOUNT, 0, 0 ) > control->mUserData )
			ClearString( controlid );

		::SendMessage( (HWND) control->mWindow, EM_SETSEL, -1, -1 );

		CHARFORMAT charformat = { sizeof( CHARFORMAT ), CFM_COLOR, 0, 0, 0, color };
		::SendMessage( (HWND) control->mWindow, EM_SETCHARFORMAT, 1, (_dword) &charformat );

		SETTEXTEX settextex = { 2, 1200 };
		::SendMessage( (HWND) control->mWindow, EM_SETTEXTEX, (_dword) &settextex, (_dword) buffer );
		::SendMessage( (HWND) control->mWindow, WM_VSCROLL, SB_BOTTOM, 0 );
	}
	// Otherwise send an async message to set control window text.
	else
	{
		struct DelayPrintStruct
		{
			_dword	mControlID;
			_dword	mColor;
			_char	mString[1];
		};

		DelayPrintStruct* delayprint = (DelayPrintStruct*) ( new _byte[ sizeof( DelayPrintStruct ) + sizeof( buffer ) ] );
		delayprint->mControlID	= controlid;
		delayprint->mColor		= color;
		Memory::MemCpy( delayprint->mString, buffer, sizeof( buffer ) );

		::SendNotifyMessage( (HWND) mWindow, WM_USER + 22, 0, (DWORD) delayprint );
	}
}

_void ViewWindow::ClearString( _dword controlid )
{
	// If current in window thread, just set control window text directly.
	if ( ::GetWindowThreadProcessId( (HWND) mWindow, _null ) == ::GetCurrentThreadId( ) )
	{
		Control* control = GetControl( controlid );

		if ( control == _null )
			return;

		SETTEXTEX settextex = { 2, sizeof( _char ) == 1 ? 0 : 1200 };
		::SendMessage( (HWND) control->mWindow, EM_SETSEL, 0, -1 );
		::SendMessage( (HWND) control->mWindow, EM_SETTEXTEX, (_dword) &settextex, (_dword) L"" );
	}
	// Otherwise send an async message to set control window text.
	else
	{
		::SendNotifyMessage( (HWND) mWindow, WM_USER + 23, controlid, 0 );
	}
}
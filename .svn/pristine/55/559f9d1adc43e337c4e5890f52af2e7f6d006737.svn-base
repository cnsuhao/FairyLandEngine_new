//============================================================================
// MonitorWindow.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// MonitorWindow Implementation
//----------------------------------------------------------------------------

template<> MonitorWindow* Singleton< MonitorWindow >::sSingleton = 0;

MonitorWindow::MonitorWindow( ) : mMonitorPool( 256 )
{
	mWindow			= _null;
	mInputWindow	= _null;
	mStateWindow	= _null;
	mSaveWindow		= _null;
	mClearWindow	= _null;
	mFont			= _null;

	mMainWindow		= _false;
	mShowing		= _false;
	mTopmost		= _false;

	mActiveColumn	= -1;
	mActiveSortMode	= 0;
	mActiveMonitor	= 0;

	mCommandFunc	= _null;
	mTimerHandle	= 0;

	// Force load richedit control.
	::LoadLibrary( L"riched32.dll" );
	// Force load list view control.
	::LoadLibrary( L"comctl32.dll" );

	mMonitorPool.Create( _null );

	WNDCLASSEX windowclass		= { sizeof( WNDCLASSEX ) };
	windowclass.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	windowclass.lpfnWndProc		= (WNDPROC) MonitorWndProc;
	windowclass.hInstance		= ::GetModuleHandle( _null );
	windowclass.hCursor			= ::LoadCursor( windowclass.hInstance, IDC_ARROW );
	windowclass.hbrBackground	= (HBRUSH) ::GetStockObject( WHITE_BRUSH );
	windowclass.lpszClassName	= L"PS Dash Monitor Window";

	::RegisterClassEx( &windowclass );

	_long x = ( ::GetSystemMetrics( SM_CXSCREEN ) - 600 ) / 2;
	_long y = ( ::GetSystemMetrics( SM_CYSCREEN ) - 400 ) / 2;

	// Create monitor window.
	mWindow = ::CreateWindowEx( 0, windowclass.lpszClassName, L"PS Dash Monitor",
		WS_OVERLAPPEDWINDOW, x, y, 600, 400, _null, _null, _null, _null );
	::SetWindowLong( (HWND) mWindow, GWL_USERDATA, (_long) this );

	// Create input window.
	mInputWindow = ::CreateWindowEx( WS_EX_CLIENTEDGE, L"EDIT", L"",
		WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_WANTRETURN, 0, 0, 0, 0, (HWND) mWindow, (HMENU) 256, _null, _null );

	// Create state window.
	mStateWindow = ::CreateWindowEx( 0, L"STATIC", L"",
		WS_VISIBLE | WS_CHILD | SS_CENTER, 0, 0, 0, 0, (HWND) mWindow, (HMENU) 257, _null, _null );

	// Create save window.
	mSaveWindow = ::CreateWindowEx( 0, L"BUTTON", L"Save",
		WS_VISIBLE | WS_CHILD | BS_FLAT, 0, 0, 0, 0, (HWND) mWindow, (HMENU) 258, _null, _null );

	// Create clear window.
	mClearWindow = ::CreateWindowEx( 0, L"BUTTON", L"Clear",
		WS_VISIBLE | WS_CHILD | BS_FLAT, 0, 0, 0, 0, (HWND) mWindow, (HMENU) 259, _null, _null );

	// Create font for all sub window, and render DC.
	mFont = ::CreateFont( 14, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, L"Arial" );

	// Set font to all sub window.
	::SendMessage( (HWND) mInputWindow, WM_SETFONT, (WPARAM) mFont, 0 );
	::SendMessage( (HWND) mStateWindow, WM_SETFONT, (WPARAM) mFont, 0 );
	::SendMessage( (HWND) mSaveWindow, WM_SETFONT, (WPARAM) mFont, 0 );
	::SendMessage( (HWND) mClearWindow, WM_SETFONT, (WPARAM) mFont, 0 );

	// Add top most menu.
	HMENU sysmenu = ::GetSystemMenu( (HWND) mWindow, _false );
	::AppendMenu( sysmenu, MF_STRING | MF_UNCHECKED, 200, L"Top Most" );
}

MonitorWindow::~MonitorWindow( )
{
	ClearAllMonitor( );

	::SetWindowLong( (HWND) mWindow, GWL_USERDATA, 0 );

	::KillTimer( (HWND) mWindow, mTimerHandle );
	::ShowWindow( (HWND) mWindow, SW_HIDE );
	::CloseWindow( (HWND) mWindow );
	::DestroyWindow( (HWND) mWindow );
	::DeleteObject( (HFONT) mFont );
}

_long MonitorWindow::MonitorWndProc( _handle window, _dword msgid, _dword wparam, _dword lparam )
{
	// Get monitor window from the handle.
	MonitorWindow* monitorwindow = (MonitorWindow*) ::GetWindowLong( (HWND) window, GWL_USERDATA );

	if ( monitorwindow == _null )
		return (_long) ::DefWindowProc( (HWND) window, msgid, wparam, lparam );

	// Close window.
	if ( msgid == WM_CLOSE )
	{
		if ( monitorwindow->mMainWindow )
		{
			if ( ::MessageBox( (HWND) window, L"Close Application", L"Alert", MB_YESNO ) == IDYES )
			{
				::CloseWindow( (HWND) window );
				::PostQuitMessage( 0 );
			}
		}
		else
		{
			::ShowWindow( (HWND) window, SW_HIDE );

			monitorwindow->mActiveColumn	= -1;
			monitorwindow->mActiveSortMode	= 0;
			monitorwindow->mShowing			= _false;
		}

		return 0;
	}

	// Resize window, and all sub windows.
	if ( msgid == WM_SIZE )
	{
		_long w = (_short) Dword( lparam ).LoWord( ), h = (_short) Dword( lparam ).HiWord( );

		// Move input window.
		::MoveWindow( (HWND) monitorwindow->mInputWindow, 0, h - 36, w, 18, _true );
		// Move state window.
		::MoveWindow( (HWND) monitorwindow->mStateWindow, 0, h - 18, w, 18, _true );
		// Move save window.
		::MoveWindow( (HWND) monitorwindow->mSaveWindow, w - 64, h - 54, 64, 18, _true );
		// Move clear window.
		::MoveWindow( (HWND) monitorwindow->mClearWindow, w - 128, h - 54, 64, 18, _true );

		// Move all sub windows.
		for ( MonitorPool::Iterator it = monitorwindow->mMonitorPool.GetHeadIterator( ); it.Valid( ); it ++ )
		{
			const Monitor* monitor = it;

			if ( monitor == _null )
				continue;

			::MoveWindow( (HWND) monitor->mMonitorWindow, 0, 0, w, h - 55, _true );
		}
	}
	// Set input window as focus.
	else if ( msgid == WM_SETFOCUS )
	{
		::SetFocus( (HWND) monitorwindow->mInputWindow );
	}
	// Capture some command.
	else if ( msgid == WM_NOTIFY )
	{
		Monitor* listmonitor = monitorwindow->GetMonitor( wparam, _MONITOR_LIST_RECORD );

		if ( listmonitor != _null )
		{
			NMLISTVIEW* listmessage = (NMLISTVIEW*) lparam;
			FL_ASSERT( listmessage != _null )

			if ( listmessage->hdr.code == LVN_COLUMNCLICK )
			{
				if ( System::IsKeyDown( VK_CONTROL ) )
					monitorwindow->SortListRecord( wparam, listmessage->iSubItem, _COLUMN_SORT_REVERSE | _COLUMN_SORT_SUBSORT );
				else
					monitorwindow->SortListRecord( wparam, listmessage->iSubItem, _COLUMN_SORT_REVERSE );
			}
		}
	}
	// Capture some command.
	else if ( msgid == WM_COMMAND )
	{
		// Input command.
		if ( HIWORD( wparam ) == EN_MAXTEXT && lparam == (_dword) monitorwindow->mInputWindow )
		{
			_char buffer[1024];

			::GetWindowText( (HWND) monitorwindow->mInputWindow, buffer, 1024 );
			::SetWindowText( (HWND) monitorwindow->mInputWindow, L"" );

			// Parse input command.
			if ( buffer[0] != 0 && monitorwindow->mCommandFunc != _null )
			{
				_char* commands[80];

				for ( _dword i = 0; i < 80; i ++ )
					commands[i] = L"";

				_dword number = StringFormatter::FormatCommandLine( buffer, commands, 80 );

				if ( number > 0 )
				{
					if ( (*monitorwindow->mCommandFunc)( commands, number ) == _false )
						::PostQuitMessage( 0 );
				}
			}

			::SetWindowText( (HWND) monitorwindow->mInputWindow, L"" );
		}
		// Save button.
		else if ( lparam == (_dword) monitorwindow->mSaveWindow )
		{
			if ( monitorwindow->mActiveMonitor != 0 )
			{
				Monitor* monitor = monitorwindow->mMonitorPool[ monitorwindow->mActiveMonitor ];
				FL_ASSERT( monitor != _null )

				String filename = StringFormatter::FormatString( L"%s_[%s]_%s.mot",
					(const _char*) StringFormatter::GetFileName( System::GetProcessFileName( ), _false ),
					(const _char*) monitor->mName, (const _char*) System::GetCurrentLocalTime( ).ToString( ) );

				monitorwindow->SaveMonitor( monitorwindow->mActiveMonitor, filename );
			}
		}
		// Clear button.
		else if ( lparam == (_dword) monitorwindow->mClearWindow )
		{
			if ( monitorwindow->mActiveMonitor != 0 )
			{
				monitorwindow->ClearMonitor( monitorwindow->mActiveMonitor );
			}
		}
	}
	// Ready to paint window.
	else if ( msgid == WM_PAINT )
	{
		PAINTSTRUCT paintstruct;
		::BeginPaint( (HWND) window, &paintstruct );

		Rect clientrect;
		::GetClientRect( (HWND) window, (RECT*) &clientrect );

		// Set render font.
		::SelectObject( paintstruct.hdc, (HFONT) monitorwindow->mFont );
		::SetBkMode( paintstruct.hdc, TRANSPARENT );

		// Create brush to render inactived tab background.
		HBRUSH newbrush = ::CreateSolidBrush( ::GetSysColor( COLOR_BTNSHADOW ) );
		HBRUSH oldbrush = (HBRUSH) ::SelectObject( paintstruct.hdc, newbrush );

		// The actived monitor saved, will be render at end.
		const Monitor* activemonitor = _null; Rect activerect;

		_long left = 4, width = 0;

		// Render all inactived monitors.
		for ( MonitorPool::Iterator it = monitorwindow->mMonitorPool.GetHeadIterator( ); it.Valid( ); it ++, left += width )
		{
			const Monitor* monitor = it;

			if ( monitor == _null )
				continue;

			// Calculate tab width.
			width = monitor->mTitle.Length( ) * 6;

			// The rect to render monitor title.
			Rect titlerect( left, clientrect.b - 55, left + width, clientrect.b - 40 );

			// Skip actived control title.
			if ( monitor->mMonitorID == monitorwindow->mActiveMonitor )
			{
				activemonitor	= monitor;
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
			::DrawText( paintstruct.hdc, monitor->mTitle, -1, (RECT*) &titlerect, DT_CENTER | DT_VCENTER | DT_SINGLELINE );
		}

		// Delete brush.
		::SelectObject( paintstruct.hdc, oldbrush );
		::DeleteObject( newbrush );

		// Render actived monitor tab.
		if ( activemonitor != _null )
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
			// Draw title.
			::DrawText( paintstruct.hdc, activemonitor->mTitle, -1, (RECT*) &activerect, DT_CENTER | DT_VCENTER | DT_SINGLELINE );
		}

		::EndPaint( (HWND) window, &paintstruct );
	}
	// Window timer.
	else if ( msgid == WM_TIMER )
	{
		Monitor* monitor = monitorwindow->GetMonitor( monitorwindow->GetActiveMonitor( ), 0 );

		if ( monitor != _null )
		{
			if ( monitor->mFreshFunc != _null )
				(*monitor->mFreshFunc)( monitorwindow->GetActiveMonitor( ) );
		}
	}
	// Click inside window.
	else if ( msgid == WM_LBUTTONDOWN )
	{
		_long x = (_short) Dword( lparam ).LoWord( ), y = (_short) Dword( lparam ).HiWord( );

		Rect clientrect;
		::GetClientRect( (HWND) window, (RECT*) &clientrect );

		_long left = 4, width = 0;

		// Render all inactived monitors.
		for ( MonitorPool::Iterator it = monitorwindow->mMonitorPool.GetHeadIterator( ); it.Valid( ); it ++, left += width )
		{
			const Monitor* monitor = it;

			if ( monitor == _null )
				continue;

			// Calculate tab width.
			width = monitor->mTitle.Length( ) * 6;

			// The rect to render monitor title.
			Rect titlerect( left, clientrect.b - 55, left + width, clientrect.b - 40 );

			// Active this monitor window.
			if ( titlerect.PointInRect( Point( x, y ) ) )
			{
				monitorwindow->ActiveMonitor( monitor->mMonitorID );

				break;
			}
		}
	}
	// System command.
	else if ( msgid == WM_SYSCOMMAND )
	{
		if ( Dword( wparam ).LoWord( ) == 200 )
		{
			monitorwindow->mTopmost = !monitorwindow->mTopmost;

			HMENU sysmenu = ::GetSystemMenu( (HWND) window, _false );

			if ( monitorwindow->mTopmost )
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
	}
	// Delay clear.
	else if ( msgid == _DELAY_MESSAGE_CLEAR )
	{
		monitorwindow->ClearMonitor( wparam );
	}
	// Delay state.
	else if ( msgid == _DELAY_MESSAGE_STATE )
	{
		_char* delaystring = (_char*) lparam;
		monitorwindow->SetState( L"%s", delaystring );
	
		delete[] delaystring;
	}
	// Delay print.
	else if ( msgid == _DELAY_MESSAGE_PRINT )
	{
		DelayPrintStruct* delayprint = (DelayPrintStruct*) lparam;
		monitorwindow->PrintString( delayprint->mMonitorID, delayprint->mColor, L"%s", delayprint->mString );

		delete[] (_byte*) delayprint;
	}
	// Delay fresh.
	else if ( msgid == _DELAY_MESSAGE_FRESH )
	{
		Monitor* monitor = monitorwindow->GetMonitor( wparam, _MONITOR_LIST_RECORD );

		if ( monitor != _null )
			monitorwindow->FreshList( (ListRecord&) *monitor, monitor->mMonitorWindow );
	}

	return (_long) ::DefWindowProc( (HWND) window, msgid, wparam, lparam );
}

_long MonitorWindow::DialogWndProc( _handle window, _dword msgid, _dword wparam, _dword lparam )
{
	// Get monitor window from the handle.
	MonitorWindow* monitorwindow = (MonitorWindow*) ::GetWindowLong( (HWND) window, GWL_USERDATA );

	if ( monitorwindow == _null )
		return (_long) ::DefWindowProc( (HWND) window, msgid, wparam, lparam );

	if ( msgid == WM_COMMAND )
	{
		Monitor* activemonitor = monitorwindow->mMonitorPool[ monitorwindow->mActiveMonitor ];

		// Process dialog command.
		if ( activemonitor != _null && activemonitor->mMonitorType == _MONITOR_DIALOG_WINDOW )
		{
			ControlArray& controlarray = ( (DialogWindow*) activemonitor )->mControlArray;

			for ( _dword i = 0; i < controlarray.Number( ); i ++ )
			{
				Control& control = controlarray[i];

				// Found the control send this command.
				if ( control.mControlWindow == (_handle) lparam )
				{
					if ( control.mNoticeFunc != _null )
						(*control.mNoticeFunc)( control.mControlName );

					break;
				}
			}
		}
	}

	return (_long) ::DefWindowProc( (HWND) window, msgid, wparam, lparam );
}

_long MonitorWindow::GraphWndProc( _handle window, _dword msgid, _dword wparam, _dword lparam )
{
	// Get monitor window from the handle.
	MonitorWindow* monitorwindow = (MonitorWindow*) ::GetWindowLong( (HWND) window, GWL_USERDATA );

	if ( monitorwindow == _null )
		return (_long) ::DefWindowProc( (HWND) window, msgid, wparam, lparam );

	// Ready to paint window.
	if ( msgid == WM_PAINT )
	{
		PAINTSTRUCT paintstruct;
		::BeginPaint( (HWND) window, &paintstruct );

		Rect clientrect;
		::GetClientRect( (HWND) window, (RECT*) &clientrect );

		// Set render font.
		::SelectObject( paintstruct.hdc, (HFONT) monitorwindow->mFont );
		::SetBkMode( paintstruct.hdc, TRANSPARENT );

		Monitor* activemonitor = monitorwindow->mMonitorPool[ monitorwindow->mActiveMonitor ];

		// Render graph record.
		if ( activemonitor != _null && activemonitor->mMonitorType == _MONITOR_GRAPH_RECORD )
		{
			const GraphRecord& graphrecord = (const GraphRecord&) *activemonitor;

			// Render this monitor value.
			monitorwindow->RenderValue( graphrecord, paintstruct.hdc, clientrect.r - 50, clientrect.b - 35 );

			// Render this monitor range.
			monitorwindow->RenderRange( graphrecord, paintstruct.hdc, clientrect.r, clientrect.b - 35 );
		}

		::EndPaint( (HWND) window, &paintstruct );
	}

	return (_long) ::DefWindowProc( (HWND) window, msgid, wparam, lparam );
}

_long MonitorWindow::ListCompareFunc( _dword param1, _dword param2, _dword paramsort )
{
	ListRecord* listrecord = (ListRecord*) paramsort;
	FL_ASSERT( listrecord != _null )

	_dword sortcolumn = listrecord->mSubSortColumn; _long compareresult = 0;

	// First sort with main column.
	if ( listrecord->mMainSortColumn != listrecord->mSubSortColumn )
	{
		listrecord->mSubSortColumn = listrecord->mMainSortColumn;
		compareresult = ListCompareFunc( param1, param2, paramsort );
		listrecord->mSubSortColumn = sortcolumn;

		if ( compareresult != 0 )
			return compareresult;
	}

	StringPtr item1 = listrecord->mRecordArray[ param1 ].mObject2;
	StringPtr item2 = listrecord->mRecordArray[ param2 ].mObject2;

	// Get the string of the column.
	for ( _dword i = 0; i < sortcolumn; i ++ )
	{
		_dword index1 = item1.SearchL2R( listrecord->mDelimiter );
		_dword index2 = item2.SearchL2R( listrecord->mDelimiter );

		if ( index1 == -1 || index2 == -1 )
			return 0;

		item1 = (const _char*) item1 + index1 + 1;
		item2 = (const _char*) item2 + index2 + 1;
	}

	_char buffer1[1024], buffer2[1024];
	StringFormatter::CopyString( buffer1, item1, 1023 );
	StringFormatter::CopyString( buffer2, item2, 1023 );

	// Clip value string.
	_dword index1 = item1.SearchL2R( listrecord->mDelimiter );
	if ( index1 < 1024 )
		buffer1[ index1 ] = 0;

	_dword index2 = item2.SearchL2R( listrecord->mDelimiter );
	if ( index2 < 1024 )
		buffer2[ index2 ] = 0;

	// Check value type of the column.
	switch ( listrecord->mTitleArray[ sortcolumn ].mObject3 )
	{
		case _COLUMN_TYPE_STRING:	compareresult = StringPtr( buffer1 ).CompareCaseInsensitive( buffer2 ); break;
		case _COLUMN_TYPE_LONG:		compareresult = (_long) ( Long( buffer1 ) - Long( buffer2 ) ); break;
		case _COLUMN_TYPE_FLOAT:	compareresult = (_long) ( Float( buffer1 ) * 1000.0f - Float( buffer2 ) * 1000.0f ); break;
	}

	// Sort ascending.
	if ( listrecord->mTitleArray[ sortcolumn ].mObject4 == _COLUMN_SORT_ASCENDING )
	{
		return compareresult;
	}
	// Sort descending.
	else
	{
		return compareresult * -1;
	}
}

MonitorWindow::Monitor* MonitorWindow::GetMonitor( _dword monitorid, _dword type ) const
{
	if ( monitorid == 0 || mMonitorPool.Index( monitorid ) == _null )
		return _null;

	Monitor* monitor = mMonitorPool[ monitorid ];
	FL_ASSERT( monitor != _null )

	if ( type != 0 )
	{
		if ( monitor->mMonitorType != type )
			return _null;
	}

	return monitor;
}

MonitorWindow::Control* MonitorWindow::GetControl( _dword monitorid, StringPtr name ) const
{
	if ( monitorid == 0 || mMonitorPool.Index( monitorid ) == _null )
		return _null;

	Monitor* monitor = mMonitorPool[ monitorid ];
	FL_ASSERT( monitor != _null )

	if ( monitor->mMonitorType != _MONITOR_DIALOG_WINDOW )
		return _null;

	DialogWindow* dialogwindow = (DialogWindow*) monitor;

	for ( _dword i = 0; i < dialogwindow->mControlArray.Number( ); i ++ )
	{
		const Control& control = dialogwindow->mControlArray[i];

		if ( control.mControlName == name )
			return (Control*) &control;
	}

	return _null;
}

_void MonitorWindow::RenderRange( const GraphRecord& monitor, _handle hdc, _dword width, _dword height )
{
	::MoveToEx( (HDC) hdc, 0, 15, _null );
	::LineTo( (HDC) hdc, width, 15 );

	::MoveToEx( (HDC) hdc, 0, height / 4 + 15, _null );
	::LineTo( (HDC) hdc, width, height / 4 + 15 );

	::MoveToEx( (HDC) hdc, 0, height / 2 + 15, _null );
	::LineTo( (HDC) hdc, width, height / 2 + 15 );

	::MoveToEx( (HDC) hdc, 0, height * 3 / 4 + 15, _null );
	::LineTo( (HDC) hdc, width, height * 3 / 4 + 15 );

	::MoveToEx( (HDC) hdc, 0, height + 15, _null );
	::LineTo( (HDC) hdc, width, height + 15 );

	_char temp[1024], buffer[1024];

	_dword position[]	= { 5, height / 4 + 5, height / 2 + 5, height * 3 / 4 + 5, height + 5 };
	_dword rangescale[]	= { 100, 75, 50, 25, 0 };

	for ( _dword x = 0; x < 5; x ++ )
	{
		// Format each item range string.
		for ( _dword i = 0; i < _MAX_GRAPH_ITEM_NUMBER; i ++ )
		{
			if ( i == 0 )
				buffer[0] = 0;

			if ( monitor.mItemName[i] == L"" )
				continue;

			_dword range = monitor.mValueRange[i] * rangescale[x] / 100;

			if ( buffer[0] == 0 )
				StringFormatter::FormatBuffer( temp, 1024, L" %d ", range );
			else
				StringFormatter::FormatBuffer( temp, 1024, L"| %d ", range );

			StringFormatter::AppendString( buffer, temp );
		}

		::TextOut( (HDC) hdc, 0, position[x], buffer, StringPtr( buffer ).Length( ) );
	}

	_dword y = 30;

	// Render each item information.
	for ( _dword i = 0; i < _MAX_GRAPH_ITEM_NUMBER; i ++ )
	{
		if ( monitor.mItemName[i] == L"" )
			continue;

		// Create pen for this monitor.
		HPEN newpen = ::CreatePen( PS_SOLID, 2, monitor.mPenColor[i] );
		HPEN oldpen = (HPEN) ::SelectObject( (HDC) hdc, newpen );

		// Render item line acoording to color.
		::MoveToEx( (HDC) hdc, width - 50, y, _null );
		::LineTo( (HDC) hdc, width - 5, y );

		// Delete pen.
		::SelectObject( (HDC) hdc, oldpen );
		::DeleteObject( newpen );

		Rect titlerect( width - 50 - 200, y - 20, width - 5 + 200, y );

		// Draw item name.
		::DrawText( (HDC) hdc, monitor.mItemName[i], -1, (RECT*) &titlerect, DT_CENTER | DT_BOTTOM | DT_SINGLELINE );

		y += 20;
	}
}

_void MonitorWindow::RenderValue( const GraphRecord& monitor, _handle hdc, _dword width, _dword height )
{
	_dword space = 1;

	// Calculate space size.
	for ( _dword i = 0; i < _MAX_GRAPH_ITEM_NUMBER; i ++ )
		space += monitor.mPenWidth[i];

	_dword recordperiod = monitor.mRecordPeriod;

	if ( recordperiod > width / space )
		recordperiod = width / space;

	// Render each sub item.
	for ( _dword i = 0, step = 0; i < _MAX_GRAPH_ITEM_NUMBER; i ++ )
	{
		if ( monitor.mPenWidth[i] == 0 || monitor.mValueList[i] == _null )
			continue;

		// Create pen for this monitor.
		HPEN newpen = ::CreatePen( PS_SOLID, monitor.mPenWidth[i], monitor.mPenColor[i] );
		HPEN oldpen = (HPEN) ::SelectObject( (HDC) hdc, newpen );

		// Render record value.
		for ( _dword j = 0; j < recordperiod; j ++ )
		{
			// Get record value.
			_dword value = monitor.mValueList[i][ recordperiod - j - 1 ];

			// Scale value to range.
			value = value * height / monitor.mValueRange[i];

			// Render as line.
			if ( monitor.mRenderStyle == _RENDER_STYLE_LINE )
			{
				if ( j == 0 )
					::MoveToEx( (HDC) hdc, j * space, height - value + 15, _null );
				else
					::LineTo( (HDC) hdc, j * space, height - value + 15 );
			}
			// Render as column.
			else
			{
				::MoveToEx( (HDC) hdc, j * space + step, height + 15, _null );
				::LineTo( (HDC) hdc, j * space + step, height - value + 15 );
			}
		}

		step += monitor.mPenWidth[i];

		// Delete pen.
		::SelectObject( (HDC) hdc, oldpen );
		::DeleteObject( newpen );
	}
}

_void MonitorWindow::FreshList( ListRecord& monitor, _handle list )
{
	if ( monitor.mFreshTickcount + 2000 > System::GetCurrentTickcount( ) )
		return;

	// Update last tickcount.
	monitor.mFreshTickcount = System::GetCurrentTickcount( );

	// Delete all items.
	::SendMessage( (HWND) list, LVM_DELETEALLITEMS, 0, 0 );

	// Buffer used to place item string.
	_char buffer[ 1024 ];

	LVITEM iteminfo = { 0 };
	iteminfo.mask		= LVIF_TEXT;
	iteminfo.cchTextMax = 1024;

	// Insert all record into list.
	for ( _dword i = 0; i < monitor.mRecordArray.Number( ); i ++ )
	{
		StringPtr record = monitor.mRecordArray[i].mObject2;

		// Insert all columns for this record.
		for ( _dword j = 0; j < monitor.mTitleArray.Number( ); j ++ )
		{
			// Parse item from record string.
			_dword index = record.SearchL2R( monitor.mDelimiter );

			if ( index == -1 )
			{
				// Use record to insert item.
				iteminfo.pszText = (_char*) (const _char*) record;
			}
			else
			{
				// Copy string into temp buffer.
				StringFormatter::CopyString( buffer, record, index );

				// Use buffer to insert item.
				iteminfo.pszText = buffer;
			}

			// Insert into list.
			iteminfo.mask		= j == 0 ? LVIF_TEXT | LVIF_PARAM : LVIF_TEXT;
			iteminfo.iItem		= i;
			iteminfo.iSubItem	= j;
			iteminfo.lParam		= i;

			::SendMessage( (HWND) list, j == 0 ? LVM_INSERTITEM : LVM_SETITEM, 0, (LPARAM) &iteminfo );

			// Parse over.
			if ( index == -1 )
				break;

			// Move record string backward.
			record = (const _char*) record + index + 1;
		}
	}

	if ( mActiveColumn != -1 )
		SortListRecord( monitor.mMonitorID, mActiveColumn, mActiveSortMode );

	::InvalidateRect( (HWND) mWindow, _null, _true );
	::UpdateWindow( (HWND) mWindow );
}

_dword MonitorWindow::CreateDialogWindow( StringPtr name )
{
	if ( mMonitorPool.Size( ) == mMonitorPool.Number( ) )
		return 0;

	DialogWindow* dialogwindow = new DialogWindow;

	dialogwindow->mName				= name;
	dialogwindow->mTitle			= name;
	dialogwindow->mMonitorWindow	= _null;
	dialogwindow->mMonitorID		= mMonitorPool.Create( dialogwindow );
	dialogwindow->mMonitorType		= _MONITOR_DIALOG_WINDOW;
	dialogwindow->mUserData			= 0;
	dialogwindow->mFreshFunc		= _null;

	WNDCLASSEX windowclass		= { sizeof( WNDCLASSEX ) };
	windowclass.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	windowclass.lpfnWndProc		= (WNDPROC) DialogWndProc;
	windowclass.hInstance		= ::GetModuleHandle( _null );
	windowclass.hCursor			= ::LoadCursor( windowclass.hInstance, IDC_ARROW );
	windowclass.hbrBackground	= (HBRUSH) ::GetStockObject( WHITE_BRUSH );
	windowclass.lpszClassName	= L"PS Dash Monitor Dialog Window";

	::RegisterClassEx( &windowclass );

	// Create monitor window.
	dialogwindow->mMonitorWindow = ::CreateWindowEx( WS_EX_CLIENTEDGE, windowclass.lpszClassName, L"",
		WS_CHILD, 0, 0, 0, 0, (HWND) mWindow, (HMENU) dialogwindow->mMonitorID, ::GetModuleHandle( _null ), _null );
	::SetWindowLong( (HWND) dialogwindow->mMonitorWindow, GWL_USERDATA, (_long) this );

	if ( mActiveMonitor == 0 )
		ActiveMonitor( dialogwindow->mMonitorID );

	return dialogwindow->mMonitorID;
}

_bool MonitorWindow::CreateDialogStatic( _dword monitorid, StringPtr name, StringPtr title, const Point& position, const Point& size, OnNotice funcpointer )
{
	Monitor* monitor = GetMonitor( monitorid, _MONITOR_DIALOG_WINDOW );
	if ( monitor == _null )
		return _false;

	_handle window = ::CreateWindowEx( 0, L"STATIC", title, WS_CHILD | WS_VISIBLE, position.x, position.y,
		size.x, size.y, (HWND) monitor->mMonitorWindow, 0, ::GetModuleHandle( _null ), _null );

	if ( window == _null )
		return _false;

	::SendMessage( (HWND) window, WM_SETFONT, (WPARAM) mFont, 0 );

	Control control;
	control.mControlWindow	= window;
	control.mControlName	= name;
	control.mNoticeFunc		= funcpointer;

	( (DialogWindow*) monitor )->mControlArray.Append( control );

	return _true;
}

_bool MonitorWindow::CreateDialogButton( _dword monitorid, StringPtr name, StringPtr title, const Point& position, const Point& size, OnNotice funcpointer )
{
	Monitor* monitor = GetMonitor( monitorid, _MONITOR_DIALOG_WINDOW );
	if ( monitor == _null )
		return _false;

	_handle window = ::CreateWindowEx( 0, L"BUTTON", title, WS_CHILD | WS_VISIBLE, position.x, position.y,
		size.x, size.y, (HWND) monitor->mMonitorWindow, 0, ::GetModuleHandle( _null ), _null );

	if ( window == _null )
		return _false;

	::SendMessage( (HWND) window, WM_SETFONT, (WPARAM) mFont, 0 );

	Control control;
	control.mControlWindow	= window;
	control.mControlName	= name;
	control.mNoticeFunc		= funcpointer;

	( (DialogWindow*) monitor )->mControlArray.Append( control );

	return _true;
}

_bool MonitorWindow::CreateDialogEdit( _dword monitorid, StringPtr name, StringPtr title, const Point& position, const Point& size, OnNotice funcpointer )
{
	Monitor* monitor = GetMonitor( monitorid, _MONITOR_DIALOG_WINDOW );
	if ( monitor == _null )
		return _false;

	_handle window = ::CreateWindowEx( WS_EX_CLIENTEDGE, L"EDIT", title, WS_CHILD | WS_VISIBLE, position.x, position.y,
		size.x, size.y, (HWND) monitor->mMonitorWindow, 0, ::GetModuleHandle( _null ), _null );

	if ( window == _null )
		return _false;

	::SendMessage( (HWND) window, WM_SETFONT, (WPARAM) mFont, 0 );

	Control control;
	control.mControlWindow	= window;
	control.mControlName	= name;
	control.mNoticeFunc		= funcpointer;

	( (DialogWindow*) monitor )->mControlArray.Append( control );

	return _true;
}

_bool MonitorWindow::EnableDialogControl( _dword monitorid, StringPtr name, _bool enable )
{
	Control* control = GetControl( monitorid, name );
	if ( control == _null )
		return _false;

	::EnableWindow( (HWND) control->mControlWindow, enable );

	return _true;
}

_bool MonitorWindow::ShowDialogControl( _dword monitorid, StringPtr name, _bool show )
{
	Control* control = GetControl( monitorid, name );
	if ( control == _null )
		return _false;

	::ShowWindow( (HWND) control->mControlWindow, show );

	return _true;
}

_bool MonitorWindow::SetDialogControlTitle( _dword monitorid, StringPtr name, StringPtr title )
{
	Control* control = GetControl( monitorid, name );
	if ( control == _null )
		return _false;

	::SetWindowText( (HWND) control->mControlWindow, title );

	return _true;
}

String MonitorWindow::GetDialogControlTitle( _dword monitorid, StringPtr name ) const
{
	Control* control = GetControl( monitorid, name );
	if ( control == _null )
		return L"";

	_char buffer[1024];
	::GetWindowText( (HWND) control->mControlWindow, buffer, 1024 );

	return String( buffer );
}

_dword MonitorWindow::CreateOutputWindow( StringPtr name )
{
	if ( mMonitorPool.Size( ) == mMonitorPool.Number( ) )
		return 0;

	OutputWindow* outputwindow = new OutputWindow;

	outputwindow->mName				= name;
	outputwindow->mTitle			= name;
	outputwindow->mMonitorWindow	= _null;
	outputwindow->mMonitorID		= mMonitorPool.Create( outputwindow );
	outputwindow->mMonitorType		= _MONITOR_OUTPUT_WINDOW;
	outputwindow->mUserData			= 0;
	outputwindow->mFreshFunc		= _null;

	outputwindow->mMaxLine			= _DEFAULT_OUTPUT_MAXLINE;
	outputwindow->mBackColor		= Color::cWhite;

	// Create edit window.
	outputwindow->mMonitorWindow = ::CreateWindowEx( WS_EX_CLIENTEDGE, L"richedit", L"",
		WS_HSCROLL | WS_VSCROLL | WS_CHILD | ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_READONLY,
		0, 0, 0, 0, (HWND) mWindow, (HMENU) outputwindow->mMonitorID, ::GetModuleHandle( _null ), _null );

	// Set font to edit window.
	::SendMessage( (HWND) outputwindow->mMonitorWindow, WM_SETFONT, (WPARAM) mFont, 0 );

	// Set richedit background color to white.
	::SendMessage( (HWND) outputwindow->mMonitorWindow, EM_SETBKGNDCOLOR, 0, 0x00FFFFFF );

	if ( mActiveMonitor == 0 )
		ActiveMonitor( outputwindow->mMonitorID );

	return outputwindow->mMonitorID;
}

_bool MonitorWindow::SetOutputWindowStyle( _dword monitorid, _dword maxline, _dword backcolor )
{
	Monitor* monitor = GetMonitor( monitorid, _MONITOR_OUTPUT_WINDOW );
	if ( monitor == _null )
		return _false;

	OutputWindow* outputwindow	= (OutputWindow*) monitor;
	outputwindow->mMaxLine		= maxline;
	outputwindow->mBackColor	= backcolor;

	// Convert from d3d color to gdi color.
	backcolor = Dword( backcolor ).InverseByteOrder( ) >> 8;

	// Set richedit background color.
	::SendMessage( (HWND) outputwindow->mMonitorWindow, EM_SETBKGNDCOLOR, 0, backcolor );

	return _true;
}

_bool MonitorWindow::PrintString( _dword monitorid, _dword color, const _char* format, ... )
{
	Monitor* monitor = GetMonitor( monitorid, _MONITOR_OUTPUT_WINDOW );
	if ( monitor == _null )
		return _false;

	OutputWindow* outputwindow = (OutputWindow*) monitor;

	// Convert from d3d color to gdi color.
	color = Dword( color ).InverseByteOrder( ) >> 8;

	_char buffer[2048];
	StringFormatter::FormatBufferV( buffer, 2048, format, (_chara*)( &format ) + sizeof( format ) );

	// If current in window thread, just set window text directly.
	if ( ::GetWindowThreadProcessId( (HWND) mWindow, _null ) == ::GetCurrentThreadId( ) )
	{
		// So many line, just clear it first.
		if ( (_dword) ::SendMessage( (HWND) outputwindow->mMonitorWindow, EM_GETLINECOUNT, 0, 0 ) > outputwindow->mMaxLine )
			ClearMonitor( monitorid );

		::SendMessage( (HWND) outputwindow->mMonitorWindow, EM_SETSEL, -1, -1 );

		CHARFORMAT charformat = { sizeof( CHARFORMAT ), CFM_COLOR, 0, 0, 0, color };
		::SendMessage( (HWND) outputwindow->mMonitorWindow, EM_SETCHARFORMAT, 1, (_dword) &charformat );

		SETTEXTEX settextex = { 2, 1200 };
		::SendMessage( (HWND) outputwindow->mMonitorWindow, EM_SETTEXTEX, (_dword) &settextex, (_dword) buffer );
		::SendMessage( (HWND) outputwindow->mMonitorWindow, WM_VSCROLL, SB_BOTTOM, 0 );
	}
	// Otherwise send an async message to set window text.
	else
	{
		DelayPrintStruct* delayprint = (DelayPrintStruct*) ( new _byte[ sizeof( DelayPrintStruct ) + sizeof( buffer ) ] );
		delayprint->mMonitorID	= monitorid;
		delayprint->mColor		= color;
		Memory::MemCpy( delayprint->mString, buffer, sizeof( buffer ) );

		::SendNotifyMessage( (HWND) mWindow, _DELAY_MESSAGE_PRINT, 0, (DWORD) delayprint );
	}

	return _true;
}

_dword MonitorWindow::CreateGraphRecord( StringPtr name )
{
	if ( mMonitorPool.Size( ) == mMonitorPool.Number( ) )
		return 0;

	GraphRecord* graphrecord = new GraphRecord;

	graphrecord->mName			= name;
	graphrecord->mTitle			= name;
	graphrecord->mMonitorWindow	= _null;
	graphrecord->mMonitorID		= mMonitorPool.Create( graphrecord );
	graphrecord->mMonitorType	= _MONITOR_GRAPH_RECORD;
	graphrecord->mUserData		= 0;
	graphrecord->mFreshFunc		= _null;

	graphrecord->mRenderStyle	= _RENDER_STYLE_COLUMN;

	for ( _dword i = 0; i < _MAX_GRAPH_ITEM_NUMBER; i ++ )
	{
		graphrecord->mPenWidth[i]	= 0;
		graphrecord->mPenColor[i]	= 0;
		graphrecord->mValueRange[i] = 1;
		graphrecord->mValueList[i]	= _null;
	}

	WNDCLASSEX windowclass		= { sizeof( WNDCLASSEX ) };
	windowclass.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	windowclass.lpfnWndProc		= (WNDPROC) GraphWndProc;
	windowclass.hInstance		= ::GetModuleHandle( _null );
	windowclass.hCursor			= ::LoadCursor( windowclass.hInstance, IDC_ARROW );
	windowclass.hbrBackground	= (HBRUSH) ::GetStockObject( WHITE_BRUSH );
	windowclass.lpszClassName	= L"PS Dash Monitor Graph Record Window";

	::RegisterClassEx( &windowclass );

	// Create monitor window.
	graphrecord->mMonitorWindow = ::CreateWindowEx( WS_EX_CLIENTEDGE, windowclass.lpszClassName, L"",
		WS_CHILD, 0, 0, 0, 0, (HWND) mWindow, (HMENU) graphrecord->mMonitorID, ::GetModuleHandle( _null ), _null );
	::SetWindowLong( (HWND) graphrecord->mMonitorWindow, GWL_USERDATA, (_long) this );

	if ( mActiveMonitor == 0 )
		ActiveMonitor( graphrecord->mMonitorID );

	return graphrecord->mMonitorID;
}

_bool MonitorWindow::SetGraphRecordStyle( _dword monitorid, _dword style )
{
	Monitor* monitor = GetMonitor( monitorid, _MONITOR_GRAPH_RECORD );
	if ( monitor == _null )
		return _false;

	GraphRecord* graphrecord	= (GraphRecord*) monitor;
	graphrecord->mRenderStyle	= style;

	return _true;
}

_bool MonitorWindow::SetGraphRecordPeriod( _dword monitorid, _dword period )
{
	FL_ASSERT( period > 1 )

	Monitor* monitor = GetMonitor( monitorid, _MONITOR_GRAPH_RECORD );
	if ( monitor == _null )
		return _false;

	GraphRecord* graphrecord	= (GraphRecord*) monitor;
	graphrecord->mRecordPeriod	= period;

	// Clear value list.
	for ( _dword i = 0; i < _MAX_GRAPH_ITEM_NUMBER; i ++ )
	{
		if ( graphrecord->mValueList[i] == _null )
			continue;

		Memory::HeapFree( graphrecord->mValueList[i] );
		graphrecord->mValueList[i] = _null;
	}

	return _true;
}

_bool MonitorWindow::SetGraphRecordItem( _dword monitorid, _dword subid, StringPtr name, _dword range, _dword width, _dword color )
{
	FL_ASSERT( subid < _MAX_GRAPH_ITEM_NUMBER )
	FL_ASSERT( range > 0 )

	Monitor* monitor = GetMonitor( monitorid, _MONITOR_GRAPH_RECORD );
	if ( monitor == _null )
		return _false;

	GraphRecord* graphrecord			= (GraphRecord*) monitor;
	graphrecord->mItemName[ subid ]		= name;
	graphrecord->mPenWidth[ subid ]		= width;
	graphrecord->mPenColor[ subid ]		= Dword( color ).InverseByteOrder( ) >> 8;;
	graphrecord->mValueRange[ subid ]	= range;

	return _true;
}

_bool MonitorWindow::AddGraphRecordSample( _dword monitorid, _dword subid, _dword value )
{
	FL_ASSERT( subid < _MAX_GRAPH_ITEM_NUMBER )

	Monitor* monitor = GetMonitor( monitorid, _MONITOR_GRAPH_RECORD );
	if ( monitor == _null )
		return _false;

	GraphRecord* graphrecord = (GraphRecord*) monitor;

	// Create value list buffer, and set to 0 as default.
	if ( graphrecord->mValueList[ subid ] == _null )
	{
		graphrecord->mValueList[ subid ] = (_dword*) Memory::HeapAlloc( sizeof( _dword ) * graphrecord->mRecordPeriod );
		Memory::MemSet( graphrecord->mValueList[ subid ], 0, sizeof( _dword ) * graphrecord->mRecordPeriod );
	}
	else
	{
		// Move old value backward.
		for ( _dword i = 0; i < graphrecord->mRecordPeriod - 1; i ++ )
			graphrecord->mValueList[ subid ][ graphrecord->mRecordPeriod - i - 1 ] = graphrecord->mValueList[ subid ][ graphrecord->mRecordPeriod - i - 2 ];
	}

	while ( value > graphrecord->mValueRange[ subid ] )
		graphrecord->mValueRange[ subid ] *= 2;

	while ( value < graphrecord->mValueRange[ subid ] / 4 && graphrecord->mValueRange[ subid ] > 256 )
		graphrecord->mValueRange[ subid ] /= 2;

	// Set value.
	graphrecord->mValueList[ subid ][ 0 ] = value;

	// Fresh graph window.
	if ( graphrecord->mFreshFunc == _null && mShowing == _true && monitorid == mActiveMonitor )
	{
		::InvalidateRect( (HWND) graphrecord->mMonitorWindow, _null, _true );
		::UpdateWindow( (HWND) graphrecord->mMonitorWindow );
	}

	return _true;
}

_dword MonitorWindow::CreateListRecord( StringPtr name )
{
	if ( mMonitorPool.Size( ) == mMonitorPool.Number( ) )
		return 0;

	ListRecord* listrecord = new ListRecord;

	listrecord->mName			= name;
	listrecord->mMonitorWindow	= _null;
	listrecord->mMonitorID		= mMonitorPool.Create( listrecord );
	listrecord->mMonitorType	= _MONITOR_LIST_RECORD;
	listrecord->mUserData		= 0;
	listrecord->mFreshFunc		= _null;

	listrecord->mDelimiter		= '$';
	listrecord->mMainSortColumn	= -1;
	listrecord->mSubSortColumn	= 0;
	listrecord->mFreshTickcount	= System::GetCurrentTickcount( );

	listrecord->mTitle.Format( L"%s (0)", (const _char*) name );

	// Create list window.
	listrecord->mMonitorWindow = ::CreateWindowEx( WS_EX_CLIENTEDGE, L"SysListView32", L"", WS_CHILD | LVS_REPORT,
		0, 0, 0, 0, (HWND) mWindow, (HMENU) listrecord->mMonitorID, ::GetModuleHandle( _null ), _null );

	// Set font to list window.
	::SendMessage( (HWND) listrecord->mMonitorWindow, WM_SETFONT, (WPARAM) mFont, 0 );

	if ( mActiveMonitor == 0 )
		ActiveMonitor( listrecord->mMonitorID );

	return listrecord->mMonitorID;
}

_bool MonitorWindow::SetListRecordDelimiter( _dword monitorid, _char delimiter )
{
	Monitor* monitor = GetMonitor( monitorid, _MONITOR_LIST_RECORD );
	if ( monitor == _null )
		return _false;

	ListRecord* listrecord	= (ListRecord*) monitor;
	listrecord->mDelimiter	= delimiter;

	return _true;
}

_bool MonitorWindow::SetListRecordColumn( _dword monitorid, StringPtr title, _dword width, _dword type )
{
	Monitor* monitor = GetMonitor( monitorid, _MONITOR_LIST_RECORD );
	if ( monitor == _null )
		return _false;

	ListRecord* listrecord = (ListRecord*) monitor;

	LVCOLUMN columninfo = { 0 };
	columninfo.mask		= LVCF_TEXT | LVCF_WIDTH;
	columninfo.pszText	= (_char*) (const _char*) title;
	columninfo.cx		= width;

	// Tell list window to creat a column.
	::SendMessage( (HWND) listrecord->mMonitorWindow, LVM_INSERTCOLUMN, listrecord->mTitleArray.Number( ), (WPARAM) &columninfo );

	// Save this title.
	listrecord->mTitleArray.Append( TitleItem( String( title ), width, type, _COLUMN_SORT_DESCENDING ) );

	return _true;
}

_bool MonitorWindow::AddListRecordSample( _dword monitorid, _dword key, StringPtr value )
{
	Monitor* monitor = GetMonitor( monitorid, _MONITOR_LIST_RECORD );
	if ( monitor == _null )
		return _false;

	ListRecord* listrecord = (ListRecord*) monitor;

	if ( listrecord->mFreshFunc == _null )
	{
		_dword index = -1;

		if ( key != 0 )
		{
			RecordArray::Iterator it = listrecord->mRecordArray.SearchAscending( key );

			if ( it.Valid( ) )
				index = it.mIndex;
		}

		// New sample, add it in.
		if ( index == -1 )
		{
			listrecord->mRecordArray.InsertAscending( RecordItem( key, String( value ) ) );

			// Update record number.
			listrecord->mTitle.Format( L"%s (%d)", (const _char*) listrecord->mName, listrecord->mRecordArray.Number( ) );
		}
		// Old sample, update value.
		else
		{
			listrecord->mRecordArray[ index ].mObject2 = value;
		}
	}
	else
	{
		listrecord->mRecordArray.Append( RecordItem( key, String( value ) ) );

		// Update record number.
		listrecord->mTitle.Format( L"%s (%d)", (const _char*) listrecord->mName, listrecord->mRecordArray.Number( ) );
	}

	// Fresh list window.
	if ( listrecord->mFreshFunc == _null && mShowing == _true && monitorid == mActiveMonitor )
	{
		// If current in window thread, just fresh list window directly.
		if ( ::GetWindowThreadProcessId( (HWND) mWindow, _null ) == ::GetCurrentThreadId( ) )
		{
			FreshList( *listrecord, listrecord->mMonitorWindow );
		}
		// Otherwise send an async message to fresh list window.
		else
		{
			::SendNotifyMessage( (HWND) mWindow, _DELAY_MESSAGE_FRESH, monitorid, 0 );
		}
	}

	return _true;
}

_bool MonitorWindow::DelListRecordSample( _dword monitorid, _dword key )
{
	Monitor* monitor = GetMonitor( monitorid, _MONITOR_LIST_RECORD );
	if ( monitor == _null )
		return _false;

	ListRecord* listrecord = (ListRecord*) monitor;

	// Search at record array.
	RecordArray::Iterator it = listrecord->mFreshFunc == _null ?
		listrecord->mRecordArray.SearchAscending( key ) : listrecord->mRecordArray.Search( key );

	if ( it.Valid( ) == _false )
		return _false;

	listrecord->mRecordArray.Remove( it.mIndex );

	// Update record number.
	listrecord->mTitle.Format( L"%s (%d)", (const _char*) listrecord->mName, listrecord->mRecordArray.Number( ) );

	// Fresh list window.
	if ( listrecord->mFreshFunc == _null && mShowing == _true && monitorid == mActiveMonitor )
	{
		// If current in window thread, just fresh list window directly.
		if ( ::GetWindowThreadProcessId( (HWND) mWindow, _null ) == ::GetCurrentThreadId( ) )
		{
			FreshList( *listrecord, listrecord->mMonitorWindow );
		}
		// Otherwise send an async message to fresh list window.
		else
		{
			::SendNotifyMessage( (HWND) mWindow, _DELAY_MESSAGE_FRESH, monitorid, 0 );
		}
	}

	return _true;
}

_bool MonitorWindow::SortListRecord( _dword monitorid, _dword column, _dword sortmode )
{
	Monitor* monitor = GetMonitor( monitorid, _MONITOR_LIST_RECORD );
	if ( monitor == _null )
		return _false;

	ListRecord* listrecord = (ListRecord*) monitor;

	if ( column >= listrecord->mTitleArray.Number( ) )
		return _false;

	// Set main column only if this is not sub sort.
	if ( ( sortmode & _COLUMN_SORT_SUBSORT ) == 0 )
		listrecord->mMainSortColumn	= column;

	// Set current column to be sort.
	listrecord->mSubSortColumn = column;

	sortmode &= ~_COLUMN_SORT_SUBSORT;

	// Set current sort mode as ascending.
	if ( sortmode == _COLUMN_SORT_ASCENDING )
	{
		listrecord->mTitleArray[ column ].mObject4 = _COLUMN_SORT_ASCENDING;
	}
	// Set current sort mode as descending.
	else if ( sortmode == _COLUMN_SORT_DESCENDING )
	{
		listrecord->mTitleArray[ column ].mObject4 = _COLUMN_SORT_DESCENDING;
	}
	// Set current sort mode as reversed with previous.
	else
	{
		listrecord->mTitleArray[ column ].mObject4 = !listrecord->mTitleArray[ column ].mObject4;
	}

	// Start sort.
	::SendMessage( (HWND) listrecord->mMonitorWindow, LVM_SORTITEMS, (_dword) listrecord, (_dword) ListCompareFunc );

	mActiveColumn	= column;
	mActiveSortMode	= sortmode;

	return _true;
}

_dword MonitorWindow::GetActiveMonitor( ) const
{
	return mActiveMonitor;
}

_dword MonitorWindow::SearchMonitor( StringPtr name ) const
{
	for ( MonitorPool::Iterator it = mMonitorPool.GetHeadIterator( ); it.Valid( ); it ++ )
	{
		Monitor* monitor = it;

		if ( monitor == _null )
			continue;

		if ( monitor->mName.CompareLowercase( name ) )
			return monitor->mMonitorID;
	}

	return 0;
}

_void MonitorWindow::SetMonitorUserData( _dword monitorid, _dword userdata )
{
	if ( monitorid == 0 || mMonitorPool.Index( monitorid ) == _null )
		return;

	Monitor* monitor = mMonitorPool[ monitorid ];
	FL_ASSERT( monitor != _null )

	monitor->mUserData = userdata;
}

_dword MonitorWindow::GetMonitorUserData( _dword monitorid ) const
{
	if ( monitorid == 0 || mMonitorPool.Index( monitorid ) == _null )
		return 0;

	Monitor* monitor = mMonitorPool[ monitorid ];
	FL_ASSERT( monitor != _null )

	return monitor->mUserData;
}

_bool MonitorWindow::RemoveMonitor( _dword monitorid )
{
	if ( monitorid == 0 || mMonitorPool.Index( monitorid ) == _null )
		return _false;

	Monitor* monitor = mMonitorPool[ monitorid ];
	FL_ASSERT( monitor != _null )

	// Delete output window.
	if ( monitor->mMonitorType == _MONITOR_OUTPUT_WINDOW )
	{
		delete (OutputWindow*) monitor;
	}
	// Delete graph record.
	else if ( monitor->mMonitorType == _MONITOR_GRAPH_RECORD )
	{
		delete (GraphRecord*) monitor;
	}
	// Delete list record.
	else if ( monitor->mMonitorType == _MONITOR_LIST_RECORD )
	{
		delete (ListRecord*) monitor;
	}
	else
	{
		FL_ASSERT( 0 )
	}

	mMonitorPool.Remove( monitorid );

	if ( mActiveMonitor == monitorid )
		mActiveMonitor = 0;

	return _true;
}

_bool MonitorWindow::ActiveMonitor( _dword monitorid )
{
	if ( monitorid == 0 || mMonitorPool.Index( monitorid ) == _null )
		return _false;

	mActiveColumn	= -1;
	mActiveSortMode	= 0;

	Monitor* oldmonitor = mMonitorPool[ mActiveMonitor ];
	Monitor* newmonitor = mMonitorPool[ monitorid ];

	// Hide old monitor.
	if ( oldmonitor != _null )
		::ShowWindow( (HWND) oldmonitor->mMonitorWindow, SW_HIDE );

	// Show new monitor.
	if ( newmonitor != _null )
	{
		// Fresh monitor.
		if ( newmonitor->mFreshFunc != _null )
		{
			(*newmonitor->mFreshFunc)( monitorid );
		}
		// Fresh list window.
		else if ( newmonitor->mMonitorType == _MONITOR_LIST_RECORD )
		{
			ListRecord* listrecord = (ListRecord*) newmonitor;

			FreshList( *listrecord, listrecord->mMonitorWindow );
		}

		::ShowWindow( (HWND) newmonitor->mMonitorWindow, SW_SHOW );
	}

	mActiveMonitor = monitorid;

	// Update window.
	::InvalidateRect( (HWND) mWindow, _null, _true );
	::UpdateWindow( (HWND) mWindow );

	return _true;
}

_bool MonitorWindow::ClearMonitor( _dword monitorid )
{
	if ( monitorid == 0 || mMonitorPool.Index( monitorid ) == _null )
		return _false;

	Monitor* monitor = mMonitorPool[ monitorid ];
	FL_ASSERT( monitor != _null )

	// If current in window thread, just clear window directly.
	if ( ::GetWindowThreadProcessId( (HWND) mWindow, _null ) == ::GetCurrentThreadId( ) )
	{
		if ( monitor->mMonitorType == _MONITOR_OUTPUT_WINDOW )
		{
			OutputWindow* outputwindow = (OutputWindow*) monitor;

			SETTEXTEX settextex = { 2, sizeof( _char ) == 1 ? 0 : 1200 };
			::SendMessage( (HWND) outputwindow->mMonitorWindow, EM_SETSEL, 0, -1 );
			::SendMessage( (HWND) outputwindow->mMonitorWindow, EM_SETTEXTEX, (_dword) &settextex, (_dword) L"" );
		}
		else if ( monitor->mMonitorType == _MONITOR_GRAPH_RECORD )
		{
			GraphRecord* graphrecord = (GraphRecord*) monitor;

			for ( _dword i = 0; i < _MAX_GRAPH_ITEM_NUMBER; i ++ )
			{
				Memory::HeapFree( graphrecord->mValueList[i] );
				graphrecord->mValueList[i] = _null;
			}

			::InvalidateRect( (HWND) graphrecord->mMonitorWindow, _null, _true );
			::UpdateWindow( (HWND) graphrecord->mMonitorWindow );
		}
		else if ( monitor->mMonitorType == _MONITOR_LIST_RECORD )
		{
			ListRecord* listrecord = (ListRecord*) monitor;

			listrecord->mRecordArray.Clear( );

			// Update record number.
			listrecord->mTitle.Format( L"%s (0)", (const _char*) listrecord->mName );

			FreshList( *listrecord, listrecord->mMonitorWindow );
		}
	}
	// Otherwise send an async message to clear window.
	else
	{
		::SendNotifyMessage( (HWND) mWindow, _DELAY_MESSAGE_CLEAR, monitorid, 0 );
	}

	return _true;
}

_bool MonitorWindow::SetFreshMonitorCallback( _dword monitorid, OnFreshMonitor funcpointer )
{
	Monitor* monitor = GetMonitor( monitorid, 0 );
	if ( monitor == _null )
		return _false;

	monitor->mFreshFunc = funcpointer;

	if ( mTimerHandle == 0 )
		mTimerHandle = (_dword) ::SetTimer( (HWND) mWindow, 0, _FRESH_MONITOR_INTERVAL, _null );

	return _true;
}

_bool MonitorWindow::FreshMonitor( _dword monitorid )
{
	Monitor* monitor = GetMonitor( monitorid, 0 );
	if ( monitor == _null )
		return _false;

	if ( monitor->mMonitorType == _MONITOR_GRAPH_RECORD )
	{
		GraphRecord* graphrecord = (GraphRecord*) monitor;

		::InvalidateRect( (HWND) mWindow, _null, _true );
		::UpdateWindow( (HWND) mWindow );
	}
	else if ( monitor->mMonitorType == _MONITOR_LIST_RECORD )
	{
		ListRecord* listrecord = (ListRecord*) monitor;

		// Reset fresh tickcount.
		listrecord->mFreshTickcount = 0;

		// If current in window thread, just fresh list window directly.
		if ( ::GetWindowThreadProcessId( (HWND) mWindow, _null ) == ::GetCurrentThreadId( ) )
		{
			FreshList( *listrecord, listrecord->mMonitorWindow );
		}
		// Otherwise send an async message to fresh list window.
		else
		{
			::SendNotifyMessage( (HWND) mWindow, _DELAY_MESSAGE_FRESH, monitorid, 0 );
		}
	}

	return _true;
}

_void MonitorWindow::ClearAllMonitor( )
{
	for ( MonitorPool::Iterator it = mMonitorPool.GetHeadIterator( ); it.Valid( ); it ++ )
	{
		Monitor* monitor = it;

		if ( monitor == _null )
			continue;

		::DestroyWindow( (HWND) monitor->mMonitorWindow );

		if ( monitor->mMonitorType == _MONITOR_DIALOG_WINDOW )
		{
			delete (DialogWindow*) monitor;
		}
		else if ( monitor->mMonitorType == _MONITOR_OUTPUT_WINDOW )
		{
			delete (OutputWindow*) monitor;
		}
		else if ( monitor->mMonitorType == _MONITOR_GRAPH_RECORD )
		{
			delete (GraphRecord*) monitor;
		}
		else if ( monitor->mMonitorType == _MONITOR_LIST_RECORD )
		{
			delete (ListRecord*) monitor;
		}
		else
		{
			FL_ASSERT( 0 )
		}
	}

	mMonitorPool.Clear( _true );
}

_bool MonitorWindow::SaveMonitor( _dword monitorid, StringPtr filename ) const
{
	if ( monitorid == 0 || mMonitorPool.Index( monitorid ) == _null )
		return _false;

	Monitor* monitor = mMonitorPool[ monitorid ];
	FL_ASSERT( monitor != _null )

	Variable record;
	record.Redeclare( L"", Variable::_TYPE_STRUCT );
	record.DefineMember( L"Version", Variable::_TYPE_DWORD );
	record.DefineMember( L"Name", Variable::_TYPE_STRING );
	record.DefineMember( L"Type", Variable::_TYPE_DWORD );

	record[ L"Version" ]	= 2;
	record[ L"Name" ]		= monitor->mName;
	record[ L"Type" ]		= monitor->mMonitorType;

	// Save as graph record.
	if ( monitor->mMonitorType == _MONITOR_GRAPH_RECORD )
	{
		GraphRecord* graphrecord = (GraphRecord*) monitor;

		record.DefineMember( L"RenderStyle", Variable::_TYPE_DWORD );
		record.DefineMember( L"RecordPeriod", Variable::_TYPE_DWORD );

		record[ L"RenderStyle" ]	= graphrecord->mRenderStyle;
		record[ L"RecordPeriod" ]	= graphrecord->mRecordPeriod;

		_char buffer[1024];

		// Save graph value.
		for ( _dword i = 0; i < _MAX_GRAPH_ITEM_NUMBER; i ++ )
		{
			if ( graphrecord->mItemName[i] == L"" )
				continue;

			StringFormatter::FormatBuffer( buffer, 1024, L"ItemName%d", i );
			record.DefineMember( buffer, Variable::_TYPE_STRING );
			record[ buffer ] = graphrecord->mItemName[i];

			StringFormatter::FormatBuffer( buffer, 1024, L"PenWidth%d", i );
			record.DefineMember( buffer, Variable::_TYPE_DWORD );
			record[ buffer ] = graphrecord->mPenWidth[i];

			StringFormatter::FormatBuffer( buffer, 1024, L"PenColor%d", i );
			record.DefineMember( buffer, Variable::_TYPE_DWORD );
			record[ buffer ] = graphrecord->mPenColor[i];

			StringFormatter::FormatBuffer( buffer, 1024, L"RecordRange%d", i );
			record.DefineMember( buffer, Variable::_TYPE_DWORD );
			record[ buffer ] = graphrecord->mValueRange[i];

			StringFormatter::FormatBuffer( buffer, 1024, L"RecordValue%d", i );
			record.DefineMember( buffer, Variable::_TYPE_DWORD | Variable::_TYPE_ARRAY );
			record.CreateElement( buffer, graphrecord->mRecordPeriod );

			if ( graphrecord->mValueList[i] != _null )
			{
				for ( _dword j = 0; j < graphrecord->mRecordPeriod; j ++ )
				{
					StringFormatter::FormatBuffer( buffer, 1024, L"RecordValue%d[%d]", i, j );
					record[ buffer ] = graphrecord->mValueList[i][j];
				}
			}
		}

		return VariableSaver::SaveVariable( filename, record, _true );
	}
	// Save as list record.
	else if ( monitor->mMonitorType == _MONITOR_LIST_RECORD )
	{
		ListRecord* listrecord = (ListRecord*) monitor;

		record.DefineMember( L"Delimiter", Variable::_TYPE_CHAR );
		record.DefineMember( L"TitleArray", Variable::_TYPE_STRUCT | Variable::_TYPE_ARRAY );
		record.DefineMember( L"TitleArray.Name", Variable::_TYPE_STRING );
		record.DefineMember( L"TitleArray.Width", Variable::_TYPE_DWORD );
		record.DefineMember( L"TitleArray.Type", Variable::_TYPE_DWORD );
		record.CreateElement( L"TitleArray", listrecord->mTitleArray.Number( ) );
		record.DefineMember( L"RecordArray", Variable::_TYPE_STRUCT | Variable::_TYPE_ARRAY );
		record.DefineMember( L"RecordArray.Key", Variable::_TYPE_DWORD );
		record.DefineMember( L"RecordArray.Value", Variable::_TYPE_STRING );
		record.CreateElement( L"RecordArray", listrecord->mRecordArray.Number( ) );

		record[ L"Delimiter" ] = listrecord->mDelimiter;

		for ( _dword i = 0; i < listrecord->mTitleArray.Number( ); i ++ )
		{
			record( L"TitleArray[%d].Name", i )		= listrecord->mTitleArray[i].mObject1;
			record( L"TitleArray[%d].Width", i )	= listrecord->mTitleArray[i].mObject2;
			record( L"TitleArray[%d].Type", i )		= listrecord->mTitleArray[i].mObject3;
		}

		for ( _dword i = 0; i < listrecord->mRecordArray.Number( ); i ++ )
		{
			record( L"RecordArray[%d].Key", i )		= listrecord->mRecordArray[i].mObject1;
			record( L"RecordArray[%d].Value", i )	= listrecord->mRecordArray[i].mObject2;
		}

		return VariableSaver::SaveVariable( filename, record, _true );
	}
	else
	{
		return _false;
	}
}

_bool MonitorWindow::LoadMonitor( StringPtr filename )
{
	Variable record;
	if ( VariableLoader::LoadVariable( filename, record ) == _false )
		return _false;

	_dword version	= record[ L"Version" ];
	_dword type		= record[ L"Type" ];

	// Version not match.
	if ( version != 2 )
		return _false;

	_dword monitorid = 0;

	if ( type == _MONITOR_OUTPUT_WINDOW	)
	{
		monitorid = CreateOutputWindow( record[ L"Name" ] );
	}
	else if ( type == _MONITOR_GRAPH_RECORD	)
	{
		monitorid = CreateGraphRecord( record[ L"Name" ] );
	}
	else if ( type == _MONITOR_LIST_RECORD )
	{
		monitorid = CreateListRecord( record[ L"Name" ] );
	}
	else
	{
		return _false;
	}

	Monitor* monitor = mMonitorPool[ monitorid ];
	FL_ASSERT( monitor != _null )

	ActiveMonitor( monitorid );

	// Load as graph record.
	if ( type == _MONITOR_GRAPH_RECORD )
	{
		GraphRecord* graphrecord = (GraphRecord*) monitor;

		graphrecord->mRenderStyle	= record[ L"RenderStyle" ];
		graphrecord->mRecordPeriod	= record[ L"RecordPeriod" ];

		// Load graph value.
		for ( _dword i = 0; i < _MAX_GRAPH_ITEM_NUMBER; i ++ )
		{
			graphrecord->mItemName[i] = record( L"ItemName%d", i );

			if ( graphrecord->mItemName[i] == L"" )
				continue;

			graphrecord->mPenWidth[i]	= record( L"PenWidth%d", i );
			graphrecord->mPenColor[i]	= record( L"PenColor%d", i );
			graphrecord->mPenColor[i]	= record( L"PenColor%d", i );
			graphrecord->mValueRange[i]	= record( L"RecordRange%d", i );

			Memory::HeapFree( graphrecord->mValueList[i] );
			graphrecord->mValueList[i] = _null;

			if ( graphrecord->mRecordPeriod != 0 )
			{
				graphrecord->mValueList[i] = (_dword*) Memory::HeapAlloc( sizeof( _dword ) * graphrecord->mRecordPeriod );
				Memory::MemSet( graphrecord->mValueList[i], 0, sizeof( _dword ) * graphrecord->mRecordPeriod );
			}

			_dword valuenumber = record( L"RecordValue%d#", i );

			FL_ASSERT( valuenumber <= graphrecord->mRecordPeriod )

			// Load values.
			for ( _dword j = 0; j < valuenumber; j ++ )
				graphrecord->mValueList[i][j] = record( L"RecordValue%d[%d]", i, j );
		}

		return _true;
	}
	// Load as list record.
	else if ( type == _MONITOR_LIST_RECORD )
	{
		ListRecord* listrecord = (ListRecord*) monitor;
		listrecord->mDelimiter = record[ L"Delimiter" ];

		_dword titlenumber = record[ L"TitleArray#" ];

		// Load titles.
		for ( _dword i = 0; i < titlenumber; i ++ )
		{
			String name	 = record( L"TitleArray[%d].Name", i );
			_dword width = record( L"TitleArray[%d].Width", i );
			_dword type  = record( L"TitleArray[%d].Type", i );

			listrecord->mTitleArray.Append( TitleItem( name, width, type, _COLUMN_SORT_DESCENDING ) );

			LVCOLUMN columninfo = { 0 };
			columninfo.mask		= LVCF_TEXT | LVCF_WIDTH;
			columninfo.pszText	= (_char*) (const _char*) name;
			columninfo.cx		= width;

			// Tell list window to creat a column.
			::SendMessage( (HWND) listrecord->mMonitorWindow, LVM_INSERTCOLUMN, i, (WPARAM) &columninfo );
		}

		_dword valuenumber = record[ L"RecordArray#" ];

		// Load values.
		for ( _dword i = 0; i < valuenumber; i ++ )
		{
			_dword key	 = record( L"RecordArray[%d].Key", i );
			String value = record( L"RecordArray[%d].Value", i );

			listrecord->mRecordArray.Append( RecordItem( key, String( value ) ) );
		}

		listrecord->mTitle.Format( L"%s (%d)", (const _char*) listrecord->mName, valuenumber );

		return _true;
	}
	else
	{
		return _false;
	}
}

_void MonitorWindow::SetCommandCallback( OnCommand funcpointer )
{
	mCommandFunc = funcpointer;
}

_void MonitorWindow::SetState( const _char* format, ... )
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

		::SendNotifyMessage( (HWND) mWindow, _DELAY_MESSAGE_STATE, 0, (DWORD) delaystate );
	}
}

_void MonitorWindow::SetTitle( StringPtr title )
{
	::SetWindowText( (HWND) mWindow, title );
}

_bool MonitorWindow::IsWindowShowing( ) const
{
	return mShowing;
}

_void MonitorWindow::RunWindow( )
{
	mMainWindow = _true;

	System::MessageLoop( );
}

_void MonitorWindow::ShowWindow( )
{
	if ( mShowing == _true )
		return;

	::ShowWindow( (HWND) mWindow, SW_SHOW );
	::UpdateWindow( (HWND) mWindow );

	mShowing = _true;

	// No monitor actived.
	if ( mActiveMonitor == 0 || mMonitorPool.Index( mActiveMonitor ) == _null )
		return;

	Monitor* monitor = mMonitorPool[ mActiveMonitor ];
	FL_ASSERT( monitor != _null )	

	// Fresh list window.		
	if ( monitor->mMonitorType == _MONITOR_LIST_RECORD )
	{
		ListRecord* listrecord = (ListRecord*) monitor;

		FreshList( *listrecord, listrecord->mMonitorWindow );
	}
}

_void MonitorWindow::HideWindow( )
{
	if ( mShowing == _false )
		return;

	::ShowWindow( (HWND) mWindow, SW_HIDE );

	mActiveColumn	= -1;
	mActiveSortMode	= 0;
	mShowing		= _false;
}
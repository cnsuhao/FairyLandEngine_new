//============================================================================
// System.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// System Helper
//----------------------------------------------------------------------------

_dword GetLanguageID( _dword langid )
{
	_dword mainid = langid & 0x00FF;

	switch ( mainid )
	{
		// Chinese.
		case 0x04:
		{
			return langid == 0x0804 ? Language::LANGUAGE_ZH_CN : Language::LANGUAGE_ZH_TW;
		}

		// German
		case 0x07:
		{
			return Language::LANGUAGE_DE_DE;
		}

		// French.
		case 0x0C:
		{
			return Language::LANGUAGE_FR_FR;
		}

		// Spanish.
		case 0x0A:
		{
			return Language::LANGUAGE_ES_ES;
		}

		// Turkish
		case 0x1F:
		{
			return Language::LANGUAGE_TR_TR;
		}
	}

	return Language::LANGUAGE_EN_US;
}

//----------------------------------------------------------------------------
// System Implementation
//----------------------------------------------------------------------------

_dword System::GetCPUNumber( )
{
	SYSTEM_INFO systeminfo;
	::GetSystemInfo( &systeminfo );

	return systeminfo.dwNumberOfProcessors;
}

_dword System::GetCPUID( )
{
	SYSTEM_INFO systeminfo;
	::GetSystemInfo( &systeminfo );

	if ( systeminfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_UNKNOWN )
		return _CPU_UNKNOWN;

	if ( systeminfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_ALPHA )
		return _CPU_ALPHA;

	if ( systeminfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_ALPHA64 )
		return _CPU_ALPHA64;

	if ( systeminfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 )
		return _CPU_AMD64;

	if ( systeminfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64 )
		return _CPU_IA64;

	if ( systeminfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL )
	{
		if ( systeminfo.wProcessorLevel == 3 )
			return _CPU_INTEL386;

		if ( systeminfo.wProcessorLevel == 4 )
			return _CPU_INTEL486;

		if ( systeminfo.wProcessorLevel == 5 )
			return _CPU_INTELP1;

		if ( systeminfo.wProcessorLevel == 6 )
			return _CPU_INTELP2;

		if ( systeminfo.wProcessorLevel == 7 )
			return _CPU_INTELP2;

		if ( systeminfo.wProcessorLevel == 10 )
			return _CPU_INTELP3;

		if ( systeminfo.wProcessorLevel == 15 )
			return _CPU_INTELP4;
	}

	return _CPU_UNKNOWN;
}

String System::GetCPUIDString( )
{
	_dword cpuid = GetCPUID( );

	if ( cpuid == _CPU_UNKNOWN )
		return String( L"unknown" );

	if ( cpuid == _CPU_INTEL386 )
		return String( L"Intel 386" );

	if ( cpuid == _CPU_INTEL486 )
		return String( L"Intel 486" );

	if ( cpuid == _CPU_INTELP1 )
		return String( L"Intel Pentium" );

	if ( cpuid == _CPU_INTELP2 )
		return String( L"Intel Pentium II" );

	if ( cpuid == _CPU_INTELP3 )
		return String( L"Intel Pentium III" );

	if ( cpuid == _CPU_INTELP4 )
		return String( L"Intel Pentium IV" );

	if ( cpuid == _CPU_ALPHA )
		return String( L"Alpha" );

	if ( cpuid == _CPU_ALPHA64 )
		return String( L"Alpha64" );

	if ( cpuid == _CPU_AMD64 )
		return String( L"AMD64" );

	if ( cpuid == _CPU_IA64 )
		return String( L"IA64" );

	return String( L"unknown" );
}

_dword System::GetTotalMemory( )
{
	MEMORYSTATUS memorystatus;
	::GlobalMemoryStatus( &memorystatus );

	return (_dword) memorystatus.dwTotalPhys;
}

String System::GetTotalMemoryString( )
{
	return StringFormatter::FormatBytesString( GetTotalMemory( ) );
}

_dword System::GetAvailableMemory( )
{
	MEMORYSTATUS memorystatus;
	::GlobalMemoryStatus( &memorystatus );

	return (_dword) memorystatus.dwAvailPhys;
}

String System::GetAvailableMemoryString( )
{
	return StringFormatter::FormatBytesString( GetAvailableMemory( ) );
}

_qword System::GetTotalDiskSpace( StringPtr directory )
{
	_qword total = 0, available = 0;

	if ( FileSystem::GetDiskSpace( directory, total, available ) == _false )
		return 0;

	return total;
}

String System::GetTotalDiskSpaceString( StringPtr directory )
{
	return StringFormatter::FormatBytesString( GetTotalDiskSpace( directory ) );
}

_qword System::GetAvailableDiskSpace( StringPtr directory )
{
	_qword total = 0, available = 0;

	if ( FileSystem::GetDiskSpace( directory, total, available ) == _false )
		return 0;

	return available;
}

String System::GetAvailableDiskSpaceString( StringPtr directory )
{
	return StringFormatter::FormatBytesString( GetAvailableDiskSpace( directory ) );
}

_dword System::GetWindowVersion( )
{
	OSVERSIONINFO osversion = { sizeof( OSVERSIONINFO ) };

	if ( ::GetVersionEx( &osversion ) == 0 )
		return _WINDOW_UNKNOWN;

	if ( osversion.dwMajorVersion == 6 )
		return _WINDOW_VISTA;

	if ( osversion.dwMajorVersion != 5 )
		return _WINDOW_UNKNOWN;

	if ( osversion.dwMinorVersion == 0 )
		return _WINDOW_2000;

	if ( osversion.dwMinorVersion == 1 )
		return _WINDOW_XP;

	if ( osversion.dwMinorVersion == 2 )
		return _WINDOW_2003;

	return 0;
}

String System::GetWindowVersionString( )
{
	_dword version = GetWindowVersion( );

	if ( version == _WINDOW_2000 )
		return String( L"Windows 2000" );

	if ( version == _WINDOW_XP )
		return String( L"Windows xp" );

	if ( version == _WINDOW_2003 )
		return String( L"Windows 2003" );

	if ( version == _WINDOW_VISTA )
		return String( L"Windows vista" );

	return String( L"unknown" );
}

_dword System::GetDirectXVersion( )
{
	Registry registry;
	registry.SetAccessRight( Registry::_ACCESS_READ );

	if ( registry.Open( Registry::_KEY_LOCAL_MACHINE, L"Software\\Microsoft\\DirectX" ) == _false )
		return _DIRECTX_UNKNOWN;

	_char version[1024];

	_dword type = Registry::_TYPE_STRING, length = sizeof( version );
	if ( registry.ReadValue( L"Version", type, version, length ) == _false )
		return _DIRECTX_UNKNOWN;

	if ( StringPtr( version ).CompareLowercase( L"4.08.01.0881" ) == _true )
		return _DIRECTX_81;

	if ( StringPtr( version ).CompareLowercase( L"4.09.00.0900" ) == _true )
		return _DIRECTX_90;

	if ( StringPtr( version ).CompareLowercase( L"4.09.00.0901" ) == _true )
		return _DIRECTX_90a;

	if ( StringPtr( version ).CompareLowercase( L"4.09.00.0902" ) == _true )
		return _DIRECTX_90b;

	if ( StringPtr( version ).CompareLowercase( L"4.09.00.0904" ) == _true )
		return _DIRECTX_90c;

	if ( StringPtr( version ).SearchL2R( L"4.10." ) == 0 )
		return _DIRECTX_100;

	return 0;
}

String System::GetDirectXVersionString( )
{
	_dword version = GetDirectXVersion( );

	if ( version == _DIRECTX_81 )
		return String( L"DirectX 8.1" );

	if ( version == _DIRECTX_90 )
		return String( L"DirectX 9.0" );

	if ( version == _DIRECTX_90a )
		return String( L"DirectX 9.0a" );

	if ( version == _DIRECTX_90b )
		return String( L"DirectX 9.0b" );

	if ( version == _DIRECTX_90c )
		return String( L"DirectX 9.0c" );

	if ( version == _DIRECTX_100 )
		return String( L"DirectX 10.0" );

	return String( L"unknown" );
}

_dword System::GetLocalLanguage( )
{
	_dword langid = ::GetUserDefaultLangID( );

	switch ( langid & 0x000003FF )
	{
		case LANG_ENGLISH:	return _LANG_ENG;
		case LANG_CHINESE:	return langid == 0x0804 ? _LANG_CHS : _LANG_CHT;
	}

	return _LANG_UNKNOWN;
}

String System::GetLocalLanguageString( )
{
	_dword language = GetLocalLanguage( );

	switch ( language )
	{
		case _LANG_ENG:	return L"English";
		case _LANG_CHS:	return L"Chinese Simplified";
		case _LANG_CHT:	return L"Chinese Traditional";
	}

	return String( L"" );
}

String System::GetLocalMACAddress( )
{
	String mac;

	// Allocate data structure for NetBIOS.
	WKSTA_TRANSPORT_INFO_0* pwkti = _null;

	_dword entriesread = 0, totalentries = 0;

	// Get MAC address via NetBIOS's enumerate function
	::NetWkstaTransportEnum( _null, 0, (_byte**) &pwkti, -1, &entriesread, &totalentries, _null );

	if ( entriesread > 1 )
		mac = pwkti[1].wkti0_transport_address;

	// Free buffer.
	::NetApiBufferFree( pwkti );

	return mac;
}

String System::GetNetworkCardName( )
{
	Library iplibrary;

	if ( iplibrary.Load( L"iphlpapi.dll" ) == _false )
		return String( L"Unknown" );

	typedef _dword (_stdcall *GetAdapter)( IP_ADAPTER_INFO*, _dword* );

	GetAdapter getadapter = (GetAdapter) iplibrary.GetFunction( L"GetAdaptersInfo" );
	if ( getadapter == _null )
		return String( L"Unknown" );

	IP_ADAPTER_INFO adapter = { 0 }; _dword size = sizeof( IP_ADAPTER_INFO );

	if ( (*getadapter)( &adapter, &size ) != 0 )
		return String( L"Unknown" );

	_char buffer[2048];
	return StringFormatter::AnsiToUnicode( buffer, 2048, adapter.Description );
}

String System::GetSoundCardName( )
{
	Library mmlibrary;

	if ( mmlibrary.Load( L"winmm.dll" ) == _false )
		return String( L"Unknown" );

	typedef _dword (_stdcall *GetDevCaps)( _dword, WAVEOUTCAPSW*, _dword );

	GetDevCaps getdevcaps = (GetDevCaps) mmlibrary.GetFunction( L"waveOutGetDevCapsW" );
	if ( getdevcaps == _null )
		return String( L"Unknown" );

	WAVEOUTCAPSW wavecaps = { 0 };

	if ( (*getdevcaps)( 0, &wavecaps, sizeof( WAVEOUTCAPSW ) ) != 0 )
		return String( L"Unknown" );

	return String( wavecaps.szPname );
}

String System::GetVideoCardName( )
{
	Library d3dlibrary;

	if ( d3dlibrary.Load( L"d3d9.dll" ) == _false )
		return String( L"Unknown" );

	typedef IDirect3D9* (_stdcall *D3DCreateDevice)( _dword );

	D3DCreateDevice createdevice = (D3DCreateDevice) d3dlibrary.GetFunction( L"Direct3DCreate9" );
	if ( createdevice == _null )
		return String( L"Unknown" );

	IDirect3D9* d3d = (*createdevice)( D3D_SDK_VERSION );
	if ( d3d == _null )
		return String( L"Unknown" );

	_dword adapternumber = d3d->GetAdapterCount( );

	for ( _dword i = 0; i < adapternumber; i ++ )
	{
		D3DADAPTER_IDENTIFIER9 adapter;
		if ( d3d->GetAdapterIdentifier( i, 0, &adapter ) != 0 )
			continue;

		d3d->Release( );

		_char buffer[2048];
		return StringFormatter::AnsiToUnicode( buffer, 2048, adapter.Description );
	}

	d3d->Release( );

	return String( L"Unknown" );
}

_dword System::GetVideoMemory( )
{
	Library ddrawlibrary;

	if ( ddrawlibrary.Load( L"ddraw.dll" ) == _false )
		return 0;

	typedef _long (_stdcall *DirectDrawCreate)( const GUID*, IDirectDraw**, _void* );

	DirectDrawCreate createdevice = (DirectDrawCreate) ddrawlibrary.GetFunction( L"DirectDrawCreate" );
	if ( createdevice == _null )
		return 0;

	IDirectDraw* ddraw = _null;

	(*createdevice)( _null, &ddraw, _null );
	if ( ddraw == _null )
		return 0;

	DDCAPS ddcaps = { sizeof( DDCAPS ) };

	ddraw->GetCaps( &ddcaps, _null );
	ddraw->Release( );

	return ddcaps.dwVidMemTotal;
}

String System::GetVideoMemoryString( )
{
	return StringFormatter::FormatBytesString( GetVideoMemory( ) );
}

String System::GetLocalComputerName( )
{
	_char buffer[1024]; _dword length = 1024;

	if ( ::GetComputerName( buffer, &length ) == 0 )
		return String( L"" );

	return String( buffer );
}

String System::GetCurrentUserName( )
{
	_char buffer[1024]; _dword length = 1024;

	if ( ::GetUserName( buffer, &length ) == 0 )
		return String( L"" );

	return String( buffer );
}

String System::GetProcessFileName( )
{
	_char buffer[1024];

	if ( ::GetModuleFileName( _null, buffer, 1024 ) == 0 )
		return String( L"" );

	return String( buffer );
}

String System::GetProcessPathName( )
{
	_char buffer[1024];

	if ( ::GetModuleFileName( _null, buffer, 1024 ) == 0 )
		return String( L"" );

	return StringFormatter::GetPathName( buffer );
}

String System::GetDllFileName( _handle dllmodule )
{
	_char buffer[1024];

	if ( ::GetModuleFileName( (HMODULE) dllmodule, buffer, 1024 ) == 0 )
		return String( L"" );

	return String( buffer );
}

String System::GetDllPathName( _handle dllmodule )
{
	_char buffer[1024];

	if ( ::GetModuleFileName( (HMODULE) dllmodule, buffer, 1024 ) == 0 )
		return String( L"" );

	return StringFormatter::GetPathName( buffer );
}

_dword System::GetSystemLanguage( )
{
	return GetLanguageID( ::GetSystemDefaultLangID( ) );
}

_dword System::GetUserLanguage( )
{
	return GetLanguageID( ::GetUserDefaultLangID( ) );
}

_dword System::GetKeyboardLanguage( )
{
	return GetLanguageID( (_dword) ::GetKeyboardLayout( 0 ) & 0x0000FFFF );
}

_bool System::SetKeyboardLanguage( _dword langid )
{
	StringPtr langname = L"00000409";

	switch ( langid )
	{
		case Language::LANGUAGE_ZH_CN:	langname = L"00000804"; break;
		case Language::LANGUAGE_ZH_TW:	langname = L"00000404"; break;
		case Language::LANGUAGE_DE_DE:	langname = L"00000407"; break;
		case Language::LANGUAGE_FR_FR:	langname = L"0000040C"; break;
		case Language::LANGUAGE_ES_ES:	langname = L"0000040A"; break;
		case Language::LANGUAGE_TR_TR:	langname = L"0000041F"; break;
	}

	HKL keyboard = ::LoadKeyboardLayout( langname, KLF_ACTIVATE );
	if ( keyboard == _null )
		return _false;

	::ActivateKeyboardLayout( keyboard, 0 );

	return _true;
}

_dword System::GetCurrentThreadID( )
{
	return ::GetCurrentThreadId( );
}

_dword System::GetCurrentProcessID( )
{
	return ::GetCurrentProcessId( );
}

_dword System::GetCurrentCPUUsage( )
{
	return 0;
}

_dword System::GetCurrentMemoryUsage( )
{
	PROCESS_MEMORY_COUNTERS memorycounter = { sizeof( PROCESS_MEMORY_COUNTERS ) };
	::GetProcessMemoryInfo( ::GetCurrentProcess( ), &memorycounter, sizeof( PROCESS_MEMORY_COUNTERS ) );

	return (_dword) memorycounter.PagefileUsage;
}

_dword System::GetCurrentTickcount( )
{
	if ( ( (_byte*) ::timeGetTime )[0] == 0xE9 ) // Hooked, jmp xxx.
	{
		static _dword sOSVersion = 0;

		if ( sOSVersion == 0 )
		{
			OSVERSIONINFO version = { sizeof( OSVERSIONINFO ) };
			::GetVersionEx( &version );

			sOSVersion = version.dwMajorVersion;
		}

		_dword tick = 0;

		// XP.
		if ( sOSVersion <= 5 )
		{
			__asm
			{
				mov		edx, 7FFE0000h
				mov		eax, dword ptr [edx]
				mul		dword ptr [edx+4]
				shrd	eax, edx, 18h
				mov		tick, eax
			}
		}
		// Vista.
		else if ( sOSVersion == 6 )
		{
			__asm
			{
				mov		ecx, dword ptr ds:[7FFE0324h]
				mov		edx, dword ptr ds:[7FFE0320h]
				mov		eax, dword ptr ds:[7FFE0004h]
				mul		edx
				shl		ecx, 8
				imul	ecx, dword ptr ds:[7FFE0004h]
				shrd	eax, edx, 18h
				shr		edx, 18h
				add		eax, ecx
				mov		tick, eax
			}
		}

		return tick;
	}
	else
	{
		return (_dword) ::timeGetTime( );
	}
}

_dword System::GetElapsedSecondSinceUTC( )
{
	_large timer;
	return (_dword) ::time( &timer );
}

_qword System::GetCurrentCycleCount( )
{
	_qword cyclecount = 0;

	::QueryPerformanceCounter( (LARGE_INTEGER*) &cyclecount );

	return cyclecount;
}

_qword System::GetSystemCycleFrequency( )
{
	_qword cyclefrequency = 0;

	::QueryPerformanceFrequency( (LARGE_INTEGER*) &cyclefrequency );

	return cyclefrequency;
}

_float System::GetElapseTime( _qword cyclecount1, _qword cyclecount2 )
{
	_qword cyclefrequency = 0;
	::QueryPerformanceFrequency( (LARGE_INTEGER*) &cyclefrequency );

	if ( cyclefrequency == 0 )
		return 0.0f;

	return (_float) ( cyclecount2 - cyclecount1 ) / (_float) cyclefrequency * 1000.0f;
}

Time System::GetCurrentLocalTime( )
{
	Time currenttime;
	::GetLocalTime( (SYSTEMTIME*) &currenttime );

	return currenttime;
}

String System::GetCurrentLocalTimeString( )
{
	Time currenttime;
	::GetLocalTime( (SYSTEMTIME*) &currenttime );

	return StringFormatter::FormatString( L"%.4d/%.2d/%.2d %.2d:%.2d:%.2d", currenttime.mYear,
		currenttime.mMonth, currenttime.mDay, currenttime.mHour, currenttime.mMinute, currenttime.mSecond );
}

Time System::GetCurrentSystemTime( )
{
	Time currenttime;
	::GetSystemTime( (SYSTEMTIME*) &currenttime );

	return currenttime;
}

String System::GetCurrentSystemTimeString( )
{
	Time currenttime;
	::GetSystemTime( (SYSTEMTIME*) &currenttime );

	return StringFormatter::FormatString( L"%.4d/%.2d/%.2d %.2d:%.2d:%.2d", currenttime.mYear,
		currenttime.mMonth, currenttime.mDay, currenttime.mHour, currenttime.mMinute, currenttime.mSecond );
}

_handle System::LoadSystemCursor( StringPtr cursor )
{
	_handle cursorhandle = _null;

	if ( cursor.CompareLowercase( L"arrow" ) )
		cursorhandle = ::LoadCursor( _null, IDC_ARROW );
	else if ( cursor.CompareLowercase( L"wait" ) )
		cursorhandle = ::LoadCursor( _null, IDC_WAIT );
	else if ( cursor.CompareLowercase( L"cross" ) )
		cursorhandle = ::LoadCursor( _null, IDC_CROSS );
	else if ( cursor.CompareLowercase( L"hand" ) )
		cursorhandle = ::LoadCursor( _null, IDC_HAND );
	else if ( cursor.CompareLowercase( L"help" ) )
		cursorhandle = ::LoadCursor( _null, IDC_HELP );
	else if ( cursor.CompareLowercase( L"input" ) )
		cursorhandle = ::LoadCursor( _null, IDC_IBEAM );

	return cursorhandle;
}

_bool System::IsKeyDown( _dword keycode )
{
	return ( ::GetKeyState( keycode ) & 0x80 ) != 0;
}

Point System::GetMousePosition( _handle window )
{
	Point mouse = Point::cZeroPoint;
	::GetCursorPos( (POINT*) &mouse );

	if ( window != _null )
		::ScreenToClient( (HWND) window, (POINT*) &mouse );

	return mouse;
}

_bool System::SelectColorDialog( _handle window, _dword& color )
{
	COLORREF clref[16] = { 0 };

	CHOOSECOLOR cc	= { sizeof( CHOOSECOLOR ) };
	cc.lpCustColors	= clref;
	cc.hwndOwner	= (HWND) window;
	cc.rgbResult	= Dword( color << 8 ).InverseByteOrder( );
	cc.Flags		= CC_FULLOPEN | CC_RGBINIT;

	if ( ::ChooseColor( &cc ) == 0 )
		return _false;

	color = ( Dword( cc.rgbResult ).InverseByteOrder( ) >> 8 ) | 0xFF000000;

	return _true;
}

_bool System::SetEnvironmentPrefix( StringPtr prefix )
{
	return ::SetEnvironmentVariable( L"FairyLandPrefix", prefix ) != 0;
}

_bool System::GetEnvironmentPrefix( String& prefix )
{
	_char buffer[1024];
	if ( ::GetEnvironmentVariable( L"FairyLandPrefix", buffer, 1024 ) == 0 )
		return _false;

	prefix = buffer;

	return _true;
}

_bool System::SetEnvironment( StringPtr name, StringPtr value )
{
	_char prefix[1024];
	if ( ::GetEnvironmentVariable( L"FairyLandPrefix", prefix, 1024 ) == 0 )
	{
		return ::SetEnvironmentVariable( name, value ) != 0;
	}
	else
	{
		_char fullname[1024];
		StringFormatter::FormatBuffer( fullname, 1024, L"%s_%s", (const _char*) prefix, (const _char*) name );

		return ::SetEnvironmentVariable( fullname, value ) != 0;
	}
}

_bool System::SetEnvironment( StringPtr name, _dword value )
{
	_char buffer[1024];
	return SetEnvironment( name, Dword( value ).ToString( buffer, 1024 ) );
}

_bool System::SetEnvironment( StringPtr name, _void* value )
{
	_char buffer[1024];
	return SetEnvironment( name, Dword( (_dword) value ).ToString( buffer, 1024 ) );
}

_bool System::GetEnvironment( StringPtr name, String& value )
{
	_char prefix[1024], buffer[1024];
	if ( ::GetEnvironmentVariable( L"FairyLandPrefix", prefix, 1024 ) == 0 )
	{
		if ( ::GetEnvironmentVariable( name, buffer, 1024 ) == 0 )
			return _false;
	}
	else
	{
		_char fullname[1024];
		StringFormatter::FormatBuffer( fullname, 1024, L"%s_%s", (const _char*) prefix, (const _char*) name );

		if ( ::GetEnvironmentVariable( fullname, buffer, 1024 ) == 0 )
			return _false;
	}

	value = buffer;

	return _true;
}

_bool System::GetEnvironment( StringPtr name, _dword& value )
{
	_char prefix[1024], buffer[1024];
	if ( ::GetEnvironmentVariable( L"FairyLandPrefix", prefix, 1024 ) == 0 )
	{
		if ( ::GetEnvironmentVariable( name, buffer, 1024 ) == 0 )
			return _false;
	}
	else
	{
		_char fullname[1024];
		StringFormatter::FormatBuffer( fullname, 1024, L"%s_%s", (const _char*) prefix, (const _char*) name );

		if ( ::GetEnvironmentVariable( fullname, buffer, 1024 ) == 0 )
			return _false;
	}

	value = Dword( buffer );

	return _true;
}

_bool System::GetEnvironment( StringPtr name, _void*& value )
{
	_char prefix[1024], buffer[1024];
	if ( ::GetEnvironmentVariable( L"FairyLandPrefix", prefix, 1024 ) == 0 )
	{
		if ( ::GetEnvironmentVariable( name, buffer, 1024 ) == 0 )
			return _false;
	}
	else
	{
		_char fullname[1024];
		StringFormatter::FormatBuffer( fullname, 1024, L"%s_%s", (const _char*) prefix, (const _char*) name );

		if ( ::GetEnvironmentVariable( fullname, buffer, 1024 ) == 0 )
			return _false;
	}

	value = (_void*) (_dword) Dword( buffer );

	return _true;
}

_void System::MessageLoop( )
{
	MSG message = { 0 };
	while ( message.message != WM_QUIT )
	{
		if ( ::GetMessage( &message, _null, 0, 0 ) )
		{
			::TranslateMessage( &message );
			::DispatchMessage( &message );
		}
	}
}

_void System::Sleep( _dword milliseconds )
{
	// Use SleepEx to make sure can be woken up.
	::SleepEx( milliseconds, _true );
}

_void System::HighFrequencySleep( _dword milliseconds )
{
	static Event timerevent;

	if ( (_handle) timerevent == _null )
		timerevent.Create( _false, _false );

	timerevent.Reset( );

	_dword timerid = ::timeSetEvent( milliseconds, 0, (LPTIMECALLBACK) (_handle) timerevent, 0, TIME_ONESHOT | TIME_CALLBACK_EVENT_SET );

	if ( timerid == 0 )
		return;

	timerevent.Wait( );

	::timeKillEvent( timerid );
}

String System::GetWindowPath( )
{
	_char buffer[1024];
	if ( ::GetWindowsDirectory( buffer, 1024 ) == 0 )
		return String( L"" );

	return String( buffer );
}

String System::GetSystemPath( )
{
	_char buffer[1024];
	if ( ::GetSystemDirectory( buffer, 1024 ) == 0 )
		return String( L"" );

	return String( buffer );
}

String System::GetDesktopPath( )
{
	_char buffer[1024];

	Registry registry;
	registry.SetAccessRight( Registry::_ACCESS_READ );

	if ( registry.Open( Registry::_KEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders" ) == _false )
		return String( L"" );

	_dword type = Registry::_TYPE_STRING, size = sizeof( buffer );
	if ( registry.ReadValue( L"Desktop", type, buffer, size ) == _false )
		return String( L"" );

	return String( buffer );
}

String System::GetUserPath( )
{
	String desktoppath = GetDesktopPath( );

	if ( desktoppath == L"" )
		return desktoppath;

	_dword index = desktoppath.SearchR2L( '\\' );

	if ( index == -1 )
		return desktoppath;

	return String( (const _char*) desktoppath, index );
}

String System::GetSystemTempPath( )
{
	_char buffer[1024];
	::GetTempPath( 1024, buffer );

	String temppath = buffer;
	temppath.TrimRight( '\\' );

	return temppath;
}

String System::GetProgramGroupPath( )
{
	_char buffer[1024];

	Registry registry;
	registry.SetAccessRight( Registry::_ACCESS_READ );

	if ( registry.Open( Registry::_KEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders" ) == _false )
		return String( L"" );

	_dword type = Registry::_TYPE_STRING, size = sizeof( buffer );
	if ( registry.ReadValue( L"Programs", type, buffer, size ) == _false )
		return String( L"" );

	return String( buffer );
}

String System::GetProgramFilesPath( )
{
	_char buffer[1024];

	Registry registry;
	registry.SetAccessRight( Registry::_ACCESS_READ );

	if ( registry.Open( Registry::_KEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion" ) == _false )
		return String( L"" );

	_dword type = Registry::_TYPE_STRING, size = sizeof( buffer );
	if ( registry.ReadValue( L"ProgramFilesDir", type, buffer, size ) == _false )
		return String( L"" );

	return String( buffer );
}

_dword System::GetLastError( )
{
	return ::GetLastError( );
}

_bool System::CreateFileLink( StringPtr linktarget, StringPtr targetpath, StringPtr linkpath, StringPtr linkdesc, StringPtr cmdline )
{
	::CoInitialize( _null );

	IShellLink* shelllink = NULL;

	// Get a pointer to the IShellLink interface.
	::CoCreateInstance( CLSID_ShellLink, _null, CLSCTX_INPROC_SERVER, IID_IShellLink, (_void**) &shelllink ); 

	if ( shelllink == _null )
		return _false;

	// Set the path to the shortcut target and add the description.
	shelllink->SetPath( linktarget );
	shelllink->SetArguments( cmdline );
	shelllink->SetDescription( linkdesc );
	shelllink->SetWorkingDirectory( targetpath );

	IPersistFile* persistfile = _null;
	// Query IShellLink for the IPersistFile interface for saving the shortcut.
	shelllink->QueryInterface( IID_IPersistFile, (_void**) &persistfile );

	if ( persistfile == _null )
		return _false;

	// Save the link by calling IPersistFile::Save. 
	persistfile->Save( linkpath, _true );
	persistfile->Release( );

	shelllink->Release();

	::CoUninitialize( );

	return _true;
}

_bool System::LaunchProcess( StringPtr cmdline, StringPtr workdir, KernelObject* processhandle, KernelObject* threadhandle )
{
	// Run process.
	PROCESS_INFORMATION	processinfo = { 0 };
	STARTUPINFOA		startupinfo = { sizeof( STARTUPINFOA ) };

	startupinfo.dwFlags = STARTF_FORCEOFFFEEDBACK;

	_chara ansicmdline[1024];
	StringFormatter::UnicodeToAnsi( ansicmdline, 1024, cmdline );

	_chara ansiworkdir[1024];
	StringFormatter::UnicodeToAnsi( ansiworkdir, 1024, workdir );

	// Use ansi version.
	_bool retval = ::CreateProcessA( _null, ansicmdline, _null, _null, _false, CREATE_UNICODE_ENVIRONMENT, _null, ansiworkdir[0] == 0 ? _null : ansiworkdir, &startupinfo, &processinfo );

	if ( threadhandle != _null )
	{
		*threadhandle = KernelObject( (_handle) processinfo.hThread );
	}
	else
	{
		::CloseHandle( processinfo.hThread );
	}

	if ( processhandle != _null )
	{
		*processhandle = KernelObject( (_handle) processinfo.hProcess );
	}
	else
	{
		::CloseHandle( processinfo.hProcess );
	}

	return retval;
}
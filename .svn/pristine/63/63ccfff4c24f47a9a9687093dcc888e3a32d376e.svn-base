//============================================================================
// System.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// System
//----------------------------------------------------------------------------

class System
{
public:
	enum
	{
		_CPU_UNKNOWN		=  0,
		_CPU_INTEL386		=  1,
		_CPU_INTEL486		=  2,
		_CPU_INTELP1		=  3,
		_CPU_INTELP2		=  4,
		_CPU_INTELP3		=  5,
		_CPU_INTELP4		=  6,
		_CPU_ALPHA			=  7,
		_CPU_ALPHA64		=  8,
		_CPU_AMD64			=  9,
		_CPU_IA64			= 10,

		_WINDOW_UNKNOWN		= 0,
		_WINDOW_2000		= 1,
		_WINDOW_XP			= 2,
		_WINDOW_2003		= 3,
		_WINDOW_VISTA		= 4,

		_DIRECTX_UNKNOWN	= 0,
		_DIRECTX_81			= 1,
		_DIRECTX_90			= 2,
		_DIRECTX_90a		= 3,
		_DIRECTX_90b		= 4,
		_DIRECTX_90c		= 5,
		_DIRECTX_100		= 6,

		_LANG_UNKNOWN		= 0,
		_LANG_ENG			= 1,
		_LANG_CHS			= 2,
		_LANG_CHT			= 3,
	};

	static _dword	GetCPUNumber( );

	static _dword	GetCPUID( );
	static String	GetCPUIDString( );

	static _dword	GetTotalMemory( );
	static String	GetTotalMemoryString( );

	static _dword	GetAvailableMemory( );
	static String	GetAvailableMemoryString( );

	static _qword	GetTotalDiskSpace( StringPtr directory );
	static String	GetTotalDiskSpaceString( StringPtr directory );

	static _qword	GetAvailableDiskSpace( StringPtr directory );
	static String	GetAvailableDiskSpaceString( StringPtr directory );

	static _dword	GetWindowVersion( );
	static String	GetWindowVersionString( );

	static _dword	GetDirectXVersion( );
	static String	GetDirectXVersionString( );

	static _dword	GetLocalLanguage( );
	static String	GetLocalLanguageString( );

	static String	GetLocalMACAddress( );
	static String	GetNetworkCardName( );
	static String	GetSoundCardName( );
	static String	GetVideoCardName( );

	static _dword	GetVideoMemory( );
	static String	GetVideoMemoryString( );

	static String	GetLocalComputerName( );
	static String	GetCurrentUserName( );

	static String	GetProcessFileName( );
	static String	GetProcessPathName( );
	static String	GetDllFileName( _handle dllmodule );
	static String	GetDllPathName( _handle dllmodule );

	static _dword	GetSystemLanguage( );
	static _dword	GetUserLanguage( );
	static _dword	GetKeyboardLanguage( );
	static _bool	SetKeyboardLanguage( _dword langid );

	static _dword	GetCurrentThreadID( );
	static _dword	GetCurrentProcessID( );

	static _dword	GetCurrentCPUUsage( );
	static _dword	GetCurrentMemoryUsage( );

	static _dword	GetCurrentTickcount( );
	static _dword	GetElapsedSecondSinceUTC( );

	static _qword	GetCurrentCycleCount( );
	static _qword	GetSystemCycleFrequency( );
	static _float	GetElapseTime( _qword cyclecount1, _qword cyclecount2 );

	static Time		GetCurrentLocalTime( );
	static String	GetCurrentLocalTimeString( );
	static Time		GetCurrentSystemTime( );
	static String	GetCurrentSystemTimeString( );

	static _handle	LoadSystemCursor( StringPtr cursor );

	static _bool	IsKeyDown( _dword keycode );
	static Point	GetMousePosition( _handle window );

	static _bool	SelectColorDialog( _handle window, _dword& color );

	static _bool	SetEnvironmentPrefix( StringPtr prefix );
	static _bool	GetEnvironmentPrefix( String& prefix );

	static _bool	SetEnvironment( StringPtr name, StringPtr value );
	static _bool	SetEnvironment( StringPtr name, _dword value );
	static _bool	SetEnvironment( StringPtr name, _void* value );
	static _bool	GetEnvironment( StringPtr name, String& value );
	static _bool	GetEnvironment( StringPtr name, _dword& value );
	static _bool	GetEnvironment( StringPtr name, _void*& value );

	static _void	MessageLoop( );
	static _void	Sleep( _dword milliseconds );
	static _void	HighFrequencySleep( _dword milliseconds );

	static String	GetWindowPath( );
	static String	GetSystemPath( );
	static String	GetDesktopPath( );
	static String	GetUserPath( );
	static String	GetSystemTempPath( );
	static String	GetProgramGroupPath( );
	static String	GetProgramFilesPath( );

	static _dword	GetLastError( );

	static _bool	CreateFileLink( StringPtr linktarget, StringPtr targetpath, StringPtr linkpath, StringPtr linkdesc, StringPtr cmdline );
	static _bool	LaunchProcess( StringPtr cmdline, StringPtr workdir = L"", KernelObject* processhandle = _null, KernelObject* threadhandle = _null );
};

};
#include "FairyLandCommon.h"

#ifdef _MULTIGLOBAL_ENABLE

_dword gDashTLS = 0xFFFFFFFF;

inline _dword GetTLSIndex( )
{
#ifdef _MULTIMODULE_ENABLE

	if ( gDashTLS == 0xFFFFFFFF )
		System::GetEnvironment( L"_FairyLand_Global_", gDashTLS );

#endif

	return gDashTLS;
}

inline _void SetTLSIndex( _dword index )
{
	FL_ASSERT( index != 0xFFFFFFFF )

		gDashTLS = index;

#ifdef _MULTIMODULE_ENABLE

	System::SetEnvironment( L"_FairyLand_Global_", gDashTLS );

#endif
}

_dword gDashToken = 0;

#else

FL::FairyLandGlobal FL::gGlobal;

#endif

#ifdef _MULTIGLOBAL_ENABLE

inline FairyLandGlobal& GetGlobalHelper( )
{
#ifdef _MULTIGLOBAL_ENABLE

	if ( GetTLSIndex( ) == 0xFFFFFFFF )
	{
		SetTLSIndex( ::TlsAlloc( ) );

		FairyLandGlobal* global = new FairyLandGlobal( );
		Memory::MemSet( global, 0, sizeof( FairyLandGlobal ) );

		::TlsSetValue( GetTLSIndex( ), global );

		return *global;
	}

	return *( (FairyLandGlobal*) ::TlsGetValue( GetTLSIndex( ) ) );

#else

	return gGlobal;

#endif
}

_dword FL::GetFairyLandSingletonToken( )
{
#ifdef _MULTIGLOBAL_ENABLE

	FL_ASSERT( gDashToken < 256 )

		return ++ gDashToken;

#else

	return 0;

#endif
}

_void FL::NewFairyLandGlobalContext( )
{
#ifdef _MULTIGLOBAL_ENABLE

	if ( GetTLSIndex( ) == 0xFFFFFFFF )
		SetTLSIndex( ::TlsAlloc( ) );

	FairyLandGlobal* global = new FairyLandGlobal( );
	Memory::MemSet( global, 0, sizeof( FairyLandGlobal ) );

	::TlsSetValue( GetTLSIndex( ), global );

#endif
}

_void FL::SetFairyLandGlobal( _void* global )
{
#ifdef _MULTIGLOBAL_ENABLE

	::TlsSetValue( gDashTLS, global );

#endif
}

#define PS_INLINE

#include "FairyLandGlobalInline.h"

#endif

_void FL::FreeAllFairyLandSingleton( )
{
	if ( GetDatabaseManagerPointer( ) != _null )
	{
		PS_TRACE( L"[KL] Releasing DatabaseManager ...\r\n" )
			GetDatabaseManagerPointer( )->Release( );
		PS_TRACE( L"[KL] Release DatabaseManager Finished\r\n" )
	}

	if ( GetNetworkManagerPointer( ) != _null )
	{
		PS_TRACE( L"[KL] Releasing NetworkManager ...\r\n" )
			GetNetworkManagerPointer( )->Release( );
		PS_TRACE( L"[KL] Release NetworkManager Finished\r\n" )
	}

	if ( GetResourceReplacerPointer( ) != _null )
	{
		PS_TRACE( L"[KL] Releasing ResourceReplacer ...\r\n" )
			GetResourceReplacerPointer( )->Release( );
		PS_TRACE( L"[KL] Release ResourceReplacer Finished\r\n" )
	}

	if ( GetResourceDownloaderPointer( ) != _null )
	{
		PS_TRACE( L"[KL] Releasing ResourceDownloader ...\r\n" )
			GetResourceDownloaderPointer( )->Release( );
		PS_TRACE( L"[KL] Release ResourceDownloader Finished\r\n" )
	}

	if ( GetResourceCachePointer( ) != _null )
	{
		PS_TRACE( L"[KL] Releasing ResourceCache ...\r\n" )
			GetResourceCachePointer( )->Release( );
		PS_TRACE( L"[KL] Release ResourceCache Finished\r\n" )
	}

	if ( GetGraphicsManagerPointer( ) != _null )
	{
		PS_TRACE( L"[KL] Releasing GraphicsManager ...\r\n" )
			GetGraphicsManagerPointer( )->Release( );
		PS_TRACE( L"[KL] Release GraphicsManager Finished\r\n" )
	}

	if ( GetSoundDevicePointer( ) != _null )
	{
		PS_TRACE( L"[KL] Releasing SoundDevice ... \r\n" )
			GetSoundDevicePointer( )->Release( );
		PS_TRACE( L"[KL] Release SoundDevice Finished\r\n" )
	}

	if ( GetResourceManagerPointer( ) != _null )
	{
		PS_TRACE( L"[KL] Releasing ResourceManager ...\r\n" )
			GetResourceManagerPointer( )->Release( );
		PS_TRACE( L"[KL] Release ResourceManager Finished\r\n" )
	}
/*
	if (GetScriptManagerPointer()!=_null)
	{
		PS_TRACE( L"[KL] Releasing ScriptManager ...\r\n" )
			GetResourceManagerPointer( )->Release( );
		PS_TRACE( L"[KL] Release ScriptManager Finished\r\n" )

	}
*/
	if ( GetFairyLandWorkerPointer( ) != _null )
	{
		PS_TRACE( L"[KL] Releasing FairyLandWorker ...\r\n" )
			GetFairyLandWorkerPointer( )->Release( );
		PS_TRACE( L"[KL] Release FairyLandWorker Finished\r\n" )
	}
}


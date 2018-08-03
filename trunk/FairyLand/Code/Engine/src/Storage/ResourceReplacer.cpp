//============================================================================
// ResourceReplacer.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandStorage.h"

//----------------------------------------------------------------------------
// ResourceReplacer Implementation
//----------------------------------------------------------------------------

ResourceReplacer::ResourceReplacer( ) : mReplaceHash( 36027 )
{
	mAccumulatedElapse = 0;
}

ResourceReplacer::~ResourceReplacer( )
{
	GetFairyLandGlobal( ).mResourceReplacer = _null;
}

ITexture* ResourceReplacer::CreateTextureReplacer( )
{
	return _null;
}

_void ResourceReplacer::Release( )
{
	delete this;
}

_void ResourceReplacer::Update( _dword elapse )
{
	//mAccumulatedElapse += elapse;

	//// Check replacer every 500 ms.
	//if ( mAccumulatedElapse < 500 )
	//	return;

	//elapse = mAccumulatedElapse;
	//mAccumulatedElapse = 0;

	//IResourceDownloader* downloader = GetResourceDownloaderPointer( );
	//if ( downloader == _null )
	//	return;

	//for ( ReplaceHash::Iterator it = mReplaceHash.GetHeadIterator( ); it.Valid( ); it ++ )
	//{
	//	ReplacedObject& replacedobject = it;

	//	DownloadTaskState taskstate;
	//	downloader->GetDownloadTaskState( downloader->GetDownloadTaskIndex( replacedobject.mTaskID ), taskstate );

	//	// File download finished, load resource.
	//	if ( taskstate.mPercentage == 1.0f )
	//	{
	//		downloader->DeleteTaskGroup( replacedobject.mTaskID );

	//		ITexture* texture = (ITexture*) replacedobject.mObject;

	//		GetRenderDeviceSingleton( )->ReplaceTexture( texture, replacedobject.mName );
	//		GetRenderDeviceSingleton( )->ReleaseTexture( texture );

	//		mReplaceHash.Remove( it );
	//		it --;
	//	}
	//}
}

_void* ResourceReplacer::GetTextureReplacer( StringPtr resname )
{
	//// Get lowercased name.
	//_char lowercasedname[1024];
	//StringFormatter::CopyString( lowercasedname, resname );
	//StringFormatter::LowercaseString( lowercasedname );

	//// Search replaced object in hash.
	//ReplacedObject* replacedobject = mReplaceHash.Index( lowercasedname );

	//if ( replacedobject == _null )
	//{
	//	ReplacedObject newobject;
	//	newobject.mName		= lowercasedname;
	//	newobject.mTaskID	= 0;
	//	newobject.mObject	= CreateTextureReplacer( );

	//	// Post download task.
	//	if ( GetResourceDownloaderPointer( ) != _null )
	//	{
	//		newobject.mTaskID = GetResourceDownloaderPointer( )->CreateTaskGroup( L"" );
	//		GetResourceDownloaderPointer( )->DownloadFile( newobject.mTaskID, lowercasedname );
	//	}

	//	mReplaceHash.Insert( newobject );

	//	return GetRenderDeviceSingleton( )->CloneTexture( (ITexture*) newobject.mObject );
	//}
	//else
	//{
	//	return GetRenderDeviceSingleton( )->CloneTexture( (ITexture*) replacedobject->mObject );
	//}

	return _null;
}
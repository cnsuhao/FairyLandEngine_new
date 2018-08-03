//============================================================================
// ResourceDownloader.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandStorage.h"

//----------------------------------------------------------------------------
// ResourceDownloader Implementation
//----------------------------------------------------------------------------

ResourceDownloader::ResourceDownloader( IFileDownloader* downloader ) : mDownloader( downloader )
{
	mTaskIDCreator			= 1;
	mBackgroundTaskBytes	= 0;
	mBackgroundTaskCount	= 0;

	mPauseAllDownloadTask	= _false;
	mEnableAutoDownloadFile	= _false;

	mAccumulatedElapse		= 0;

	if ( mDownloader != _null )
	{
		mDownloader->SetBackgroundDownloadBeginCallback( OnBackgroundDownloadBeginCallback );
		mDownloader->SetBackgroundDownloadFinishedCallback( OnBackgroundDownloadFinishedCallback );
	}
}

ResourceDownloader::~ResourceDownloader( )
{
	mPreloadThread.Close( );

	if ( mDownloader != _null )
		mDownloader->Release( );

	for ( _dword i = 0; i < mDownloadTaskArray.Number( ); i ++ )
		delete mDownloadTaskArray[i];

	GetFairyLandGlobal( ).mResourceDownloader = _null;
}

_bool ResourceDownloader::OnPreloadThread( _void* parameter1, _void* parameter2 )
{
	ResourceDownloader* resourcedownloader = (ResourceDownloader*) parameter1;
	FL_ASSERT( resourcedownloader != _null )

	IResourceManager* resourcemanager = GetResourceManagerPointer( );
	FL_ASSERT( resourcemanager != _null )

	IResourceCache* resourcecache = GetResourceCachePointer( );
	FL_ASSERT( resourcecache != _null )

	DownloadTask* downloadtask = (DownloadTask*) parameter2;
	FL_ASSERT( downloadtask != _null )

	_dword starttickcount = System::GetCurrentTickcount( );

	// Set task phase.
	downloadtask->mTaskPhase	= _PHASE_PROCESSING;
	downloadtask->mLastProgress = 0.0f;

	_bool taskfinished = _true;

	// Process all task in the download task.
	for ( _dword i = 0; i < downloadtask->mProcessTaskArray.Number( ); i ++ )
	{
		const ProcessTask& processtask = downloadtask->mProcessTaskArray[i];

		// The thread need to stop.
		if ( resourcedownloader->mPreloadThread.GetThreadID( ) == 0 )
			break;

		// This process task must process at main thread.
		if ( processtask.mProcessAtMainThread )
		{
			taskfinished = _false;
			continue;
		}

		if ( processtask.mProcessFunc != _null )
			(*processtask.mProcessFunc)( processtask.mProcessParameter1, processtask.mProcessParameter2 );
	}

	// Set task phase.
	downloadtask->mTaskPhase = _PHASE_PRELOADING;

	_dword fileneedtoload = 0, fileloaded = 0;

	// First get task number to preload.
	for ( _dword i = 0; i < downloadtask->mPreloadTaskArray.Number( ); i ++ )
	{
		const PreloadTask& preloadtask = downloadtask->mPreloadTaskArray[i];

		fileneedtoload += preloadtask.mFileNameArray.Number( );
	}

	if ( fileneedtoload > 0 )
	{
		// Preload files in the download task.
		for ( _dword i = 0; i < downloadtask->mPreloadTaskArray.Number( ); i ++ )
		{
			const PreloadTask& preloadtask = downloadtask->mPreloadTaskArray[i];

			// The thread need to stop.
			if ( resourcedownloader->mPreloadThread.GetThreadID( ) == 0 )
				break;

			// Insert all search path.
			for ( _dword j = 0; j < preloadtask.mSearchPathArray.Number( ); j ++ )
				resourcemanager->InsertSearchPath( preloadtask.mSearchPathArray[j] );

			// Preload all files.
			for ( _dword j = 0; j < preloadtask.mFileNameArray.Number( ); j ++ )
			{
				// The thread need to stop.
				if ( resourcedownloader->mPreloadThread.GetThreadID( ) == 0 )
					break;

				switch ( preloadtask.mPreloadType )
				{
					// Preload as binary file.
					case PreloadTask::_PRELOAD_FILE:
						resourcecache->PreloadResource( preloadtask.mFileNameArray[j] );
						break;

					// Preload as texture.
					case PreloadTask::_PRELOAD_TEXTURE:
						resourcecache->PreloadTexture( preloadtask.mFileNameArray[j], preloadtask.mPreloadMethod );
						break;

					// Preload as mesh.
					case PreloadTask::_PRELOAD_MESH:
						resourcecache->PreloadMesh( preloadtask.mFileNameArray[j], preloadtask.mPreloadMethod );
						break;

					// Preload as mesh animation.
					case PreloadTask::_PRELOAD_MESHANIMATION:
						resourcecache->PreloadMeshAnimation( preloadtask.mFileNameArray[j] );
						break;

					// Preload as skeleton.
					case PreloadTask::_PRELOAD_SKELETON:
						resourcecache->PreloadSkeleton( preloadtask.mFileNameArray[j] );
						break;

					// Preload as skeleton animation.
					case PreloadTask::_PRELOAD_SKELETONANIMATION:
						resourcecache->PreloadSkeletonAnimation( preloadtask.mFileNameArray[j] );
						break;

					// Preload as bone shadow.
					case PreloadTask::_PRELOAD_BONESHADOW:
						resourcecache->PreloadBoneShadow( preloadtask.mFileNameArray[j] );
						break;

					// Preload as particle system.
					case PreloadTask::_PRELOAD_PARTICLESYSTEM:
						resourcecache->PreloadParticleSystem( preloadtask.mFileNameArray[j] );
						break;
				}

				fileloaded ++;

				// Update preload progress.
				downloadtask->mLastProgress = (_float) fileloaded / (_float) fileneedtoload;

				if ( downloadtask->mLastProgress > 0.999f )
					downloadtask->mLastProgress = 0.999f;
			}

			// Remove all search path.
			for ( _dword j = 0; j < preloadtask.mSearchPathArray.Number( ); j ++ )
				resourcemanager->RemoveSearchPath( preloadtask.mSearchPathArray[j] );
		}
	}

	if ( taskfinished && downloadtask->mProcessMinTime > 0 )
	{
		while ( System::GetCurrentTickcount( ) - starttickcount < downloadtask->mProcessMinTime )
		{
			System::Sleep( 1 );

			// The thread need to stop.
			if ( resourcedownloader->mPreloadThread.GetThreadID( ) == 0 )
				break;
		}
	}

	if ( taskfinished )
	{
		// Set task phase.
		downloadtask->mTaskPhase = _PHASE_FINISHED;
		downloadtask->mLastProgress = 1.0f;
	}
	else
	{
		downloadtask->mTaskPhase = _PHASE_LOADING;
	}

	downloadtask->mTaskFlag &= ~_FLAG_PRELOADING;

	// We can auto delete this download task.
	if ( downloadtask->mTaskFlag & _FLAG_DELETED )
		delete downloadtask;

	return _false;
}

_bool ResourceDownloader::OnCheckLocalFileCallback( StringPtr realname, StringPtr logicname, _dword size,
	const Oword& checksum, const Oword& fastchecksum, _dword flag, _bool needed, _void* parameter1, _void* parameter2 )
{
	ResourceDownloader* resourcedownloader = (ResourceDownloader*) parameter1;
	FL_ASSERT( resourcedownloader != _null )

	// This file is not existing, skip it.
	Oword oldchecksum;
	if ( GetResourceManagerPointer( )->SearchResource( realname, _null, &oldchecksum ) == _false )
		return _true;

	// Checksum is not same, delete the local file.
	if ( checksum != oldchecksum )
		GetResourceManagerPointer( )->DeleteResource( realname );

	return _true;
}

_bool ResourceDownloader::OnDownloadNeededFileCallback( StringPtr realname, StringPtr logicname, _dword size,
	const Oword& checksum, const Oword& fastchecksum, _dword flag, _bool needed, _void* parameter1, _void* parameter2 )
{
	ResourceDownloader* resourcedownloader = (ResourceDownloader*) parameter1;
	FL_ASSERT( resourcedownloader != _null )

	DownloadParameter* downloadparameter = (DownloadParameter*) parameter2;
	FL_ASSERT( downloadparameter != _null )

	// Skip none needed file.
	if ( needed == _false )
		return _true;

	// This file can be skiped.
	if ( downloadparameter->mSkipFile != L"" && logicname.SearchL2RLowercase( downloadparameter->mSkipFile ) != -1 )
		return _true;

	resourcedownloader->DownloadFile( downloadparameter->mTaskID, realname, checksum, size, downloadparameter->mDeleteInvalidFile );

	return _true;
}

_void ResourceDownloader::OnBackgroundDownloadBeginCallback( StringPtr filename, const Oword& checksum, _dword filesize, _dword taskid )
{
}

_void ResourceDownloader::OnBackgroundDownloadFinishedCallback( StringPtr filename, const Oword& checksum, _dword filesize, _dword taskid )
{
	ResourceDownloader* resourcedownloader = (ResourceDownloader*) GetResourceDownloaderPointer( );
	FL_ASSERT( resourcedownloader != _null )

	IResourceManager* resourcemanager = GetResourceManagerPointer( );
	FL_ASSERT( resourcemanager != _null )

	if ( resourcedownloader->mDownloader != _null )
		resourcedownloader->mDownloader->FinishFile( taskid );

	resourcedownloader->mBackgroundTaskBytes -= filesize;
	if ( resourcedownloader->mBackgroundTaskCount != 0 )
		resourcedownloader->mBackgroundTaskCount --;

	while( ( resourcedownloader->mBackgroundTaskBytes < BG_TASK_BYTES && resourcedownloader->mBackgroundTaskCount < BG_TASK_COUNT ) || resourcedownloader->mBackgroundTaskCount == 0 )
	{
		Oword newchecksum; _dword newfilesize;
		StringPtr newfilename = resourcemanager->GetNextFileInManifest( &newfilesize, &newchecksum );

		// Auto download files in manifest at background.
		if ( resourcedownloader->AutoDownloadFile( newfilename, newchecksum, newfilesize ) == _false )
			return;
	}
}

_bool ResourceDownloader::AutoDownloadFile( StringPtr filename, Oword& checksum, _dword filesize )
{
	if ( mEnableAutoDownloadFile == _false )
		return _false;

	if ( filename == L"" )
		return _false;

	if ( mDownloader == _null )
		return _false;

	IResourceManager* resourcemanager = GetResourceManagerPointer( );
	if ( resourcemanager == _null )
		return _false;

	while ( resourcemanager->SearchResource( filename ) )
	{
		filename = resourcemanager->GetNextFileInManifest( &filesize, &checksum );

		// All file downloaded.
		if ( filename == L"" )
		{
			mDownloader->SetDownloadFinish( );

			return _false;
		}
	}

	mDownloader->BackgroundDownloadFile( filename, checksum, filesize );
	FL_ASSERT(filesize != -1);
	mBackgroundTaskBytes += filesize;
	++mBackgroundTaskCount;
	return _true;
}

_void ResourceDownloader::Release( )
{
	delete this;
}

_void ResourceDownloader::Update( _dword elapse )
{
	mAccumulatedElapse += elapse;

	// Check downloader every 1 second.
	if ( mAccumulatedElapse < 1000 )
		return;

	elapse = mAccumulatedElapse;
	mAccumulatedElapse = 0;
}

_void ResourceDownloader::InsertSource( StringPtr sourcename )
{
	if ( mDownloader != _null )
		mDownloader->InsertSource( sourcename );
}

_void ResourceDownloader::ClearSource( )
{
	if ( mDownloader != _null )
		mDownloader->ClearSource( );
}

StringPtr ResourceDownloader::GetConnectedSource( ) const
{
	if ( mDownloader == _null )
		return L"";

	return mDownloader->GetConnectedSource( );
}

_dword ResourceDownloader::CreateTaskGroup( StringPtr taskname, _dword phase, _dword taskflag )
{
	DownloadTask* downloadtask = new DownloadTask;

	downloadtask->mTaskID				= mTaskIDCreator ++;
	downloadtask->mTaskPhase			= phase == _PHASE_VERIFYING ? _PHASE_VERIFYING : _PHASE_DOWNLOADING;
	downloadtask->mTaskFlag				= taskflag & _FLAG_MASK;
	downloadtask->mTaskName				= taskname;

	downloadtask->mLastProgress			= 0.0f;
	downloadtask->mLastTickcount		= 0;

	downloadtask->mLoadingIndex			= 0;
	downloadtask->mProcessMinTime		= 0;

	mDownloadTaskArray.Append( downloadtask );

	return downloadtask->mTaskID;
}

_bool ResourceDownloader::DeleteTaskGroup( _dword taskid )
{
	for ( _dword i = 0; i < mDownloadTaskArray.Number( ); i ++ )
	{
		// Found the task.
		if ( mDownloadTaskArray[i]->mTaskID == taskid )
		{
			DownloadTask* downloadtask = mDownloadTaskArray[i];

			if ( mDownloader != _null )
				mDownloader->PauseDownload( );

			// Finishe files in the download task.
			for ( _dword j = 0; j < downloadtask->mFileTaskArray.Number( ); j ++ )
			{
				if ( mDownloader != _null )
				{
					if ( downloadtask->mTaskFlag & _FLAG_PAUSED )
						mDownloader->ResumeFile( downloadtask->mFileTaskArray[j].mFileTaskID );

					mDownloader->FinishFile( downloadtask->mFileTaskArray[j].mFileTaskID );
				}
			}

			if ( ( downloadtask->mTaskFlag & _FLAG_PRELOADING ) == 0 )
				downloadtask->mTaskPhase = _PHASE_FINISHED;

			// We cant delete download task when it is processing, so we wait.
			while ( downloadtask->mTaskPhase == _PHASE_PROCESSING )
				System::Sleep( 1 );

			// We cant delete download task when it is loading.
			if ( downloadtask->mTaskPhase == _PHASE_PRELOADING )
			{
				downloadtask->mTaskFlag |= _FLAG_DELETED;
			}
			// We delete it directly.
			else
			{
				delete downloadtask;
			}

			mDownloadTaskArray.Remove( i );

			if ( mDownloader != _null )
				mDownloader->ResumeDownload( );

			return _true;
		}
	}

	return _false;
}

_bool ResourceDownloader::PauseTaskGroup( _dword taskid )
{
	if ( mPauseAllDownloadTask )
		return _false;

	for ( _dword i = 0; i < mDownloadTaskArray.Number( ); i ++ )
	{
		// Found the task.
		if ( mDownloadTaskArray[i]->mTaskID == taskid )
		{
			DownloadTask* downloadtask = mDownloadTaskArray[i];

			// Already paused.
			if ( downloadtask->mTaskFlag & _FLAG_PAUSED )
				return _true;

			// Pause files in the download task.
			for ( _dword j = 0; j < downloadtask->mFileTaskArray.Number( ); j ++ )
			{
				if ( mDownloader != _null )
					mDownloader->PauseFile( downloadtask->mFileTaskArray[j].mFileTaskID );
			}

			downloadtask->mTaskFlag |= _FLAG_PAUSED;

			return _true;
		}
	}

	return _false;
}

_bool ResourceDownloader::ResumeTaskGroup( _dword taskid )
{
	if ( mPauseAllDownloadTask )
		return _false;

	for ( _dword i = 0; i < mDownloadTaskArray.Number( ); i ++ )
	{
		// Found the task.
		if ( mDownloadTaskArray[i]->mTaskID == taskid )
		{
			DownloadTask* downloadtask = mDownloadTaskArray[i];

			// Already not paused.
			if ( ( downloadtask->mTaskFlag & _FLAG_PAUSED ) == 0 )
				return _true;

			// Resume files in the download task.
			for ( _dword j = 0; j < downloadtask->mFileTaskArray.Number( ); j ++ )
			{
				if ( mDownloader != _null )
					mDownloader->ResumeFile( downloadtask->mFileTaskArray[j].mFileTaskID );
			}

			downloadtask->mTaskFlag &= ~_FLAG_PAUSED;

			return _true;
		}
	}

	return _false;
}

_bool ResourceDownloader::CreatePreloadTask( _dword taskid, const PreloadTask& preloadtask )
{
	for ( _dword i = 0; i < mDownloadTaskArray.Number( ); i ++ )
	{
		// Found the task.
		if ( mDownloadTaskArray[i]->mTaskID == taskid )
		{
			DownloadTask* downloadtask = mDownloadTaskArray[i];

			downloadtask->mLastProgress		= 0.0f;
			downloadtask->mTaskPhase		= _PHASE_DOWNLOADING;
			downloadtask->mLastTickcount	= 0;
			downloadtask->mPreloadTaskArray.Append( preloadtask );

			return _true;
		}
	}

	return _false;
}

_bool ResourceDownloader::ClearPreloadTask( _dword taskid )
{
	for ( _dword i = 0; i < mDownloadTaskArray.Number( ); i ++ )
	{
		// Found the task.
		if ( mDownloadTaskArray[i]->mTaskID == taskid )
		{
			DownloadTask* downloadtask = mDownloadTaskArray[i];

			downloadtask->mPreloadTaskArray.Clear( _true );

			return _true;
		}
	}

	return _false;
}

_bool ResourceDownloader::CreateProcessTask( _dword taskid, const ProcessTask& processtask )
{
	for ( _dword i = 0; i < mDownloadTaskArray.Number( ); i ++ )
	{
		// Found the task.
		if ( mDownloadTaskArray[i]->mTaskID == taskid )
		{
			DownloadTask* downloadtask = mDownloadTaskArray[i];

			downloadtask->mLastProgress		= 0.0f;
			downloadtask->mTaskPhase		= _PHASE_DOWNLOADING;
			downloadtask->mLastTickcount	= 0;
			downloadtask->mProcessTaskArray.Append( processtask );

			return _true;
		}
	}

	return _false;
}

_bool ResourceDownloader::ClearProcessTask( _dword taskid )
{
	for ( _dword i = 0; i < mDownloadTaskArray.Number( ); i ++ )
	{
		// Found the task.
		if ( mDownloadTaskArray[i]->mTaskID == taskid )
		{
			DownloadTask* downloadtask = mDownloadTaskArray[i];

			downloadtask->mProcessTaskArray.Clear( _true );

			return _true;
		}
	}

	return _false;
}

_bool ResourceDownloader::SetProcessMinTime( _dword taskid, _dword mintime )
{
	for ( _dword i = 0; i < mDownloadTaskArray.Number( ); i ++ )
	{
		// Found the task.
		if ( mDownloadTaskArray[i]->mTaskID == taskid )
		{
			DownloadTask* downloadtask = mDownloadTaskArray[i];

			downloadtask->mProcessMinTime = mintime;

			return _true;
		}
	}

	return _false;
}

_bool ResourceDownloader::DownloadFile( _dword taskid, StringPtr filename )
{
	return DownloadFile( taskid, filename, Oword( 0, 0, 0, 0 ), -1, _false );
}

_bool ResourceDownloader::DownloadFile( _dword taskid, StringPtr filename, const Oword& checksum, _dword filesize, _bool checklocalfile )
{
	if ( filename.IsBlank( ) )
		return _true;

	_dword length = filename.Length( );
	if ( filename[ length - 1 ] == '\\' || filename[ length - 1 ] == '/' )
		return _true;

	if ( mDownloader == _null )
		return _true;

	if ( GetResourceManagerPointer( ) == _null )
		return _false;

	// Make file name lowercased.
	_char lowercasedfilename[1024];
	StringFormatter::CopyString( lowercasedfilename, filename );
	StringFormatter::LowercaseString( lowercasedfilename );

	for ( _dword i = 0; i < mDownloadTaskArray.Number( ); i ++ )
	{
		DownloadTask* downloadtask = mDownloadTaskArray[ mDownloadTaskArray.Number( ) - i - 1 ];

		if ( downloadtask->mTaskID != taskid )
			continue;

		// If file name is same as existing file, just return.
		if ( ( downloadtask->mTaskFlag & _FLAG_UNIQUEFLIE ) == 0 )
		{
			if ( downloadtask->mFileTaskArray.SearchAscending( lowercasedfilename ).Valid( ) )
				return _true;
		}

		// Check local file with hash code.
		if ( checklocalfile )
		{
			Oword oldchecksum;
			if ( GetResourceManagerPointer( )->SearchResource( lowercasedfilename, _null, &oldchecksum ) )
			{
				if ( checksum != oldchecksum )
					GetResourceManagerPointer( )->DeleteResource( lowercasedfilename );
			}
		}

		FileTask filetask;
		filetask.mFileName		= lowercasedfilename;
		filetask.mFileTaskID	= 0;
		filetask.mFileSize		= filesize;
		filetask.mProgress		= _null;

		// Post a download request to downloader.
		filetask.mFileTaskID = mDownloader->DownloadFile( lowercasedfilename, checksum, filesize, &filetask.mProgress );
		if ( filetask.mFileTaskID == 0 )
			return _false;

		if ( ( downloadtask->mTaskFlag & _FLAG_UNIQUEFLIE ) == 0 )
		{
			downloadtask->mFileTaskArray.InsertAscending( filetask );
		}
		else
		{
			downloadtask->mFileTaskArray.Append( filetask );
		}

		// Update task phase.
		downloadtask->mTaskPhase	= _PHASE_DOWNLOADING;
		downloadtask->mLastProgress	= 0.0f;

		return _true;
	}

	return _false;
}

_void ResourceDownloader::ClearDownloadTask( )
{
	mPreloadThread.ClearAllFunc( );

	for ( _dword i = 0; i < mDownloadTaskArray.Number( ); i ++ )
	{
		DownloadTask* downloadtask = mDownloadTaskArray[i];

		for ( _dword j = 0; j < downloadtask->mFileTaskArray.Number( ); j ++ )
		{
			if ( mDownloader != _null )
				mDownloader->FinishFile( downloadtask->mFileTaskArray[j].mFileTaskID );
		}

		if ( ( downloadtask->mTaskFlag & _FLAG_PRELOADING ) == 0 )
			downloadtask->mTaskPhase = _PHASE_FINISHED;

		// We cant delete download task when it is processing, so we wait.
		while ( downloadtask->mTaskPhase == _PHASE_PROCESSING )
			System::Sleep( 1 );

		// We cant delete download task when it is loading.
		if ( downloadtask->mTaskPhase == _PHASE_PRELOADING )
		{
			downloadtask->mTaskFlag |= _FLAG_DELETED;
		}
		// We delete it directly.
		else
		{
			delete downloadtask;
		}
	}

	mDownloadTaskArray.Clear( );
}

_bool ResourceDownloader::IsAllDownloadTaskPaused( ) const
{
	return mPauseAllDownloadTask;
}

_void ResourceDownloader::PauseAllDownloadTask( )
{
	mPauseAllDownloadTask = _true;

	for ( _dword i = 0; i < mDownloadTaskArray.Number( ); i ++ )
	{
		DownloadTask* downloadtask = mDownloadTaskArray[i];

		if ( downloadtask->mTaskFlag & _FLAG_PAUSED )
			continue;

		// Pause files in the download task.
		for ( _dword j = 0; j < downloadtask->mFileTaskArray.Number( ); j ++ )
		{
			if ( mDownloader != _null )
				mDownloader->PauseFile( downloadtask->mFileTaskArray[j].mFileTaskID );
		}

		downloadtask->mTaskFlag |= _FLAG_PAUSED;
	}

	if ( mDownloader != _null )
		mDownloader->PauseDownload( );
}

_void ResourceDownloader::ResumeAllDownloadTask( )
{
	mPauseAllDownloadTask = _false;

	for ( _dword i = 0; i < mDownloadTaskArray.Number( ); i ++ )
	{
		DownloadTask* downloadtask = mDownloadTaskArray[i];

		if ( ( downloadtask->mTaskFlag & _FLAG_PAUSED ) == 0 )
			continue;

		// Pause files in the download task.
		for ( _dword j = 0; j < downloadtask->mFileTaskArray.Number( ); j ++ )
		{
			if ( mDownloader != _null )
				mDownloader->ResumeFile( downloadtask->mFileTaskArray[j].mFileTaskID );
		}

		downloadtask->mTaskFlag &= ~_FLAG_PAUSED;
	}

	if ( mDownloader != _null )
		mDownloader->ResumeDownload( );
}

_bool ResourceDownloader::CheckLocalFile( )
{
	IResourceManager* resourcemanager = GetResourceManagerPointer( );
	if ( resourcemanager == _null )
		return _false;

	resourcemanager->EnableRootArchive( _false );
	resourcemanager->EnumManifest( OnCheckLocalFileCallback, this, _null );
	resourcemanager->EnableRootArchive( _true );

	return _true;
}

_bool ResourceDownloader::DownloadNeededFile( _dword taskid, StringPtr skipfile, _bool deleteinvalidfile )
{
	if ( taskid == 0 )
		return _false;

	if ( mDownloader == _null )
		return _true;

	IResourceManager* resourcemanager = GetResourceManagerPointer( );
	if ( resourcemanager == _null )
		return _false;

	LockOwner lockowner( GetNetworkManager( ).GetNetworkLock( ) );

	DownloadParameter downloadparameter;
	downloadparameter.mTaskID				= taskid;
	downloadparameter.mSkipFile				= skipfile;
	downloadparameter.mDeleteInvalidFile	= deleteinvalidfile;

	DownloadTask* downloadtask = _null;

	for ( _dword i = 0; i < mDownloadTaskArray.Number( ); i ++ )
	{
		// Found the task.
		if ( mDownloadTaskArray[i]->mTaskID == taskid )
		{
			downloadtask = mDownloadTaskArray[i];
			break;
		}
	}

	FL_ASSERT( downloadtask != _null )

	mDownloader->PauseDownload( );

	resourcemanager->EnableRootArchive( _false );
	resourcemanager->EnumManifest( OnDownloadNeededFileCallback, this, &downloadparameter );
	resourcemanager->EnableRootArchive( _true );

	mDownloader->ResumeDownload( );

	downloadtask->mTaskPhase = _PHASE_DOWNLOADING;

	return _true;
}

_void ResourceDownloader::EnableAutoDownloadFile( _bool enable )
{
	if ( mEnableAutoDownloadFile == enable )
		return;

	mEnableAutoDownloadFile = enable;

	if ( mEnableAutoDownloadFile )
	{
		IResourceManager* resourcemanager = GetResourceManagerPointer( );
		if ( resourcemanager == _null )
			return;

		PS_TRACE( L"[RD] Start auto download\r\n" )

		Oword checksum; _dword filesize;
		StringPtr filename = resourcemanager->GetFirstFileInManifest( &filesize, &checksum );
		// Auto download files in manifest at background.
		AutoDownloadFile( filename, checksum, filesize );
		filename = resourcemanager->GetNextFileInManifest( &filesize, &checksum );
		AutoDownloadFile( filename, checksum, filesize );
	}
}

_bool ResourceDownloader::IsAutoDownloadFileEnabled( ) const
{
	return mEnableAutoDownloadFile;
}

_void ResourceDownloader::SetMaxDownloadSpeed( _dword bytespersecond )
{
	if ( mDownloader != _null )
		mDownloader->SetMaxDownloadSpeed( bytespersecond );
}

_void ResourceDownloader::SetMaxUploadSpeed( _dword bytespersecond )
{
	if ( mDownloader != _null )
		mDownloader->SetMaxUploadSpeed( bytespersecond );
}

_void ResourceDownloader::SetDownloadedResource( _dword resourceid )
{
	if ( mDownloadedResourceArray.SearchAscending( resourceid ).Valid( ) )
		return;

	mDownloadedResourceArray.InsertAscending( resourceid );
}

_bool ResourceDownloader::IsResourceDownloaded( _dword resourceid ) const
{
	return mDownloadedResourceArray.SearchAscending( resourceid ).Valid( );
}

_dword ResourceDownloader::GetDownloadTaskNumber( ) const
{
	return mDownloadTaskArray.Number( );
}

_dword ResourceDownloader::GetDownloadTaskIndex( _dword taskid ) const
{
	for ( _dword i = 0; i < mDownloadTaskArray.Number( ); i ++ )
	{
		DownloadTask* downloadtask = mDownloadTaskArray[i];

		if ( downloadtask->mTaskID == taskid )
			return i;
	}

	return -1;
}

_bool ResourceDownloader::GetDownloadTaskState( _dword taskindex, DownloadTaskState& downloadtaskstate, _bool startpreload )
{
	downloadtaskstate.mTaskID		= 0;
	downloadtaskstate.mTaskName		= L"";
	downloadtaskstate.mTaskPhase	= _PHASE_DOWNLOADING;
	downloadtaskstate.mFileNumber	= 0;
	downloadtaskstate.mPercentage	= 0.0f;

	if ( mDownloader == _null )
		return _false;

	if ( taskindex >= mDownloadTaskArray.Number( ) )
		return _false;

	DownloadTask* downloadtask = mDownloadTaskArray[ taskindex ];
	FL_ASSERT( downloadtask != _null )

	downloadtaskstate.mTaskID		= downloadtask->mTaskID;
	downloadtaskstate.mTaskName		= downloadtask->mTaskName;
	downloadtaskstate.mTaskPhase	= downloadtask->mTaskPhase;
	downloadtaskstate.mFileNumber	= downloadtask->mFileTaskArray.Number( );
	downloadtaskstate.mPercentage	= downloadtask->mLastProgress;

	// In verifying phase.
	if ( downloadtask->mTaskPhase == _PHASE_VERIFYING )
		return _true;

	// Not ready to get task state yet.
	if ( downloadtask->mLastTickcount + 100 >= System::GetCurrentTickcount( ) )
		return _true;

	// Update query task state tickcount.
	downloadtask->mLastTickcount = System::GetCurrentTickcount( );

	// All download task paused, we dont need check.
	if ( mPauseAllDownloadTask )
		return _true;

	// It is paused.
	if ( downloadtask->mTaskFlag & _FLAG_PAUSED )
		return _true;

	// It is preloading.
	if ( downloadtask->mTaskFlag & _FLAG_PRELOADING )
		return _true;

	// In loading phase.
	if ( downloadtask->mTaskPhase == _PHASE_LOADING )
	{
		_qword startcycle	= System::GetCurrentCycleCount( );
		_qword cyclefreq	= System::GetSystemCycleFrequency( );

		for ( ; downloadtask->mLoadingIndex < downloadtask->mProcessTaskArray.Number( ); downloadtask->mLoadingIndex ++ )
		{
			const ProcessTask& processtask = downloadtask->mProcessTaskArray[ downloadtask->mLoadingIndex ];

			if ( processtask.mProcessAtMainThread == _false )
				continue;

			if ( processtask.mProcessFunc != _null )
				(*processtask.mProcessFunc)( processtask.mProcessParameter1, processtask.mProcessParameter2 );

			// Get process time, in millisecond.
			_float taketime = (_float) ( System::GetCurrentCycleCount( ) - startcycle ) / (_float) cyclefreq * 1000.0f;

			// Already process enough time.
			if ( taketime > 50.0f )
				break;
		}

		if ( downloadtask->mLoadingIndex == downloadtask->mProcessTaskArray.Number( ) )
			downloadtask->mTaskPhase = _PHASE_FINISHED;

		// Already finished.
		if ( downloadtask->mTaskPhase == _PHASE_FINISHED )
			downloadtask->mLastProgress = 1.0f;

		downloadtaskstate.mTaskPhase	= downloadtask->mTaskPhase;
		downloadtaskstate.mPercentage	= downloadtask->mLastProgress;

		return _true;
	}

	// Already finished.
	if ( downloadtask->mTaskPhase == _PHASE_FINISHED )
	{
		downloadtask->mLastProgress   = 1.0f;
		downloadtaskstate.mPercentage = 1.0f;
		return _true;
	}

	_bool allfinished = _true; _dword total = 0, current = 0;

	if ( downloadtask->mFileTaskArray.Number( ) > 0 )
	{
		// Network thread so busy now.
		if ( GetNetworkManager( ).GetNetworkLock( ).TryEnter( 5 ) == _false )
			return _true;

		// Check file array in this download task.
		for ( _dword i = 0; i < downloadtask->mFileTaskArray.Number( ); i ++ )
		{
			FileTask& filetask = downloadtask->mFileTaskArray[i];

			_float progress = filetask.mProgress == _null ? 0.0f : *filetask.mProgress;

			// At least one task not finished.
			if ( progress != 1.0f )
				allfinished = _false;

			_dword size = filetask.mFileSize == -1 ? 1024 * 1024 : filetask.mFileSize;

			total	+= size;
			current	+= (_dword) ( size * progress );
		}

		GetNetworkManager( ).GetNetworkLock( ).Leave( );
	}

	if ( allfinished == _true )
	{
		downloadtaskstate.mPercentage = 1.0f;
	}
	else
	{
		if ( total > 0 )
			downloadtaskstate.mPercentage = (_float) current / (_float) total;
		else
			downloadtaskstate.mPercentage = 0.0f;

		// Make sure the percentage will not reach 1.0 before finished.
		if ( downloadtaskstate.mPercentage > 0.996f )
			downloadtaskstate.mPercentage = 0.996f;
	}

	if ( downloadtaskstate.mPercentage == 1.0f )
	{
		// Set task finish flag.
		downloadtask->mTaskPhase = _PHASE_FINISHED;

		_dword preloadtasknumber = downloadtask->mPreloadTaskArray.Number( );
		_dword processtasknumber = downloadtask->mProcessTaskArray.Number( );

		// We need preload some resource using preload callback function.
		if ( startpreload && ( preloadtasknumber != 0 || processtasknumber != 0 ) )
		{
			_bool needbackgroundthread = preloadtasknumber > 0;

			// No preload task, let's check if there is background process task.
			if ( needbackgroundthread == _false )
			{
				for ( _dword i = 0; i < processtasknumber; i ++ )
				{
					const ProcessTask& processtask = downloadtask->mProcessTaskArray[i];

					// At least one process task need processed at background task.
					if ( processtask.mProcessAtMainThread == _false )
					{
						needbackgroundthread = _true;
						break;
					}
				}
			}

			// Process at main thread.
			if ( needbackgroundthread == _false )
			{
				downloadtask->mTaskPhase		= _PHASE_LOADING;
				downloadtask->mLastProgress		= 0.0f;
				downloadtaskstate.mPercentage	= 0.0f;
			}
			// Process at background thread.
			else
			{
				Thread* loadthread = (Thread*) &mPreloadThread;

				// Create load thead as needed.
				if ( loadthread->GetThreadID( ) == 0 )
					loadthread->Create( 100 );

				// Post load request to the thread.
				if ( loadthread->GetThreadID( ) != 0 )
				{
					downloadtask->mTaskFlag			|= _FLAG_PRELOADING;
					downloadtask->mTaskPhase		= _PHASE_PROCESSING;
					downloadtask->mLastProgress		= 0.0f;
					downloadtaskstate.mPercentage	= 0.0f;

					loadthread->RegisterUpdateFunc( OnPreloadThread, (_void*) this, downloadtask );
				}
			}
		}
	}

	// Make sure progress never goes down.
	if ( downloadtask->mLastProgress > downloadtaskstate.mPercentage )
		downloadtaskstate.mPercentage = downloadtask->mLastProgress;

	// Update cached task state.
	downloadtask->mLastProgress = downloadtaskstate.mPercentage;
	downloadtaskstate.mTaskPhase = downloadtask->mTaskPhase;

	if ( downloadtask->mTaskPhase == _PHASE_FINISHED )
	{
		downloadtask->mLastProgress   = 1.0f;
		downloadtaskstate.mPercentage = 1.0f;
	}

	return _true;
}

_bool ResourceDownloader::GetDownloadFileState( _dword taskindex, _dword fileindex, DownloadFileState& downloadfilestate )
{
	if ( mDownloader == _null )
		return _false;

	if ( taskindex >= mDownloadTaskArray.Number( ) )
		return _false;

	DownloadTask* downloadtask = mDownloadTaskArray[ taskindex ];
	FL_ASSERT( downloadtask != _null )

	if ( fileindex >= downloadtask->mFileTaskArray.Number( ) )
		return _false;

	// Get downloading file state from downloader by task id.
	return mDownloader->GetDownloadFileState( downloadtask->mFileTaskArray[ fileindex ].mFileTaskID, downloadfilestate );
}

_bool ResourceDownloader::GetDownloadSourceState( _dword taskindex, _dword fileindex, _dword sourceindex, DownloadSourceState& downloadsourcestate )
{
	if ( mDownloader == _null )
		return _false;

	if ( taskindex >= mDownloadTaskArray.Number( ) )
		return _false;

	DownloadTask* downloadtask = mDownloadTaskArray[ taskindex ];
	FL_ASSERT( downloadtask != _null )

	if ( fileindex >= downloadtask->mFileTaskArray.Number( ) )
		return _false;

	// Get downloading file state from downloader by task id.
	return mDownloader->GetDownloadSourceState( downloadtask->mFileTaskArray[ fileindex ].mFileTaskID, sourceindex, downloadsourcestate );
}

_dword ResourceDownloader::GetRecentDownloadBytes( _dword seconds ) const
{
	if ( mDownloader == _null )
		return 0;

	return mDownloader->GetRecentDownloadBytes( seconds );
}

_float ResourceDownloader::GetRecentDownloadSpeed( _dword seconds ) const
{
	if ( mDownloader == _null )
		return 0.0f;

	return mDownloader->GetRecentDownloadSpeed( seconds );
}

_dword ResourceDownloader::GetRecentUploadBytes( _dword seconds ) const
{
	if ( mDownloader == _null )
		return 0;

	return mDownloader->GetRecentUploadBytes( seconds );
}

_float ResourceDownloader::GetRecentUploadSpeed( _dword seconds ) const
{
	if ( mDownloader == _null )
		return 0.0f;

	return mDownloader->GetRecentUploadSpeed( seconds );
}

_dword ResourceDownloader::GetTotalDownloadBytes( ) const
{
	if ( mDownloader == _null )
		return 0;

	return mDownloader->GetTotalDownloadBytes( );
}

_float ResourceDownloader::GetTotalDownloadSpeed( ) const
{
	if ( mDownloader == _null )
		return 0.0f;

	return mDownloader->GetTotalDownloadSpeed( );
}

_dword ResourceDownloader::GetTotalUploadBytes( ) const
{
	if ( mDownloader == _null )
		return 0;

	return mDownloader->GetTotalUploadBytes( );
}

_float ResourceDownloader::GetTotalUploadSpeed( ) const
{
	if ( mDownloader == _null )
		return 0.0f;

	return mDownloader->GetTotalUploadSpeed( );
}

_dword ResourceDownloader::GetCurrentDownloadSourceNumber( ) const
{
	if ( mDownloader == _null )
		return 0;

	return mDownloader->GetCurrentDownloadSourceNumber( );
}

_dword ResourceDownloader::GetCurrentUploadSourceNumber( ) const
{	
	if ( mDownloader == _null )
		return 0;

	return mDownloader->GetCurrentUploadSourceNumber( );
}

_dword ResourceDownloader::GetDownloadingTaskNumber( ) const
{
	return mDownloadTaskArray.Number( );
}

_dword ResourceDownloader::GetDownloadingFileNumber( ) const
{
	if ( mDownloader == _null )
		return 0;

	return mDownloader->GetDownloadingFileNumber( );
}

_void ResourceDownloader::SetCurrentDownloadingProduct( StringPtr product )
{
	if ( mDownloader == _null )
		return;

    mDownloader->SetCurrentDownloadingProduct( product );
}

_bool ResourceDownloader::RequestGameMainManifestInfo( StringPtr product, StringPtr version )
{
	if ( mDownloader == _null )
		return _false;

	return mDownloader->RequestGameMainManifestInfo( product, version ) != 0 ? _true : _false;
}

_bool ResourceDownloader::GetGameMainManifestInfo( ManifestInfo &info, _bool clear )
{
	if ( mDownloader == _null )
		return _false;

	return mDownloader->GetGameMainManifestInfo( info, clear );
}

_bool ResourceDownloader::RequestGameMainManifestInfo( StringPtr product, StringPtr version0, StringPtr version1 )
{
	if ( mDownloader == _null ) return _false;
	Array< StringPtr > versions;
	versions.Append( version0 );
	versions.Append( version1 );
	return mDownloader->RequestManifestInfo( product, versions );
}

_bool ResourceDownloader::GetGameMainManifestInfos( Array< ManifestInfo > &infos )
{
	if ( mDownloader == _null ) return _false;
	return mDownloader->GetManifestInfo( infos );
}

_void ResourceDownloader::SetDebugParam( StringPtr key, StringPtr value )
{
	if ( mDownloader ) mDownloader->SetDebugParam( key, value );
}

//============================================================================
// HTTPDownloader.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandNetwork.h"

//----------------------------------------------------------------------------
// HTTPDownloader Implementation
//----------------------------------------------------------------------------

HTTPDownloader::HTTPDownloader( ) : mDownloadTaskPool( 0xFFFF )
{
	// Attempts to make a connection to the internet.
	::InternetAttemptConnect( 0 );

	// Open an internet handle.
	mInternet = ::InternetOpen( L"Mozilla/4.0 (compatible; )", INTERNET_OPEN_TYPE_PRECONFIG, _null, _null, 0 );

	mConnectedServerFunc			= _null;
	mDownloadBeginFunc				= _null;
	mDownloadFinishedFunc			= _null;
	mBackgroundDownloadBeginFunc	= _null;
	mBackgroundDownloadFinishedFunc	= _null;

	mDownloadTaskPool.Create( _null );
}

HTTPDownloader::~HTTPDownloader( )
{
	if ( mInternet != _null )
		::InternetCloseHandle( mInternet );

	// First clear all source, and stop download thread.
	ClearSource( );

	// Then delete all download task.
	for ( DownloadTaskPool::Iterator it = mDownloadTaskPool.GetHeadIterator( ); it.Valid( ); it ++ )
	{
		DownloadTask* dltask = (DownloadTask*) it;

		if ( dltask == _null )
			continue;

		for ( _dword i = 0; i < dltask->mChunkArray.Number( ); i ++ )
			delete dltask->mChunkArray[i];

		if ( dltask->mDataStream != _null )
			dltask->mDataStream->Release( );

		delete dltask;
	}
}

_bool HTTPDownloader::DownloadThread( _void* param1, _void* param2 )
{
	DownloadSource* dlsource = (DownloadSource*) param2;

	dlsource->mTaskLock.Enter( );
	_dword number = dlsource->mTaskQueue.Number( );
	dlsource->mTaskLock.Leave( );

	IResourceManager* resourcemanager = GetResourceManagerPointer( );
	FL_ASSERT( resourcemanager != _null )

	// Process each task at background thread.
	for ( _dword i = 0; i < number; i ++ )
	{
		dlsource->mTaskLock.Enter( );
		DownloadTask* dltask = dlsource->mTaskQueue.Front( );
		dlsource->mTaskQueue.Remove( );
		dlsource->mTaskLock.Leave( );

		_bool downloaded = _false;

		// We need process this download task.
		if ( dltask->GetRefCount( ) > 1 )
		{
			_dword start = -1, end = -1, filesize = -1, offset = -1; Oword fullchecksum, datachecksum;

			if ( resourcemanager->GetResourceInformation( dltask->mFileURL, filesize, fullchecksum, datachecksum, &offset ) )
			{
				// File size is 0.
				if ( filesize == 0 )
				{
					// Set progress to finish.
					dltask->mProgress = 1.0f;
					dltask->DecreaseRefCount( );

					resourcemanager->CreateResource( dltask->mFileURL, _null, 0, &fullchecksum );

					PS_TRACE1( L"[DL] Create empty file %s\r\n", (const _char*) dltask->mFileURL );

					continue;
				}

				if ( offset != -1 )
				{
					start = offset;
					end   = offset + filesize - 1;

					PS_TRACE3( L"[DL] Start download file %s (%d-%d)\r\n", (const _char*) dltask->mFileURL, start, end );
				}
			}
			else
			{
				PS_TRACE1( L"[DL] Start download file %s\r\n", (const _char*) dltask->mFileURL );
			}

			downloaded = dlsource->mSession->StartRequest( dltask->mFileURL, start, end );

			// Check response data type.
			if ( downloaded )
			{
				_dword type = dlsource->mSession->GetResponseType( );

				downloaded = type == HTTPSession::_TYPE_BINARY || type == HTTPSession::_TYPE_IMAGE;
			}

			// Download started.
			if ( downloaded )
			{
				filesize = offset = 0;

				// Get total response size.
				_dword totalsize = dlsource->mSession->GetResponseSize( );

				while ( _true )
				{
					DownloadChunk* chunk = new DownloadChunk;

					// Read from http response.
					dlsource->mSession->ReadResponse( chunk->mBuffer, DownloadChunk::_CHUNK_SIZE, &chunk->mSize );

					// Download finish.
					if ( chunk->mSize == 0 )
					{
						delete chunk;
						break;
					}

					filesize += chunk->mSize;

					// Put into downloaded chunk array.
					dltask->mChunkArray.Append( chunk );

					// Update progress.
					if ( totalsize != -1 )
					{
						_float progress = (_float) filesize / (_float) totalsize;
						if ( progress > 0.99f )
							progress = 0.99f;

						dltask->mProgress = progress;
					}
				}

				// Create data stream.
				dltask->mDataStream = resourcemanager->CreateMemStream( L"", filesize, fullchecksum );
				FL_ASSERT( dltask->mDataStream != _null )

				// Put all chunk into data stream.
				for ( _dword c = 0; c < dltask->mChunkArray.Number( ); c ++ )
				{
					DownloadChunk* chunk = dltask->mChunkArray[c];

					dltask->mDataStream->Write( chunk->mBuffer, chunk->mSize, offset );
					offset += chunk->mSize;

					delete chunk;
				}

				dltask->mChunkArray.Clear( _true );

				_bool validstream = _true;

				// Check stream with md5 checksum.
				if ( datachecksum != Oword( ) )
				{
					Oword checksum;
					Encryption::MD5Checksum( dltask->mDataStream->GetBuffer( ), dltask->mDataStream->GetSize( ), checksum );

					if ( datachecksum != checksum )
						validstream = _false;
				}

				// Create resource using data stream.
				if ( validstream )
				{
					resourcemanager->CreateResource( dltask->mFileURL, dltask->mDataStream );
					dltask->mProgress = 1.0f;
				}
				else
				{
					dltask->mProgress = 0.0f;

					PS_ERROR1( L"[DL] Bad checksum for file %s\r\n", (const _char*) dltask->mFileURL );

					downloaded = _false;
				}

				dltask->mDataStream->Release( );
				dltask->mDataStream = _null;
			}
			else
			{
				PS_ERROR1( L"[DL] Can not download file %s\r\n", (const _char*) dltask->mFileURL );
			}

			// Stop request.
			dlsource->mSession->StopRequest( );
		}

		// This download task not finished, we need try later.
		if ( downloaded == _false )
		{
			dlsource->mTaskLock.Enter( );
			dlsource->mTaskQueue.Append( dltask );
			dlsource->mTaskLock.Leave( );

			continue;
		}

		// This download task need to be delete now.
		if ( dltask->DecreaseRefCount( ) == 0 )
			delete dltask;
	}

	return _true;
}

HTTPDownloader::DownloadSource* HTTPDownloader::GetDownloadSource( )
{
	_dword bestindex = -1, taskinqueue = -1;

	// Find a source with less task in queue.
	for ( _dword i = 0; i < mDownloadSourceArray.Number( ); i ++ )
	{
		DownloadSource* dlsource = mDownloadSourceArray[i];

		_dword tasknumber = dlsource->mTaskQueue.Number( );

		if ( tasknumber < taskinqueue )
		{
			bestindex = i;
			taskinqueue = tasknumber;
		}
	}

	if ( bestindex == -1 )
		return _null;

	return mDownloadSourceArray[ bestindex ];
}

_void HTTPDownloader::Release( )
{
	delete this;
}

_void HTTPDownloader::SetConnectedServerCallback( OnConnectedServer funcpointer )
{
	mConnectedServerFunc = funcpointer;
}

_void HTTPDownloader::SetDownloadBeginCallback( OnDownloadBegin funcpointer )
{
	mDownloadBeginFunc = funcpointer;
}

_void HTTPDownloader::SetDownloadFinishedCallback( OnDownloadFinished funcpointer )
{
	mDownloadFinishedFunc = funcpointer;
}

_void HTTPDownloader::SetBackgroundDownloadBeginCallback( OnBackgroundDownloadBegin funcpointer )
{
	mBackgroundDownloadBeginFunc = funcpointer;
}

_void HTTPDownloader::SetBackgroundDownloadFinishedCallback( OnBackgroundDownloadFinished funcpointer )
{
	mBackgroundDownloadFinishedFunc = funcpointer;
}

_void HTTPDownloader::InsertSource( StringPtr sourceaddress )
{
	if ( mInternet == _null )
		return;

	DownloadSource* dlsource = _null;

	for ( _dword i = 0; i < mDownloadSourceArray.Number( ); i ++ )
	{
		dlsource = mDownloadSourceArray[i];

		// Found an existing download source with same HTTP host url.
		if ( dlsource->mSession->GetHostURL( ).CompareLowercase( StringFormatter::GetPathName( sourceaddress ) ) )
			break;
	}

	if ( dlsource == _null )
	{
		dlsource = new DownloadSource;
		dlsource->mSession = new HTTPSession( mInternet, sourceaddress );

		// Create download thread.
		dlsource->mTaskThread.RegisterUpdateFunc( DownloadThread, this, dlsource );
		dlsource->mTaskThread.Create( 10 );

		mDownloadSourceArray.Append( dlsource );
	}
}

_void HTTPDownloader::ClearSource( )
{
	for ( _dword i = 0; i < mDownloadSourceArray.Number( ); i ++ )
	{
		DownloadSource* dlsource = mDownloadSourceArray[i];
		if ( dlsource == _null )
			continue;

		dlsource->mTaskThread.Close( );
		delete dlsource;
	}

	mDownloadSourceArray.Clear( _true );
}

StringPtr HTTPDownloader::GetConnectedSource( ) const
{
	return L"";
}

_dword HTTPDownloader::DownloadFile( StringPtr filename, _float** progress )
{
	DownloadTask* newtask = new DownloadTask;

	_dword taskid = mDownloadTaskPool.Create( newtask );
	if ( taskid == -1 )
	{
		delete newtask;
		return 0;
	}

	newtask->mFileURL = filename;
	newtask->mFileURL.Lowercase( );

	_dword length = newtask->mFileURL.Length( );

	for ( _dword i = 0; i < length; i ++ )
	{
		if ( newtask->mFileURL[i] == '\\' )
			newtask->mFileURL.Replace( i, '/' );
	}

	newtask->mProgress = 0.0f;

	if ( progress != _null )
		*progress = &newtask->mProgress;

	IResourceManager* resourcemanager = GetResourceManagerPointer( );
	FL_ASSERT( resourcemanager != _null )

	// File already existing.
	if ( resourcemanager->SearchResource( newtask->mFileURL ) )
	{
		newtask->mProgress = 1.0f;
	}
	// Post request to download source.
	else
	{
		DownloadSource* dlsource = GetDownloadSource( );

		if ( dlsource != _null )
		{
			newtask->IncreaseRefCount( );

			// Put into download task queue.
			dlsource->mTaskLock.Enter( );
			dlsource->mTaskQueue.Append( newtask );
			dlsource->mTaskLock.Leave( );
		}
	}

	return taskid;
}

_dword HTTPDownloader::DownloadFile( StringPtr filename, const Oword& hashcode, _dword filesize, _float** progress )
{
	return DownloadFile( filename, progress );
}

_dword HTTPDownloader::BackgroundDownloadFile( StringPtr filename, _float** progress )
{
	return 0;
}

_dword HTTPDownloader::BackgroundDownloadFile( StringPtr filename, const Oword& hashcode, _dword filesize, _float** progress )
{
	return 0;
}

_bool HTTPDownloader::FinishFile( _dword taskid )
{
	if ( mDownloadTaskPool.Index( taskid ) == _null )
		return _false;

	DownloadTask* dltask = mDownloadTaskPool[taskid];
	if ( dltask == _null )
		return _false;

	mDownloadTaskPool.Remove( taskid );

	// This download task need to be delete now.
	if ( dltask->DecreaseRefCount( ) == 0 )
		delete dltask;

	return _true;
}

_bool HTTPDownloader::PauseFile( _dword taskid )
{
	return _false;
}

_bool HTTPDownloader::ResumeFile( _dword taskid )
{
	return _false;
}

_void HTTPDownloader::SetMaxDownloadSpeed( _dword bytespersecond )
{
}

_void HTTPDownloader::SetMaxUploadSpeed( _dword bytespersecond )
{
}

_void HTTPDownloader::SetDownloadFinish( )
{
}

_void HTTPDownloader::PauseDownload( )
{
}

_void HTTPDownloader::ResumeDownload( )
{
}

_bool HTTPDownloader::GetDownloadFileState( _dword taskid, DownloadFileState& downloadfilestate ) const
{
	return _false;
}

_bool HTTPDownloader::GetDownloadSourceState( _dword taskid, _dword sourceindex, DownloadSourceState& downloadsourcestate ) const
{
	return _false;
}

_dword HTTPDownloader::GetRecentDownloadBytes( _dword seconds ) const
{
	return 0;
}

_float HTTPDownloader::GetRecentDownloadSpeed( _dword seconds ) const
{
	return 0.0f;
}

_dword HTTPDownloader::GetRecentUploadBytes( _dword seconds ) const
{
	return 0;
}

_float HTTPDownloader::GetRecentUploadSpeed( _dword seconds ) const
{
	return 0.0f;
}

_dword HTTPDownloader::GetTotalDownloadBytes( ) const
{
	return 0;
}

_float HTTPDownloader::GetTotalDownloadSpeed( ) const
{
	return 0.0f;
}

_dword HTTPDownloader::GetTotalUploadBytes( ) const
{
	return 0;
}

_float HTTPDownloader::GetTotalUploadSpeed( ) const
{
	return 0.0f;
}

_dword HTTPDownloader::GetCurrentDownloadSourceNumber( ) const
{
	return 0;
}

_dword HTTPDownloader::GetCurrentUploadSourceNumber( ) const
{
	return 0;
}

_dword HTTPDownloader::GetDownloadingFileNumber( ) const
{
	return 0;
}

_void HTTPDownloader::SetCurrentDownloadingProduct( StringPtr mProduct )
{
}

StringPtr HTTPDownloader::GetCurrentDownloadingProduct( ) const
{
	return L"";
}

_dword HTTPDownloader::RequestGameMainManifestInfo( StringPtr product, StringPtr version )
{
	return 0;
}

_bool HTTPDownloader::GetGameMainManifestInfo( ManifestInfo& info, _bool clear )
{
	return _false;
}

_bool HTTPDownloader::RequestManifestInfo( StringPtr product, const Array< StringPtr >& versions )
{
	return _false;
}

_bool HTTPDownloader::GetManifestInfo( Array< ManifestInfo >& infos )
{
	return _false;
}

_void HTTPDownloader::SetDebugParam( StringPtr key, StringPtr value )
{
}
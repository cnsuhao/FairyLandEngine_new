//============================================================================
// HTTPDownloader.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// HTTPDownloader
//----------------------------------------------------------------------------

class HTTPDownloader : public IFileDownloader
{
private:
	struct DownloadChunk
	{
		enum { _CHUNK_SIZE = 4096 };

		_byte				mBuffer[ _CHUNK_SIZE ];
		_dword				mSize;

		DownloadChunk( ) : mSize( 0 ) { }
	};

	typedef Array< DownloadChunk* >	DownloadChunkArray;

	struct DownloadTask : public ReferencedObject
	{
		String				mFileURL;
		_float				mProgress;
		IDataStream*		mDataStream;
		DownloadChunkArray	mChunkArray;

		DownloadTask( ) : mProgress( 0.0f ), mDataStream( _null )
			{ IncreaseRefCount( ); }
	};

	typedef Pool< DownloadTask* >	DownloadTaskPool;
	typedef Queue< DownloadTask* >	DownloadTaskQueue;

	struct DownloadSource
	{
		HTTPSession*		mSession;
		Lock				mTaskLock;
		Thread				mTaskThread;
		DownloadTaskQueue	mTaskQueue;
	};

	typedef Array< DownloadSource* > DownloadSourceArray;

	OnConnectedServer				mConnectedServerFunc;
	OnDownloadBegin					mDownloadBeginFunc;
	OnDownloadFinished				mDownloadFinishedFunc;
	OnBackgroundDownloadBegin		mBackgroundDownloadBeginFunc;
	OnBackgroundDownloadFinished	mBackgroundDownloadFinishedFunc;

	_handle							mInternet;
	DownloadTaskPool				mDownloadTaskPool;
	DownloadSourceArray				mDownloadSourceArray;

	static _bool DownloadThread( _void* param1, _void* param2 );

	DownloadSource* GetDownloadSource( );

public:
	HTTPDownloader( );
	~HTTPDownloader( );

	virtual _void		Release( );

	virtual _void		SetConnectedServerCallback( OnConnectedServer funcpointer );
	virtual _void		SetDownloadBeginCallback( OnDownloadBegin funcpointer );
	virtual _void		SetDownloadFinishedCallback( OnDownloadFinished funcpointer );
	virtual _void		SetBackgroundDownloadBeginCallback( OnBackgroundDownloadBegin funcpointer );
	virtual _void		SetBackgroundDownloadFinishedCallback( OnBackgroundDownloadFinished funcpointer );

	virtual _void		InsertSource( StringPtr sourceaddress );
	virtual _void		ClearSource( );
	virtual StringPtr	GetConnectedSource( ) const;

	virtual _dword		DownloadFile( StringPtr filename, _float** progress );
	virtual _dword		DownloadFile( StringPtr filename, const Oword& hashcode, _dword filesize, _float** progress );
	virtual _dword		BackgroundDownloadFile( StringPtr filename, _float** progress );
	virtual _dword		BackgroundDownloadFile( StringPtr filename, const Oword& hashcode, _dword filesize, _float** progress );
	virtual _bool		FinishFile( _dword taskid );
	virtual _bool		PauseFile( _dword taskid );
	virtual _bool		ResumeFile( _dword taskid );

	virtual _void		SetMaxDownloadSpeed( _dword bytespersecond );
	virtual _void		SetMaxUploadSpeed( _dword bytespersecond );
	virtual _void		SetDownloadFinish( );

	virtual _void		PauseDownload( );
	virtual _void		ResumeDownload( );

	virtual _bool		GetDownloadFileState( _dword taskid, DownloadFileState& downloadfilestate ) const;
	virtual _bool		GetDownloadSourceState( _dword taskid, _dword sourceindex, DownloadSourceState& downloadsourcestate ) const;

	virtual _dword		GetRecentDownloadBytes( _dword seconds ) const;
	virtual _float		GetRecentDownloadSpeed( _dword seconds ) const;
	virtual _dword		GetRecentUploadBytes( _dword seconds ) const;
	virtual _float		GetRecentUploadSpeed( _dword seconds ) const;

	virtual _dword		GetTotalDownloadBytes( ) const;
	virtual _float		GetTotalDownloadSpeed( ) const;
	virtual _dword		GetTotalUploadBytes( ) const;
	virtual _float		GetTotalUploadSpeed( ) const;

	virtual _dword		GetCurrentDownloadSourceNumber( ) const;
	virtual _dword		GetCurrentUploadSourceNumber( ) const;
	virtual _dword		GetDownloadingFileNumber( ) const;

	virtual _void		SetCurrentDownloadingProduct( StringPtr mProduct );
	virtual StringPtr	GetCurrentDownloadingProduct( ) const;

	virtual _dword		RequestGameMainManifestInfo( StringPtr product, StringPtr version );
	virtual _bool		GetGameMainManifestInfo( ManifestInfo& info, _bool clear );
	virtual _bool		RequestManifestInfo( StringPtr product, const Array< StringPtr >& versions );
	virtual _bool		GetManifestInfo( Array< ManifestInfo >& infos );

	virtual _void		SetDebugParam( StringPtr key, StringPtr value );
};

};
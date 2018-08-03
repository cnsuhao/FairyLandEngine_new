//============================================================================
// ResourceDownloader.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// ResourceDownloader
//----------------------------------------------------------------------------

class ResourceDownloader : public IResourceDownloader
{
private:
	enum
	{
		BG_TASK_BYTES = (1 << 22),
		BG_TASK_COUNT = 8,
	};

	struct DownloadParameter
	{
		_dword		mTaskID;
		StringPtr	mSkipFile;
		_bool		mDeleteInvalidFile;
	};

	struct FileTask
	{
		String		mFileName;
		_dword		mFileTaskID;
		_dword		mFileSize;
		_float*		mProgress;

		operator StringPtr ( ) const
			{ return mFileName; }
	};

	typedef Array< _dword >					DwordArray;
	typedef Array< FileTask, StringPtr >	FileTaskArray;
	typedef Array< PreloadTask >			PreloadTaskArray;
	typedef Array< ProcessTask >			ProcessTaskArray;

	struct DownloadTask
	{
		_dword				mTaskID;
		_dword				mTaskPhase;
		_dword				mTaskFlag;
		String				mTaskName;
		FileTaskArray		mFileTaskArray;

		_float				mLastProgress;
		_dword				mLastTickcount;

		_dword				mLoadingIndex;
		_dword				mProcessMinTime;

		ProcessTaskArray	mProcessTaskArray;
		PreloadTaskArray	mPreloadTaskArray;
	};

	typedef Array< DownloadTask* > DownloadTaskArray;

	IFileDownloader*		mDownloader;
	Thread					mPreloadThread;

	_dword					mTaskIDCreator;
	_dword					mBackgroundTaskBytes;
	_dword					mBackgroundTaskCount;

	_bool					mPauseAllDownloadTask;
	_bool					mEnableAutoDownloadFile;

	DownloadTaskArray		mDownloadTaskArray;
	DwordArray				mDownloadedResourceArray;

	_dword					mAccumulatedElapse;

	static _bool OnPreloadThread( _void* parameter1, _void* parameter2 );

	static _bool OnCheckLocalFileCallback( StringPtr realname, StringPtr logicname, _dword size, const Oword& checksum, const Oword& fastchecksum, _dword flag, _bool needed, _void* parameter1, _void* parameter2 );
	static _bool OnDownloadNeededFileCallback( StringPtr realname, StringPtr logicname, _dword size, const Oword& checksum, const Oword& fastchecksum, _dword flag, _bool needed, _void* parameter1, _void* parameter2 );

	static _void OnBackgroundDownloadBeginCallback( StringPtr filename, const Oword& checksum, _dword filesize, _dword taskid );
	static _void OnBackgroundDownloadFinishedCallback( StringPtr filename, const Oword& checksum, _dword filesize, _dword taskid );

	_bool AutoDownloadFile( StringPtr filename, Oword& hashcode, _dword filesize );

public:
	ResourceDownloader( IFileDownloader* downloader );
	~ResourceDownloader( );

	virtual _void		Release( );

	virtual _void		Update( _dword elapse );

	virtual _void		InsertSource( StringPtr sourcename );
	virtual _void		ClearSource( );
	virtual StringPtr	GetConnectedSource( ) const;

	virtual _dword		CreateTaskGroup( StringPtr taskname, _dword phase = _PHASE_DOWNLOADING, _dword taskflag = 0 );
	virtual _bool		DeleteTaskGroup( _dword taskid );
	virtual _bool		PauseTaskGroup( _dword taskid );
	virtual _bool		ResumeTaskGroup( _dword taskid );
	virtual _bool		CreatePreloadTask( _dword taskid, const PreloadTask& preloadtask );
	virtual _bool		ClearPreloadTask( _dword taskid );
	virtual _bool		CreateProcessTask( _dword taskid, const ProcessTask& processtask );
	virtual _bool		ClearProcessTask( _dword taskid );
	virtual _bool		SetProcessMinTime( _dword taskid, _dword mintime );
	virtual _bool		DownloadFile( _dword taskid, StringPtr filename );
	virtual _bool		DownloadFile( _dword taskid, StringPtr filename, const Oword& hashcode, _dword filesize, _bool checklocalfile );
	virtual _void		ClearDownloadTask( );

	virtual _bool		IsAllDownloadTaskPaused( ) const;
	virtual _void		PauseAllDownloadTask( );
	virtual _void		ResumeAllDownloadTask( );

	virtual _bool		CheckLocalFile( );
	virtual _bool		DownloadNeededFile( _dword taskid, StringPtr skipfile, _bool deleteinvalidfile );

	virtual _void		EnableAutoDownloadFile( _bool enable );
	virtual _bool		IsAutoDownloadFileEnabled( ) const;

	virtual _void		SetMaxDownloadSpeed( _dword bytespersecond );
	virtual _void		SetMaxUploadSpeed( _dword bytespersecond );

	virtual _void		SetDownloadedResource( _dword resourceid );
	virtual _bool		IsResourceDownloaded( _dword resourceid ) const;

	virtual _dword		GetDownloadTaskNumber( ) const;
	virtual _dword		GetDownloadTaskIndex( _dword taskid ) const;

	virtual _bool		GetDownloadTaskState( _dword taskindex, DownloadTaskState& downloadtaskstate, _bool startpreload );
	virtual _bool		GetDownloadFileState( _dword taskindex, _dword fileindex, DownloadFileState& downloadfilestate );
	virtual _bool		GetDownloadSourceState( _dword taskindex, _dword fileindex, _dword sourceindex, DownloadSourceState& downloadsourcestate );

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

	virtual _dword		GetDownloadingTaskNumber( ) const;
	virtual _dword		GetDownloadingFileNumber( ) const;

    virtual _void		SetCurrentDownloadingProduct( StringPtr product );
	virtual _bool		RequestGameMainManifestInfo( StringPtr product, StringPtr version );
	virtual _bool		GetGameMainManifestInfo( ManifestInfo &info, _bool clear );

	virtual _bool		RequestGameMainManifestInfo( StringPtr product, StringPtr version0, StringPtr version1 );
	virtual _bool		GetGameMainManifestInfos( Array< ManifestInfo > &infos );

	virtual _void		SetDebugParam( StringPtr key, StringPtr value );
};

};
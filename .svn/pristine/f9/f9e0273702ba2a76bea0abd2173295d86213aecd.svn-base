//============================================================================
// IResourceDownloader.h
//
// Copyright 2006-2007 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// PreloadTask
//----------------------------------------------------------------------------

struct PreloadTask
{
	// Preload means load files in memory before parse them. Preload type just
	// point out which type files should be preload. Because 3D/graphics need 
	// better performance.
	enum
	{
		_PRELOAD_FILE				= 0,	// preload general files
		_PRELOAD_TEXTURE			= 1,	// preload texture files
		_PRELOAD_MESH				= 2,	// preload mesh files
		_PRELOAD_MESHANIMATION		= 3,	// preload mesh animation files
		_PRELOAD_SKELETON			= 4,	// preload skeleton files
		_PRELOAD_SKELETONANIMATION	= 5,	// preload skeleton animation files
		_PRELOAD_BONESHADOW			= 6,	// preload bone shadow files
		_PRELOAD_PARTICLESYSTEM		= 7,	// preload particle system files
	};

	_dword			mPreloadType;
	String			mPreloadMethod;			// the mothed to process the preloaded resource
	Array< String >	mSearchPathArray;
	Array< String >	mFileNameArray;

	PreloadTask( _dword type = 0 ) : mPreloadType( type ) { }
};

//----------------------------------------------------------------------------
// ProcessTask
//----------------------------------------------------------------------------

struct ProcessTask
{
	typedef _void (*OnProcessResource)( _void* parameter1, _void* parameter2 );

	_bool				mProcessAtMainThread;
	OnProcessResource	mProcessFunc;
	_void*				mProcessParameter1;
	_void*				mProcessParameter2;

	ProcessTask( ) : mProcessAtMainThread( _true ), mProcessFunc( _null ), mProcessParameter1( 0 ), mProcessParameter2( 0 ) { }
};

//----------------------------------------------------------------------------
// DownloadTaskState
//----------------------------------------------------------------------------

struct DownloadTaskState
{
	_dword		mTaskID;
	StringPtr	mTaskName;
	_dword		mTaskPhase;		// IResourceDownloader::_TASK_PHASE
	_dword		mFileNumber;
	_float		mPercentage;
};

//----------------------------------------------------------------------------
// IResourceDownloader
//----------------------------------------------------------------------------

class IResourceDownloader
{
public:
	enum _TASK_PRIORITY
	{
		_TASK_PRIORITY_NONE		= 0,
		_TASK_PRIORITY_LOW		= 1,
		_TASK_PRIORITY_NORMAL	= 2,
		_TASK_PRIORITY_HIGH		= 3,
	};

	enum _TASK_PHASE
	{
		_PHASE_VERIFYING		= 0,
		_PHASE_DOWNLOADING		= 1,
		_PHASE_PROCESSING		= 2,
		_PHASE_PRELOADING		= 3,
		_PHASE_LOADING			= 4,
		_PHASE_FINISHED			= 5,
	};

	enum _TASK_FLAG
	{
		_FLAG_AUTOCHECK			= 0x00000001,
		_FLAG_UNIQUEFLIE		= 0x00000002,
		_FLAG_PAUSED			= 0x00010000,
		_FLAG_DELETED			= 0x00020000,
		_FLAG_PRELOADING		= 0x00040000,

		_FLAG_MASK				= 0x0000FFFF,
	};

public:
	virtual _void		Release( )																													= 0;

	virtual _void		Update( _dword elapse )																										= 0;

	virtual _void		InsertSource( StringPtr sourcename )																						= 0;
	virtual _void		ClearSource( )																												= 0;
	virtual StringPtr	GetConnectedSource( ) const																									= 0;

	virtual _dword		CreateTaskGroup( StringPtr taskname, _dword phase = _PHASE_DOWNLOADING, _dword taskflag = 0 )								= 0;
	virtual _bool		DeleteTaskGroup( _dword taskid )																							= 0;
	virtual _bool		PauseTaskGroup( _dword taskid )																								= 0;
	virtual _bool		ResumeTaskGroup( _dword taskid )																							= 0;

	virtual _bool		CreatePreloadTask( _dword taskid, const PreloadTask& preloadtask )															= 0;
	virtual _bool		ClearPreloadTask( _dword taskid )																							= 0;
	virtual _bool		CreateProcessTask( _dword taskid, const ProcessTask& processtask )															= 0;
	virtual _bool		ClearProcessTask( _dword taskid )																							= 0;
	virtual _bool		SetProcessMinTime( _dword taskid, _dword mintime )																			= 0;
	virtual _bool		DownloadFile( _dword taskid, StringPtr filename )																			= 0;
	virtual _bool		DownloadFile( _dword taskid, StringPtr filename, const Oword& checksum, _dword filesize, _bool checklocalfile = _false )	= 0;
	virtual _void		ClearDownloadTask( )																										= 0;

	virtual _bool		IsAllDownloadTaskPaused( ) const																							= 0;
	virtual _void		PauseAllDownloadTask( )																										= 0;
	virtual _void		ResumeAllDownloadTask( )																									= 0;

	virtual _bool		CheckLocalFile( )																											= 0;
	virtual _bool		DownloadNeededFile( _dword taskid, StringPtr skipfile = L"", _bool deleteinvalidfile = _true )								= 0;

	virtual _void		EnableAutoDownloadFile( _bool enable )																						= 0;
	virtual _bool		IsAutoDownloadFileEnabled( ) const																							= 0;

	virtual _void		SetMaxDownloadSpeed( _dword bytespersecond )																				= 0;
	virtual _void		SetMaxUploadSpeed( _dword bytespersecond )																					= 0;

	virtual _void		SetDownloadedResource( _dword resourceid )																					= 0;
	virtual _bool		IsResourceDownloaded( _dword resourceid ) const																				= 0;

	virtual _dword		GetDownloadTaskNumber( ) const																								= 0;
	virtual _dword		GetDownloadTaskIndex( _dword taskid ) const																					= 0;

	virtual _bool		GetDownloadTaskState( _dword taskindex, DownloadTaskState& downloadtaskstate, _bool startpreload = _true )					= 0;
	virtual _bool		GetDownloadFileState( _dword taskindex, _dword fileindex, DownloadFileState& downloadfilestate )							= 0;
	virtual _bool		GetDownloadSourceState( _dword taskindex, _dword fileindex, _dword sourceindex, DownloadSourceState& downloadsourcestate )	= 0;

	virtual _dword		GetRecentDownloadBytes( _dword seconds ) const																				= 0;
	virtual _float		GetRecentDownloadSpeed( _dword seconds ) const																				= 0;
	virtual _dword		GetRecentUploadBytes( _dword seconds ) const																				= 0;
	virtual _float		GetRecentUploadSpeed( _dword seconds ) const																				= 0;

	virtual _dword		GetTotalDownloadBytes( ) const																								= 0;
	virtual _float		GetTotalDownloadSpeed( ) const																								= 0;
	virtual _dword		GetTotalUploadBytes( ) const																								= 0;
	virtual _float		GetTotalUploadSpeed( ) const																								= 0;

	virtual _dword		GetCurrentDownloadSourceNumber( ) const																						= 0;
	virtual _dword		GetCurrentUploadSourceNumber( ) const																						= 0;

	virtual _dword		GetDownloadingTaskNumber( ) const																							= 0;
	virtual _dword		GetDownloadingFileNumber( ) const																							= 0;

	virtual _void		SetCurrentDownloadingProduct( StringPtr product )																			= 0;
	virtual _bool		RequestGameMainManifestInfo( StringPtr product, StringPtr version )															= 0;
	virtual _bool		GetGameMainManifestInfo( ManifestInfo &info, _bool clear = _true )															= 0;

	virtual _bool		RequestGameMainManifestInfo( StringPtr product, StringPtr version0, StringPtr version1 )									= 0;
	virtual _bool		GetGameMainManifestInfos( Array< ManifestInfo > &infos )																	= 0;

	virtual _void		SetDebugParam( StringPtr key, StringPtr value )																				= 0;
};

};
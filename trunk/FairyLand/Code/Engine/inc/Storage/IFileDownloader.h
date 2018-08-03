//============================================================================
// IFileDownloader.h
//
// Copyright 2006-2007 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// DownloadFileState
//----------------------------------------------------------------------------

struct DownloadFileState
{
	StringPtr	mFileName;
	_dword		mFileSize;
	_float		mSpeed;
	_float		mPercentage;
	_dword		mSourceNumber;
};

//----------------------------------------------------------------------------
// DownloadSourceState
//----------------------------------------------------------------------------

struct DownloadSourceState
{
	String		mSourceName;
	_float		mSpeed;
	_dword		mBytesReceived;
};

struct ManifestInfo
{
	ManifestInfo(): mFileSize(0) {}

	Array<String, StringPtr> mVersionStrArray;
	String	mVersion;
	String	mFileName;
	String	mFileDate;
	Oword	mHashCode;
	_dword	mFileSize;
};

//----------------------------------------------------------------------------
// IFileDownloader
//----------------------------------------------------------------------------

class IFileDownloader
{
public:
	typedef _void (*OnConnectedServer)( StringPtr sourceaddress );

	typedef _void (*OnDownloadBegin)( StringPtr filename, const Oword& hashcode, _dword filesize, _dword taskid );
	typedef _void (*OnDownloadFinished)( StringPtr filename, const Oword& hashcode, _dword filesize, _dword taskid );

	typedef _void (*OnBackgroundDownloadBegin)( StringPtr filename, const Oword& hashcode, _dword filesize, _dword taskid );
	typedef _void (*OnBackgroundDownloadFinished)( StringPtr filename, const Oword& hashcode, _dword filesize, _dword taskid );

public:
	virtual _void		Release( )																										= 0;

	virtual _void		SetConnectedServerCallback( OnConnectedServer funcpointer )														= 0;
	virtual _void		SetDownloadBeginCallback( OnDownloadBegin funcpointer )															= 0;
	virtual _void		SetDownloadFinishedCallback( OnDownloadFinished funcpointer )													= 0;
	virtual _void		SetBackgroundDownloadBeginCallback( OnBackgroundDownloadBegin funcpointer )										= 0;
	virtual _void		SetBackgroundDownloadFinishedCallback( OnBackgroundDownloadFinished funcpointer )								= 0;

	virtual _void		InsertSource( StringPtr sourceaddress )																			= 0;
	virtual _void		ClearSource( )																									= 0;
	virtual StringPtr	GetConnectedSource( ) const																						= 0;

	virtual _dword		DownloadFile( StringPtr filename, _float** progress = _null )													= 0;
	virtual _dword		DownloadFile( StringPtr filename, const Oword& hashcode, _dword filesize, _float** progress = _null )			= 0;
	virtual _dword		BackgroundDownloadFile( StringPtr filename, _float** progress = _null )											= 0;
	virtual _dword		BackgroundDownloadFile( StringPtr filename, const Oword& hashcode, _dword filesize, _float** progress = _null )	= 0;
	virtual _bool		FinishFile( _dword taskid )																						= 0;
	virtual _bool		PauseFile( _dword taskid )																						= 0;
	virtual _bool		ResumeFile( _dword taskid )																						= 0;

	virtual _void		SetMaxDownloadSpeed( _dword bytespersecond )																	= 0;
	virtual _void		SetMaxUploadSpeed( _dword bytespersecond )																		= 0;
	virtual _void		SetDownloadFinish( )																							= 0;

	virtual _void		PauseDownload( )																								= 0;
	virtual _void		ResumeDownload( )																								= 0;

	virtual _bool		GetDownloadFileState( _dword taskid, DownloadFileState& downloadfilestate ) const								= 0;
	virtual _bool		GetDownloadSourceState( _dword taskid, _dword sourceindex, DownloadSourceState& downloadsourcestate ) const		= 0;

	virtual _dword		GetRecentDownloadBytes( _dword seconds ) const																	= 0;
	virtual _float		GetRecentDownloadSpeed( _dword seconds ) const																	= 0;
	virtual _dword		GetRecentUploadBytes( _dword seconds ) const																	= 0;
	virtual _float		GetRecentUploadSpeed( _dword seconds ) const																	= 0;

	virtual _dword		GetTotalDownloadBytes( ) const																					= 0;
	virtual _float		GetTotalDownloadSpeed( ) const																					= 0;
	virtual _dword		GetTotalUploadBytes( ) const																					= 0;
	virtual _float		GetTotalUploadSpeed( ) const																					= 0;

	virtual _dword		GetCurrentDownloadSourceNumber( ) const																			= 0;
	virtual _dword		GetCurrentUploadSourceNumber( ) const																			= 0;

	virtual _dword		GetDownloadingFileNumber( ) const																				= 0;

	virtual _void		SetCurrentDownloadingProduct( StringPtr product )																= 0;
	virtual StringPtr	GetCurrentDownloadingProduct( ) const																			= 0;

	virtual _dword		RequestGameMainManifestInfo( StringPtr product, StringPtr version )												= 0;
	virtual _bool		GetGameMainManifestInfo( ManifestInfo &info, _bool clear )														= 0;
	virtual _bool		RequestManifestInfo( StringPtr product, const Array< StringPtr > &versions )									= 0;
	virtual _bool		GetManifestInfo( Array< ManifestInfo > &infos )																	= 0;

	virtual _void		SetDebugParam( StringPtr key, StringPtr value )																	= 0;
};

};
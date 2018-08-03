//============================================================================
// ResourceReplacer.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// ResourceReplacer
//----------------------------------------------------------------------------

class ResourceReplacer : public IResourceReplacer
{
private:
	struct ReplacedObject
	{
		String		mName;
		_dword		mTaskID;
		_void*		mObject;

		operator StringPtr( ) const
			{ return mName; }
	};

	typedef Hash< ReplacedObject, StringPtr > ReplaceHash;

	ReplaceHash		mReplaceHash;
	_dword			mAccumulatedElapse;

	ITexture* CreateTextureReplacer( );

public:
	ResourceReplacer( );
	~ResourceReplacer( );

	virtual _void	Release( );

	virtual _void	Update( _dword elapse );

	virtual _void*	GetTextureReplacer( StringPtr resname );
};

};
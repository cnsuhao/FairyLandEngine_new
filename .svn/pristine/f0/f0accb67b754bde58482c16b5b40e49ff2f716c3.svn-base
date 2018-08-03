//============================================================================
// GraphicsManager.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// GraphicsManager
//----------------------------------------------------------------------------

class GraphicsManager : public IGraphicsManager
{
private:
	_dword				mGdiToken;

	_bool				mEnableAsyncLoader;

	RenderAPI*			mRenderAPI;
	RenderDevice*		mRenderDevice;

	GeometryFactory*	mGeometryFactory;
	TextureFactory*		mTextureFactory;

	TechniqueFactory*	mTechniqueFactory;

	ModelFactory*		mModelFactory;
	ParticleFactory*	mParticleFactory;
	PhysicsFactory*		mPhysicsFactory;

	SceneFactory*		mSceneFactory;

	RenderConfig*		mRenderConfig;

	RenderStatistics*	mRenderStatistics;

	RenderQueue*		mRenderQueue;

public:
	GraphicsManager( );
	~GraphicsManager( );

	inline static RenderAPI& GetRenderAPIHelper( )
		{ return *( (GraphicsManager&) GetGraphicsManager( ) ).mRenderAPI; }

	inline static RenderQueue& GetRenderQueueHelper( )
		{ return *( (GraphicsManager&) GetGraphicsManager( ) ).mRenderQueue; }

	inline static RenderSet& GetRenderSetHelper( )
		{ return *( ( (GraphicsManager&) GetGraphicsManager( ) ).mRenderQueue->GetCurrentRenderSet( ) ); }

	virtual _void	Release( );
	virtual _void	Update( _dword elapse );

	virtual _void	EnableAsyncLoader( _bool enable );
	virtual _bool	IsAsyncLoaderEnabled( ) const;

	virtual _bool	CreateRenderDevice( _handle window, _dword devicetype );
};

};
//============================================================================
// GraphicsManager.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandGraphics.h"

//----------------------------------------------------------------------------
// GraphicsManager Implementation
//----------------------------------------------------------------------------

GraphicsManager::GraphicsManager( )
{
	mGdiToken			= 0;

	mEnableAsyncLoader	= _false;

	mRenderAPI			= _null;
	mRenderDevice		= _null;
	mGeometryFactory	= _null;
	mTextureFactory		= _null;
	mTechniqueFactory	= _null;
	mModelFactory		= _null;
	mParticleFactory	= _null;
	mPhysicsFactory		= _null;
	mSceneFactory		= _null;
	mRenderConfig		= _null;
	mRenderStatistics	= _null;
	mRenderQueue		= _null;

	Gdiplus::GdiplusStartupInput gdistartup;
	Gdiplus::GdiplusStartup( &mGdiToken, &gdistartup, _null );
}

GraphicsManager::~GraphicsManager( )
{
	delete mRenderQueue;
	delete mRenderStatistics;
	delete mRenderConfig;
	delete mSceneFactory;
	delete mModelFactory;
	delete mParticleFactory;
	delete mPhysicsFactory;
	delete mTechniqueFactory;
	delete mGeometryFactory;
	delete mTextureFactory;
	delete mRenderDevice;
	delete mRenderAPI;

	GetFairyLandGlobal( ).mGraphicsManager		= _null;
	GetFairyLandGlobal( ).mRenderDevice		= _null;
	GetFairyLandGlobal( ).mGeometryFactory		= _null;
	GetFairyLandGlobal( ).mTextureFactory		= _null;
	GetFairyLandGlobal( ).mTechniqueFactory	= _null;
	GetFairyLandGlobal( ).mModelFactory		= _null;
	GetFairyLandGlobal( ).mParticleFactory		= _null;
	GetFairyLandGlobal( ).mPhysicsFactory		= _null;
	GetFairyLandGlobal( ).mSceneFactory		= _null;
	GetFairyLandGlobal( ).mRenderConfig		= _null;
	GetFairyLandGlobal( ).mRenderStatistics	= _null;

	Gdiplus::GdiplusShutdown( mGdiToken );
}

_void GraphicsManager::Release( )
{
	delete this;
}

_void GraphicsManager::Update( _dword elapse )
{
}

_void GraphicsManager::EnableAsyncLoader( _bool enable )
{
	mEnableAsyncLoader = enable;
}

_bool GraphicsManager::IsAsyncLoaderEnabled( ) const
{
	return mEnableAsyncLoader;
}

_bool GraphicsManager::CreateRenderDevice( _handle window, _dword devicetype )
{
	if ( mRenderDevice != _null )
		return _true;

	if ( devicetype & IRenderDevice::_DEVICE_OPENGL )
	{
		#ifdef _OPENGL_RENDER

		RenderAPIOpenGL* renderapi = new RenderAPIOpenGL( );
		if ( renderapi->Initialize( window, devicetype ) == _false )
		{
			delete renderapi;
			return _false;
		}

		mRenderAPI = renderapi;

		#endif
	}
	else
	{
		#ifdef _DIRECTX9_RENDER

		RenderAPID3D9* renderapi = new RenderAPID3D9( );
		if ( renderapi->Initialize( window, devicetype ) == _false )
		{
			delete renderapi;
			return _false;
		}

		mRenderAPI = renderapi;

		#endif
	}

	if ( mRenderAPI == _null )
		return _false;

	mRenderDevice		= new RenderDevice( );
	mGeometryFactory	= new GeometryFactory( );
	mTextureFactory		= new TextureFactory( );
	mTechniqueFactory	= new TechniqueFactory( mRenderAPI->GetVertexShaderVersion( ), mRenderAPI->GetPixelShaderVersion( ) );
	mModelFactory		= new ModelFactory( );
	mParticleFactory	= new ParticleFactory( );
	mPhysicsFactory		= new PhysicsFactory( );
	mSceneFactory		= new SceneFactory( );
	mRenderConfig		= new RenderConfig( );
	mRenderStatistics	= new RenderStatistics( );
	mRenderQueue		= new RenderQueue( );

	mTextureFactory->CreateDefaultTexture( );

	Viewport viewport;
	mRenderAPI->GetViewport( viewport );
	mRenderDevice->SetViewport( viewport );

	GetFairyLandGlobal( ).mRenderDevice		= mRenderDevice;
	GetFairyLandGlobal( ).mGeometryFactory		= mGeometryFactory;
	GetFairyLandGlobal( ).mTextureFactory		= mTextureFactory;
	GetFairyLandGlobal( ).mTechniqueFactory	= mTechniqueFactory;
	GetFairyLandGlobal( ).mModelFactory		= mModelFactory;
	GetFairyLandGlobal( ).mParticleFactory		= mParticleFactory;
	GetFairyLandGlobal( ).mPhysicsFactory		= mPhysicsFactory;
	GetFairyLandGlobal( ).mSceneFactory		= mSceneFactory;
	GetFairyLandGlobal( ).mRenderConfig		= mRenderConfig;
	GetFairyLandGlobal( ).mRenderStatistics	= mRenderStatistics;

	return _true;
}
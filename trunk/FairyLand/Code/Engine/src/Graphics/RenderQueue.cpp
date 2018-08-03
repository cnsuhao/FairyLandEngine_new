//============================================================================
// RenderQueue.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandGraphics.h"

//----------------------------------------------------------------------------
// RenderQueue Implementation
//----------------------------------------------------------------------------

RenderQueue::RenderQueue( )
{
	mDefaultRenderSet		= new RenderSet( );
	mTargetRenderSet		= new RenderSet( );
	mCurrentRenderSet		= mDefaultRenderSet;

	mDefaultRenderLayer		= new RenderLayer( _null );
	mCurrentRenderLayer		= mDefaultRenderLayer;

	mRenderSetArray.Append( mDefaultRenderSet );
	mRenderSetArray.Append( mTargetRenderSet );

	mLastVertexDeclaration		= _null;
	mLastVertexBuffer			= _null;
	mLastIndexBuffer			= _null;

	for ( _dword i = 0; i < Geometry::_MAX_TEXTURE_NUMBER; i ++ )
		mLastTexture[i] = _null;

	mLastWorldTransform			= _null;
	mLastViewTransform			= _null;
	mLastProjectionTransform	= _null;

	for ( _dword i = 0; i < Geometry::_MAX_TEXTURE_NUMBER; i ++ )
		mLastTextureTransform[i] = _null;

	mLastViewport = _null;

	mLastCullMode = IRenderMethod::_CULLMODE_CCW;
}

RenderQueue::~RenderQueue( )
{
	delete mDefaultRenderSet;
	delete mTargetRenderSet;

	delete mDefaultRenderLayer;
}

_void RenderQueue::ClearQueue( )
{
	mLastVertexDeclaration	= _null;
	mLastVertexBuffer		= _null;
	mLastIndexBuffer		= _null;

	for ( _dword i = 0; i < Geometry::_MAX_TEXTURE_NUMBER; i ++ )
		mLastTexture[i] = _null;

	mLastWorldTransform			= _null;
	mLastViewTransform			= _null;
	mLastProjectionTransform	= _null;

	for ( _dword i = 0; i < Geometry::_MAX_TEXTURE_NUMBER; i ++ )
		mLastTextureTransform[i] = _null;

	mLastViewport	= _null;

	mLastGeometry	= Geometry( );
	mLastTechnique	= TechniqueUnit( );
	mLastCullMode	= IRenderMethod::_CULLMODE_CCW;
}

_void RenderQueue::PrepareRender( ISurface* rendertarget, ISurface* depthstencil )
{
	RenderAPI& renderapi = GraphicsManager::GetRenderAPIHelper( );

	// Set render target and depth stencil buffer.
	if ( rendertarget != _null )
		renderapi.SetRenderTargetSurface( ( (const Surface*) rendertarget )->GetSurfaceObject( ) );
	if ( depthstencil != _null )
		renderapi.SetDepthStencilSurface( ( (const Surface*) depthstencil )->GetSurfaceObject( ) );

	// Prepare to render.
	for ( _dword i = 0; i < mRenderSetArray.Number( ); i ++ )
	{
		RenderSet* renderset = mRenderSetArray[i];

		// Current render target is not match this render set, skip it.
		if ( renderset->CheckRenderTarget( rendertarget, depthstencil ) == _false )
			continue;

		RenderUnitArray& renderunitarray = renderset->GetRenderUnitArray( );

		// Put render unit from this render set into queue.
		if ( renderunitarray.Number( ) == 0 )
			continue;

		if ( renderset == mDefaultRenderSet || renderset == mTargetRenderSet )
			renderset->Prepare( );

		for ( _dword j = 0; j < renderunitarray.Number( ); j ++ )
		{
			RenderUnit& renderunit = renderunitarray[j];

			_dword layerflag = renderunit.mLayer & 0xFFFF0000;

			// Finish sub layer.
			if ( layerflag == 0x00020000 )
			{
				mCurrentRenderLayer = mCurrentRenderLayer->ParentLayer( );
				FL_ASSERT( mCurrentRenderLayer != _null )

				continue;
			}

			// Switch to sub layer.
			if ( layerflag == 0x00010000 )
				renderunit.mSubLayer = new RenderLayer( mCurrentRenderLayer );

			mCurrentRenderLayer->AppendRenderUnit( renderunit );

			// Reset layer informat in render set.
			if ( layerflag == 0x00010000 )
			{
				mCurrentRenderLayer = (RenderLayer*) renderunit.mSubLayer;
				renderunit.mSubLayer = _null;
			}
		}
	}
}

_void RenderQueue::FinishRender( ISurface* rendertarget, ISurface* depthstencil )
{
	RenderAPI& renderapi = GraphicsManager::GetRenderAPIHelper( );

	mLastTechnique.Disable( );

	// Reset render target and depth stencil buffer.
	if ( rendertarget != _null )
		renderapi.SetRenderTargetSurface( renderapi.GetDefaultRenderTargetSurface( ) );
	if ( depthstencil != _null )
		renderapi.SetDepthStencilSurface( renderapi.GetDefaultDepthStencilSurface( ) );

	// Reset texture factor.
	if ( mLastGeometry.mTextureFactor != 0xFFFFFFFF )
		renderapi.EnableRenderState( D3DRS_TEXTUREFACTOR, 0xFFFFFFFF );

	// Reset cull mode.
	if ( mLastCullMode != IRenderMethod::_CULLMODE_CCW )
		renderapi.EnableRenderState( D3DRS_CULLMODE, IRenderMethod::_CULLMODE_CCW );

	// Reset depth bias.
	if ( mLastGeometry.mDepthBias != 0.0f )
	{
		_float depthbias = 0.0f;
		renderapi.EnableRenderState( D3DRS_DEPTHBIAS, *( (_dword*) &depthbias ) );
	}

	// Reset to defauult render set.
	mCurrentRenderSet = mDefaultRenderSet;

	// Clear render set of this frame, and put default render set into.
	for ( _dword i = 0; i < mRenderSetArray.Number( ); i ++ )
	{
		RenderSet* renderset = mRenderSetArray[i];

		// Current render target is not match this render set, skip it.
		if ( renderset->CheckRenderTarget( rendertarget, depthstencil ) == _false )
			continue;

		if ( renderset == mDefaultRenderSet || renderset == mTargetRenderSet )
		{
			// Finish default render set.
			renderset->Finish( );
			continue;
		}
	}

	// Clear render layer of this frame.
	mDefaultRenderLayer->Clear( );
	mCurrentRenderLayer = mDefaultRenderLayer;
}

_void RenderQueue::DoRender( RenderLayer* layer )
{
	layer->SortRenderUnit( GetRenderDevice( ).GetViewFrustum( ) );

	RenderArray( layer->GetBackgroundUnitArray( ) );
	RenderArray( layer->GetEnvironmentUnitArray( ) );
	RenderArray( layer->GetNormalUnitArray( ) );
	RenderArray( layer->GetForegroundUnitArray( ) );
	RenderArray( layer->GetOverlayUnitArray( ) );
}

_void RenderQueue::RenderArray( RenderUnitArray& renderunitarray )
{
	//_bool prewritedepth = _true;

	// Render renderables.
	for ( _dword i = 0; i < renderunitarray.Number( ); i ++ )
	{
		RenderUnit& renderunit = renderunitarray[i];

		// Get the geometry to be render.
		const Geometry* geometry = renderunit.mGeometry;

		if ( geometry != _null && geometry->mVertexDeclaration != _null && geometry->mVertexBuffer != _null )
			RenderGeometry( *geometry, renderunit );

		if ( renderunit.mSubLayer != _null )
		{
			DoRender( (RenderLayer*) renderunit.mSubLayer );
			delete renderunit.mSubLayer;
		}
	}
}

_void RenderQueue::RenderGeometry( const Geometry& geometry, const RenderUnit& renderunit )
{
	if ( geometry.mVertexDeclaration == _null || geometry.mVertexBuffer == _null )
		return;

	RenderAPI& renderapi = GraphicsManager::GetRenderAPIHelper( );

	// Set vertex declaration.
	if ( mLastVertexDeclaration != geometry.mVertexDeclaration )
	{
		( (VertexDeclaration*) geometry.mVertexDeclaration )->EnableDeclaration( );

		mLastVertexDeclaration = geometry.mVertexDeclaration;
	}

	// Set vertex buffer.
	if ( mLastVertexBuffer != geometry.mVertexBuffer )
	{
		_void* buffer = geometry.mVertexBuffer == _null ? _null :
			( (VertexBuffer*) geometry.mVertexBuffer )->GetBufferObject( );

		renderapi.SetVertexBuffer( buffer, geometry.mVertexSize );

		mLastVertexBuffer = geometry.mVertexBuffer;
	}

	// Set index buffer.
	if ( mLastIndexBuffer != geometry.mIndexBuffer )
	{
		_void* buffer = geometry.mIndexBuffer == _null ? _null :
			( (IndexBuffer*) geometry.mIndexBuffer )->GetBufferObject( );

		_dword indexsize = geometry.mIndexBuffer == _null ? 0 :
			geometry.mIndexBuffer->GetIndexSize( );

		renderapi.SetIndexBuffer( buffer, indexsize );

		mLastIndexBuffer = geometry.mIndexBuffer;
	}

	// Set world transform.
	if ( mLastWorldTransform != renderunit.mWorldTransform )
	{
		renderapi.SetWorldTransform( *renderunit.mWorldTransform );

		mLastWorldTransform = renderunit.mWorldTransform;
	}

	// Set view transform.
	if ( mLastViewTransform != renderunit.mViewTransform )
	{
		renderapi.SetViewTransform( *renderunit.mViewTransform );

		mLastViewTransform = renderunit.mViewTransform;
	}

	// Set projection transform.
	if ( mLastProjectionTransform != renderunit.mProjectionTransform )
	{
		renderapi.SetProjectionTransform( *renderunit.mProjectionTransform );

		mLastProjectionTransform = renderunit.mProjectionTransform;
	}

	// Set texture transform.
	for ( _dword i = 0; i < Geometry::_MAX_TEXTURE_NUMBER; i ++ )
	{
		if ( mLastTextureTransform[i] == renderunit.mTextureTransform[i] )
			continue;

		renderapi.SetTextureTransform( i, *renderunit.mTextureTransform[i] );

		mLastTextureTransform[i] = renderunit.mTextureTransform[i];
	}

	if ( mLastViewport != renderunit.mViewport )
	{
		if ( renderunit.mViewport != _null )
			renderapi.SetViewport( *renderunit.mViewport );

		mLastViewport = renderunit.mViewport;
	}

	// Enable render method by change last method, it will be faster.
	renderunit.mTechnique.Enable( mLastTechnique );

	// Set texture factor.
	if ( geometry.mTextureFactor != mLastGeometry.mTextureFactor )
		renderapi.EnableRenderState( D3DRS_TEXTUREFACTOR, geometry.mTextureFactor );

	// Set material.
	if ( renderunit.mTechnique.IsLightingEnabled( ) )
	{
		renderapi.SetMaterial( geometry.mMaterialDiffuse, geometry.mMaterialAmbient,
			geometry.mMaterialSpecular, geometry.mMaterialEmissive, geometry.mMaterialPower );
	}

	// Set cull mode.
	_dword cullmode = renderunit.mTechnique.GetCullMode( geometry.mMirror );

	if ( cullmode != mLastCullMode )
		renderapi.EnableRenderState( D3DRS_CULLMODE, cullmode );

	// Set depth bias.
	if ( geometry.mDepthBias != mLastGeometry.mDepthBias )
	{
		_float depthbias = -geometry.mDepthBias;
		renderapi.EnableRenderState( D3DRS_DEPTHBIAS, *( (_dword*) &depthbias ) );
	}

	// Set texture.
	for ( _dword t = 0; t < Geometry::_MAX_TEXTURE_NUMBER; t ++ )
	{
		if ( geometry.mTexture[t] == _null )
		{
			if ( t == 0 )
				renderapi.SetTexture( 0, _null );

			break;
		}

		renderapi.SetTexture( t, ( (Texture*) geometry.mTexture[t] )->GetTextureObject( ) );
	}

	// Render the geometry without index.
	if ( geometry.mIndexBuffer == _null )
	{
		renderapi.RenderPrimitive( geometry.mGeometryType, geometry.mStartVertex, geometry.mGeometryNumber );
	}
	// Render the geometry with index.
	else
	{
		renderapi.RenderPrimitive( geometry.mGeometryType, geometry.mStartVertex, geometry.mStartIndex, geometry.mVertexNumber, geometry.mGeometryNumber );
	}

	mLastGeometry	= geometry;
	mLastTechnique	= renderunit.mTechnique;
	mLastCullMode	= cullmode;
}

_void RenderQueue::SwitchRenderSet( )
{
	ISurface* rendertarget = GetRenderDevice( ).GetRenderTargetSurface( );
	ISurface* depthstencil = GetRenderDevice( ).GetDepthStencilSurface( );

	if ( mCurrentRenderSet->CheckRenderTarget( rendertarget, depthstencil ) )
		return;

	//if ( mCurrentRenderSet == mTargetRenderSet )
	//	mTargetRenderSet->Finish( );

	if ( rendertarget == _null && depthstencil == _null )
	{
		mCurrentRenderSet = mDefaultRenderSet;
	}
	else
	{
		mCurrentRenderSet = mTargetRenderSet;
		mCurrentRenderSet->SetRenderTarget( rendertarget, depthstencil );
	}
}

_void RenderQueue::UseDefaultRenderSet( )
{
}

_void RenderQueue::SetCurrentRenderSet( RenderSet* renderset )
{
}

_void RenderQueue::AppendRenderSet( RenderSet* renderset )
{
}

_void RenderQueue::Present( ISurface* rendertarget, ISurface* depthstencil )
{
	PrepareRender( rendertarget, depthstencil );

	GraphicsManager::GetRenderAPIHelper( ).BeginScene( );
	DoRender( mDefaultRenderLayer );
	GraphicsManager::GetRenderAPIHelper( ).EndScene( );

	FinishRender( rendertarget, depthstencil );

	ClearQueue( );
}
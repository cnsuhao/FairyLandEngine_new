//============================================================================
// RenderQueue.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

class RenderDevice;

//----------------------------------------------------------------------------
// RenderQueue
//----------------------------------------------------------------------------

class RenderQueue
{
private:
	typedef Array< RenderSet* >		RenderSetArray;
	typedef Array< RenderLayer* >	RenderLayerArray;
	typedef Array< _dword >			IndexArray;

	RenderSet*				mDefaultRenderSet;
	RenderSet*				mTargetRenderSet;
	RenderSet*				mCurrentRenderSet;

	RenderLayer*			mDefaultRenderLayer;
	RenderLayer*			mCurrentRenderLayer;

	RenderSetArray			mRenderSetArray;

	IVertexDeclaration*		mLastVertexDeclaration;
	IVertexBuffer*			mLastVertexBuffer;
	IIndexBuffer*			mLastIndexBuffer;
	ITexture*				mLastTexture[ Geometry::_MAX_TEXTURE_NUMBER ];

	const Matrix4*			mLastWorldTransform;
	const Matrix4*			mLastViewTransform;
	const Matrix4*			mLastProjectionTransform;
	const Matrix4*			mLastTextureTransform[ Geometry::_MAX_TEXTURE_NUMBER ];
	const Viewport*			mLastViewport;

	Geometry				mLastGeometry;

	TechniqueUnit			mLastTechnique;

	_dword					mLastCullMode;

	_void ClearQueue( );
	_void PrepareRender( ISurface* rendertarget, ISurface* depthstencil );
	_void FinishRender( ISurface* rendertarget, ISurface* depthstencil );
	_void DoRender( RenderLayer* layer );

	_void RenderArray( RenderUnitArray& renderunitarray );
	_void RenderGeometry( const Geometry& geometry, const RenderUnit& renderunit );

public:
	RenderQueue( );
	~RenderQueue( );

	inline RenderSet* GetCurrentRenderSet( )
		{ return mCurrentRenderSet; }

	_void SwitchRenderSet( );
	_void UseDefaultRenderSet( );
	_void SetCurrentRenderSet( RenderSet* renderset );
	_void AppendRenderSet( RenderSet* renderset );

	_void Present( ISurface* rendertarget, ISurface* depthstencil );
};

};
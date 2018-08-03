//============================================================================
// RenderDevice.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandGraphics.h"

//----------------------------------------------------------------------------
// RenderDevice Implementation
//----------------------------------------------------------------------------

RenderDevice::RenderDevice( )
{
	mRenderTargetTexture	= _null;
	mRenderTargetSurface	= _null;

	mDepthStencilTexture	= _null;
	mDepthStencilSurface	= _null;

	mNeedOverlayClip		= _false;
	mOverlayClipRegion		= Region::cInitRegion;

	mNeedOverlayTransform	= _false;
	mOverlayTransform		= Matrix3::cIdentity;

	mNeedWorldTransform		= _false;
	mWorldTransform			= Matrix4::cIdentity;
	mNormalTransform		= Matrix3::cIdentity;
	mScaleTransform			= 1.0f;

	mViewTransform			= Matrix4::cIdentity;
	mProjectionTransform	= Matrix4::cIdentity;

	for ( _dword i = 0; i < Geometry::_MAX_TEXTURE_NUMBER; i ++ )
		mTextureTransform[i] = Matrix4::cIdentity;

	mViewMatrix				= Matrix4::cIdentity;
	mViewFrustum			= Frustum( Matrix4::cIdentity, Matrix4::cIdentity );
}

RenderDevice::~RenderDevice( )
{
	GetFairyLandGlobal( ).mRenderDevice = _null;
}

_bool RenderDevice::IsDeviceLost( )
{
	return GraphicsManager::GetRenderAPIHelper( ).IsDeviceLost( );
}

_bool RenderDevice::ResizeDevice( _dword width, _dword height )
{
	if ( width == GraphicsManager::GetRenderAPIHelper( ).GetDeviceWidth( ) &&
		 height == GraphicsManager::GetRenderAPIHelper( ).GetDeviceHeight( ) )
	{
		return _true;
	}

	if ( GraphicsManager::GetRenderAPIHelper( ).ResizeDevice( width, height ) == _false )
		return _false;

	Viewport viewport;
	GraphicsManager::GetRenderAPIHelper( ).GetViewport( viewport );

	SetViewport( viewport );

	return _true;
}

_bool RenderDevice::ResetDevice( )
{
	GetTextureFactory( ).ReleaseLostResource( );

	if ( GraphicsManager::GetRenderAPIHelper( ).ResetDevice( ) == _false )
		return _false;

	GetTextureFactory( ).RecreateLostResource( );
	return _true;
}

_handle RenderDevice::GetDeviceHandle( ) const
{
	return GraphicsManager::GetRenderAPIHelper( ).GetDeviceHandle( );
}

_dword RenderDevice::GetDeviceWidth( ) const
{
	return GraphicsManager::GetRenderAPIHelper( ).GetDeviceWidth( );
}

_dword RenderDevice::GetDeviceHeight( ) const
{
	return GraphicsManager::GetRenderAPIHelper( ).GetDeviceHeight( );
}

_void RenderDevice::SetClearColor( _dword color )
{
	GraphicsManager::GetRenderAPIHelper( ).SetClearColor( color );
}

_void RenderDevice::SetClearDepth( _float depth )
{
	GraphicsManager::GetRenderAPIHelper( ).SetClearDepth( depth );
}

_void RenderDevice::SetClearStencil( _dword stencil )
{
	GraphicsManager::GetRenderAPIHelper( ).SetClearStencil( stencil );
}

_void RenderDevice::ClearBuffer( _bool color, _bool depth, _bool stencil )
{
	RenderAPI& renderapi = GraphicsManager::GetRenderAPIHelper( );

	Viewport vp;
	renderapi.GetViewport( vp );

	if ( vp != mViewport )
		renderapi.SetViewport( mViewport );

	if ( mRenderTargetSurface != _null )
		renderapi.SetRenderTargetSurface( ( (const Surface*) mRenderTargetSurface )->GetSurfaceObject( ) );

	if ( mDepthStencilSurface != _null )
		renderapi.SetDepthStencilSurface( ( (const Surface*) mDepthStencilSurface )->GetSurfaceObject( ) );

	renderapi.ClearBuffer( color, depth, stencil );

	if ( mRenderTargetSurface != _null )
		renderapi.SetRenderTargetSurface( renderapi.GetDefaultRenderTargetSurface( ) );

	if ( mDepthStencilSurface != _null )
		renderapi.SetDepthStencilSurface( renderapi.GetDefaultDepthStencilSurface( ) );

	if ( vp != mViewport )
		renderapi.SetViewport( vp );
}

_void RenderDevice::Flush( )
{
	GraphicsManager::GetRenderQueueHelper( ).Present( mRenderTargetSurface, mDepthStencilSurface );
}

_void RenderDevice::Present( _handle targetwindow )
{
	if ( mRenderTargetTexture != _null || mDepthStencilTexture != _null )
		ResetRenderTarget( );

	( (RenderStatistics&) GetRenderStatistics( ) ).BeforePresent( );
	GraphicsManager::GetRenderQueueHelper( ).Present( _null, _null );
	( (RenderStatistics&) GetRenderStatistics( ) ).AfterPresent( );

	GraphicsManager::GetRenderAPIHelper( ).Present( targetwindow );
}

_void RenderDevice::CombineOverlayClipRegion( const Region& clipregion )
{
	if ( mNeedOverlayClip && clipregion != Region::cInitRegion )
		mOverlayClipRegion = Region::Intersect( mOverlayClipRegion, clipregion );
	else
		mOverlayClipRegion = clipregion;

	mNeedOverlayClip = mOverlayClipRegion != Region::cInitRegion;
}

_void RenderDevice::SetOverlayClipRegion( const Region& clipregion )
{
	mOverlayClipRegion = clipregion;

	mNeedOverlayClip = mOverlayClipRegion != Region::cInitRegion;
}

const Region& RenderDevice::GetOverlayClipRegion( ) const
{
	return mOverlayClipRegion;
}

_void RenderDevice::SetOverlayTransform( const Matrix3& matrix )
{
	mOverlayTransform		= matrix;
	mNeedOverlayTransform	= mOverlayTransform != Matrix3::cIdentity;
}

const Matrix3& RenderDevice::GetOverlayTransform( ) const
{
	return mOverlayTransform;
}

_void RenderDevice::PushOverlayTransform( )
{
	mOverlayTransformStack.Push( mOverlayTransform );

	FL_ASSERT( mOverlayTransformStack.Number( ) <= _MAX_STACK_SIZE )
}

_void RenderDevice::PopOverlayTransform( )
{
	if ( mOverlayTransformStack.Number( ) == 0 )
		return;

	SetOverlayTransform( mOverlayTransformStack.Top( ) );

	mOverlayTransformStack.Pop( );
}

_void RenderDevice::SetWorldTransform( const Matrix4& matrix )
{
	mWorldTransform		= matrix;
	mNormalTransform	= matrix.GetRotationMatrix( );
	mScaleTransform		= ( Vector3::cZAxis * mNormalTransform ).Magnitude( );

	mNeedWorldTransform	= matrix != Matrix4::cIdentity;

	mDeviceToken.mWorldTransformToken ++;
}

const Matrix4& RenderDevice::GetWorldTransform( ) const
{
	return mWorldTransform;
}

const Matrix3& RenderDevice::GetNormalTransform( ) const
{
	return mNormalTransform;
}

_float RenderDevice::GetScaleTransform( ) const
{
	return mScaleTransform;
}

_void RenderDevice::PushWorldTransform( )
{
	mWorldTransformStack.Push( mWorldTransform );

	FL_ASSERT( mWorldTransformStack.Number( ) <= _MAX_STACK_SIZE )
}

_void RenderDevice::PopWorldTransform( )
{
	if ( mWorldTransformStack.Number( ) == 0 )
		return;

	SetWorldTransform( mWorldTransformStack.Top( ) );

	mWorldTransformStack.Pop( );
}

_void RenderDevice::SetViewTransform( const Matrix4& matrix )
{
	mViewTransform = matrix;

	// Reset view frustum.
	mViewFrustum = Frustum( mViewTransform, mProjectionTransform );

	mDeviceToken.mViewTransformToken ++;
}

const Matrix4& RenderDevice::GetViewTransform( ) const
{
	return mViewTransform;
}

_void RenderDevice::PushViewTransform( )
{
	mViewTransformStack.Push( mViewTransform );

	FL_ASSERT( mViewTransformStack.Number( ) <= _MAX_STACK_SIZE )
}

_void RenderDevice::PopViewTransform( )
{
	if ( mViewTransformStack.Number( ) == 0 )
		return;

	SetViewTransform( mViewTransformStack.Top( ) );

	mViewTransformStack.Pop( );
}

_void RenderDevice::SetProjectionTransform( const Matrix4& matrix )
{
	mProjectionTransform = matrix;

	// Reset view frustum.
	mViewFrustum = Frustum( mViewTransform, mProjectionTransform );

	mDeviceToken.mProjectionTransformToken ++;
}

const Matrix4& RenderDevice::GetProjectionTransform( ) const
{
	return mProjectionTransform;
}

_void RenderDevice::PushProjectionTransform( )
{
	mProjectionTransformStack.Push( mProjectionTransform );

	FL_ASSERT( mProjectionTransformStack.Number( ) <= _MAX_STACK_SIZE )
}

_void RenderDevice::PopProjectionTransform( )
{
	if ( mProjectionTransformStack.Number( ) == 0 )
		return;

	SetProjectionTransform( mProjectionTransformStack.Top( ) );

	mProjectionTransformStack.Pop( );
}

_void RenderDevice::SetTextureTransform( _dword index, const Matrix4& matrix )
{
	if ( index >= Geometry::_MAX_TEXTURE_NUMBER )
		return;

	mTextureTransform[ index ] = matrix;

	mDeviceToken.mTextureTransformToken[ index ] ++;
}

const Matrix4& RenderDevice::GetTextureTransform( _dword index ) const
{
	if ( index >= Geometry::_MAX_TEXTURE_NUMBER )
		return Matrix4::cIdentity;

	return mTextureTransform[ index ];
}

_void RenderDevice::PushTextureTransform( _dword index )
{
	if ( index >= Geometry::_MAX_TEXTURE_NUMBER )
		return;

	mTextureTransformStack[ index ].Push( mTextureTransform[ index ] );

	FL_ASSERT( mTextureTransformStack[ index ].Number( ) <= _MAX_STACK_SIZE )
}

_void RenderDevice::PopTextureTransform( _dword index )
{
	if ( index >= Geometry::_MAX_TEXTURE_NUMBER )
		return;

	if ( mTextureTransformStack[ index ].Number( ) == 0 )
		return;

	SetTextureTransform( index, mTextureTransformStack[ index ].Top( ) );

	mTextureTransformStack[ index ].Pop( );
}

_void RenderDevice::SetCamera( const Camera& camera )
{
	mCamera = camera;

	SetViewTransform( Matrix4::CreateLookAtLH( mCamera.mPosition, mCamera.mLook, mCamera.mUp ) );
}

const Camera& RenderDevice::GetCamera( ) const
{
	return mCamera;
}

_void RenderDevice::PushCamera( )
{
	mCameraStack.Push( mCamera );

	FL_ASSERT( mCameraStack.Number( ) <= _MAX_STACK_SIZE )
}

_void RenderDevice::PopCamera( )
{
	if ( mCameraStack.Number( ) == 0 )
		return;

	SetCamera( mCameraStack.Top( ) );

	mCameraStack.Pop( );
}

_void RenderDevice::SetViewport( const Viewport& viewport )
{
	mViewport = viewport;

	mViewMatrix = Matrix4::cIdentity;
	mViewMatrix( 0, 0 ) = viewport.w / 2.0f;
	mViewMatrix( 1, 1 ) = -viewport.h / 2.0f;
	mViewMatrix( 2, 2 ) = viewport.maxz - viewport.minz;
	mViewMatrix( 3, 0 ) = viewport.x + viewport.w / 2.0f;
	mViewMatrix( 3, 1 ) = viewport.y + viewport.h / 2.0f;
	mViewMatrix( 3, 2 ) = viewport.minz;

	mDeviceToken.mViewportToken ++;
}

const Viewport&	RenderDevice::GetViewport( ) const
{
	return mViewport;
}

_void RenderDevice::PushViewport( )
{
	mViewportStack.Push( mViewport );

	FL_ASSERT( mViewportStack.Number( ) <= _MAX_STACK_SIZE )
}

_void RenderDevice::PopViewport( )
{
	if ( mViewportStack.Number( ) == 0 )
		return;

	SetViewport( mViewportStack.Top( ) );

	mViewportStack.Pop( );
}

const Matrix4& RenderDevice::GetViewMatrix( ) const
{
	return mViewMatrix;
}

const Frustum& RenderDevice::GetViewFrustum( ) const
{
	return mViewFrustum;
}

Ray RenderDevice::GetPickRay( _long x, _long y, _bool ortho ) const
{
	Ray pickray;

	x -= mViewport.x;
	y -= mViewport.y;

	if ( ortho == _false )
	{
		pickray = Ray( 2.0f * x / mViewport.w - 1.0f, 2.0f * y / mViewport.h - 1.0f, mViewTransform, mProjectionTransform );
	}
	else
	{
		Vector3 temp1 = Vector3::cOrigin * mViewTransform;
		Vector4 temp2 = -Vector4::cZAxis * mViewTransform;

		pickray.mOrigin = Vector3( (_float)  x, temp1.z, (_float) -y );
		pickray.mDirection = temp2;
	}

	return pickray;
}

Matrix4 RenderDevice::GetClip2ImageMatrix( const Matrix4& view, const Matrix4& projection ) const
{
	Matrix4 viewinverse = mViewTransform;
	viewinverse.Inverse( );

	return viewinverse * view * projection * Matrix4::cClip2Image;
}

Vector2 RenderDevice::ProjectPoint2Screen( const Vector3& point ) const
{
	Vector3 point2d = point * mViewTransform * mProjectionTransform;

	if ( point2d.z >= 1.0f || point2d.z <= 0.0001f )
		return Vector2( Math::cMinFloat, Math::cMinFloat );
	else
		return Vector2( ( 1.0f + point2d.x ) * mViewport.w / 2.0f + mViewport.x, ( 1.0f - point2d.y ) * mViewport.h / 2.0f + mViewport.y );
}

Region RenderDevice::ProjectBox2Screen( const AxisAlignedBox& boundbox ) const
{
	Region region = Region::cInitRegion; Box box( boundbox.mMinVector, boundbox.mMaxVector );

	for ( _dword i = 0; i < 8; i ++ )
	{
		Vector3 point2d = box.mVectors[i] * mViewTransform * mProjectionTransform;

		if ( point2d.z >= 1.0f || point2d.z <= 0.0001f )
			continue;

		region += Vector2( ( 1.0f + point2d.x ) * mViewport.w / 2.0f + mViewport.x, ( 1.0f - point2d.y ) * mViewport.h / 2.0f + mViewport.y );
	}

	return region;
}

_dword RenderDevice::GetRenderTargetFormat( ) const
{
	return GraphicsManager::GetRenderAPIHelper( ).GetRenderTargetFormat( );
}

ITexture* RenderDevice::GetRenderTargetTexture( ) const
{
	return mRenderTargetTexture;
}

ISurface* RenderDevice::GetRenderTargetSurface( ) const
{
	return mRenderTargetSurface;
}

_dword RenderDevice::GetDepthStencilFormat( ) const
{
	return GraphicsManager::GetRenderAPIHelper( ).GetDepthStencilFormat( );
}

ITexture* RenderDevice::GetDepthStencilTexture( ) const
{
	return mDepthStencilTexture;
}

ISurface* RenderDevice::GetDepthStencilSurface( ) const
{
	return mDepthStencilSurface;
}

_void RenderDevice::SetRenderTexture( ITexture* rendertarget, ITexture* depthstencil )
{
	if ( mRenderTargetTexture == rendertarget && mDepthStencilTexture == depthstencil )
		return;

	mRenderTargetTexture = GetTextureFactory( ).CloneTexture( rendertarget );
	mRenderTargetSurface = _null;

	if ( rendertarget != _null )
		mRenderTargetSurface = GetTextureFactory( ).CloneSurface( rendertarget->GetSurface( 0 ) );

	mDepthStencilTexture = GetTextureFactory( ).CloneTexture( depthstencil );
	mDepthStencilSurface = _null;

	if ( depthstencil != _null )
		mDepthStencilSurface = GetTextureFactory( ).CloneSurface( depthstencil->GetSurface( 0 ) );

	GraphicsManager::GetRenderQueueHelper( ).SwitchRenderSet( );
}

_void RenderDevice::SetRenderSurface( ISurface* rendertarget, ISurface* depthstencil )
{
	if ( mRenderTargetSurface == rendertarget && mDepthStencilSurface == depthstencil )
		return;

	GetTextureFactory( ).ReleaseTexture( mRenderTargetTexture );
	mRenderTargetSurface = GetTextureFactory( ).CloneSurface( rendertarget );

	GetTextureFactory( ).ReleaseTexture( mDepthStencilTexture );
	mDepthStencilSurface = GetTextureFactory( ).CloneSurface( depthstencil );

	GraphicsManager::GetRenderQueueHelper( ).SwitchRenderSet( );
}

_void RenderDevice::ResetRenderTarget( )
{
	if ( mRenderTargetSurface == _null && mDepthStencilSurface == _null )
		return;

	GetTextureFactory( ).ReleaseTexture( mRenderTargetTexture );
	GetTextureFactory( ).ReleaseSurface( mRenderTargetSurface );

	GetTextureFactory( ).ReleaseTexture( mDepthStencilTexture );
	GetTextureFactory( ).ReleaseSurface( mDepthStencilSurface );

	GraphicsManager::GetRenderQueueHelper( ).SwitchRenderSet( );
}

_void RenderDevice::SetRenderMethod( IRenderMethod* method )
{
	mTechniqueUnit.SetRenderMethod( method );
}

IRenderMethod* RenderDevice::GetRenderMethod( ) const
{
	return mTechniqueUnit.mRenderMethod;
}

_void RenderDevice::SetSamplerMethod( ISamplerMethod* method )
{
	mTechniqueUnit.SetSamplerMethod( method );
}

ISamplerMethod*	RenderDevice::GetSamplerMethod( ) const
{
	return mTechniqueUnit.mSamplerMethod;
}

_void RenderDevice::SetTextureMethod( ITextureMethod* method )
{
	mTechniqueUnit.SetTextureMethod( method );
}

ITextureMethod*	RenderDevice::GetTextureMethod( ) const
{
	return mTechniqueUnit.mTextureMethod;
}

_void RenderDevice::SetGlobalRenderMethod( IRenderMethod* method )
{
	mTechniqueUnit.SetGlobalRenderMethod( method );
}

IRenderMethod* RenderDevice::GetGlobalRenderMethod( ) const
{
	return mTechniqueUnit.mGlobalRenderMethod;
}

_void RenderDevice::SetGlobalSamplerMethod( ISamplerMethod* method )
{
	mTechniqueUnit.SetGlobalSamplerMethod( method );
}

ISamplerMethod* RenderDevice::GetGlobalSamplerMethod( ) const
{
	return mTechniqueUnit.mGlobalSamplerMethod;
}

_void RenderDevice::SetGlobalTextureMethod( ITextureMethod* method )
{
	mTechniqueUnit.SetGlobalTextureMethod( method );
}

ITextureMethod* RenderDevice::GetGlobalTextureMethod( ) const
{
	return mTechniqueUnit.mGlobalTextureMethod;
}

_void RenderDevice::SetShaderConst( IShaderConst* shaderconst )
{
	mTechniqueUnit.SetShaderConst( shaderconst );
}

IShaderConst* RenderDevice::GetShaderConst( ) const
{
	return mTechniqueUnit.mShaderConst;
}

_void RenderDevice::PushAllRenderMethod( )
{
	TechniqueUnit newtech;
	newtech.Clone( mTechniqueUnit );

	mTechniqueUnitStack.Push( newtech );
}

_void RenderDevice::PopAllRenderMethod( )
{
	if ( mTechniqueUnitStack.Number( ) == 0 )
		return;

	mTechniqueUnit.Clone( mTechniqueUnitStack.Top( ) );
	mTechniqueUnitStack.Top( ).Clear( );

	mTechniqueUnitStack.Pop( );
}

_void RenderDevice::SetRenderLayer( _dword layer )
{
	GraphicsManager::GetRenderSetHelper( ).SetRenderLayer( layer );
}

_void RenderDevice::ResetRenderLayer( )
{
	GraphicsManager::GetRenderSetHelper( ).ResetRenderLayer( );
}

_void RenderDevice::BeginSubRenderLayer( _dword layer )
{
	GraphicsManager::GetRenderSetHelper( ).BeginSubRenderLayer( layer );
}

_void RenderDevice::EndSubRenderLayer( )
{
	GraphicsManager::GetRenderSetHelper( ).EndSubRenderLayer( );
}

_void RenderDevice::Render( const Geometry& geometry )
{
	if ( geometry.mVertexDeclaration == _null || geometry.mVertexBuffer == _null )
		return;

	GraphicsManager::GetRenderSetHelper( ).Render( geometry, mTechniqueUnit, Matrix4::cIdentity );
}

_void RenderDevice::Render( const Geometry& geometry, const Matrix4& transform )
{
	if ( geometry.mVertexDeclaration == _null || geometry.mVertexBuffer == _null )
		return;

	GraphicsManager::GetRenderSetHelper( ).Render( geometry, mTechniqueUnit, transform );
}

_void RenderDevice::Render( const OverlayPoint& point )
{
	if ( mNeedOverlayTransform == _false && mNeedOverlayClip == _false )
	{
		GraphicsManager::GetRenderSetHelper( ).Render( point, mTechniqueUnit );
	}
	else
	{
		OverlayPoint transformedpoint = point;

		if ( mNeedOverlayTransform )
		{
			transformedpoint.mPosition *= mOverlayTransform;
		}

		if ( mNeedOverlayClip )
		{
			if ( OverlayClipper::ClipPoint( mOverlayClipRegion, transformedpoint ) == _false )
				return;
		}

		GraphicsManager::GetRenderSetHelper( ).Render( transformedpoint, mTechniqueUnit );
	}
}

_void RenderDevice::Render( const OverlayLine& line )
{
	if ( mNeedOverlayTransform == _false && mNeedOverlayClip == _false )
	{
		GraphicsManager::GetRenderSetHelper( ).Render( line, mTechniqueUnit );
	}
	else
	{
		OverlayLine transformedline = line;

		if ( mNeedOverlayTransform )
		{
			transformedline.mPosition1 *= mOverlayTransform;
			transformedline.mPosition2 *= mOverlayTransform;
		}

		if ( mNeedOverlayClip )
		{
			if ( OverlayClipper::ClipLine( mOverlayClipRegion, transformedline ) == _false )
				return;
		}

		GraphicsManager::GetRenderSetHelper( ).Render( transformedline, mTechniqueUnit );
	}
}

_void RenderDevice::Render( const OverlayTriangle& triangle )
{
	if ( mNeedOverlayTransform == _false && mNeedOverlayClip == _false )
	{
		GraphicsManager::GetRenderSetHelper( ).Render( triangle, mTechniqueUnit );
	}
	else
	{
		OverlayTriangle transformedtriangle = triangle;

		if ( mNeedOverlayTransform )
		{
			transformedtriangle.mPosition1 *= mOverlayTransform;
			transformedtriangle.mPosition2 *= mOverlayTransform;
			transformedtriangle.mPosition3 *= mOverlayTransform;
		}

		GraphicsManager::GetRenderSetHelper( ).Render( transformedtriangle, mTechniqueUnit );
	}
}

_void RenderDevice::Render( const OverlayTriangleImage& triangleimage )
{
	if ( mNeedOverlayTransform == _false && mNeedOverlayClip == _false )
	{
		GraphicsManager::GetRenderSetHelper( ).Render( triangleimage, mTechniqueUnit );
	}
	else
	{
		OverlayTriangleImage transformedtriangle = triangleimage;

		if ( mNeedOverlayTransform )
		{
			transformedtriangle.mPosition1 *= mOverlayTransform;
			transformedtriangle.mPosition2 *= mOverlayTransform;
			transformedtriangle.mPosition3 *= mOverlayTransform;
		}

		GraphicsManager::GetRenderSetHelper( ).Render( transformedtriangle, mTechniqueUnit );
	}
}

_void RenderDevice::Render( const OverlayRectangle& rectangle )
{
	// We dont need transform.
	if ( mNeedOverlayTransform == _false )
	{
		if ( mNeedOverlayClip )
		{
			OverlayRectangle rectanglecopy = rectangle;
			if ( OverlayClipper::ClipRectangle( mOverlayClipRegion, rectanglecopy ) == _false )
				return;

			GraphicsManager::GetRenderSetHelper( ).Render( rectanglecopy, mTechniqueUnit );
		}
		else
		{
			GraphicsManager::GetRenderSetHelper( ).Render( rectangle, mTechniqueUnit );
		}
	}
	// We need transform.
	else
	{
		OverlayQuadrangle transformedquadrangle;

		transformedquadrangle.mPositionLT	= rectangle.mPositionLT;
		transformedquadrangle.mPositionRT	= Vector2( rectangle.mPositionRB.x, rectangle.mPositionLT.y );
		transformedquadrangle.mPositionLB	= Vector2( rectangle.mPositionLT.x, rectangle.mPositionRB.y );
		transformedquadrangle.mPositionRB	= rectangle.mPositionRB;
		transformedquadrangle.mColorLT		= rectangle.mColor;
		transformedquadrangle.mColorRT		= rectangle.mColor;
		transformedquadrangle.mColorLB		= rectangle.mColor;
		transformedquadrangle.mColorRB		= rectangle.mColor;

		if ( mNeedOverlayTransform )
		{
			transformedquadrangle.mPositionLT *= mOverlayTransform;
			transformedquadrangle.mPositionRT *= mOverlayTransform;
			transformedquadrangle.mPositionLB *= mOverlayTransform;
			transformedquadrangle.mPositionRB *= mOverlayTransform;
		}

		if ( mNeedOverlayClip )
		{
			if ( OverlayClipper::ClipQuadrangle( mOverlayClipRegion, transformedquadrangle ) == _false )
				return;
		}

		GraphicsManager::GetRenderSetHelper( ).Render( transformedquadrangle, mTechniqueUnit );
	}
}

_void RenderDevice::Render( const OverlayRectangleImage& rectangleimage )
{
	// We dont need transform.
	if ( mNeedOverlayTransform == _false )
	{
		if ( mNeedOverlayClip )
		{
			OverlayRectangleImage rectangleimagecopy = rectangleimage;
			if ( OverlayClipper::ClipRectangle( mOverlayClipRegion, rectangleimagecopy ) == _false )
				return;

			GraphicsManager::GetRenderSetHelper( ).Render( rectangleimagecopy, mTechniqueUnit );
		}
		else
		{
			GraphicsManager::GetRenderSetHelper( ).Render( rectangleimage, mTechniqueUnit );
		}
	}
	// We need transform.
	else
	{
		OverlayQuadrangleImage transformedquadrangle;

		transformedquadrangle.mPositionLT	= rectangleimage.mPositionLT;
		transformedquadrangle.mPositionRT	= Vector2( rectangleimage.mPositionRB.x, rectangleimage.mPositionLT.y );
		transformedquadrangle.mPositionLB	= Vector2( rectangleimage.mPositionLT.x, rectangleimage.mPositionRB.y );
		transformedquadrangle.mPositionRB	= rectangleimage.mPositionRB;
		transformedquadrangle.mTexcoordLT	= rectangleimage.mTexcoordLT;
		transformedquadrangle.mTexcoordRT	= Vector2( rectangleimage.mTexcoordRB.x, rectangleimage.mTexcoordLT.y );
		transformedquadrangle.mTexcoordLB	= Vector2( rectangleimage.mTexcoordLT.x, rectangleimage.mTexcoordRB.y );
		transformedquadrangle.mTexcoordRB	= rectangleimage.mTexcoordRB;
		transformedquadrangle.mColorLT		= rectangleimage.mColor;
		transformedquadrangle.mColorRT		= rectangleimage.mColor;
		transformedquadrangle.mColorLB		= rectangleimage.mColor;
		transformedquadrangle.mColorRB		= rectangleimage.mColor;
		transformedquadrangle.mTexture		= rectangleimage.mTexture;

		if ( mNeedOverlayTransform )
		{
			transformedquadrangle.mPositionLT *= mOverlayTransform;
			transformedquadrangle.mPositionRT *= mOverlayTransform;
			transformedquadrangle.mPositionLB *= mOverlayTransform;
			transformedquadrangle.mPositionRB *= mOverlayTransform;
		}

		if ( mNeedOverlayClip )
		{
			if ( OverlayClipper::ClipQuadrangle( mOverlayClipRegion, transformedquadrangle ) == _false )
				return;
		}

		GraphicsManager::GetRenderSetHelper( ).Render( transformedquadrangle, mTechniqueUnit );
	}
}

_void RenderDevice::Render( const OverlayQuadrangle& quadrangle )
{
	if ( mNeedOverlayTransform == _false && mNeedOverlayClip == _false )
	{
		GraphicsManager::GetRenderSetHelper( ).Render( quadrangle, mTechniqueUnit );
	}
	else
	{
		OverlayQuadrangle transformedquadrangle = quadrangle;

		if ( mNeedOverlayTransform )
		{
			transformedquadrangle.mPositionLT *= mOverlayTransform;
			transformedquadrangle.mPositionRT *= mOverlayTransform;
			transformedquadrangle.mPositionLB *= mOverlayTransform;
			transformedquadrangle.mPositionRB *= mOverlayTransform;
		}

		if ( mNeedOverlayClip )
		{
			if ( OverlayClipper::ClipQuadrangle( mOverlayClipRegion, transformedquadrangle ) == _false )
				return;
		}

		GraphicsManager::GetRenderSetHelper( ).Render( transformedquadrangle, mTechniqueUnit );
	}
}

_void RenderDevice::Render( const OverlayQuadrangleImage& quadrangleimage )
{
	if ( mNeedOverlayTransform == _false && mNeedOverlayClip == _false )
	{
		GraphicsManager::GetRenderSetHelper( ).Render( quadrangleimage, mTechniqueUnit );
	}
	else
	{
		OverlayQuadrangleImage transformedquadrangle = quadrangleimage;

		if ( mNeedOverlayTransform )
		{
			transformedquadrangle.mPositionLT *= mOverlayTransform;
			transformedquadrangle.mPositionRT *= mOverlayTransform;
			transformedquadrangle.mPositionLB *= mOverlayTransform;
			transformedquadrangle.mPositionRB *= mOverlayTransform;
		}

		if ( mNeedOverlayClip )
		{
			if ( OverlayClipper::ClipQuadrangle( mOverlayClipRegion, transformedquadrangle ) == _false )
				return;
		}

		GraphicsManager::GetRenderSetHelper( ).Render( transformedquadrangle, mTechniqueUnit );
	}
}

_void RenderDevice::Render( const OverlayRectangleMask& rectanglemask )
{
	if ( rectanglemask.mPercentage <= 0.0f )
		return;

	_float x1 = rectanglemask.mPositionLT.x;
	_float y1 = rectanglemask.mPositionLT.y;
	_float x2 = rectanglemask.mPositionRB.x;
	_float y2 = rectanglemask.mPositionRB.y;
	_float x0 = ( x1 + x2 ) * 0.5f;
	_float y0 = ( y1 + y2 ) * 0.5f;
	_float r  = Vector2( rectanglemask.mPositionLT - rectanglemask.mPositionRB ).Magnitude( ) * 0.5f;

	if ( rectanglemask.mPercentage > 0.0f )
	{
		_float p = rectanglemask.mPercentage;

		if ( p > 0.125f )
			p = 0.125f;

		_float xs = x0 + Math::Sin( ( 1.0f - p ) * Math::c2Pi ) * r;
		_float ys = y0 - Math::Cos( ( 1.0f - p ) * Math::c2Pi ) * r;

		if ( ys < y1 )
			ys = y1;

		_float xe = x0;
		_float ye = y1;

		Render( OverlayTriangle( Vector2( xs, ys ), Vector2( xe, ye ), Vector2( x0, y0 ), rectanglemask.mColor ) );
	}

	if ( rectanglemask.mPercentage >= 0.125f )
	{
		_float p = rectanglemask.mPercentage;

		if ( p > 0.250f )
			p = 0.250f;

		_float xs = x0 + Math::Sin( ( 1.0f - p ) * Math::c2Pi ) * r;
		_float ys = y0 - Math::Cos( ( 1.0f - p ) * Math::c2Pi ) * r;

		if ( xs < x1 )
			xs = x1;

		_float xe = x1;
		_float ye = y1;

		Render( OverlayTriangle( Vector2( xs, ys ), Vector2( xe, ye ), Vector2( x0, y0 ), rectanglemask.mColor ) );
	}

	if ( rectanglemask.mPercentage >= 0.250f )
	{
		_float p = rectanglemask.mPercentage;

		if ( p > 0.375f )
			p = 0.375f;

		_float xs = x0 + Math::Sin( ( 1.0f - p ) * Math::c2Pi ) * r;
		_float ys = y0 - Math::Cos( ( 1.0f - p ) * Math::c2Pi ) * r;

		if ( xs < x1 )
			xs = x1;

		_float xe = x1;
		_float ye = y0;

		Render( OverlayTriangle( Vector2( xs, ys ), Vector2( xe, ye ), Vector2( x0, y0 ), rectanglemask.mColor ) );
	}

	if ( rectanglemask.mPercentage >= 0.375f )
	{
		_float p = rectanglemask.mPercentage;

		if ( p > 0.500f )
			p = 0.500f;

		_float xs = x0 + Math::Sin( ( 1.0f - p ) * Math::c2Pi ) * r;
		_float ys = y0 - Math::Cos( ( 1.0f - p ) * Math::c2Pi ) * r;

		if ( ys > y2 )
			ys = y2;

		_float xe = x1;
		_float ye = y2;

		Render( OverlayTriangle( Vector2( xs, ys ), Vector2( xe, ye ), Vector2( x0, y0 ), rectanglemask.mColor ) );
	}

	if ( rectanglemask.mPercentage >= 0.500f )
	{
		_float p = rectanglemask.mPercentage;

		if ( p > 0.625f )
			p = 0.625f;

		_float xs = x0 + Math::Sin( ( 1.0f - p ) * Math::c2Pi ) * r;
		_float ys = y0 - Math::Cos( ( 1.0f - p ) * Math::c2Pi ) * r;

		if ( ys > y2 )
			ys = y2;

		_float xe = x0;
		_float ye = y2;

		Render( OverlayTriangle( Vector2( xs, ys ), Vector2( xe, ye ), Vector2( x0, y0 ), rectanglemask.mColor ) );
	}

	if ( rectanglemask.mPercentage >= 0.625f )
	{
		_float p = rectanglemask.mPercentage;

		if ( p > 0.750f )
			p = 0.750f;

		_float xs = x0 + Math::Sin( ( 1.0f - p ) * Math::c2Pi ) * r;
		_float ys = y0 - Math::Cos( ( 1.0f - p ) * Math::c2Pi ) * r;

		if ( xs > x2 )
			xs = x2;

		_float xe = x2;
		_float ye = y2;

		Render( OverlayTriangle( Vector2( xs, ys ), Vector2( xe, ye ), Vector2( x0, y0 ), rectanglemask.mColor ) );
	}

	if ( rectanglemask.mPercentage >= 0.750f )
	{
		_float p = rectanglemask.mPercentage;

		if ( p > 0.875f )
			p = 0.875f;

		_float xs = x0 + Math::Sin( ( 1.0f - p ) * Math::c2Pi ) * r;
		_float ys = y0 - Math::Cos( ( 1.0f - p ) * Math::c2Pi ) * r;

		if ( xs > x2 )
			xs = x2;

		_float xe = x2;
		_float ye = y0;

		Render( OverlayTriangle( Vector2( xs, ys ), Vector2( xe, ye ), Vector2( x0, y0 ), rectanglemask.mColor ) );
	}

	if ( rectanglemask.mPercentage >= 0.875f )
	{
		_float p = rectanglemask.mPercentage;

		if ( p > 1.000f )
			p = 1.000f;

		_float xs = x0 + Math::Sin( ( 1.0f - p ) * Math::c2Pi ) * r;
		_float ys = y0 - Math::Cos( ( 1.0f - p ) * Math::c2Pi ) * r;

		if ( ys < y1 )
			ys = y1;

		_float xe = x2;
		_float ye = y1;

		Render( OverlayTriangle( Vector2( xs, ys ), Vector2( xe, ye ), Vector2( x0, y0 ), rectanglemask.mColor ) );
	}
}

_void RenderDevice::Render( const PrimitivePoint& point )
{
	if ( mNeedWorldTransform == _false )
	{
		GraphicsManager::GetRenderSetHelper( ).Render( point, mTechniqueUnit );
	}
	else
	{
		PrimitivePoint transformedpoint = point;

		if ( mNeedWorldTransform )
		{
			transformedpoint.mPosition *= mWorldTransform;
		}

		GraphicsManager::GetRenderSetHelper( ).Render( transformedpoint, mTechniqueUnit );
	}
}

_void RenderDevice::Render( const PrimitiveLine& line )
{
	if ( mNeedWorldTransform == _false )
	{
		GraphicsManager::GetRenderSetHelper( ).Render( line, mTechniqueUnit );
	}
	else
	{
		PrimitiveLine transformedline = line;

		if ( mNeedWorldTransform )
		{
			transformedline.mPosition1 *= mWorldTransform;
			transformedline.mPosition2 *= mWorldTransform;
		}

		GraphicsManager::GetRenderSetHelper( ).Render( transformedline, mTechniqueUnit );
	}
}

_void RenderDevice::Render( const PrimitiveTriangle& triangle )
{
	if ( mNeedWorldTransform == _false )
	{
		GraphicsManager::GetRenderSetHelper( ).Render( triangle, mTechniqueUnit );
	}
	else
	{
		PrimitiveTriangle transformedtriangle = triangle;

		if ( mNeedWorldTransform )
		{
			transformedtriangle.mPosition1 *= mWorldTransform;
			transformedtriangle.mPosition2 *= mWorldTransform;
			transformedtriangle.mPosition3 *= mWorldTransform;
		}

		GraphicsManager::GetRenderSetHelper( ).Render( transformedtriangle, mTechniqueUnit );
	}
}

_void RenderDevice::Render( const PrimitiveTriangleImage& triangleimage )
{
	if ( mNeedWorldTransform == _false )
	{
		GraphicsManager::GetRenderSetHelper( ).Render( triangleimage, mTechniqueUnit );
	}
	else
	{
		PrimitiveTriangleImage transformedtriangleimage = triangleimage;

		if ( mNeedWorldTransform )
		{
			transformedtriangleimage.mPosition1 *= mWorldTransform;
			transformedtriangleimage.mPosition2 *= mWorldTransform;
			transformedtriangleimage.mPosition3 *= mWorldTransform;
		}

		GraphicsManager::GetRenderSetHelper( ).Render( transformedtriangleimage, mTechniqueUnit );
	}
}

_void RenderDevice::Render( const PrimitiveQuadrangle& quadrangle )
{
	if ( mNeedWorldTransform == _false )
	{
		GraphicsManager::GetRenderSetHelper( ).Render( quadrangle, mTechniqueUnit );
	}
	else
	{
		PrimitiveQuadrangle transformedquadrangle = quadrangle;

		if ( mNeedWorldTransform )
		{
			transformedquadrangle.mCenter	*= mWorldTransform;
			transformedquadrangle.mWidth	*= mNormalTransform;
			transformedquadrangle.mHeight	*= mNormalTransform;
		}

		GraphicsManager::GetRenderSetHelper( ).Render( transformedquadrangle, mTechniqueUnit );
	}
}

_void RenderDevice::Render( const PrimitiveQuadrangleImage& quadrangleimage )
{
	if ( mNeedWorldTransform == _false )
	{
		GraphicsManager::GetRenderSetHelper( ).Render( quadrangleimage, mTechniqueUnit );
	}
	else
	{
		PrimitiveQuadrangleImage transformedquadrangleimage = quadrangleimage;

		if ( mNeedWorldTransform )
		{
			transformedquadrangleimage.mCenter	*= mWorldTransform;
			transformedquadrangleimage.mWidth	*= mNormalTransform;
			transformedquadrangleimage.mHeight	*= mNormalTransform;
		}

		GraphicsManager::GetRenderSetHelper( ).Render( transformedquadrangleimage, mTechniqueUnit );
	}
}

_void RenderDevice::Render( const PrimitiveBox& box )
{
	if ( mNeedWorldTransform == _false )
	{
		GraphicsManager::GetRenderSetHelper( ).Render( box, mTechniqueUnit );
	}
	else
	{
		PrimitiveBox transformedbox = box;

		if ( mNeedWorldTransform )
		{
			transformedbox.mCenter	*= mWorldTransform;
			transformedbox.mWidth	*= mNormalTransform;
			transformedbox.mHeight	*= mNormalTransform;
			transformedbox.mDepth	*= mNormalTransform;
		}

		GraphicsManager::GetRenderSetHelper( ).Render( transformedbox, mTechniqueUnit );
	}
}

_void RenderDevice::Render( const PrimitiveCircle& circle )
{
	if ( mNeedWorldTransform == _false )
	{
		GraphicsManager::GetRenderSetHelper( ).Render( circle, mTechniqueUnit );
	}
	else
	{
		PrimitiveCircle transformedcircle = circle;

		if ( mNeedWorldTransform )
		{
			transformedcircle.mCenter *= mWorldTransform;
			transformedcircle.mNormal *= mNormalTransform;
		}

		GraphicsManager::GetRenderSetHelper( ).Render( transformedcircle, mTechniqueUnit );
	}
}

_void RenderDevice::Render( const PrimitiveSphere& sphere )
{
	if ( mNeedWorldTransform == _false )
	{
		GraphicsManager::GetRenderSetHelper( ).Render( sphere, mTechniqueUnit );
	}
	else
	{
		PrimitiveSphere transformedsphere = sphere;

		if ( mNeedWorldTransform )
		{
			transformedsphere.mCenter *= mWorldTransform;
		}

		GraphicsManager::GetRenderSetHelper( ).Render( transformedsphere, mTechniqueUnit );
	}
}

_void RenderDevice::Render( const PrimitivePyramid& pyramid )
{
	if ( mNeedWorldTransform == _false )
	{
		GraphicsManager::GetRenderSetHelper( ).Render( pyramid, mTechniqueUnit );
	}
	else
	{
		PrimitivePyramid transformedpyramid = pyramid;

		if ( mNeedWorldTransform )
		{
			transformedpyramid.mBase *= mWorldTransform;
			transformedpyramid.mPeak *= mWorldTransform;
		}

		GraphicsManager::GetRenderSetHelper( ).Render( transformedpyramid, mTechniqueUnit );
	}
}

_void RenderDevice::Render( const PrimitiveAxis& axis )
{
	if ( mNeedWorldTransform == _false )
	{
		GraphicsManager::GetRenderSetHelper( ).Render( axis, mTechniqueUnit );
	}
	else
	{
		PrimitiveAxis transformedaxis = axis;

		if ( mNeedWorldTransform )
		{
			transformedaxis.mOrigin	*= mWorldTransform;
			transformedaxis.mXAxis	*= mNormalTransform;
			transformedaxis.mYAxis	*= mNormalTransform;
		}

		GraphicsManager::GetRenderSetHelper( ).Render( transformedaxis, mTechniqueUnit );
	}
}

_void RenderDevice::Render( const PrimitiveFrustum& frustum )
{
	GraphicsManager::GetRenderSetHelper( ).Render( frustum, mTechniqueUnit );
}

_void RenderDevice::Render( const BillboardScreenAligned& screenaligned )
{
	if ( mNeedWorldTransform == _false )
	{
		GraphicsManager::GetRenderSetHelper( ).Render( screenaligned, mTechniqueUnit );
	}
	else
	{
		BillboardScreenAligned transformedbillboard = screenaligned;

		if ( mNeedWorldTransform )
		{
			transformedbillboard.mPosition	*= mWorldTransform;
			transformedbillboard.mSize		*= mScaleTransform;
		}

		GraphicsManager::GetRenderSetHelper( ).Render( transformedbillboard, mTechniqueUnit );
	}
}

_void RenderDevice::Render( const BillboardWorldOriented& worldoriented )
{
	if ( mNeedWorldTransform == _false )
	{
		GraphicsManager::GetRenderSetHelper( ).Render( worldoriented, mTechniqueUnit );
	}
	else
	{
		BillboardWorldOriented transformedbillboard = worldoriented;

		if ( mNeedWorldTransform )
		{
			transformedbillboard.mPosition	*= mWorldTransform;
			transformedbillboard.mSize		*= mScaleTransform;
		}

		GraphicsManager::GetRenderSetHelper( ).Render( transformedbillboard, mTechniqueUnit );
	}
}

_void RenderDevice::Render( const BillboardAxial& axial )
{
	if ( mNeedWorldTransform == _false )
	{
		GraphicsManager::GetRenderSetHelper( ).Render( axial, mTechniqueUnit );
	}
	else
	{
		BillboardAxial transformedbillboard = axial;

		if ( mNeedWorldTransform )
		{
			transformedbillboard.mPosition	*= mWorldTransform;
			transformedbillboard.mAxis		*= mNormalTransform;
			transformedbillboard.mSize		*= mScaleTransform;
		}

		GraphicsManager::GetRenderSetHelper( ).Render( transformedbillboard, mTechniqueUnit );
	}
}

_void RenderDevice::Render( IFontTexture* font, const Vector2& position, _char character )
{
	if ( font == _null )
		return;

	_char string[2] = { character, 0 };

	( (FontTexture*) font )->Render( position.x, position.y, 0, string );
}

_void RenderDevice::Render( IFontTexture* font, const Vector2& position, _dword align, const _char* format, ... )
{
	if ( font == _null )
		return;

	FL_ASSERT( format != _null )

	_char buffer[10240];
	StringFormatter::FormatBufferV( buffer, 10240, format, (_chara*)( &format ) + sizeof( format ) );

	_float x = position.x;
	_float y = position.y;

	_char* string = buffer;

	for ( _dword i = 0; buffer[i] != 0; i ++ )
	{
		if ( buffer[i] != '\n' )
			continue;

		buffer[i] = 0;

		( (FontTexture*) font )->Render( x, y, align, string );

		string = buffer + i + 1; y += ( font->GetFontHeight( ) + font->GetFontSpaceVertical( ) ) * font->GetFontScaleVertical( );
	}

	( (FontTexture*) font )->Render( x, y, align, string );
}

_void RenderDevice::Render( IFontTexture* font, const Vector3& center, _float height, _char character )
{
	_char string[2] = { character, 0 };
	Render( font, center, height, string );
}

_void RenderDevice::Render( IFontTexture* font, const Vector3& center, _float height, const _char* format, ... )
{
	if ( font == _null )
		return;

	FL_ASSERT( format != _null )

	_char buffer[10240];
	StringFormatter::FormatBufferV( buffer, 10240, format, (_chara*)( &format ) + sizeof( format ) );

	_char* string = buffer;

	Vector3 newcenter = center;
	for ( _dword i = 0; buffer[i] != 0; i ++ )
	{
		if ( buffer[i] != '\n' )
			continue;

		buffer[i] = 0;

		( (FontTexture*) font )->Render( newcenter, height, string );

		string = buffer + i + 1;
		newcenter.y += font->GetFontHeight( );
	}

	( (FontTexture*) font )->Render( newcenter, height, string );
}

_void RenderDevice::Render( IFontTexture* font, const Vector3& center, const Vector3& width, const Vector3& height, _char character )
{
	_char string[2] = { character, 0 };
	Render( font, center, width, height, string );
}

_void RenderDevice::Render( IFontTexture* font, const Vector3& center, const Vector3& width, const Vector3& height, const _char* format, ... )
{
	if ( font == _null )
		return;

	FL_ASSERT( format != _null )

	_char buffer[10240];
	StringFormatter::FormatBufferV( buffer, 10240, format, (_chara*)( &format ) + sizeof( format ) );

	_char* string = buffer;

	_float y = 0;
	for ( _dword i = 0; buffer[i] != 0; i ++ )
	{
		if ( buffer[i] != '\n' )
			continue;

		buffer[i] = 0;

		( (FontTexture*) font )->Render( y, center, width, height, width.Magnitude( ), string );

		string = buffer + i + 1; y += font->GetFontHeight( );
	}

	( (FontTexture*) font )->Render( y, center, width, height, width.Magnitude( ), string );
}

_void RenderDevice::Render( IMesh* mesh )
{
	if ( mesh == _null )
		return;

	for ( _dword i = 0; i < mesh->GetModelNumber( ); i ++ )
		Render( mesh->GetModelGeometry( i ) );

	for ( _dword j = 0; j < mesh->GetMarkerNumber( ); j ++ )
		Render( PrimitiveAxis( mesh->GetMarkerTranslation( j ), mesh->GetMarkerRotation( j ), 50.0f ) );
}

_void RenderDevice::Render( ISkeleton* skeleton, _dword parentcolor, _dword childcolor, _dword dummycolor )
{
	if ( skeleton == _null )
		return;

	// Only one bone, just render it as sphere.
	if ( skeleton->GetBoneNumber( ) == 1 )
	{
		Render( PrimitiveSphere( skeleton->GetBoneTranslation( 0 ), 1.0f, 20, parentcolor ) );
	}
	// More than one bone, render pyramid as link from client to parent.
	else
	{
		for ( _dword i = 0; i < skeleton->GetBoneNumber( ); i ++ )
		{
			_dword parent = skeleton->GetBoneParent( i );

			if ( parent == -1 )
				continue;

			_dword color = -1;

			if ( skeleton->GetBoneParent( parent ) == -1 )
				color = parentcolor;
			else
				color = childcolor;

			Render( PrimitivePyramid( skeleton->GetBoneTranslation( parent ), skeleton->GetBoneTranslation( i ), color ) );
		}
	}
}

_void RenderDevice::Render( IParticleSystem* particlesystem )
{
	if ( particlesystem == _null )
		return;

	( (ParticleSystem*) particlesystem )->RenderParticles( );
}

_void RenderDevice::Render( IRenderSet* renderset )
{
	if ( renderset == _null )
		return;

	GraphicsManager::GetRenderQueueHelper( ).AppendRenderSet( (RenderSet*) renderset );
}
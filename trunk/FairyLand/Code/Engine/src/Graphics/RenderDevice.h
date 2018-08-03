//============================================================================
// RenderDevice.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// RenderDevice
//----------------------------------------------------------------------------

class RenderDevice : public IRenderDevice
{
private:
	enum { _MAX_STACK_SIZE = 32 };

	ITexture*				mRenderTargetTexture;
	ISurface*				mRenderTargetSurface;

	ITexture*				mDepthStencilTexture;
	ISurface*				mDepthStencilSurface;

	_bool					mNeedOverlayClip;
	Region					mOverlayClipRegion;

	_bool					mNeedOverlayTransform;
	Matrix3					mOverlayTransform;
	Stack< Matrix3 >		mOverlayTransformStack;

	_bool					mNeedWorldTransform;
	Matrix4					mWorldTransform;
	Matrix3					mNormalTransform;
	_float					mScaleTransform;
	Stack< Matrix4 >		mWorldTransformStack;

	Matrix4					mViewTransform;
	Stack< Matrix4 >		mViewTransformStack;

	Matrix4					mProjectionTransform;
	Stack< Matrix4 >		mProjectionTransformStack;

	Matrix4					mTextureTransform[ Geometry::_MAX_TEXTURE_NUMBER ];
	Stack< Matrix4 >		mTextureTransformStack[ Geometry::_MAX_TEXTURE_NUMBER ];

	Camera					mCamera;
	Stack< Camera >			mCameraStack;

	Viewport				mViewport;
	Stack< Viewport >		mViewportStack;

	Matrix4					mViewMatrix;
	Frustum					mViewFrustum;
	DeviceToken				mDeviceToken;

	TechniqueUnit			mTechniqueUnit;
	Stack< TechniqueUnit >	mTechniqueUnitStack;

public:
	RenderDevice( );
	~RenderDevice( );

	inline const DeviceToken& GetDeviceToken( )
		{ return mDeviceToken; }

	virtual _bool			IsDeviceLost( );
	virtual _bool			ResizeDevice( _dword width, _dword height );
	virtual _bool			ResetDevice( );

	virtual _handle			GetDeviceHandle( ) const;
	virtual _dword			GetDeviceWidth( ) const;
	virtual _dword			GetDeviceHeight( ) const;

	virtual _void			SetClearColor( _dword color );
	virtual _void			SetClearDepth( _float depth );
	virtual _void			SetClearStencil( _dword stencil );

	virtual _void			ClearBuffer( _bool color, _bool depth, _bool stencil );
	virtual _void			Flush( );
	virtual _void			Present( _handle targetwindow );

	virtual _void			CombineOverlayClipRegion( const Region& clipregion );
	virtual _void			SetOverlayClipRegion( const Region& clipregion );
	virtual const Region&	GetOverlayClipRegion( ) const;

	virtual _void			SetOverlayTransform( const Matrix3& matrix );
	virtual const Matrix3&	GetOverlayTransform( ) const;
	virtual _void			PushOverlayTransform( );
	virtual _void			PopOverlayTransform( );

	virtual _void			SetWorldTransform( const Matrix4& matrix );
	virtual const Matrix4&	GetWorldTransform( ) const;
	virtual const Matrix3&	GetNormalTransform( ) const;
	virtual _float			GetScaleTransform( ) const;
	virtual _void			PushWorldTransform( );
	virtual _void			PopWorldTransform( );

	virtual _void			SetViewTransform( const Matrix4& matrix );
	virtual const Matrix4&	GetViewTransform( ) const;
	virtual _void			PushViewTransform( );
	virtual _void			PopViewTransform( );

	virtual _void			SetProjectionTransform( const Matrix4& matrix );
	virtual const Matrix4&	GetProjectionTransform( ) const;
	virtual _void			PushProjectionTransform( );
	virtual _void			PopProjectionTransform( );

	virtual _void			SetTextureTransform( _dword index, const Matrix4& matrix );
	virtual const Matrix4&	GetTextureTransform( _dword index ) const;
	virtual _void			PushTextureTransform( _dword index );
	virtual _void			PopTextureTransform( _dword index );

	virtual _void			SetCamera( const Camera& camera );
	virtual const Camera&	GetCamera( ) const;
	virtual _void			PushCamera( );
	virtual _void			PopCamera( );

	virtual _void			SetViewport( const Viewport& viewport );
	virtual const Viewport&	GetViewport( ) const;
	virtual _void			PushViewport( );
	virtual _void			PopViewport( );

	virtual const Matrix4&	GetViewMatrix( ) const;
	virtual const Frustum&	GetViewFrustum( ) const;

	virtual Ray				GetPickRay( _long x, _long y, _bool ortho ) const;
	virtual Matrix4			GetClip2ImageMatrix( const Matrix4& view, const Matrix4& projection ) const;
	virtual Vector2			ProjectPoint2Screen( const Vector3& point ) const;
	virtual Region			ProjectBox2Screen( const AxisAlignedBox& boundbox ) const;

	virtual _dword			GetRenderTargetFormat( ) const;
	virtual ITexture*		GetRenderTargetTexture( ) const;
	virtual ISurface*		GetRenderTargetSurface( ) const;

	virtual _dword			GetDepthStencilFormat( ) const;
	virtual ITexture*		GetDepthStencilTexture( ) const;
	virtual ISurface*		GetDepthStencilSurface( ) const;

	virtual _void			SetRenderTexture( ITexture* rendertarget, ITexture* depthstencil );
	virtual _void			SetRenderSurface( ISurface* rendertarget, ISurface* depthstencil );
	virtual _void			ResetRenderTarget( );

	virtual _void			SetRenderMethod( IRenderMethod* method );
	virtual IRenderMethod*	GetRenderMethod( ) const;
	virtual _void			SetSamplerMethod( ISamplerMethod* method );
	virtual ISamplerMethod*	GetSamplerMethod( ) const;
	virtual _void			SetTextureMethod( ITextureMethod* method );
	virtual ITextureMethod*	GetTextureMethod( ) const;

	virtual _void			SetGlobalRenderMethod( IRenderMethod* method );
	virtual IRenderMethod*	GetGlobalRenderMethod( ) const;
	virtual _void			SetGlobalSamplerMethod( ISamplerMethod* method );
	virtual ISamplerMethod*	GetGlobalSamplerMethod( ) const;
	virtual _void			SetGlobalTextureMethod( ITextureMethod* method );
	virtual ITextureMethod*	GetGlobalTextureMethod( ) const;

	virtual _void			SetShaderConst( IShaderConst* shaderconst );
	virtual IShaderConst*	GetShaderConst( ) const;

	virtual _void			PushAllRenderMethod( );
	virtual _void			PopAllRenderMethod( );

	virtual _void			SetRenderLayer( _dword layer );
	virtual _void			ResetRenderLayer( );
	virtual _void			BeginSubRenderLayer( _dword layer );
	virtual _void			EndSubRenderLayer( );

	virtual _void			Render( const Geometry& geometry );
	virtual _void			Render( const Geometry& geometry, const Matrix4& transform );

	virtual _void			Render( const OverlayPoint& point );
	virtual _void			Render( const OverlayLine& line );
	virtual _void			Render( const OverlayTriangle& triangle );
	virtual _void			Render( const OverlayTriangleImage& triangleimage );
	virtual _void			Render( const OverlayRectangle& rectangle );
	virtual _void			Render( const OverlayRectangleImage& rectangleimage );
	virtual _void			Render( const OverlayQuadrangle& quadrangle );
	virtual _void			Render( const OverlayQuadrangleImage& quadrangleimage );
	virtual _void			Render( const OverlayRectangleMask& rectanglemask );

	virtual _void			Render( const PrimitivePoint& point );
	virtual _void			Render( const PrimitiveLine& line );
	virtual _void			Render( const PrimitiveTriangle& triangle );
	virtual _void			Render( const PrimitiveTriangleImage& triangleimage );
	virtual _void			Render( const PrimitiveQuadrangle& quadrangle );
	virtual _void			Render( const PrimitiveQuadrangleImage& quadrangleimage );
	virtual _void			Render( const PrimitiveBox& box );
	virtual _void			Render( const PrimitiveCircle& circle );
	virtual _void			Render( const PrimitiveSphere& sphere );
	virtual _void			Render( const PrimitivePyramid& pyramid );
	virtual _void			Render( const PrimitiveAxis& axis );
	virtual _void			Render( const PrimitiveFrustum& frustum );

	virtual _void			Render( const BillboardScreenAligned& screenaligned );
	virtual _void			Render( const BillboardWorldOriented& worldoriented );
	virtual _void			Render( const BillboardAxial& axial );

	virtual _void			Render( IFontTexture* font, const Vector2& position, _char character );
	virtual _void			Render( IFontTexture* font, const Vector2& position, _dword align, const _char* format, ... );
	virtual _void			Render( IFontTexture* font, const Vector3& center, _float height, _char character );
	virtual _void			Render( IFontTexture* font, const Vector3& center, _float height, const _char* format, ... );
	virtual _void			Render( IFontTexture* font, const Vector3& center, const Vector3& width, const Vector3& height, _char character );
	virtual _void			Render( IFontTexture* font, const Vector3& center, const Vector3& width, const Vector3& height, const _char* format, ... );

	virtual _void			Render( IMesh* mesh );
	virtual _void			Render( ISkeleton* skeleton, _dword parentcolor, _dword childcolor, _dword dummycolor );
	virtual _void			Render( IParticleSystem* particlesystem );

	virtual _void			Render( IRenderSet* renderset );
};

};
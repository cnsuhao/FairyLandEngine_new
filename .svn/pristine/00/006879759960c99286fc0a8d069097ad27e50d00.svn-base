//============================================================================
// IRenderDevice.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// IRenderDevice
//----------------------------------------------------------------------------

class IRenderDevice
{
public:
	enum _DEVICE_TYPE
	{
		_DEVICE_HARDWARE_NOVSYNC	= 0,
		_DEVICE_HARDWARE_VSYNC		= 1,
		_DEVICE_HARDWARE_REF		= 2,
		_DEVICE_HARDWARE_PERFHUD	= 3,

		_DEVICE_MAX					= 4,

		_DEVICE_MASK				= 0x000000FF,

		_DEVICE_OPENGL				= 0x40000000,
		_DEVICE_FPU_PRESERVE		= 0x80000000,
	};

	enum _RENDER_LAYER
	{
		_LAYER_BACKGROUND			= 1,
		_LAYER_ENVIRONMENT			= 2,
		_LAYER_NORMAL				= 3,
		_LAYER_FOREGROUND			= 4,
		_LAYER_OVERLAY				= 5,
	};

	virtual _bool			IsDeviceLost( ) = 0;
	virtual _bool			ResizeDevice( _dword width, _dword height ) = 0;
	virtual _bool			ResetDevice( ) = 0;

	virtual _handle			GetDeviceHandle( ) const = 0;
	virtual _dword			GetDeviceWidth( ) const = 0;
	virtual _dword			GetDeviceHeight( ) const = 0;

	virtual _void			SetClearColor( _dword color ) = 0;
	virtual _void			SetClearDepth( _float depth ) = 0;
	virtual _void			SetClearStencil( _dword stencil ) = 0;

	virtual _void			ClearBuffer( _bool color = _true, _bool depth = _true, _bool stencil = _true ) = 0;
	virtual _void			Flush( ) = 0;
	virtual _void			Present( _handle targetwindow = _null ) = 0;

	virtual _void			CombineOverlayClipRegion( const Region& clipregion ) = 0;
	virtual _void			SetOverlayClipRegion( const Region& clipregion ) = 0;
	virtual const Region&	GetOverlayClipRegion( ) const = 0;

	virtual _void			SetOverlayTransform( const Matrix3& matrix ) = 0;
	virtual const Matrix3&	GetOverlayTransform( ) const = 0;
	virtual _void			PushOverlayTransform( ) = 0;
	virtual _void			PopOverlayTransform( ) = 0;

	virtual _void			SetWorldTransform( const Matrix4& matrix ) = 0;
	virtual const Matrix4&	GetWorldTransform( ) const = 0;
	virtual const Matrix3&	GetNormalTransform( ) const = 0;
	virtual _float			GetScaleTransform( ) const = 0;
	virtual _void			PushWorldTransform( ) = 0;
	virtual _void			PopWorldTransform( ) = 0;

	virtual _void			SetViewTransform( const Matrix4& matrix ) = 0;
	virtual const Matrix4&	GetViewTransform( ) const = 0;
	virtual _void			PushViewTransform( ) = 0;
	virtual _void			PopViewTransform( ) = 0;

	virtual _void			SetProjectionTransform( const Matrix4& matrix ) = 0;
	virtual const Matrix4&	GetProjectionTransform( ) const = 0;
	virtual _void			PushProjectionTransform( ) = 0;
	virtual _void			PopProjectionTransform( ) = 0;

	virtual _void			SetTextureTransform( _dword index, const Matrix4& matrix ) = 0;
	virtual const Matrix4&	GetTextureTransform( _dword index ) const = 0;
	virtual _void			PushTextureTransform( _dword index ) = 0;
	virtual _void			PopTextureTransform( _dword index ) = 0;

	virtual _void			SetCamera( const Camera& camera ) = 0;
	virtual const Camera&	GetCamera( ) const = 0;
	virtual _void			PushCamera( ) = 0;
	virtual _void			PopCamera( ) = 0;

	virtual _void			SetViewport( const Viewport& viewport ) = 0;
	virtual const Viewport&	GetViewport( ) const = 0;
	virtual _void			PushViewport( ) = 0;
	virtual _void			PopViewport( ) = 0;

	virtual const Matrix4&	GetViewMatrix( ) const = 0;
	virtual const Frustum&	GetViewFrustum( ) const = 0;

	virtual Ray				GetPickRay( _long x, _long y, _bool ortho = _false ) const = 0;
	virtual Matrix4			GetClip2ImageMatrix( const Matrix4& view, const Matrix4& projection ) const = 0;
	virtual Vector2			ProjectPoint2Screen( const Vector3& point ) const = 0;
	virtual Region			ProjectBox2Screen( const AxisAlignedBox& boundbox ) const = 0;

	virtual _dword			GetRenderTargetFormat( ) const = 0;
	virtual ITexture*		GetRenderTargetTexture( ) const = 0;
	virtual ISurface*		GetRenderTargetSurface( ) const = 0;

	virtual _dword			GetDepthStencilFormat( ) const = 0;
	virtual ITexture*		GetDepthStencilTexture( ) const = 0;
	virtual ISurface*		GetDepthStencilSurface( ) const = 0;

	virtual _void			SetRenderTexture( ITexture* rendertarget, ITexture* depthstencil ) = 0;
	virtual _void			SetRenderSurface( ISurface* rendertarget, ISurface* depthstencil ) = 0;
	virtual _void			ResetRenderTarget( ) = 0;

	virtual _void			SetRenderMethod( IRenderMethod* method ) = 0;
	virtual IRenderMethod*	GetRenderMethod( ) const = 0;
	virtual _void			SetSamplerMethod( ISamplerMethod* method ) = 0;
	virtual ISamplerMethod*	GetSamplerMethod( ) const = 0;
	virtual _void			SetTextureMethod( ITextureMethod* method ) = 0;
	virtual ITextureMethod*	GetTextureMethod( ) const = 0;

	virtual _void			SetGlobalRenderMethod( IRenderMethod* method ) = 0;
	virtual IRenderMethod*	GetGlobalRenderMethod( ) const = 0;
	virtual _void			SetGlobalSamplerMethod( ISamplerMethod* method ) = 0;
	virtual ISamplerMethod*	GetGlobalSamplerMethod( ) const = 0;
	virtual _void			SetGlobalTextureMethod( ITextureMethod* method ) = 0;
	virtual ITextureMethod*	GetGlobalTextureMethod( ) const = 0;

	virtual _void			SetShaderConst( IShaderConst* shaderconst ) = 0;
	virtual IShaderConst*	GetShaderConst( ) const = 0;

	virtual _void			PushAllRenderMethod( ) = 0;
	virtual _void			PopAllRenderMethod( ) = 0;

	virtual _void			SetRenderLayer( _dword layer ) = 0;
	virtual _void			ResetRenderLayer( ) = 0;
	virtual _void			BeginSubRenderLayer( _dword layer ) = 0;
	virtual _void			EndSubRenderLayer( ) = 0;

	virtual _void			Render( const Geometry& geometry ) = 0;
	virtual _void			Render( const Geometry& geometry, const Matrix4& transform ) = 0;

	virtual _void			Render( const OverlayPoint& point ) = 0;
	virtual _void			Render( const OverlayLine& line ) = 0;
	virtual _void			Render( const OverlayTriangle& triangle ) = 0;
	virtual _void			Render( const OverlayTriangleImage& triangleimage ) = 0;
	virtual _void			Render( const OverlayRectangle& rectangle ) = 0;
	virtual _void			Render( const OverlayRectangleImage& rectangleimage ) = 0;
	virtual _void			Render( const OverlayQuadrangle& quadrangle ) = 0;
	virtual _void			Render( const OverlayQuadrangleImage& quadrangleimage ) = 0;
	virtual _void			Render( const OverlayRectangleMask& rectanglemask ) = 0;

	virtual _void			Render( const PrimitivePoint& point ) = 0;
	virtual _void			Render( const PrimitiveLine& line ) = 0;
	virtual _void			Render( const PrimitiveTriangle& triangle ) = 0;
	virtual _void			Render( const PrimitiveTriangleImage& triangleimage ) = 0;
	virtual _void			Render( const PrimitiveQuadrangle& quadrangle ) = 0;
	virtual _void			Render( const PrimitiveQuadrangleImage& quadrangleimage ) = 0;
	virtual _void			Render( const PrimitiveBox& box ) = 0;
	virtual _void			Render( const PrimitiveCircle& circle ) = 0;
	virtual _void			Render( const PrimitiveSphere& sphere ) = 0;
	virtual _void			Render( const PrimitivePyramid& pyramid ) = 0;
	virtual _void			Render( const PrimitiveAxis& axis ) = 0;
	virtual _void			Render( const PrimitiveFrustum& frustum ) = 0;

	virtual _void			Render( const BillboardScreenAligned& screenaligned ) = 0;
	virtual _void			Render( const BillboardWorldOriented& worldoriented ) = 0;
	virtual _void			Render( const BillboardAxial& axial ) = 0;

	virtual _void			Render( IFontTexture* font, const Vector2& position, _char character ) = 0;
	virtual _void			Render( IFontTexture* font, const Vector2& position, _dword align, const _char* format, ... ) = 0;
	virtual _void			Render( IFontTexture* font, const Vector3& center, _float height, _char character ) = 0;
	virtual _void			Render( IFontTexture* font, const Vector3& center, _float height, const _char* format, ... ) = 0;
	virtual _void			Render( IFontTexture* font, const Vector3& center, const Vector3& width, const Vector3& height, _char character ) = 0;
	virtual _void			Render( IFontTexture* font, const Vector3& center, const Vector3& width, const Vector3& height, const _char* format, ... ) = 0;

	virtual _void			Render( IMesh* mesh ) = 0;
	virtual _void			Render( ISkeleton* skeleton, _dword parentcolor, _dword childcolor, _dword dummycolor ) = 0;
	virtual _void			Render( IParticleSystem* particlesystem ) = 0;

	virtual _void			Render( IRenderSet* renderset ) = 0;
};

}; 
//============================================================================
// RenderSet.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// RenderSet
//----------------------------------------------------------------------------

class RenderSet : public IRenderSet, public ReferencedObject
{
private:
	struct OverlayVertexNoneTex
	{
		Vector4	mPosition;
		_dword	mColor;
	};

	struct OverlayVertexWithTex
	{
		Vector4	mPosition;
		_dword	mColor;
		Vector2	mTexcoord;
	};

	struct PrimitiveVertexNoneTex
	{
		Vector3	mPosition;
		_dword	mColor;
	};

	struct PrimitiveVertexWithTex
	{
		Vector3	mPosition;
		_dword	mColor;
		Vector2	mTexcoord;
	};

	struct BillboardVertex
	{
		Vector3	mPosition;
		_dword	mColor;
		Vector2	mTexcoord;
	};

	ISurface*				mRenderTarget;
	ISurface*				mDepthStencil;

	IVertexDeclaration*		mOverlayNoneTexVD;
	IVertexDeclaration*		mOverlayWithTexVD;
	IVertexDeclaration*		mPrimitiveNoneTexVD;
	IVertexDeclaration*		mPrimitiveWithTexVD;
	IVertexDeclaration*		mBillboardVD;

	IVertexBuffer*			mOverlayNoneTexVB;
	IVertexBuffer*			mOverlayWithTexVB;
	IVertexBuffer*			mPrimitiveNoneTexVB;
	IVertexBuffer*			mPrimitiveWithTexVB;
	IVertexBuffer*			mBillboardVB;

	Allocater				mRenderUnitAllocater;
	RenderUnitArray			mRenderUnitArray;
	RenderToken				mRenderToken;
	DeviceToken				mDeviceToken;

	_dword					mDefaultLayer;

	_void	CreateOverlayNoneTexVB( );
	_void	CreateOverlayWithTexVB( );
	_void	CreatePrimitiveNoneTexVB( );
	_void	CreatePrimitiveWithTexVB( );
	_void	CreateBillboardVB( );

	_void	SetupEnvironment( const Matrix4& worldtransform, _bool needtransform );
	_bool	CompareRenderable( const Renderable* oldrenderable, const Renderable* newrenderable );
	_bool	CombineRenderUnit( const RenderUnit& renderunit );
	_void	AppendRenderUnit( Renderable* renderable, const TechniqueUnit& technique, const Matrix4& worldtransform, _bool needtransform, _dword layer, _bool alpha, const Vector3& center );
	_void	ReleaseRenderUnit( RenderUnit& renderunit );

public:
	RenderSet( );
	~RenderSet( );

	inline RenderUnitArray& GetRenderUnitArray( )
		{ return mRenderUnitArray; }

	inline _void SetRenderTarget( ISurface* rendertarget, ISurface* depthstencil )
		{ mRenderTarget = rendertarget; mDepthStencil = depthstencil; }

	inline _bool CheckRenderTarget( ISurface* rendertarget, ISurface* depthstencil ) const
		{ return mRenderTarget == rendertarget && mDepthStencil == depthstencil; }

	inline _dword GetDefaultLayer( ) const
		{ return mDefaultLayer == 0 ? IRenderDevice::_LAYER_NORMAL : mDefaultLayer; }

	inline _dword GetOverlayLayer( ) const
		{ return mDefaultLayer == 0 ? IRenderDevice::_LAYER_OVERLAY : mDefaultLayer; }

	inline _bool IsAlphaColor( _dword color )
		{ return ( color & 0xFF000000 ) != 0xFF000000; }

	_void	Render( const Geometry& geometry, const TechniqueUnit& technique, const Matrix4& worldtransform );

	_void	Render( const OverlayPoint& point, const TechniqueUnit& technique );
	_void	Render( const OverlayLine& line, const TechniqueUnit& technique );
	_void	Render( const OverlayTriangle& triangle, const TechniqueUnit& technique );
	_void	Render( const OverlayTriangleImage& triangleimage, const TechniqueUnit& technique );
	_void	Render( const OverlayRectangle& rectangle, const TechniqueUnit& technique );
	_void	Render( const OverlayRectangleImage& rectangleimage, const TechniqueUnit& technique );
	_void	Render( const OverlayQuadrangle& quadrangle, const TechniqueUnit& technique );
	_void	Render( const OverlayQuadrangleImage& quadrangleimage, const TechniqueUnit& technique );

	_void	Render( const PrimitivePoint& point, const TechniqueUnit& technique );
	_void	Render( const PrimitiveLine& line, const TechniqueUnit& technique );
	_void	Render( const PrimitiveTriangle& triangle, const TechniqueUnit& technique );
	_void	Render( const PrimitiveTriangleImage& triangleimage, const TechniqueUnit& technique );
	_void	Render( const PrimitiveQuadrangle& quadrangle, const TechniqueUnit& technique );
	_void	Render( const PrimitiveQuadrangleImage& quadrangleimage, const TechniqueUnit& technique );
	_void	Render( const PrimitiveBox& box, const TechniqueUnit& technique );
	_void	Render( const PrimitiveCircle& circle, const TechniqueUnit& technique );
	_void	Render( const PrimitiveSphere& sphere, const TechniqueUnit& technique );
	_void	Render( const PrimitivePyramid& pyramid, const TechniqueUnit& technique );
	_void	Render( const PrimitiveAxis& axis, const TechniqueUnit& technique );
	_void	Render( const PrimitiveFrustum& frustum, const TechniqueUnit& technique );

	_void	Render( const BillboardScreenAligned& screenaligned, const TechniqueUnit& technique );
	_void	Render( const BillboardWorldOriented& worldoriented, const TechniqueUnit& technique );
	_void	Render( const BillboardAxial& axial, const TechniqueUnit& technique );

	_void	SetRenderLayer( _dword layer );
	_void	ResetRenderLayer( );
	_void	BeginSubRenderLayer( _dword layer );
	_void	EndSubRenderLayer( );

	_void	Prepare( );
	_void	Finish( );

	virtual _void	BeginRecord( );
	virtual _void	EndRecord( );
	virtual _void	ClearRecord( );
};

};
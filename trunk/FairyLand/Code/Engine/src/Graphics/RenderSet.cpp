//============================================================================
// RenderSet.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandGraphics.h"

//----------------------------------------------------------------------------
// RenderSet Implementation
//----------------------------------------------------------------------------

RenderSet::RenderSet( )
{
	mRenderTarget			= _null;
	mDepthStencil			= _null;

	mOverlayNoneTexVD		= _null;
	mOverlayWithTexVD		= _null;
	mPrimitiveNoneTexVD		= _null;
	mPrimitiveWithTexVD		= _null;
	mBillboardVD			= _null;

	mOverlayNoneTexVB		= _null;
	mOverlayWithTexVB		= _null;
	mPrimitiveNoneTexVB		= _null;
	mPrimitiveWithTexVB		= _null;
	mBillboardVB			= _null;

	mDefaultLayer			= 0;

	IncreaseRefCount( );
}

RenderSet::~RenderSet( )
{
	Finish( );

	GetGeometryFactory( ).ReleaseVertexDeclaration( mOverlayNoneTexVD );
	GetGeometryFactory( ).ReleaseVertexDeclaration( mOverlayWithTexVD );
	GetGeometryFactory( ).ReleaseVertexDeclaration( mPrimitiveNoneTexVD );
	GetGeometryFactory( ).ReleaseVertexDeclaration( mPrimitiveWithTexVD );
	GetGeometryFactory( ).ReleaseVertexDeclaration( mBillboardVD );

	GetGeometryFactory( ).ReleaseVertexBuffer( mOverlayNoneTexVB );
	GetGeometryFactory( ).ReleaseVertexBuffer( mOverlayWithTexVB );
	GetGeometryFactory( ).ReleaseVertexBuffer( mPrimitiveNoneTexVB );
	GetGeometryFactory( ).ReleaseVertexBuffer( mPrimitiveWithTexVB );
	GetGeometryFactory( ).ReleaseVertexBuffer( mBillboardVB );
}

_void RenderSet::CreateOverlayNoneTexVB( )
{
	// If there is no vertex declaration for position and color, create it.
	if ( mOverlayNoneTexVD == _null )
	{
		mOverlayNoneTexVD = GetGeometryFactory( ).CreateVertexDeclaration( );

		if ( mOverlayNoneTexVD != _null )
		{
			mOverlayNoneTexVD->AddVertexElement( 0, 0, IVertexDeclaration::_TYPE_FLOAT4, IVertexDeclaration::_USAGE_POSITIONT, 0 );
			mOverlayNoneTexVD->AddVertexElement( 0, 16, IVertexDeclaration::_TYPE_COLOR, IVertexDeclaration::_USAGE_DIFFUSE, 0 );
		}
	}

	// Create vertex buffer when necessary.
	if ( mOverlayNoneTexVB == _null || mOverlayNoneTexVB->GetSize( ) < sizeof( OverlayVertexNoneTex ) * mRenderToken.mOverlayNoneTexVN )
	{
		// Release the old buffer.
		if ( mOverlayNoneTexVB != _null )
			GetGeometryFactory( ).ReleaseVertexBuffer( mOverlayNoneTexVB );

		// Create the new buffer.
		mOverlayNoneTexVB = GetGeometryFactory( ).CreateVertexBuffer(
			IGeometryFactory::_TYPE_STATIC,
			sizeof( OverlayVertexNoneTex ) * mRenderToken.mOverlayNoneTexVN );
	}
}

_void RenderSet::CreateOverlayWithTexVB( )
{
	// If there is no vertex declaration, create it.
	if ( mOverlayWithTexVD == _null )
	{
		mOverlayWithTexVD = GetGeometryFactory( ).CreateVertexDeclaration( );

		if ( mOverlayWithTexVD != _null )
		{
			mOverlayWithTexVD->AddVertexElement( 0, 0, IVertexDeclaration::_TYPE_FLOAT4, IVertexDeclaration::_USAGE_POSITIONT, 0 );
			mOverlayWithTexVD->AddVertexElement( 0, 16, IVertexDeclaration::_TYPE_COLOR, IVertexDeclaration::_USAGE_DIFFUSE, 0 );
			mOverlayWithTexVD->AddVertexElement( 0, 20, IVertexDeclaration::_TYPE_FLOAT2, IVertexDeclaration::_USAGE_TEXCOORD, 0 );
		}
	}

	// Create vertex buffer when necessary.
	if ( mOverlayWithTexVB == _null || mOverlayWithTexVB->GetSize( ) < sizeof( OverlayVertexWithTex ) * mRenderToken.mOverlayWithTexVN )
	{
		// Release the old buffer.
		if ( mOverlayWithTexVB != _null )
			GetGeometryFactory( ).ReleaseVertexBuffer( mOverlayWithTexVB );

		// Create the new buffer.
		mOverlayWithTexVB = GetGeometryFactory( ).CreateVertexBuffer(
			IGeometryFactory::_TYPE_STATIC, sizeof( OverlayVertexWithTex ) * mRenderToken.mOverlayWithTexVN );
	}
}

_void RenderSet::CreatePrimitiveNoneTexVB( )
{
	// If there is no vertex declaration, create it.
	if ( mPrimitiveNoneTexVD == _null )
	{
		mPrimitiveNoneTexVD = GetGeometryFactory( ).CreateVertexDeclaration( );

		if ( mPrimitiveNoneTexVD != _null )
		{
			mPrimitiveNoneTexVD->AddVertexElement( 0, 0, IVertexDeclaration::_TYPE_FLOAT3, IVertexDeclaration::_USAGE_POSITION, 0 );
			mPrimitiveNoneTexVD->AddVertexElement( 0, 12, IVertexDeclaration::_TYPE_COLOR, IVertexDeclaration::_USAGE_DIFFUSE, 0 );
		}
	}

	// Create vertex buffer when necessary.
	if ( mPrimitiveNoneTexVB == _null || mPrimitiveNoneTexVB->GetSize( ) < sizeof( PrimitiveVertexNoneTex ) * mRenderToken.mPrimitiveNoneTexVN )
	{
		// Release the old buffer.
		if ( mPrimitiveNoneTexVB != _null )
			GetGeometryFactory( ).ReleaseVertexBuffer( mPrimitiveNoneTexVB );

		// Create the new buffer.
		mPrimitiveNoneTexVB = GetGeometryFactory( ).CreateVertexBuffer(
			IGeometryFactory::_TYPE_STATIC, sizeof( PrimitiveVertexNoneTex ) * mRenderToken.mPrimitiveNoneTexVN );
	}
}

_void RenderSet::CreatePrimitiveWithTexVB( )
{
	// If there is no vertex declaration, create it.
	if ( mPrimitiveWithTexVD == _null )
	{
		mPrimitiveWithTexVD = GetGeometryFactory( ).CreateVertexDeclaration( );

		if ( mPrimitiveWithTexVD != _null )
		{
			mPrimitiveWithTexVD->AddVertexElement( 0, 0, IVertexDeclaration::_TYPE_FLOAT3, IVertexDeclaration::_USAGE_POSITION, 0 );
			mPrimitiveWithTexVD->AddVertexElement( 0, 12, IVertexDeclaration::_TYPE_COLOR, IVertexDeclaration::_USAGE_DIFFUSE, 0 );
			mPrimitiveWithTexVD->AddVertexElement( 0, 16, IVertexDeclaration::_TYPE_FLOAT2, IVertexDeclaration::_USAGE_TEXCOORD, 0 );
		}
	}

	// Create vertex buffer when necessary.
	if ( mPrimitiveWithTexVB == _null || mPrimitiveWithTexVB->GetSize( ) < sizeof( PrimitiveVertexWithTex ) * mRenderToken.mPrimitiveWithTexVN )
	{
		// Release the old buffer.
		if ( mPrimitiveWithTexVB != _null )
			GetGeometryFactory( ).ReleaseVertexBuffer( mPrimitiveWithTexVB );

		// Create the new buffer.
		mPrimitiveWithTexVB = GetGeometryFactory( ).CreateVertexBuffer(
			IGeometryFactory::_TYPE_STATIC, sizeof( PrimitiveVertexWithTex ) * mRenderToken.mPrimitiveWithTexVN );
	}
}

_void RenderSet::CreateBillboardVB( )
{
	// If there is no vertex declaration, create it.
	if ( mBillboardVD == _null )
	{
		mBillboardVD = GetGeometryFactory( ).CreateVertexDeclaration( );

		if ( mBillboardVD != _null )
		{
			mBillboardVD->AddVertexElement( 0, 0, IVertexDeclaration::_TYPE_FLOAT3, IVertexDeclaration::_USAGE_POSITION, 0 );
			mBillboardVD->AddVertexElement( 0, 12, IVertexDeclaration::_TYPE_COLOR, IVertexDeclaration::_USAGE_DIFFUSE, 0 );
			mBillboardVD->AddVertexElement( 0, 16, IVertexDeclaration::_TYPE_FLOAT2, IVertexDeclaration::_USAGE_TEXCOORD, 0 );
		}
	}

	// Create vertex buffer when necessary.
	if ( mBillboardVB == _null || mBillboardVB->GetSize( ) < sizeof( BillboardVertex ) * mRenderToken.mBillboardVN )
	{
		// Release the old buffer.
		if ( mBillboardVB != _null )
			GetGeometryFactory( ).ReleaseVertexBuffer( mBillboardVB );

		// Create the new buffer.
		mBillboardVB = GetGeometryFactory( ).CreateVertexBuffer(
			IGeometryFactory::_TYPE_STATIC, sizeof( BillboardVertex ) * mRenderToken.mBillboardVN );
	}
}

_void RenderSet::SetupEnvironment( const Matrix4& worldtransform, _bool needtransform )
{
	RenderDevice& renderdevice = (RenderDevice&) GetRenderDevice( );

	const DeviceToken& dt = renderdevice.GetDeviceToken( );

	if ( needtransform )
	{
		// Update world transform.
		if ( &worldtransform != &Matrix4::cIdentity )
		{
			mRenderToken.mWorldTransform = (Matrix4*) mRenderUnitAllocater.Allocate(
				&( worldtransform * renderdevice.GetWorldTransform( ) ), sizeof( Matrix4 ) );
		}
		if ( mRenderToken.mWorldTransform == _null || mDeviceToken.mWorldTransformToken != dt.mWorldTransformToken )
		{
			mRenderToken.mWorldTransform = (Matrix4*) mRenderUnitAllocater.Allocate(
				&( renderdevice.GetWorldTransform( ) ), sizeof( Matrix4 ) );

			mDeviceToken.mWorldTransformToken = dt.mWorldTransformToken;
		}
	}

	// Update view transform.
	if ( mRenderToken.mViewTransform == _null || mDeviceToken.mViewTransformToken != dt.mViewTransformToken )
	{
		mRenderToken.mViewTransform = (Matrix4*) mRenderUnitAllocater.Allocate(
			&( renderdevice.GetViewTransform( ) ), sizeof( Matrix4 ) );

		mDeviceToken.mViewTransformToken = dt.mViewTransformToken;
	}

	// Update projection transform.
	if ( mRenderToken.mProjectionTransform == _null || mDeviceToken.mProjectionTransformToken != dt.mProjectionTransformToken )
	{
		mRenderToken.mProjectionTransform = (Matrix4*) mRenderUnitAllocater.Allocate(
			&( renderdevice.GetProjectionTransform( ) ), sizeof( Matrix4 ) );

		mDeviceToken.mProjectionTransformToken = dt.mProjectionTransformToken;
	}

	for ( _dword i = 0; i < Geometry::_MAX_TEXTURE_NUMBER; i ++ )
	{
		// Update texture transform.
		if ( mRenderToken.mTextureTransform[i] == _null || mDeviceToken.mTextureTransformToken[i] != dt.mTextureTransformToken[i] )
		{
			mRenderToken.mTextureTransform[i] = (Matrix4*) mRenderUnitAllocater.Allocate(
				&( renderdevice.GetTextureTransform( i ) ), sizeof( Matrix4 ) );

			mDeviceToken.mTextureTransformToken[i] = dt.mTextureTransformToken[i];
		}
	}

	// Update View port.
	if ( mRenderToken.mViewport == _null || mDeviceToken.mViewportToken != dt.mViewportToken )
	{
		mRenderToken.mViewport = (Viewport*) mRenderUnitAllocater.Allocate(
			&( renderdevice.GetViewport( ) ), sizeof( Viewport ) );

		mDeviceToken.mViewportToken = dt.mViewportToken;
	}
}

_bool RenderSet::CompareRenderable( const Renderable* oldrenderable, const Renderable* newrenderable )
{
	if ( oldrenderable->mType != newrenderable->mType )
		return _false;

	switch ( oldrenderable->mType )
	{
		case Renderable::_TYPE_OVERLAY_POINT:
		{
			OverlayPoint* point1 = (OverlayPoint*) oldrenderable;
			OverlayPoint* point2 = (OverlayPoint*) newrenderable;

			return	point1->mColor == point2->mColor &&
					point1->mPosition.Equal( point2->mPosition );
		}

		case Renderable::_TYPE_OVERLAY_LINE:
		{
			OverlayLine* line1 = (OverlayLine*) oldrenderable;
			OverlayLine* line2 = (OverlayLine*) newrenderable;

			return	line1->mColor1 == line2->mColor1 &&
					line1->mColor2 == line2->mColor2 &&
					line1->mPosition1.Equal( line2->mPosition1 ) &&
					line1->mPosition2.Equal( line2->mPosition2 );
		}

		case Renderable::_TYPE_OVERLAY_TRIANGLE:
		{
			OverlayTriangle* triangle1 = (OverlayTriangle*) oldrenderable;
			OverlayTriangle* triangle2 = (OverlayTriangle*) newrenderable;

			return	triangle1->mColor1 == triangle2->mColor1 &&
					triangle1->mColor2 == triangle2->mColor2 &&
					triangle1->mColor3 == triangle2->mColor3 &&
					triangle1->mPosition1.Equal( triangle2->mPosition1 ) &&
					triangle1->mPosition2.Equal( triangle2->mPosition2 ) &&
					triangle1->mPosition3.Equal( triangle2->mPosition3 );
		}

		case Renderable::_TYPE_OVERLAY_RECTANGLE:
		{
			OverlayRectangle* rectangle1 = (OverlayRectangle*) oldrenderable;
			OverlayRectangle* rectangle2 = (OverlayRectangle*) newrenderable;

			return	rectangle1->mColor == rectangle2->mColor &&
					rectangle1->mPositionLT.Equal( rectangle2->mPositionLT ) &&
					rectangle1->mPositionRB.Equal( rectangle2->mPositionRB );
		}

		case Renderable::_TYPE_OVERLAY_QUADRANGLE:
		{
			OverlayQuadrangle* quadrangle1 = (OverlayQuadrangle*) oldrenderable;
			OverlayQuadrangle* quadrangle2 = (OverlayQuadrangle*) newrenderable;

			return	quadrangle1->mColorLT == quadrangle2->mColorLT &&
					quadrangle1->mColorRT == quadrangle2->mColorRT &&
					quadrangle1->mColorLB == quadrangle2->mColorLB &&
					quadrangle1->mColorRB == quadrangle2->mColorRB &&
					quadrangle1->mPositionLT.Equal( quadrangle2->mPositionLT ) &&
					quadrangle1->mPositionRT.Equal( quadrangle2->mPositionRT ) &&
					quadrangle1->mPositionLB.Equal( quadrangle2->mPositionLB ) &&
					quadrangle1->mPositionRB.Equal( quadrangle2->mPositionRB );
		}

		case Renderable::_TYPE_OVERLAY_RECTANGLEMASK:
		{
			OverlayRectangleMask* rectanglemask1 = (OverlayRectangleMask*) oldrenderable;
			OverlayRectangleMask* rectanglemask2 = (OverlayRectangleMask*) newrenderable;

			return	Math::Abs( rectanglemask1->mPercentage - rectanglemask2->mPercentage ) < Math::cEpsilon &&
					rectanglemask1->mColor == rectanglemask2->mColor &&
					rectanglemask1->mPositionLT.Equal( rectanglemask2->mPositionLT ) &&
					rectanglemask1->mPositionRB.Equal( rectanglemask2->mPositionRB );
		}

		case Renderable::_TYPE_OVERLAY_TRIANGLEIMAGE:
		{
			OverlayTriangleImage* triangle1 = (OverlayTriangleImage*) oldrenderable;
			OverlayTriangleImage* triangle2 = (OverlayTriangleImage*) newrenderable;

			return	triangle1->mTexture == triangle2->mTexture &&
					triangle1->mColor1 == triangle2->mColor1 &&
					triangle1->mColor2 == triangle2->mColor2 &&
					triangle1->mColor3 == triangle2->mColor3 &&
					triangle1->mPosition1.Equal( triangle2->mPosition1 ) &&
					triangle1->mPosition2.Equal( triangle2->mPosition2 ) &&
					triangle1->mPosition3.Equal( triangle2->mPosition3 ) &&
					triangle1->mTexcoord1.Equal( triangle2->mTexcoord1 ) &&
					triangle1->mTexcoord2.Equal( triangle2->mTexcoord2 ) &&
					triangle1->mTexcoord3.Equal( triangle2->mTexcoord3 );
		}

		case Renderable::_TYPE_OVERLAY_RECTANGLEIMAGE:
		{
			OverlayRectangleImage* rectangle1 = (OverlayRectangleImage*) oldrenderable;
			OverlayRectangleImage* rectangle2 = (OverlayRectangleImage*) newrenderable;

			return	rectangle1->mTexture == rectangle2->mTexture &&
					rectangle1->mColor == rectangle2->mColor &&
					rectangle1->mPositionLT.Equal( rectangle2->mPositionLT ) &&
					rectangle1->mPositionRB.Equal( rectangle2->mPositionRB ) &&
					rectangle1->mTexcoordLT.Equal( rectangle2->mTexcoordLT ) &&
					rectangle1->mTexcoordRB.Equal( rectangle2->mTexcoordRB );
		}

		case Renderable::_TYPE_OVERLAY_QUADRANGLEIMAGE:
		{
			OverlayQuadrangleImage* quadrangle1 = (OverlayQuadrangleImage*) oldrenderable;
			OverlayQuadrangleImage* quadrangle2 = (OverlayQuadrangleImage*) newrenderable;

			return	quadrangle1->mColorLT == quadrangle2->mColorLT &&
					quadrangle1->mColorRT == quadrangle2->mColorRT &&
					quadrangle1->mColorLB == quadrangle2->mColorLB &&
					quadrangle1->mColorRB == quadrangle2->mColorRB &&
					quadrangle1->mPositionLT.Equal( quadrangle2->mPositionLT ) &&
					quadrangle1->mPositionRT.Equal( quadrangle2->mPositionRT ) &&
					quadrangle1->mPositionLB.Equal( quadrangle2->mPositionLB ) &&
					quadrangle1->mPositionRB.Equal( quadrangle2->mPositionRB ) &&
					quadrangle1->mTexcoordLT.Equal( quadrangle2->mTexcoordLT ) &&
					quadrangle1->mTexcoordRT.Equal( quadrangle2->mTexcoordRT ) &&
					quadrangle1->mTexcoordLB.Equal( quadrangle2->mTexcoordLB ) &&
					quadrangle1->mTexcoordRB.Equal( quadrangle2->mTexcoordRB );
		}
	}

	 return _false;
}

_bool RenderSet::CombineRenderUnit( const RenderUnit& renderunit )
{
	_dword index = mRenderToken.mLastRenderUnitIndex;

	if ( index >= mRenderUnitArray.Number( ) )
		return _false;

	RenderUnit& combineunit = mRenderUnitArray[ index ];

	// Layer not same, cant combine.
	if ( combineunit.mLayer != renderunit.mLayer )
		return _false;

	// We cant combine if render method not same.
	if ( combineunit.mTechnique != renderunit.mTechnique )
		return _false;

	Renderable* newrenderable = (Renderable*) renderunit.mRenderable;
	Renderable* oldrenderable = (Renderable*) combineunit.mRenderable;

	// Type not same, cant combine.
	if ( oldrenderable->mType != newrenderable->mType )
		return _false;

	// Try to combine renderable.
	switch ( newrenderable->mType )
	{
		case Renderable::_TYPE_OVERLAY_TRIANGLEIMAGE:
		case Renderable::_TYPE_OVERLAY_RECTANGLEIMAGE:
		case Renderable::_TYPE_OVERLAY_QUADRANGLEIMAGE:
		{
			OverlayImageBase* overlayimage1 = (OverlayImageBase*) newrenderable;
			OverlayImageBase* overlayimage2 = (OverlayImageBase*) oldrenderable;

			if ( overlayimage1->mTexture != overlayimage2->mTexture )
				return _false;

			GetTextureFactory( ).ReleaseTexture( overlayimage1->mTexture );

			break;
		}

		case Renderable::_TYPE_BILLBOARD_SCREENALIGNED:
		case Renderable::_TYPE_BILLBOARD_WORLDORIENTED:
		case Renderable::_TYPE_BILLBOARD_AXIAL:
		{
			BillboardBase* billboard1 = (BillboardBase*) newrenderable;
			BillboardBase* billboard2 = (BillboardBase*) oldrenderable;

			if ( billboard1->mTexture != billboard2->mTexture )
				return _false;

			GetTextureFactory( ).ReleaseTexture( billboard1->mTexture );

			break;
		}

		default:
		{
			return _false;
		}
	}

	// Link renderable together.
	if ( combineunit.mLinkTail != _null )
		combineunit.mLinkTail->mType = (_dword) newrenderable;

	if ( combineunit.mLinkHead == _null )
		combineunit.mLinkHead = newrenderable;

	combineunit.mLinkTail = newrenderable;
	combineunit.mLinkNumber ++;

	newrenderable->mType = 0;

	return _true;
}

_void RenderSet::AppendRenderUnit( Renderable* renderable, const TechniqueUnit& technique, const Matrix4& worldtransform, _bool needtransform, _dword layer, _bool alpha, const Vector3& center )
{
	ITechniqueFactory& techfactory = GetTechniqueFactory( );

	RenderUnit renderunit;
	renderunit.mLayer			= layer;
	renderunit.mRenderable		= renderable;
	renderunit.mRenderCenter	= center;
	renderunit.mTechnique.Clone( technique );

	// No user render method, use default one.
	if ( renderunit.mTechnique.mRenderMethod == _null )
		renderunit.mTechnique.mRenderMethod = techfactory.GetDefaultRenderMethod( *renderable, alpha );

	// No user sampler method, use default one.
	if ( renderunit.mTechnique.mSamplerMethod == _null )
		renderunit.mTechnique.mSamplerMethod = techfactory.GetDefaultSamplerMethod( *renderable );

	// No user texture method, use default one.
	if ( renderunit.mTechnique.mTextureMethod == _null )
		renderunit.mTechnique.mTextureMethod = techfactory.GetDefaultTextureMethod( *renderable, alpha );

	_bool canbecombine = ( renderable->mType >= Renderable::_TYPE_OVERLAY_WITHTEX_BEGIN && renderable->mType <= Renderable::_TYPE_OVERLAY_WITHTEX_END ) ||
						 ( renderable->mType >= Renderable::_TYPE_BILLBOARD_BEGIN && renderable->mType <= Renderable::_TYPE_BILLBOARD_END );

	if ( canbecombine && CombineRenderUnit( renderunit ) )
		return;

	SetupEnvironment( worldtransform, needtransform );

	renderunit.mWorldTransform		= needtransform ? mRenderToken.mWorldTransform : &Matrix4::cIdentity;
	renderunit.mViewTransform		= mRenderToken.mViewTransform;
	renderunit.mProjectionTransform	= mRenderToken.mProjectionTransform;

	for ( _dword i = 0; i < Geometry::_MAX_TEXTURE_NUMBER; i ++ )
		renderunit.mTextureTransform[i] = mRenderToken.mTextureTransform[i];

	renderunit.mViewport			= mRenderToken.mViewport;

	mRenderUnitArray.Append( renderunit );

	mRenderToken.mLastRenderUnitIndex = mRenderToken.mRenderUnitNumber;
	mRenderToken.mRenderUnitNumber ++;
}

_void RenderSet::ReleaseRenderUnit( RenderUnit& renderunit )
{
	renderunit.mTechnique.Clear( );

	switch ( renderunit.mRenderable->mType )
	{
		case Renderable::_TYPE_GEOMETRY:
		{
			GetGeometryFactory( ).ReleaseGeometry( *( (Geometry*) renderunit.mRenderable ) );

			break;
		}

		case Renderable::_TYPE_OVERLAY_TRIANGLEIMAGE:
		case Renderable::_TYPE_OVERLAY_RECTANGLEIMAGE:
		case Renderable::_TYPE_OVERLAY_QUADRANGLEIMAGE:
		{
			GetTextureFactory( ).ReleaseTexture( ( (OverlayImageBase*) renderunit.mRenderable )->mTexture );

			break;
		}

		case Renderable::_TYPE_PRIMITIVE_TRIANGLEIMAGE:
		case Renderable::_TYPE_PRIMITIVE_QUADRANGLEIMAGE:
		{
			GetTextureFactory( ).ReleaseTexture( ( (PrimitiveImageBase*) renderunit.mRenderable )->mTexture );

			break;
		}

		case Renderable::_TYPE_BILLBOARD_SCREENALIGNED:
		case Renderable::_TYPE_BILLBOARD_WORLDORIENTED:
		case Renderable::_TYPE_BILLBOARD_AXIAL:
		{
			GetTextureFactory( ).ReleaseTexture( ( (BillboardBase*) renderunit.mRenderable )->mTexture );

			break;
		}
	}
}

_void RenderSet::Render( const Geometry& geometry, const TechniqueUnit& technique, const Matrix4& worldtransform )
{
	if ( geometry.mRenderFlag & Geometry::_RENDERFLAG_INVISIBLE )
	{
		// Skip invisible geometry.
		if ( GetRenderConfig( ).IsRenderInvisibleGeometryEnabled( ) == _false )
			return;
	}

	if ( ( geometry.mRenderFlag & Geometry::_RENDERFLAG_BILLBOARD ) == 0 )
	{
		if ( geometry.mRenderFlag == 0 )
		{
			// Skip normal geometry.
			if ( GetRenderConfig( ).IsRenderNormalGeometryEnabled( ) == _false )
				return;
		}
		else
		{
			// Skip alpha geometry.
			if ( GetRenderConfig( ).IsRenderAlphaGeometryEnabled( ) == _false )
				return;
		}
	}

	Matrix4 transform = GetRenderDevice( ).GetWorldTransform( );

	if ( &worldtransform != &Matrix4::cIdentity )
		transform = worldtransform * transform;

	AxisAlignedBox boundbox = geometry.mVertexBuffer->GetBoundBox( ) * transform;

	// Clip by view frustum.
	if ( ( geometry.mFormatFlag & Geometry::_FORMATFLAG_POSITIONT ) == 0 &&
		GetRenderConfig( ).IsViewFrustumClipEnabled( ) )
	{
		// Skip geometry outside view frustum.
		if ( GetRenderDevice( ).GetViewFrustum( ).Inside( boundbox ) == _false )
			return;
	}

	// Render as billboard.
	if ( geometry.mRenderFlag & Geometry::_RENDERFLAG_BILLBOARD )
	{
		_dword texnumber = 0;

		for ( _dword i = 0; i < Geometry::_MAX_TEXTURE_NUMBER; i ++ )
		{
			if ( geometry.mTexture[i] != _null )
				texnumber ++;
			else
				break;
		}

		Vector3 position = boundbox.CenterVector( );

		//IRenderMethod* newmethod = _null;
		//IRenderMethod* oldmethod = (IRenderMethod*) rendermethod[0];

		//if ( geometry.mRenderFlag & Geometry::_RENDERFLAG_ADDITIVE )
		//{
		//	newmethod = GetTechniqueFactory( ).CreateRenderMethod(
		//		L"cullmode:none|depthwrite:false|alphablend:additive" );
		//}
		//else if ( geometry.mRenderFlag & Geometry::_RENDERFLAG_SUBTRACTIVE )
		//{
		//	newmethod = GetTechniqueFactory( ).CreateRenderMethod(
		//		L"cullmode:none|depthwrite:false|alphablend:subtractive" );
		//}
		//else if ( geometry.mRenderFlag & Geometry::_RENDERFLAG_ALPHA )
		//{
		//	newmethod = GetTechniqueFactory( ).CreateRenderMethod(
		//		L"cullmode:none|depthwrite:false|alphablend:normal" );
		//}

		//if ( newmethod != _null )
		//	rendermethod[0] = newmethod;

		BillboardWorldOriented billboard( position, Vector2( geometry.mDepthBias, geometry.mDepthBias ),
			geometry.mTextureFactor, 0.0f, geometry.mTexture[0], 0, 0 );

		Render( billboard, technique );

		//if ( newmethod != _null )
		//	rendermethod[0] = oldmethod;

		//if ( newmethod != _null )
		//	GetTechniqueFactory( ).ReleaseRenderMethod( newmethod );
	}
	// Render as geometry.
	else
	{
		_void* buffer = mRenderUnitAllocater.Allocate( &geometry, sizeof( Geometry ) );

		GetGeometryFactory( ).CloneGeometry( (Geometry&) geometry, _true );

		AppendRenderUnit( (Renderable*) buffer, technique, worldtransform, _true,
			GetDefaultLayer( ), IsAlphaColor( geometry.mTextureFactor ), boundbox.CenterVector( ) );
	}
}

_void RenderSet::Render( const OverlayPoint& point, const TechniqueUnit& technique )
{
	_void* buffer = mRenderUnitAllocater.Allocate( &point, sizeof( OverlayPoint ) );

	// Align overlay to pixel.
	if ( GetRenderConfig( ).IsOverlayPixelAlignEnabled( ) )
	{
		_long x = (_long) ( (OverlayPoint*) buffer )->mPosition.x;
		_long y = (_long) ( (OverlayPoint*) buffer )->mPosition.y;

		( (OverlayPoint*) buffer )->mPosition.x = (_float) x;
		( (OverlayPoint*) buffer )->mPosition.y = (_float) y;
	}

	AppendRenderUnit( (Renderable*) buffer, technique, Matrix4::cIdentity, _false,
		GetOverlayLayer( ), IsAlphaColor( point.mColor ), Vector3::cOrigin );

	mRenderToken.mOverlayNoneTexVN += 1;
}

_void RenderSet::Render( const OverlayLine& line, const TechniqueUnit& technique )
{
	_void* buffer = mRenderUnitAllocater.Allocate( &line, sizeof( OverlayLine ) );

	// Align overlay to pixel.
	if ( GetRenderConfig( ).IsOverlayPixelAlignEnabled( ) )
	{
		_long x1 = (_long) ( (OverlayLine*) buffer )->mPosition1.x;
		_long y1 = (_long) ( (OverlayLine*) buffer )->mPosition1.y;
		_long x2 = (_long) ( (OverlayLine*) buffer )->mPosition2.x;
		_long y2 = (_long) ( (OverlayLine*) buffer )->mPosition2.y;

		( (OverlayLine*) buffer )->mPosition1.x = (_float) x1;
		( (OverlayLine*) buffer )->mPosition1.y = (_float) y1;
		( (OverlayLine*) buffer )->mPosition2.x = (_float) x2;
		( (OverlayLine*) buffer )->mPosition2.y = (_float) y2;
	}

	AppendRenderUnit( (Renderable*) buffer, technique, Matrix4::cIdentity, _false,
		GetOverlayLayer( ), IsAlphaColor( line.mColor1 ) || IsAlphaColor( line.mColor2 ), Vector3::cOrigin );

	mRenderToken.mOverlayNoneTexVN += 2;
}

_void RenderSet::Render( const OverlayTriangle& triangle, const TechniqueUnit& technique )
{
	_void* buffer = mRenderUnitAllocater.Allocate( &triangle, sizeof( OverlayTriangle ) );

	// Align overlay to pixel.
	if ( GetRenderConfig( ).IsOverlayPixelAlignEnabled( ) )
	{
		_long x1 = (_long) ( (OverlayTriangle*) buffer )->mPosition1.x;
		_long y1 = (_long) ( (OverlayTriangle*) buffer )->mPosition1.y;
		_long x2 = (_long) ( (OverlayTriangle*) buffer )->mPosition2.x;
		_long y2 = (_long) ( (OverlayTriangle*) buffer )->mPosition2.y;
		_long x3 = (_long) ( (OverlayTriangle*) buffer )->mPosition3.x;
		_long y3 = (_long) ( (OverlayTriangle*) buffer )->mPosition3.y;

		( (OverlayTriangle*) buffer )->mPosition1.x = (_float) x1;
		( (OverlayTriangle*) buffer )->mPosition1.y = (_float) y1;
		( (OverlayTriangle*) buffer )->mPosition2.x = (_float) x2;
		( (OverlayTriangle*) buffer )->mPosition2.y = (_float) y2;
		( (OverlayTriangle*) buffer )->mPosition3.x = (_float) x3;
		( (OverlayTriangle*) buffer )->mPosition3.y = (_float) y3;
	}

	AppendRenderUnit( (Renderable*) buffer, technique, Matrix4::cIdentity, _false, GetOverlayLayer( ),
		IsAlphaColor( triangle.mColor1 ) || IsAlphaColor( triangle.mColor2 ) || IsAlphaColor( triangle.mColor3 ), Vector3::cOrigin );

	mRenderToken.mOverlayNoneTexVN += 3;
}

_void RenderSet::Render( const OverlayTriangleImage& triangleimage, const TechniqueUnit& technique )
{
	_void* buffer = mRenderUnitAllocater.Allocate( &triangleimage, sizeof( OverlayTriangleImage ) );

	// Align overlay to pixel.
	if ( GetRenderConfig( ).IsOverlayPixelAlignEnabled( ) )
	{
		_long x1 = (_long) ( (OverlayTriangleImage*) buffer )->mPosition1.x;
		_long y1 = (_long) ( (OverlayTriangleImage*) buffer )->mPosition1.y;
		_long x2 = (_long) ( (OverlayTriangleImage*) buffer )->mPosition2.x;
		_long y2 = (_long) ( (OverlayTriangleImage*) buffer )->mPosition2.y;
		_long x3 = (_long) ( (OverlayTriangleImage*) buffer )->mPosition3.x;
		_long y3 = (_long) ( (OverlayTriangleImage*) buffer )->mPosition3.y;

		( (OverlayTriangleImage*) buffer )->mPosition1.x = (_float) x1;
		( (OverlayTriangleImage*) buffer )->mPosition1.y = (_float) y1;
		( (OverlayTriangleImage*) buffer )->mPosition2.x = (_float) x2;
		( (OverlayTriangleImage*) buffer )->mPosition2.y = (_float) y2;
		( (OverlayTriangleImage*) buffer )->mPosition3.x = (_float) x3;
		( (OverlayTriangleImage*) buffer )->mPosition3.y = (_float) y3;
	}

	// Increase texture reference.
	GetTextureFactory( ).CloneTexture( triangleimage.mTexture, _true );

	AppendRenderUnit( (Renderable*) buffer, technique, Matrix4::cIdentity, _false,
		GetOverlayLayer( ), _true, Vector3::cOrigin );

	mRenderToken.mOverlayWithTexVN += 3;
}

_void RenderSet::Render( const OverlayRectangle& rectangle, const TechniqueUnit& technique )
{
	_void* buffer = mRenderUnitAllocater.Allocate( &rectangle, sizeof( OverlayRectangle ) );

	// Align overlay to pixel.
	if ( GetRenderConfig( ).IsOverlayPixelAlignEnabled( ) )
	{
		_long xlt = (_long) ( (OverlayRectangle*) buffer )->mPositionLT.x;
		_long ylt = (_long) ( (OverlayRectangle*) buffer )->mPositionLT.y;
		_long xrb = (_long) ( (OverlayRectangle*) buffer )->mPositionRB.x;
		_long yrb = (_long) ( (OverlayRectangle*) buffer )->mPositionRB.y;

		( (OverlayRectangle*) buffer )->mPositionLT.x = (_float) xlt;
		( (OverlayRectangle*) buffer )->mPositionLT.y = (_float) ylt;
		( (OverlayRectangle*) buffer )->mPositionRB.x = (_float) xrb;
		( (OverlayRectangle*) buffer )->mPositionRB.y = (_float) yrb;
	}

	AppendRenderUnit( (Renderable*) buffer, technique, Matrix4::cIdentity, _false,
		GetOverlayLayer( ), IsAlphaColor( rectangle.mColor ), Vector3::cOrigin );

	mRenderToken.mOverlayNoneTexVN += 6;
}

_void RenderSet::Render( const OverlayRectangleImage& rectangleimage, const TechniqueUnit& technique )
{
	_void* buffer = mRenderUnitAllocater.Allocate( &rectangleimage, sizeof( OverlayRectangleImage ) );

	// Align overlay to pixel.
	if ( GetRenderConfig( ).IsOverlayPixelAlignEnabled( ) )
	{
		_long xlt = (_long) ( (OverlayRectangleImage*) buffer )->mPositionLT.x;
		_long ylt = (_long) ( (OverlayRectangleImage*) buffer )->mPositionLT.y;
		_long xrb = (_long) ( (OverlayRectangleImage*) buffer )->mPositionRB.x;
		_long yrb = (_long) ( (OverlayRectangleImage*) buffer )->mPositionRB.y;

		( (OverlayRectangleImage*) buffer )->mPositionLT.x = (_float) xlt;
		( (OverlayRectangleImage*) buffer )->mPositionLT.y = (_float) ylt;
		( (OverlayRectangleImage*) buffer )->mPositionRB.x = (_float) xrb;
		( (OverlayRectangleImage*) buffer )->mPositionRB.y = (_float) yrb;
	}

	// Increase texture reference.
	GetTextureFactory( ).CloneTexture( rectangleimage.mTexture, _true );

	AppendRenderUnit( (Renderable*) buffer, technique, Matrix4::cIdentity, _false,
		GetOverlayLayer( ), _true, Vector3::cOrigin );

	mRenderToken.mOverlayWithTexVN += 6;
}

_void RenderSet::Render( const OverlayQuadrangle& quadrangle, const TechniqueUnit& technique )
{
	_void* buffer = mRenderUnitAllocater.Allocate( &quadrangle, sizeof( OverlayQuadrangle ) );

	// Align overlay to pixel.
	if ( GetRenderConfig( ).IsOverlayPixelAlignEnabled( ) )
	{
		_long xlt = (_long) ( (OverlayQuadrangle*) buffer )->mPositionLT.x;
		_long ylt = (_long) ( (OverlayQuadrangle*) buffer )->mPositionLT.y;
		_long xrt = (_long) ( (OverlayQuadrangle*) buffer )->mPositionRT.x;
		_long yrt = (_long) ( (OverlayQuadrangle*) buffer )->mPositionRT.y;
		_long xlb = (_long) ( (OverlayQuadrangle*) buffer )->mPositionLB.x;
		_long ylb = (_long) ( (OverlayQuadrangle*) buffer )->mPositionLB.y;
		_long xrb = (_long) ( (OverlayQuadrangle*) buffer )->mPositionRB.x;
		_long yrb = (_long) ( (OverlayQuadrangle*) buffer )->mPositionRB.y;

		( (OverlayQuadrangle*) buffer )->mPositionLT.x = (_float) xlt;
		( (OverlayQuadrangle*) buffer )->mPositionLT.y = (_float) ylt;
		( (OverlayQuadrangle*) buffer )->mPositionRT.x = (_float) xrt;
		( (OverlayQuadrangle*) buffer )->mPositionRT.y = (_float) yrt;
		( (OverlayQuadrangle*) buffer )->mPositionLB.x = (_float) xlb;
		( (OverlayQuadrangle*) buffer )->mPositionLB.y = (_float) ylb;
		( (OverlayQuadrangle*) buffer )->mPositionRB.x = (_float) xrb;
		( (OverlayQuadrangle*) buffer )->mPositionRB.y = (_float) yrb;
	}

	AppendRenderUnit( (Renderable*) buffer, technique, Matrix4::cIdentity, _false, GetOverlayLayer( ),
		IsAlphaColor( quadrangle.mColorLT ) || IsAlphaColor( quadrangle.mColorLB ) || IsAlphaColor( quadrangle.mColorRT ) || IsAlphaColor( quadrangle.mColorRB ), Vector3::cOrigin );

	mRenderToken.mOverlayNoneTexVN += 6;
}

_void RenderSet::Render( const OverlayQuadrangleImage& quadrangleimage, const TechniqueUnit& technique )
{
	_void* buffer = mRenderUnitAllocater.Allocate( &quadrangleimage, sizeof( OverlayQuadrangleImage ) );

	// Align overlay to pixel.
	if ( GetRenderConfig( ).IsOverlayPixelAlignEnabled( ) )
	{
		_long xlt = (_long) ( (OverlayQuadrangleImage*) buffer )->mPositionLT.x;
		_long ylt = (_long) ( (OverlayQuadrangleImage*) buffer )->mPositionLT.y;
		_long xrt = (_long) ( (OverlayQuadrangleImage*) buffer )->mPositionRT.x;
		_long yrt = (_long) ( (OverlayQuadrangleImage*) buffer )->mPositionRT.y;
		_long xlb = (_long) ( (OverlayQuadrangleImage*) buffer )->mPositionLB.x;
		_long ylb = (_long) ( (OverlayQuadrangleImage*) buffer )->mPositionLB.y;
		_long xrb = (_long) ( (OverlayQuadrangleImage*) buffer )->mPositionRB.x;
		_long yrb = (_long) ( (OverlayQuadrangleImage*) buffer )->mPositionRB.y;

		( (OverlayQuadrangleImage*) buffer )->mPositionLT.x = (_float) xlt;
		( (OverlayQuadrangleImage*) buffer )->mPositionLT.y = (_float) ylt;
		( (OverlayQuadrangleImage*) buffer )->mPositionRT.x = (_float) xrt;
		( (OverlayQuadrangleImage*) buffer )->mPositionRT.y = (_float) yrt;
		( (OverlayQuadrangleImage*) buffer )->mPositionLB.x = (_float) xlb;
		( (OverlayQuadrangleImage*) buffer )->mPositionLB.y = (_float) ylb;
		( (OverlayQuadrangleImage*) buffer )->mPositionRB.x = (_float) xrb;
		( (OverlayQuadrangleImage*) buffer )->mPositionRB.y = (_float) yrb;
	}

	// Increase texture reference.
	GetTextureFactory( ).CloneTexture( quadrangleimage.mTexture, _true );

	AppendRenderUnit( (Renderable*) buffer, technique, Matrix4::cIdentity, _false,
		GetOverlayLayer( ), _true, Vector3::cOrigin );

	mRenderToken.mOverlayWithTexVN += 6;
}

_void RenderSet::Render( const PrimitivePoint& point, const TechniqueUnit& technique )
{
	_void* buffer = mRenderUnitAllocater.Allocate( &point, sizeof( PrimitivePoint ) );

	AppendRenderUnit( (Renderable*) buffer, technique, Matrix4::cIdentity, _false,
		GetDefaultLayer( ), IsAlphaColor( point.mColor ), point.mPosition );

	mRenderToken.mPrimitiveNoneTexVN += 1;
}

_void RenderSet::Render( const PrimitiveLine& line, const TechniqueUnit& technique )
{
	_void* buffer = mRenderUnitAllocater.Allocate( &line, sizeof( PrimitiveLine ) );

	AppendRenderUnit( (Renderable*) buffer, technique, Matrix4::cIdentity, _false, GetDefaultLayer( ),
		IsAlphaColor( line.mColor1 ) || IsAlphaColor( line.mColor2 ), line.mPosition1 );

	mRenderToken.mPrimitiveNoneTexVN += 2;
}

_void RenderSet::Render( const PrimitiveTriangle& triangle, const TechniqueUnit& technique )
{
	_void* buffer = mRenderUnitAllocater.Allocate( &triangle, sizeof( PrimitiveTriangle ) );

	AppendRenderUnit( (Renderable*) buffer, technique, Matrix4::cIdentity, _false, GetDefaultLayer( ),
		IsAlphaColor( triangle.mColor1 ) || IsAlphaColor( triangle.mColor2 ) || IsAlphaColor( triangle.mColor3 ), triangle.mPosition1 );

	mRenderToken.mPrimitiveNoneTexVN += 3;
}

_void RenderSet::Render( const PrimitiveTriangleImage& triangleimage, const TechniqueUnit& technique )
{
	_void* buffer = mRenderUnitAllocater.Allocate( &triangleimage, sizeof( PrimitiveTriangleImage ) );

	// Increase texture reference.
	GetTextureFactory( ).CloneTexture( triangleimage.mTexture, _true );

	AppendRenderUnit( (Renderable*) buffer, technique, Matrix4::cIdentity, _false,
		GetDefaultLayer( ), _true, triangleimage.mPosition1 );

	mRenderToken.mPrimitiveWithTexVN += 3;
}

_void RenderSet::Render( const PrimitiveQuadrangle& quadrangle, const TechniqueUnit& technique )
{
	_void* buffer = mRenderUnitAllocater.Allocate( &quadrangle, sizeof( PrimitiveQuadrangle ) );

	AppendRenderUnit( (Renderable*) buffer, technique, Matrix4::cIdentity, _false,
		GetDefaultLayer( ), IsAlphaColor( quadrangle.mColor ), quadrangle.mCenter );

	mRenderToken.mPrimitiveNoneTexVN += 8;
}

_void RenderSet::Render( const PrimitiveQuadrangleImage& quadrangleimage, const TechniqueUnit& technique )
{
	_void* buffer = mRenderUnitAllocater.Allocate( &quadrangleimage, sizeof( PrimitiveQuadrangleImage ) );

	// Increase texture reference.
	GetTextureFactory( ).CloneTexture( quadrangleimage.mTexture, _true );

	AppendRenderUnit( (Renderable*) buffer, technique, Matrix4::cIdentity, _false,
		GetDefaultLayer( ), _true, quadrangleimage.mCenter );

	mRenderToken.mPrimitiveWithTexVN += 6;
}

_void RenderSet::Render( const PrimitiveBox& box, const TechniqueUnit& technique )
{
	_void* buffer = mRenderUnitAllocater.Allocate( &box, sizeof( PrimitiveBox ) );

	AppendRenderUnit( (Renderable*) buffer, technique, Matrix4::cIdentity, _false,
		GetDefaultLayer( ), IsAlphaColor( box.mColor ), box.mCenter );

	mRenderToken.mPrimitiveNoneTexVN += 24;
}

_void RenderSet::Render( const PrimitiveCircle& circle, const TechniqueUnit& technique )
{
	_void* buffer = mRenderUnitAllocater.Allocate( &circle, sizeof( PrimitiveCircle ) );

	AppendRenderUnit( (Renderable*) buffer, technique, Matrix4::cIdentity, _false,
		GetDefaultLayer( ), IsAlphaColor( circle.mColor ), circle.mCenter );

	mRenderToken.mPrimitiveNoneTexVN += circle.mSegment * 2;
}

_void RenderSet::Render( const PrimitiveSphere& sphere, const TechniqueUnit& technique )
{
	_void* buffer = mRenderUnitAllocater.Allocate( &sphere, sizeof( PrimitiveSphere ) );

	AppendRenderUnit( (Renderable*) buffer, technique, Matrix4::cIdentity, _false,
		GetDefaultLayer( ), IsAlphaColor( sphere.mColor ), sphere.mCenter );

	mRenderToken.mPrimitiveNoneTexVN += sphere.mSegment * 2 * 3;
}

_void RenderSet::Render( const PrimitivePyramid& pyramid, const TechniqueUnit& technique )
{
	_void* buffer = mRenderUnitAllocater.Allocate( &pyramid, sizeof( PrimitivePyramid ) );

	AppendRenderUnit( (Renderable*) buffer, technique, Matrix4::cIdentity, _false,
		GetDefaultLayer( ), IsAlphaColor( pyramid.mColor ), pyramid.mBase );

	mRenderToken.mPrimitiveNoneTexVN += 16;
}

_void RenderSet::Render( const PrimitiveAxis& axis, const TechniqueUnit& technique )
{
	_void* buffer = mRenderUnitAllocater.Allocate( &axis, sizeof( PrimitiveAxis ) );

	AppendRenderUnit( (Renderable*) buffer, technique, Matrix4::cIdentity, _false,
		GetDefaultLayer( ), _false, axis.mOrigin );

	mRenderToken.mPrimitiveNoneTexVN += 6;
}

_void RenderSet::Render( const PrimitiveFrustum& frustum, const TechniqueUnit& technique )
{
	_void* buffer = mRenderUnitAllocater.Allocate( &frustum, sizeof( PrimitiveFrustum ) );

	AppendRenderUnit( (Renderable*) buffer, technique, Matrix4::cIdentity, _false,
		GetDefaultLayer( ), IsAlphaColor( frustum.mColor ), Vector3::cOrigin );

	mRenderToken.mPrimitiveNoneTexVN += 36;
}

_void RenderSet::Render( const BillboardScreenAligned& screenaligned, const TechniqueUnit& technique )
{
	_void* buffer = mRenderUnitAllocater.Allocate( &screenaligned, sizeof( BillboardScreenAligned ) );

	// Increase texture reference.
	GetTextureFactory( ).CloneTexture( screenaligned.mTexture, _true );

	AppendRenderUnit( (Renderable*) buffer, technique, Matrix4::cIdentity, _false,
		GetDefaultLayer( ), _true, screenaligned.mPosition );

	mRenderToken.mBillboardVN += 6;
}

_void RenderSet::Render( const BillboardWorldOriented& worldoriented, const TechniqueUnit& technique )
{
	_void* buffer = mRenderUnitAllocater.Allocate( &worldoriented, sizeof( BillboardWorldOriented ) );

	// Increase texture reference.
	GetTextureFactory( ).CloneTexture( worldoriented.mTexture, _true );

	AppendRenderUnit( (Renderable*) buffer, technique, Matrix4::cIdentity, _false,
		GetDefaultLayer( ), _true, worldoriented.mPosition );

	mRenderToken.mBillboardVN += 6;
}

_void RenderSet::Render( const BillboardAxial& axial, const TechniqueUnit& technique )
{
	_void* buffer = mRenderUnitAllocater.Allocate( &axial, sizeof( BillboardAxial ) );

	// Increase texture reference.
	GetTextureFactory( ).CloneTexture( axial.mTexture, _true );

	AppendRenderUnit( (Renderable*) buffer, technique, Matrix4::cIdentity, _false,
		GetDefaultLayer( ), _true, axial.mPosition );

	mRenderToken.mBillboardVN += 6;
}

_void RenderSet::SetRenderLayer( _dword layer )
{
	mDefaultLayer = layer;
}

_void RenderSet::ResetRenderLayer( )
{
	mDefaultLayer = 0;
}

_void RenderSet::BeginSubRenderLayer( _dword layer )
{
	RenderUnit renderunit;
	renderunit.mLayer = 0x00010000 | layer;

	// Put a begin layer flag.
	mRenderUnitArray.Append( renderunit );

	mRenderToken.mLastRenderUnitIndex = mRenderToken.mRenderUnitNumber;
	mRenderToken.mRenderUnitNumber ++;
}

_void RenderSet::EndSubRenderLayer( )
{
	RenderUnit renderunit;
	renderunit.mLayer = 0x00020000;

	// Put an end layer flag.
	mRenderUnitArray.Append( renderunit );

	mRenderToken.mLastRenderUnitIndex = mRenderToken.mRenderUnitNumber;
	mRenderToken.mRenderUnitNumber ++;
}

_void RenderSet::Prepare( )
{
	// Prepare overlay none texcoord vertex buffer.
	if ( mRenderToken.mOverlayNoneTexVN != 0 )
		CreateOverlayNoneTexVB( );

	// Prepare overlay with texcoord vertex buffer.
	if ( mRenderToken.mOverlayWithTexVN != 0 )
		CreateOverlayWithTexVB( );

	// Prepare primitive none texcoord vertex buffer.
	if ( mRenderToken.mPrimitiveNoneTexVN != 0 )
		CreatePrimitiveNoneTexVB( );

	// Prepare primitive with texcoord vertex buffer.
	if ( mRenderToken.mPrimitiveWithTexVN != 0 )
		CreatePrimitiveWithTexVB( );

	// Prepare billboard vertex buffer.
	if ( mRenderToken.mBillboardVN != 0 )
		CreateBillboardVB( );

	// Lock vertex buffer for overlay none texcoord.
	OverlayVertexNoneTex* overlaynonetexbuffer = _null;
	if ( mRenderToken.mOverlayNoneTexVN != 0 && mOverlayNoneTexVB != _null )
	{
		overlaynonetexbuffer = (OverlayVertexNoneTex*) mOverlayNoneTexVB->Lock( 0,
			mRenderToken.mOverlayNoneTexVN * sizeof( OverlayVertexNoneTex ), IGeometryFactory::_FLAG_DISCARD );
	}

	// Lock vertex buffer for overlay with texcoord.
	OverlayVertexWithTex* overlaywithtexbuffer = _null;
	if ( mRenderToken.mOverlayWithTexVN != 0 && mOverlayWithTexVB != _null )
	{
		overlaywithtexbuffer = (OverlayVertexWithTex*) mOverlayWithTexVB->Lock( 0,
			mRenderToken.mOverlayWithTexVN * sizeof( OverlayVertexWithTex ), IGeometryFactory::_FLAG_DISCARD );
	}

	// Lock vertex buffer for primitive none texcoord.
	PrimitiveVertexNoneTex* primitivenonetexbuffer = _null;
	if ( mRenderToken.mPrimitiveNoneTexVN != 0 && mPrimitiveNoneTexVB != _null )
	{
		primitivenonetexbuffer = (PrimitiveVertexNoneTex*) mPrimitiveNoneTexVB->Lock( 0,
			mRenderToken.mPrimitiveNoneTexVN * sizeof( PrimitiveVertexNoneTex ), IGeometryFactory::_FLAG_DISCARD );
	}

	// Lock vertex buffer for primitive with texcoord.
	PrimitiveVertexWithTex* primitivewithtexbuffer = _null;
	if ( mRenderToken.mPrimitiveWithTexVN != 0 && mPrimitiveWithTexVB != _null )
	{
		primitivewithtexbuffer = (PrimitiveVertexWithTex*) mPrimitiveWithTexVB->Lock( 0,
			mRenderToken.mPrimitiveWithTexVN * sizeof( PrimitiveVertexWithTex ), IGeometryFactory::_FLAG_DISCARD );
	}

	// Lock vertex buffer for billboard.
	BillboardVertex* billboardbuffer = _null;
	if ( mRenderToken.mBillboardVN != 0 && mBillboardVB != _null )
	{
		billboardbuffer = (BillboardVertex*) mBillboardVB->Lock( 0,
			mRenderToken.mBillboardVN * sizeof( BillboardVertex ), IGeometryFactory::_FLAG_DISCARD );
	}

	_dword overlaynonetexvi = 0;
	_dword overlaywithtexvi = 0;
	_dword primitivenonetexvi = 0;
	_dword primitivewithtexvi = 0;
	_dword billboardvi = 0;

	// Fill the renderable buffer.
	for ( _dword i = 0; i < mRenderUnitArray.Number( ); i ++ )
	{
		RenderUnit& renderunit = mRenderUnitArray[i];

		if ( renderunit.mRenderable == _null )
			continue;

		if ( renderunit.mRenderable->mType == Renderable::_TYPE_GEOMETRY )
		{
			renderunit.mGeometry = (const Geometry*) renderunit.mRenderable;

			continue;
		}

		// Create a geometry to render it.
		renderunit.mGeometry = (Geometry*) mRenderUnitAllocater.Allocate( sizeof( Geometry ) );

		// Ready to fill this geometry.
		Geometry& geometry = *( (Geometry*) renderunit.mGeometry );
		geometry.Reset( );

		// Fill to vertex buffer according to type.
		switch ( renderunit.mRenderable->mType )
		{
			case Renderable::_TYPE_OVERLAY_POINT:
			{
				OverlayPoint* point = (OverlayPoint*) renderunit.mRenderable;

				if ( overlaynonetexbuffer == _null )
					continue;

				geometry.mVertexDeclaration	= mOverlayNoneTexVD;
				geometry.mVertexBuffer		= mOverlayNoneTexVB;
				geometry.mIndexBuffer		= _null;
				geometry.mGeometryType		= Geometry::_GEOMETRY_POINTLIST;
				geometry.mGeometryNumber	= 1;
				geometry.mVertexSize		= sizeof( OverlayVertexNoneTex );
				geometry.mVertexNumber		= 1;
				geometry.mStartVertex		= overlaynonetexvi;
				geometry.mStartIndex		= 0;

				overlaynonetexvi += 1;

				// Fill vertex buffer.
				overlaynonetexbuffer->mPosition.x	= point->mPosition.x;
				overlaynonetexbuffer->mPosition.y	= point->mPosition.y;
				overlaynonetexbuffer->mPosition.z	= 1.0f;
				overlaynonetexbuffer->mPosition.w	= 1.0f;
				overlaynonetexbuffer->mColor		= GraphicsManager::GetRenderAPIHelper( ).ConvertColor( point->mColor );
				overlaynonetexbuffer ++;

				break;
			}

			case Renderable::_TYPE_OVERLAY_LINE:
			{
				OverlayLine* line = (OverlayLine*) renderunit.mRenderable;

				if ( overlaynonetexbuffer == _null )
					continue;

				geometry.mVertexDeclaration	= mOverlayNoneTexVD;
				geometry.mVertexBuffer		= mOverlayNoneTexVB;
				geometry.mIndexBuffer		= _null;
				geometry.mGeometryType		= Geometry::_GEOMETRY_LINELIST;
				geometry.mGeometryNumber	= 1;
				geometry.mVertexSize		= sizeof( OverlayVertexNoneTex );
				geometry.mVertexNumber		= 2;
				geometry.mStartVertex		= overlaynonetexvi;
				geometry.mStartIndex		= 0;

				overlaynonetexvi += 2;

				// Fill vertex buffer.
				overlaynonetexbuffer->mPosition.x	= line->mPosition1.x;
				overlaynonetexbuffer->mPosition.y	= line->mPosition1.y;
				overlaynonetexbuffer->mPosition.z	= 1.0f;
				overlaynonetexbuffer->mPosition.w	= 1.0f;
				overlaynonetexbuffer->mColor		= GraphicsManager::GetRenderAPIHelper( ).ConvertColor( line->mColor1 );
				overlaynonetexbuffer ++;
				overlaynonetexbuffer->mPosition.x	= line->mPosition2.x;
				overlaynonetexbuffer->mPosition.y	= line->mPosition2.y;
				overlaynonetexbuffer->mPosition.z	= 1.0f;
				overlaynonetexbuffer->mPosition.w	= 1.0f;
				overlaynonetexbuffer->mColor		= GraphicsManager::GetRenderAPIHelper( ).ConvertColor( line->mColor2 );
				overlaynonetexbuffer ++;

				break;
			}

			case Renderable::_TYPE_OVERLAY_TRIANGLE:
			{
				OverlayTriangle* triangle = (OverlayTriangle*) renderunit.mRenderable;

				if ( overlaynonetexbuffer == _null )
					continue;

				geometry.mVertexDeclaration	= mOverlayNoneTexVD;
				geometry.mVertexBuffer		= mOverlayNoneTexVB;
				geometry.mIndexBuffer		= _null;
				geometry.mGeometryType		= Geometry::_GEOMETRY_TRIANGLELIST;
				geometry.mGeometryNumber	= 1;
				geometry.mVertexSize		= sizeof( OverlayVertexNoneTex );
				geometry.mVertexNumber		= 3;
				geometry.mStartVertex		= overlaynonetexvi;
				geometry.mStartIndex		= 0;

				overlaynonetexvi += 3;

				// Fill vertex buffer.
				overlaynonetexbuffer->mPosition.x	= triangle->mPosition1.x;
				overlaynonetexbuffer->mPosition.y	= triangle->mPosition1.y;
				overlaynonetexbuffer->mPosition.z	= 1.0f;
				overlaynonetexbuffer->mPosition.w	= 1.0f;
				overlaynonetexbuffer->mColor		= GraphicsManager::GetRenderAPIHelper( ).ConvertColor( triangle->mColor1 );
				overlaynonetexbuffer ++;
				overlaynonetexbuffer->mPosition.x	= triangle->mPosition2.x;
				overlaynonetexbuffer->mPosition.y	= triangle->mPosition2.y;
				overlaynonetexbuffer->mPosition.z	= 1.0f;
				overlaynonetexbuffer->mPosition.w	= 1.0f;
				overlaynonetexbuffer->mColor		= GraphicsManager::GetRenderAPIHelper( ).ConvertColor( triangle->mColor2 );
				overlaynonetexbuffer ++;
				overlaynonetexbuffer->mPosition.x	= triangle->mPosition3.x;
				overlaynonetexbuffer->mPosition.y	= triangle->mPosition3.y;
				overlaynonetexbuffer->mPosition.z	= 1.0f;
				overlaynonetexbuffer->mPosition.w	= 1.0f;
				overlaynonetexbuffer->mColor		= GraphicsManager::GetRenderAPIHelper( ).ConvertColor( triangle->mColor3 );
				overlaynonetexbuffer ++;

				break;
			}

			case Renderable::_TYPE_OVERLAY_TRIANGLEIMAGE:
			{
				OverlayTriangleImage* triangleimage = (OverlayTriangleImage*) renderunit.mRenderable;

				if ( overlaywithtexbuffer == _null )
					continue;

				geometry.mVertexDeclaration	= mOverlayWithTexVD;
				geometry.mVertexBuffer		= mOverlayWithTexVB;
				geometry.mIndexBuffer		= _null;
				geometry.mTexture[0]		= triangleimage->mTexture;
				geometry.mGeometryType		= Geometry::_GEOMETRY_TRIANGLELIST;
				geometry.mGeometryNumber	= 1 + renderunit.mLinkNumber;
				geometry.mVertexSize		= sizeof( OverlayVertexWithTex );
				geometry.mVertexNumber		= 3 + 3 * renderunit.mLinkNumber;
				geometry.mStartVertex		= overlaywithtexvi;
				geometry.mStartIndex		= 0;

				overlaywithtexvi += geometry.mVertexNumber;

				// Fill vertex buffer.
				do
				{
					overlaywithtexbuffer->mPosition.x	= triangleimage->mPosition1.x;
					overlaywithtexbuffer->mPosition.y	= triangleimage->mPosition1.y;
					overlaywithtexbuffer->mPosition.z	= 1.0f;
					overlaywithtexbuffer->mPosition.w	= 1.0f;
					overlaywithtexbuffer->mColor		= GraphicsManager::GetRenderAPIHelper( ).ConvertColor( triangleimage->mColor1 );
					overlaywithtexbuffer->mTexcoord.x	= triangleimage->mTexcoord1.x;
					overlaywithtexbuffer->mTexcoord.y	= triangleimage->mTexcoord1.y;
					overlaywithtexbuffer ++;
					overlaywithtexbuffer->mPosition.x	= triangleimage->mPosition2.x;
					overlaywithtexbuffer->mPosition.y	= triangleimage->mPosition2.y;
					overlaywithtexbuffer->mPosition.z	= 1.0f;
					overlaywithtexbuffer->mPosition.w	= 1.0f;
					overlaywithtexbuffer->mColor		= GraphicsManager::GetRenderAPIHelper( ).ConvertColor( triangleimage->mColor2 );
					overlaywithtexbuffer->mTexcoord.x	= triangleimage->mTexcoord2.x;
					overlaywithtexbuffer->mTexcoord.y	= triangleimage->mTexcoord2.y;
					overlaywithtexbuffer ++;
					overlaywithtexbuffer->mPosition.x	= triangleimage->mPosition3.x;
					overlaywithtexbuffer->mPosition.y	= triangleimage->mPosition3.y;
					overlaywithtexbuffer->mPosition.z	= 1.0f;
					overlaywithtexbuffer->mPosition.w	= 1.0f;
					overlaywithtexbuffer->mColor		= GraphicsManager::GetRenderAPIHelper( ).ConvertColor( triangleimage->mColor3 );
					overlaywithtexbuffer->mTexcoord.x	= triangleimage->mTexcoord3.x;
					overlaywithtexbuffer->mTexcoord.y	= triangleimage->mTexcoord3.y;
					overlaywithtexbuffer ++;

					if ( triangleimage == renderunit.mRenderable )
						triangleimage = (OverlayTriangleImage*) renderunit.mLinkHead;
					else
						triangleimage = (OverlayTriangleImage*) triangleimage->mType;
				}
				while ( triangleimage != _null );

				break;
			}

			case Renderable::_TYPE_OVERLAY_RECTANGLE:
			{
				OverlayRectangle* rectangle = (OverlayRectangle*) renderunit.mRenderable;

				if ( overlaynonetexbuffer == _null )
					continue;

				geometry.mVertexDeclaration	= mOverlayNoneTexVD;
				geometry.mVertexBuffer		= mOverlayNoneTexVB;
				geometry.mIndexBuffer		= _null;
				geometry.mGeometryType		= Geometry::_GEOMETRY_TRIANGLELIST;
				geometry.mGeometryNumber	= 2;
				geometry.mVertexSize		= sizeof( OverlayVertexNoneTex );
				geometry.mVertexNumber		= 6;
				geometry.mStartVertex		= overlaynonetexvi;
				geometry.mStartIndex		= 0;

				overlaynonetexvi += 6;

				_dword color = GraphicsManager::GetRenderAPIHelper( ).ConvertColor( rectangle->mColor );

				// Fill vertex buffer.
				overlaynonetexbuffer->mPosition.x	= rectangle->mPositionLT.x;
				overlaynonetexbuffer->mPosition.y	= rectangle->mPositionLT.y;
				overlaynonetexbuffer->mPosition.z	= 1.0f;
				overlaynonetexbuffer->mPosition.w	= 1.0f;
				overlaynonetexbuffer->mColor		= color;
				overlaynonetexbuffer ++;
				overlaynonetexbuffer->mPosition.x	= rectangle->mPositionRB.x;
				overlaynonetexbuffer->mPosition.y	= rectangle->mPositionLT.y;
				overlaynonetexbuffer->mPosition.z	= 1.0f;
				overlaynonetexbuffer->mPosition.w	= 1.0f;
				overlaynonetexbuffer->mColor		= color;
				overlaynonetexbuffer ++;
				overlaynonetexbuffer->mPosition.x	= rectangle->mPositionLT.x;
				overlaynonetexbuffer->mPosition.y	= rectangle->mPositionRB.y;
				overlaynonetexbuffer->mPosition.z	= 1.0f;
				overlaynonetexbuffer->mPosition.w	= 1.0f;
				overlaynonetexbuffer->mColor		= color;
				overlaynonetexbuffer ++;

				overlaynonetexbuffer->mPosition.x	= rectangle->mPositionLT.x;
				overlaynonetexbuffer->mPosition.y	= rectangle->mPositionRB.y;
				overlaynonetexbuffer->mPosition.z	= 1.0f;
				overlaynonetexbuffer->mPosition.w	= 1.0f;
				overlaynonetexbuffer->mColor		= color;
				overlaynonetexbuffer ++;
				overlaynonetexbuffer->mPosition.x	= rectangle->mPositionRB.x;
				overlaynonetexbuffer->mPosition.y	= rectangle->mPositionLT.y;
				overlaynonetexbuffer->mPosition.z	= 1.0f;
				overlaynonetexbuffer->mPosition.w	= 1.0f;
				overlaynonetexbuffer->mColor		= color;
				overlaynonetexbuffer ++;
				overlaynonetexbuffer->mPosition.x	= rectangle->mPositionRB.x;
				overlaynonetexbuffer->mPosition.y	= rectangle->mPositionRB.y;
				overlaynonetexbuffer->mPosition.z	= 1.0f;
				overlaynonetexbuffer->mPosition.w	= 1.0f;
				overlaynonetexbuffer->mColor		= color;
				overlaynonetexbuffer ++;

				break;
			}

			case Renderable::_TYPE_OVERLAY_RECTANGLEIMAGE:
			{
				OverlayRectangleImage* rectangleimage = (OverlayRectangleImage*) renderunit.mRenderable;

				if ( overlaywithtexbuffer == _null )
					continue;

				geometry.mVertexDeclaration	= mOverlayWithTexVD;
				geometry.mVertexBuffer		= mOverlayWithTexVB;
				geometry.mIndexBuffer		= _null;
				geometry.mTexture[0]		= rectangleimage->mTexture;
				geometry.mGeometryType		= Geometry::_GEOMETRY_TRIANGLELIST;
				geometry.mGeometryNumber	= 2 + 2 * renderunit.mLinkNumber;
				geometry.mVertexSize		= sizeof( OverlayVertexWithTex );
				geometry.mVertexNumber		= 6 + 6 * renderunit.mLinkNumber;
				geometry.mStartVertex		= overlaywithtexvi;
				geometry.mStartIndex		= 0;

				overlaywithtexvi += geometry.mVertexNumber;

				_dword color = GraphicsManager::GetRenderAPIHelper( ).ConvertColor( rectangleimage->mColor );

				// Fill vertex buffer.
				do
				{
					overlaywithtexbuffer->mPosition.x	= rectangleimage->mPositionLT.x;
					overlaywithtexbuffer->mPosition.y	= rectangleimage->mPositionLT.y;
					overlaywithtexbuffer->mPosition.z	= 1.0f;
					overlaywithtexbuffer->mPosition.w	= 1.0f;
					overlaywithtexbuffer->mColor		= color;
					overlaywithtexbuffer->mTexcoord.x	= rectangleimage->mTexcoordLT.x;
					overlaywithtexbuffer->mTexcoord.y	= rectangleimage->mTexcoordLT.y;
					overlaywithtexbuffer ++;
					overlaywithtexbuffer->mPosition.x	= rectangleimage->mPositionRB.x;
					overlaywithtexbuffer->mPosition.y	= rectangleimage->mPositionLT.y;
					overlaywithtexbuffer->mPosition.z	= 1.0f;
					overlaywithtexbuffer->mPosition.w	= 1.0f;
					overlaywithtexbuffer->mColor		= color;
					overlaywithtexbuffer->mTexcoord.x	= rectangleimage->mTexcoordRB.x;
					overlaywithtexbuffer->mTexcoord.y	= rectangleimage->mTexcoordLT.y;
					overlaywithtexbuffer ++;
					overlaywithtexbuffer->mPosition.x	= rectangleimage->mPositionLT.x;
					overlaywithtexbuffer->mPosition.y	= rectangleimage->mPositionRB.y;
					overlaywithtexbuffer->mPosition.z	= 1.0f;
					overlaywithtexbuffer->mPosition.w	= 1.0f;
					overlaywithtexbuffer->mColor		= color;
					overlaywithtexbuffer->mTexcoord.x	= rectangleimage->mTexcoordLT.x;
					overlaywithtexbuffer->mTexcoord.y	= rectangleimage->mTexcoordRB.y;
					overlaywithtexbuffer ++;

					overlaywithtexbuffer->mPosition.x	= rectangleimage->mPositionLT.x;
					overlaywithtexbuffer->mPosition.y	= rectangleimage->mPositionRB.y;
					overlaywithtexbuffer->mPosition.z	= 1.0f;
					overlaywithtexbuffer->mPosition.w	= 1.0f;
					overlaywithtexbuffer->mColor		= color;
					overlaywithtexbuffer->mTexcoord.x	= rectangleimage->mTexcoordLT.x;
					overlaywithtexbuffer->mTexcoord.y	= rectangleimage->mTexcoordRB.y;
					overlaywithtexbuffer ++;
					overlaywithtexbuffer->mPosition.x	= rectangleimage->mPositionRB.x;
					overlaywithtexbuffer->mPosition.y	= rectangleimage->mPositionLT.y;
					overlaywithtexbuffer->mPosition.z	= 1.0f;
					overlaywithtexbuffer->mPosition.w	= 1.0f;
					overlaywithtexbuffer->mColor		= color;
					overlaywithtexbuffer->mTexcoord.x	= rectangleimage->mTexcoordRB.x;
					overlaywithtexbuffer->mTexcoord.y	= rectangleimage->mTexcoordLT.y;
					overlaywithtexbuffer ++;
					overlaywithtexbuffer->mPosition.x	= rectangleimage->mPositionRB.x;
					overlaywithtexbuffer->mPosition.y	= rectangleimage->mPositionRB.y;
					overlaywithtexbuffer->mPosition.z	= 1.0f;
					overlaywithtexbuffer->mPosition.w	= 1.0f;
					overlaywithtexbuffer->mColor		= color;
					overlaywithtexbuffer->mTexcoord.x	= rectangleimage->mTexcoordRB.x;
					overlaywithtexbuffer->mTexcoord.y	= rectangleimage->mTexcoordRB.y;
					overlaywithtexbuffer ++;

					if ( rectangleimage == renderunit.mRenderable )
						rectangleimage = (OverlayRectangleImage*) renderunit.mLinkHead;
					else
						rectangleimage = (OverlayRectangleImage*) rectangleimage->mType;
				}
				while ( rectangleimage != _null );

				break;
			}

			case Renderable::_TYPE_OVERLAY_QUADRANGLE:
			{
				OverlayQuadrangle* quadrangle = (OverlayQuadrangle*) renderunit.mRenderable;

				if ( overlaynonetexbuffer == _null )
					continue;

				geometry.mVertexDeclaration	= mOverlayNoneTexVD;
				geometry.mVertexBuffer		= mOverlayNoneTexVB;
				geometry.mIndexBuffer		= _null;
				geometry.mGeometryType		= Geometry::_GEOMETRY_TRIANGLELIST;
				geometry.mGeometryNumber	= 2;
				geometry.mVertexSize		= sizeof( OverlayVertexNoneTex );
				geometry.mVertexNumber		= 6;
				geometry.mStartVertex		= overlaynonetexvi;
				geometry.mStartIndex		= 0;

				overlaynonetexvi += 6;

				// Fill vertex buffer.
				overlaynonetexbuffer->mPosition.x	= quadrangle->mPositionLT.x;
				overlaynonetexbuffer->mPosition.y	= quadrangle->mPositionLT.y;
				overlaynonetexbuffer->mPosition.z	= 1.0f;
				overlaynonetexbuffer->mPosition.w	= 1.0f;
				overlaynonetexbuffer->mColor		= GraphicsManager::GetRenderAPIHelper( ).ConvertColor( quadrangle->mColorLT );
				overlaynonetexbuffer ++;
				overlaynonetexbuffer->mPosition.x	= quadrangle->mPositionRT.x;
				overlaynonetexbuffer->mPosition.y	= quadrangle->mPositionRT.y;
				overlaynonetexbuffer->mPosition.z	= 1.0f;
				overlaynonetexbuffer->mPosition.w	= 1.0f;
				overlaynonetexbuffer->mColor		= GraphicsManager::GetRenderAPIHelper( ).ConvertColor( quadrangle->mColorRT );
				overlaynonetexbuffer ++;
				overlaynonetexbuffer->mPosition.x	= quadrangle->mPositionLB.x;
				overlaynonetexbuffer->mPosition.y	= quadrangle->mPositionLB.y;
				overlaynonetexbuffer->mPosition.z	= 1.0f;
				overlaynonetexbuffer->mPosition.w	= 1.0f;
				overlaynonetexbuffer->mColor		= GraphicsManager::GetRenderAPIHelper( ).ConvertColor( quadrangle->mColorLB );
				overlaynonetexbuffer ++;

				overlaynonetexbuffer->mPosition.x	= quadrangle->mPositionLB.x;
				overlaynonetexbuffer->mPosition.y	= quadrangle->mPositionLB.y;
				overlaynonetexbuffer->mPosition.z	= 1.0f;
				overlaynonetexbuffer->mPosition.w	= 1.0f;
				overlaynonetexbuffer->mColor		= GraphicsManager::GetRenderAPIHelper( ).ConvertColor( quadrangle->mColorLB );
				overlaynonetexbuffer ++;
				overlaynonetexbuffer->mPosition.x	= quadrangle->mPositionRT.x;
				overlaynonetexbuffer->mPosition.y	= quadrangle->mPositionRT.y;
				overlaynonetexbuffer->mPosition.z	= 1.0f;
				overlaynonetexbuffer->mPosition.w	= 1.0f;
				overlaynonetexbuffer->mColor		= GraphicsManager::GetRenderAPIHelper( ).ConvertColor( quadrangle->mColorRT );
				overlaynonetexbuffer ++;
				overlaynonetexbuffer->mPosition.x	= quadrangle->mPositionRB.x;
				overlaynonetexbuffer->mPosition.y	= quadrangle->mPositionRB.y;
				overlaynonetexbuffer->mPosition.z	= 1.0f;
				overlaynonetexbuffer->mPosition.w	= 1.0f;
				overlaynonetexbuffer->mColor		= GraphicsManager::GetRenderAPIHelper( ).ConvertColor( quadrangle->mColorRB );
				overlaynonetexbuffer ++;

				break;
			}

			case Renderable::_TYPE_OVERLAY_QUADRANGLEIMAGE:
			{
				OverlayQuadrangleImage* quadrangleimage = (OverlayQuadrangleImage*) renderunit.mRenderable;

				if ( overlaywithtexbuffer == _null )
					continue;

				geometry.mVertexDeclaration	= mOverlayWithTexVD;
				geometry.mVertexBuffer		= mOverlayWithTexVB;
				geometry.mIndexBuffer		= _null;
				geometry.mTexture[0]		= quadrangleimage->mTexture;
				geometry.mGeometryType		= Geometry::_GEOMETRY_TRIANGLELIST;
				geometry.mGeometryNumber	= 2 + 2 * renderunit.mLinkNumber;
				geometry.mVertexSize		= sizeof( OverlayVertexWithTex );
				geometry.mVertexNumber		= 6 + 6 * renderunit.mLinkNumber;
				geometry.mStartVertex		= overlaywithtexvi;
				geometry.mStartIndex		= 0;

				overlaywithtexvi += geometry.mVertexNumber;

				// Fill vertex buffer.
				do
				{
					overlaywithtexbuffer->mPosition.x	= quadrangleimage->mPositionLT.x;
					overlaywithtexbuffer->mPosition.y	= quadrangleimage->mPositionLT.y;
					overlaywithtexbuffer->mPosition.z	= 1.0f;
					overlaywithtexbuffer->mPosition.w	= 1.0f;
					overlaywithtexbuffer->mColor		= GraphicsManager::GetRenderAPIHelper( ).ConvertColor( quadrangleimage->mColorLT );
					overlaywithtexbuffer->mTexcoord.x	= quadrangleimage->mTexcoordLT.x;
					overlaywithtexbuffer->mTexcoord.y	= quadrangleimage->mTexcoordLT.y;
					overlaywithtexbuffer ++;
					overlaywithtexbuffer->mPosition.x	= quadrangleimage->mPositionRT.x;
					overlaywithtexbuffer->mPosition.y	= quadrangleimage->mPositionRT.y;
					overlaywithtexbuffer->mPosition.z	= 1.0f;
					overlaywithtexbuffer->mPosition.w	= 1.0f;
					overlaywithtexbuffer->mColor		= GraphicsManager::GetRenderAPIHelper( ).ConvertColor( quadrangleimage->mColorRT );
					overlaywithtexbuffer->mTexcoord.x	= quadrangleimage->mTexcoordRT.x;
					overlaywithtexbuffer->mTexcoord.y	= quadrangleimage->mTexcoordRT.y;
					overlaywithtexbuffer ++;
					overlaywithtexbuffer->mPosition.x	= quadrangleimage->mPositionLB.x;
					overlaywithtexbuffer->mPosition.y	= quadrangleimage->mPositionLB.y;
					overlaywithtexbuffer->mPosition.z	= 1.0f;
					overlaywithtexbuffer->mPosition.w	= 1.0f;
					overlaywithtexbuffer->mColor		= GraphicsManager::GetRenderAPIHelper( ).ConvertColor( quadrangleimage->mColorLB );
					overlaywithtexbuffer->mTexcoord.x	= quadrangleimage->mTexcoordLB.x;
					overlaywithtexbuffer->mTexcoord.y	= quadrangleimage->mTexcoordLB.y;
					overlaywithtexbuffer ++;

					overlaywithtexbuffer->mPosition.x	= quadrangleimage->mPositionLB.x;
					overlaywithtexbuffer->mPosition.y	= quadrangleimage->mPositionLB.y;
					overlaywithtexbuffer->mPosition.z	= 1.0f;
					overlaywithtexbuffer->mPosition.w	= 1.0f;
					overlaywithtexbuffer->mColor		= GraphicsManager::GetRenderAPIHelper( ).ConvertColor( quadrangleimage->mColorLB );
					overlaywithtexbuffer->mTexcoord.x	= quadrangleimage->mTexcoordLB.x;
					overlaywithtexbuffer->mTexcoord.y	= quadrangleimage->mTexcoordLB.y;
					overlaywithtexbuffer ++;
					overlaywithtexbuffer->mPosition.x	= quadrangleimage->mPositionRT.x;
					overlaywithtexbuffer->mPosition.y	= quadrangleimage->mPositionRT.y;
					overlaywithtexbuffer->mPosition.z	= 1.0f;
					overlaywithtexbuffer->mPosition.w	= 1.0f;
					overlaywithtexbuffer->mColor		= GraphicsManager::GetRenderAPIHelper( ).ConvertColor( quadrangleimage->mColorRT );
					overlaywithtexbuffer->mTexcoord.x	= quadrangleimage->mTexcoordRT.x;
					overlaywithtexbuffer->mTexcoord.y	= quadrangleimage->mTexcoordRT.y;
					overlaywithtexbuffer ++;
					overlaywithtexbuffer->mPosition.x	= quadrangleimage->mPositionRB.x;
					overlaywithtexbuffer->mPosition.y	= quadrangleimage->mPositionRB.y;
					overlaywithtexbuffer->mPosition.z	= 1.0f;
					overlaywithtexbuffer->mPosition.w	= 1.0f;
					overlaywithtexbuffer->mColor		= GraphicsManager::GetRenderAPIHelper( ).ConvertColor( quadrangleimage->mColorRB );
					overlaywithtexbuffer->mTexcoord.x	= quadrangleimage->mTexcoordRB.x;
					overlaywithtexbuffer->mTexcoord.y	= quadrangleimage->mTexcoordRB.y;
					overlaywithtexbuffer ++;

					if ( quadrangleimage == renderunit.mRenderable )
						quadrangleimage = (OverlayQuadrangleImage*) renderunit.mLinkHead;
					else
						quadrangleimage = (OverlayQuadrangleImage*) quadrangleimage->mType;
				}
				while ( quadrangleimage != _null );

				break;
			}

			case Renderable::_TYPE_PRIMITIVE_POINT:
			{
				PrimitivePoint* point = (PrimitivePoint*) renderunit.mRenderable;

				if ( primitivenonetexbuffer == _null )
					continue;

				geometry.mVertexDeclaration	= mPrimitiveNoneTexVD;
				geometry.mVertexBuffer		= mPrimitiveNoneTexVB;
				geometry.mIndexBuffer		= _null;
				geometry.mGeometryType		= Geometry::_GEOMETRY_POINTLIST;
				geometry.mGeometryNumber	= 1;
				geometry.mVertexSize		= sizeof( PrimitiveVertexNoneTex );
				geometry.mVertexNumber		= 1;
				geometry.mStartVertex		= primitivenonetexvi;
				geometry.mStartIndex		= 0;

				primitivenonetexvi += 1;

				// Fill vertex buffer.
				primitivenonetexbuffer->mPosition	= point->mPosition;
				primitivenonetexbuffer->mColor		= GraphicsManager::GetRenderAPIHelper( ).ConvertColor( point->mColor );
				primitivenonetexbuffer ++;

				break;
			}

			case Renderable::_TYPE_PRIMITIVE_LINE:
			{
				PrimitiveLine* line = (PrimitiveLine*) renderunit.mRenderable;

				if ( primitivenonetexbuffer == _null )
					continue;

				geometry.mVertexDeclaration	= mPrimitiveNoneTexVD;
				geometry.mVertexBuffer		= mPrimitiveNoneTexVB;
				geometry.mIndexBuffer		= _null;
				geometry.mGeometryType		= Geometry::_GEOMETRY_LINELIST;
				geometry.mGeometryNumber	= 1;
				geometry.mVertexSize		= sizeof( PrimitiveVertexNoneTex );
				geometry.mVertexNumber		= 2;
				geometry.mStartVertex		= primitivenonetexvi;
				geometry.mStartIndex		= 0;

				primitivenonetexvi += 2;

				// Fill vertex buffer.
				primitivenonetexbuffer->mPosition	= line->mPosition1;
				primitivenonetexbuffer->mColor		= GraphicsManager::GetRenderAPIHelper( ).ConvertColor( line->mColor1 );
				primitivenonetexbuffer ++;
				primitivenonetexbuffer->mPosition	= line->mPosition2;
				primitivenonetexbuffer->mColor		= GraphicsManager::GetRenderAPIHelper( ).ConvertColor( line->mColor2 );
				primitivenonetexbuffer ++;

				break;
			}

			case Renderable::_TYPE_PRIMITIVE_TRIANGLE:
			{
				PrimitiveTriangle* triangle = (PrimitiveTriangle*) renderunit.mRenderable;

				if ( primitivenonetexbuffer == _null )
					continue;

				geometry.mVertexDeclaration	= mPrimitiveNoneTexVD;
				geometry.mVertexBuffer		= mPrimitiveNoneTexVB;
				geometry.mIndexBuffer		= _null;
				geometry.mGeometryType		= Geometry::_GEOMETRY_TRIANGLELIST;
				geometry.mGeometryNumber	= 1;
				geometry.mVertexSize		= sizeof( PrimitiveVertexNoneTex );
				geometry.mVertexNumber		= 3;
				geometry.mStartVertex		= primitivenonetexvi;
				geometry.mStartIndex		= 0;

				primitivenonetexvi += 3;

				// Fill vertex buffer.
				primitivenonetexbuffer->mPosition	= triangle->mPosition1;
				primitivenonetexbuffer->mColor		= GraphicsManager::GetRenderAPIHelper( ).ConvertColor( triangle->mColor1 );
				primitivenonetexbuffer ++;
				primitivenonetexbuffer->mPosition	= triangle->mPosition2;
				primitivenonetexbuffer->mColor		= GraphicsManager::GetRenderAPIHelper( ).ConvertColor( triangle->mColor2 );
				primitivenonetexbuffer ++;
				primitivenonetexbuffer->mPosition	= triangle->mPosition3;
				primitivenonetexbuffer->mColor		= GraphicsManager::GetRenderAPIHelper( ).ConvertColor( triangle->mColor3 );
				primitivenonetexbuffer ++;

				break;
			}

			case Renderable::_TYPE_PRIMITIVE_TRIANGLEIMAGE:
			{
				PrimitiveTriangleImage* triangleimage = (PrimitiveTriangleImage*) renderunit.mRenderable;

				if ( primitivewithtexbuffer == _null )
					continue;

				geometry.mVertexDeclaration	= mPrimitiveWithTexVD;
				geometry.mVertexBuffer		= mPrimitiveWithTexVB;
				geometry.mIndexBuffer		= _null;
				geometry.mTexture[0]		= triangleimage->mTexture;
				geometry.mGeometryType		= Geometry::_GEOMETRY_TRIANGLELIST;
				geometry.mGeometryNumber	= 1;
				geometry.mVertexSize		= sizeof( PrimitiveVertexWithTex );
				geometry.mVertexNumber		= 3;
				geometry.mStartVertex		= primitivewithtexvi;
				geometry.mStartIndex		= 0;

				primitivewithtexvi += 3;

				// Fill vertex buffer.
				primitivewithtexbuffer->mPosition	= triangleimage->mPosition1;
				primitivewithtexbuffer->mColor		= GraphicsManager::GetRenderAPIHelper( ).ConvertColor( triangleimage->mColor1 );
				primitivewithtexbuffer->mTexcoord	= triangleimage->mTexcoord1;
				primitivewithtexbuffer ++;
				primitivewithtexbuffer->mPosition	= triangleimage->mPosition2;
				primitivewithtexbuffer->mColor		= GraphicsManager::GetRenderAPIHelper( ).ConvertColor( triangleimage->mColor2 );
				primitivewithtexbuffer->mTexcoord	= triangleimage->mTexcoord2;
				primitivewithtexbuffer ++;
				primitivewithtexbuffer->mPosition	= triangleimage->mPosition3;
				primitivewithtexbuffer->mColor		= GraphicsManager::GetRenderAPIHelper( ).ConvertColor( triangleimage->mColor3 );
				primitivewithtexbuffer->mTexcoord	= triangleimage->mTexcoord3;
				primitivewithtexbuffer ++;

				break;
			}

			case Renderable::_TYPE_PRIMITIVE_QUADRANGLE:
			{
				PrimitiveQuadrangle* quadrangle = (PrimitiveQuadrangle*) renderunit.mRenderable;

				if ( primitivenonetexbuffer == _null )
					continue;

				geometry.mVertexDeclaration	= mPrimitiveNoneTexVD;
				geometry.mVertexBuffer		= mPrimitiveNoneTexVB;
				geometry.mIndexBuffer		= _null;
				geometry.mGeometryType		= Geometry::_GEOMETRY_LINELIST;
				geometry.mGeometryNumber	= 4;
				geometry.mVertexSize		= sizeof( PrimitiveVertexNoneTex );
				geometry.mVertexNumber		= 8;
				geometry.mStartVertex		= primitivenonetexvi;
				geometry.mStartIndex		= 0;

				primitivenonetexvi += 8;

				_dword color = GraphicsManager::GetRenderAPIHelper( ).ConvertColor( quadrangle->mColor );

				// Fill vertex buffer.
				primitivenonetexbuffer->mPosition	= quadrangle->mCenter - quadrangle->mWidth - quadrangle->mHeight;
				primitivenonetexbuffer->mColor		= color;
				primitivenonetexbuffer ++;
				primitivenonetexbuffer->mPosition	= quadrangle->mCenter + quadrangle->mWidth - quadrangle->mHeight;
				primitivenonetexbuffer->mColor		= color;
				primitivenonetexbuffer ++;

				primitivenonetexbuffer->mPosition	= quadrangle->mCenter + quadrangle->mWidth - quadrangle->mHeight;
				primitivenonetexbuffer->mColor		= color;
				primitivenonetexbuffer ++;
				primitivenonetexbuffer->mPosition	= quadrangle->mCenter + quadrangle->mWidth + quadrangle->mHeight;
				primitivenonetexbuffer->mColor		= color;
				primitivenonetexbuffer ++;

				primitivenonetexbuffer->mPosition	= quadrangle->mCenter + quadrangle->mWidth + quadrangle->mHeight;
				primitivenonetexbuffer->mColor		= color;
				primitivenonetexbuffer ++;
				primitivenonetexbuffer->mPosition	= quadrangle->mCenter - quadrangle->mWidth + quadrangle->mHeight;
				primitivenonetexbuffer->mColor		= color;
				primitivenonetexbuffer ++;

				primitivenonetexbuffer->mPosition	= quadrangle->mCenter - quadrangle->mWidth + quadrangle->mHeight;
				primitivenonetexbuffer->mColor		= color;
				primitivenonetexbuffer ++;
				primitivenonetexbuffer->mPosition	= quadrangle->mCenter - quadrangle->mWidth - quadrangle->mHeight;
				primitivenonetexbuffer->mColor		= color;
				primitivenonetexbuffer ++;

				break;
			}

			case Renderable::_TYPE_PRIMITIVE_QUADRANGLEIMAGE:
			{
				PrimitiveQuadrangleImage* quadrangleimage = (PrimitiveQuadrangleImage*) renderunit.mRenderable;

				if ( primitivewithtexbuffer == _null )
					continue;

				geometry.mVertexDeclaration	= mPrimitiveWithTexVD;
				geometry.mVertexBuffer		= mPrimitiveWithTexVB;
				geometry.mIndexBuffer		= _null;
				geometry.mTexture[0]		= quadrangleimage->mTexture;
				geometry.mGeometryType		= Geometry::_GEOMETRY_TRIANGLELIST;
				geometry.mGeometryNumber	= 2;
				geometry.mVertexSize		= sizeof( PrimitiveVertexWithTex );
				geometry.mVertexNumber		= 6;
				geometry.mStartVertex		= primitivewithtexvi;
				geometry.mStartIndex		= 0;

				primitivewithtexvi += 6;

				_dword color = GraphicsManager::GetRenderAPIHelper( ).ConvertColor( quadrangleimage->mColor );

				// Fill vertex buffer.
				primitivewithtexbuffer->mPosition	= quadrangleimage->mCenter - quadrangleimage->mWidth - quadrangleimage->mHeight;
				primitivewithtexbuffer->mColor		= color;
				primitivewithtexbuffer->mTexcoord	= quadrangleimage->mTexcoordLT;
				primitivewithtexbuffer ++;
				primitivewithtexbuffer->mPosition	= quadrangleimage->mCenter + quadrangleimage->mWidth - quadrangleimage->mHeight;
				primitivewithtexbuffer->mColor		= color;
				primitivewithtexbuffer->mTexcoord	= quadrangleimage->mTexcoordRT;
				primitivewithtexbuffer ++;
				primitivewithtexbuffer->mPosition	= quadrangleimage->mCenter - quadrangleimage->mWidth + quadrangleimage->mHeight;
				primitivewithtexbuffer->mColor		= color;
				primitivewithtexbuffer->mTexcoord	= quadrangleimage->mTexcoordLB;
				primitivewithtexbuffer ++;

				primitivewithtexbuffer->mPosition	= quadrangleimage->mCenter - quadrangleimage->mWidth + quadrangleimage->mHeight;
				primitivewithtexbuffer->mColor		= color;
				primitivewithtexbuffer->mTexcoord	= quadrangleimage->mTexcoordLB;
				primitivewithtexbuffer ++;
				primitivewithtexbuffer->mPosition	= quadrangleimage->mCenter + quadrangleimage->mWidth - quadrangleimage->mHeight;
				primitivewithtexbuffer->mColor		= color;
				primitivewithtexbuffer->mTexcoord	= quadrangleimage->mTexcoordRT;
				primitivewithtexbuffer ++;
				primitivewithtexbuffer->mPosition	= quadrangleimage->mCenter + quadrangleimage->mWidth + quadrangleimage->mHeight;
				primitivewithtexbuffer->mColor		= color;
				primitivewithtexbuffer->mTexcoord	= quadrangleimage->mTexcoordRB;
				primitivewithtexbuffer ++;

				break;
			}

			case Renderable::_TYPE_PRIMITIVE_BOX:
			{
				PrimitiveBox* box = (PrimitiveBox*) renderunit.mRenderable;

				if ( primitivenonetexbuffer == _null )
					continue;

				geometry.mVertexDeclaration	= mPrimitiveNoneTexVD;
				geometry.mVertexBuffer		= mPrimitiveNoneTexVB;
				geometry.mIndexBuffer		= _null;
				geometry.mGeometryType		= Geometry::_GEOMETRY_LINELIST;
				geometry.mGeometryNumber	= 12;
				geometry.mVertexSize		= sizeof( PrimitiveVertexNoneTex );
				geometry.mVertexNumber		= 24;
				geometry.mStartVertex		= primitivenonetexvi;
				geometry.mStartIndex		= 0;

				primitivenonetexvi += 24;

				_dword color = GraphicsManager::GetRenderAPIHelper( ).ConvertColor( box->mColor );

				// Bottom line 1
				primitivenonetexbuffer->mPosition	= box->mCenter - box->mWidth - box->mHeight - box->mDepth;
				primitivenonetexbuffer->mColor		= color;
				primitivenonetexbuffer ++;
				primitivenonetexbuffer->mPosition	= box->mCenter + box->mWidth - box->mHeight - box->mDepth;
				primitivenonetexbuffer->mColor		= color;
				primitivenonetexbuffer ++;

				// Bottom line 2
				primitivenonetexbuffer->mPosition	= box->mCenter + box->mWidth - box->mHeight - box->mDepth;
				primitivenonetexbuffer->mColor		= color;
				primitivenonetexbuffer ++;
				primitivenonetexbuffer->mPosition	= box->mCenter + box->mWidth + box->mHeight - box->mDepth;
				primitivenonetexbuffer->mColor		= color;
				primitivenonetexbuffer ++;

				// Bottom line 3
				primitivenonetexbuffer->mPosition	= box->mCenter + box->mWidth + box->mHeight - box->mDepth;
				primitivenonetexbuffer->mColor		= color;
				primitivenonetexbuffer ++;
				primitivenonetexbuffer->mPosition	= box->mCenter - box->mWidth + box->mHeight - box->mDepth;
				primitivenonetexbuffer->mColor		= color;
				primitivenonetexbuffer ++;

				// Bottom line 4
				primitivenonetexbuffer->mPosition	= box->mCenter - box->mWidth + box->mHeight - box->mDepth;
				primitivenonetexbuffer->mColor		= color;
				primitivenonetexbuffer ++;
				primitivenonetexbuffer->mPosition	= box->mCenter - box->mWidth - box->mHeight - box->mDepth;
				primitivenonetexbuffer->mColor		= color;
				primitivenonetexbuffer ++;

				// Top line 1
				primitivenonetexbuffer->mPosition	= box->mCenter - box->mWidth - box->mHeight + box->mDepth;
				primitivenonetexbuffer->mColor		= color;
				primitivenonetexbuffer ++;
				primitivenonetexbuffer->mPosition	= box->mCenter + box->mWidth - box->mHeight + box->mDepth;
				primitivenonetexbuffer->mColor		= color;
				primitivenonetexbuffer ++;

				// Top line 2
				primitivenonetexbuffer->mPosition	= box->mCenter + box->mWidth - box->mHeight + box->mDepth;
				primitivenonetexbuffer->mColor		= color;
				primitivenonetexbuffer ++;
				primitivenonetexbuffer->mPosition	= box->mCenter + box->mWidth + box->mHeight + box->mDepth;
				primitivenonetexbuffer->mColor		= color;
				primitivenonetexbuffer ++;

				// Top line 3
				primitivenonetexbuffer->mPosition	= box->mCenter + box->mWidth + box->mHeight + box->mDepth;
				primitivenonetexbuffer->mColor		= color;
				primitivenonetexbuffer ++;
				primitivenonetexbuffer->mPosition	= box->mCenter - box->mWidth + box->mHeight + box->mDepth;
				primitivenonetexbuffer->mColor		= color;
				primitivenonetexbuffer ++;

				// Top line 4
				primitivenonetexbuffer->mPosition	= box->mCenter - box->mWidth + box->mHeight + box->mDepth;
				primitivenonetexbuffer->mColor		= color;
				primitivenonetexbuffer ++;
				primitivenonetexbuffer->mPosition	= box->mCenter - box->mWidth - box->mHeight + box->mDepth;
				primitivenonetexbuffer->mColor		= color;
				primitivenonetexbuffer ++;

				// Side line 1
				primitivenonetexbuffer->mPosition	= box->mCenter - box->mWidth - box->mHeight - box->mDepth;
				primitivenonetexbuffer->mColor		= color;
				primitivenonetexbuffer ++;
				primitivenonetexbuffer->mPosition	= box->mCenter - box->mWidth - box->mHeight + box->mDepth;
				primitivenonetexbuffer->mColor		= color;
				primitivenonetexbuffer ++;

				// Side line 2
				primitivenonetexbuffer->mPosition	= box->mCenter + box->mWidth - box->mHeight - box->mDepth;
				primitivenonetexbuffer->mColor		= color;
				primitivenonetexbuffer ++;
				primitivenonetexbuffer->mPosition	= box->mCenter + box->mWidth - box->mHeight + box->mDepth;
				primitivenonetexbuffer->mColor		= color;
				primitivenonetexbuffer ++;

				// Side line 3
				primitivenonetexbuffer->mPosition	= box->mCenter + box->mWidth + box->mHeight - box->mDepth;
				primitivenonetexbuffer->mColor		= color;
				primitivenonetexbuffer ++;
				primitivenonetexbuffer->mPosition	= box->mCenter + box->mWidth + box->mHeight + box->mDepth;
				primitivenonetexbuffer->mColor		= color;
				primitivenonetexbuffer ++;

				// Side line 4
				primitivenonetexbuffer->mPosition	= box->mCenter - box->mWidth + box->mHeight - box->mDepth;
				primitivenonetexbuffer->mColor		= color;
				primitivenonetexbuffer ++;
				primitivenonetexbuffer->mPosition	= box->mCenter - box->mWidth + box->mHeight + box->mDepth;
				primitivenonetexbuffer->mColor		= color;
				primitivenonetexbuffer ++;

				break;
			}

			case Renderable::_TYPE_PRIMITIVE_CIRCLE:
			{
				PrimitiveCircle* circle = (PrimitiveCircle*) renderunit.mRenderable;

				if ( primitivenonetexbuffer == _null )
					continue;

				geometry.mVertexDeclaration	= mPrimitiveNoneTexVD;
				geometry.mVertexBuffer		= mPrimitiveNoneTexVB;
				geometry.mIndexBuffer		= _null;
				geometry.mGeometryType		= Geometry::_GEOMETRY_LINELIST;
				geometry.mGeometryNumber	= circle->mSegment;
				geometry.mVertexSize		= 16;
				geometry.mVertexNumber		= circle->mSegment * 2;
				geometry.mStartVertex		= primitivenonetexvi;
				geometry.mStartIndex		= 0;

				primitivenonetexvi += circle->mSegment * 2;

				_dword color = GraphicsManager::GetRenderAPIHelper( ).ConvertColor( circle->mColor );

				// Fill Vertex buffer.

				Matrix4 rotation;
				rotation.Rotation( Vector3::cZAxis, circle->mNormal );

				_float as = Math::cPi / circle->mSegment * 2.0f, r = circle->mRadius;

				for ( _dword i = 0; i < circle->mSegment; i ++ )
				{
					_float x1 = Math::Sin( i * as );
					_float y1 = Math::Cos( i * as );
					_float x2 = Math::Sin( ( i + 1 ) * as );
					_float y2 = Math::Cos( ( i + 1 ) * as );

					primitivenonetexbuffer->mPosition	= circle->mCenter + Vector3( x1 * r, y1 * r, 0.0f ) * rotation;
					primitivenonetexbuffer->mColor		= color;
					primitivenonetexbuffer ++;

					primitivenonetexbuffer->mPosition	= circle->mCenter + Vector3( x2 * r, y2 * r, 0.0f ) * rotation;
					primitivenonetexbuffer->mColor		= color;
					primitivenonetexbuffer ++;
				}

				break;
			}

			case Renderable::_TYPE_PRIMITIVE_SPHERE:
			{
				PrimitiveSphere* sphere = (PrimitiveSphere*) renderunit.mRenderable;

				if ( primitivenonetexbuffer == _null )
					continue;

				geometry.mVertexDeclaration	= mPrimitiveNoneTexVD;
				geometry.mVertexBuffer		= mPrimitiveNoneTexVB;
				geometry.mIndexBuffer		= _null;
				geometry.mGeometryType		= Geometry::_GEOMETRY_LINELIST;
				geometry.mGeometryNumber	= sphere->mSegment * 3;
				geometry.mVertexSize		= sizeof( PrimitiveVertexNoneTex );
				geometry.mVertexNumber		= sphere->mSegment * 2 * 3;
				geometry.mStartVertex		= primitivenonetexvi;
				geometry.mStartIndex		= 0;

				primitivenonetexvi += sphere->mSegment * 2 * 3;

				_dword color = GraphicsManager::GetRenderAPIHelper( ).ConvertColor( sphere->mColor );

				// Fill Vertex buffer.
				_float as = Math::cPi / sphere->mSegment * 2.0f, r = sphere->mRadius;

				for ( _dword i = 0; i < sphere->mSegment; i ++ )
				{
					_float x1 = Math::Sin( i * as );
					_float y1 = Math::Cos( i * as );
					_float x2 = Math::Sin( ( i + 1 ) * as );
					_float y2 = Math::Cos( ( i + 1 ) * as );

					primitivenonetexbuffer->mPosition	= sphere->mCenter + Vector3( x1 * r, y1 * r, 0.0f );
					primitivenonetexbuffer->mColor		= color;
					primitivenonetexbuffer ++;

					primitivenonetexbuffer->mPosition	= sphere->mCenter + Vector3( x2 * r, y2 * r, 0.0f );
					primitivenonetexbuffer->mColor		= color;
					primitivenonetexbuffer ++;
				}

				for ( _dword j = 0; j < sphere->mSegment; j ++ )
				{
					_float x1 = Math::Sin( j * as );
					_float y1 = Math::Cos( j * as );
					_float x2 = Math::Sin( ( j + 1 ) * as );
					_float y2 = Math::Cos( ( j + 1 ) * as );

					primitivenonetexbuffer->mPosition	= sphere->mCenter + Vector3( x1 * r, 0.0f, y1 * r );
					primitivenonetexbuffer->mColor		= color;
					primitivenonetexbuffer ++;

					primitivenonetexbuffer->mPosition	= sphere->mCenter + Vector3( x2 * r, 0.0f, y2 * r );
					primitivenonetexbuffer->mColor		= color;
					primitivenonetexbuffer ++;
				}

				for ( _dword k = 0; k < sphere->mSegment; k ++ )
				{
					_float x1 = Math::Sin( k * as );
					_float y1 = Math::Cos( k * as );
					_float x2 = Math::Sin( ( k + 1 ) * as );
					_float y2 = Math::Cos( ( k + 1 ) * as );

					primitivenonetexbuffer->mPosition	= sphere->mCenter + Vector3( 0.0f, x1 * r, y1 * r );
					primitivenonetexbuffer->mColor		= color;
					primitivenonetexbuffer ++;

					primitivenonetexbuffer->mPosition	= sphere->mCenter + Vector3( 0.0f, x2 * r, y2 * r );
					primitivenonetexbuffer->mColor		= color;
					primitivenonetexbuffer ++;
				}

				break;
			}

			case Renderable::_TYPE_PRIMITIVE_PYRAMID:
			{
				PrimitivePyramid* pyramid = (PrimitivePyramid*) renderunit.mRenderable;

				if ( primitivenonetexbuffer == _null )
					continue;

				geometry.mVertexDeclaration	= mPrimitiveNoneTexVD;
				geometry.mVertexBuffer		= mPrimitiveNoneTexVB;
				geometry.mIndexBuffer		= _null;
				geometry.mGeometryType		= Geometry::_GEOMETRY_LINELIST;
				geometry.mGeometryNumber	= 8;
				geometry.mVertexSize		= sizeof( PrimitiveVertexNoneTex );
				geometry.mVertexNumber		= 16;
				geometry.mStartVertex		= primitivenonetexvi;
				geometry.mStartIndex		= 0;

				primitivenonetexvi += 16;

				_dword color = GraphicsManager::GetRenderAPIHelper( ).ConvertColor( pyramid->mColor );

				// Fill Vertex buffer.
				Vector3 height	= pyramid->mPeak - pyramid->mBase;
				Vector3 vector0 = Vector3( 0.0f, 0.0f, height.Magnitude( ) );
				Vector3 vector1 = Vector3(   pyramid->mRadius,   pyramid->mRadius, 0.0f );
				Vector3 vector2 = Vector3( - pyramid->mRadius,   pyramid->mRadius, 0.0f );
				Vector3 vector3 = Vector3( - pyramid->mRadius, - pyramid->mRadius, 0.0f );
				Vector3 vector4 = Vector3(   pyramid->mRadius, - pyramid->mRadius, 0.0f );

				Matrix4 transform = Matrix4( ).Rotation( vector0, height ) *
					Matrix4( ).Translation( pyramid->mBase );

				vector0 *= transform;
				vector1 *= transform;
				vector2 *= transform;
				vector3 *= transform;
				vector4 *= transform;

				// Bottom line 1
				primitivenonetexbuffer->mPosition	= vector1;
				primitivenonetexbuffer->mColor		= color;
				primitivenonetexbuffer ++;
				primitivenonetexbuffer->mPosition	= vector2;
				primitivenonetexbuffer->mColor		= color;
				primitivenonetexbuffer ++;

				// Bottom line 2
				primitivenonetexbuffer->mPosition	= vector2;
				primitivenonetexbuffer->mColor		= color;
				primitivenonetexbuffer ++;
				primitivenonetexbuffer->mPosition	= vector3;
				primitivenonetexbuffer->mColor		= color;
				primitivenonetexbuffer ++;

				// Bottom line 3
				primitivenonetexbuffer->mPosition	= vector3;
				primitivenonetexbuffer->mColor		= color;
				primitivenonetexbuffer ++;
				primitivenonetexbuffer->mPosition	= vector4;
				primitivenonetexbuffer->mColor		= color;
				primitivenonetexbuffer ++;

				// Bottom line 4
				primitivenonetexbuffer->mPosition	= vector4;
				primitivenonetexbuffer->mColor		= color;
				primitivenonetexbuffer ++;
				primitivenonetexbuffer->mPosition	= vector1;
				primitivenonetexbuffer->mColor		= color;
				primitivenonetexbuffer ++;

				// Side line 1
				primitivenonetexbuffer->mPosition	= vector0;
				primitivenonetexbuffer->mColor		= color;
				primitivenonetexbuffer ++;
				primitivenonetexbuffer->mPosition	= vector1;
				primitivenonetexbuffer->mColor		= color;
				primitivenonetexbuffer ++;

				// Side line 2
				primitivenonetexbuffer->mPosition	= vector0;
				primitivenonetexbuffer->mColor		= color;
				primitivenonetexbuffer ++;
				primitivenonetexbuffer->mPosition	= vector2;
				primitivenonetexbuffer->mColor		= color;
				primitivenonetexbuffer ++;

				// Side line 3
				primitivenonetexbuffer->mPosition	= vector0;
				primitivenonetexbuffer->mColor		= color;
				primitivenonetexbuffer ++;
				primitivenonetexbuffer->mPosition	= vector3;
				primitivenonetexbuffer->mColor		= color;
				primitivenonetexbuffer ++;

				// Side line 4
				primitivenonetexbuffer->mPosition	= vector0;
				primitivenonetexbuffer->mColor		= color;
				primitivenonetexbuffer ++;
				primitivenonetexbuffer->mPosition	= vector4;
				primitivenonetexbuffer->mColor		= color;
				primitivenonetexbuffer ++;

				break;
			}

			case Renderable::_TYPE_PRIMITIVE_AXIS:
			{
				PrimitiveAxis* axis = (PrimitiveAxis*) renderunit.mRenderable;

				if ( primitivenonetexbuffer == _null )
					continue;

				geometry.mVertexDeclaration	= mPrimitiveNoneTexVD;
				geometry.mVertexBuffer		= mPrimitiveNoneTexVB;
				geometry.mIndexBuffer		= _null;
				geometry.mGeometryType		= Geometry::_GEOMETRY_LINELIST;
				geometry.mGeometryNumber	= 3;
				geometry.mVertexSize		= sizeof( PrimitiveVertexNoneTex );
				geometry.mVertexNumber		= 6;
				geometry.mStartVertex		= primitivenonetexvi;
				geometry.mStartIndex		= 0;

				primitivenonetexvi += 6;

				// Fill Vertex buffer.
				primitivenonetexbuffer->mPosition	= axis->mOrigin;
				primitivenonetexbuffer->mColor		= GraphicsManager::GetRenderAPIHelper( ).ConvertColor( 0xFFFF0000 );
				primitivenonetexbuffer ++;
				primitivenonetexbuffer->mPosition	= axis->mOrigin + axis->mXAxis * axis->mLength;
				primitivenonetexbuffer->mColor		= GraphicsManager::GetRenderAPIHelper( ).ConvertColor( 0xFFFF0000 );
				primitivenonetexbuffer ++;

				primitivenonetexbuffer->mPosition	= axis->mOrigin;
				primitivenonetexbuffer->mColor		= GraphicsManager::GetRenderAPIHelper( ).ConvertColor( 0xFF00FF00 );
				primitivenonetexbuffer ++;
				primitivenonetexbuffer->mPosition	= axis->mOrigin + axis->mYAxis * axis->mLength;
				primitivenonetexbuffer->mColor		= GraphicsManager::GetRenderAPIHelper( ).ConvertColor( 0xFF00FF00 );
				primitivenonetexbuffer ++;

				primitivenonetexbuffer->mPosition	= axis->mOrigin;
				primitivenonetexbuffer->mColor		= GraphicsManager::GetRenderAPIHelper( ).ConvertColor( 0xFF0000FF );
				primitivenonetexbuffer ++;
				primitivenonetexbuffer->mPosition	= axis->mOrigin + Vector3::Cross( axis->mXAxis, axis->mYAxis ) * axis->mLength;
				primitivenonetexbuffer->mColor		= GraphicsManager::GetRenderAPIHelper( ).ConvertColor( 0xFF0000FF );
				primitivenonetexbuffer ++;

				break;
			}

			case Renderable::_TYPE_PRIMITIVE_FRUSTUM:
			{
				PrimitiveFrustum* frustum = (PrimitiveFrustum*) renderunit.mRenderable;

				if ( primitivenonetexbuffer == _null )
					continue;

				geometry.mVertexDeclaration	= mPrimitiveNoneTexVD;
				geometry.mVertexBuffer		= mPrimitiveNoneTexVB;
				geometry.mIndexBuffer		= _null;
				geometry.mGeometryType		= Geometry::_GEOMETRY_LINELIST;
				geometry.mGeometryNumber	= 18;
				geometry.mVertexSize		= sizeof( PrimitiveVertexNoneTex );
				geometry.mVertexNumber		= 36;
				geometry.mStartVertex		= primitivenonetexvi;
				geometry.mStartIndex		= 0;

				primitivenonetexvi += 36;

				_dword color = GraphicsManager::GetRenderAPIHelper( ).ConvertColor( frustum->mColor );

				// Fill Vertex buffer.
				for ( _dword k = 0; k < 4; k ++ )
				{
					primitivenonetexbuffer->mPosition	= frustum->mFrustum.mVectors[ k ];
					primitivenonetexbuffer->mColor		= color;
					primitivenonetexbuffer ++;
					primitivenonetexbuffer->mPosition	= frustum->mFrustum.mVectors[ ( k + 1 ) % 4 ];
					primitivenonetexbuffer->mColor		= color;
					primitivenonetexbuffer ++;
				}

				for ( _dword k = 0; k < 4; k ++ )
				{
					primitivenonetexbuffer->mPosition	= frustum->mFrustum.mVectors[ k + 4 ];
					primitivenonetexbuffer->mColor		= color;
					primitivenonetexbuffer ++;
					primitivenonetexbuffer->mPosition	= frustum->mFrustum.mVectors[ ( k + 1 ) % 4 + 4 ];
					primitivenonetexbuffer->mColor		= color;
					primitivenonetexbuffer ++;
				}

				for ( _dword k = 0; k < 4; k ++ )
				{
					primitivenonetexbuffer->mPosition	= frustum->mFrustum.mVectors[ k ];
					primitivenonetexbuffer->mColor		= color;
					primitivenonetexbuffer ++;
					primitivenonetexbuffer->mPosition	= frustum->mFrustum.mVectors[ k + 4 ];
					primitivenonetexbuffer->mColor		= color;
					primitivenonetexbuffer ++;
				}

				{
					Vector3 center = ( frustum->mFrustum.mVectors[0] + frustum->mFrustum.mVectors[2] ) * 0.5f;

					primitivenonetexbuffer->mPosition	= center;
					primitivenonetexbuffer->mColor		= color;
					primitivenonetexbuffer ++;
					primitivenonetexbuffer->mPosition	= center + frustum->mFrustum.mPlanes[0].Normal( ) * frustum->mLength;
					primitivenonetexbuffer->mColor		= color;
					primitivenonetexbuffer ++;
				}

				{
					Vector3 center = ( frustum->mFrustum.mVectors[5] + frustum->mFrustum.mVectors[7] ) * 0.5f;

					primitivenonetexbuffer->mPosition	= center;
					primitivenonetexbuffer->mColor		= color;
					primitivenonetexbuffer ++;
					primitivenonetexbuffer->mPosition	= center + frustum->mFrustum.mPlanes[1].Normal( ) * frustum->mLength;
					primitivenonetexbuffer->mColor		= color;
					primitivenonetexbuffer ++;
				}

				{
					Vector3 center = ( frustum->mFrustum.mVectors[0] + frustum->mFrustum.mVectors[1] ) * 0.5f;

					primitivenonetexbuffer->mPosition	= center;
					primitivenonetexbuffer->mColor		= color;
					primitivenonetexbuffer ++;
					primitivenonetexbuffer->mPosition	= center + frustum->mFrustum.mPlanes[2].Normal( ) * frustum->mLength;
					primitivenonetexbuffer->mColor		= color;
					primitivenonetexbuffer ++;
				}

				{
					Vector3 center = ( frustum->mFrustum.mVectors[2] + frustum->mFrustum.mVectors[3] ) * 0.5f;

					primitivenonetexbuffer->mPosition	= center;
					primitivenonetexbuffer->mColor		= color;
					primitivenonetexbuffer ++;
					primitivenonetexbuffer->mPosition	= center + frustum->mFrustum.mPlanes[3].Normal( ) * frustum->mLength;
					primitivenonetexbuffer->mColor		= color;
					primitivenonetexbuffer ++;
				}

				{
					Vector3 center = ( frustum->mFrustum.mVectors[1] + frustum->mFrustum.mVectors[2] ) * 0.5f;

					primitivenonetexbuffer->mPosition	= center;
					primitivenonetexbuffer->mColor		= color;
					primitivenonetexbuffer ++;
					primitivenonetexbuffer->mPosition	= center + frustum->mFrustum.mPlanes[4].Normal( ) * frustum->mLength;
					primitivenonetexbuffer->mColor		= color;
					primitivenonetexbuffer ++;
				}

				{
					Vector3 center = ( frustum->mFrustum.mVectors[3] + frustum->mFrustum.mVectors[0] ) * 0.5f;

					primitivenonetexbuffer->mPosition	= center;
					primitivenonetexbuffer->mColor		= color;
					primitivenonetexbuffer ++;
					primitivenonetexbuffer->mPosition	= center + frustum->mFrustum.mPlanes[5].Normal( ) * frustum->mLength;
					primitivenonetexbuffer->mColor		= color;
					primitivenonetexbuffer ++;
				}

				break;
			}

			case Renderable::_TYPE_BILLBOARD_SCREENALIGNED:
			{
				BillboardScreenAligned* screenaligned = (BillboardScreenAligned*) renderunit.mRenderable;

				if ( billboardbuffer == _null )
					continue;

				geometry.mVertexDeclaration	= mBillboardVD;
				geometry.mVertexBuffer		= mBillboardVB;
				geometry.mIndexBuffer		= _null;
				geometry.mTexture[0]		= screenaligned->mTexture;
				geometry.mGeometryType		= Geometry::_GEOMETRY_TRIANGLELIST;
				geometry.mGeometryNumber	= 2 + 2 * renderunit.mLinkNumber;
				geometry.mVertexSize		= sizeof( BillboardVertex );
				geometry.mVertexNumber		= 6 + 6 * renderunit.mLinkNumber;
				geometry.mStartVertex		= billboardvi;
				geometry.mStartIndex		= 0;

				billboardvi += geometry.mVertexNumber;

				// Fill Vertex buffer.
				const Matrix4& viewtransform = *renderunit.mViewTransform;

				Vector3 upaxis		= Vector3( viewtransform( 0, 1 ), viewtransform( 1, 1 ), viewtransform( 2, 1 ) );
				Vector3 rightaxis	= Vector3( viewtransform( 0, 0 ), viewtransform( 1, 0 ), viewtransform( 2, 0 ) );
				Vector3 lookaxis	= Vector3::Cross( upaxis, rightaxis ).Normalize( );

				do
				{

					_float x = Math::Abs( screenaligned->mSize.x ), y = Math::Abs( screenaligned->mSize.y );

					Vector3 vertexlt = upaxis * x - rightaxis * y;
					Vector3 vertexrt = upaxis * x + rightaxis * y;
					Vector3 vertexlb = upaxis * x + rightaxis * y;
					Vector3 vertexrb = upaxis * x - rightaxis * y;

					// Rotate billboard.
					if ( screenaligned->mRotation != 0.0f )
					{
						Matrix4 rotation;
						rotation.Rotation( lookaxis, screenaligned->mRotation );

						vertexlt *= rotation;
						vertexrt *= rotation;
						vertexlb *= rotation;
						vertexrb *= rotation;
					}

					vertexlt = screenaligned->mPosition + vertexlt;
					vertexrt = screenaligned->mPosition + vertexrt;
					vertexlb = screenaligned->mPosition - vertexlb;
					vertexrb = screenaligned->mPosition - vertexrb;

					billboardbuffer->mPosition	= vertexlt;
					billboardbuffer->mColor		= GraphicsManager::GetRenderAPIHelper( ).ConvertColor( screenaligned->mColorLT );
					billboardbuffer->mTexcoord	= screenaligned->mTexcoordLT;
					billboardbuffer ++;
					billboardbuffer->mPosition	= vertexrt;
					billboardbuffer->mColor		= GraphicsManager::GetRenderAPIHelper( ).ConvertColor( screenaligned->mColorRT );
					billboardbuffer->mTexcoord	= screenaligned->mTexcoordRT;
					billboardbuffer ++;
					billboardbuffer->mPosition	= vertexlb;
					billboardbuffer->mColor		= GraphicsManager::GetRenderAPIHelper( ).ConvertColor( screenaligned->mColorLB );
					billboardbuffer->mTexcoord	= screenaligned->mTexcoordLB;
					billboardbuffer ++;

					billboardbuffer->mPosition	= vertexlb;
					billboardbuffer->mColor		= GraphicsManager::GetRenderAPIHelper( ).ConvertColor( screenaligned->mColorLB );
					billboardbuffer->mTexcoord	= screenaligned->mTexcoordLB;
					billboardbuffer ++;
					billboardbuffer->mPosition	= vertexrt;
					billboardbuffer->mColor		= GraphicsManager::GetRenderAPIHelper( ).ConvertColor( screenaligned->mColorRT );
					billboardbuffer->mTexcoord	= screenaligned->mTexcoordRT;
					billboardbuffer ++;
					billboardbuffer->mPosition	= vertexrb;
					billboardbuffer->mColor		= GraphicsManager::GetRenderAPIHelper( ).ConvertColor( screenaligned->mColorRB );
					billboardbuffer->mTexcoord	= screenaligned->mTexcoordRB;
					billboardbuffer ++;
					
					if ( screenaligned == renderunit.mRenderable )
						screenaligned = (BillboardScreenAligned*) renderunit.mLinkHead;
					else
						screenaligned = (BillboardScreenAligned*) screenaligned->mType;
				}
				while ( screenaligned != _null );

				break;
			}

			case Renderable::_TYPE_BILLBOARD_WORLDORIENTED:
			{
				BillboardWorldOriented* worldoriented = (BillboardWorldOriented*) renderunit.mRenderable;

				if ( billboardbuffer == _null )
					continue;

				geometry.mVertexDeclaration	= mBillboardVD;
				geometry.mVertexBuffer		= mBillboardVB;
				geometry.mIndexBuffer		= _null;
				geometry.mTexture[0]		= worldoriented->mTexture;
				geometry.mGeometryType		= Geometry::_GEOMETRY_TRIANGLELIST;
				geometry.mGeometryNumber	= 2 + 2 * renderunit.mLinkNumber;
				geometry.mVertexSize		= sizeof( BillboardVertex );
				geometry.mVertexNumber		= 6 + 6 * renderunit.mLinkNumber;
				geometry.mStartVertex		= billboardvi;
				geometry.mStartIndex		= 0;

				billboardvi += geometry.mVertexNumber;

				// Fill Vertex buffer.
				const Matrix4& viewtransform = *renderunit.mViewTransform;

				Matrix4 viewinverse = viewtransform;
				viewinverse.Inverse( );

				do
				{
					Vector3 upaxis		= Vector3( viewtransform( 0, 1 ), viewtransform( 1, 1 ), viewtransform( 2, 1 ) );
					Vector3 lookaxis	= Vector3( viewinverse( 3, 0 ), viewinverse( 3, 1 ), viewinverse( 3, 2 ) ) - worldoriented->mPosition;
					Vector3 rightaxis	= Vector3::Cross( lookaxis, upaxis ).Normalize( );

					_float x = Math::Abs( worldoriented->mSize.x ), y = Math::Abs( worldoriented->mSize.y );

					Vector3 vertexlt = upaxis * x - rightaxis * y;
					Vector3 vertexrt = upaxis * x + rightaxis * y;
					Vector3 vertexlb = upaxis * x + rightaxis * y;
					Vector3 vertexrb = upaxis * x - rightaxis * y;

					// Rotate billboard.
					if ( worldoriented->mRotation != 0.0f )
					{
						Matrix4 rotation;
						rotation.Rotation( lookaxis, worldoriented->mRotation );

						vertexlt *= rotation;
						vertexrt *= rotation;
						vertexlb *= rotation;
						vertexrb *= rotation;
					}

					vertexlt = worldoriented->mPosition + vertexlt;
					vertexrt = worldoriented->mPosition + vertexrt;
					vertexlb = worldoriented->mPosition - vertexlb;
					vertexrb = worldoriented->mPosition - vertexrb;

					billboardbuffer->mPosition	= vertexlt;
					billboardbuffer->mColor		= GraphicsManager::GetRenderAPIHelper( ).ConvertColor( worldoriented->mColorLT );
					billboardbuffer->mTexcoord	= worldoriented->mTexcoordLT;
					billboardbuffer ++;
					billboardbuffer->mPosition	= vertexrt;
					billboardbuffer->mColor		= GraphicsManager::GetRenderAPIHelper( ).ConvertColor( worldoriented->mColorRT );
					billboardbuffer->mTexcoord	= worldoriented->mTexcoordRT;
					billboardbuffer ++;
					billboardbuffer->mPosition	= vertexlb;
					billboardbuffer->mColor		= GraphicsManager::GetRenderAPIHelper( ).ConvertColor( worldoriented->mColorLB );
					billboardbuffer->mTexcoord	= worldoriented->mTexcoordLB;
					billboardbuffer ++;

					billboardbuffer->mPosition	= vertexlb;
					billboardbuffer->mColor		= GraphicsManager::GetRenderAPIHelper( ).ConvertColor( worldoriented->mColorLB );
					billboardbuffer->mTexcoord	= worldoriented->mTexcoordLB;
					billboardbuffer ++;
					billboardbuffer->mPosition	= vertexrt;
					billboardbuffer->mColor		= GraphicsManager::GetRenderAPIHelper( ).ConvertColor( worldoriented->mColorRT );
					billboardbuffer->mTexcoord	= worldoriented->mTexcoordRT;
					billboardbuffer ++;
					billboardbuffer->mPosition	= vertexrb;
					billboardbuffer->mColor		= GraphicsManager::GetRenderAPIHelper( ).ConvertColor( worldoriented->mColorRB );
					billboardbuffer->mTexcoord	= worldoriented->mTexcoordRB;
					billboardbuffer ++;
					
					if ( worldoriented == renderunit.mRenderable )
						worldoriented = (BillboardWorldOriented*) renderunit.mLinkHead;
					else
						worldoriented = (BillboardWorldOriented*) worldoriented->mType;
				}
				while ( worldoriented != _null );

				break;
			}

			case Renderable::_TYPE_BILLBOARD_AXIAL:
			{
				BillboardAxial* axial = (BillboardAxial*) renderunit.mRenderable;

				if ( billboardbuffer == _null )
					continue;

				geometry.mVertexDeclaration	= mBillboardVD;
				geometry.mVertexBuffer		= mBillboardVB;
				geometry.mIndexBuffer		= _null;
				geometry.mTexture[0]		= axial->mTexture;
				geometry.mGeometryType		= Geometry::_GEOMETRY_TRIANGLELIST;
				geometry.mGeometryNumber	= 2 + 2 * renderunit.mLinkNumber;
				geometry.mVertexSize		= sizeof( BillboardVertex );
				geometry.mVertexNumber		= 6 + 6 * renderunit.mLinkNumber;
				geometry.mStartVertex		= billboardvi;
				geometry.mStartIndex		= 0;

				billboardvi += geometry.mVertexNumber;

				// Fill Vertex buffer.
				const Matrix4& viewtransform = *renderunit.mViewTransform;

				Matrix4 viewinverse = viewtransform;
				viewinverse.Inverse( );

				do
				{
					Vector3 upaxis		= Vector3( axial->mAxis ).Normalize( );
					Vector3 lookaxis	= Vector3( viewinverse( 3, 0 ), viewinverse( 3, 1 ), viewinverse( 3, 2 ) ) - axial->mPosition;
					Vector3 rightaxis	= Vector3::Cross( lookaxis, upaxis ).Normalize( );

					_float x = Math::Abs( axial->mSize.x ), y = Math::Abs( axial->mSize.y );

					Vector3 vertexlt = axial->mPosition + upaxis * x - rightaxis * y;
					Vector3 vertexrt = axial->mPosition + upaxis * x + rightaxis * y;
					Vector3 vertexlb = axial->mPosition - upaxis * x - rightaxis * y;
					Vector3 vertexrb = axial->mPosition - upaxis * x + rightaxis * y;

					billboardbuffer->mPosition	= vertexlt;
					billboardbuffer->mColor		= GraphicsManager::GetRenderAPIHelper( ).ConvertColor( axial->mColorLT );
					billboardbuffer->mTexcoord	= axial->mTexcoordLT;
					billboardbuffer ++;
					billboardbuffer->mPosition	= vertexrt;
					billboardbuffer->mColor		= GraphicsManager::GetRenderAPIHelper( ).ConvertColor( axial->mColorRT );
					billboardbuffer->mTexcoord	= axial->mTexcoordRT;
					billboardbuffer ++;
					billboardbuffer->mPosition	= vertexlb;
					billboardbuffer->mColor		= GraphicsManager::GetRenderAPIHelper( ).ConvertColor( axial->mColorLB );
					billboardbuffer->mTexcoord	= axial->mTexcoordLB;
					billboardbuffer ++;

					billboardbuffer->mPosition	= vertexlb;
					billboardbuffer->mColor		= GraphicsManager::GetRenderAPIHelper( ).ConvertColor( axial->mColorLB );
					billboardbuffer->mTexcoord	= axial->mTexcoordLB;
					billboardbuffer ++;
					billboardbuffer->mPosition	= vertexrt;
					billboardbuffer->mColor		= GraphicsManager::GetRenderAPIHelper( ).ConvertColor( axial->mColorRT );
					billboardbuffer->mTexcoord	= axial->mTexcoordRT;
					billboardbuffer ++;
					billboardbuffer->mPosition	= vertexrb;
					billboardbuffer->mColor		= GraphicsManager::GetRenderAPIHelper( ).ConvertColor( axial->mColorRB );
					billboardbuffer->mTexcoord	= axial->mTexcoordRB;
					billboardbuffer ++;

					if ( axial == renderunit.mRenderable )
						axial = (BillboardAxial*) renderunit.mLinkHead;
					else
						axial = (BillboardAxial*) axial->mType;
				}
				while ( axial != _null );

				break;
			}
		}
	}

	if ( mRenderToken.mOverlayNoneTexVN != 0 && mOverlayNoneTexVB != _null )
		mOverlayNoneTexVB->Unlock( );

	if ( mRenderToken.mOverlayWithTexVN != 0 && mOverlayWithTexVB != _null )
		mOverlayWithTexVB->Unlock( );

	if ( mRenderToken.mPrimitiveNoneTexVN != 0 && mPrimitiveNoneTexVB != _null )
		mPrimitiveNoneTexVB->Unlock( );

	if ( mRenderToken.mPrimitiveWithTexVN != 0 && mPrimitiveWithTexVB != _null )
		mPrimitiveWithTexVB->Unlock( );

	if ( mRenderToken.mBillboardVN != 0 && mBillboardVB != _null )
		mBillboardVB->Unlock( );
}

_void RenderSet::Finish( )
{
	mRenderTarget = _null;
	mDepthStencil = _null;

	for ( _dword i = 0; i < mRenderUnitArray.Number( ); i ++ )
	{
		RenderUnit& renderunit = mRenderUnitArray[i];

		if ( renderunit.mRenderable != _null )
			ReleaseRenderUnit( renderunit );
	}

	mRenderUnitAllocater.Clear( );
	mRenderUnitArray.Clear( );
	mRenderToken.Clear( );
}

_void RenderSet::BeginRecord( )
{
	GraphicsManager::GetRenderQueueHelper( ).SetCurrentRenderSet( this );
}

_void RenderSet::EndRecord( )
{
	Prepare( );

	GraphicsManager::GetRenderQueueHelper( ).UseDefaultRenderSet( );
}

_void RenderSet::ClearRecord( )
{
	Finish( );
}
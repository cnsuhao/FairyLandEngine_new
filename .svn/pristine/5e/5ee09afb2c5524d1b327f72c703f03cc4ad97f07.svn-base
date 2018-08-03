//============================================================================
// Renderable.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

class IVertexDeclaration;
class IVertexBuffer;
class IVertexBuffer;
class IVertexBuffer;
class IIndexBuffer;
class ITexture;
class IShaderConst;

//----------------------------------------------------------------------------
// Renderable
//----------------------------------------------------------------------------

struct Renderable
{
	enum _RENDERABLE_TYPE
	{
		_TYPE_GEOMETRY						=  1,

		_TYPE_OVERLAY_BEGIN					=  2,
			_TYPE_OVERLAY_POINT				=  2,
			_TYPE_OVERLAY_LINE				=  3,
			_TYPE_OVERLAY_TRIANGLE			=  4,
			_TYPE_OVERLAY_RECTANGLE			=  5,
			_TYPE_OVERLAY_QUADRANGLE		=  6,
			_TYPE_OVERLAY_RECTANGLEMASK		=  7,
			_TYPE_OVERLAY_WITHTEX_BEGIN		=  8,
			_TYPE_OVERLAY_TRIANGLEIMAGE		=  8,
			_TYPE_OVERLAY_RECTANGLEIMAGE	=  9,
			_TYPE_OVERLAY_QUADRANGLEIMAGE	= 10,
			_TYPE_OVERLAY_WITHTEX_END		= 10,
		_TYPE_OVERLAY_END					= 10,

		_TYPE_PRIMITIVE_BEGIN				= 11,
			_TYPE_PRIMITIVE_POINT			= 11,
			_TYPE_PRIMITIVE_LINE			= 12,
			_TYPE_PRIMITIVE_TRIANGLE		= 13,
			_TYPE_PRIMITIVE_QUADRANGLE		= 14,
			_TYPE_PRIMITIVE_BOX				= 15,
			_TYPE_PRIMITIVE_CIRCLE			= 16,
			_TYPE_PRIMITIVE_SPHERE			= 17,
			_TYPE_PRIMITIVE_PYRAMID			= 18,
			_TYPE_PRIMITIVE_AXIS			= 19,
			_TYPE_PRIMITIVE_FRUSTUM			= 20,
			_TYPE_PRIMITIVE_TRIANGLEIMAGE	= 21,
			_TYPE_PRIMITIVE_QUADRANGLEIMAGE	= 22,
		_TYPE_PRIMITIVE_END					= 22,

		_TYPE_BILLBOARD_BEGIN				= 23,
			_TYPE_BILLBOARD_SCREENALIGNED	= 23,
			_TYPE_BILLBOARD_WORLDORIENTED	= 24,
			_TYPE_BILLBOARD_AXIAL			= 25,
		_TYPE_BILLBOARD_END					= 25,
	};

	_dword	mType;

	_dword  mModelGroup;
	_dword  mModelLayer;

	Renderable( ) : mType( 0 ), mModelGroup( 0 ), mModelLayer( 0 ) { }
};

//----------------------------------------------------------------------------
// Geometry
//----------------------------------------------------------------------------

struct Geometry : public Renderable
{
	enum { _MAX_TEXTURE_NUMBER = 6 };

	enum _GEOMETRY_TYPE
	{
		_GEOMETRY_POINTLIST		= 1,
		_GEOMETRY_LINELIST		= 2,
		_GEOMETRY_LINESTRIP		= 3,
		_GEOMETRY_TRIANGLELIST	= 4,
		_GEOMETRY_TRIANGLESTRIP	= 5,
		_GEOMETRY_TRIANGLEFAN	= 6,
	};

	enum _FORMATFLAG
	{
		_FORMATFLAG_NORMAL		= 0x00000001,
		_FORMATFLAG_DIFFUSE		= 0x00000002,
		_FORMATFLAG_SPECULAR	= 0x00000004,
		_FORMATFLAG_POSITION	= 0x00000008,
		_FORMATFLAG_TEXCOORD1	= 0x00000010,
		_FORMATFLAG_TEXCOORD2	= 0x00000020,
		_FORMATFLAG_TEXCOORD3	= 0x00000040,
		_FORMATFLAG_TEXCOORD4	= 0x00000080,
		_FORMATFLAG_BINORMAL	= 0x00000100,
        _FORMATFLAG_TANGENT		= 0x00000200,
		_FORMATFLAG_INFLUENCE	= 0x00000400,
		_FORMATFLAG_POSITIONT	= 0x00000800,
	};

	enum _RENDERFLAG
	{
		_RENDERFLAG_NORMAL		= 0x00000000,
		_RENDERFLAG_INVISIBLE	= 0x00000001,
		_RENDERFLAG_DECAL		= 0x00000002,
		_RENDERFLAG_ALPHA		= 0x00000004,
		_RENDERFLAG_COLORKEY	= 0x00000008,
		_RENDERFLAG_BILLBOARD	= 0x00000010,
		_RENDERFLAG_BILLBOARDX	= 0x00000020,
		_RENDERFLAG_BILLBOARDY	= 0x00000040,
		_RENDERFLAG_BILLBOARDZ	= 0x00000080,
		_RENDERFLAG_ADDITIVE	= 0x00000100,
		_RENDERFLAG_SUBTRACTIVE	= 0x00000200,
		_RENDERFLAG_ONLYZBUFFER	= 0x00000400,
	};

	IVertexDeclaration*	mVertexDeclaration;
	IVertexBuffer*		mVertexBuffer;
	IVertexBuffer*		mBackupBuffer;
	IVertexBuffer*		mHelperBuffer;
	IIndexBuffer*		mIndexBuffer;
	ITexture*			mTexture[ _MAX_TEXTURE_NUMBER ];

	_dword				mGeometryType;
	_dword				mGeometryNumber;

	_dword				mVertexSize;
	_dword				mHelperSize;
	_dword				mVertexNumber;
	_dword				mStartVertex;
	_dword				mStartIndex;

	_dword				mFormatFlag;
	_dword				mRenderFlag;

	_dword				mTextureFactor;

	_dword				mMaterialDiffuse;
	_dword				mMaterialAmbient;
	_dword				mMaterialSpecular;
	_dword				mMaterialEmissive;
	_float				mMaterialPower;
	_float				mDepthBias;

	_byte				mMirror;
	_byte				mProjectTexture;
	_byte				mProcessFlag;
	_byte				mReserved;

	_void Reset( )
	{
		mType				= _TYPE_GEOMETRY;
		mVertexDeclaration	= _null;
		mVertexBuffer		= _null;
		mBackupBuffer		= _null;
		mHelperBuffer		= _null;
		mIndexBuffer		= _null;

		for ( _dword i = 0; i < _MAX_TEXTURE_NUMBER; i ++ )
			mTexture[i] = _null;

		mGeometryType		= 0;
		mGeometryNumber		= 0;

		mVertexSize			= 0;
		mHelperSize			= 0;
		mVertexNumber		= 0;
		mStartVertex		= 0;
		mStartIndex			= 0;

		mFormatFlag			= 0;
		mRenderFlag			= 0;

		mTextureFactor		= 0xFFFFFFFF;

		mMaterialDiffuse	= 0xFFFFFFFF;
		mMaterialAmbient	= 0xFF202020;
		mMaterialSpecular	= 0;
		mMaterialEmissive	= 0;
		mMaterialPower		= 0.0f;
		mDepthBias			= 0.0f;

		mMirror				= _false;
		mProjectTexture		= _false;
		mProcessFlag		= _false;
		mReserved			= _false;
	}

	Geometry( )
	{
		Reset( );
	}

	_dword GetTextureNumber( ) const
	{
		_dword number = 0;

		for ( _dword i = 0; i < _MAX_TEXTURE_NUMBER; i ++ )
		{
			if ( mTexture[i] == _null )
				break;

			number ++;
		}

		return number;
	}
};

//----------------------------------------------------------------------------
// OverlayPoint
//----------------------------------------------------------------------------

struct OverlayPoint : public Renderable
{
	Vector2		mPosition;
	_dword		mColor;

	OverlayPoint( )
	{
		mType		= _TYPE_OVERLAY_POINT;
	}

	OverlayPoint( const Vector2& position, _dword color )
	{
		mType		= _TYPE_OVERLAY_POINT;
		mPosition	= position;
	}
};

//----------------------------------------------------------------------------
// OverlayLine
//----------------------------------------------------------------------------

struct OverlayLine : public Renderable
{
	Vector2		mPosition1;
	Vector2		mPosition2;
	_dword		mColor1;
	_dword		mColor2;

	OverlayLine( )
	{
		mType		= _TYPE_OVERLAY_LINE;
	}

	OverlayLine( const Vector2& position1, const Vector2& position2, _dword color )
	{
		mType		= _TYPE_OVERLAY_LINE;
		mPosition1	= position1;
		mPosition2	= position2;
		mColor1		= color;
		mColor2		= color;
	}
};

//----------------------------------------------------------------------------
// OverlayTriangle
//----------------------------------------------------------------------------

struct OverlayTriangle : public Renderable
{
	Vector2		mPosition1;
	Vector2		mPosition2;
	Vector2		mPosition3;
	_dword		mColor1;
	_dword		mColor2;
	_dword		mColor3;

	OverlayTriangle( )
	{
		mType		= _TYPE_OVERLAY_TRIANGLE;
	}

	OverlayTriangle( const Vector2& position1, const Vector2& position2, const Vector2& position3, _dword color )
	{
		mType		= _TYPE_OVERLAY_TRIANGLE;
		mPosition1	= position1;
		mPosition2	= position2;
		mPosition3	= position3;
		mColor1		= color;
		mColor2		= color;
		mColor3		= color;
	}
};

//----------------------------------------------------------------------------
// OverlayRectangle
//----------------------------------------------------------------------------

struct OverlayRectangle : public Renderable
{
	Vector2		mPositionLT;
	Vector2		mPositionRB;
	_dword		mColor;

	OverlayRectangle( )
	{
		mType			= _TYPE_OVERLAY_RECTANGLE;
	}

	OverlayRectangle( const Vector2& positionlt, const Vector2& positionrb, _dword color )
	{
		mType			= _TYPE_OVERLAY_RECTANGLE;
		mPositionLT		= positionlt;
		mPositionRB		= positionrb;
		mColor			= color;
	}
};

//----------------------------------------------------------------------------
// OverlayQuadrangle
//----------------------------------------------------------------------------

struct OverlayQuadrangle : public Renderable
{
	Vector2		mPositionLT;
	Vector2		mPositionRT;
	Vector2		mPositionLB;
	Vector2		mPositionRB;
	_dword		mColorLT;
	_dword		mColorRT;
	_dword		mColorLB;
	_dword		mColorRB;

	OverlayQuadrangle( )
	{
		mType		= _TYPE_OVERLAY_QUADRANGLE;
	}

	OverlayQuadrangle( const Vector2& positionlt, const Vector2& positionrt,
		const Vector2& positionlb, const Vector2& positionrb, _dword color )
	{
		mType			= _TYPE_OVERLAY_QUADRANGLE;
		mPositionLT		= positionlt;
		mPositionRT		= positionrt;
		mPositionLB		= positionlb;
		mPositionRB		= positionrb;
		mColorLT		= color;
		mColorRT		= color;
		mColorLB		= color;
		mColorRB		= color;
	}
};

//----------------------------------------------------------------------------
// OverlayRectangleMask
//----------------------------------------------------------------------------

struct OverlayRectangleMask : public Renderable
{
	Vector2		mPositionLT;
	Vector2		mPositionRB;
	_float		mPercentage;
	_dword		mColor;

	OverlayRectangleMask( )
	{
		mType		= _TYPE_OVERLAY_RECTANGLEMASK;
	}

	OverlayRectangleMask( const Vector2& positionlt, const Vector2& positionrb, _float percentage, _dword color )
	{
		mType		= _TYPE_OVERLAY_RECTANGLEMASK;
		mPositionLT	= positionlt;
		mPositionRB	= positionrb;
		mPercentage	= percentage;
		mColor		= color;
	}
};

//----------------------------------------------------------------------------
// OverlayImageBase
//----------------------------------------------------------------------------

struct OverlayImageBase : public Renderable
{
	ITexture*	mTexture;
};

//----------------------------------------------------------------------------
// OverlayTriangleImage
//----------------------------------------------------------------------------

struct OverlayTriangleImage : public OverlayImageBase
{
	Vector2		mPosition1;
	Vector2		mPosition2;
	Vector2		mPosition3;
	Vector2		mTexcoord1;
	Vector2		mTexcoord2;
	Vector2		mTexcoord3;
	_dword		mColor1;
	_dword		mColor2;
	_dword		mColor3;

	OverlayTriangleImage( )
	{
		mType		= _TYPE_OVERLAY_TRIANGLEIMAGE;
	}

	OverlayTriangleImage( const Vector2& position1, const Vector2& position2, const Vector2& position3, const Vector2& texcoord1,
		const Vector2& texcoord2, const Vector2& texcoord3, _dword color, ITexture* texture )
	{
		mType		= _TYPE_OVERLAY_TRIANGLEIMAGE;
		mPosition1	= position1;
		mPosition2	= position2;
		mPosition3	= position3;
		mTexcoord1	= texcoord1;
		mTexcoord2	= texcoord2;
		mTexcoord3	= texcoord3;
		mColor1		= color;
		mColor2		= color;
		mColor3		= color;
		mTexture	= texture;
	}
};

//----------------------------------------------------------------------------
// OverlayRectangleImage
//----------------------------------------------------------------------------

struct OverlayRectangleImage : public OverlayImageBase
{
	Vector2		mPositionLT;
	Vector2		mPositionRB;
	Vector2		mTexcoordLT;
	Vector2		mTexcoordRB;
	_dword		mColor;

	OverlayRectangleImage( )
	{
		mType			= _TYPE_OVERLAY_RECTANGLEIMAGE;
	}

	OverlayRectangleImage( const Vector2& positionlt, const Vector2& positionrb, _dword color, ITexture* texture )
	{
		mType			= _TYPE_OVERLAY_RECTANGLEIMAGE;
		mPositionLT		= positionlt;
		mPositionRB		= positionrb;
		mTexcoordLT		= Vector2( 0.0f, 0.0f );
		mTexcoordRB		= Vector2( 1.0f, 1.0f );
		mColor			= color;
		mTexture		= texture;
	}

	OverlayRectangleImage( const Vector2& positionlt, const Vector2& positionrb, const Vector2& texcoordlt,
		const Vector2& texcoordrb, _dword color, ITexture* texture )
	{
		mType			= _TYPE_OVERLAY_RECTANGLEIMAGE;
		mPositionLT		= positionlt;
		mPositionRB		= positionrb;
		mTexcoordLT		= texcoordlt;
		mTexcoordRB		= texcoordrb;
		mColor			= color;
		mTexture		= texture;
	}
};

//----------------------------------------------------------------------------
// OverlayQuadrangleImage
//----------------------------------------------------------------------------

struct OverlayQuadrangleImage : public OverlayImageBase
{
	Vector2		mPositionLT;
	Vector2		mPositionRT;
	Vector2		mPositionLB;
	Vector2		mPositionRB;
	Vector2		mTexcoordLT;
	Vector2		mTexcoordRT;
	Vector2		mTexcoordLB;
	Vector2		mTexcoordRB;
	_dword		mColorLT;
	_dword		mColorRT;
	_dword		mColorLB;
	_dword		mColorRB;

	OverlayQuadrangleImage( )
	{
		mType		= _TYPE_OVERLAY_QUADRANGLEIMAGE;
	}

	OverlayQuadrangleImage( const Vector2& positionlt, const Vector2& positionrt,
		const Vector2& positionlb, const Vector2& positionrb, _dword color, ITexture* texture )
	{
		mType			= _TYPE_OVERLAY_QUADRANGLEIMAGE;
		mPositionLT		= positionlt;
		mPositionRT		= positionrt;
		mPositionLB		= positionlb;
		mPositionRB		= positionrb;
		mTexcoordLT		= Vector2( 0.0f, 0.0f );
		mTexcoordRT		= Vector2( 1.0f, 0.0f );
		mTexcoordLB		= Vector2( 0.0f, 1.0f );
		mTexcoordRB		= Vector2( 1.0f, 1.0f );
		mColorLT		= color;
		mColorRT		= color;
		mColorLB		= color;
		mTexture		= texture;
	}

	OverlayQuadrangleImage( const Vector2& positionlt, const Vector2& positionrt, const Vector2& positionlb, const Vector2& positionrb,
		const Vector2& texcoordlt, const Vector2& texcoordrb, _dword color, ITexture* texture )
	{
		mType			= _TYPE_OVERLAY_QUADRANGLEIMAGE;
		mPositionLT		= positionlt;
		mPositionRT		= positionrt;
		mPositionLB		= positionlb;
		mPositionRB		= positionrb;
		mTexcoordLT		= texcoordlt;
		mTexcoordRT		= Vector2( texcoordrb.x, texcoordlt.y );
		mTexcoordLB		= Vector2( texcoordlt.x, texcoordrb.y );
		mTexcoordRB		= texcoordrb;
		mColorLT		= color;
		mColorRT		= color;
		mColorLB		= color;
		mColorRB		= color;
		mTexture		= texture;
	}
};

//----------------------------------------------------------------------------
// PrimitivePoint
//----------------------------------------------------------------------------

struct PrimitivePoint : public Renderable
{
	Vector3		mPosition;
	_dword		mColor;

	PrimitivePoint( )
	{
		mType		= _TYPE_PRIMITIVE_POINT;
	}

	PrimitivePoint( const Vector3& position, _dword color )
	{
		mType		= _TYPE_PRIMITIVE_POINT;
		mPosition	= position;
		mColor		= color;
	}
};

//----------------------------------------------------------------------------
// PrimitiveLine
//----------------------------------------------------------------------------

struct PrimitiveLine : public Renderable
{
	Vector3		mPosition1;
	Vector3		mPosition2;
	_dword		mColor1;
	_dword		mColor2;

	PrimitiveLine( )
	{
		mType		= _TYPE_PRIMITIVE_LINE;
	}

	PrimitiveLine( const Vector3& position1, const Vector3& position2, _dword color )
	{
		mType		= _TYPE_PRIMITIVE_LINE;
		mPosition1	= position1;
		mPosition2	= position2;
		mColor1		= color;
		mColor2		= color;
	}

	PrimitiveLine( const Vector3& position1, const Vector3& position2, _dword color1, _dword color2 )
	{
		mType		= _TYPE_PRIMITIVE_LINE;
		mPosition1	= position1;
		mPosition2	= position2;
		mColor1		= color1;
		mColor2		= color2;
	}
};

//----------------------------------------------------------------------------
// PrimitiveTriangle
//----------------------------------------------------------------------------

struct PrimitiveTriangle : public Renderable
{
	Vector3		mPosition1;
	Vector3		mPosition2;
	Vector3		mPosition3;
	_dword		mColor1;
	_dword		mColor2;
	_dword		mColor3;

	PrimitiveTriangle( )
	{
		mType		= _TYPE_PRIMITIVE_TRIANGLE;
	}

	PrimitiveTriangle( const Vector3& position1, const Vector3& position2, const Vector3& position3, _dword color )
	{
		mType		= _TYPE_PRIMITIVE_TRIANGLE;
		mPosition1	= position1;
		mPosition2	= position2;
		mPosition3	= position3;
		mColor1		= color;
		mColor2		= color;
		mColor3		= color;
	}

	PrimitiveTriangle( const Vector3& position1, const Vector3& position2, const Vector3& position3,
		_dword color1, _dword color2, _dword color3 )
	{
		mType		= _TYPE_PRIMITIVE_TRIANGLE;
		mPosition1	= position1;
		mPosition2	= position2;
		mPosition3	= position3;
		mColor1		= color1;
		mColor2		= color2;
		mColor3		= color3;
	}
};

//----------------------------------------------------------------------------
// PrimitiveQuadrangle
//----------------------------------------------------------------------------

struct PrimitiveQuadrangle : public Renderable
{
	Vector3		mCenter;
	Vector3		mWidth;
	Vector3		mHeight;
	_dword		mColor;

	PrimitiveQuadrangle( )
	{
		mType		= _TYPE_PRIMITIVE_QUADRANGLE;
	}

	PrimitiveQuadrangle( const Vector3& center, const Vector3& width, const Vector3& height, _dword color )
	{
		mType		= _TYPE_PRIMITIVE_QUADRANGLE;
		mCenter		= center;
		mWidth		= width;
		mHeight		= height;
		mColor		= color;
	}
};

//----------------------------------------------------------------------------
// PrimitiveBox
//----------------------------------------------------------------------------

struct PrimitiveBox : public Renderable
{
	Vector3		mCenter;
	Vector3		mWidth;
	Vector3		mHeight;
	Vector3		mDepth;
	_dword		mColor;

	PrimitiveBox( )
	{
		mType		= _TYPE_PRIMITIVE_BOX;
	}

	PrimitiveBox( const AxisAlignedBox& boundbox, _dword color )
	{
		Vector3 boxsize = boundbox.BoxSize( ) * 0.5f;

		mType		= _TYPE_PRIMITIVE_BOX;
		mCenter		= boundbox.CenterVector( );
		mWidth		= Vector3( boxsize.x, 0.0f, 0.0f );
		mHeight		= Vector3( 0.0f, boxsize.y, 0.0f );
		mDepth		= Vector3( 0.0f, 0.0f, boxsize.z );
		mColor		= color;
	}

	PrimitiveBox( const Vector3& center, const Vector3& width, const Vector3& height, const Vector3& depth, _dword color )
	{
		mType		= _TYPE_PRIMITIVE_BOX;
		mCenter		= center;
		mWidth		= width;
		mHeight		= height;
		mDepth		= depth;
		mColor		= color;
	}
};

//----------------------------------------------------------------------------
// PrimitiveCircle
//----------------------------------------------------------------------------

struct PrimitiveCircle : public Renderable
{
	Vector3		mCenter;
	Vector3		mNormal;
	_float		mRadius;
	_dword		mColor;
	_dword		mSegment;

	PrimitiveCircle( )
	{
		mType		= _TYPE_PRIMITIVE_CIRCLE;
	}

	PrimitiveCircle( const Vector3& center, const Vector3& normal, _float radius, _dword segment, _dword color )
	{
		mType		= _TYPE_PRIMITIVE_CIRCLE;
		mCenter		= center;
		mNormal		= normal;
		mRadius		= radius;
		mSegment	= segment;
		mColor		= color;
	}
};

//----------------------------------------------------------------------------
// PrimitiveSphere
//----------------------------------------------------------------------------

struct PrimitiveSphere : public Renderable
{
	Vector3		mCenter;
	_float		mRadius;
	_dword		mSegment;
	_dword		mColor;

	PrimitiveSphere( )
	{
		mType		= _TYPE_PRIMITIVE_SPHERE;
	}

	PrimitiveSphere( const Vector3& center, _float radius, _dword segment, _dword color )
	{
		mType		= _TYPE_PRIMITIVE_SPHERE;
		mCenter		= center;
		mRadius		= radius;
		mSegment	= segment;
		mColor		= color;
	}
};

//----------------------------------------------------------------------------
// PrimitivePyramid
//----------------------------------------------------------------------------

struct PrimitivePyramid : public Renderable
{
	Vector3		mBase;
	Vector3		mPeak;
	_float		mRadius;
	_dword		mColor;

	PrimitivePyramid( )
	{
		mType		= _TYPE_PRIMITIVE_PYRAMID;
	}

	PrimitivePyramid( const Vector3& base, const Vector3& peak, _dword color )
	{
		mType		= _TYPE_PRIMITIVE_PYRAMID;
		mBase		= base;
		mPeak		= peak;
		mRadius		= ( mPeak - mBase ).Magnitude( ) / 8;
		mColor		= color;
	}

	PrimitivePyramid( const Vector3& base, const Vector3& peak, _float radius, _dword color )
	{
		mType		= _TYPE_PRIMITIVE_PYRAMID;
		mBase		= base;
		mPeak		= peak;
		mRadius		= radius;
		mColor		= color;
	}
};

//----------------------------------------------------------------------------
// PrimitiveAxis
//----------------------------------------------------------------------------

struct PrimitiveAxis : public Renderable
{
	Vector3		mOrigin;
	Vector3		mXAxis;
	Vector3		mYAxis;
	_float		mLength;

	PrimitiveAxis( )
	{
		mType		= _TYPE_PRIMITIVE_AXIS;
	}

	PrimitiveAxis( const Vector3& origin, _float length )
	{
		mType		= _TYPE_PRIMITIVE_AXIS;
		mOrigin		= origin;
		mXAxis		= Vector3::cXAxis;
		mYAxis		= Vector3::cYAxis;
		mLength		= length;
	}

	PrimitiveAxis( const Vector3& origin, const Vector3& axisx, const Vector3& axisy, _float length )
	{
		mType		= _TYPE_PRIMITIVE_AXIS;
		mOrigin		= origin;
		mXAxis		= axisx;
		mYAxis		= axisy;
		mLength		= length;
	}

	PrimitiveAxis( const Vector3& translation, const Quaternion& rotation, _float length )
	{
		mType		= _TYPE_PRIMITIVE_AXIS;
		mOrigin		= Vector3::cOrigin + translation;
		mXAxis		= Vector3::cXAxis * rotation;
		mYAxis		= Vector3::cYAxis * rotation;
		mLength		= length;

		mXAxis.Normalize( );
		mYAxis.Normalize( );
	}
};

//----------------------------------------------------------------------------
// PrimitiveFrustum
//----------------------------------------------------------------------------

struct PrimitiveFrustum : public Renderable
{
	Frustum		mFrustum;
	_float		mLength;
	_dword		mColor;

	PrimitiveFrustum( )
	{
		mType		= _TYPE_PRIMITIVE_FRUSTUM;
	}

	PrimitiveFrustum( const Frustum& frustum, _float length, _dword color )
	{
		mType		= _TYPE_PRIMITIVE_FRUSTUM;
		mFrustum	= frustum;
		mLength		= length;
		mColor		= color;
	}
};

//----------------------------------------------------------------------------
// PrimitiveImageBase
//----------------------------------------------------------------------------

struct PrimitiveImageBase : public Renderable
{
	ITexture*	mTexture;
};

//----------------------------------------------------------------------------
// PrimitiveTriangleImage
//----------------------------------------------------------------------------

struct PrimitiveTriangleImage : public PrimitiveImageBase
{
	Vector3		mPosition1;
	Vector3		mPosition2;
	Vector3		mPosition3;
	Vector2		mTexcoord1;
	Vector2		mTexcoord2;
	Vector2		mTexcoord3;
	_dword		mColor1;
	_dword		mColor2;
	_dword		mColor3;

	PrimitiveTriangleImage( )
	{
		mType		= _TYPE_PRIMITIVE_TRIANGLEIMAGE;
	}

	PrimitiveTriangleImage( const Vector3& position1, const Vector3& position2, const Vector3& position3,
		const Vector2& texcoord1, const Vector2& texcoord2, const Vector2& texcoord3, _dword color, ITexture* texture )
	{
		mType		= _TYPE_PRIMITIVE_TRIANGLEIMAGE;
		mPosition1	= position1;
		mPosition2	= position2;
		mPosition3	= position3;
		mTexcoord1	= texcoord1;
		mTexcoord2	= texcoord2;
		mTexcoord3	= texcoord3;
		mColor1		= color;
		mColor2		= color;
		mColor3		= color;
		mTexture	= texture;
	}

	PrimitiveTriangleImage( const Vector3& position1, const Vector3& position2, const Vector3& position3,
		const Vector2& texcoord1, const Vector2& texcoord2, const Vector2& texcoord3, _dword color1, _dword color2, _dword color3, ITexture* texture )
	{
		mType		= _TYPE_PRIMITIVE_TRIANGLEIMAGE;
		mPosition1	= position1;
		mPosition2	= position2;
		mPosition3	= position3;
		mTexcoord1	= texcoord1;
		mTexcoord2	= texcoord2;
		mTexcoord3	= texcoord3;
		mColor1		= color1;
		mColor2		= color2;
		mColor3		= color3;
		mTexture	= texture;
	}
};

//----------------------------------------------------------------------------
// PrimitiveQuadrangleImage
//----------------------------------------------------------------------------

struct PrimitiveQuadrangleImage : public PrimitiveImageBase
{
	Vector3		mCenter;
	Vector3		mWidth;
	Vector3		mHeight;
	Vector2		mTexcoordLT;
	Vector2		mTexcoordRT;
	Vector2		mTexcoordLB;
	Vector2		mTexcoordRB;
	_dword		mColor;

	PrimitiveQuadrangleImage( )
	{
		mType		= _TYPE_PRIMITIVE_QUADRANGLEIMAGE;
	}

	PrimitiveQuadrangleImage( const Vector3& center, const Vector3& width, const Vector3& height, _dword color, ITexture* texture )
	{
		mType		= _TYPE_PRIMITIVE_QUADRANGLEIMAGE;
		mCenter		= center;
		mWidth		= width;
		mHeight		= height;
		mTexcoordLT	= Vector2( 0.0f, 0.0f );
		mTexcoordRT	= Vector2( 1.0f, 0.0f );
		mTexcoordLB	= Vector2( 0.0f, 1.0f );
		mTexcoordRB	= Vector2( 1.0f, 1.0f );
		mColor		= color;
		mTexture	= texture;
	}

	PrimitiveQuadrangleImage( const Vector3& center, const Vector3& width, const Vector3& height, const Vector2& texcoordlt,
		const Vector2& texcoordrt, const Vector2& texcoordlb, const Vector2& texcoordrb, _dword color, ITexture* texture )
	{
		mType		= _TYPE_PRIMITIVE_QUADRANGLEIMAGE;
		mCenter		= center;
		mWidth		= width;
		mHeight		= height;
		mTexcoordLT	= texcoordlt;
		mTexcoordRT	= texcoordrt;
		mTexcoordLB	= texcoordlb;
		mTexcoordRB	= texcoordrb;
		mColor		= color;
		mTexture	= texture;
	}
};

//----------------------------------------------------------------------------
// BillboardBase
//----------------------------------------------------------------------------

struct BillboardBase : public Renderable
{
	Vector3		mPosition;
	Vector2		mSize;

	Vector2		mTexcoordLT;
	Vector2		mTexcoordRT;
	Vector2		mTexcoordLB;
	Vector2		mTexcoordRB;

	_dword		mColorLT;
	_dword		mColorRT;
	_dword		mColorLB;
	_dword		mColorRB;

	ITexture*	mTexture;
};

//----------------------------------------------------------------------------
// BillboardScreenAligned
//----------------------------------------------------------------------------

struct BillboardScreenAligned : public BillboardBase
{
	_float		mRotation;

	BillboardScreenAligned( )
	{
		mType		= _TYPE_BILLBOARD_SCREENALIGNED;
	}

	BillboardScreenAligned( const Vector3& position, const Vector2& size, _dword color,
		_float rotation, ITexture* texture, _dword group, _dword layer )
	{
		mType		= _TYPE_BILLBOARD_SCREENALIGNED;
		mPosition	= position;
		mSize		= size;
		mTexcoordLT	= Vector2( 0.0f, 0.0f );
		mTexcoordRT	= Vector2( 1.0f, 0.0f );
		mTexcoordLB	= Vector2( 0.0f, 1.0f );
		mTexcoordRB	= Vector2( 1.0f, 1.0f );
		mRotation	= rotation;
		mColorLT	= color;
		mColorRT	= color;
		mColorLB	= color;
		mColorRB	= color;
		mTexture	= texture;
		mModelGroup = group;
		mModelLayer = layer;
	}

	BillboardScreenAligned( const Vector3& position, const Vector2& size, const Vector2& texcoordlt,
		const Vector2& texcoordrb, _dword color, _float rotation, ITexture* texture, _dword group, _dword layer )
	{
		mType		= _TYPE_BILLBOARD_SCREENALIGNED;
		mPosition	= position;
		mSize		= size;
		mTexcoordLT	= texcoordlt;
		mTexcoordRT	= Vector2( texcoordrb.x, texcoordlt.y );
		mTexcoordLB	= Vector2( texcoordlt.x, texcoordrb.y );
		mTexcoordRB	= texcoordrb;
		mRotation	= rotation;
		mColorLT	= color;
		mColorRT	= color;
		mColorLB	= color;
		mColorRB	= color;
		mTexture	= texture;
		mModelGroup = group;
		mModelLayer = layer;
	}

	BillboardScreenAligned( const Vector3& position, const Vector2& size, const Vector2& texcoordlt, const Vector2& texcoordrt,
		const Vector2& texcoordlb, const Vector2& texcoordrb, _dword colorlt, _dword colorrt, _dword colorlb, _dword colorrb, _float rotation, ITexture* texture, _dword group, _dword layer )
	{
		mType		= _TYPE_BILLBOARD_SCREENALIGNED;
		mPosition	= position;
		mSize		= size;
		mTexcoordLT	= texcoordlt;
		mTexcoordRT	= texcoordrt;
		mTexcoordLB	= texcoordlb;
		mTexcoordRB	= texcoordrb;
		mRotation	= rotation;
		mColorLT	= colorlt;
		mColorRT	= colorrt;
		mColorLB	= colorlb;
		mColorRB	= colorrb;
		mTexture	= texture;
		mModelGroup = group;
		mModelLayer = layer;
	}
};

//----------------------------------------------------------------------------
// BillboardWorldOriented
//----------------------------------------------------------------------------

struct BillboardWorldOriented : public BillboardBase
{
	_float		mRotation;

	BillboardWorldOriented( )
	{
		mType		= _TYPE_BILLBOARD_WORLDORIENTED;
	}

	BillboardWorldOriented( const Vector3& position, const Vector2& size, _dword color,
		_float rotation, ITexture* texture, _dword group, _dword layer )
	{
		mType		= _TYPE_BILLBOARD_WORLDORIENTED;
		mPosition	= position;
		mSize		= size;
		mTexcoordLT	= Vector2( 0.0f, 0.0f );
		mTexcoordRT	= Vector2( 1.0f, 0.0f );
		mTexcoordLB	= Vector2( 0.0f, 1.0f );
		mTexcoordRB	= Vector2( 1.0f, 1.0f );
		mRotation	= rotation;
		mColorLT	= color;
		mColorRT	= color;
		mColorLB	= color;
		mColorRB	= color;
		mTexture	= texture;
		mModelGroup = group;
		mModelLayer = layer;
	}

	BillboardWorldOriented( const Vector3& position, const Vector2& size, const Vector2& texcoordlt,
		const Vector2& texcoordrb, _dword color, _float rotation, ITexture* texture, _dword group, _dword layer )
	{
		mType		= _TYPE_BILLBOARD_WORLDORIENTED;
		mPosition	= position;
		mSize		= size;
		mTexcoordLT	= texcoordlt;
		mTexcoordRT	= Vector2( texcoordrb.x, texcoordlt.y );
		mTexcoordLB	= Vector2( texcoordlt.x, texcoordrb.y );
		mTexcoordRB	= texcoordrb;
		mRotation	= rotation;
		mColorLT	= color;
		mColorRT	= color;
		mColorLB	= color;
		mColorRB	= color;
		mTexture	= texture;
		mModelGroup = group;
		mModelLayer = layer;
	}

	BillboardWorldOriented( const Vector3& position, const Vector2& size, const Vector2& texcoordlt, const Vector2& texcoordrt,
		const Vector2& texcoordlb, const Vector2& texcoordrb, _dword colorlt, _dword colorrt, _dword colorlb, _dword colorrb, _float rotation, ITexture* texture, _dword group, _dword layer )
	{
		mType		= _TYPE_BILLBOARD_WORLDORIENTED;
		mPosition	= position;
		mSize		= size;
		mTexcoordLT	= texcoordlt;
		mTexcoordRT	= texcoordrt;
		mTexcoordLB	= texcoordlb;
		mTexcoordRB	= texcoordrb;
		mRotation	= rotation;
		mColorLT	= colorlt;
		mColorRT	= colorrt;
		mColorLB	= colorlb;
		mColorRB	= colorrb;
		mTexture	= texture;
		mModelGroup = group;
		mModelLayer = layer;
	}
};

//----------------------------------------------------------------------------
// BillboardAxial
//----------------------------------------------------------------------------

struct BillboardAxial : public BillboardBase
{
	Vector3		mAxis;

	BillboardAxial( )
	{
		mType		= _TYPE_BILLBOARD_AXIAL;
	}

	BillboardAxial( const Vector3& position, const Vector3& axis, const Vector2& size,
		_dword color, ITexture* texture, _dword group, _dword layer )
	{
		mType		= _TYPE_BILLBOARD_AXIAL;
		mPosition	= position;
		mAxis		= axis;
		mSize		= size;
		mTexcoordLT	= Vector2( 0.0f, 0.0f );
		mTexcoordRT	= Vector2( 1.0f, 0.0f );
		mTexcoordLB	= Vector2( 0.0f, 1.0f );
		mTexcoordRB	= Vector2( 1.0f, 1.0f );
		mColorLT	= color;
		mColorRT	= color;
		mColorLB	= color;
		mColorRB	= color;
		mTexture	= texture;
		mModelGroup = group;
		mModelLayer = layer;
	}

	BillboardAxial( const Vector3& position, const Vector3& axis, const Vector2& size,
		const Vector2& texcoordlt, const Vector2& texcoordrb, _dword color, ITexture* texture, _dword group, _dword layer )
	{
		mType		= _TYPE_BILLBOARD_AXIAL;
		mPosition	= position;
		mAxis		= axis;
		mSize		= size;
		mTexcoordLT	= texcoordlt;
		mTexcoordRT	= Vector2( texcoordrb.x, texcoordlt.y );
		mTexcoordLB	= Vector2( texcoordlt.x, texcoordrb.y );
		mTexcoordRB	= texcoordrb;
		mColorLT	= color;
		mColorRT	= color;
		mColorLB	= color;
		mColorRB	= color;
		mTexture	= texture;
		mModelGroup = group;
		mModelLayer = layer;
	}

	BillboardAxial( const Vector3& position, const Vector3& axis, const Vector2& size, const Vector2& texcoordlt, const Vector2& texcoordrt,
		const Vector2& texcoordlb, const Vector2& texcoordrb, _dword colorlt, _dword colorrt, _dword colorlb, _dword colorrb, ITexture* texture, _dword group, _dword layer )
	{
		mType		= _TYPE_BILLBOARD_AXIAL;
		mPosition	= position;
		mAxis		= axis;
		mSize		= size;
		mTexcoordLT	= texcoordlt;
		mTexcoordRT	= texcoordrt;
		mTexcoordLB	= texcoordlb;
		mTexcoordRB	= texcoordrb;
		mColorLT	= colorlt;
		mColorRT	= colorrt;
		mColorLB	= colorlb;
		mColorRB	= colorrb;
		mTexture	= texture;
		mModelGroup = group;
		mModelLayer = layer;
	}
};

};
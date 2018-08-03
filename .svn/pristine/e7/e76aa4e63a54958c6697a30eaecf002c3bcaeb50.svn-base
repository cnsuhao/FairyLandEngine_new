//============================================================================
// RenderStatistics.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

namespace FL
{

//----------------------------------------------------------------------------
// RenderStatistics
//----------------------------------------------------------------------------

class RenderStatistics : public IRenderStatistics
{
public:
	_float		mFramePerSecond;
	_dword		mFrameTickcount;
	_dword		mFrameNumber;
	_dword		mTotalFrameCount;
	_dword		mTextureMemoryUsed;
	_dword		mVertexMemoryUsed;
	_dword		mIndexMemoryUsed;
	_dword		mPolygonNumber;
	_dword		mVertexNumber;
	_dword		mUseTextureTimes;
	_dword		mDrawPrimitiveTimes;

public:
	RenderStatistics( );
	~RenderStatistics( );

	_void		BeforePresent( );
	_void		AfterPresent( );

	_void		DrawPrimitive( _dword vertex, _dword polygon );

	virtual _float	GetFramePerSecond( ) const;

	virtual _dword	GetTextureMemoryUsed( ) const;
	virtual _dword	GetVertexMemoryUsed( ) const;
	virtual _dword	GetIndexMemoryUsed( ) const;

	virtual _dword	GetPolygonNumber( ) const;
	virtual _dword	GetVertexNumber( ) const;

	virtual _dword	GetUseTextureTimes( ) const;
	virtual _dword	GetDrawPrimitiveTimes( ) const;
};

};
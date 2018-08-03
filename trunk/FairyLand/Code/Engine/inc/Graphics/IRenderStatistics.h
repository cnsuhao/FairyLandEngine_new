//============================================================================
// IRenderStatistics.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

namespace FL
{

//----------------------------------------------------------------------------
// IRenderStatistics
//----------------------------------------------------------------------------

class IRenderStatistics
{
public:	
	virtual _float	GetFramePerSecond( ) const = 0;

	virtual _dword	GetTextureMemoryUsed( ) const = 0;
	virtual _dword	GetVertexMemoryUsed( ) const = 0;
	virtual _dword	GetIndexMemoryUsed( ) const = 0;
	virtual _dword	GetPolygonNumber( ) const = 0;
	virtual _dword	GetVertexNumber( ) const = 0;

	virtual _dword	GetUseTextureTimes( ) const = 0;
	virtual _dword	GetDrawPrimitiveTimes( ) const = 0;
};

};
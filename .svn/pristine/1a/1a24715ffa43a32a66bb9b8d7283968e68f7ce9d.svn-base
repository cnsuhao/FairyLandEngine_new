//============================================================================
// ModelLoader.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// ModelLoader
//----------------------------------------------------------------------------

class ModelLoader
{
private:
	BinFile&	mFile;

public:
	ModelLoader( BinFile& file );

	_bool	IsFormatValid( );

	_bool	LoadChunk( ModelFileChunk& chunk );
	_bool	SkipChunk( ModelFileChunk& chunk );

	_bool	LoadChunk( ModelTextureAnimation& animation );
	_bool	LoadChunk( ModelMesh& mesh, _bool userbuffer = _false );
	_bool	LoadChunk( ModelMeshAnimation& animation );
	_bool	LoadChunk( ModelSkeleton& skeleton );
	_bool	LoadChunk( ModelSkeletonAnimation& animation );
	_bool	LoadChunk( ModelTimeTag& timetag );
	_bool	LoadChunk( ModelMarker& marker );
	_bool	LoadChunk( ModelCamera& camera );
	_bool	LoadChunk( ModelModifier& modifier );
	_bool	LoadChunk( ModelAlteration& alteration );
};

};
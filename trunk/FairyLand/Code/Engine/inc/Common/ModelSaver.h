//============================================================================
// ModelSaver.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// ModelSaver
//----------------------------------------------------------------------------

class ModelSaver
{
private:
	File	mFile;

public:
	_bool	Create( StringPtr filename );

	_bool	SaveChunk( const ModelTextureAnimation& animation );
	_bool	SaveChunk( const ModelMesh& mesh, _bool userbuffer = _false );
	_bool	SaveChunk( const ModelMeshAnimation& animation );
	_bool	SaveChunk( const ModelSkeleton& skeleton );
	_bool	SaveChunk( const ModelSkeletonAnimation& animation );
	_bool	SaveChunk( const ModelTimeTag& timetag );
	_bool	SaveChunk( const ModelMarker& marker );
	_bool	SaveChunk( const ModelCamera& camera );
	_bool	SaveChunk( const ModelModifier& modifier );
	_bool	SaveChunk( const ModelAlteration& alteration );
};

};
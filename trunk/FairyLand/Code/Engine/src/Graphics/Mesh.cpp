//============================================================================
// Mesh.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandGraphics.h"

//----------------------------------------------------------------------------
// Mesh Implementation
//----------------------------------------------------------------------------

Mesh::Mesh( StringPtr name ) : mSkeleton( _null ), mName( name ), mBoundBox( AxisAlignedBox::cNullBox )
{
	mOriginalModelGroup		= 0;
	mOriginalModelNumber	= 0;
	mMaxInfluenceNumber		= 0;

	mUpdateInterval			= 0;
	mUpdateTickcount		= 0;
	mUpdateFlag				= 0;
	mUserData				= 0;

	mModelChangeListener	= _null;

	IncreaseRefCount( );
}

Mesh::~Mesh( )
{
	for ( _dword i = 0; i < mModelArray.Number( ); i ++ )
		GetGeometryFactory( ).ReleaseGeometry( mModelArray[i].mGeometry );

	GetModelFactory( ).ReleaseSkeleton( mSkeleton );
}

_bool Mesh::LoadModel( BinFile& modelfile, _dword modelgroup )
{
	ModelLoader loader( modelfile );

	if ( loader.IsFormatValid( ) == _false )
		return _false;

	_dword modelnumber = 0;

	// Read chunks in the file.
	ModelFileChunk chunk;
	while ( loader.LoadChunk( chunk ) )
	{
		// Found mesh chunk.
		if ( chunk.mChunkID == ModelFileChunk::_CHUNKID_MSH )
		{
			ModelMesh mesh;

			// Load mesh chunk.
			if ( loader.LoadChunk( mesh ) == _false )
				return _false;

			if ( mesh.mVertexNumber == 0 || mesh.mFaceNumber == 0 )
				continue;

			Model model;

			model.mUserData = 0;

			// Create geometry of mesh.
			model.mGeometry = GetGeometryFactory( ).CreateGeometry( mesh.mFormatFlag,
				mesh.mVertexNumber, mesh.mFaceNumber * 3, Geometry::_GEOMETRY_TRIANGLELIST, mesh.mFaceNumber );

			// Create geometry failed.
			if ( model.mGeometry.mVertexDeclaration == _null )
				return _false;

			// Set model name.
			model.mModelName = mesh.mMeshName;

			// Set geometry attribute.
			model.mGeometry.mRenderFlag	= mesh.mRenderFlag;
			model.mGeometry.mModelGroup	= modelgroup;
			model.mGeometry.mModelLayer	= mesh.mModelLayer;

			// Get model layer from mesh name again.
			model.mGeometry.mModelLayer = ModelHelper::GetRenderLayerFromName( mesh.mMeshName );

			// Fill vertex buffer.
			_byte* vertexbuffer = (_byte*) model.mGeometry.mVertexBuffer->Lock( 0, 0 );
			_byte* backupbuffer = _null;
			_byte* helperbuffer = _null;

			if ( model.mGeometry.mBackupBuffer != _null )
				backupbuffer = (_byte*) model.mGeometry.mBackupBuffer->Lock( 0, 0 );

			if ( model.mGeometry.mHelperBuffer != _null )
				helperbuffer = (_byte*) model.mGeometry.mHelperBuffer->Lock( 0, 0 );

			if ( vertexbuffer != _null )
			{
				if ( mesh.mFormatFlag & Geometry::_FORMATFLAG_INFLUENCE )
				{
					ModelHelper::CopyVertexBufferIntoMemory( mesh.mFormatFlag, mesh.mVertexNumber,
						vertexbuffer, helperbuffer, mesh.mVertexBuffer.mBuffer, GetGeometryFactory( ).GetVertexAddon( ) );
				}
				else
				{
					ModelHelper::CopyVertexBufferIntoMemory( mesh.mFormatFlag, mesh.mVertexNumber,
						vertexbuffer, mesh.mVertexBuffer.mBuffer, GetGeometryFactory( ).GetVertexAddon( ) );
				}
			}

			if ( backupbuffer != _null )
				Memory::MemCpy( backupbuffer, vertexbuffer, model.mGeometry.mVertexBuffer->GetSize( ) );

			if ( model.mGeometry.mHelperBuffer != _null )
				model.mGeometry.mHelperBuffer->Unlock( );

			model.mGeometry.mVertexBuffer->Unlock( );

			// Fill index buffer.
			_byte* indexbuffer = (_byte*) model.mGeometry.mIndexBuffer->Lock( 0, 0 );

			if ( indexbuffer != _null )
				ModelHelper::CopyIndexBufferIntoMemory( mesh.mFaceNumber * 3, indexbuffer, mesh.mIndexBuffer.mBuffer );

			model.mGeometry.mIndexBuffer->Unlock( );

			// Compute bound box when necessary.
			if ( mesh.mBoundBox == AxisAlignedBox::cNullBox )
				mesh.mBoundBox = ModelHelper::GetBoundBoxFromVertexBuffer( mesh.mFormatFlag, mesh.mVertexBuffer );

			// Set bound box.
			model.mGeometry.mVertexBuffer->SetBoundBox( mesh.mBoundBox );

			// This is billboard, use depth bias as billboard size.
			if ( model.mGeometry.mRenderFlag & Geometry::_RENDERFLAG_BILLBOARD )
			{
				Vector3 size = mesh.mBoundBox.BoxSize( );

				model.mGeometry.mDepthBias = Math::Max( Math::Max( size.x, size.y ), size.z );
			}

			StringPtr textures[4] = { mesh.mTexName1, mesh.mTexName2, mesh.mTexName3, mesh.mTexName4 };

			// Load textures.
			for ( _dword i = 0; i < 4; i ++ )
			{
				if ( textures[i] == L"" )
					break;

				model.mGeometry.mTexture[i] = GetTextureFactory( ).CreateTexture( textures[i] );
			}

			// Insert into model array.
			mModelArray.Append( model );

			modelnumber ++;

			// Set model change flag.
			if ( mModelChangeListener != _null )
				*mModelChangeListener = _true;

			// Adjust bound box to whole mesh.
			if ( mBoundBox == AxisAlignedBox::cNullBox )
				mBoundBox = mesh.mBoundBox;
			else
				mBoundBox += mesh.mBoundBox;

			if ( mMaxInfluenceNumber < mesh.mMaxInfluenceNumber )
				mMaxInfluenceNumber = mesh.mMaxInfluenceNumber;
		}
		// Found marker chunk.
		else if ( chunk.mChunkID == ModelFileChunk::_CHUNKID_MRK )
		{
			ModelMarker marker;

			// Load marker chunk.
			if ( loader.LoadChunk( marker ) == _false )
				return _false;

			// Insert into marker array.
			mMarkerArray.Append( marker );
		}
		// Found camera chunk.
		else if ( chunk.mChunkID == ModelFileChunk::_CHUNKID_CAM ||
				  chunk.mChunkID == ModelFileChunk::_CHUNKID_FOV )
		{
			ModelCamera camera;

			// Load camera chunk.
			if ( loader.LoadChunk( camera ) == _false )
				return _false;

			if ( camera.mCamera.mFOVDegree == 0.0f )
				camera.mCamera.mFOVDegree = 58.0f;

			// Insert into camera array.
			mCameraArray.Append( camera );
		}
		// Found texture animation.
		else if ( chunk.mChunkID == ModelFileChunk::_CHUNKID_TAN )
		{
			ModelTextureAnimation animation;

			// Load animation chunk.
			if ( loader.LoadChunk( animation ) == _false )
				return _false;

			ITexture* texture = GetTextureFactory( ).CreateTexture( animation.mTargetTexName );

			if ( texture == _null )
				continue;

			GetTextureFactory( ).ReleaseTexture( texture );
		}
		// Skip other chunks.
		else
		{
			if ( loader.SkipChunk( chunk ) == _false )
				return _false;
		}
	}

	mUpdateFlag				= 0;
	mOriginalModelGroup		= modelgroup;
	mOriginalModelNumber	= modelnumber;

	return modelnumber > 0;
}

_void Mesh::AttachSkeleton( ISkeleton* skeleton )
{
	if ( mSkeleton == skeleton )
		return;

	GetModelFactory( ).ReleaseSkeleton( mSkeleton );
	mSkeleton = GetModelFactory( ).CloneSkeleton( skeleton, _true );
}

_void Mesh::CombineModel( Mesh* mesh )
{
	for ( _dword i = 0; i < mesh->mModelArray.Number( ); i ++ )
	{
		mBoundBox += mesh->GetModelBoundBox( i );

		Model newmodel;

		// Clone geometry.
		newmodel.mModelName		= mesh->mModelArray[i].mModelName;
		newmodel.mUserData		= mesh->mModelArray[i].mUserData;
		newmodel.mUserString	= mesh->mModelArray[i].mUserString;
		newmodel.mGeometry		= GetGeometryFactory( ).CloneGeometry( mesh->GetModelGeometry( i ), _false );

		newmodel.mGeometry.mModelGroup = mOriginalModelGroup;

		mModelArray.Append( newmodel );

		// Set model change flag.
		if ( mModelChangeListener != _null )
			*mModelChangeListener = _true;
	}

	if ( mMaxInfluenceNumber < mesh->mMaxInfluenceNumber )
		mMaxInfluenceNumber = mesh->mMaxInfluenceNumber;

	mUpdateFlag = 0;
}

_void Mesh::CombineMarker( Mesh* mesh )
{
	for ( _dword i = 0; i < mesh->mMarkerArray.Number( ); i ++ )
		mMarkerArray.Append( mesh->mMarkerArray[i] );
}

_void Mesh::SetupUpdateRef( )
{
	// Only setup ref when not updated.
	if ( mUpdateFlag != _UPDATE_NOTUPDATED )
		return;

	Skeleton* skeleton = (Skeleton*) mSkeleton;
	FL_ASSERT( skeleton != _null )

	// Set update flag to input ready.
	mUpdateFlag = _UPDATE_INPUTREADY;
}

_void Mesh::SwitchBuffer( )
{
	// Buffer not ready yet.
	if ( mUpdateFlag != _UPDATE_OUTPUTREADY )
		return;

	for ( _dword i = 0; i < mModelArray.Number( ); i ++ )
	{
		Geometry& geometry = mModelArray[i].mGeometry;

		// There is no backup buffer, skip it.
		if ( geometry.mBackupBuffer == _null )
			continue;

		GetGeometryFactory( ).CopyVertexBuffer( geometry.mVertexBuffer, geometry.mBackupBuffer );
	}

	// Set update flag to finish.
	mUpdateFlag = _UPDATE_FINISHUPDATE;
}

_void Mesh::UpdateBuffer( )
{
	// Only start update when input is ready.
	if ( mUpdateFlag != _UPDATE_INPUTREADY )
		return;

	if ( mUpdateInterval != 0 )
	{
		if ( System::GetCurrentTickcount( ) - mUpdateTickcount < mUpdateInterval )
			return;

		mUpdateTickcount = System::GetCurrentTickcount( );
	}

	// Set update flag to processing.
	mUpdateFlag = _UPDATE_PROCESSING;

	Skeleton* skeleton = (Skeleton*) mSkeleton;
	FL_ASSERT( skeleton != _null )

	// Ready to update bound box.
	mBoundBox = AxisAlignedBox::cInitBox;

	// Prepare parameter for algorithm.
	const Vector3* bonelocaltranslation	= &Vector3::cOrigin;
	const Matrix3* bonelocalrotation	= &Matrix3::cIdentity;
	const Matrix4* bonealtertransform	= &Matrix4::cIdentity;
	_bool		   needaltertransform	= _false;

	// Update each geometry.
	for ( _dword i = 0; i < mModelArray.Number( ); i ++ )
	{
		Geometry& geometry = mModelArray[i].mGeometry;

		if ( ( geometry.mFormatFlag & Geometry::_FORMATFLAG_INFLUENCE ) == 0 )
			continue;

		IVertexBuffer* meshvertexbuffer = geometry.mVertexBuffer;
		IVertexBuffer* meshhelperbuffer = geometry.mHelperBuffer;

		if ( meshvertexbuffer == _null || meshhelperbuffer == _null )
			continue;

		_dword vertexnumber	= meshvertexbuffer->GetSize( ) / geometry.mVertexSize;
		_dword helpernumber = meshhelperbuffer->GetSize( ) / geometry.mHelperSize;

		if ( vertexnumber == 0 || vertexnumber != helpernumber )
			continue;

		_byte* vertexbuffer = (_byte*) meshvertexbuffer->Lock( geometry.mStartVertex * geometry.mVertexSize,
			geometry.mVertexNumber * geometry.mVertexSize, IGeometryFactory::_FLAG_DISCARD );

		_byte* helperbuffer = (_byte*) meshhelperbuffer->Lock( geometry.mStartVertex * geometry.mHelperSize,
			geometry.mVertexNumber * geometry.mHelperSize, IGeometryFactory::_FLAG_DISCARD );

		if ( vertexbuffer == _null || helperbuffer == _null )
		{
			meshvertexbuffer->Unlock( );
			meshhelperbuffer->Unlock( );

			continue;
		}

		_dword influenceoffset = geometry.mHelperSize - sizeof( ModelInfluenceGroup );

		AxisAlignedBoxMaker boundboxmaker;

		// Process each vertex.
		for ( _dword j = 0; j < vertexnumber; j ++ )
		{
			Vector3* sourcevertex = (Vector3*) ( helperbuffer );
			Vector3* targetvertex = (Vector3*) ( vertexbuffer );

			// Get influence group from helper buffer.
			const ModelInfluenceGroup& influencegroup = *(ModelInfluenceGroup*) ( helperbuffer + influenceoffset );

			// Process bone influence.
			for ( _dword k = 0; k < ModelInfluenceGroup::_MAX_INFLUENCE_NUMBER; k ++ )
			{
				_dword boneid = influencegroup.mBoneIDList[k];
				_float weight = influencegroup.mWeightList[k];

				if ( boneid >= mSkeleton->GetBoneNumber( ) )
				{
					// There is no influence, so we just copy position ( and normal ) from helper buffer.
					if ( k == 0 )
					{
						targetvertex[0] = sourcevertex[0];

						if ( geometry.mFormatFlag & Geometry::_FORMATFLAG_NORMAL )
							targetvertex[1] = sourcevertex[1];
					}

					break;
				}

				Skeleton::Bone& bone = skeleton->GetBoneByMap( boneid );

				bonelocaltranslation	= &bone.mLocalTranslation;
				bonelocalrotation		= &bone.mLocalRotation;
				needaltertransform		= skeleton->NeedBoneAlterTransform( boneid );

				if ( needaltertransform )
					bonealtertransform	= skeleton->GetBoneAlterTransform( boneid );

				// Update vertex position.
				Vector3 boneposition( sourcevertex[0] );

				// Alter happened, we must rebind this vertex to another bone.
				if ( needaltertransform )
					boneposition *= *bonealtertransform;

				boneposition *= *bonelocalrotation;
				boneposition += *bonelocaltranslation;

				if ( k == 0 )
					targetvertex[0]	 = boneposition * weight;
				else
					targetvertex[0]	+= boneposition * weight;

				// Update vertex normal.
				if ( geometry.mFormatFlag & Geometry::_FORMATFLAG_NORMAL )
				{
					Vector3 bonenormal( sourcevertex[1] );

					bonenormal *= *bonelocalrotation;

					if ( k == 0 )
						targetvertex[1]  = bonenormal * weight;
					else
						targetvertex[1] += bonenormal * weight;
				}
			}

			vertexbuffer += geometry.mVertexSize;
			helperbuffer += geometry.mHelperSize;

			// Combine bound box with a new vertex.
			boundboxmaker.Expand( targetvertex[0] );
		}

		meshvertexbuffer->Unlock( );
		meshhelperbuffer->Unlock( );

		meshvertexbuffer->SetBoundBox( boundboxmaker.GetAxisAlignedBox( ) );

		// Combine bound box.
		mBoundBox += boundboxmaker.GetAxisAlignedBox( );
	}

	// Set update flag to output buffer ready.
	mUpdateFlag = _UPDATE_OUTPUTREADY;
}

IMesh* Mesh::Clone( _dword modelgroup, _bool cloneall ) const
{
	if ( mOriginalModelNumber == 0 && cloneall == _false )
		return _null;

	FL_ASSERT( mOriginalModelNumber <= mModelArray.Number( ) )

	// Clone an new mesh.
	Mesh* newmesh = new Mesh( mName );

	// Reference skeleton.
	newmesh->mSkeleton = GetModelFactory( ).CloneSkeleton( mSkeleton, _true );

	// Copy bound box.
	newmesh->mBoundBox = mBoundBox;

	_dword modelnumber = mOriginalModelNumber;

	if ( cloneall )
		modelnumber = mModelArray.Number( );

	modelnumber = Math::Min( modelnumber, mModelArray.Number( ) );

	// Clone models.
	for ( _dword i = 0; i < modelnumber; i ++ )
	{
		Model newmodel;

		// Clone geometry.
		newmodel.mModelName	= mModelArray[i].mModelName;
		newmodel.mUserData	= 0;
		newmodel.mGeometry	= GetGeometryFactory( ).CloneGeometry( mModelArray[i].mGeometry, _false );

		newmodel.mGeometry.mModelGroup			= modelgroup;
		newmodel.mGeometry.mTextureFactor		= 0xFFFFFFFF;
		newmodel.mGeometry.mMaterialDiffuse		= 0xFFFFFFFF;
		newmodel.mGeometry.mMaterialAmbient		= 0xFF202020;
		newmodel.mGeometry.mMaterialSpecular	= 0;
		newmodel.mGeometry.mMaterialEmissive	= 0;
		newmodel.mGeometry.mMaterialPower		= 0.0f;
		newmodel.mGeometry.mMirror				= _false;
		newmodel.mGeometry.mProjectTexture		= _false;

		newmesh->mModelArray.Append( newmodel );

		// Set model change flag.
		if ( newmesh->mModelChangeListener != _null )
			*newmesh->mModelChangeListener = _true;
	}

	newmesh->mOriginalModelGroup	= modelgroup;
	newmesh->mOriginalModelNumber	= mOriginalModelNumber;
	newmesh->mMaxInfluenceNumber	= mMaxInfluenceNumber;

	// Clone markers.
	for ( _dword i = 0; i < mMarkerArray.Number( ); i ++ )
		newmesh->mMarkerArray.Append( mMarkerArray[i] );

	// Clone cameras.
	for ( _dword i = 0; i < mCameraArray.Number( ); i ++ )
		newmesh->mCameraArray.Append( mCameraArray[i] );

	return newmesh;
}

StringPtr Mesh::GetName( ) const
{
	return mName;
}

const AxisAlignedBox& Mesh::GetBoundBox( ) const
{
	return mBoundBox;
}

_dword Mesh::GetModelNumber( ) const
{
	return mModelArray.Number( );
}

StringPtr Mesh::GetModelName( _dword modelindex ) const
{
	return mModelArray[ modelindex ].mModelName;
}

const AxisAlignedBox& Mesh::GetModelBoundBox( _dword modelindex ) const
{
	return mModelArray[ modelindex ].mGeometry.mVertexBuffer->GetBoundBox( );
}

ITexture* Mesh::GetModelTexture( _dword modelindex, _dword textureindex ) const
{
	if ( textureindex >= Geometry::_MAX_TEXTURE_NUMBER )
		return _null;

	if ( modelindex >= mModelArray.Number( ) )
		return _null;

	return mModelArray[ modelindex ].mGeometry.mTexture[ textureindex ];
}

Geometry& Mesh::GetModelGeometry( _dword modelindex )
{
	return mModelArray[ modelindex ].mGeometry;
}

_dword Mesh::GetMarkerNumber( ) const
{
	return mMarkerArray.Number( );
}

StringPtr Mesh::GetMarkerName( _dword markerindex ) const
{
	return mMarkerArray[ markerindex ].mMarkerName;
}

_dword Mesh::GetMarkerHandle( _dword markerindex ) const
{
	return mMarkerArray[ markerindex ].mMarkerHandle;
}

const Vector3& Mesh::GetMarkerTranslation( _dword markerindex ) const
{
	return mMarkerArray[ markerindex ].mTranslation;
}

const Quaternion& Mesh::GetMarkerRotation( _dword markerindex ) const
{
	return mMarkerArray[ markerindex ].mRotation;
}

Matrix4 Mesh::GetMarkerTransform( _dword markerindex ) const
{
	Matrix4 transform;
	transform.FromTranslationRotation( mMarkerArray[ markerindex ].mTranslation, mMarkerArray[ markerindex ].mRotation );

	return transform;
}

_dword Mesh::GetCameraNumber( ) const
{
	return mCameraArray.Number( );
}

StringPtr Mesh::GetCameraName( _dword cameraindex ) const
{
	return mCameraArray[ cameraindex ].mCameraName;
}

const Camera& Mesh::GetCamera( _dword cameraindex ) const
{
	return mCameraArray[ cameraindex ].mCamera;
}

_dword Mesh::SearchModel( StringPtr modelname ) const
{
	for ( _dword i = 0; i < mModelArray.Number( ); i ++ )
	{
		if ( mModelArray[i].mModelName.CompareLowercase( modelname ) == _true )
			return i;
	}

	return -1;
}

_dword Mesh::SearchMarker( StringPtr markername ) const
{
	for ( _dword i = 0; i < mMarkerArray.Number( ); i ++ )
	{
		if ( mMarkerArray[i].mMarkerName.CompareLowercase( markername ) == _true )
			return i;
	}

	return -1;
}

_dword Mesh::SearchCamera( StringPtr cameraname ) const
{
	for ( _dword i = 0; i < mCameraArray.Number( ); i ++ )
	{
		if ( mCameraArray[i].mCameraName.CompareLowercase( cameraname ) == _true )
			return i;
	}

	return -1;
}

_bool Mesh::RemoveModel( _dword modelindex )
{
	if ( modelindex >= mModelArray.Number( ) )
		return _false;

	// The mesh is broken, so we cant use it make a clone, just set model number to 0.
	if ( modelindex <= mOriginalModelNumber )
		mOriginalModelNumber = 0;

	GetGeometryFactory( ).ReleaseGeometry( mModelArray[ modelindex ].mGeometry );

	mModelArray.Remove( modelindex );

	// Set model change flag.
	if ( mModelChangeListener != _null )
		*mModelChangeListener = _true;

	return _true;
}

_bool Mesh::RemoveMarker( _dword markerindex )
{
	if ( markerindex >= mMarkerArray.Number( ) )
		return _false;

	mMarkerArray.Remove( markerindex );

	return _true;
}

_bool Mesh::RemoveCamera( _dword cameraindex )
{
	if ( cameraindex >= mCameraArray.Number( ) )
		return _false;

	mCameraArray.Remove( cameraindex );

	return _true;
}

_void Mesh::SetAllModelUserData( _dword userdata )
{
	for ( _dword i = 0; i < mModelArray.Number( ); i ++ )
		mModelArray[i].mUserData = userdata;
}

_void Mesh::SetModelUserData( _dword modelindex, _dword userdata )
{
	mModelArray[ modelindex ].mUserData = userdata;
}

_dword Mesh::GetModelUserData( _dword modelindex ) const
{
	return mModelArray[ modelindex ].mUserData;
}

_void Mesh::SetAllModelUserString( StringPtr string )
{
	for ( _dword i = 0; i < mModelArray.Number( ); i ++ )
		mModelArray[i].mUserString = string;
}

_void Mesh::SetModelUserString( _dword modelindex, StringPtr string )
{
	mModelArray[ modelindex ].mUserString = string;
}

StringPtr Mesh::GetModelUserString( _dword modelindex )
{
	return mModelArray[ modelindex ].mUserString;
}

_void Mesh::SetModelChangeListener( _bool* listener )
{
	mModelChangeListener = listener;
}

_void Mesh::SetUpdateInterval( _dword interval )
{
	mUpdateInterval = interval;
}

_dword Mesh::GetUpdateInterval( ) const
{
	return mUpdateInterval;
}

_void Mesh::SetUserData( _dword userdata )
{
	mUserData = userdata;
}

_dword Mesh::GetUserData( ) const
{
	return mUserData;
}

_void Mesh::Update( _bool forceupdate )
{
	if ( mSkeleton == _null )
		return;

	if ( mModelArray.Number( ) == 0 )
		return;

	// Skeleton changed, so we need update mesh.
	if ( ( (Skeleton*) mSkeleton )->HasChanged( ) )
		mUpdateFlag = _UPDATE_NOTUPDATED;

	// Already updated, skip it.
	if ( mUpdateFlag == _UPDATE_FINISHUPDATE )
		return;

	// This mesh not render at this frame, we can delay update.
	if ( forceupdate == _false )
		return;

	// Set update flag to input ready.
	mUpdateFlag = _UPDATE_INPUTREADY;

	UpdateBuffer( );

	// Set update flag to updated.
	mUpdateFlag = _UPDATE_FINISHUPDATE;
}
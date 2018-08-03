//============================================================================
// VertexAnimation.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandGraphics.h"

//----------------------------------------------------------------------------
// VertexAnimation Implementation
//----------------------------------------------------------------------------

VertexAnimation::VertexAnimation( StringPtr name, _dword format, _float duration ) : Animation( name ), mKeyframeFormat( format )
{
	mMesh			= _null;
	mLastKeyframe1	= -1;
	mLastKeyframe2	= -1;

	mDuration		= duration;

	VertexKeyframe keyframe1;
	keyframe1.mTime = 0.0f;
	mKeyframeArray.Append( keyframe1 );

	VertexKeyframe keyframe2;
	keyframe2.mTime = duration;
	mKeyframeArray.Append( keyframe2 );

	IncreaseRefCount( );
}

VertexAnimation::~VertexAnimation( )
{
	GetModelFactory( ).ReleaseMesh( mMesh );
}

VertexAnimation::VertexKeyframe& VertexAnimation::AddKeyframe( _float time )
{
	VertexKeyframe keyframe1;
	VertexKeyframe keyframe2;

	// Get animation keyframe.
	GetKeyframe( time, keyframe1, keyframe2 );

	if ( keyframe1.mTime == time )
		return mKeyframeArray[ mLastKeyframe1 ];

	if ( keyframe2.mTime == time )
		return mKeyframeArray[ mLastKeyframe2 ];

	// Add new keyframe.
	VertexKeyframe newkeyframe;
	newkeyframe.mTime = time;
	mKeyframeArray.Insert( newkeyframe, mLastKeyframe2 );

	return mKeyframeArray[ mLastKeyframe2 ];
}

_void VertexAnimation::GetKeyframe( _float time, VertexKeyframe& keyframe1, VertexKeyframe& keyframe2 )
{
	_long i = 0, j = 0;

	// Get keyframe from array using binary search.
	if ( mLastKeyframe1 >= mKeyframeArray.Number( ) || mLastKeyframe2 >= mKeyframeArray.Number( ) )
	{
		BinarySearch< VertexKeyframe, _float > binarysearch;
		binarysearch.SearchAscending( mKeyframeArray.GetArrayBuffer( ), time, mKeyframeArray.Number( ), i, j );
	}
	else
	{
		i = mLastKeyframe1;
		j = mLastKeyframe2;

		// Try to fast adjust time to keyframe.
		if ( time < mKeyframeArray[i].mTime )
		{
			i = 0;
			j = 1;
		}
		else if ( time > mKeyframeArray[j].mTime )
		{
			i ++;
			j ++;
		}

		// Still cant get right keyframe, ust binary search.
		if ( time < mKeyframeArray[i].mTime || time > mKeyframeArray[j].mTime )
		{
			BinarySearch< VertexKeyframe, _float > binarysearch;
			binarysearch.SearchAscending( mKeyframeArray.GetArrayBuffer( ), time, mKeyframeArray.Number( ), i, j );
		}
	}

	keyframe1 = mKeyframeArray[i];
	keyframe2 = mKeyframeArray[j];

	mLastKeyframe1 = i;
	mLastKeyframe2 = j;
}

_void VertexAnimation::UpdateMesh( _float current )
{
	if ( mMesh == _null )
		return;

	if ( current < 0.0f )
		current = 0.0f;

	Animation::Update( mCurrent, current );

	if ( mCircular == _true )
	{
		if ( current >= mDuration )
			current = Math::Fmod( current, mDuration );
	}
	else
	{
		if ( current > mDuration )
			current = mDuration;
	}

	VertexKeyframe keyframe1;
	VertexKeyframe keyframe2;
	VertexKeyframe newkeyframe;

	// Get animation keyframe.
	GetKeyframe( current, keyframe1, keyframe2 );

	_float factor = ( current - keyframe1.mTime ) / ( keyframe2.mTime - keyframe1.mTime );

	// Get keyframe by interpolation two keyframes.
	{
		if ( mKeyframeFormat & Geometry::_FORMATFLAG_POSITION )
			newkeyframe.mPosition = Vector3::Lerp( keyframe1.mPosition, keyframe2.mPosition, factor );

		if ( mKeyframeFormat & Geometry::_FORMATFLAG_NORMAL )
			newkeyframe.mNormal = Vector3::Lerp( keyframe1.mNormal, keyframe2.mNormal, factor );

		if ( mKeyframeFormat & Geometry::_FORMATFLAG_DIFFUSE )
			newkeyframe.mDiffuse = Color::Lerp( Color( keyframe1.mDiffuse ), Color( keyframe2.mDiffuse ), factor );

		if ( mKeyframeFormat & Geometry::_FORMATFLAG_SPECULAR )
			newkeyframe.mSpecular = Color::Lerp( Color( keyframe1.mSpecular ), Color( keyframe2.mSpecular ), factor );
	}

	// Update each geometry.
	for ( _dword i = 0; i < mMesh->GetModelNumber( ); i ++ )
	{
		Geometry& geometry = mMesh->GetModelGeometry( i );

		_dword geotexnumber = ModelHelper::GetTexcoordNumberFromFormat( geometry.mFormatFlag );

		// Lock vertex buffer, ready to update.
		_byte* vertexbuffer = (_byte*) geometry.mVertexBuffer->Lock( geometry.mStartVertex * geometry.mVertexSize, geometry.mVertexNumber * geometry.mVertexSize );

		if ( vertexbuffer == _null )
		{
			geometry.mVertexBuffer->Unlock( );
			continue;
		}

		// Update each vertex data.
		for ( _dword i = 0; i < geometry.mVertexNumber; i ++ )
		{
			// Update position.
			if ( mKeyframeFormat & Geometry::_FORMATFLAG_POSITION )
			{
				*( (Vector3*) vertexbuffer ) = newkeyframe.mPosition;

				vertexbuffer += sizeof( Vector3 );
			}
			// Skip position, vertex buffer will sure have position.
			else
			{
				vertexbuffer += sizeof( Vector3 );
			}

			// Update normal.
			if ( mKeyframeFormat & Geometry::_FORMATFLAG_NORMAL )
			{
				*( (Vector3*) vertexbuffer ) = newkeyframe.mNormal;

				vertexbuffer += sizeof( Vector3 );
			}
			// Skip normal.
			else if ( geometry.mFormatFlag & Geometry::_FORMATFLAG_NORMAL )
			{
				vertexbuffer += sizeof( Vector3 );
			}

			// Update diffuse.
			if ( mKeyframeFormat & Geometry::_FORMATFLAG_DIFFUSE )
			{
				*( (_dword*) vertexbuffer ) = newkeyframe.mDiffuse;

				vertexbuffer += sizeof( _dword );
			}
			// Skip diffuse.
			else if ( geometry.mFormatFlag & Geometry::_FORMATFLAG_DIFFUSE )
			{
				vertexbuffer += sizeof( _dword );
			}

			// Update specular.
			if ( mKeyframeFormat & Geometry::_FORMATFLAG_SPECULAR )
			{
				*( (_dword*) vertexbuffer ) = newkeyframe.mSpecular;

				vertexbuffer += sizeof( _dword );
			}
			// Skip specular.
			else if ( geometry.mFormatFlag & Geometry::_FORMATFLAG_SPECULAR )
			{
				vertexbuffer += sizeof( _dword );
			}

			// Skip texcoord.
			vertexbuffer += sizeof( Vector2 ) * geotexnumber;

			// Skip binormal.
			if ( geometry.mFormatFlag & Geometry::_FORMATFLAG_BINORMAL )
				vertexbuffer += sizeof( Vector3 );

			// Skip tangent.
			if ( geometry.mFormatFlag & Geometry::_FORMATFLAG_TANGENT )
				vertexbuffer += sizeof( Vector3 );
		}

		// Update finished, unlock vertex buffer.
		geometry.mVertexBuffer->Unlock( );
	}

	mCurrent = current;
}

_void VertexAnimation::AttachMesh( IMesh* mesh )
{
	if ( mMesh == mesh )
		return;

	GetModelFactory( ).ReleaseMesh( mMesh );
	mMesh = GetModelFactory( ).CloneMesh( mesh, _true, _false, _null );
}

VertexAnimation* VertexAnimation::Clone( ) const
{
	// Clone an new mesh animation.
	VertexAnimation* newanimation = new VertexAnimation( mName, mKeyframeFormat, mDuration );

	// Reference mesh.
	newanimation->mMesh = GetModelFactory( ).CloneMesh( mMesh, _true, _false, _null );

	// Set animation circular type.
	newanimation->mCircular = mCircular;

	// Clone animation data.
	newanimation->mKeyframeArray = mKeyframeArray;

	return newanimation;
}

StringPtr VertexAnimation::GetName( ) const
{
	return mName;
}

_float VertexAnimation::GetDuration( ) const
{
	return mDuration;
}

_void VertexAnimation::SetCurrent( _float current )
{
	UpdateMesh( current );
}

_float VertexAnimation::GetCurrent( ) const
{
	return mCurrent;
}

_void VertexAnimation::SetSpeed( _float speed )
{
	mSpeed = speed;
}

_float VertexAnimation::GetSpeed( ) const
{
	return mSpeed;
}

_void VertexAnimation::SetCircular( _bool circular )
{
	mCircular = circular;
}

_bool VertexAnimation::GetCircular( ) const
{
	return mCircular;
}

_void VertexAnimation::SetBackward( _bool backward )
{
	mBackward = backward;
}

_bool VertexAnimation::GetBackward( ) const
{
	return mBackward;
}

_void VertexAnimation::SetAutoClearEvent( _bool autoclear )
{
	mAutoClearEvent = autoclear;
}

_bool VertexAnimation::GetAutoClearEvent( ) const
{
	return mAutoClearEvent;
}

_void VertexAnimation::SetUserData( _dword userdata )
{
	mUserData1 = userdata;
}

_dword VertexAnimation::GetUserData( ) const
{
	return mUserData1;
}

_void VertexAnimation::SetUserData2( _dword userdata )
{
	mUserData2 = userdata;
}

_dword VertexAnimation::GetUserData2( ) const
{
	return mUserData2;
}

_dword VertexAnimation::GetTotalEventNumber( ) const
{
	return Animation::GetTotalEventNumber( );
}

_float VertexAnimation::GetTotalEventTime( _dword index ) const
{
	return Animation::GetTotalEventTime( index );
}

StringPtr VertexAnimation::GetTotalEventName( _dword index ) const
{
	return Animation::GetTotalEventName( index );
}

_dword VertexAnimation::GetHappenedEventNumber( ) const
{
	return Animation::GetHappenedEventNumber( );
}

_float VertexAnimation::GetHappenedEventTime( _dword index ) const
{
	return Animation::GetHappenedEventTime( index );
}

StringPtr VertexAnimation::GetHappenedEventName( _dword index ) const
{
	return Animation::GetHappenedEventName( index );
}

_bool VertexAnimation::InsertEvent( _float time, StringPtr name )
{
	return Animation::InsertEvent( time, name );
}

_bool VertexAnimation::RemoveEvent( _dword index )
{
	return Animation::RemoveEvent( index );
}

_bool VertexAnimation::ClearEvent( )
{
	return Animation::ClearEvent( );
}

_void VertexAnimation::Update( _dword elapse, _float weight, _bool onlyroot )
{
	if ( elapse == 0 )
		return;

	UpdateMesh( mCurrent + elapse / 1000.0f * mSpeed );
}

_void VertexAnimation::Reset( )
{
	mCurrent = 0.0f;
}

IMesh* VertexAnimation::GetAttachedMesh( ) const
{
	return mMesh;
}

_bool VertexAnimation::SetPositionKeyframe( _float time, const Vector3& position )
{
	if ( time < 0.0f || time > mDuration )
		return _false;

	if ( ( mKeyframeFormat & Geometry::_FORMATFLAG_POSITION ) == 0 )
		return _false;

	// Add animation keyframe.
	VertexKeyframe& newkeyframe = AddKeyframe( time );

	newkeyframe.mPosition = position;

	return _true;
}

_bool VertexAnimation::SetNormalKeyframe( _float time, const Vector3& normal )
{
	if ( time < 0.0f || time > mDuration )
		return _false;

	if ( ( mKeyframeFormat & Geometry::_FORMATFLAG_NORMAL ) == 0 )
		return _false;

	// Add animation keyframe.
	VertexKeyframe& newkeyframe = AddKeyframe( time );

	newkeyframe.mNormal = normal;

	return _true;
}

_bool VertexAnimation::SetDiffuseKeyframe( _float time, _dword diffuse )
{
	if ( time < 0.0f || time > mDuration )
		return _false;

	if ( ( mKeyframeFormat & Geometry::_FORMATFLAG_DIFFUSE ) == 0 )
		return _false;

	// Add animation keyframe.
	VertexKeyframe& newkeyframe = AddKeyframe( time );

	newkeyframe.mDiffuse = diffuse;

	return _true;
}

_bool VertexAnimation::SetSpecularKeyframe( _float time, _dword specular )
{
	if ( time < 0.0f || time > mDuration )
		return _false;

	if ( ( mKeyframeFormat & Geometry::_FORMATFLAG_SPECULAR ) == 0 )
		return _false;

	// Add animation keyframe.
	VertexKeyframe& newkeyframe = AddKeyframe( time );

	newkeyframe.mSpecular = specular;

	return _true;
}
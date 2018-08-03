//============================================================================
// TagPoint.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// TagPoint
//----------------------------------------------------------------------------

class TagPoint : public ITagPoint, public ReferencedObject
{
private:
	Vector3		mPosition;
	Vector3		mDirection;
	Matrix3		mRotation;

public:
	TagPoint( );

	virtual _void SetPosition( const Vector3& position );
	virtual const Vector3& GetPosition( ) const;
	virtual _void SetDirection( const Vector3& direction );
	virtual const Vector3& GetDirection( ) const;
	virtual _void SetRotation( const Matrix3& rotation );
	virtual const Matrix3& GetRotation( ) const;
};

};
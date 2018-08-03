//============================================================================
// Vector3.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

class Matrix4;

//----------------------------------------------------------------------------
// Vector4
//----------------------------------------------------------------------------

#pragma pack( push, 16 )

//! This class represents a 4D vector.
class Vector4
{
public:
	//! The origin vector.
	const static Vector4 cOrigin;
	//! The identity vector.
	const static Vector4 cIdentity;
	//! The x-axis vector.
	const static Vector4 cXAxis;
	//! The y-axis vector.
	const static Vector4 cYAxis;
	//! The z-axis vector.
	const static Vector4 cZAxis;
	//! The w-axis vector.
	const static Vector4 cWAxis;
	//! The maximal vector.
	const static Vector4 cMaxVector;
	//! The minimal vector.
	const static Vector4 cMinVector;

	//! X-component of the vector.
	_float	x;
	//! Y-component of the vector.
	_float	y;
	//! Z-component of the vector.
	_float	z;
	//! W-component of the vector.
	_float	w;

public:
	//! Constructor, create a vector without sets x, y, z, w components.
	//! @param		none
	inline Vector4( );
	//! Constructor, create a vector and sets x, y, z components.
	//! @param		xx			X-component of the vector.
	//! @param		yy			Y-component of the vector.
	//! @param		zz			Z-component of the vector.
	inline Vector4( _float xx, _float yy, _float zz, _float ww );
	//! Constructor, create a 4D vector from a 3D vector and a w-component.
	//! @param		vector		The 3D vector, used to set x, y, z components.
	//! @param		ww			W-component of the vector.
	inline Vector4( const Vector3& vector, _float ww );
	//! Copy Constructor, create a vector and sets components by a given vector.
	//! @param		m		The vector create from.
	inline Vector4( const Vector4 &v );

	//! Copy components from another vector.
	//! @param		m		The other vector.
	//! @return		The reference of current vector.
	inline Vector4& operator = ( const Vector3 &v );
	//! Copy components from another vector.
	//! @param		m		The other vector.
	//! @return		The reference of current vector.
	inline Vector4& operator = ( const Vector4 &v );

	//! Type conversion, get the 3D vector from x, y, z components.
	//! @param		none
	//! @return		The 3D vector.
	inline operator Vector3 ( ) const;

	//! Get positive vector.
	//! @param		none
	//! @return		The positive vector.
	inline Vector4 operator + ( ) const;
	//! Get negative vector.
	//! @param		none
	//! @return		The negative vector.
	inline Vector4 operator - ( ) const;

	//! Add a vector to current vector, and save result in current one.
	//! @param		vector		The vector to add.
	//! @return		The result vector of addition.
	inline Vector4& operator += ( const Vector4& vector );
	//! Subtract a vector from current vector, and save result in current one.
	//! @param		vector		The vector to subtract.
	//! @return		The result vector of subtraction.
	inline Vector4& operator -= ( const Vector4& vector );
	//! Multiply a scalar to current vector, and save result in current one.
	//! @param		scalar		The scalar to multiply.
	//! @return		The result vector of multiplication.
	inline Vector4& operator *= ( _float scalar );
	//! Multiply a vector to current vector, and save result in current one.
	//! @param		vector		The vector to multiply.
	//! @return		The result vector of multiplication.
	inline Vector4& operator *= ( const Vector4& vector );
	//! Divide a scalar to current vector, and save result in current one.
	//! @param		scalar		The scalar to divide.
	//! @return		The result vector of division.
	inline Vector4& operator /= ( _float scalar );
	//! Divide a vector to current vector, and save result in current one.
	//! @param		vector		The vector to divide.
	//! @return		The result vector of division.
	inline Vector4& operator /= ( const Vector4& vector );

	//! Multiply a matrix to current vector, and save result in current one.
	//! @param		matrix		The matrix to multiply.
	//! @return		The result vector of multiplication.
	Vector4& operator *= ( const Matrix4& matrix );

	//! Add a vector to current vector, return a new vector to save result.
	//! @param		vector		The vector to add.
	//! @return		The result vector of addition.
	inline Vector4 operator + ( const Vector4& vector ) const;
	//! Subtract a vector from current vector, return a new vector to save result.
	//! @param		vector		The vector to subtract.
	//! @return		The result vector of subtraction.
	inline Vector4 operator - ( const Vector4& vector ) const;
	//! Multiply a scalar to current vector, return a new vector to save result.
	//! @param		scalar		The scalar to multiply.
	//! @return		The result vector of multiplication.
	inline Vector4 operator * ( _float scalar ) const;
	//! Multiply a vector to current vector, return a new vector to save result.
	//! @param		vector		The vector to multiply.
	//! @return		The result vector of multiplication.
	inline Vector4 operator * ( const Vector4& vector ) const;
	//! Divide a scalar to current vector, return a new vector to save result.
	//! @param		scalar		The scalar to divide.
	//! @return		The result vector of division.
	inline Vector4 operator / ( _float scalar ) const;
	//! Divide a vector to current vector, return a new vector to save result.
	//! @param		vector		The vector to divide.
	//! @return		The result vector of division.
	inline Vector4 operator / ( const Vector4& vector ) const;

	//! Compare two vectors.
	//! @param		vector		The vector to compare with.
	//! @return		True if they are identical, false otherwise.
	inline _bool operator == ( const Vector4& vector ) const;
	//! Compare two vectors.
	//! @param		vector		The vector to compare with.
	//! @return		True if they are not identical, false otherwise.
	inline _bool operator != ( const Vector4& vector ) const;

	//! Multiply a matrix to current vector, return a new vector to save result.
	//! @param		matrix		The matrix to multiply.
	//! @return		The result vector of multiplication.
	Vector4 operator * ( const Matrix4& matrix ) const;

	//! Compare two vectors with error margin.
	//! @param		vector		The vector to compare with.
	//! @param		margin		The margin that allows some error.
	//! @return		True if they are identical, false otherwise.
	inline _bool Equal( const Vector4& vector, _float margin = Math::cEpsilon ) const;

	//! Make the current vector absolute, that all coordinates are nonnegative.
	//! @param		none
	//! @return		The absolute vector.
	inline Vector4& Absolute( );
	//! Make the current vector normalized, that has one unit magnitude.
	//! @param		none
	//! @return		The normalized vector.
	inline Vector4& Normalize( );

	//! Calculates vector magnitude.
	//! @param		none
	//! @return		The magnitude of vector.
	inline _float Magnitude( ) const;
	//! Calculates the square of vector magnitude, this function is faster than Magnitude.
	//! @param		none
	//! @return		The square of magnitude of vector.
	inline _float MagnitudeSquared( ) const;

	//! Calculates dot product of two vectors.
	//! @param		vector1		Source vector 1.
	//! @param		vector2		Source vector 2.
	//! @return		The dot product result.
	inline static _float Dot( const Vector4& vector1, const Vector4& vector2 );
	//! Calculates vector by project one vector to another.
	//! @param		vector1		The vector to be projected.
	//! @param		vector2		The project reference vector.
	//! @return		The projected vector, that has same direction of vector2.
	static Vector4 Project( const Vector4& vector1, const Vector4& vector2 );
	//! Calculates vector by plumb one vector to another.
	//! @param		vector1		The vector to be projected.
	//! @param		vector2		The project reference vector.
	//! @return		The plumbed vector, that is vertical of vector2.
	static Vector4 Plumb( const Vector4& vector1, const Vector4& vector2 );
	//! Calculates linear interpolation of two vectors.
	//! @param		vector1		Source vector 1.
	//! @param		vector2		Source vector 2.
	//! @param		factor		The interpolation factor, will clamp to between 0 to 1.
	//! @return		The result vector of interpolation, vector1 + ( vector2 - vector1 ) * factor.
	static Vector4 Lerp( const Vector4& vector1, const Vector4& vector2, _float factor );
};

#pragma pack( pop ) 

//----------------------------------------------------------------------------
// Vector4 Implementation
//----------------------------------------------------------------------------

Vector4::Vector4( )
{
}

Vector4::Vector4( _float xx, _float yy, _float zz, _float ww )
	: x( xx ), y( yy ), z( zz ), w( ww )
{
}

Vector4::Vector4( const Vector3& vector, _float ww )
	: x( vector.x ), y( vector.y ), z( vector.z ), w( ww )
{
}

Vector4::Vector4( const Vector4 &v )
{
	_float4* m1 = (_float4*) &x;
	_float4* m2 = (_float4*) &v.x;

	if( ( (_dword) m1 ) % 16 == 0 && ( (_dword) m2 ) % 16 == 0 )
	{
		*m1 = *m2;
	}
	else
	{
		x = v.x; y = v.y; z = v.z, w = v.w;
	}
}

Vector4& Vector4::operator = ( const Vector3 &v )
{
	_float4* m = (_float4*) &x;

	if( ( (_dword) m ) % 16 == 0 )
	{
		*m = _mm_set_ps( 0.0f, v.z, v.y, v.x );
	}
	else
	{
		x = v.x; y = v.y; z = v.z, w = 0.0f;
	}

	return *this;
}

Vector4& Vector4::operator= ( const Vector4 &v )
{
	_float4* m1 = (_float4*) &x;
	_float4* m2 = (_float4*) &v.x;

	if( ( (_dword) m1 ) % 16 == 0 && ( (_dword) m2 ) % 16 == 0 )
	{
		*m1 = *m2;
	}
	else
	{
		x = v.x; y = v.y; z = v.z, w = v.w;
	}

	return *this;
}

Vector4::operator Vector3 ( ) const
{
	return Vector3( x, y, z );
}

Vector4 Vector4::operator + ( ) const
{
	return Vector4( x, y, z, w );
}

Vector4 Vector4::operator - ( ) const
{
	return Vector4( -x, -y, -z, -w );
}

Vector4& Vector4::operator += ( const Vector4& vector )
{
	_float4* m1 = (_float4*) &x;
	_float4* m2 = (_float4*) &vector.x;

	if( ( (_dword) m1 ) % 16 == 0 && ( (_dword) m2 ) % 16 == 0 )
	{
		*m1 = _mm_add_ps( *m1, *m2 );
	}
	else
	{
		x += vector.x;
		y += vector.y;
		z += vector.z;
		w += vector.w;
	}

	return *this;
}

Vector4& Vector4::operator -= ( const Vector4& vector )
{
	_float4* m1 = (_float4*) &x;
	_float4* m2 = (_float4*) &vector.x;

	if( ( (_dword) m1 ) % 16 == 0 && ( (_dword) m2 ) % 16 == 0 )
	{
		*m1 = _mm_sub_ps( *m1, *m2 );
	}
	else
	{
		x -= vector.x;
		y -= vector.y;
		z -= vector.z;
		w -= vector.w;
	}

	return *this;
}

Vector4& Vector4::operator *= ( _float scalar )
{
	_float4* m = (_float4*) &x;

	if( ( (_dword) m ) % 16 == 0 )
	{
		*m = _mm_mul_ps( *m, _mm_set_ps1( scalar ) );
	}
	else
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		w *= scalar;
	}

	return *this;
}

Vector4& Vector4::operator *= ( const Vector4& vector )
{
	_float4* m1 = (_float4*) &x;
	_float4* m2 = (_float4*) &vector.x;

	if( ( (_dword) m1 ) % 16 == 0 && ( (_dword) m2 ) % 16 == 0 )
	{
		*m1 = _mm_mul_ps( *m1, *m2 );
	}
	else
	{
		x *= vector.x;
		y *= vector.y;
		z *= vector.z;
		w *= vector.w;
	}

	return *this;
}

Vector4& Vector4::operator /= ( _float scalar )
{
	_float4* m = (_float4*) &x;

	if( ( (_dword) m ) % 16 == 0  )
	{
		*m = _mm_div_ps( *m, _mm_set_ps1( scalar ) );
	}
	else
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;
		w /= scalar;
	}

	return *this;
}

Vector4& Vector4::operator /= ( const Vector4& vector )
{
	_float4* m1 = (_float4*) &x;
	_float4* m2 = (_float4*) &vector.x;

	if( ( (_dword) m1 ) % 16 == 0 && ( (_dword) m2 ) % 16 == 0 )
	{
		*m1 = _mm_div_ps( *m1, *m2 );
	}
	else
	{
		x /= vector.x;
		y /= vector.y;
		z /= vector.z;
		w /= vector.w;
	}

	return *this;
}

Vector4 Vector4::operator + ( const Vector4& vector ) const
{
	Vector4 result;
	_float4* m1 = (_float4*) &x;
	_float4* m2 = (_float4*) &vector.x;
	
	if( ( (_dword) m1 ) % 16 == 0 && ( (_dword) m2 ) % 16 == 0 )
	{
		_float4* pointer = (_float4*) &result.x;

		if( ( (_dword) pointer ) % 16 == 0 )
		{
			*pointer = _mm_add_ps( *m1, *m2 );
		}
		else
		{
			_float4 temp = _mm_add_ps( *m1, *m2 );
			_float* start = (_float*) &temp;

			result.x = *start;
			result.y = *( start + 1 );
			result.z = *( start + 2 );
			result.w = *( start + 3 );
		}
	}
	else
	{
		result.x = x + vector.x;
		result.y = y + vector.y;
		result.z = z + vector.z;
		result.w = w + vector.w;
	}

	return result;
}

Vector4 Vector4::operator - ( const Vector4& vector ) const
{
	Vector4 result;
	_float4* m1 = (_float4*) &x;
	_float4* m2 = (_float4*) &vector.x;
	
	if( ( (_dword) m1 ) % 16 == 0 && ( (_dword) m2 ) % 16 == 0 )
	{
		_float4* pointer = (_float4*) &result.x;

		if( ( (_dword) pointer ) % 16 == 0 )
		{
			*pointer = _mm_sub_ps( *m1, *m2 );
		}
		else
		{
			_float4 temp = _mm_sub_ps( *m1, *m2 );
			_float* start = (_float*) &temp;

			result.x = *start;
			result.y = *( start + 1 );
			result.z = *( start + 2 );
			result.w = *( start + 3 );
		}
	}
	else
	{
		result.x = x - vector.x;
		result.y = y - vector.y;
		result.z = z - vector.z;
		result.w = w - vector.w;
	}

	return result;
}

Vector4 Vector4::operator * ( _float scalar ) const
{
	Vector4 result;
	_float4* m = (_float4*) &x;
	
	if( ( (_dword) m ) % 16 == 0 )
	{
		_float4 scalar4 = _mm_set_ps1( scalar );
		_float4* pointer = (_float4*) &result.x;

		if( ( (_dword) pointer ) % 16 == 0 )
		{
			*pointer = _mm_mul_ps( *m, scalar4 );
		}
		else
		{
			_float4 temp = _mm_mul_ps( *m, scalar4 );
			_float* start = (_float*) &temp;

			result.x = *start;
			result.y = *( start + 1 );
			result.z = *( start + 2 );
			result.w = *( start + 3 );
		}
	}
	else
	{
		result.x = x * scalar;
		result.y = y * scalar;
		result.z = z * scalar;
		result.w = w * scalar;
	}

	return result;
}

Vector4 Vector4::operator * ( const Vector4& vector ) const
{
	Vector4 result;
	_float4* m1 = (_float4*) &x;
	_float4* m2 = (_float4*) &vector.x;
	
	if( ( (_dword) m1 ) % 16 == 0 && ( (_dword) m2 ) % 16 == 0 )
	{
		_float4* pointer = (_float4*) &result.x;

		if( ( (_dword) pointer ) % 16 == 0 )
		{
			*pointer = _mm_mul_ps( *m1, *m2 );
		}
		else
		{
			_float4 temp = _mm_mul_ps( *m1, *m2 );
			_float* start = (_float*) &temp;

			result.x = *start;
			result.y = *( start + 1 );
			result.z = *( start + 2 );
			result.w = *( start + 3 );
		}
	}
	else
	{
		result.x = x * vector.x;
		result.y = y * vector.y;
		result.z = z * vector.z;
		result.w = w * vector.w;
	}

	return result;
}

Vector4 Vector4::operator / ( _float scalar ) const
{
	Vector4 result;
	_float4* m = (_float4*) &x;
	
	if( ( (_dword) m ) % 16 == 0 )
	{
		_float4 scalar4 = _mm_set_ps1( scalar );
		_float4* pointer = (_float4*) &result.x;

		if( ( (_dword) pointer ) % 16 == 0 )
		{
			*pointer = _mm_div_ps( *m, scalar4 );
		}
		else
		{
			_float4 temp = _mm_div_ps( *m, scalar4 );
			_float* start = (_float*) &temp;

			result.x = *start;
			result.y = *( start + 1 );
			result.z = *( start + 2 );
			result.w = *( start + 3 );
		}
	}
	else
	{
		result.x = x / scalar;
		result.y = y / scalar;
		result.z = z / scalar;
		result.w = w / scalar;
	}

	return result;
}

Vector4 Vector4::operator / ( const Vector4& vector ) const
{
	Vector4 result;
	_float4* m1 = (_float4*) &x;
	_float4* m2 = (_float4*) &vector.x;
	
	if( ( (_dword) m1 ) % 16 == 0 && ( (_dword) m2 ) % 16 == 0 )
	{
		_float4* pointer = (_float4*) &result.x;

		if( ( (_dword) pointer ) % 16 == 0 )
		{
			*pointer = _mm_div_ps( *m1, *m2 );
		}
		else
		{
			_float4 temp = _mm_div_ps( *m1, *m2 );
			_float* start = (_float*) &temp;

			result.x = *start;
			result.y = *( start + 1 );
			result.z = *( start + 2 );
			result.w = *( start + 3 );
		}
	}
	else
	{
		result.x = x / vector.x;
		result.y = y / vector.y;
		result.z = z / vector.z;
		result.w = w / vector.w;
	}

	return result;
}

_bool Vector4::operator == ( const Vector4& vector ) const
{
	return x == vector.x && y == vector.y && z == vector.z && w == vector.w;
}

_bool Vector4::operator != ( const Vector4& vector ) const
{
	return x != vector.x || y != vector.y || z != vector.z || w != vector.w;
}

_bool Vector4::Equal( const Vector4& vector, _float margin ) const
{
	if ( Math::Abs( x - vector.x ) > margin )
		return _false;

	if ( Math::Abs( y - vector.y ) > margin )
		return _false;

	if ( Math::Abs( z - vector.z ) > margin )
		return _false;

	if ( Math::Abs( w - vector.w ) > margin )
		return _false;

	return _true;
}

Vector4& Vector4::Absolute( )
{
	if ( x < 0.0f )
		x = -x;

	if ( y < 0.0f )
		y = -y;

	if ( z < 0.0f )
		z = -z;

	if ( w < 0.0f )
		w = -w;

	return *this;
}

Vector4& Vector4::Normalize( )
{
	_float magnitude = Magnitude( );

	Vector4 result;
	_float4* m = (_float4*) &x;
	
	if( ( (_dword) m ) % 16 == 0 )
	{
		if ( magnitude > 0.0f )
		{
			*m = _mm_div_ps( *m, _mm_set_ps1( magnitude ) );
		}
		else
		{
			*m = _mm_setzero_ps();
			*(_float*)m = 1.0f;
		}
	}
	else
	{
		if ( magnitude > 0.0f )
		{
			x /= magnitude;
			y /= magnitude;
			z /= magnitude;
			w /= magnitude;
		}
		else
		{
			x = 1.0f;
			y = 0.0f;
			z = 0.0f;
			w = 0.0f;
		}
	}

	return *this;
}

_float Vector4::Magnitude( ) const
{
	return Math::Sqrt( Dot( *this, *this ) );
}

_float Vector4::MagnitudeSquared( ) const
{
	return Dot( *this, *this );
}

_float Vector4::Dot( const Vector4& vector1, const Vector4& vector2 )
{
	_float4* m1 = (_float4*) &vector1.x;
	_float4* m2 = (_float4*) &vector2.x;
	
	if( ( (_dword) m1 ) % 16 == 0 && ( (_dword) m2 ) % 16 == 0 )
	{
		_float4 r = _mm_mul_ps( *m1, *m2 );

		r = _mm_add_ps( _mm_movehl_ps( r, r ), r );

		return *(float *)&( _mm_add_ss( _mm_shuffle_ps( r, r, 1 ), r ) );
	}
	else
	{
		return vector1.x * vector2.x + vector1.y * vector2.y + vector1.z * vector2.z + vector1.w * vector2.w;
	}
}

};
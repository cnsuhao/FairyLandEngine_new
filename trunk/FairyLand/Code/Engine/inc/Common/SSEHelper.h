//============================================================================
// SSEHelper.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// SSE macros and helpers
//----------------------------------------------------------------------------

const _float4 _VECTOR4_X_AXIS = _mm_set_ps( 0.0f, 0.0f, 0.0f, 1.0f );
const _float4 _VECTOR4_Y_AXIS = _mm_set_ps( 0.0f, 0.0f, 1.0f, 0.0f );
const _float4 _VECTOR4_Z_AXIS = _mm_set_ps( 0.0f, 1.0f, 0.0f, 0.0f );
const _float4 _VECTOR4_W_AXIS = _mm_set_ps( 1.0f, 0.0f, 0.0f, 0.0f );

#define __MM_RSQRT_PS( x ) _mm_rsqrt_ps( x )

#define __MM_SELECT( v, fp ) _mm_shuffle_ps( ( v ), ( v ), _MM_SHUFFLE( ( fp ), ( fp ), ( fp ), ( fp ) ) )

#define __MM_ACCUM4_PS( a, b, c, d ) _mm_add_ps( _mm_add_ps( a, b ), _mm_add_ps( c, d ) )

#define __MM_DOT4x4_PS( a0, a1, a2, a3, b0, b1, b2, b3 ) \
	__MM_ACCUM4_PS( _mm_mul_ps( a0, b0 ), _mm_mul_ps( a1, b1 ), _mm_mul_ps( a2, b2 ), _mm_mul_ps( a3, b3 ) )

#define __MM_DOT4x3_PS( r0, r1, r2, r3, v0, v1, v2 ) \
	__MM_ACCUM4_PS( _mm_mul_ps( r0, v0 ), _mm_mul_ps( r1, v1 ), _mm_mul_ps( r2, v2 ), r3 )

#define __MM_ACCUM3_PS( a, b, c ) _mm_add_ps( _mm_add_ps( a, b ), c )

#define __MM_DOT3x3_PS( r0, r1, r2, v0, v1, v2 ) \
	__MM_ACCUM3_PS( _mm_mul_ps( r0, v0 ), _mm_mul_ps( r1, v1 ), _mm_mul_ps( r2, v2 ) )

#define __MM_MADD_PS( a, b, c ) _mm_add_ps( _mm_mul_ps( a, b ), c )

#define __MM_LERP_PS( t, a, b ) __MM_MADD_PS( _mm_sub_ps( b, a ), t, a )

#define __MM_MADD_SS( a, b, c ) _mm_add_ss( _mm_mul_ss( a, b ), c )

#define __MM_LERP_SS( t, a, b ) __MM_MADD_SS( _mm_sub_ss( b, a ), t, a )

#define __MM_LOAD_PS( p ) ( *( _float4* )( p ) )

#define __MM_STORE_PS( p, v ) ( *( _float4* )( p ) = ( v ) )


#define __MM_MULTIPLY3x4_PS( u0, u1, u2, v0, v1, v2 )                               \
	{                                                                               \
		_float4 t0, t1, t2, t3;                                                     \
		_float4 zero = _mm_setzero_ps( );                                           \
		                                                                            \
		t0 = _mm_mul_ps( __MM_SELECT( u0, 0 ), v0 );                                \
		t1 = _mm_mul_ps( __MM_SELECT( u0, 1 ), v1 );                                \
		t2 = _mm_mul_ps( __MM_SELECT( u0, 2 ), v2 );                                \
		t3 = _mm_shuffle_ps( zero, u0, _MM_SHUFFLE( 3, 3, 0, 0 ) );                 \
		t3 = _mm_shuffle_ps( zero, t3, _MM_SHUFFLE( 2, 1, 0, 0 ) );                 \
		u0 = __MM_ACCUM4_PS( t0, t1, t2, t3 );                                      \
		                                                                            \
		t0 = _mm_mul_ps( __MM_SELECT( u1, 0 ), v0 );                                \
		t1 = _mm_mul_ps( __MM_SELECT( u1, 1 ), v1 );                                \
		t2 = _mm_mul_ps( __MM_SELECT( u1, 2 ), v2 );                                \
		t3 = _mm_shuffle_ps( zero, u1, _MM_SHUFFLE( 3, 3, 0, 0 ) );                 \
		t3 = _mm_shuffle_ps( zero, t3, _MM_SHUFFLE( 2, 1, 0, 0 ) );                 \
		u1 = __MM_ACCUM4_PS( t0, t1, t2, t3 );                                      \
		                                                                            \
		t0 = _mm_mul_ps( __MM_SELECT( u2, 0 ), v0 );                                \
		t1 = _mm_mul_ps( __MM_SELECT( u2, 1 ), v1 );                                \
		t2 = _mm_mul_ps( __MM_SELECT( u2, 2 ), v2 );                                \
		t3 = _mm_shuffle_ps( zero, u2, _MM_SHUFFLE( 3, 3, 0, 0 ) );                 \
		t3 = _mm_shuffle_ps( zero, t3, _MM_SHUFFLE( 2, 1, 0, 0 ) );                 \
		u2 = __MM_ACCUM4_PS( t0, t1, t2, t3 );                                      \
	}

#define __MM_TRANSPOSE4x4_PS( r0, r1, r2, r3 )                                      \
	{                                                                               \
		_float4 s3, s2, s1, s0;                                                     \
		                                                                            \
		                                                    /* r00 r01 r02 r03 */   \
		                                                    /* r10 r11 r12 r13 */   \
		                                                    /* r20 r21 r22 r23 */   \
		                                                    /* r30 r31 r32 r33 */   \
		                                                                            \
		s0 = _mm_unpacklo_ps( r0, r1 );                     /* r00 r10 r01 r11 */   \
		s2 = _mm_unpackhi_ps( r0, r1 );                     /* r02 r12 r03 r13 */   \
		s1 = _mm_unpacklo_ps( r2, r3 );                     /* r20 r30 r21 r31 */   \
		s3 = _mm_unpackhi_ps( r2, r3 );                     /* r22 r32 r23 r33 */   \
		                                                                            \
		r0 = _mm_movelh_ps( s0, s1 );                       /* r00 r10 r20 r30 */   \
		r1 = _mm_movehl_ps( s1, s0 );                       /* r01 r11 r21 r31 */   \
		r2 = _mm_movelh_ps( s2, s3 );                       /* r02 r12 r22 r32 */   \
		r3 = _mm_movehl_ps( s3, s2 );                       /* r03 r13 r23 r33 */   \
	}

#define __MM_TRANSPOSE4x3_PS( v0, v1, v2 )                                          \
	{                                                                               \
		_float4 t0, t1, t2;                                                         \
		                                                                            \
		                                                    /* r00 r01 r02 r10 */   \
		                                                    /* r11 r12 r20 r21 */   \
		                                                    /* r22 r30 r31 r32 */   \
		                                                                            \
		t0 = _mm_shuffle_ps( v0, v2, _MM_SHUFFLE(3,0,3,0) );/* r00 r10 r22 r32 */   \
		t1 = _mm_shuffle_ps( v0, v1, _MM_SHUFFLE(1,0,2,1) );/* r01 r02 r11 r12 */   \
		t2 = _mm_shuffle_ps( v1, v2, _MM_SHUFFLE(2,1,3,2) );/* r20 r21 r30 r31 */   \
		                                                                            \
		v0 = _mm_shuffle_ps( t0, t2, _MM_SHUFFLE(2,0,1,0) );/* r00 r10 r20 r30 */   \
		v1 = _mm_shuffle_ps( t1, t2, _MM_SHUFFLE(3,1,2,0) );/* r01 r11 r21 r31 */   \
		v2 = _mm_shuffle_ps( t1, t0, _MM_SHUFFLE(3,2,3,1) );/* r02 r12 r22 r32 */   \
	}

#define __MM_TRANSPOSE3x4_PS( v0, v1, v2 )                                          \
	{                                                                               \
		_float4 t0, t1, t2;                                                         \
		                                                                            \
		                                                    /* r00 r10 r20 r30 */   \
		                                                    /* r01 r11 r21 r31 */   \
		                                                    /* r02 r12 r22 r32 */   \
		                                                                            \
		t0 = _mm_shuffle_ps( v0, v2, _MM_SHUFFLE(2,0,3,1) );/* r10 r30 r02 r22 */   \
		t1 = _mm_shuffle_ps( v1, v2, _MM_SHUFFLE(3,1,3,1) );/* r11 r31 r12 r32 */   \
		t2 = _mm_shuffle_ps( v0, v1, _MM_SHUFFLE(2,0,2,0) );/* r00 r20 r01 r21 */   \
		                                                                            \
		v0 = _mm_shuffle_ps( t2, t0, _MM_SHUFFLE(0,2,2,0) );/* r00 r01 r02 r10 */   \
		v1 = _mm_shuffle_ps( t1, t2, _MM_SHUFFLE(3,1,2,0) );/* r11 r12 r20 r21 */   \
		v2 = _mm_shuffle_ps( t0, t1, _MM_SHUFFLE(3,1,1,3) );/* r22 r30 r31 r32 */   \
	}

//----------------------------------------------------------------------------
// SSEMemoryAccessor
//----------------------------------------------------------------------------

template <_bool aligned = _false>
struct SSEMemoryAccessor
{
	static __forceinline _float4 load( const _float *p )
	{
		return _mm_loadu_ps( p );
	}
	
	static __forceinline void store( _float *p, const _float4& v )
	{
		_mm_storeu_ps( p, v );
	}
};

template <>
struct SSEMemoryAccessor<_true>
{
	static __forceinline const _float4& load( const _float *p )
	{
		return __MM_LOAD_PS( p );
	}
	
	static __forceinline void store( _float *p, const _float4& v )
	{
		__MM_STORE_PS( p, v );
	}
};

struct SSEMemory
{
	enum{ ALIGMENT = 16 };

	static _void* Allocate( _dword size )
	{
		_byte* p = new _byte[size + ALIGMENT];
		_dword offset = ALIGMENT - ( _dword( p ) & ( ALIGMENT - 1 ) );

		_byte* result = p + offset;
		result[-1] = (_byte)offset;

		return result;
	}

	static _void Deallocate( _void* p )
	{
		if ( p )
		{
			_byte* mem = ( _byte* ) p;
			mem = mem - mem[-1];
			delete [] mem;
		}
	}

	static _void* AlignmentAddress( _void* p )
	{
		if ( p )
		{
			return ( _void* ) ( ( _byte* )p + ALIGMENT - ( _dword( p ) & ( ALIGMENT - 1 ) ) );

		}

		return p;
	}

	static __forceinline _bool _IsAlignedForSSE( const _void *p )
	{
		return ( ( ( _dword )p ) & 15 ) == 0;
	}
};

template <class T>
static __forceinline T* RawOffsetPointer( T* ptr, ptrdiff_t offset )
{
	return ( T* )( ( _byte* )( ptr ) + offset );
}

template <class T>
static __forceinline _void AdvanceRawPointer( T*& ptr, ptrdiff_t offset )
{
	ptr = RawOffsetPointer( ptr, offset );
}

//---------------------------------------------------------------------
// Some useful macro for collapse matrices.
//---------------------------------------------------------------------

#define __LOAD_MATRIX( row0, row1, row2, matrix )                       \
	{                                                                   \
		row0 = __MM_LOAD_PS( matrix[0] );                               \
		row1 = __MM_LOAD_PS( matrix[1] );                               \
		row2 = __MM_LOAD_PS( matrix[2] );                               \
	}

#define __LOAD_MUL_MATRIX( row0, row1, row2, matrix1, alter, matrix2 )  \
	{                                                                   \
		__LOAD_MATRIX( row0, row1, row2, matrix1 );                     \
		if( alter == _true )                                            \
		{                                                               \
			_float4 a0, a1, a2;                                         \
			__LOAD_MATRIX( a0, a1, a2, matrix2 );                       \
			__MM_MULTIPLY3x4_PS( row0, row1, row2, a0, a1, a2 );        \
		}                                                               \
	}

#define __LERP_MATRIX( row0, row1, row2, weight, matrix )               \
	{                                                                   \
		row0 = __MM_LERP_PS( weight, row0, __MM_LOAD_PS( matrix[0] ) ); \
		row1 = __MM_LERP_PS( weight, row1, __MM_LOAD_PS( matrix[1] ) ); \
		row2 = __MM_LERP_PS( weight, row2, __MM_LOAD_PS( matrix[2] ) ); \
	}

#define __LERP_MUL_MATRIX( row0, row1, row2, matrix1, alter, matrix2 )  \
	{                                                                   \
		_float4 b0, b1, b2;                                             \
		__LOAD_MUL_MATRIX( b0, b1, b2, matrix1, alter, matrix2 );       \
		row0 = __MM_LERP_PS( weight, row0, b0 );                        \
		row1 = __MM_LERP_PS( weight, row1, b1 );                        \
		row2 = __MM_LERP_PS( weight, row2, b2 );                        \
	}

#define __LOAD_WEIGHTED_MATRIX( row0, row1, row2, weight, matrix )      \
	{                                                                   \
		row0 = _mm_mul_ps( __MM_LOAD_PS( matrix[0] ), weight );         \
		row1 = _mm_mul_ps( __MM_LOAD_PS( matrix[1] ), weight );         \
		row2 = _mm_mul_ps( __MM_LOAD_PS( matrix[2] ), weight );         \
	}

#define __LOAD_WEIGHTED_MUL_MATRIX( row0, row1, row2, weight,           \
		matrix1, alter, matrix2 )                                       \
	{                                                                   \
		__LOAD_WEIGHTED_MATRIX( row0, row1, row2, weight, matrix1 );    \
		if( alter == _true )                                            \
        {                                                               \
			_float4 c0, c1, c2;                                         \
			__LOAD_MATRIX( c0, c1, c2, matrix2 );                       \
			__MM_MULTIPLY3x4_PS( row0, row1, row2, c0, c1, c2 );        \
		}                                                               \
	}

#define __ACCUM_WEIGHTED_MATRIX( row0, row1, row2, weight, matrix )     \
	{                                                                   \
		row0 = __MM_MADD_PS( __MM_LOAD_PS( matrix[0] ), weight, row0 ); \
		row1 = __MM_MADD_PS( __MM_LOAD_PS( matrix[1] ), weight, row1 ); \
		row2 = __MM_MADD_PS( __MM_LOAD_PS( matrix[2] ), weight, row2 ); \
	}

#define __ACCUM_WEIGHTED_MUL_MATRIX( row0, row1, row2, weight,          \
		matrix1, alter, matrix2 )                                       \
	{                                                                   \
		_float4 d0, d1, d2;                                             \
                                                                        \
		__LOAD_WEIGHTED_MUL_MATRIX( d0, d1, d2, weight,                 \
			matrix1, alter, matrix2 );                                  \
		row0 = _mm_add_ps( row0, d0 );                                  \
		row1 = _mm_add_ps( row1, d1 );                                  \
		row2 = _mm_add_ps( row2, d2 );                                  \
	}

//---------------------------------------------------------------------
// The following macros request variables declared by caller.
//---------------------------------------------------------------------

#define __COLLAPSE_MATRIX_W1( row0, row1, row2, pIndices, pWeights,                    \
		pMatrices, pNeedAlters, pAlterMatrices )                                       \
	{                                                                                  \
		__LOAD_MUL_MATRIX( row0, row1, row2, pMatrices[pIndices[0]],                   \
				pNeedAlters[pIndices[0]], pAlterMatrices[pIndices[0]] );               \
	}

#define __COLLAPSE_MATRIX_W2( row0, row1, row2, pIndices, pWeights,                    \
		pMatrices, pNeedAlters, pAlterMatrices )                                       \
	{                                                                                  \
		weight = _mm_load_ps1( pWeights + 1 );                                         \
		__LOAD_MUL_MATRIX( row0, row1, row2, pMatrices[ pIndices[0] ],                 \
				pNeedAlters[ pIndices[0] ], pAlterMatrices[ pIndices[0] ] );           \
		__LERP_MUL_MATRIX( row0, row1, row2, pMatrices[ pIndices[1]] ,                 \
				pNeedAlters[ pIndices[1] ], pAlterMatrices[ pIndices[1] ] );           \
	}

#define __COLLAPSE_MATRIX_W3( row0, row1, row2, pIndices, pWeights,                    \
		pMatrices, pNeedAlters, pAlterMatrices )                                       \
	{                                                                                  \
		weight = _mm_load_ps1( pWeights + 0 );                                         \
		__LOAD_WEIGHTED_MUL_MATRIX( row0, row1, row2, weight, pMatrices[pIndices[0]],  \
				pNeedAlters[pIndices[0]], pAlterMatrices[pIndices[0]] );               \
		weight = _mm_load_ps1( pWeights + 1 );                                         \
		__ACCUM_WEIGHTED_MUL_MATRIX( row0, row1, row2, weight, pMatrices[pIndices[1]], \
				pNeedAlters[pIndices[1]], pAlterMatrices[pIndices[1]] );               \
		weight = _mm_load_ps1( pWeights + 2 );                                         \
		__ACCUM_WEIGHTED_MUL_MATRIX( row0, row1, row2, weight, pMatrices[pIndices[2]], \
				pNeedAlters[pIndices[2]], pAlterMatrices[pIndices[2]] );               \
	}

#define __COLLAPSE_MATRIX_W4( row0, row1, row2, pIndices, pWeights,                    \
		pMatrices, pNeedAlters, pAlterMatrices )                                       \
	{                                                                                  \
		weights = _mm_loadu_ps( pWeights );                                            \
		                                                                               \
		weight = __MM_SELECT( weights, 0 );                                            \
		__LOAD_WEIGHTED_MUL_MATRIX( row0, row1, row2, weight, pMatrices[pIndices[0]],  \
				pNeedAlters[pIndices[0]], pAlterMatrices[pIndices[0]] );               \
		weight = __MM_SELECT( weights, 1 );                                            \
		__ACCUM_WEIGHTED_MUL_MATRIX( row0, row1, row2, weight, pMatrices[pIndices[1]], \
				pNeedAlters[pIndices[1]], pAlterMatrices[pIndices[1]] );               \
		weight = __MM_SELECT( weights, 2 );                                            \
		__ACCUM_WEIGHTED_MUL_MATRIX( row0, row1, row2, weight, pMatrices[pIndices[2]], \
				pNeedAlters[pIndices[2]], pAlterMatrices[pIndices[2]] );               \
		weight = __MM_SELECT( weights, 3 );                                            \
		__ACCUM_WEIGHTED_MUL_MATRIX( row0, row1, row2, weight, pMatrices[pIndices[3]], \
				pNeedAlters[pIndices[3]], pAlterMatrices[pIndices[3]] );               \
	}

#define __COLLAPSE_MATRIX_W5( row0, row1, row2, pIndices, pWeights,                    \
		pMatrices, pNeedAlters, pAlterMatrices )                                       \
	{                                                                                  \
		__COLLAPSE_MATRIX_W4( row0, row1, row2, pIndices, pWeights,                    \
				pMatrices, pNeedAlters, pAlterMatrices );                              \
		weight = _mm_load_ps1( pWeights + 4 );                                         \
		__ACCUM_WEIGHTED_MUL_MATRIX( row0, row1, row2, weight, pMatrices[pIndices[4]], \
				pNeedAlters[pIndices[4]], pAlterMatrices[pIndices[4]] );               \
	}

#define __COLLAPSE_MATRIX_W6( row0, row1, row2, pIndices, pWeights,                    \
		pMatrices, pNeedAlters, pAlterMatrices )                                       \
	{                                                                                  \
		__COLLAPSE_MATRIX_W5( row0, row1, row2, pIndices, pWeights,                    \
				pMatrices, pNeedAlters, pAlterMatrices );                              \
		weight = _mm_load_ps1( pWeights + 5) ;                                         \
		__ACCUM_WEIGHTED_MUL_MATRIX( row0, row1, row2, weight, pMatrices[pIndices[5]], \
				pNeedAlters[pIndices[5]], pAlterMatrices[pIndices[5]] );               \
	}

#define __COLLAPSE_MATRIX_W7( row0, row1, row2, pIndices, pWeights,                    \
		pMatrices, pNeedAlters, pAlterMatrices )                                       \
	{                                                                                  \
		__COLLAPSE_MATRIX_W6( row0, row1, row2, pIndices, pWeights,                    \
				pMatrices, pNeedAlters, pAlterMatrices );                              \
		weight = _mm_load_ps1( pWeights + 6 );                                         \
		__ACCUM_WEIGHTED_MUL_MATRIX( row0, row1, row2, weight, pMatrices[pIndices[6]], \
				pNeedAlters[pIndices[6]], pAlterMatrices[pIndices[6]] );               \
	}

#define __COLLAPSE_MATRIX_W8( row0, row1, row2, pIndices, pWeights,                    \
		pMatrices, pNeedAlters, pAlterMatrices )                                       \
	{                                                                                  \
		__COLLAPSE_MATRIX_W4( row0, row1, row2, pIndices, pWeights,                    \
				pMatrices, pNeedAlters, pAlterMatrices );                              \
		                                                                               \
		weights = _mm_loadu_ps( pWeights + 4 );                                        \
		                                                                               \
		weight = __MM_SELECT( weights, 0 );                                            \
		__ACCUM_WEIGHTED_MUL_MATRIX( row0, row1, row2, weight, pMatrices[pIndices[4]], \
				pNeedAlters[pIndices[4]], pAlterMatrices[pIndices[4]] );               \
		weight = __MM_SELECT( weights, 1 );                                            \
		__ACCUM_WEIGHTED_MUL_MATRIX( row0, row1, row2, weight, pMatrices[pIndices[5]], \
				pNeedAlters[pIndices[5]], pAlterMatrices[pIndices[5]] );               \
		weight = __MM_SELECT( weights, 2 );                                            \
		__ACCUM_WEIGHTED_MUL_MATRIX( row0, row1, row2, weight, pMatrices[pIndices[6]], \
				pNeedAlters[pIndices[6]], pAlterMatrices[pIndices[6]] );               \
		weight = __MM_SELECT(weights, 3);                                              \
		__ACCUM_WEIGHTED_MUL_MATRIX( row0, row1, row2, weight, pMatrices[pIndices[7]], \
				pNeedAlters[pIndices[7]], pAlterMatrices[pIndices[6]] );               \
	}

#define __CollapseOneMatrix(                                                        \
		m00, m01, m02,                                                              \
		pBlendWeight, pBlendIndex,                                                  \
		blendWeightStride, blendIndexStride,                                        \
		blendMatrices, needAlter, alterMatrices,                                    \
		numWeightsPerVertex, bonenumber )                                           \
	{                                                                               \
		_bool isBoneIDValid = _true;                                                \
		                                                                            \
		for ( _dword w = 0; w < *numWeightsPerVertex; ++w )                         \
		{                                                                           \
			if ( pBlendIndex[w] >= bonenumber )                                     \
			{                                                                       \
				isBoneIDValid = _false;                                             \
				break;                                                              \
			}                                                                       \
		}                                                                           \
		                                                                            \
		if ( !isBoneIDValid )                                                       \
		{                                                                           \
			m00 = _VECTOR4_X_AXIS;                                                  \
			m01 = _VECTOR4_Y_AXIS;                                                  \
			m02 = _VECTOR4_Z_AXIS;                                                  \
		}                                                                           \
		else                                                                        \
		{                                                                           \
			_float4 weight, weights;                                                \
			switch ( *numWeightsPerVertex )                                         \
			{                                                                       \
			default:                                                                \
			case 1:                                                                 \
				__COLLAPSE_MATRIX_W1( m00, m01, m02,                                \
					RawOffsetPointer( pBlendIndex, 0 * blendIndexStride ),          \
					RawOffsetPointer( pBlendWeight, 0 * blendWeightStride ),        \
					blendMatrices, needAlter, alterMatrices );                      \
				break;                                                              \
																					\
			case 2:                                                                 \
				__COLLAPSE_MATRIX_W2( m00, m01, m02,                                \
					RawOffsetPointer( pBlendIndex, 0 * blendIndexStride ),          \
					RawOffsetPointer( pBlendWeight, 0 * blendWeightStride ),        \
					blendMatrices, needAlter, alterMatrices );                      \
				break;                                                              \
																					\
			case 3:                                                                 \
				__COLLAPSE_MATRIX_W3( m00, m01, m02,                                \
					RawOffsetPointer( pBlendIndex, 0 * blendIndexStride ),          \
					RawOffsetPointer( pBlendWeight, 0 * blendWeightStride ),        \
					blendMatrices, needAlter, alterMatrices );                      \
				break;                                                              \
																					\
			case 4:                                                                 \
				__COLLAPSE_MATRIX_W4( m00, m01, m02,                                \
					RawOffsetPointer( pBlendIndex, 0 * blendIndexStride) ,          \
					RawOffsetPointer( pBlendWeight, 0 * blendWeightStride ),        \
					blendMatrices, needAlter, alterMatrices );                      \
				break;                                                              \
																					\
			case 5:                                                                 \
				__COLLAPSE_MATRIX_W5( m00, m01, m02,                                \
					RawOffsetPointer( pBlendIndex, 0 * blendIndexStride ),          \
					RawOffsetPointer( pBlendWeight, 0 * blendWeightStride ),        \
					blendMatrices, needAlter, alterMatrices );                      \
				break;                                                              \
																					\
			case 6:                                                                 \
				__COLLAPSE_MATRIX_W6( m00, m01, m02,                                \
					RawOffsetPointer( pBlendIndex, 0 * blendIndexStride ),          \
					RawOffsetPointer( pBlendWeight, 0 * blendWeightStride ),        \
					blendMatrices, needAlter, alterMatrices );                      \
				break;                                                              \
																					\
			case 7:                                                                 \
				__COLLAPSE_MATRIX_W7( m00, m01, m02,                                \
					RawOffsetPointer( pBlendIndex, 0 * blendIndexStride ),          \
					RawOffsetPointer( pBlendWeight, 0 * blendWeightStride ),        \
					blendMatrices, needAlter, alterMatrices );                      \
				break;                                                              \
																					\
			case 8:                                                                 \
				__COLLAPSE_MATRIX_W8( m00, m01, m02,                                \
					RawOffsetPointer( pBlendIndex, 0 * blendIndexStride ),          \
					RawOffsetPointer( pBlendWeight, 0 * blendWeightStride ),        \
					blendMatrices, needAlter, alterMatrices );                      \
				break;                                                              \
			}                                                                       \
		}                                                                           \
	}

#define __CollapseFourMatrices(                                                     \
		m00, m01, m02,                                                              \
		m10, m11, m12,                                                              \
		m20, m21, m22,                                                              \
		m30, m31, m32,                                                              \
		pBlendWeight, pBlendIndex,                                                  \
		blendWeightStride, blendIndexStride,                                        \
		blendMatrices, needAlter, alterMatrices,                                    \
		numWeightsPerVertex, bonenumber )                                           \
	{                                                                               \
		_float4 weight, weights;                                                    \
		                                                                            \
		_bool isBoneIDValid = _true;                                                \
		                                                                            \
		for ( _dword w = 0; w < numWeightsPerVertex[0]; ++w )                       \
		{                                                                           \
			if ( pBlendIndex[w] >= bonenumber )                                     \
			{                                                                       \
				isBoneIDValid = _false;                                             \
				break;                                                              \
			}                                                                       \
		}                                                                           \
		                                                                            \
		if ( !isBoneIDValid )                                                       \
		{                                                                           \
			m00 = _VECTOR4_X_AXIS;                                                  \
			m01 = _VECTOR4_Y_AXIS;                                                  \
			m02 = _VECTOR4_Z_AXIS;                                                  \
		}                                                                           \
		else                                                                        \
		{                                                                           \
			switch ( numWeightsPerVertex[0] )                                       \
			{                                                                       \
			default:                                                                \
			case 1:                                                                 \
				__COLLAPSE_MATRIX_W1( m00, m01, m02,                                \
					RawOffsetPointer( pBlendIndex, 0 * blendIndexStride ),          \
					RawOffsetPointer( pBlendWeight, 0 * blendWeightStride ),        \
					blendMatrices, needAlter, alterMatrices );                      \
				break;                                                              \
																					\
			case 2:                                                                 \
				__COLLAPSE_MATRIX_W2( m00, m01, m02,                                \
					RawOffsetPointer( pBlendIndex, 0 * blendIndexStride ),          \
					RawOffsetPointer( pBlendWeight, 0 * blendWeightStride ),        \
					blendMatrices, needAlter, alterMatrices );                      \
				break;                                                              \
																					\
			case 3:                                                                 \
				__COLLAPSE_MATRIX_W3( m00, m01, m02,                                \
					RawOffsetPointer( pBlendIndex, 0 * blendIndexStride ),          \
					RawOffsetPointer( pBlendWeight, 0 * blendWeightStride ),        \
					blendMatrices, needAlter, alterMatrices );                      \
				break;                                                              \
																					\
			case 4:                                                                 \
				__COLLAPSE_MATRIX_W4( m00, m01, m02,                                \
					RawOffsetPointer( pBlendIndex, 0 * blendIndexStride ),          \
					RawOffsetPointer( pBlendWeight, 0 * blendWeightStride ),        \
					blendMatrices, needAlter, alterMatrices );                      \
				break;                                                              \
																					\
			case 5:                                                                 \
				__COLLAPSE_MATRIX_W5( m00, m01, m02,                                \
					RawOffsetPointer( pBlendIndex, 0 * blendIndexStride ),          \
					RawOffsetPointer( pBlendWeight, 0 * blendWeightStride ),        \
					blendMatrices, needAlter, alterMatrices );                      \
				break;                                                              \
																					\
			case 6:                                                                 \
				__COLLAPSE_MATRIX_W6( m00, m01, m02,                                \
					RawOffsetPointer( pBlendIndex, 0 * blendIndexStride ),          \
					RawOffsetPointer( pBlendWeight, 0 * blendWeightStride ),        \
					blendMatrices, needAlter, alterMatrices );                      \
				break;                                                              \
																					\
			case 7:                                                                 \
				__COLLAPSE_MATRIX_W7( m00, m01, m02,                                \
					RawOffsetPointer( pBlendIndex, 0 * blendIndexStride ),          \
					RawOffsetPointer( pBlendWeight, 0 * blendWeightStride ),        \
					blendMatrices, needAlter, alterMatrices );                      \
				break;                                                              \
																					\
			case 8:                                                                 \
				__COLLAPSE_MATRIX_W8( m00, m01, m02,                                \
					RawOffsetPointer( pBlendIndex, 0 * blendIndexStride ),          \
					RawOffsetPointer( pBlendWeight, 0 * blendWeightStride ),        \
					blendMatrices, needAlter, alterMatrices );                      \
				break;                                                              \
			}                                                                       \
		}                                                                           \
                                                                                    \
		isBoneIDValid = _true;                                                      \
		const _byte* pBoneID = RawOffsetPointer( pBlendIndex, blendIndexStride );   \
		                                                                            \
		for ( _dword w = 0; w < numWeightsPerVertex[1]; ++w )                       \
		{                                                                           \
			if ( pBoneID[w] >= bonenumber )                                         \
			{                                                                       \
				isBoneIDValid = _false;                                             \
				break;                                                              \
			}                                                                       \
		}                                                                           \
		                                                                            \
		if ( !isBoneIDValid )                                                       \
		{                                                                           \
			m10 = _VECTOR4_X_AXIS;                                                  \
			m11 = _VECTOR4_Y_AXIS;                                                  \
			m12 = _VECTOR4_Z_AXIS;                                                  \
		}                                                                           \
		else                                                                        \
		{                                                                           \
			switch ( numWeightsPerVertex[1] )                                       \
			{                                                                       \
			default:                                                                \
			case 1:                                                                 \
				__COLLAPSE_MATRIX_W1( m10, m11, m12,                                \
					RawOffsetPointer( pBlendIndex, 1 * blendIndexStride ),          \
					RawOffsetPointer( pBlendWeight, 1 * blendWeightStride ),        \
					blendMatrices, needAlter, alterMatrices );                      \
				break;                                                              \
																					\
			case 2:                                                                 \
				__COLLAPSE_MATRIX_W2( m10, m11, m12,                                \
					RawOffsetPointer( pBlendIndex, 1 * blendIndexStride ),          \
					RawOffsetPointer( pBlendWeight, 1 * blendWeightStride ),        \
					blendMatrices, needAlter, alterMatrices );                      \
				break;                                                              \
																					\
			case 3:                                                                 \
				__COLLAPSE_MATRIX_W3( m10, m11, m12,                                \
					RawOffsetPointer( pBlendIndex, 1 * blendIndexStride ),          \
					RawOffsetPointer( pBlendWeight, 1 * blendWeightStride ),        \
					blendMatrices, needAlter, alterMatrices );                      \
				break;                                                              \
																					\
			case 4:                                                                 \
				__COLLAPSE_MATRIX_W4( m10, m11, m12,                                \
					RawOffsetPointer( pBlendIndex, 1 * blendIndexStride ),          \
					RawOffsetPointer( pBlendWeight, 1 * blendWeightStride ),        \
					blendMatrices, needAlter, alterMatrices );                      \
				break;                                                              \
																					\
			case 5:                                                                 \
				__COLLAPSE_MATRIX_W5( m10, m11, m12,                                \
					RawOffsetPointer( pBlendIndex, 1 * blendIndexStride ),          \
					RawOffsetPointer( pBlendWeight, 1 * blendWeightStride ),        \
					blendMatrices, needAlter, alterMatrices );                      \
				break;                                                              \
																					\
			case 6:                                                                 \
				__COLLAPSE_MATRIX_W6( m10, m11, m12,                                \
					RawOffsetPointer( pBlendIndex, 1 * blendIndexStride ),          \
					RawOffsetPointer( pBlendWeight, 1 * blendWeightStride) ,        \
					blendMatrices, needAlter, alterMatrices );                      \
				break;                                                              \
																					\
			case 7:                                                                 \
				__COLLAPSE_MATRIX_W7( m10, m11, m12,                                \
					RawOffsetPointer( pBlendIndex, 1 * blendIndexStride ),          \
					RawOffsetPointer( pBlendWeight, 1 * blendWeightStride ),        \
					blendMatrices, needAlter, alterMatrices );                      \
				break;                                                              \
																					\
			case 8:                                                                 \
				__COLLAPSE_MATRIX_W8( m10, m11, m12,                                \
					RawOffsetPointer( pBlendIndex, 1 * blendIndexStride ),          \
					RawOffsetPointer( pBlendWeight, 1 * blendWeightStride ),        \
					blendMatrices, needAlter, alterMatrices );                      \
				break;                                                              \
			}                                                                       \
		}                                                                           \
		                                                                            \
		isBoneIDValid = _true;                                                      \
		pBoneID = RawOffsetPointer( pBlendIndex, 2 * blendIndexStride );            \
		                                                                            \
		for ( _dword w = 0; w < numWeightsPerVertex[2]; ++w )                       \
		{                                                                           \
			if ( pBoneID[w] >= bonenumber )                                         \
			{                                                                       \
				isBoneIDValid = _false;                                             \
				break;                                                              \
			}                                                                       \
		}                                                                           \
		                                                                            \
		if ( !isBoneIDValid )                                                       \
		{                                                                           \
			m20 = _VECTOR4_X_AXIS;                                                  \
			m21 = _VECTOR4_Y_AXIS;                                                  \
			m22 = _VECTOR4_Z_AXIS;                                                  \
		}                                                                           \
		else                                                                        \
		{                                                                           \
			switch ( numWeightsPerVertex[2] )                                       \
			{                                                                       \
			default:                                                                \
			case 1:                                                                 \
				__COLLAPSE_MATRIX_W1( m20, m21, m22,                                \
					RawOffsetPointer( pBlendIndex, 2 * blendIndexStride ),          \
					RawOffsetPointer( pBlendWeight, 2 * blendWeightStride ),        \
					blendMatrices, needAlter, alterMatrices );                      \
				break;                                                              \
																					\
			case 2:                                                                 \
				__COLLAPSE_MATRIX_W2( m20, m21, m22,                                \
					RawOffsetPointer( pBlendIndex, 2 * blendIndexStride ),          \
					RawOffsetPointer( pBlendWeight, 2 * blendWeightStride ),        \
					blendMatrices, needAlter, alterMatrices );                      \
				break;                                                              \
																					\
			case 3:                                                                 \
				__COLLAPSE_MATRIX_W3( m20, m21, m22,                                \
					RawOffsetPointer( pBlendIndex, 2 * blendIndexStride ),          \
					RawOffsetPointer( pBlendWeight, 2 * blendWeightStride ),        \
					blendMatrices, needAlter, alterMatrices );                      \
				break;                                                              \
																					\
			case 4:                                                                 \
				__COLLAPSE_MATRIX_W4( m20, m21, m22,                                \
					RawOffsetPointer( pBlendIndex, 2 * blendIndexStride ),          \
					RawOffsetPointer( pBlendWeight, 2 * blendWeightStride ),        \
					blendMatrices, needAlter, alterMatrices );                      \
				break;                                                              \
																					\
			case 5:                                                                 \
				__COLLAPSE_MATRIX_W5( m20, m21, m22,                                \
					RawOffsetPointer( pBlendIndex, 2 * blendIndexStride ),          \
					RawOffsetPointer( pBlendWeight, 2 * blendWeightStride ),        \
					blendMatrices, needAlter, alterMatrices );                      \
				break;                                                              \
																					\
			case 6:                                                                 \
				__COLLAPSE_MATRIX_W6( m20, m21, m22,                                \
					RawOffsetPointer( pBlendIndex, 2 * blendIndexStride ),          \
					RawOffsetPointer( pBlendWeight, 2 * blendWeightStride ),        \
					blendMatrices, needAlter, alterMatrices );                      \
				break;                                                              \
																					\
			case 7:                                                                 \
				__COLLAPSE_MATRIX_W7( m20, m21, m22,                                \
					RawOffsetPointer( pBlendIndex, 2 * blendIndexStride ),          \
					RawOffsetPointer( pBlendWeight, 2 * blendWeightStride ),        \
					blendMatrices, needAlter, alterMatrices );                      \
				break;                                                              \
																					\
			case 8:                                                                 \
				__COLLAPSE_MATRIX_W8( m20, m21, m22,                                \
					RawOffsetPointer( pBlendIndex, 2 * blendIndexStride ),          \
					RawOffsetPointer( pBlendWeight, 2 * blendWeightStride ),        \
					blendMatrices, needAlter, alterMatrices );                      \
				break;                                                              \
			}                                                                       \
		}                                                                           \
                                                                                    \
		isBoneIDValid = _true;                                                      \
		pBoneID = RawOffsetPointer( pBlendIndex, 3 * blendIndexStride );            \
		                                                                            \
		for ( _dword w = 0; w < numWeightsPerVertex[3]; ++w )                       \
		{                                                                           \
			if ( pBoneID[w] >= bonenumber )                                         \
			{                                                                       \
				isBoneIDValid = _false;                                             \
				break;                                                              \
			}                                                                       \
		}                                                                           \
		                                                                            \
		if ( !isBoneIDValid )                                                       \
		{                                                                           \
			m30 = _VECTOR4_X_AXIS;                                                  \
			m31 = _VECTOR4_Y_AXIS;                                                  \
			m32 = _VECTOR4_Z_AXIS;                                                  \
		}                                                                           \
		else                                                                        \
		{                                                                           \
			switch ( numWeightsPerVertex[3] )                                       \
			{                                                                       \
			default:                                                                \
			case 1:                                                                 \
				__COLLAPSE_MATRIX_W1( m30, m31, m32,                                \
					RawOffsetPointer( pBlendIndex, 3 * blendIndexStride ),          \
					RawOffsetPointer( pBlendWeight, 3 * blendWeightStride ),        \
					blendMatrices, needAlter, alterMatrices );                      \
				break;                                                              \
																					\
			case 2:                                                                 \
				__COLLAPSE_MATRIX_W2( m30, m31, m32,                                \
					RawOffsetPointer( pBlendIndex, 3 * blendIndexStride ),          \
					RawOffsetPointer( pBlendWeight, 3 * blendWeightStride ),        \
					blendMatrices, needAlter, alterMatrices );                      \
				break;                                                              \
																					\
			case 3:                                                                 \
				__COLLAPSE_MATRIX_W3( m30, m31, m32,                                \
					RawOffsetPointer( pBlendIndex, 3 * blendIndexStride ),          \
					RawOffsetPointer( pBlendWeight, 3 * blendWeightStride ),        \
					blendMatrices, needAlter, alterMatrices );                      \
				break;                                                              \
																					\
			case 4:                                                                 \
				__COLLAPSE_MATRIX_W4( m30, m31, m32,                                \
					RawOffsetPointer( pBlendIndex, 3 * blendIndexStride ),          \
					RawOffsetPointer( pBlendWeight, 3 * blendWeightStride ),        \
					blendMatrices, needAlter, alterMatrices );                      \
				break;                                                              \
																					\
			case 5:                                                                 \
				__COLLAPSE_MATRIX_W5( m30, m31, m32,                                \
					RawOffsetPointer( pBlendIndex, 3 * blendIndexStride ),          \
					RawOffsetPointer( pBlendWeight, 3 * blendWeightStride ),        \
					blendMatrices, needAlter, alterMatrices );                      \
				break;                                                              \
																					\
			case 6:                                                                 \
				__COLLAPSE_MATRIX_W6( m30, m31, m32,                                \
					RawOffsetPointer( pBlendIndex, 3 * blendIndexStride ),          \
					RawOffsetPointer( pBlendWeight, 3 * blendWeightStride ),        \
					blendMatrices, needAlter, alterMatrices );                      \
				break;                                                              \
																					\
			case 7:                                                                 \
				__COLLAPSE_MATRIX_W7( m30, m31, m32,                                \
					RawOffsetPointer( pBlendIndex, 3 * blendIndexStride ),          \
					RawOffsetPointer( pBlendWeight, 3 * blendWeightStride ),        \
					blendMatrices, needAlter, alterMatrices );                      \
				break;                                                              \
																					\
			case 8:                                                                 \
				__COLLAPSE_MATRIX_W8( m30, m31, m32,                                \
					RawOffsetPointer( pBlendIndex, 3 * blendIndexStride ),          \
					RawOffsetPointer( pBlendWeight, 3 * blendWeightStride ),        \
					blendMatrices, needAlter, alterMatrices );                      \
				break;                                                              \
			}                                                                       \
		}                                                                           \
	}

};

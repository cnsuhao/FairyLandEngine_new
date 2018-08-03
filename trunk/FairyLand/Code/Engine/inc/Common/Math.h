//============================================================================
// Math.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// Math
//----------------------------------------------------------------------------

//! This class provides many common mathematics functions.
class Math
{
public:
	//! 0.0f
	const static _float cZero;
	//! 1.0f
	const static _float cOne;
	//! The ratio of a circle's circumference to its diameter.
	const static _float cPi;
	//! Double of the ratio of a circle's circumference to its diameter.
	const static _float c2Pi;
	//! The base of the natural system of logarithms (e).
	const static _float cE;
	//! The maximum float value.
	const static _float cMaxFloat;
	//! The minimum float value.
	const static _float cMinFloat;
	//! The epsilon value for float value comparison.
	const static _float cEpsilon;

	//! Calculates the absolute value.
	//! @param		value		Integer value.
	//! @return		The absolute value of argument.
	inline static _long Abs( _long value );
	//! Calculates the absolute value.
	//! @param		value		Floating-point value.
	//! @return		The absolute value of argument.
	inline static _float Abs( _float value );
	//! Compares two arguments and returns the larger one.
	//! @param		value1		Integer value 1.
	//! @param		value2		Integer value 2.
	//! @return		The larger one.
	inline static _long Max( _long value1, _long value2 );
	//! Compares two arguments and returns the larger one.
	//! @param		value1		Integer value 1.
	//! @param		value2		Integer value 2.
	//! @return		The larger one.
	inline static _dword Max( _dword value1, _dword value2 );
	//! Compares two arguments and returns the larger one.
	//! @param		value1		Floating-point value 1.
	//! @param		value2		Floating-point value 2.
	//! @return		The larger one.
	inline static _float Max( _float value1, _float value2 );
	//! Compares two arguments and returns the smaller one.
	//! @param		value1		Integer value 1.
	//! @param		value2		Integer value 2.
	//! @return		The smaller one.
	inline static _long Min( _long value1, _long value2 );
	//! Compares two arguments and returns the smaller one.
	//! @param		value1		Integer value 1.
	//! @param		value2		Integer value 2.
	//! @return		The smaller one.
	inline static _dword Min( _dword value1, _dword value2 );
	//! Compares two arguments and returns the smaller one.
	//! @param		value1		Floating-point value 1.
	//! @param		value2		Floating-point value 2.
	//! @return		The smaller one.
	inline static _float Min( _float value1, _float value2 );
	//! Swap two arguments.
	//! @param		value1		Integer value 1.
	//! @param		value2		Integer value 2.
	//! @return		none.
	inline static _void Swap( _long& value1, _long& value2 );
	//! Swap two arguments.
	//! @param		value1		Integer value 1.
	//! @param		value2		Integer value 2.
	//! @return		none.
	inline static _void Swap( _dword& value1, _dword& value2 );
	//! Swap two arguments.
	//! @param		value1		Floating-point value 1.
	//! @param		value2		Floating-point value 2.
	//! @return		none.
	inline static _void Swap( _float& value1, _float& value2 );

	//! Clamp value into the range specified by min and max.
	//! @param		value		Integer value to be clamped.
	//! @param		min			Minimal value of the range.
	//! @param		max			Maximal value of the range.
	//! @return		The clamped value.
	inline static _long Clamp( _long value, _long min, _long max );
	//! Clamp value into the range specified by min and max.
	//! @param		value		Floating-point value to be clamped.
	//! @param		min			Minimal value of the range.
	//! @param		max			Maximal value of the range.
	//! @return		The clamped value.
	inline static _float Clamp( _float value, _float min, _float max );

	//! Calculates the ceiling of a value.
	//! @param		value		Floating-point value.
	//! @return		A float value representing the smallest integer that is greater than or equal to argument.
	static _float Ceil( _float value );
	//! Calculates the floor of a value.
	//! @param		value		Floating-point value.
	//! @return		A float value representing the largest integer that is less than or equal to argument.
	static _float Floor( _float value );
	//! Calculates the floating-point remainder.
	//! @param		valuex		Floating-point value.
	//! @param		valuey		Floating-point value.
	//! @return		Floating-point remainder of valuex/valuey.
	static _float Fmod( _float valuex, _float valuey );
	//! Calculates linear interpolation of two floating-point value.
	//! @param		vector1		Source floating-point value 1.
	//! @param		vector2		Source floating-point value 2.
	//! @param		factor		The interpolation factor, will clamp to between 0 to 1.
	//! @return		The result floating-point value of interpolation, value1 + ( value2 - value1 ) * factor.
	static _float Lerp( _float value1, _float value2, _float factor );
	//! Calculates the integer closest in value to the argument.
	//! @param		value		Floating-point value.
	//! @return		The integer closest in value to the argument.
	static _long Round( _float value );

	//! Calculates the square root.
	//! @param		value		Nonnegative floating-point value.
	//! @return		The square root of argument, or an indefinite if argument is negative.
	static _float Sqrt( _float value );
	//! Calculates the exponential.
	//! @param		value		Floating-point value.
	//! @return		The exponential value of argument, or INF if overflow, or 0 if underflow.
	static _float Exp( _float value );
	//! Calculates logarithms.
	//! @param		value		Value whose logarithm is to be found.
	//! @return		The logarithm of argument, or an indefinite if argument is negative, or INF if argument is 0.
	static _float Log( _float value );
	//! Calculates base raised to the power of exponent.
	//! @param		base		The base.
	//! @param		exponent	The exponent.
	//! @return		The value that base raised to the power of exponent.
	static _float Pow( _float base, _float exponent );

	//! Wrap an angle in range ¨Cpi...pi by adding the correct multiple of 2 pi.
	//! @param		radian		Angle in radians.
	//! @return		Angle in range ¨Cpi...pi.
	static _float WrapPi( _float radian );
	//! Calculate the sine.
	//! @param		radian		Angle in radians.
	//! @return		The sine of argument.
	static _float Sin( _float radian );
	//! Calculate the cosine.
	//! @param		radian		Angle in radians.
	//! @return		The cosine of argument.
	static _float Cos( _float radian );
	//! Calculate the tangent.
	//! @param		radian		Angle in radians.
	//! @return		The tangent of argument.
	static _float Tan( _float radian );
	//! Calculate the cotangent.
	//! @param		radian		Angle in radians.
	//! @return		The cotangent of argument.
	static _float Cot( _float radian );
	//! Calculates the arcsine.
	//! @param		value		Value whose arcsine is to be calculated.
	//! @return		The arcsine of argument in the range -PI/2 to PI/2 radians.
	static _float Asin( _float value );
	//! Calculates the arccosine.
	//! @param		value		Value between -1 and 1 whose arccosine is to be calculated. 
	//! @return		The arccosine of argument in the range 0 to PI radians. 
	static _float Acos( _float value );
	//! Calculates the arctangent.
	//! @param		value		Value whose arctangent is to be calculated.
	//! @return		The arctangent of argument in the range -PI/2 to PI/2 radians, or 0 if value is 0.
	static _float Atan( _float value );
	//! Calculates the arctangent.
	//! @param		valuey		Value whose arctangent is to be calculated.
	//! @param		valuex		Value whose arctangent is to be calculated.
	//! @return		The arctangent of valuey/valuex in the range -PI to PI radians, or 0 if valuex is 0.
	static _float Atan( _float valuey, _float valuex );

	//! Converts radian to degree.
	//! @param		radian		Angle in radians.
	//! @return		The angle value of argument.
	inline static _float Radian2Degree( _float radian );
	//! Converts degree to radian.
	//! @param		degree		Angle in degrees.
	//! @return		The radian value of argument.
	inline static _float Degree2Radian( _float degree );

	//! Sets a random starting point.
	//! @param		seed		Seed for random-number generation.
	static _void RandomSeed( _dword seed );
	//! Generates a pseudorandom number.
	//! @param		none
	//! @return		A pseudorandom number, from zero to 0x7FFF.
	static _dword Random( );
	//! Generates a pseudorandom number.
	//! @param		maxvalue	The limitation of the random number.
	//! @return		A pseudorandom number, from zero to maxvalue - 1.
	static _dword Random( _dword maxvalue );
	//! Generates a pseudorandom number.
	//! @param		minvalue	The limitation of the random number.
	//! @param		maxvalue	The limitation of the random number.
	//! @return		A pseudorandom number, from minvalue to maxvalue.
	static _dword Random( _dword minvalue, _dword maxvalue );
	//! Generates a pseudorandom number.
	//! @param		minvalue	The limitation of the random number.
	//! @param		maxvalue	The limitation of the random number.
	//! @return		A pseudorandom number, from minvalue to maxvalue.
	static _float Random( _float minvalue, _float maxvalue );
	//! Generates a random sign, -1 or 1.
	//! @param		none
	//! @return		A random sign, -1 or 1.
	static _long RandomSign( );
};

//----------------------------------------------------------------------------
// Math Implementation
//----------------------------------------------------------------------------

_long Math::Abs( _long value )
{
	return value >= 0 ? value : -value;
}

_float Math::Abs( _float value )
{
	return value >= 0.0f ? value : -value;
}

_long Math::Max( _long value1, _long value2 )
{
	return value1 >= value2 ? value1 : value2;
}

_dword Math::Max( _dword value1, _dword value2 )
{
	return value1 >= value2 ? value1 : value2;
}

_float Math::Max( _float value1, _float value2 )
{
	return value1 >= value2 ? value1 : value2;
}

_long Math::Min( _long value1, _long value2 )
{
	return value1 < value2 ? value1 : value2;
}

_dword Math::Min( _dword value1, _dword value2 )
{
	return value1 < value2 ? value1 : value2;
}

_float Math::Min( _float value1, _float value2 )
{
	return value1 < value2 ? value1 : value2;
}

_void Math::Swap( _long& value1, _long& value2 )
{
	_long temp = value1;
	value1 = value2;
	value2 = temp;
}

_void Math::Swap( _dword& value1, _dword& value2 )
{
	_dword temp = value1;
	value1 = value2;
	value2 = temp;
}

_void Math::Swap( _float& value1, _float& value2 )
{
	_float temp = value1;
	value1 = value2;
	value2 = temp;
}

_long Math::Clamp( _long value, _long min, _long max )
{
	if ( value < min )
		value = min;

	if ( value > max )
		value = max;

	return value;
}

_float Math::Clamp( _float value, _float min, _float max )
{
	if ( value < min )
		value = min;

	if ( value > max )
		value = max;

	return value;
}

_float Math::Radian2Degree( _float radian )
{
	return radian / cPi * 180.0f;
}

_float Math::Degree2Radian( _float degree )
{
	return degree / 180.0f * cPi;
}

};
//============================================================================
// Color.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// Color
//----------------------------------------------------------------------------

//! This class represents a color use four float-point number to store r, g, b, a components.
class Color
{
public:
	//! The null color, has RGBA as 0.0f, 0.0f, 0.0f, 0.0f.
	const static Color cNull;

	//! Red, #FF0000
	const static Color cRed;
	//! Dark red, #8B0000
	const static Color cDarkRed;
	//! Indian red, #CD5C5C
	const static Color cIndianRed;
	//! Orange red, #FF4500
	const static Color cOrangeRed;

	//! Green, #00FF00
	const static Color cGreen;
	//! Dark green, #008B00
	const static Color cDarkGreen;
	//! Lawn green, #7CFC00
	const static Color cLawnGreen;
	//! Sea green, #2E8B57
	const static Color cSeaGreen;

	//! Blue, #0000FF
	const static Color cBlue;
	//! Dark blue, #00008B
	const static Color cDarkBlue;
	//! Sky blue, #00BFFF
	const static Color cSkyBlue;
	//! Midnight blue, #191970
	const static Color cMidnightBlue;

	//! White, #FFFFFF
	const static Color cWhite;
	//! Black, #000000
	const static Color cBlack;
	//! Gray, #808080
	const static Color cGray;
	//! Silver, #C0C0C0
	const static Color cSilver;

	//! Yellow, #FFFF00
	const static Color cYellow;
	//! Purple, #800080
	const static Color cPurple;
	//! Teal, #008080
	const static Color cTeal;
	//! Lime, #00FF00
	const static Color cLime;
	//! Tomato, #FF6347
	const static Color cTomato;
	//! Orange, #FFA500
	const static Color cOrange;
	//! Brown, #A52A2A
	const static Color cBrown;
	//! Bisque, #FFE4C4
	const static Color cBisque;
	//! Pink, #FFC0CB
	const static Color cPink;
	//! Violet, #EE82EE
	const static Color cViolet;
	//! Magenta, #FF00FF
	const static Color cMagenta;
	//! Orchid, #DA70D6
	const static Color cOrchid;
	//! Chocolate, #D2691E
	const static Color cChocolate;

	//! Red-component of the Color.
	_float	r;
	//! Green-component of the Color.
	_float	g;
	//! Blue-component of the Color.
	_float	b;
	//! Alpha-component of the Color.
	_float	a;

	//! Constructor, create a color without sets rgba components.
	//! @param		none
	inline Color( );
	//! Constructor, create a color and sets rgba components.
	//! @param		rr			Red-component of the vector.
	//! @param		gg			Green-component of the vector.
	//! @param		bb			Blue-component of the vector.
	//! @param		aa			Alpha-component of the vector.
	inline Color( _float rr, _float gg, _float bb, _float aa = 1.0f );
	//! Constructor, create a color from a dword.
	//! @param		color		The ARGB color dword.
	inline Color( _dword color );

	//! Type conversion, converts color from 4 floats to a dword.
	//! @param		none
	//! @return		The color represent in dword.
	inline operator _dword ( ) const;

	//! Get HSL value
	_void ToHSL( _float& h, _float& s, _float& l );

	//! Set HSL value
	_void FromHSL( _float h, _float s, _float l );

	//! Add a color to current color, and save result in current one.
	//! @param		color		The color to add.
	//! @return		The result color of addition.
	inline Color& operator += ( const Color& color );
	//! Subtract a color to current color, and save result in current one.
	//! @param		color		The color to subtract.
	//! @return		The result color of subtraction.
	inline Color& operator -= ( const Color& color );
	//! Multiply a scalar to current color, and save result in current one.
	//! @param		scalar		The scalar to multiply.
	//! @return		The result color of multiplication.
	inline Color& operator *= ( _float scalar );
	//! Multiply a color to current color, and save result in current one.
	//! @param		color		The color to multiply.
	//! @return		The result color of multiplication.
	inline Color& operator *= ( const Color& color );
	//! Divide a scalar to current color, and save result in current one.
	//! @param		scalar		The scalar to divide.
	//! @return		The result color of division.
	inline Color& operator /= ( _float scalar );
	//! Divide a color to current color, and save result in current one.
	//! @param		color		The color to divide.
	//! @return		The result color of division.
	inline Color& operator /= ( const Color& color );

	//! Add a color to current color, return a new color to save result.
	//! @param		color		The color to add.
	//! @return		The result color of addition.
	inline Color operator + ( const Color& color ) const;
	//! Subtract a color to current color, return a new color to save result.
	//! @param		color		The color to subtract.
	//! @return		The result color of subtraction.
	inline Color operator - ( const Color& color ) const;
	//! Multiply a scalar to current color, return a new color to save result.
	//! @param		scalar		The scalar to multiply.
	//! @return		The result color of multiplication.
	inline Color operator * ( _float scalar ) const;
	//! Multiply a color to current color, return a new color to save result.
	//! @param		color		The color to multiply.
	//! @return		The result color of multiplication.
	inline Color operator * ( const Color& color ) const;
	//! Divide a scalar to current color, return a new color to save result.
	//! @param		scalar		The scalar to divide.
	//! @return		The result color of division.
	inline Color operator / ( _float scalar ) const;
	//! Divide a color to current color, return a new color to save result.
	//! @param		color		The color to divide.
	//! @return		The result color of division.
	inline Color operator / ( const Color& color ) const;

	//! Calculates dot product of two colors.
	//! @param		color1		Source color 1.
	//! @param		color2		Source color 2.
	//! @return		The result color dot product.
	//! @remarks	Modulate the components of each argument as signed components, add their products;
	//!				then replicate the sum to all color channels, including alpha.
	static Color Dot( const Color& color1, const Color& color2 );
	//! Calculates linear interpolation of two colors.
	//! @param		color1		Source color 1.
	//! @param		color2		Source color 2.
	//! @param		alpha		The interpolation factor, will clamp to between 0 to 1.
	//! @return		The result color of interpolation, color1 + ( color2 - color1 ) * alpha.
	static Color Lerp( const Color& color1, const Color& color2, _float alpha );
};

//----------------------------------------------------------------------------
// Color Implementation
//----------------------------------------------------------------------------

Color::Color( )
{
}

Color::Color( _float rr, _float gg, _float bb, _float aa )
	: r( rr ), g( gg ), b( bb ), a( aa )
{
}

Color::Color( _dword color )
{
	Dword dwordcolor( color );

	a = dwordcolor.Byte4( ) / 255.0f;
	r = dwordcolor.Byte3( ) / 255.0f;
	g = dwordcolor.Byte2( ) / 255.0f;
	b = dwordcolor.Byte1( ) / 255.0f;
}

Color::operator _dword ( ) const
{
	_dword rr = (_byte) ( r * 255.0f );
	_dword gg = (_byte) ( g * 255.0f );
	_dword bb = (_byte) ( b * 255.0f );
	_dword aa = (_byte) ( a * 255.0f );

	return ( aa << 24 ) | ( rr << 16 ) | ( gg << 8 ) | ( bb );
}

Color& Color::operator += ( const Color& color )
{
	r = Math::Clamp( r + color.r, 0.0f, 1.0f );
	g = Math::Clamp( g + color.g, 0.0f, 1.0f );
	b = Math::Clamp( b + color.b, 0.0f, 1.0f );
	a = Math::Clamp( a + color.a, 0.0f, 1.0f );

	return *this;
}

Color& Color::operator -= ( const Color& color )
{
	r = Math::Clamp( r - color.r, 0.0f, 1.0f );
	g = Math::Clamp( g - color.g, 0.0f, 1.0f );
	b = Math::Clamp( b - color.b, 0.0f, 1.0f );
	a = Math::Clamp( a - color.a, 0.0f, 1.0f );

	return *this;
}

Color& Color::operator *= ( _float scalar )
{
	r = Math::Clamp( r * scalar, 0.0f, 1.0f );
	g = Math::Clamp( g * scalar, 0.0f, 1.0f );
	b = Math::Clamp( b * scalar, 0.0f, 1.0f );
	a = Math::Clamp( a * scalar, 0.0f, 1.0f );

	return *this;
}

Color& Color::operator *= ( const Color& color )
{
	r = Math::Clamp( r * color.r, 0.0f, 1.0f );
	g = Math::Clamp( g * color.g, 0.0f, 1.0f );
	b = Math::Clamp( b * color.b, 0.0f, 1.0f );
	a = Math::Clamp( a * color.a, 0.0f, 1.0f );

	return *this;
}

Color& Color::operator /= ( _float scalar )
{
	r = Math::Clamp( r / scalar, 0.0f, 1.0f );
	g = Math::Clamp( g / scalar, 0.0f, 1.0f );
	b = Math::Clamp( b / scalar, 0.0f, 1.0f );
	a = Math::Clamp( a / scalar, 0.0f, 1.0f );

	return *this;
}

Color& Color::operator /= ( const Color& color )
{
	r = Math::Clamp( r / color.r, 0.0f, 1.0f );
	g = Math::Clamp( g / color.g, 0.0f, 1.0f );
	b = Math::Clamp( b / color.b, 0.0f, 1.0f );
	a = Math::Clamp( a / color.a, 0.0f, 1.0f );

	return *this;
}

Color Color::operator + ( const Color& color ) const
{
	return Color(
		Math::Clamp( r + color.r, 0.0f, 1.0f ),
		Math::Clamp( g + color.g, 0.0f, 1.0f ),
		Math::Clamp( b + color.b, 0.0f, 1.0f ),
		Math::Clamp( a + color.a, 0.0f, 1.0f ) );
}

Color Color::operator - ( const Color& color ) const
{
	return Color(
		Math::Clamp( r - color.r, 0.0f, 1.0f ),
		Math::Clamp( g - color.g, 0.0f, 1.0f ),
		Math::Clamp( b - color.b, 0.0f, 1.0f ),
		Math::Clamp( a - color.a, 0.0f, 1.0f ) );
}

Color Color::operator * ( _float scalar ) const
{
	return Color(
		Math::Clamp( r * scalar, 0.0f, 1.0f ),
		Math::Clamp( g * scalar, 0.0f, 1.0f ),
		Math::Clamp( b * scalar, 0.0f, 1.0f ),
		Math::Clamp( a * scalar, 0.0f, 1.0f ) );
}

Color Color::operator * ( const Color& color ) const
{
	return Color(
		Math::Clamp( r * color.r, 0.0f, 1.0f ),
		Math::Clamp( g * color.g, 0.0f, 1.0f ),
		Math::Clamp( b * color.b, 0.0f, 1.0f ),
		Math::Clamp( a * color.a, 0.0f, 1.0f ) );
}

Color Color::operator / ( _float scalar ) const
{
	return Color(
		Math::Clamp( r / scalar, 0.0f, 1.0f ),
		Math::Clamp( g / scalar, 0.0f, 1.0f ),
		Math::Clamp( b / scalar, 0.0f, 1.0f ),
		Math::Clamp( a / scalar, 0.0f, 1.0f ) );
}

Color Color::operator / ( const Color& color ) const
{
	return Color(
		Math::Clamp( r / color.r, 0.0f, 1.0f ),
		Math::Clamp( g / color.g, 0.0f, 1.0f ),
		Math::Clamp( b / color.b, 0.0f, 1.0f ),
		Math::Clamp( a / color.a, 0.0f, 1.0f ) );
}

};

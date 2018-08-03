//============================================================================
// DataType.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// Tiny
//----------------------------------------------------------------------------

//! This class represents a signed 8-bits integer.
class Tiny
{
private:
	//!	The signed 8-bits integer.
	_tiny	mValue;

public:
	//! Constructor, create an object from a specified value.
	//! @param		value		The value to be set.
	Tiny( _tiny value );
	//! Constructor, create an object from a string.
	//! @param		string		The string contains the decimal integer.
	Tiny( StringPtr string );

	//! Type conversion, get the none-const reference of the integer, can be used to change the object.
	//! @param		none
	//! @return		The none-const reference of the integer.
	operator _tiny& ( );

	//! Convert to a string.
	//! @param		none
	//! @return		The string presentation of the integer in decimal.
	String ToString( ) const;
	//! Convert to a string.
	//! @param		buffer		The buffer used to copy string.
	//! @param		size		The max size of buffer in number of characters.
	//! @return		The string presentation of the integer in decimal.
	StringPtr ToString( _char* buffer, _dword size ) const;
};

//----------------------------------------------------------------------------
// Short
//----------------------------------------------------------------------------

//! This class represents a signed 16-bits integer.
class Short
{
private:
	_short	mValue;

public:
	//! Constructor, create an object from a specified value.
	//! @param		value		The value to be set.
	Short( _short value );
	//! Constructor, create an object from a string.
	//! @param		string		The string contains the decimal integer.
	Short( StringPtr string );

	//! Get the none-const reference of the integer, can be used to change the object.
	//! @return		The none-const reference of the integer.
	operator _short& ( );

	//! Convert to a string.
	//! @param		none
	//! @return		The string presentation of the integer in decimal.
	String ToString( ) const;
	//! Convert to a string.
	//! @param		buffer		The buffer used to copy string.
	//! @param		size		The max size of buffer in number of characters.
	//! @return		The string presentation of the integer in decimal.
	StringPtr ToString( _char* buffer, _dword size ) const;
};

//----------------------------------------------------------------------------
// Long
//----------------------------------------------------------------------------

//! This class represents a signed 32-bits integer.
class Long
{
private:
	//!	The signed 32-bits integer.
	_long	mValue;

public:
	//! Constructor, create an object from a specified value.
	//! @param		value		The value to be set.
	Long( _long value );
	//! Constructor, create an object from a string.
	//! @param		string		The string contains the decimal integer.
	Long( StringPtr string );

	//! Type conversion, get the none-const reference of the integer, can be used to change the object.
	//! @param		none
	//! @return		The none-const reference of the integer.
	operator _long& ( );

	//! Convert to a string.
	//! @param		none
	//! @return		The string presentation of the integer in decimal.
	String ToString( ) const;
	//! Convert to a string.
	//! @param		buffer		The buffer used to copy string.
	//! @param		size		The max size of buffer in number of characters.
	//! @return		The string presentation of the integer in decimal.
	StringPtr ToString( _char* buffer, _dword size ) const;
};

//----------------------------------------------------------------------------
// Large
//----------------------------------------------------------------------------

//! This class represents a signed 64-bits integer.
class Large
{
private:
	//!	The signed 64-bits integer.
	_large	mValue;

public:
	//! Constructor, create an object from a specified value.
	//! @param		value		The value to be set.
	Large( _large value );
	//! Constructor, create an object from a string.
	//! @param		string		The string contains the decimal integer.
	Large( StringPtr string );

	//! Type conversion, get the none-const reference of the integer, can be used to change the object.
	//! @param		none
	//! @return		The none-const reference of the integer.
	operator _large& ( );

	//! Convert to a string.
	//! @param		none
	//! @return		The string presentation of the integer in decimal.
	String ToString( ) const;
	//! Convert to a string.
	//! @param		buffer		The buffer used to copy string.
	//! @param		size		The max size of buffer in number of characters.
	//! @return		The string presentation of the integer in decimal.
	StringPtr ToString( _char* buffer, _dword size ) const;
};

//----------------------------------------------------------------------------
// Byte
//----------------------------------------------------------------------------

//! This class represents a unsigned 8-bits integer.
class Byte
{
private:
	//!	The unsigned 8-bits integer.
	_byte	mValue;

public:
	//! Constructor, create an object from a specified value.
	//! @param		value		The value to be set.
	Byte( _byte value );
	//! Constructor, create an object from a string.
	//! @param		string		The string contains the hex integer.
	Byte( StringPtr string );

	//! Type conversion, get the none-const reference of the integer, can be used to change the object.
	//! @param		none
	//! @return		The none-const reference of the integer.
	operator _byte& ( );

	//! Convert to a string.
	//! @param		none
	//! @return		The string presentation of the integer in hex.
	String ToString( ) const;
	//! Convert to a string.
	//! @param		buffer		The buffer used to copy string.
	//! @param		size		The max size of buffer in number of characters.
	//! @return		The string presentation of the integer in hex.
	StringPtr ToString( _char* buffer, _dword size ) const;
};

//----------------------------------------------------------------------------
// Word
//----------------------------------------------------------------------------

//! This class represents a unsigned 16-bits integer.
class Word
{
private:
	//!	The unsigned 16-bits integer.
	_word	mValue;

public:
	//! Constructor, create an object from a specified value.
	//! @param		value		The value to be set.
	Word( _word value );
	//! Constructor, create an object from two bytes.
	//! @param		lobyte		The lower byte of the word.
	//! @param		hibyte		The higher byte of the word.
	Word( _byte lobyte, _byte hibyte );
	//! Constructor, create an object from a string.
	//! @param		string		The string contains the hex integer.
	Word( StringPtr string );

	//! Type conversion, get the none-const reference of the integer, can be used to change the object.
	//! @param		none
	//! @return		The none-const reference of the integer.
	operator _word& ( );

	//! Inverse the order of bytes.
	//! @param		none
	//! @return		The new word object.
	Word InverseByteOrder( ) const;

	//! Convert to a string.
	//! @param		none
	//! @return		The string presentation of the integer in hex.
	String ToString( ) const;
	//! Convert to a string.
	//! @param		buffer		The buffer used to copy string.
	//! @param		size		The max size of buffer in number of characters.
	//! @return		The string presentation of the integer in hex.
	StringPtr ToString( _char* buffer, _dword size ) const;

	//! Get the lower byte of the word.
	//! @param		none
	//! @return		The lower byte of the word.
	_byte LoByte( ) const;
	//! Get the higher byte of the word.
	//! @param		none
	//! @return		The higher byte of the word.
	_byte HiByte( ) const;
};

//----------------------------------------------------------------------------
// Dword
//----------------------------------------------------------------------------

//! This class represents a unsigned 32-bits integer.
class Dword
{
private:
	//!	The unsigned 32-bits integer.
	_dword	mValue;

public:
	//! Constructor, create an object from a specified value.
	//! @param		value		The value to be set.
	Dword( _dword value );
	//! Constructor, create an object from two words.
	//! @param		loword		The lower word of the dword.
	//! @param		hiword		The higher word of the dword.
	Dword( _word loword, _word hiword );
	//! Constructor, create an object from four bytes.
	//! @param		byte1		The 1st byte of the dword.
	//! @param		byte2		The 2nd byte of the dword.
	//! @param		byte3		The 3rd byte of the dword.
	//! @param		byte4		The 4th byte of the dword.
	Dword( _byte byte1, _byte byte2, _byte byte3, _byte byte4 );
	//! Constructor, create an object from a string.
	//! @param		string		The string contains the hex integer.
	Dword( StringPtr string );

	//! Type conversion, get the none-const reference of the integer, can be used to change the object.
	//! @param		none
	//! @return		The none-const reference of the integer.
	operator _dword& ( );

	//! Inverse the order of bytes.
	//! @param		none
	//! @return		The new dword object.
	Dword InverseByteOrder( ) const;

	//! Shift the order of bytes to left.
	//! @param		none
	//! @return		The new dword object.
	Dword CircleShift1( ) const;
	//! Shift the order of bytes to left.
	//! @param		none
	//! @return		The new dword object.
	Dword CircleShift2( ) const;
	//! Shift the order of bytes to left.
	//! @param		none
	//! @return		The new dword object.
	Dword CircleShift3( ) const;

	//! Convert to a string.
	//! @param		none
	//! @return		The string presentation of the integer in hex.
	String ToString( ) const;
	//! Convert to a string.
	//! @param		buffer		The buffer used to copy string.
	//! @param		size		The max size of buffer in number of characters.
	//! @return		The string presentation of the integer in hex.
	StringPtr ToString( _char* buffer, _dword size ) const;

	//! Get the 1st byte of the dword.
	//! @param		none
	//! @return		The 1st byte of the dword.
	_byte Byte1( ) const;
	//! Get the 2nd byte of the dword.
	//! @param		none
	//! @return		The 2nd byte of the dword.
	_byte Byte2( ) const;
	//! Get the 3rd byte of the dword.
	//! @param		none
	//! @return		The 3rd byte of the dword.
	_byte Byte3( ) const;
	//! Get the 4th byte of the dword.
	//! @param		none
	//! @return		The 4th byte of the dword.
	_byte Byte4( ) const;
	//! Get the lower word of the dword.
	//! @param		none
	//! @return		The lower word of the dword.
	_word LoWord( ) const;
	//! Get the higher word of the dword.
	//! @param		none
	//! @return		The higher word of the dword.
	_word HiWord( ) const;
};

//----------------------------------------------------------------------------
// Qword
//----------------------------------------------------------------------------

//! This class represents a unsigned 64-bits integer.
class Qword
{
private:
	//!	The unsigned 32-bits integer.
	_qword	mValue;

public:
	//! Constructor, create an object from a specified value.
	//! @param		value		The value to be set.
	Qword( _qword value );
	//! Constructor, create an object from two dwords.
	//! @param		lodword		The lower dword of the qword.
	//! @param		hidword		The higher dword of the qword.
	Qword( _dword lodword, _dword hidword );
	//! Constructor, create an object from a string.
	//! @param		string		The string contains the hex integer.
	Qword( StringPtr string );

	//! Type conversion, get the none-const reference of the integer, can be used to change the object.
	//! @param		none
	//! @return		The none-const reference of the integer.
	operator _qword& ( );

	//! Inverse the order of bytes.
	//! @param		none
	//! @return		The new qword object.
	Qword InverseByteOrder( ) const;

	//! Convert to a string.
	//! @param		none
	//! @return		The string presentation of the integer in hex.
	String ToString( ) const;
	//! Convert to a string.
	//! @param		buffer		The buffer used to copy string.
	//! @param		size		The max size of buffer in number of characters.
	//! @return		The string presentation of the integer in hex.
	StringPtr ToString( _char* buffer, _dword size ) const;

	//! Get the lower dword of the qword.
	//! @param		none
	//! @return		The lower dword of the qword.
	_dword LoDword( ) const;
	//! Get the higher dword of the qword.
	//! @param		none
	//! @return		The higher dword of the qword.
	_dword HiDword( ) const;
};

//----------------------------------------------------------------------------
// Oword
//----------------------------------------------------------------------------

//! This class represents a unsigned 128-bits integer.
class Oword
{
private:
	//!	The 1st dword of the unsigned 128-bits integer.
	_dword	mDword1;
	//!	The 2nd dword of the unsigned 128-bits integer.
	_dword	mDword2;
	//!	The 3rd dword of the unsigned 128-bits integer.
	_dword	mDword3;
	//!	The 4th dword of the unsigned 128-bits integer.
	_dword	mDword4;

public:
	//! Constructor, create an object that all bits are zero.
	Oword( );
	//! Constructor, create an object from four dwords.
	//! @param		dword1		The 1st dword of the oword.
	//! @param		dword2		The 2nd dword of the oword.
	//! @param		dword3		The 3rd dword of the oword.
	//! @param		dword4		The 4th dword of the oword.
	Oword( _dword dword1, _dword dword2, _dword dword3, _dword dword4 );
	//! Constructor, create an object from a string.
	//! @param		string		The string contains the hex integer.
	Oword( StringPtr string );

	//! Type conversion, convert the 128-bits integer to a hash code.
	//! @param		none
	//! @return		The hash code of the 128-bits integer.
	operator _dword ( ) const;

	//! Inverse the order of bytes.
	//! @param		none
	//! @return		The new oword object.
	Oword InverseByteOrder( ) const;

	//! Convert to a string.
	//! @param		none
	//! @return		The string presentation of the integer in hex.
	String ToString( ) const;
	//! Convert to a string.
	//! @param		buffer		The buffer used to copy string.
	//! @param		size		The max size of buffer in number of characters.
	//! @return		The string presentation of the integer in hex.
	StringPtr ToString( _char* buffer, _dword size ) const;

	//! Get the 1st dword of the oword.
	//! @param		none
	//! @return		The 1st dword of the oword.
	_dword Dword1( ) const;
	//! Get the 2nd dword of the oword.
	//! @param		none
	//! @return		The 2nd dword of the oword.
	_dword Dword2( ) const;
	//! Get the 3rd dword of the oword.
	//! @param		none
	//! @return		The 3rd dword of the oword.
	_dword Dword3( ) const;
	//! Get the 4th dword of the oword.
	//! @param		none
	//! @return		The 4th dword of the oword.
	_dword Dword4( ) const;

	//! Compare two 128-bits integer.
	//! @param		oword		The second 128-bits integer.
	//! @return		True if two 128-bits integer are same, false otherwise.
	_bool operator == ( const Oword& oword ) const;
	//! Compare two 128-bits integer.
	//! @param		oword		The second 128-bits integer.
	//! @return		True if two 128-bits integer are not same, false otherwise.
	_bool operator != ( const Oword& oword ) const;
};

//----------------------------------------------------------------------------
// Float
//----------------------------------------------------------------------------

//! This class represents a 32-bits floating point number.
class Float
{
private:
	//!	The 32-bits floating point number.
	_float	mValue;

public:
	//! Constructor, create an object from a specified value.
	//! @param		value	The value to be set.
	Float( _float value );
	//! Constructor, create an object from a string.
	//! @param		string	The string contains the floating point number.
	Float( StringPtr string );

	//! Type conversion, get the none-const reference of the number, can be used to change the object.
	//! @return		The none-const reference of the number.
	operator _float& ( );

	//! Convert to a string.
	//! @param		none
	//! @return		The string presentation of the number in floating point.
	String ToString( ) const;
	//! Convert to a string.
	//! @param		buffer		The buffer used to copy string.
	//! @param		size		The max size of buffer in number of characters.
	//! @return		The string presentation of the integer in floating point.
	StringPtr ToString( _char* buffer, _dword size ) const;
};

//----------------------------------------------------------------------------
// Double
//----------------------------------------------------------------------------

//! This class represents a 64-bits floating point number.
class Double
{
private:
	//!	The 64-bits floating point number.
	_double	mValue;

public:
	//! Constructor, create an object from a specified value.
	//! @param		value		The value to be set.
	Double( _double value );
	//! Constructor, create an object from a string.
	//! @param		string		The string contains the floating point number.
	Double( StringPtr string );

	//! Type conversion, get the none-const reference of the number, can be used to change the object.
	//! @param		none
	//! @return		The none-const reference of the number.
	operator _double& ( );

	//! Convert to a string.
	//! @param		none
	//! @return		The string presentation of the number in floating point.
	String ToString( ) const;
	//! Convert to a string.
	//! @param		buffer		The buffer used to copy string.
	//! @param		size		The max size of buffer in number of characters.
	//! @return		The string presentation of the integer in floating point.
	StringPtr ToString( _char* buffer, _dword size ) const;
};

};
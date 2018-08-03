//============================================================================
// Encryption.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// Encryption Implementation
//----------------------------------------------------------------------------

inline _void MD5FF( _dword& a, _dword b, _dword c, _dword d, _dword x, _dword s, _dword t )
{
	a += ( ( b & c ) | ( ~b & d ) ) + x + t;
	a = b + ( ( a << s ) | ( a >> ( 32 - s ) ) );
}

inline _void MD5GG( _dword& a, _dword b, _dword c, _dword d, _dword x, _dword s, _dword t )
{
	a += ( ( b & d ) | ( c & ~d ) ) + x + t;
	a = b + ( ( a << s ) | ( a >> ( 32 - s ) ) );
}

inline _void MD5HH( _dword& a, _dword b, _dword c, _dword d, _dword x, _dword s, _dword t )
{
	a += ( b ^ c ^ d ) + x + t;
	a = b + ( ( a << s ) | ( a >> ( 32 - s ) ) );
}

inline _void MD5II( _dword& a, _dword b, _dword c, _dword d, _dword x, _dword s, _dword t )
{
	a += ( c ^ ( b | ~d ) ) + x + t;
	a = b + ( ( a << s ) | ( a >> ( 32 - s ) ) );
}

_dword Encryption::Adler32Checksum( _void* buffer, _dword length )
{
	return ::adler32( ::adler32( 0, _null, 0 ), (_byte*) buffer, length );
}

_dword Encryption::Adler32Checksum( _dword adler, _void* buffer, _dword length )
{
	return ::adler32( adler, (_byte*) buffer, length );
}

_dword Encryption::CRC32Checksum( _void* buffer, _dword length )
{
	return ::crc32( ::crc32( 0, _null, 0 ), (_byte*) buffer, length );
}

_dword Encryption::CRC32Checksum( _dword crc, _void* buffer, _dword length )
{
	return ::crc32( crc, (_byte*) buffer, length );
}

_dword Encryption::MD5Checksum( _void* buffer, _dword length, Oword& checksum )
{
	_dword md5a = 0x67452301;
	_dword md5b = 0xEFCDAB89;
	_dword md5c = 0x98BADCFE;
	_dword md5d = 0x10325476;

	const _byte cPadding[64] = { 0x80 };

	_dword tempbuffer[16], a, b, c, d, size = length, maxsize = size, needpadding = 1;

	do
	{
		a = md5a; b = md5b; c = md5c; d = md5d;

		_dword templength = Math::Min( size, (_dword) 64 );
		Memory::MemCpy( tempbuffer, buffer, templength );

		if ( needpadding == 0 )
		{
			Memory::MemSet( tempbuffer, 0, 64 );

			tempbuffer[14] = maxsize << 3;
			tempbuffer[15] = maxsize >> 29;

			needpadding = 1;
		}
		else if ( templength <= 56 )
		{
			Memory::MemCpy( (_byte*) tempbuffer + templength, cPadding, 64 - templength );

			tempbuffer[14] = maxsize << 3;
			tempbuffer[15] = maxsize >> 29;

			needpadding = 1;
		}
		else if ( templength < 64 )
		{
			Memory::MemCpy( (_byte*) tempbuffer + templength, cPadding, 64 - templength );
			size = 64;

			needpadding = 0;
		}

		MD5FF( a, b, c, d, tempbuffer[ 0],  7, 0xD76AA478 );
		MD5FF( d, a, b, c, tempbuffer[ 1], 12, 0xE8C7B756 );
		MD5FF( c, d, a, b, tempbuffer[ 2], 17, 0x242070DB );
		MD5FF( b, c, d, a, tempbuffer[ 3], 22, 0xC1BDCEEE );
		MD5FF( a, b, c, d, tempbuffer[ 4],  7, 0xF57C0FAF );
		MD5FF( d, a, b, c, tempbuffer[ 5], 12, 0x4787C62A );
		MD5FF( c, d, a, b, tempbuffer[ 6], 17, 0xA8304613 );
		MD5FF( b, c, d, a, tempbuffer[ 7], 22, 0xFD469501 );
		MD5FF( a, b, c, d, tempbuffer[ 8],  7, 0x698098D8 );
		MD5FF( d, a, b, c, tempbuffer[ 9], 12, 0x8B44F7AF );
		MD5FF( c, d, a, b, tempbuffer[10], 17, 0xFFFF5BB1 );
		MD5FF( b, c, d, a, tempbuffer[11], 22, 0x895CD7BE );
		MD5FF( a, b, c, d, tempbuffer[12],  7, 0x6B901122 );
		MD5FF( d, a, b, c, tempbuffer[13], 12, 0xFD987193 );
		MD5FF( c, d, a, b, tempbuffer[14], 17, 0xA679438E );
		MD5FF( b, c, d, a, tempbuffer[15], 22, 0x49B40821 );

		MD5GG( a, b, c, d, tempbuffer[ 1],  5, 0xF61E2562 );
		MD5GG( d, a, b, c, tempbuffer[ 6],  9, 0xC040B340 );
		MD5GG( c, d, a, b, tempbuffer[11], 14, 0x265E5A51 );
		MD5GG( b, c, d, a, tempbuffer[ 0], 20, 0xE9B6C7AA );
		MD5GG( a, b, c, d, tempbuffer[ 5],  5, 0xD62F105D );
		MD5GG( d, a, b, c, tempbuffer[10],  9, 0x02441453 );
		MD5GG( c, d, a, b, tempbuffer[15], 14, 0xD8A1E681 );
		MD5GG( b, c, d, a, tempbuffer[ 4], 20, 0xE7D3FBC8 );
		MD5GG( a, b, c, d, tempbuffer[ 9],  5, 0x21E1CDE6 );
		MD5GG( d, a, b, c, tempbuffer[14],  9, 0xC33707D6 );
		MD5GG( c, d, a, b, tempbuffer[ 3], 14, 0xF4D50D87 );
		MD5GG( b, c, d, a, tempbuffer[ 8], 20, 0x455A14ED );
		MD5GG( a, b, c, d, tempbuffer[13],  5, 0xA9E3E905 );
		MD5GG( d, a, b, c, tempbuffer[ 2],  9, 0xFCEFA3F8 );
		MD5GG( c, d, a, b, tempbuffer[ 7], 14, 0x676F02D9 );
		MD5GG( b, c, d, a, tempbuffer[12], 20, 0x8D2A4C8A );

		MD5HH( a, b, c, d, tempbuffer[ 5],  4, 0xFFFA3942 );
		MD5HH( d, a, b, c, tempbuffer[ 8], 11, 0x8771F681 );
		MD5HH( c, d, a, b, tempbuffer[11], 16, 0x6D9D6122 );
		MD5HH( b, c, d, a, tempbuffer[14], 23, 0xFDE5380C );
		MD5HH( a, b, c, d, tempbuffer[ 1],  4, 0xA4BEEA44 );
		MD5HH( d, a, b, c, tempbuffer[ 4], 11, 0x4BDECFA9 );
		MD5HH( c, d, a, b, tempbuffer[ 7], 16, 0xF6BB4B60 );
		MD5HH( b, c, d, a, tempbuffer[10], 23, 0xBEBFBC70 );
		MD5HH( a, b, c, d, tempbuffer[13],  4, 0x289B7EC6 );
		MD5HH( d, a, b, c, tempbuffer[ 0], 11, 0xEAA127FA );
		MD5HH( c, d, a, b, tempbuffer[ 3], 16, 0xD4EF3085 );
		MD5HH( b, c, d, a, tempbuffer[ 6], 23, 0x04881D05 );
		MD5HH( a, b, c, d, tempbuffer[ 9],  4, 0xD9D4D039 );
		MD5HH( d, a, b, c, tempbuffer[12], 11, 0xE6DB99E5 );
		MD5HH( c, d, a, b, tempbuffer[15], 16, 0x1FA27CF8 );
		MD5HH( b, c, d, a, tempbuffer[ 2], 23, 0xC4AC5665 );

		MD5II( a, b, c, d, tempbuffer[ 0],  6, 0xF4292244 );
		MD5II( d, a, b, c, tempbuffer[ 7], 10, 0x432AFF97 );
		MD5II( c, d, a, b, tempbuffer[14], 15, 0xAB9423A7 );
		MD5II( b, c, d, a, tempbuffer[ 5], 21, 0xFC93A039 );
		MD5II( a, b, c, d, tempbuffer[12],  6, 0x655B59C3 );
		MD5II( d, a, b, c, tempbuffer[ 3], 10, 0x8F0CCC92 );
		MD5II( c, d, a, b, tempbuffer[10], 15, 0xFFEFF47D );
		MD5II( b, c, d, a, tempbuffer[ 1], 21, 0x85845DD1 );
		MD5II( a, b, c, d, tempbuffer[ 8],  6, 0x6FA87E4F );
		MD5II( d, a, b, c, tempbuffer[15], 10, 0xFE2CE6E0 );
		MD5II( c, d, a, b, tempbuffer[ 6], 15, 0xA3014314 );
		MD5II( b, c, d, a, tempbuffer[13], 21, 0x4E0811A1 );
		MD5II( a, b, c, d, tempbuffer[ 4],  6, 0xF7537E82 );
		MD5II( d, a, b, c, tempbuffer[11], 10, 0xBD3AF235 );
		MD5II( c, d, a, b, tempbuffer[ 2], 15, 0x2AD7D2BB );
		MD5II( b, c, d, a, tempbuffer[ 9], 21, 0xEB86D391 );

		md5a += a; md5b += b; md5c += c; md5d += d;

		buffer = (_byte*) buffer + 64;

		if ( size > 64 )
			size -= 64;
		else
			size = 0;
	}
	while ( size > 0 || needpadding == 0 );

	checksum = Oword( md5a, md5b, md5c, md5d );

	return md5a;
}

_void Encryption::NOTEncrypt( _void* buffer, _dword length )
{
	_byte* pointer = (_byte*) buffer;

	for ( _dword i = 0; i < length; i ++ )
		pointer[i] = ~pointer[i];
}

_void Encryption::NOTDecrypt( _void* buffer, _dword length )
{
	_byte* pointer = (_byte*) buffer;

	for ( _dword i = 0; i < length; i ++ )
		pointer[i] = ~pointer[i];
}

_void Encryption::XOREncrypt( _void* buffer, _dword length, _byte key )
{
	_byte* pointer = (_byte*) buffer;

	for ( _dword i = 0; i < length; i ++ )
		pointer[i] ^= key;
}

_void Encryption::XORDecrypt( _void* buffer, _dword length, _byte key )
{
	_byte* pointer = (_byte*) buffer;

	for ( _dword i = 0; i < length; i ++ )
		pointer[i] ^= key;
}
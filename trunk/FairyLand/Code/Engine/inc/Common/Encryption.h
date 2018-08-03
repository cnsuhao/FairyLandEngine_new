//============================================================================
// Encryption.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// Encryption
//----------------------------------------------------------------------------

class Encryption
{
public:
	static _dword Adler32Checksum( _void* buffer, _dword length );
	static _dword Adler32Checksum( _dword adler, _void* buffer, _dword length );

	static _dword CRC32Checksum( _void* buffer, _dword length );
	static _dword CRC32Checksum( _dword crc, _void* buffer, _dword length );

	static _dword MD5Checksum( _void* buffer, _dword length, Oword& checksum );

	static _void NOTEncrypt( _void* buffer, _dword length );
	static _void NOTDecrypt( _void* buffer, _dword length );

	static _void XOREncrypt( _void* buffer, _dword length, _byte key );
	static _void XORDecrypt( _void* buffer, _dword length, _byte key );
};

};
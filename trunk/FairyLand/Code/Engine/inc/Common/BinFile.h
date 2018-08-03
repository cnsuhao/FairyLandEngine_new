//============================================================================
// BinFile.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// BinFile
//----------------------------------------------------------------------------

class BinFile : public MemFile
{
public:
	BinFile( );

	inline _bool  ReachEnd( ) const;
	inline _dword RemainBytes( ) const;

	_bool WriteBuffer( const _void* buffer, _dword size );
	_bool WriteTiny( _tiny data );
	_bool WriteShort( _short data );
	_bool WriteLong( _long data );
	_bool WriteByte( _byte data );
	_bool WriteWord( _word data );
	_bool WriteDword( _dword data );
	_bool WriteFloat( _float data );
	_bool WriteDouble( _double data );
	_bool WriteBool( _bool data );
	_bool WriteString( StringPtr string );
	_bool WriteString( StringPtr string, _dword length );

	_bool ReadBuffer( _void* buffer, _dword size );
	_bool ReadTiny( _tiny& data );
	_bool ReadShort( _short& data );
	_bool ReadLong( _long& data );
	_bool ReadByte( _byte& data );
	_bool ReadWord( _word& data );
	_bool ReadDword( _dword& data );
	_bool ReadFloat( _float& data );
	_bool ReadDouble( _double& data );
	_bool ReadBool( _bool& data );
	_bool ReadString( String& string );
	_bool ReadString( String& string, _dword length );

	_bool OffsetFromBegin( _long offset );
	_bool OffsetFromEnd( _long offset );
	_bool OffsetFromCurrent( _long offset );
};

//----------------------------------------------------------------------------
// BinFile Implementation
//----------------------------------------------------------------------------

_bool BinFile::ReachEnd( ) const
{
	return (_dword) ( mPointer - mBuffer ) == mLength;
}

_dword BinFile::RemainBytes( ) const
{
	return mLength - ( mPointer - mBuffer );
}

};
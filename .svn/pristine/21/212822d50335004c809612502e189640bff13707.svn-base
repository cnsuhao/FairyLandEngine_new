//============================================================================
// Language.cpp
//
// Copyright 2006-2008 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// Language Implementation
//----------------------------------------------------------------------------

const	LanguageDesc	Language::cLanguageDescTable[ LANGUAGE_MAX - LANGUAGE_FIRST ] =
{	
	{ LANGUAGE_EN_US,	L"en_us" },
	{ LANGUAGE_EN_EU,	L"en_eu" },
	{ LANGUAGE_ZH_CN,	L"zh_cn" },
	{ LANGUAGE_ZH_TW,	L"zh_tw" },
	{ LANGUAGE_DE_DE,	L"de_de" },
	{ LANGUAGE_FR_FR,	L"fr_fr" },
	{ LANGUAGE_ES_ES,	L"es_es" },
	{ LANGUAGE_TR_TR,	L"tr_tr" },
};

const	LanguageDesc*	Language::GetLanguageDesc( _dword id )
{
	for ( _dword index = 0; index < LANGUAGE_MAX - LANGUAGE_FIRST; ++index )
	{
		if ( cLanguageDescTable[ index ].mID == id )
			return	cLanguageDescTable + index;
	}

	return	_null;
}

const	LanguageDesc*	Language::GetLanguageDesc( StringPtr typeString )
{
	for ( _dword index = 0; index < LANGUAGE_MAX - LANGUAGE_FIRST; ++index )
	{
		if ( cLanguageDescTable[ index ].mTypeString == typeString )
			return	cLanguageDescTable + index;
	}

	return	_null;
}

StringPtr	Language::GetLanguageAlias( StringPtr typeString )
{
	if ( typeString == L"en_eu" )
		return L"en_us";

	return typeString;
}
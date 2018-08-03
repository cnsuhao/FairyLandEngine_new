//============================================================================
// Language.h
//
// Copyright 2006-2008 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// Language
//----------------------------------------------------------------------------

struct LanguageDesc
{
	_dword		mID;
	StringPtr	mTypeString;
};

class Language
{
public:
	enum LANGUAGE_TYPE_ID
	{
		LANGUAGE_FIRST	= 1,
		LANGUAGE_EN_US  = LANGUAGE_FIRST,
		LANGUAGE_EN_EU,
		LANGUAGE_ZH_CN,
		LANGUAGE_ZH_TW,
		LANGUAGE_DE_DE,
		LANGUAGE_FR_FR,
		LANGUAGE_ES_ES,
		LANGUAGE_TR_TR,
		LANGUAGE_MAX,
	};

	static	const	LanguageDesc*	GetLanguageDesc( _dword id );
	static	const	LanguageDesc*	GetLanguageDesc( StringPtr typeString );
	static	StringPtr				GetLanguageAlias( StringPtr typeString );

private:
	static	const	LanguageDesc	cLanguageDescTable[ LANGUAGE_MAX - LANGUAGE_FIRST ];
};

};
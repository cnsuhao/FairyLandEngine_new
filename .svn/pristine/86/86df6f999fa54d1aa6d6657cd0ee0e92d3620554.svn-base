#pragma once
#include "FairyLand.h"
#pragma warning( disable : 4819 )

#define _WIN32_WINNT 0x0500

#include "windows.h"
#include "tchar.h"
#include "richedit.h"
#include "stdio.h"
#include "math.h"
#include "shlobj.h"
#include "dbghelp.h"
#include "psapi.h"
#include "wininet.h"
#include "lm.h"
#include "sql.h"
#include "sqlext.h"
#include "iptypes.h"
#include "ddraw.h"
#include "d3d9.h"
#include "zlib.h"
#include "vtuneapi.h"

extern "C"
{
	void __cdecl mmx_memcpy(void *d,void *s,int bytes);
	void __cdecl mmx_memset(void *d,unsigned int val,int bytes);
}

extern "C"
{
	// Pcre
#define PCRE_STATIC
#include "pcre.h"
}

#pragma comment( lib, "dbghelp" )
#pragma comment( lib, "winmm" )
#pragma comment( lib, "psapi" )
#pragma comment( lib, "wininet" )
#pragma comment( lib, "netapi32" )

using namespace FL;
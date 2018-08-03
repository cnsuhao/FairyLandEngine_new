// lua.hpp
// Lua header files for C++
// <<extern "C">> not supplied automatically because Lua also compiles as C++

#ifdef _DEBUG
#pragma comment(lib, "lua52d.lib")
#else
#pragma comment(lib, "lua52.lib")
#endif

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

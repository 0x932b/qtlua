// -*- C++ -*-
// Compatibility macros for different Lua versions

#ifndef QTLUA_COMPAT_H
#define QTLUA_COMPAT_H

// String quoting macros if not defined by Lua
#ifndef LUA_QS
#define LUA_QS      "\"%s\""
#endif

#ifndef LUA_QL
#define LUA_QL(x)   "'" x "'"
#endif

#endif // QTLUA_COMPAT_H
#ifndef LUA_WEAPONDEFS_H
#define LUA_WEAPONDEFS_H
// LuaWeaponDefs.h: interface for the LuaWeaponDefs class.
//
//////////////////////////////////////////////////////////////////////

#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

struct lua_State;

class LuaWeaponDefs {
	public:
		static bool PushEntries(lua_State* L);
};


#endif /* LUA_WEAPONDEFS_H */

#ifndef LUA_GAIA_H
#define LUA_GAIA_H
// LuaGaia.h: interface for the CLuaGaia class.
//
//////////////////////////////////////////////////////////////////////

#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

#include <string>
using std::string;

#include "LuaHandleSynced.h"


class CLuaGaia : public CLuaHandleSynced
{
	public:
		static void LoadHandler();
		static void FreeHandler();

		static bool SetConfigString(const string& cfg);
		static const string& GetConfigString() { return configString; }

	protected:
		bool AddSyncedCode();
		bool AddUnsyncedCode();

	private:
		CLuaGaia();
		~CLuaGaia();

	private: // call-outs
		static int GetConfigString(lua_State* L);

	private:
		static void CobCallback(int retCode, void* p1, void* p2);

	private:
		static string configString;
};


extern CLuaGaia* luaGaia;


#endif /* LUA_GAIA_H */

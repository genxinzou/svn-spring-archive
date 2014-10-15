// ScriptHandler.cpp: implementation of the CScriptHandler class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "mmgr.h"

#include "ScriptHandler.h"
#include "Game/Game.h"
#include "FileSystem/FileHandler.h"
#include "FileSystem/VFSHandler.h"
#include "LoadScript.h"
#include "CommanderScript.h"
#include "CommanderScript2.h"
#include "AirScript.h"
#include "GlobalAITestScript.h"
#include "SpawnScript.h"
#include "EmptyScript.h"
#include "TestScript.h"
#include "Platform/SharedLib.h"
#include "Exceptions.h"
#ifndef NO_LUA
#  include "System/Platform/errorhandler.h"
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ListSelectCallback CScriptHandler::callback = 0;

CScriptHandler::CScriptHandler() : chosenScript(0)
{
}

/** Load all scripts. */
void CScriptHandler::LoadScripts() {

	ENTER_SYNCED;
	loaded_scripts.push_back( SAFE_NEW CCommanderScript() );
	loaded_scripts.push_back( SAFE_NEW CCommanderScript2() );
	loaded_scripts.push_back( SAFE_NEW CAirScript() );
	loaded_scripts.push_back( SAFE_NEW CEmptyScript() );
	loaded_scripts.push_back( SAFE_NEW CSpawnScript(false) );
	loaded_scripts.push_back( SAFE_NEW CSpawnScript(true) );
	loaded_scripts.push_back( SAFE_NEW CTestScript() );

	const char *path = "AI/Bot-libs/";
	std::vector<std::string> f = CFileHandler::FindFiles(path, std::string("*.") + SharedLib::GetLibExtension());

	for(std::vector<std::string>::iterator fi = f.begin(), e = f.end(); fi != e; ++fi)
		loaded_scripts.push_back(SAFE_NEW CGlobalAITestScript(*fi));

	f = CFileHandler::FindFiles("Saves/", "*.ssf");
	for(std::vector<std::string>::iterator fi = f.begin(), e = f.end(); fi != e; ++fi) {
		loaded_scripts.push_back(SAFE_NEW CLoadScript(*fi));
	}
	ENTER_UNSYNCED;
}

void CScriptHandler::StartLua()
{
#ifndef NO_LUA
	std::vector<std::string> files = vfsHandler->GetFilesInDir("startscripts");
	for (std::vector<std::string>::iterator i = files.begin(); i != files.end(); ++i) {
		char buffer[16000];
		const int returned = vfsHandler->LoadFile(std::string("startscripts/"+*i), buffer);
		CLuaBinder* lua = SAFE_NEW CLuaBinder();
		if (!lua->LoadScript(*i, buffer, returned))
			handleerror(NULL, lua->lastError.c_str(), "Lua", MBF_OK|MBF_EXCL);
		lua_binders.push_back(lua);
	}
#endif
}

/** @return a reference to the only CScriptHandler instance */
CScriptHandler& CScriptHandler::Instance()
{
	static bool created = false;
	static CScriptHandler instance;
	if( !created ) {
		created = true;
		instance.StartLua();
		instance.LoadScripts();
	}
	return instance;
}

CScriptHandler::~CScriptHandler()
{
	while(!loaded_scripts.empty()) {
		delete loaded_scripts.back();
		loaded_scripts.pop_back();
	}

#ifndef NO_LUA
	while (!lua_binders.empty()) {
		delete lua_binders.back();
		lua_binders.pop_back();
	}
#endif
}

/** Called by the CScript constructors to add themselves to the CScriptHandler. */
void CScriptHandler::AddScript(std::string name, CScript *s)
{
	scripts[name] = s;
}

void CScriptHandler::LoadScriptFile(const std::string& file)
{
	char buffer[16000];
	const int returned = vfsHandler->LoadFile(std::string("startscripts/"+file), buffer);
	if (returned > 0)
	{
		CLuaBinder* lua = SAFE_NEW CLuaBinder();
		if (!lua->LoadScript(file, buffer, returned))
			handleerror(NULL, lua->lastError.c_str(), "Lua", MBF_OK|MBF_EXCL);
		lua_binders.push_back(lua);
	}
	else
		throw std::runtime_error("scriptfile not found: " + file);
}

/** Called by the CglList generated by CScriptHandler::GenList on selection of a script. */
void CScriptHandler::SelectScript(std::string s)
{
	CScriptHandler::Instance().chosenName = s;
	if (CScriptHandler::Instance().scripts.find(s) == CScriptHandler::Instance().scripts.end()) {
		throw std::runtime_error("script not found: " + s);
	}
	CScriptHandler::Instance().chosenScript = CScriptHandler::Instance().scripts[s];
	CScriptHandler::Instance().chosenScript->ScriptSelected();
	if (CScriptHandler::Instance().callback)
		CScriptHandler::Instance().callback(s);
	callback = 0; // invalidate after selection
}

/** Generate a CglList with all available scripts. */
CglList* CScriptHandler::GenList(ListSelectCallback callb)
{
	CglList* list = SAFE_NEW CglList("Select script", SelectScript, 1);
	for (std::map<std::string,CScript*>::const_iterator it = scripts.begin(); it != scripts.end(); ++it)
		list->AddItem(it->first.c_str(), it->first.c_str());
	callback = callb;
	return list;
}
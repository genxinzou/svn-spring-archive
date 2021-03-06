#include "StdAfx.h"
#include "Rendering/GL/myGL.h"
#include <map>
#include <SDL_keysym.h>
#include <SDL_timer.h>
#include <SDL_types.h>
#include <set>
#include <cfloat>

#include "mmgr.h"

#include "PreGame.h"
#include "Game.h"
#include "GameVersion.h"
#include "Sim/Misc/TeamHandler.h"
#include "FPUCheck.h"
#include "GameServer.h"
#include "GameSetup.h"
#include "GameData.h"
#include "Sim/Misc/GlobalSynced.h"
#include "NetProtocol.h"
#include "Net/RawPacket.h"
#include "DemoRecorder.h"
#include "DemoReader.h"
#include "LoadSaveHandler.h"
#include "TdfParser.h"
#include "FileSystem/ArchiveScanner.h"
#include "FileSystem/FileHandler.h"
#include "FileSystem/VFSHandler.h"
#include "Lua/LuaGaia.h"
#include "Lua/LuaRules.h"
#include "Lua/LuaParser.h"
#include "Map/MapParser.h"
#include "ConfigHandler.h"
#include "FileSystem/FileSystem.h"
#include "Rendering/glFont.h"
#include "Rendering/Textures/TAPalette.h"
#include "StartScripts/ScriptHandler.h"
#include "UI/InfoConsole.h"
#include "Exceptions.h"


CPreGame* pregame = NULL;
using netcode::RawPacket;

extern Uint8* keys;
extern bool globalQuit;
std::string stupidGlobalMapname;


CPreGame::CPreGame(const LocalSetup* setup) :
		settings(setup),
		savefile(NULL)
{
	net = SAFE_NEW CNetProtocol();
	activeController=this;

	if(!settings->isHost)
	{
		net->InitClient(settings->hostip.c_str(), settings->hostport, settings->sourceport, settings->myPlayerName, SpringVersion::GetFull());
		timer = SDL_GetTicks();
	}
	else
	{
		net->InitLocalClient();
	}
}


CPreGame::~CPreGame()
{
	// don't delete infoconsole, its beeing reused by CGame
}

void CPreGame::LoadSetupscript(const std::string& script)
{
	assert(settings->isHost);
	StartServer(script);
}

void CPreGame::LoadDemo(const std::string& demo)
{
	assert(settings->isHost);
	ReadDataFromDemo(demo);
}

void CPreGame::LoadSavefile(const std::string& save)
{
	assert(settings->isHost);
	savefile = new CLoadSaveHandler();
	savefile->LoadGameStartInfo(savefile->FindSaveFile(save.c_str()));
	StartServer(savefile->scriptText);
}

int CPreGame::KeyPressed(unsigned short k,bool isRepeat)
{
	if (k == SDLK_ESCAPE){
		if(keys[SDLK_LSHIFT]){
			logOutput.Print("User exited");
			globalQuit=true;
		} else
			logOutput.Print("Use shift-esc to quit");
	}
	return 0;
}


bool CPreGame::Draw()
{
	SDL_Delay(10); // milliseconds
	ClearScreen();

	if (!net->Connected())
	{
		if (settings->isHost)
			font->glPrintCentered (0.5f,0.48f, 2.0f, "Waiting for server to start");
		else
		{
			font->glPrintCentered (0.5f,0.48f, 2.0f, "Connecting to server (%d s)", (SDL_GetTicks()-timer)/1000);
		}
	}
	else
	{
		font->glPrintCentered (0.5f,0.48f, 2.0f, "Waiting for server response");
	}

	font->glFormatAt(0.60f,0.40f, 1.0f, "Server: %s:%d", settings->hostip.c_str(), settings->hostport);
	if (!settings->isHost)
		font->glFormatAt(0.60f,0.35f, 1.0f, "Local endpoint: port %d UDP%s", settings->sourceport, (settings->sourceport == 0) ? " (autoselect)" : "");
	else
		font->glFormatAt (0.60f,0.35f, 1.0f, "Local endpoint: shared memory");

	font->glFormatAt (0.60f,0.30f, 1.0f, "Using playername: %s", settings->myPlayerName.c_str());

	// credits
	font->glPrintCentered(0.5f,0.06f,1.0f,"Spring %s", SpringVersion::GetFull().c_str());
	font->glPrintCentered(0.5f,0.02f,0.6f,"This program is distributed under the GNU General Public License, see license.html for more info");

	return true;
}


bool CPreGame::Update()
{
	good_fpu_control_registers("CPreGame::Update");
	net->Update();
	UpdateClientNet();

	return true;
}


void CPreGame::StartServer(const std::string& setupscript)
{
	assert(!gameServer);
	GameData* startupData = new GameData();
	CGameSetup* setup = new CGameSetup();
	setup->Init(setupscript);
	std::string map = setup->mapName;
	std::string mod = setup->baseMod;
	std::string script = setup->scriptName;

	startupData->SetRandomSeed(static_cast<unsigned>(gu->usRandInt()));
	bool mapHasStartscript = false;
	if (!map.empty())
	{
		// would be better to use MapInfo here, but this doesn't work
		LoadMap(map); // map into VFS
		std::string mapDefFile;
		const std::string extension = map.substr(map.length()-3);
		if (extension == "smf")
			mapDefFile = std::string("maps/")+map.substr(0,map.find_last_of('.'))+".smd";
		else if(extension == "sm3")
			mapDefFile = string("maps/")+map;
		else
			throw std::runtime_error("CPreGame::StartServer(): Unknown extension: " + extension);

		MapParser mp(map);
		LuaTable mapRoot = mp.GetRoot();
		const std::string mapWantedScript = mapRoot.GetString("script",     "");
		const std::string scriptFile      = mapRoot.GetString("scriptFile", "");
		if (!scriptFile.empty()) {
			CScriptHandler::Instance().LoadScriptFile(scriptFile);
		}
		if (!mapWantedScript.empty()) {
			script = mapWantedScript;
			mapHasStartscript = true;
		}
	}
	startupData->SetScript(script);
	// here we now the name of the script to use

	try { // to load the script
		CScriptHandler::SelectScript(script);
		std::string scriptWantedMod;
		scriptWantedMod = CScriptHandler::Instance().chosenScript->GetModName();
		if (!scriptWantedMod.empty()) {
			mod = scriptWantedMod;
		}
		LoadMod(mod);
	}
	catch (const std::runtime_error& err) { // script not found, so it may be in the modarchive?
		LoadMod(mod); // new map into VFS
		CScriptHandler::SelectScript(script);
	}
	// make sure s is a modname (because the same mod can be in different archives on different computers)
	mod = archiveScanner->ModArchiveToModName(mod);
	std::string modArchive = archiveScanner->ModNameToModArchive(mod);
	startupData->SetMod(mod, archiveScanner->GetModChecksum(modArchive));

	if (!mapHasStartscript) {
		std::string mapFromScript = CScriptHandler::Instance().chosenScript->GetMapName();
		if (!mapFromScript.empty() && map != mapFromScript) {
			//TODO unload old map
			LoadMap(mapFromScript, true);
		}
	}
	startupData->SetMap(map, archiveScanner->GetMapChecksum(map));
	setup->LoadStartPositions();

	good_fpu_control_registers("before CGameServer creation");
	startupData->SetSetup(setup->gameSetupText);
	gameServer = new CGameServer(settings.get(), false, startupData, setup);
	gameServer->AddLocalClient(settings->myPlayerName, SpringVersion::GetFull());
	good_fpu_control_registers("after CGameServer creation");
}


void CPreGame::UpdateClientNet()
{
	if (!net->Active())
	{
		logOutput.Print("Server not reachable");
		globalQuit = true;
		return;
	}

	boost::shared_ptr<const RawPacket> packet;
	while ((packet = net->GetData()))
	{
		const unsigned char* inbuf = packet->data;
		switch (inbuf[0]) {
			case NETMSG_GAMEDATA: { // server first sends this to let us know about teams, allyteams etc.
				GameDataReceived(packet);
				break;
			}
			case NETMSG_SETPLAYERNUM: { // this is sent afterwards to let us know which playernum we have
				gu->SetMyPlayer(packet->data[1]);
				logOutput.Print("Became player %i (team %i, allyteam %i)", gu->myPlayerNum, gu->myTeam, gu->myAllyTeam);

				const CTeam* team = teamHandler->Team(gu->myTeam);
				assert(team);
				LoadStartPicture(team->side);

				game = SAFE_NEW CGame(gameData->GetMap(), modArchive, savefile);

				if (savefile) {
					savefile->LoadGame();
				}

				pregame=0;
				delete this;
				return;
			}
			default: {
				logOutput.Print("Unknown net-msg recieved from CPreGame: %i", int(packet->data[0]));
				break;
			}
		}
	}
}

void CPreGame::ReadDataFromDemo(const std::string& demoName)
{
	assert(!gameServer);
	logOutput.Print("Pre-scanning demo file for game data...");
	CDemoReader scanner(demoName, 0);

	boost::shared_ptr<const RawPacket> buf(scanner.GetData(static_cast<float>(FLT_MAX )));
	while ( buf )
	{
		if (buf->data[0] == NETMSG_GAMEDATA)
		{
			GameData *data = new GameData(boost::shared_ptr<const RawPacket>(buf));

			// modify the startscriptscript so it can be used to watch the demo
			TdfParser script(data->GetSetup().c_str(), data->GetSetup().size());
			TdfParser::TdfSection* tgame = script.GetRootSection()->sections["game"];

			tgame->AddPair("ScriptName", data->GetScript());
			tgame->AddPair("MapName", data->GetMap());
			tgame->AddPair("Gametype", data->GetMod());

			tgame->AddPair("Demofile", demoName);

			unsigned numPlayers = 0;
			for (std::map<std::string, TdfParser::TdfSection*>::iterator it = tgame->sections.begin(); it != tgame->sections.end(); ++it) {
				if (it->first.substr(0, 6) == "player")
				{
					it->second->AddPair("isfromdemo", 1);
					++numPlayers;
				}
			}

			// add local spectator (and assert we didn't already have MAX_PLAYERS players)
			char section[50];
			sprintf(section, "PLAYER%i", numPlayers);
			string s(section);
			TdfParser::TdfSection* me = tgame->construct_subsection(s);
			me->AddPair("name", settings->myPlayerName);
			me->AddPair("spectator", 1);

			std::ostringstream buf;
			script.print(buf);
			data->SetSetup(buf.str());
			CGameSetup* tempSetup = new CGameSetup();

			if (!tempSetup->Init(buf.str()))
			{
				throw content_error("Demo contains incorrect script");
			}
			good_fpu_control_registers("before CGameServer creation");
			gameServer = new CGameServer(settings.get(), true, data, tempSetup);
			gameServer->AddLocalClient(settings->myPlayerName, SpringVersion::GetFull());
			good_fpu_control_registers("after CGameServer creation");
			break;
		}

		if (scanner.ReachedEnd())
		{
			throw content_error("End of demo reached and no game data found");
		}
		buf.reset(scanner.GetData(static_cast<float>(FLT_MAX )));
	}

	assert(gameServer);
}

void CPreGame::LoadMap(const std::string& mapName, const bool forceReload)
{
	static bool alreadyLoaded = false;

	if (!alreadyLoaded || forceReload)
	{
		CFileHandler* f = SAFE_NEW CFileHandler("maps/" + mapName);
		if (!f->FileExists()) {
			vector<string> ars = archiveScanner->GetArchivesForMap(mapName);
			if (ars.empty()) {
				throw content_error("Couldn't find any archives for map '" + mapName + "'.");
			}
			for (vector<string>::iterator i = ars.begin(); i != ars.end(); ++i) {
				if (!vfsHandler->AddArchive(*i, false)) {
					throw content_error("Couldn't load archive '" + *i + "' for map '" + mapName + "'.");
				}
			}
		}
		delete f;
		alreadyLoaded = true;
	}
}


void CPreGame::LoadMod(const std::string& modName)
{
	static bool alreadyLoaded = false;

	if (!alreadyLoaded) {
		// Map all required archives depending on selected mod(s)
		std::string modArchive = archiveScanner->ModNameToModArchive(modName);
		vector<string> ars = archiveScanner->GetArchives(modArchive);
		if (ars.empty()) {
			throw content_error("Couldn't find any archives for mod '" + modName + "'");
		}
		for (vector<string>::iterator i = ars.begin(); i != ars.end(); ++i) {

			if (!vfsHandler->AddArchive(*i, false)) {
				throw content_error("Couldn't load archive '" + *i + "' for mod '" + modName + "'.");
			}
		}
		alreadyLoaded = true;
	}
}

void CPreGame::GameDataReceived(boost::shared_ptr<const netcode::RawPacket> packet)
{
	gameData.reset(new GameData(packet));

	CGameSetup* temp = new CGameSetup();
	if (temp->Init(gameData->GetSetup()))
	{
		temp->scriptName = gameData->GetScript();
		temp->mapName = gameData->GetMap();
		temp->baseMod = gameData->GetMod();

		gameSetup = const_cast<const CGameSetup*>(temp);
		gs->LoadFromSetup(gameSetup);
	}
	else
	{
		throw content_error("Server sent us incorrect script");
	}

	gs->SetRandSeed(gameData->GetRandomSeed(), true);
	logOutput << "Using map " << gameData->GetMap() << "\n";
	stupidGlobalMapname = gameData->GetMap();

	if (net && net->GetDemoRecorder()) {
		net->GetDemoRecorder()->SetName(gameData->GetMap());
		logOutput << "Recording demo " << net->GetDemoRecorder()->GetName() << "\n";
	}
	LoadMap(gameData->GetMap());
	archiveScanner->CheckMap(gameData->GetMap(), gameData->GetMapChecksum());

	logOutput << "Using script " << gameData->GetScript() << "\n";
	CScriptHandler::SelectScript(gameData->GetScript());

	logOutput << "Using mod " << gameData->GetMod() << "\n";
	LoadMod(gameData->GetMod());
	modArchive = archiveScanner->ModNameToModArchive(gameData->GetMod());
	logOutput << "Using mod archive " << modArchive << "\n";
	archiveScanner->CheckMod(modArchive, gameData->GetModChecksum());

}

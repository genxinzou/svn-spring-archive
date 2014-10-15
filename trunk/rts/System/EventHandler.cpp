#include "StdAfx.h"
// EventHandler.cpp: implementation of the CEventHandler class.
//
//////////////////////////////////////////////////////////////////////

#include "EventHandler.h"
#include "Lua/LuaOpenGL.h"  // FIXME -- should be moved

using std::string;
using std::vector;
using std::map;


CEventHandler eventHandler;


/******************************************************************************/
/******************************************************************************/

void CEventHandler::SetupEvent(const string& eName,
                               EventClientList* list, int props)
{
	eventMap[eName] = EventInfo(eName, list, props);
}

#define SETUP_EVENT(name, props) SetupEvent(#name, &list ## name, props)


/******************************************************************************/
/******************************************************************************/

CEventHandler::CEventHandler()
{
	mouseOwner = NULL;

	// synced call-ins
	SETUP_EVENT(GamePreload,   MANAGED_BIT);
	SETUP_EVENT(GameStart,     MANAGED_BIT);
	SETUP_EVENT(GameOver,      MANAGED_BIT);
	SETUP_EVENT(TeamDied,      MANAGED_BIT);
	SETUP_EVENT(TeamChanged,   MANAGED_BIT);
	SETUP_EVENT(PlayerChanged, MANAGED_BIT);

	SETUP_EVENT(UnitCreated,     MANAGED_BIT);
	SETUP_EVENT(UnitFinished,    MANAGED_BIT);
	SETUP_EVENT(UnitFromFactory, MANAGED_BIT);
	SETUP_EVENT(UnitDestroyed,   MANAGED_BIT);
	SETUP_EVENT(UnitTaken,       MANAGED_BIT);
	SETUP_EVENT(UnitGiven,       MANAGED_BIT);

	SETUP_EVENT(UnitIdle,       MANAGED_BIT);
	SETUP_EVENT(UnitCommand,    MANAGED_BIT);
	SETUP_EVENT(UnitCmdDone,    MANAGED_BIT);
	SETUP_EVENT(UnitDamaged,    MANAGED_BIT);
	SETUP_EVENT(UnitExperience, MANAGED_BIT);

	SETUP_EVENT(UnitSeismicPing,  MANAGED_BIT);
	SETUP_EVENT(UnitEnteredRadar, MANAGED_BIT);
	SETUP_EVENT(UnitEnteredLos,   MANAGED_BIT);
	SETUP_EVENT(UnitLeftRadar,    MANAGED_BIT);
	SETUP_EVENT(UnitLeftLos,      MANAGED_BIT);

	SETUP_EVENT(UnitEnteredWater, MANAGED_BIT);
	SETUP_EVENT(UnitEnteredAir,   MANAGED_BIT);
	SETUP_EVENT(UnitLeftWater,    MANAGED_BIT);
	SETUP_EVENT(UnitLeftAir,      MANAGED_BIT);

	SETUP_EVENT(UnitLoaded,     MANAGED_BIT);
	SETUP_EVENT(UnitUnloaded,   MANAGED_BIT);
	SETUP_EVENT(UnitCloaked,    MANAGED_BIT);
	SETUP_EVENT(UnitDecloaked,  MANAGED_BIT);

	SETUP_EVENT(UnitMoveFailed, MANAGED_BIT);

	SETUP_EVENT(FeatureCreated,   MANAGED_BIT);
	SETUP_EVENT(FeatureDestroyed, MANAGED_BIT);

	SETUP_EVENT(ProjectileCreated,   MANAGED_BIT);
	SETUP_EVENT(ProjectileDestroyed, MANAGED_BIT);

	SETUP_EVENT(Explosion, MANAGED_BIT);

	SETUP_EVENT(StockpileChanged, MANAGED_BIT);

	// unsynced call-ins
	SETUP_EVENT(Update,         MANAGED_BIT | UNSYNCED_BIT);

	SETUP_EVENT(KeyPress,       MANAGED_BIT | UNSYNCED_BIT);
	SETUP_EVENT(KeyRelease,     MANAGED_BIT | UNSYNCED_BIT);
	SETUP_EVENT(MouseMove,      MANAGED_BIT | UNSYNCED_BIT);
	SETUP_EVENT(MousePress,     MANAGED_BIT | UNSYNCED_BIT);
	SETUP_EVENT(MouseRelease,   MANAGED_BIT | UNSYNCED_BIT);
	SETUP_EVENT(MouseWheel,     MANAGED_BIT | UNSYNCED_BIT);
	SETUP_EVENT(IsAbove,        MANAGED_BIT | UNSYNCED_BIT);
	SETUP_EVENT(GetTooltip,     MANAGED_BIT | UNSYNCED_BIT);

	SETUP_EVENT(DefaultCommand, MANAGED_BIT | UNSYNCED_BIT);
	SETUP_EVENT(CommandNotify,  MANAGED_BIT | UNSYNCED_BIT);
	SETUP_EVENT(AddConsoleLine, MANAGED_BIT | UNSYNCED_BIT);
	SETUP_EVENT(GroupChanged,   MANAGED_BIT | UNSYNCED_BIT);
	SETUP_EVENT(GameSetup,      MANAGED_BIT | UNSYNCED_BIT);
	SETUP_EVENT(WorldTooltip,   MANAGED_BIT | UNSYNCED_BIT);
	SETUP_EVENT(MapDrawCmd,     MANAGED_BIT | UNSYNCED_BIT);

	SETUP_EVENT(ViewResize,     MANAGED_BIT | UNSYNCED_BIT);

	SETUP_EVENT(DrawGenesis,         MANAGED_BIT | UNSYNCED_BIT);
	SETUP_EVENT(DrawWorld,           MANAGED_BIT | UNSYNCED_BIT);
	SETUP_EVENT(DrawWorldPreUnit,    MANAGED_BIT | UNSYNCED_BIT);
	SETUP_EVENT(DrawWorldShadow,     MANAGED_BIT | UNSYNCED_BIT);
	SETUP_EVENT(DrawWorldReflection, MANAGED_BIT | UNSYNCED_BIT);
	SETUP_EVENT(DrawWorldRefraction, MANAGED_BIT | UNSYNCED_BIT);
	SETUP_EVENT(DrawScreenEffects,   MANAGED_BIT | UNSYNCED_BIT);
	SETUP_EVENT(DrawScreen,          MANAGED_BIT | UNSYNCED_BIT);
	SETUP_EVENT(DrawInMiniMap,       MANAGED_BIT | UNSYNCED_BIT);

	// unmanaged call-ins
	SetupEvent("RecvLuaMsg", NULL, 0);

	SetupEvent("AICallIn", NULL, UNSYNCED_BIT);
	SetupEvent("DrawUnit", NULL, UNSYNCED_BIT);

	// LuaRules
	SetupEvent("CommandFallback",        NULL, CONTROL_BIT);
	SetupEvent("AllowCommand",           NULL, CONTROL_BIT);
	SetupEvent("AllowUnitCreation",      NULL, CONTROL_BIT);
	SetupEvent("AllowUnitTransfer",      NULL, CONTROL_BIT);
	SetupEvent("AllowUnitBuildStep",     NULL, CONTROL_BIT);
	SetupEvent("AllowFeatureCreation",   NULL, CONTROL_BIT);
	SetupEvent("AllowFeatureBuildStep",  NULL, CONTROL_BIT);
	SetupEvent("AllowResourceLevel",     NULL, CONTROL_BIT);
	SetupEvent("AllowResourceTransfer",  NULL, CONTROL_BIT);
	SetupEvent("AllowDirectUnitControl", NULL, CONTROL_BIT);
	SetupEvent("AllowStartPosition",     NULL, CONTROL_BIT);
	SetupEvent("TerraformComplete",      NULL, CONTROL_BIT);
	SetupEvent("MoveCtrlNotify",         NULL, CONTROL_BIT);
}


CEventHandler::~CEventHandler()
{
}


/******************************************************************************/
/******************************************************************************/

void CEventHandler::AddClient(CEventClient* ec)
{
	ListInsert(handles, ec);

	EventMap::const_iterator it;
	for (it = eventMap.begin(); it != eventMap.end(); ++it) {
		const EventInfo& ei = it->second;
		if (ei.HasPropBit(MANAGED_BIT) && (ei.GetList() != NULL)) {
			if (ec->WantsEvent(it->first)) {
				ListInsert(*ei.GetList(), ec);
			}
		}
	}
}


void CEventHandler::RemoveClient(CEventClient* ec)
{
	if (mouseOwner == ec) {
		mouseOwner = NULL;
	}

	ListRemove(handles, ec);

	EventMap::const_iterator it;
	for (it = eventMap.begin(); it != eventMap.end(); ++it) {
		const EventInfo& ei = it->second;
		if (ei.HasPropBit(MANAGED_BIT) && (ei.GetList() != NULL)) {
			ListRemove(*ei.GetList(), ec);
		}
	}
}


/******************************************************************************/
/******************************************************************************/

void CEventHandler::GetEventList(vector<string>& list) const
{
	list.clear();
	EventMap::const_iterator it;
	for (it = eventMap.begin(); it != eventMap.end(); ++it) {
		list.push_back(it->first);
	}
}


bool CEventHandler::IsKnown(const string& eName) const
{
	return (eventMap.find(eName) != eventMap.end());
}


bool CEventHandler::IsManaged(const string& eName) const
{
	EventMap::const_iterator it = eventMap.find(eName);
	return ((it != eventMap.end()) && (it->second.HasPropBit(MANAGED_BIT)));
}


bool CEventHandler::IsUnsynced(const string& eName) const
{
	EventMap::const_iterator it = eventMap.find(eName);
	return ((it != eventMap.end()) && (it->second.HasPropBit(UNSYNCED_BIT)));
}


bool CEventHandler::IsController(const string& eName) const
{
	EventMap::const_iterator it = eventMap.find(eName);
	return ((it != eventMap.end()) && (it->second.HasPropBit(CONTROL_BIT)));
}


/******************************************************************************/

bool CEventHandler::InsertEvent(CEventClient* ec, const string& ciName)
{
	EventMap::iterator it = eventMap.find(ciName);
	if ((it == eventMap.end()) || (it->second.GetList() == NULL)) {
		return false;
	}
	if (ec->GetSynced() && it->second.HasPropBit(UNSYNCED_BIT)) {
		return false;
	}
	ListInsert(*it->second.GetList(), ec);
	return true;
}


bool CEventHandler::RemoveEvent(CEventClient* ec, const string& ciName)
{
	EventMap::iterator it = eventMap.find(ciName);
	if ((it == eventMap.end()) || (it->second.GetList() == NULL)) {
		return false;
	}
	ListRemove(*it->second.GetList(), ec);
	return true;
}


/******************************************************************************/

void CEventHandler::ListInsert(EventClientList& ecList, CEventClient* ec)
{
	EventClientList::iterator it;
	for (it = ecList.begin(); it != ecList.end(); ++it) {
		const CEventClient* ecIt = *it;
		if (ec == ecIt) {
			return; // already in the list
		}
		else if ((ec->GetOrder()  <  ecIt->GetOrder()) ||
		         ((ec->GetOrder() == ecIt->GetOrder()) &&
		          (ec->GetName()  <  ecIt->GetName()))) { // should not happen
			ecList.insert(it, ec);
			return;
		}
	}
	ecList.push_back(ec);
}


void CEventHandler::ListRemove(EventClientList& ecList, CEventClient* ec)
{
	// FIXME: efficient, hardly
	EventClientList newList;
	for (int i = 0; i < ecList.size(); i++) {
		if (ec != ecList[i]) {
			newList.push_back(ecList[i]);
		}
	}
	ecList = newList;
}


/******************************************************************************/
/******************************************************************************/

void CEventHandler::GamePreload()
{
	const int count = listGamePreload.size();
	for (int i = 0; i < count; i++) {
		CEventClient* ec = listGamePreload[i];
		ec->GamePreload();
	}
}

void CEventHandler::GameStart()
{
	const int count = listGameStart.size();
	for (int i = 0; i < count; i++) {
		CEventClient* ec = listGameStart[i];
		ec->GameStart();
	}
}

void CEventHandler::GameOver()
{
	const int count = listGameOver.size();
	for (int i = 0; i < count; i++) {
		CEventClient* ec = listGameOver[i];
		ec->GameOver();
	}
}


void CEventHandler::TeamDied(int teamID)
{
	const int count = listTeamDied.size();
	for (int i = 0; i < count; i++) {
		CEventClient* ec = listTeamDied[i];
		ec->TeamDied(teamID);
	}
}


void CEventHandler::TeamChanged(int teamID)
{
	const int count = listTeamChanged.size();
	for (int i = 0; i < count; i++) {
		CEventClient* ec = listTeamChanged[i];
		ec->TeamChanged(teamID);
	}
}


void CEventHandler::PlayerChanged(int playerID)
{
	const int count = listPlayerChanged.size();
	for (int i = 0; i < count; i++) {
		CEventClient* ec = listPlayerChanged[i];
		ec->PlayerChanged(playerID);
	}
}


/******************************************************************************/
/******************************************************************************/

void CEventHandler::Update()
{
	const int count = listUpdate.size();
	for (int i = 0; i < count; i++) {
		CEventClient* ec = listUpdate[i];
		ec->Update();
	}
}


void CEventHandler::ViewResize()
{
	const int count = listViewResize.size();
	for (int i = 0; i < count; i++) {
		CEventClient* ec = listViewResize[i];
		ec->ViewResize();
	}
}


#define DRAW_CALLIN(name)                         \
  void CEventHandler:: Draw ## name ()        \
  {                                               \
    const int count = listDraw ## name.size();    \
    if (count <= 0) {                             \
      return;                                     \
    }                                             \
                                                  \
    LuaOpenGL::EnableDraw ## name ();             \
    listDraw ## name [0]->Draw ## name ();        \
                                                  \
    for (int i = 1; i < count; i++) {             \
      LuaOpenGL::ResetDraw ## name ();            \
      CEventClient* ec = listDraw ## name [i];      \
      ec-> Draw ## name ();                       \
    }                                             \
                                                  \
    LuaOpenGL::DisableDraw ## name ();            \
  }

DRAW_CALLIN(Genesis)
DRAW_CALLIN(World)
DRAW_CALLIN(WorldPreUnit)
DRAW_CALLIN(WorldShadow)
DRAW_CALLIN(WorldReflection)
DRAW_CALLIN(WorldRefraction)
DRAW_CALLIN(ScreenEffects)
DRAW_CALLIN(Screen)
DRAW_CALLIN(InMiniMap)


/******************************************************************************/
/******************************************************************************/

bool CEventHandler::CommandNotify(const Command& cmd)
{
	// reverse order, user has the override
	const int count = listCommandNotify.size();
	for (int i = (count - 1); i >= 0; i--) {
		CEventClient* ec = listCommandNotify[i];
		if (ec->CommandNotify(cmd)) {
			return true;
		}
	}
	return false;
}


bool CEventHandler::KeyPress(unsigned short key, bool isRepeat)
{
	// reverse order, user has the override
	const int count = listKeyPress.size();
	for (int i = (count - 1); i >= 0; i--) {
		CEventClient* ec = listKeyPress[i];
		if (ec->KeyPress(key, isRepeat)) {
			return true;
		}
	}
	return false;
}


bool CEventHandler::KeyRelease(unsigned short key)
{
	// reverse order, user has the override
	const int count = listKeyRelease.size();
	for (int i = (count - 1); i >= 0; i--) {
		CEventClient* ec = listKeyRelease[i];
		if (ec->KeyRelease(key)) {
			return true;
		}
	}
	return false;
}


bool CEventHandler::MousePress(int x, int y, int button)
{
	// reverse order, user has the override
	const int count = listMousePress.size();
	for (int i = (count - 1); i >= 0; i--) {
		CEventClient* ec = listMousePress[i];
		if (ec->MousePress(x, y, button)) {
			mouseOwner = ec;
			return true;
		}
	}
	return false;
}


// return a cmd index, or -1
int CEventHandler::MouseRelease(int x, int y, int button)
{
	if (mouseOwner == NULL) {
		return -1;
	}
	const int retval = mouseOwner->MouseRelease(x, y, button);
	mouseOwner = NULL;
	return retval;
}


bool CEventHandler::MouseMove(int x, int y, int dx, int dy, int button)
{
	if (mouseOwner == NULL) {
		return false;
	}
	return mouseOwner->MouseMove(x, y, dx, dy, button);
}


bool CEventHandler::MouseWheel(bool up, float value)
{
	// reverse order, user has the override
	const int count = listMouseWheel.size();
	for (int i = (count - 1); i >= 0; i--) {
		CEventClient* ec = listMouseWheel[i];
		if (ec->MouseWheel(up, value)) {
			return true;
		}
	}
	return false;
}


bool CEventHandler::IsAbove(int x, int y)
{
	// reverse order, user has the override
	const int count = listIsAbove.size();
	for (int i = (count - 1); i >= 0; i--) {
		CEventClient* ec = listIsAbove[i];
		if (ec->IsAbove(x, y)) {
			return true;
		}
	}
	return false;
}


string CEventHandler::GetTooltip(int x, int y)
{
	// reverse order, user has the override
	const int count = listGetTooltip.size();
	for (int i = (count - 1); i >= 0; i--) {
		CEventClient* ec = listGetTooltip[i];
		const string tt = ec->GetTooltip(x, y);
		if (!tt.empty()) {
			return tt;
		}
	}
	return "";
}


bool CEventHandler::AddConsoleLine(const string& msg, CLogSubsystem& subsystem)
{
	const int count = listAddConsoleLine.size();
	for (int i = 0; i < count; i++) {
		CEventClient* ec = listAddConsoleLine[i];
		ec->AddConsoleLine(msg, subsystem);
	}
	return true;
}


bool CEventHandler::GroupChanged(int groupID)
{
	const int count = listGroupChanged.size();
	for (int i = 0; i < count; i++) {
		CEventClient* ec = listGroupChanged[i];
		ec->GroupChanged(groupID);
	}
	return false;
}



bool CEventHandler::GameSetup(const string& state, bool& ready,
                                  const map<int, string>& playerStates)
{
	// reverse order, user has the override
	const int count = listGameSetup.size();
	for (int i = (count - 1); i >= 0; i--) {
		CEventClient* ec = listGameSetup[i];
		if (ec->GameSetup(state, ready, playerStates)) {
			return true;
		}
	}
	return false;
}


string CEventHandler::WorldTooltip(const CUnit* unit,
                                   const CFeature* feature,
                                   const float3* groundPos)
{
	// reverse order, user has the override
	const int count = listWorldTooltip.size();
	for (int i = (count - 1); i >= 0; i--) {
		CEventClient* ec = listWorldTooltip[i];
		const string tt = ec->WorldTooltip(unit, feature, groundPos);
		if (!tt.empty()) {
			return tt;
		}
	}
	return "";
}


bool CEventHandler::MapDrawCmd(int playerID, int type,
                               const float3* pos0, const float3* pos1,
                                   const string* label)
{
	// reverse order, user has the override
	const int count = listMapDrawCmd.size();
	for (int i = (count - 1); i >= 0; i--) {
		CEventClient* ec = listMapDrawCmd[i];
		if (ec->MapDrawCmd(playerID, type, pos0, pos1, label)) {
			return true;
		}
	}
	return false;
}


/******************************************************************************/
/******************************************************************************/
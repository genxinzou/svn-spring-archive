// Generalized callback interface - shared between global AI and group AI
#include "StdAfx.h"
#include "Rendering/GL/myGL.h"
#include "FileSystem/FileHandler.h"
#include "Game/Camera/CameraController.h"
#include "Game/Camera.h"
#include "Game/CameraHandler.h"
#include "Game/GameHelper.h"
#include "Game/GameSetup.h"
#include "Game/PlayerHandler.h"
#include "Game/SelectedUnits.h"
#include "Sim/Misc/TeamHandler.h"
#include "Game/UI/MiniMap.h"
#include "Game/UI/MouseHandler.h"
#include "Lua/LuaRules.h"
#include "Map/MapInfo.h"
#include "Map/MetalMap.h"
#include "Map/ReadMap.h"
#include "NetProtocol.h"
#include "ConfigHandler.h"
#include "Platform/errorhandler.h"
#include "FileSystem/FileSystem.h"
#include "Rendering/InMapDraw.h"
#include "Rendering/UnitModels/3DModel.h"
#include "Rendering/UnitModels/UnitDrawer.h"
#include "Sim/Features/Feature.h"
#include "Sim/Features/FeatureHandler.h"
#include "Sim/Misc/DamageArrayHandler.h"
#include "Sim/Misc/GroundBlockingObjectMap.h"
#include "Sim/Misc/GeometricObjects.h"
#include "Sim/Misc/LosHandler.h"
#include "Sim/Misc/QuadField.h"
#include "Sim/Misc/ModInfo.h"
#include "Sim/Path/PathManager.h"
#include "Sim/Units/CommandAI/CommandAI.h"
#include "Sim/Units/CommandAI/LineDrawer.h"
#include "Sim/Units/UnitTypes/Factory.h"
#include "Sim/Units/UnitDefHandler.h"
#include "Sim/Units/Unit.h"
#include "Sim/Units/UnitHandler.h"
#include "Sim/Weapons/WeaponDefHandler.h"
#include "Sim/Weapons/Weapon.h"
#include "AICheats.h"
#include "GlobalAICallback.h"
#include "GlobalAI.h"
#include "GlobalAIHandler.h"
#include "Group.h"
#include "GroupHandler.h"
#include "IGroupAI.h"
#include "LogOutput.h"
#include "mmgr.h"

/* Cast id to unsigned to catch negative ids in the same operations,
cast MAX_* to unsigned to suppress GCC comparison between signed/unsigned warning. */
#define CHECK_UNITID(id) ((unsigned)(id) < (unsigned)MAX_UNITS)
#define CHECK_GROUPID(id) ((unsigned)(id) < (unsigned)gh->groups.size())
/* With some hacking you can raise an abort (assert) instead of ignoring the id, */
//#define CHECK_UNITID(id) (assert(id > 0 && id < MAX_UNITS), true)
/* ...or disable the check altogether for release... */
//#define CHECK_UNITID(id) true

CAICallback::CAICallback(int Team, CGroupHandler *ghandler)
: team(Team), noMessages(false), gh(ghandler), group (0)
{}

CAICallback::~CAICallback(void)
{}

void CAICallback::SendStartPos(bool ready, float3 startPos)
{
	if(startPos.z<gameSetup->allyStartingData[gu->myAllyTeam].startRectTop *gs->mapy*8)
		startPos.z=gameSetup->allyStartingData[gu->myAllyTeam].startRectTop*gs->mapy*8;

	if(startPos.z>gameSetup->allyStartingData[gu->myAllyTeam].startRectBottom*gs->mapy*8)
		startPos.z=gameSetup->allyStartingData[gu->myAllyTeam].startRectBottom*gs->mapy*8;

	if(startPos.x<gameSetup->allyStartingData[gu->myAllyTeam].startRectLeft*gs->mapx*8)
		startPos.x=gameSetup->allyStartingData[gu->myAllyTeam].startRectLeft*gs->mapx*8;

	if(startPos.x>gameSetup->allyStartingData[gu->myAllyTeam].startRectRight*gs->mapx*8)
		startPos.x=gameSetup->allyStartingData[gu->myAllyTeam].startRectRight*gs->mapx*8;

	unsigned char readyness = ready? 1: 0;
	net->Send(CBaseNetProtocol::Get().SendStartPos(gu->myPlayerNum, team, readyness, startPos.x, startPos.y, startPos.z));
}

void CAICallback::SendTextMsg(const char* text, int zone)
{
	if (group)
		logOutput.Print("Group%i: %s", group->id, text);
	else
		logOutput.Print("GlobalAI%i: %s", team, text);
}

void CAICallback::SetLastMsgPos(float3 pos)
{
	logOutput.SetLastMsgPos(pos);
}

void CAICallback::AddNotification(float3 pos, float3 color, float alpha)
{
	minimap->AddNotification(pos,color,alpha);
}



bool CAICallback::SendResources(float mAmount, float eAmount, int receivingTeamId)
{
	typedef unsigned char ubyte;
	bool ret = false;

	if (team != receivingTeamId) {
		if (receivingTeamId >= 0 && receivingTeamId < (MAX_TEAMS - 1)) {
			if (teamHandler->Team(receivingTeamId) && teamHandler->Team(team)) {
				if (!teamHandler->Team(receivingTeamId)->isDead && !teamHandler->Team(team)->isDead) {
					// note: we can't use the existing SendShare()
					// since its handler in CGame uses myPlayerNum
					// (NETMSG_SHARE param) to determine which team
					// the resources came from, which is not always
					// our AI team
					ret = true;

					// cap the amounts to how much M and E we have
					mAmount = std::max(0.0f, std::min(mAmount, GetMetal()));
					eAmount = std::max(0.0f, std::min(eAmount, GetEnergy()));
					std::vector<short> empty;

					net->Send(CBaseNetProtocol::Get().SendAIShare(ubyte(gu->myPlayerNum), ubyte(team), ubyte(receivingTeamId), mAmount, eAmount, empty));
				}
			}
		}
	}

	return ret;
}

int CAICallback::SendUnits(const std::vector<int>& unitIds, int receivingTeamId)
{
	typedef unsigned char ubyte;
	std::vector<short> sentUnitIDs;

	if (team != receivingTeamId) {
		if (receivingTeamId >= 0 && receivingTeamId < (MAX_TEAMS - 1)) {
			if (teamHandler->Team(receivingTeamId) && teamHandler->Team(team)) {
				if (!teamHandler->Team(receivingTeamId)->isDead && !teamHandler->Team(team)->isDead) {
					// we must iterate over the ID's to check if
					// all of them really belong to the AI's team
					for (std::vector<int>::const_iterator it = unitIds.begin(); it != unitIds.end(); it++ ) {
						const int unitID = *it;

						if (unitID > 0 && unitID < MAX_UNITS) {
							CUnit* unit = uh->units[unitID];

							if (unit && unit->team == team) {
								// we own this unit, save it (note: safe cast
								// since MAX_UNITS currently fits in a short)
								sentUnitIDs.push_back(short(unitID));

								// stop whatever this unit is doing
								Command c;
								c.id = CMD_STOP;
								GiveOrder(unitID, &c);
							}
						}
					}

					if (sentUnitIDs.size() > 0) {
						// we can't use SendShare() here either, since
						// AI's don't have a notion of "selected units"
						net->Send(CBaseNetProtocol::Get().SendAIShare(ubyte(gu->myPlayerNum), ubyte(team), ubyte(receivingTeamId), 0.0f, 0.0f, sentUnitIDs));
					}
				}
			}
		}
	}

	// return how many units were actually put up for transfer
	return (sentUnitIDs.size());
}



bool CAICallback::PosInCamera(float3 pos, float radius)
{
	return camera->InView(pos,radius);
}

// see if the AI hasn't modified any parts of this callback
// (still completely insecure ofcourse, but it filters out the easiest way of cheating)
void CAICallback::verify()
{
	CGlobalAI *gai = globalAI->ais [team];
	if (gai && (((group && group->handler != /*gai->gh*/grouphandlers[gai->team] /*&& group->handler != grouphandler*/) || gai->team != team))) {
		handleerror (0, "AI has modified spring components(possible cheat)", "Spring is closing:", MBF_OK | MBF_EXCL);
		exit (-1);
	}
}

int CAICallback::GetCurrentFrame()
{
	return gs->frameNum;
}

int CAICallback::GetMyTeam()
{
	return team;
}

int CAICallback::GetMyAllyTeam()
{
	return teamHandler->AllyTeam(team);
}

int CAICallback::GetPlayerTeam(int playerId)
{
	CPlayer* pl = playerHandler->Player(playerId);
	if (pl->spectator)
		return -1;
	return pl->team;
}

const char* CAICallback::GetTeamSide(int teamId)
{
	if (teamId < teamHandler->ActiveTeams() && gameSetup) {
		return teamHandler->Team(teamId)->side.c_str();
	} else {
		// if this is not a GameSetup-type game but a
		// GlobalAI-test one, the side-strings for all
		// active teams (0, 1, 2) will always be "arm"
		// since CGlobalAITestScript does not override
		// the CTeam defaults (unlike CGameSetup), so
		// return 0 or AI's that rely on this function
		// will break in GlobalAI tests
		return 0;
	}
}

void* CAICallback::CreateSharedMemArea(char* name, int size)
{
	return globalAI->GetAIBuffer(team,name,size);
}

void CAICallback::ReleasedSharedMemArea(char* name)
{
	globalAI->ReleaseAIBuffer(team,name);
}

int CAICallback::CreateGroup(const char* libraryName, unsigned aiNumber)
{
	AIKey key;
	key.dllName=libraryName;
	key.aiNumber=aiNumber;
	CGroup* g=gh->CreateNewGroup(key);
	return g->id;
}

void CAICallback::EraseGroup(int groupId)
{
	if (CHECK_GROUPID(groupId)) {
		if(gh->groups[groupId])
			gh->RemoveGroup(gh->groups[groupId]);
	}
}

bool CAICallback::AddUnitToGroup(int unitId, int groupId)
{
	if (CHECK_UNITID(unitId) && CHECK_GROUPID(groupId)) {
		CUnit* u=uh->units[unitId];
		if(u && u->team==team && gh->groups[groupId]){
			return u->SetGroup(gh->groups[groupId]);
		}
	}
	return false;
}

bool CAICallback::RemoveUnitFromGroup(int unitId)
{
	if (CHECK_UNITID(unitId)) {
		CUnit* u=uh->units[unitId];
		if(u && u->team==team){
			u->SetGroup(0);
			return true;
		}
	}
	return false;
}

int CAICallback::GetUnitGroup(int unitId)
{
	if (CHECK_UNITID(unitId)) {
		CUnit *unit = uh->units[unitId];
		if (unit && unit->team == team) {
			CGroup* g=uh->units[unitId]->group;
			if(g)
				return g->id;
		}
	}
	return -1;
}

const std::vector<CommandDescription>* CAICallback::GetGroupCommands(int groupId)
{
	static std::vector<CommandDescription> tempcmds;

	if (CHECK_GROUPID(groupId)) {
		if(gh->groups[groupId] && gh->groups[groupId]->ai)
			return &gh->groups[groupId]->ai->GetPossibleCommands();
	}
	return &tempcmds;
}

int CAICallback::GiveGroupOrder(int groupId, Command* c)
{
	if (CHECK_GROUPID(groupId) && c != NULL) {
		if(gh->groups[groupId] && gh->groups[groupId]->ai)
			gh->groups[groupId]->ai->GiveCommand(c);
	}
	return 0;
}

int CAICallback::GiveOrder(int unitId, Command* c)
{
	verify ();

	if (!CHECK_UNITID(unitId) || c == NULL)
		return -1;

	if (noMessages)
		return -1;

	CUnit *unit = uh->units[unitId];

	if (!unit)
		return -1;

	if (group && unit->group != group)
		return -1;

	if (unit->team != team)
		return -1;

	net->Send(CBaseNetProtocol::Get().SendAICommand(gu->myPlayerNum, unitId, c->id, c->options, c->params));
	return 0;
}

const vector<CommandDescription>* CAICallback::GetUnitCommands(int unitId)
{
	if (CHECK_UNITID(unitId)) {
		CUnit *unit = uh->units[unitId];
		if (unit && unit->team == team)
			return &unit->commandAI->possibleCommands;
	}
	return 0;
}

const CCommandQueue* CAICallback::GetCurrentUnitCommands(int unitId)
{
	if (CHECK_UNITID(unitId)) {
		CUnit *unit = uh->units[unitId];
		if (unit && unit->team == team)
			return &unit->commandAI->commandQue;
	}
	return 0;
}

int CAICallback::GetUnitAiHint(int unitId)
{
	verify ();
	if (CHECK_UNITID(unitId)) {
		CUnit* unit = uh->units[unitId];
		if (unit) {
			const int allyTeam = teamHandler->AllyTeam(team);
			if (teamHandler->Ally(unit->allyteam, allyTeam)) {
				return unit->aihint;
			}
			else if (unit->losStatus[allyTeam] & LOS_INLOS) {
				const UnitDef* unitDef = unit->unitDef;
				const UnitDef* decoyDef = unitDef->decoyDef;
				if (decoyDef == NULL) {
					return unit->aihint;
				} else {
					return decoyDef->aihint;
				}
			}
		}
	}
	return 0;
}

int CAICallback::GetUnitTeam(int unitId)
{
	verify ();
	if (CHECK_UNITID(unitId)) {
		CUnit* unit=uh->units[unitId];
		if(unit && (unit->losStatus[teamHandler->AllyTeam(team)] & LOS_INLOS)){
			return unit->team;
		}
	}
	return 0;
}

int CAICallback::GetUnitAllyTeam(int unitId)
{
	verify ();
	if (CHECK_UNITID(unitId)) {
		CUnit* unit=uh->units[unitId];
		if(unit && (unit->losStatus[teamHandler->AllyTeam(team)] & LOS_INLOS)){
			return unit->allyteam;
		}
	}
	return 0;
}

float CAICallback::GetUnitHealth(int unitId)
{
	verify();
	if (CHECK_UNITID(unitId)) {
		CUnit* unit = uh->units[unitId];

		if (unit) {
			const int allyTeam = teamHandler->AllyTeam(team);

			if (teamHandler->Ally(unit->allyteam, allyTeam)) {
				return unit->health;
			}
			else if (unit->losStatus[allyTeam] & LOS_INLOS) {
				const UnitDef* unitDef = unit->unitDef;
				const UnitDef* decoyDef = unitDef->decoyDef;

				if (decoyDef == NULL) {
					return unit->health;
				} else {
					const float scale = (decoyDef->health / unitDef->health);
					return (unit->health * scale);
				}
			}
		}
	}

	return 0;
}

float CAICallback::GetUnitMaxHealth(int unitId)		//the units max health
{
	verify ();
	if (CHECK_UNITID(unitId)) {
		CUnit* unit = uh->units[unitId];
		if (unit) {
			const int allyTeam = teamHandler->AllyTeam(team);
			if (teamHandler->Ally(unit->allyteam, allyTeam)) {
				return unit->maxHealth;
			}
			else if (unit->losStatus[allyTeam] & LOS_INLOS) {
				const UnitDef* unitDef = unit->unitDef;
				const UnitDef* decoyDef = unitDef->decoyDef;
				if (decoyDef == NULL) {
					return unit->maxHealth;
				} else {
					const float scale = (decoyDef->health / unitDef->health);
					return (unit->maxHealth * scale);
				}
			}
		}
	}
	return 0;
}

float CAICallback::GetUnitSpeed(int unitId)				//the units max speed
{
	verify ();
	if (CHECK_UNITID(unitId)) {
		CUnit* unit = uh->units[unitId];
		if (unit) {
			const int allyTeam = teamHandler->AllyTeam(team);
			if (teamHandler->Ally(unit->allyteam, allyTeam)) {
				return unit->maxSpeed;
			}
			else if (unit->losStatus[allyTeam] & LOS_INLOS) {
				const UnitDef* unitDef = unit->unitDef;
				const UnitDef* decoyDef = unitDef->decoyDef;
				if (decoyDef == NULL) {
					return unitDef->speed;
				} else {
					return decoyDef->speed;
				}
			}
		}
	}
	return 0;
}

float CAICallback::GetUnitPower(int unitId)				//sort of the measure of the units overall power
{
	verify ();
	if (CHECK_UNITID(unitId)) {
		CUnit* unit = uh->units[unitId];
		if (unit) {
			const int allyTeam = teamHandler->AllyTeam(team);
			if (teamHandler->Ally(unit->allyteam, allyTeam)) {
				return unit->power;
			}
			else if (unit->losStatus[allyTeam] & LOS_INLOS) {
				const UnitDef* unitDef = unit->unitDef;
				const UnitDef* decoyDef = unitDef->decoyDef;
				if (decoyDef == NULL) {
					return unit->power;
				} else {
					const float scale = (decoyDef->power / unitDef->power);
					return (unit->power * scale);
				}
			}
		}
	}
	return 0;
}

float CAICallback::GetUnitExperience(int unitId)	//how experienced the unit is (0.0-1.0)
{
	verify ();
	if (CHECK_UNITID(unitId)) {
		CUnit* unit=uh->units[unitId];
		if (unit && (unit->losStatus[teamHandler->AllyTeam(team)] & LOS_INLOS)) {
			return unit->experience;
		}
	}
	return 0;
}

float CAICallback::GetUnitMaxRange(int unitId)		//the furthest any weapon of the unit can fire
{
	verify ();
	if (CHECK_UNITID(unitId)) {
		CUnit* unit = uh->units[unitId];
		if (unit) {
			const int allyTeam = teamHandler->AllyTeam(team);
			if (teamHandler->Ally(unit->allyteam, allyTeam)) {
				return unit->maxRange;
			}
			else if (unit->losStatus[allyTeam] & LOS_INLOS) {
				const UnitDef* unitDef = unit->unitDef;
				const UnitDef* decoyDef = unitDef->decoyDef;
				if (decoyDef == NULL) {
					return unit->maxRange;
				} else {
					return decoyDef->maxWeaponRange;
				}
			}
		}
	}
	return 0;
}

const UnitDef* CAICallback::GetUnitDef(int unitId)
{
	verify ();
	if (CHECK_UNITID(unitId)) {
		CUnit* unit = uh->units[unitId];
		if (unit) {
			const UnitDef* unitDef = unit->unitDef;
			const int allyTeam = teamHandler->AllyTeam(team);
			if (teamHandler->Ally(unit->allyteam, allyTeam)) {
				return unitDef;
			}
			const unsigned short losStatus = unit->losStatus[allyTeam];
			const unsigned short prevMask = (LOS_PREVLOS | LOS_CONTRADAR);
			if (((losStatus & LOS_INLOS) != 0) ||
					((losStatus & prevMask) == prevMask)) {
				const UnitDef* decoyDef = unitDef->decoyDef;
				if (decoyDef == NULL) {
					return unitDef;
				} else {
					return decoyDef;
				}
			}
		}
	}
	return 0;
}

const UnitDef* CAICallback::GetUnitDef(const char* unitName)
{
	return unitDefHandler->GetUnitByName(unitName);
}
const UnitDef* CAICallback::GetUnitDefById (int unitDefId)
{
	return unitDefHandler->GetUnitByID(unitDefId);
}

float3 CAICallback::GetUnitPos(int unitId)
{
	verify ();
	if (CHECK_UNITID(unitId)) {
		CUnit* unit=uh->units[unitId];
		if(unit && (unit->losStatus[teamHandler->AllyTeam(team)] & (LOS_INLOS|LOS_INRADAR))){
			return helper->GetUnitErrorPos(unit,teamHandler->AllyTeam(team));
		}
	}
	return ZeroVector;
}

int CAICallback::GetBuildingFacing(int unitId) {
	verify ();
	if (CHECK_UNITID(unitId)) {
		CUnit* unit=uh->units[unitId];
		if(unit && (unit->losStatus[teamHandler->AllyTeam(team)] & LOS_INLOS)){
			return unit->buildFacing;
		}
	}
	return 0;
}

bool CAICallback::IsUnitCloaked(int unitId) {
	verify ();
	if (CHECK_UNITID(unitId)) {
		CUnit* unit=uh->units[unitId];
		if(unit && (unit->losStatus[teamHandler->AllyTeam(team)] & LOS_INLOS)){
			return unit->isCloaked;
		}
	}
	return false;
}

bool CAICallback::IsUnitParalyzed(int unitId){
	verify();

	if (CHECK_UNITID(unitId)) {
		CUnit* unit = uh->units[unitId];
		if (unit && (unit->losStatus[teamHandler->AllyTeam(team)] & LOS_INLOS)) {
			return unit->stunned;
		}
	}

	return false;
}

bool CAICallback::IsUnitNeutral(int unitId) {
	verify();

	if (CHECK_UNITID(unitId)) {
		CUnit* unit = uh->units[unitId];

		if (unit && (unit->losStatus[teamHandler->AllyTeam(team)] & LOS_INLOS)) {
			if (unit->IsNeutral())
				return true;
		}
	}

	return false;
}

int CAICallback::InitPath(float3 start,float3 end,int pathType)
{
	return pathManager->RequestPath(moveinfo->moveData.at(pathType),start,end);
}

float3 CAICallback::GetNextWaypoint(int pathId)
{
	return pathManager->NextWaypoint(pathId,ZeroVector);
}

void CAICallback::FreePath(int pathId)
{
	pathManager->DeletePath(pathId);
}

float CAICallback::GetPathLength(float3 start, float3 end, int pathType)
{
	// return pathfinder->GetPathLength(start, end, pathType);
	return 0;
}




int CAICallback::GetEnemyUnits(int* unitIds)
{
	verify();
	std::list<CUnit*>::iterator ui;
	int a = 0;

	for (std::list<CUnit*>::iterator ui = uh->activeUnits.begin(); ui != uh->activeUnits.end(); ++ui) {
		CUnit* u = *ui;

		if (!teamHandler->Ally(u->allyteam, teamHandler->AllyTeam(team)) &&
		    (u->losStatus[teamHandler->AllyTeam(team)] & LOS_INLOS)) {
			if (!IsUnitNeutral(u->id)) {
				unitIds[a++] = u->id;
			}
		}
	}

	return a;
}

int CAICallback::GetEnemyUnitsInRadarAndLos(int* unitIds)
{
	verify();
	std::list<CUnit*>::iterator ui;
	int a = 0;

	for (std::list<CUnit*>::iterator ui = uh->activeUnits.begin(); ui != uh->activeUnits.end(); ++ui) {
		CUnit* u = *ui;

		if (!teamHandler->Ally(u->allyteam, teamHandler->AllyTeam(team)) && (u->losStatus[teamHandler->AllyTeam(team)] & (LOS_INLOS | LOS_INRADAR))) {
			if (!IsUnitNeutral(u->id)) {
				unitIds[a++] = u->id;
			}
		}
	}

	return a;
}

int CAICallback::GetEnemyUnits(int* unitIds, const float3& pos, float radius)
{
	verify();
	vector<CUnit*> unit = qf->GetUnitsExact(pos, radius);
	vector<CUnit*>::iterator ui;
	int a = 0;

	for (ui = unit.begin(); ui != unit.end(); ++ui) {
		CUnit* u = *ui;

		if (!teamHandler->Ally(u->allyteam, teamHandler->AllyTeam(team)) && (u->losStatus[teamHandler->AllyTeam(team)] & LOS_INLOS)) {
			if (!IsUnitNeutral(u->id)) {
				unitIds[a] = u->id;
				++a;
			}
		}
	}

	return a;
}


int CAICallback::GetFriendlyUnits(int *unitIds)
{
	verify();
	int a = 0;

	for (std::list<CUnit*>::iterator ui = uh->activeUnits.begin(); ui != uh->activeUnits.end(); ++ui) {
		CUnit* u = *ui;

		if (teamHandler->Ally(u->allyteam, teamHandler->AllyTeam(team))) {
			// IsUnitNeutral does a LOS check, but inconsequential
			// since we can always see friendly units anyway
			if (!IsUnitNeutral(u->id)) {
				unitIds[a++] = u->id;
			}
		}
	}

	return a;
}

int CAICallback::GetFriendlyUnits(int *unitIds, const float3& pos, float radius)
{
	verify();
	vector<CUnit*> unit = qf->GetUnitsExact(pos, radius);
	vector<CUnit*>::iterator ui;
	int a = 0;

	for (ui = unit.begin(); ui != unit.end(); ++ui) {
		CUnit* u = *ui;

		if (teamHandler->Ally(u->allyteam, teamHandler->AllyTeam(team))) {
			// IsUnitNeutral does a LOS check, but inconsequential
			// since we can always see friendly units anyway
			if (!IsUnitNeutral(u->id)) {
				unitIds[a] = u->id;
				++a;
			}
		}
	}

	return a;
}


int CAICallback::GetNeutralUnits(int* unitIds)
{
	verify();
	int a = 0;

	for (std::list<CUnit*>::iterator ui = uh->activeUnits.begin(); ui != uh->activeUnits.end(); ++ui) {
		CUnit* u = *ui;

		// IsUnitNeutral does the LOS check
		if (IsUnitNeutral(u->id)) {
			unitIds[a++] = u->id;
		}
	}

	return a;
}

int CAICallback::GetNeutralUnits(int* unitIds, const float3& pos, float radius)
{
	verify();
	vector<CUnit*> unit = qf->GetUnitsExact(pos, radius);
	vector<CUnit*>::iterator ui;
	int a = 0;

	for (ui = unit.begin(); ui != unit.end(); ++ui) {
		CUnit* u = *ui;

		// IsUnitNeutral does the LOS check
		if (IsUnitNeutral(u->id)) {
			unitIds[a] = u->id;
			++a;
		}
	}

	return a;
}




int CAICallback::GetMapWidth()
{
	return gs->mapx;
}

int CAICallback::GetMapHeight()
{
	return gs->mapy;
}

extern string stupidGlobalMapname;
const char* CAICallback::GetMapName ()
{
	return stupidGlobalMapname.c_str();
}

const char* CAICallback::GetModName()
{
	return modInfo.filename.c_str();
}

float CAICallback::GetMaxMetal()
{
	return mapInfo->map.maxMetal;
}

float CAICallback::GetExtractorRadius()
{
	return mapInfo->map.extractorRadius;
}

float CAICallback::GetMinWind()
{
	return mapInfo->atmosphere.minWind;
}

float CAICallback::GetMaxWind()
{
	return mapInfo->atmosphere.maxWind;
}

float CAICallback::GetTidalStrength()
{
	return mapInfo->map.tidalStrength;
}

float CAICallback::GetGravity()
{
	return mapInfo->map.gravity;
}

/*const unsigned char* CAICallback::GetSupplyMap()
{
	return supplyhandler->supplyLevel[gu->myTeam];
}

const unsigned char* CAICallback::GetTeamMap()
{
	return readmap->teammap;
}*/

const float* CAICallback::GetHeightMap()
{
	return readmap->centerheightmap;
}

float CAICallback::GetMinHeight()
{
	return readmap->minheight;
}

float CAICallback::GetMaxHeight()
{
	return readmap->maxheight;
}

const float* CAICallback::GetSlopeMap()
{
	return readmap->slopemap;
}

const unsigned short* CAICallback::GetLosMap()
{
	return &loshandler->losMap[teamHandler->AllyTeam(team)].front();
}

const unsigned short* CAICallback::GetRadarMap()
{
	return &radarhandler->radarMaps[teamHandler->AllyTeam(team)].front();
}

const unsigned short* CAICallback::GetJammerMap()
{
	return &radarhandler->jammerMaps[teamHandler->AllyTeam(team)].front();
}

const unsigned char* CAICallback::GetMetalMap()
{
	return readmap->metalMap->metalMap;
}

float CAICallback::GetElevation(float x,float z)
{
	return ground->GetHeight2(x,z);
}

void CAICallback::LineDrawerStartPath(const float3& pos, const float* color)
{
	lineDrawer.StartPath(pos, color);
}

void CAICallback::LineDrawerFinishPath()
{
	lineDrawer.FinishPath();
}

void CAICallback::LineDrawerDrawLine(const float3& endPos, const float* color)
{
	lineDrawer.DrawLine(endPos,color);
}

void CAICallback::LineDrawerDrawLineAndIcon(int commandId, const float3& endPos, const float* color)
{
	lineDrawer.DrawLineAndIcon(commandId,endPos,color);
}

void CAICallback::LineDrawerDrawIconAtLastPos(int commandId)
{
	lineDrawer.DrawIconAtLastPos(commandId);
}

void CAICallback::LineDrawerBreak(const float3& endPos, const float* color)
{
	lineDrawer.Break(endPos,color);
}

void CAICallback::LineDrawerRestart()
{
	lineDrawer.Restart();
}

void CAICallback::LineDrawerRestartSameColor()
{
	lineDrawer.RestartSameColor();
}

int CAICallback::CreateSplineFigure(float3 pos1,float3 pos2,float3 pos3,float3 pos4,float width,int arrow,int lifetime,int group)
{
	return geometricObjects->AddSpline(pos1,pos2,pos3,pos4,width,arrow,lifetime,group);
}

int CAICallback::CreateLineFigure(float3 pos1,float3 pos2,float width,int arrow,int lifetime,int group)
{
	return geometricObjects->AddLine(pos1,pos2,width,arrow,lifetime,group);
}

void CAICallback::SetFigureColor(int group,float red,float green,float blue,float alpha)
{
	geometricObjects->SetColor(group,red,green,blue,alpha);
}

void CAICallback::DeleteFigureGroup(int group)
{
	geometricObjects->DeleteGroup(group);
}

void CAICallback::DrawUnit(const char* unitName,float3 pos,float rotation,int lifetime,int teamId,bool transparent,bool drawBorder,int facing)
{
	CUnitDrawer::TempDrawUnit tdu;
	tdu.unitdef=unitDefHandler->GetUnitByName(unitName);
	if(!tdu.unitdef){
		logOutput.Print("Unknown unit in CAICallback::DrawUnit %s",unitName);
		return;
	}
	tdu.pos=pos;
	tdu.rotation=rotation;
	tdu.team=teamId;
	tdu.drawBorder=drawBorder;
	tdu.facing=facing;
	std::pair<int,CUnitDrawer::TempDrawUnit> tp(gs->frameNum+lifetime,tdu);

	GML_STDMUTEX_LOCK(temp); //unit); // maybe superfluous

	if(transparent)
		unitDrawer->tempTransparentDrawUnits.insert(tp);
	else
		unitDrawer->tempDrawUnits.insert(tp);
}

bool CAICallback::CanBuildAt(const UnitDef* unitDef, float3 pos, int facing)
{
	CFeature* f;
	BuildInfo bi(unitDef, pos, facing);
	bi.pos=helper->Pos2BuildPos (bi);
	return !!uh->TestUnitBuildSquare(bi,f,teamHandler->AllyTeam(team));
}


struct SearchOffset {
	int dx,dy;
	int qdist; // dx*dx+dy*dy
};


bool SearchOffsetComparator (const SearchOffset& a, const SearchOffset& b)
{
	return a.qdist < b.qdist;
}

const vector<SearchOffset>& GetSearchOffsetTable (int radius)
{
	static vector <SearchOffset> searchOffsets;
	int size = radius*radius*4;
	if (size > searchOffsets.size()) {
		searchOffsets.resize (size);

		for (int y=0;y<radius*2;y++)
			for (int x=0;x<radius*2;x++)
			{
				SearchOffset& i = searchOffsets[y*radius*2+x];

				i.dx = x-radius;
				i.dy = y-radius;
				i.qdist = i.dx*i.dx+i.dy*i.dy;
			}

		std::sort (searchOffsets.begin(), searchOffsets.end(), SearchOffsetComparator);
	}

	return searchOffsets;
}

float3 CAICallback::ClosestBuildSite(const UnitDef* unitdef,float3 pos,float searchRadius,int minDist, int facing)
{
	if (!unitdef) return float3(-1.0f,0.0f,0.0f);
	CFeature* feature;
	int allyteam=teamHandler->AllyTeam(team);

	int endr = (int)(searchRadius / (SQUARE_SIZE*2));
	const vector<SearchOffset>& ofs = GetSearchOffsetTable (endr);

	for(int so=0;so<endr*endr*4;so++) {
		float x = pos.x+ofs[so].dx*SQUARE_SIZE*2;
		float z = pos.z+ofs[so].dy*SQUARE_SIZE*2;
		BuildInfo bi(unitdef, float3(x,0,z), facing);
		bi.pos = helper->Pos2BuildPos (bi);
		if(uh->TestUnitBuildSquare(bi,feature,allyteam) && (!feature || feature->allyteam!=allyteam))
		{
			int xs=(int)(x/SQUARE_SIZE);
			int zs=(int)(z/SQUARE_SIZE);
			bool good=true;
			int xsize=bi.GetXSize();
			int zsize=bi.GetZSize();
			for(int z2=std::max(0,zs-zsize/2-minDist);z2<std::min(gs->mapy,zs+(zsize+1)/2+minDist);++z2){
				for(int x2=std::max(0,xs-xsize/2-minDist);x2<std::min(gs->mapx,xs+(xsize+1)/2+minDist);++x2){
					CSolidObject* so = groundBlockingObjectMap->GroundBlockedUnsafe(z2 * gs->mapx + x2);
					if(so && so->immobile && !dynamic_cast<CFeature*>(so)){
						good=false;
						break;
					}
				}
			}
			//Checking factories near - factory can open yard for building
			if (good) for(int z2=std::max(0,zs-zsize/2-minDist-2);z2<std::min(gs->mapy,zs+(zsize+1)/2+minDist+2);++z2){
				for(int x2=std::max(0,xs-xsize/2-minDist-2);x2<std::min(gs->mapx,xs+(xsize+1)/2+minDist+2);++x2){
					CSolidObject* so = groundBlockingObjectMap->GroundBlockedUnsafe(z2 * gs->mapx + x2);
					if(so && so->immobile && dynamic_cast<CFactory*>(so) && ((CFactory*)so)->opening){
						good=false;
						break;
					}
				}
			}
			if(good)
				return bi.pos;
		}
	}

	return float3(-1.0f,0.0f,0.0f);
}


float CAICallback::GetMetal()
{
	return teamHandler->Team(team)->metal;
}

float CAICallback::GetMetalIncome()
{
	return teamHandler->Team(team)->prevMetalIncome;
}

float CAICallback::GetMetalUsage()
{
	return teamHandler->Team(team)->prevMetalExpense;
}

float CAICallback::GetMetalStorage()
{
	return teamHandler->Team(team)->metalStorage;
}

float CAICallback::GetEnergy()
{
	return teamHandler->Team(team)->energy;
}

float CAICallback::GetEnergyIncome()
{
	return teamHandler->Team(team)->prevEnergyIncome;
}

float CAICallback::GetEnergyUsage()
{
	return teamHandler->Team(team)->prevEnergyExpense;
}

float CAICallback::GetEnergyStorage()
{
	return teamHandler->Team(team)->energyStorage;
}

bool CAICallback::GetUnitResourceInfo (int unitId, UnitResourceInfo *i)
{
	if (CHECK_UNITID(unitId)) {
		CUnit* unit=uh->units[unitId];
		if(unit && (unit->losStatus[teamHandler->AllyTeam(team)] & LOS_INLOS))
		{
			i->energyMake = unit->energyMake;
			i->energyUse = unit->energyUse;
			i->metalMake = unit->metalMake;
			i->metalUse = unit->metalUse;
			return true;
		}
	}
	return false;
}

bool CAICallback::IsUnitActivated (int unitId)
{
	verify ();
	if (CHECK_UNITID(unitId)) {
		CUnit* unit=uh->units[unitId];
		if(unit && (unit->losStatus[teamHandler->AllyTeam(team)] & LOS_INLOS))
			return unit->activated;
	}
	return false;
}

bool CAICallback::UnitBeingBuilt (int unitId)
{
	verify ();
	if (CHECK_UNITID(unitId)) {
		CUnit* unit=uh->units[unitId];
		if(unit && (unit->losStatus[teamHandler->AllyTeam(team)] & LOS_INLOS))
			return unit->beingBuilt;
	}
	return false;
}

int CAICallback::GetFeatures (int *features, int max)
{
	verify ();
	int i = 0;
	int allyteam = teamHandler->AllyTeam(team);

	const CFeatureSet& fset = featureHandler->GetActiveFeatures();
	for (CFeatureSet::const_iterator it = fset.begin(); it != fset.end(); ++i) {
		CFeature *f = *it;

		if (f->allyteam >= 0 && f->allyteam!=allyteam && !loshandler->InLos(f->pos,allyteam))
			continue;

		features [i++] = f->id;

		if (i == max)
			break;
	}
	return i;
}

int CAICallback::GetFeatures (int *features, int maxids, const float3& pos, float radius)
{
	verify ();
	vector<CFeature*> ft = qf->GetFeaturesExact (pos, radius);
	int allyteam = teamHandler->AllyTeam(team);
	int n = 0;

	for (int a=0;a<ft.size();a++)
	{
		CFeature *f = ft[a];

		if (f->allyteam >= 0 && f->allyteam!=allyteam && !loshandler->InLos(f->pos,allyteam))
			continue;

		features [n++] = f->id;
		if (maxids == n)
			break;
	}

	return n;
}

const FeatureDef* CAICallback::GetFeatureDef (int feature)
{
	verify ();

	const CFeatureSet& fset = featureHandler->GetActiveFeatures();
	CFeatureSet::const_iterator it = fset.find(feature);

	if (it != fset.end()) {
		const CFeature *f = *it;
		int allyteam = teamHandler->AllyTeam(team);
		if (f->allyteam < 0 || f->allyteam == allyteam || loshandler->InLos(f->pos,allyteam))
			return f->def;
	}

	return 0;
}
const FeatureDef* CAICallback::GetFeatureDefById(int featureDefId)
{
	return featureHandler->GetFeatureDefByID(featureDefId);
}

float CAICallback::GetFeatureHealth (int feature)
{
	verify ();

	const CFeatureSet& fset = featureHandler->GetActiveFeatures();
	CFeatureSet::const_iterator it = fset.find(feature);

	if (it != fset.end()) {
		const CFeature *f = *it;
		int allyteam = teamHandler->AllyTeam(team);
		if (f->allyteam < 0 || f->allyteam == allyteam || loshandler->InLos(f->pos,allyteam))
			return f->health;
	}
	return 0.0f;
}

float CAICallback::GetFeatureReclaimLeft (int feature)
{
	verify ();

	const CFeatureSet& fset = featureHandler->GetActiveFeatures();
	CFeatureSet::const_iterator it = fset.find(feature);

	if (it != fset.end()) {
		const CFeature *f = *it;
		int allyteam = teamHandler->AllyTeam(team);
		if (f->allyteam < 0 || f->allyteam == allyteam || loshandler->InLos(f->pos,allyteam))
			return f->reclaimLeft;
	}
	return 0.0f;
}

float3 CAICallback::GetFeaturePos (int feature)
{
	verify ();

	const CFeatureSet& fset = featureHandler->GetActiveFeatures();
	CFeatureSet::const_iterator it = fset.find(feature);

	if (it != fset.end()) {
		const CFeature *f = *it;
		int allyteam = teamHandler->AllyTeam(team);
		if (f->allyteam < 0 || f->allyteam == allyteam || loshandler->InLos(f->pos,allyteam))
			return f->pos;
	}
	return ZeroVector;
}

bool CAICallback::GetValue(int id, void *data)
{
	verify();
	switch (id) {
		case AIVAL_NUMDAMAGETYPES:{
			*((int*)data) = damageArrayHandler->GetNumTypes();
			return true;
		}case AIVAL_EXCEPTION_HANDLING:{
			*(bool*)data = CGlobalAIHandler::CatchException();
			return true;
		}case AIVAL_MAP_CHECKSUM:{
			*(unsigned int*)data = readmap->mapChecksum;
			return true;
		}case AIVAL_DEBUG_MODE:{
			*(bool*)data = gu->drawdebug;
			return true;
		}case AIVAL_GAME_MODE:{
			*(int*)data = gs->gameMode;
			return true;
		}case AIVAL_GAME_PAUSED:{
			*(bool*)data = gs->paused;
			return true;
		}case AIVAL_GAME_SPEED_FACTOR:{
			*(float*)data = gs->speedFactor;
			return true;
		}case AIVAL_GUI_VIEW_RANGE:{
			*(float*)data = gu->viewRange;
			return true;
		}case AIVAL_GUI_SCREENX:{
			*(float*)data = gu->viewSizeX;
			return true;
		}case AIVAL_GUI_SCREENY:{
			*(float*)data = gu->viewSizeY;
			return true;
		}case AIVAL_GUI_CAMERA_DIR:{
			*(float3*)data = camHandler->GetCurrentController().GetDir();
			return true;
		}case AIVAL_GUI_CAMERA_POS:{
			*(float3*)data = camHandler->GetCurrentController().GetPos();
			return true;
		}case AIVAL_LOCATE_FILE_R:{
			std::string f((char*) data);
			f = filesystem.LocateFile(f);
			strcpy((char*) data, f.c_str());
			return true;
		}case AIVAL_LOCATE_FILE_W:{
			std::string f((char*) data);
			f = filesystem.LocateFile(f, FileSystem::WRITE | FileSystem::CREATE_DIRS);
			strcpy((char*) data, f.c_str());
			return true;
		}
		case AIVAL_UNIT_LIMIT: {
			*(int*) data = uh->maxUnits;
			return true;
		}
		case AIVAL_SCRIPT: {
			*(const char**) data = gameSetup ? gameSetup->gameSetupText.c_str() : "";
			return true;
		}
		default:
			return false;
	}
}

int CAICallback::HandleCommand (int commandId, void *data)
{
	switch (commandId)
	{
	case AIHCQuerySubVersionId:
		return 1; // current version of Handle Command interface
	case AIHCAddMapPointId:
		net->Send(CBaseNetProtocol::Get().SendMapDrawPoint(team, (short)((AIHCAddMapPoint *)data)->pos.x, (short)((AIHCAddMapPoint *)data)->pos.z, std::string(((AIHCAddMapPoint *)data)->label)));
		return 1;
	case AIHCAddMapLineId:
		net->Send(CBaseNetProtocol::Get().SendMapDrawLine(team, (short)((AIHCAddMapLine *)data)->posfrom.x, (short)((AIHCAddMapLine *)data)->posfrom.z, (short)((AIHCAddMapLine *)data)->posto.x, (short)((AIHCAddMapLine *)data)->posto.z));
		return 1;
	case AIHCRemoveMapPointId:
		net->Send(CBaseNetProtocol::Get().SendMapErase(team, (short)((AIHCRemoveMapPoint *)data)->pos.x, (short)((AIHCRemoveMapPoint *)data)->pos.z));
		return 1;
	case AIHCSendStartPosId:
		SendStartPos(((AIHCSendStartPos *)data)->ready,((AIHCSendStartPos *)data)->pos);
		return 1;
	case AIHCGetUnitDefByIdId:
	{
		AIHCGetUnitDefById* cmdData = (AIHCGetUnitDefById*) data;
		cmdData->ret = GetUnitDefById(cmdData->unitDefId);
		return 1;
	}
	case AIHCGetWeaponDefByIdId:
	{
		AIHCGetWeaponDefById* cmdData = (AIHCGetWeaponDefById*) data;
		cmdData->ret = GetWeaponDefById(cmdData->weaponDefId);
		return 1;
	}
	case AIHCGetFeatureDefByIdId:
	{
		AIHCGetFeatureDefById* cmdData = (AIHCGetFeatureDefById*) data;
		cmdData->ret = GetFeatureDefById(cmdData->featureDefId);
		return 1;
	}
	default:
		return 0;
	}
}

int CAICallback::GetNumUnitDefs ()
{
	return unitDefHandler->numUnitDefs;
}

void CAICallback::GetUnitDefList (const UnitDef** list)
{
	for (int a=0;a<unitDefHandler->numUnitDefs;a++)
		list [a] = unitDefHandler->GetUnitByID (a+1);
}


float CAICallback::GetUnitDefRadius(int def)
{
	UnitDef *ud = &unitDefHandler->unitDefs[def];
	S3DModel* mdl = ud->LoadModel();
	return mdl->radius;
}

float CAICallback::GetUnitDefHeight(int def)
{
	UnitDef *ud = &unitDefHandler->unitDefs[def];
	S3DModel* mdl = ud->LoadModel();
	return mdl->height;
}


bool CAICallback::GetProperty(int unitId, int property, void *data)
{
	verify ();
	if (CHECK_UNITID(unitId)) {
		CUnit* unit = uh->units[unitId];
		const int allyTeam = teamHandler->AllyTeam(team);
		if (!(unit && (unit->losStatus[allyTeam] & LOS_INLOS))) {
			return false;  //return if the unit doesn't exist or cant be seen
		}

		switch (property) {
			case AIVAL_UNITDEF: {
				if (teamHandler->Ally(unit->allyteam, allyTeam)) {
					(*(const UnitDef**)data) = unit->unitDef;
				} else {
					const UnitDef* unitDef = unit->unitDef;
					const UnitDef* decoyDef = unitDef->decoyDef;
					if (decoyDef == NULL) {
						(*(const UnitDef**)data) = unitDef;
					} else {
						(*(const UnitDef**)data) = decoyDef;
					}
				}
				return true;
			}
			case AIVAL_CURRENT_FUEL: {
				(*(float*)data) = unit->currentFuel;
				return true;
			}
			case AIVAL_STOCKPILED: {
				if (!unit->stockpileWeapon || !teamHandler->Ally(unit->allyteam, allyTeam)) {
					return false;
				}
				(*(int*)data) = unit->stockpileWeapon->numStockpiled;
				return true;
			}
			case AIVAL_STOCKPILE_QUED: {
				if (!unit->stockpileWeapon || !teamHandler->Ally(unit->allyteam, allyTeam)) {
					return false;
				}
				(*(int*)data) = unit->stockpileWeapon->numStockpileQued;
				return true;
			}
			case AIVAL_UNIT_MAXSPEED: {
				(*(float*) data) = unit->maxSpeed;
				return true;
			}
			default:
				return false;
		}
	}
	return false;
}


int CAICallback::GetFileSize (const char *filename)
{
	CFileHandler fh (filename);

	if (!fh.FileExists ())
		return -1;

	return fh.FileSize();
}


int CAICallback::GetFileSize (const char *filename, const char* modes)
{
	CFileHandler fh (filename, modes);

	if (!fh.FileExists ())
		return -1;

	return fh.FileSize();
}


bool CAICallback::ReadFile (const char *filename, void *buffer, int bufferLength)
{
	CFileHandler fh (filename);
	int fs;
	if (!fh.FileExists() || bufferLength < (fs = fh.FileSize()))
		return false;

	fh.Read (buffer, fs);
	return true;
}


bool CAICallback::ReadFile (const char *filename, const char* modes,
                            void *buffer, int bufferLength)
{
	CFileHandler fh (filename, modes);
	int fs;
	if (!fh.FileExists() || bufferLength < (fs = fh.FileSize()))
		return false;

	fh.Read (buffer, fs);
	return true;
}


// Additions to the interface by Alik
int CAICallback::GetSelectedUnits(int *unitIds)
{
	verify();
	int a = 0;

	GML_RECMUTEX_LOCK(sel); // GetSelectedUnit
	// check if the allyteam of the player running
	// the AI lib matches the AI's actual allyteam
	if (gu->myAllyTeam == teamHandler->AllyTeam(team)) {
		for (CUnitSet::iterator ui = selectedUnits.selectedUnits.begin(); ui != selectedUnits.selectedUnits.end(); ++ui)
			unitIds[a++] = (*ui)->id;
	}
	return a;
}


float3 CAICallback::GetMousePos() {
	verify ();
	if (gu->myAllyTeam == teamHandler->AllyTeam(team))
		return inMapDrawer->GetMouseMapPos();
	else
		return ZeroVector;
}


int CAICallback::GetMapPoints(PointMarker *pm, int maxPoints)
{
	int a=0;
	verify ();

	if (gu->myAllyTeam != teamHandler->AllyTeam(team))
		return 0;

	for (int i=0;i<inMapDrawer->numQuads;i++){
		if(!inMapDrawer->drawQuads[i].points.empty()){
			for(std::list<CInMapDraw::MapPoint>::iterator mp=inMapDrawer->drawQuads[i].points.begin();mp!=inMapDrawer->drawQuads[i].points.end();++mp){
				if(mp->color==teamHandler->Team(team)->color) { //Maybe add so that markers of your ally team would be also found?
					pm[a].pos=mp->pos;
					pm[a].color=mp->color;
					pm[a].label=mp->label.c_str();
					if (++a == maxPoints) return a;
				}
				else{ continue; }
			}
		}
	}
	return a;
}

int CAICallback::GetMapLines(LineMarker *lm, int maxLines)
{
	int a=0;
	verify ();

	if (gu->myAllyTeam != teamHandler->AllyTeam(team))
		return 0;

	for (int i=0;i<inMapDrawer->numQuads;i++){
		if(!inMapDrawer->drawQuads[i].points.empty()){
			for(std::list<CInMapDraw::MapLine>::iterator ml=inMapDrawer->drawQuads[i].lines.begin();ml!=inMapDrawer->drawQuads[i].lines.end();++ml){
				if(ml->color==teamHandler->Team(team)->color){ //Maybe add so that markers of your ally team would be also found?
					lm[a].pos=ml->pos;
					lm[a].color=ml->color;
					lm[a].pos2=ml->pos2;
					if (++a == maxLines) return a;
				}
				else {continue;}
			}
		}
	}
	return a;
}


const WeaponDef* CAICallback::GetWeapon(const char* weaponName)
{
	return weaponDefHandler->GetWeapon(weaponName);
}
const WeaponDef* CAICallback::GetWeaponDefById(int weaponDefId)
{
	return weaponDefHandler->GetWeaponById(weaponDefId);
}


bool CAICallback::CanBuildUnit(int unitDefID)
{
	const UnitDef* ud = unitDefHandler->GetUnitByID(unitDefID);
	if (ud == NULL) {
		return false;
	}
	return uh->CanBuildUnit(ud, team);
}


const float3 *CAICallback::GetStartPos()
{
	return &teamHandler->Team(team)->startPos;
}


const char* CAICallback::CallLuaRules(const char* data, int inSize, int* outSize)
{
	if (luaRules == NULL) {
		return NULL;
	}
	return luaRules->AICallIn(data, inSize, outSize);
}


IMPLEMENT_PURE_VIRTUAL(IAICallback::~IAICallback())

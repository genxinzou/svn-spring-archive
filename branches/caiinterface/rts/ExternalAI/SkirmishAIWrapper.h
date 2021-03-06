/*
	Copyright (c) 2008 Robin Vobruba <hoijui.quaero@gmail.com>

	This program is free software {} you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation {} either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY {} without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _SKIRMISHAIWRAPPER_H
#define _SKIRMISHAIWRAPPER_H

#include "Object.h"
#include "ISkirmishAI.h"
#include "GlobalAICallback.h"
#include "SkirmishAIKey.h"
#include "Interface/SAICallback.h"
#include "Platform/SharedLib.h"

#include <map>
#include <string>

class CAICallback;
struct Command;
struct float3;

class CSkirmishAIWrapper : public CObject, public ISkirmishAI {
public:
	CR_DECLARE(CSkirmishAIWrapper);

	CSkirmishAIWrapper(int teamId, const SkirmishAIKey& key);
	~CSkirmishAIWrapper();

	void Serialize(creg::ISerializer *s);
	void PostLoad();

	// AI Events
	virtual void Load(std::istream *s);
	virtual void Save(std::ostream *s);

	virtual void UnitIdle(int unitId);
	virtual void UnitCreated(int unitId);
	virtual void UnitFinished(int unitId);
	virtual void UnitDestroyed(int unitId, int attackerUnitId);
	virtual void UnitDamaged(int unitId, int attackerUnitId, float damage, const float3& dir);
	virtual void UnitMoveFailed(int unitId);
	virtual void UnitGiven(int unitId, int oldTeam, int newTeam);
	virtual void UnitCaptured(int unitId, int oldTeam, int newTeam);
	virtual void EnemyEnterLOS(int unitId);
	virtual void EnemyLeaveLOS(int unitId);
	virtual void EnemyEnterRadar(int unitId);
	virtual void EnemyLeaveRadar(int unitId);
	virtual void EnemyDestroyed(int enemyUnitId, int attackerUnitId);
	virtual void EnemyDamaged(int enemyUnitId, int attackerUnitId, float damage, const float3& dir);
	virtual void Update(int frame);
	virtual void GotChatMsg(const char* msg, int fromPlayerId);
	virtual void WeaponFired(int unitId, int weaponDefId);
	virtual void PlayerCommandGiven(const std::vector<int>& selectedUnits, const Command& c, int playerId);
	virtual void CommandFinished(int unitId, int commandTopicId);
	virtual void SeismicPing(int allyTeam, int unitId, const float3& pos, float strength);


	virtual void PreDestroy(); // called just before all the units are destroyed

	virtual int GetTeamId() const;

	virtual void SetCheatEventsEnabled(bool enable);
	virtual bool IsCheatEventsEnabled() const;

	/**
	 * inherited from ISkirmishAI.
	 * CAUTION: takes C AI Interface events, not engine C++ ones!
	 */
	virtual int HandleEvent(int topic, const void* data) const;

private:
	virtual void Init();
	virtual void Release();

private:
	int teamId;
	bool cheatEvents;

	ISkirmishAI* ai;
	CGlobalAICallback* callback;
	SAICallback* c_callback;
	SkirmishAIKey key;
	const struct InfoItem* info;
	unsigned int size_info;

private:
	void LoadSkirmishAI(bool postLoad);
};

#endif // _SKIRMISHAIWRAPPER_H

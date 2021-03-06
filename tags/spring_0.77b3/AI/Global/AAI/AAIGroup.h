#pragma once

#include "aidef.h"
#include "AAISector.h"

class AAI;
class AAIBuildTable;
class AAIAttack;

class AAIGroup
{
public:
	AAIGroup(IAICallback *cb, AAI* ai, const UnitDef *def, UnitType unit_type);
	~AAIGroup(void);

	bool AddUnit(int unit_id, int def_id, UnitType type);

	bool RemoveUnit(int unit, int attacker);

	void GiveOrder(Command *c, float importance, UnitTask task, const char *owner);

	void AttackSector(AAISector *dest, float importance);

	// defend unit vs enemy (0; zerovector if enemy unknown)
	void Defend(int unit, float3 *enemy_pos, int importance);

	// retreat combat groups to pos
	void Retreat(float3 *pos);

	// bombs target (only for bomber groups)
	void BombTarget(int target_id, float3 *target_pos);

	// orders fighters to defend air space
	void DefendAirSpace(float3 *pos);

	// orders air units to attack 
	void AirRaidUnit(int unit_id);

	int GetRandomUnit();

	void Update();

	void TargetUnitKilled();

	// checks current rally point and chooses new one if necessary
	void UpdateRallyPoint();

	// gets a new rally point and orders units to get there
	void GetNewRallyPoint();

	void UnitIdle(int unit);

	float GetPowerVS(int assault_cat_id);

	float3 GetGroupPos();

	// returns true if group is strong enough to attack
	bool SufficientAttackPower();

	int maxSize;
	int size;
	int speed_group; 
	float avg_speed;
	
	list<int2> units;
	
	Command lastCommand;
	int lastCommandFrame;

	float task_importance;	// importance of current task

	GroupTask task;

	UnitCategory category;
	int combat_category;

	UnitType group_unit_type;

	unsigned int group_movement_type;

	AAISector *target_sector;

	// attack the group takes part in
	AAIAttack *attack;

	// rally point of the group, ZeroVector if none...
	float3 rally_point;

private:
	
	IAICallback *cb;
	AAI* ai;
	AAIBuildTable *bt;
};

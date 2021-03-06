#ifndef UNIT_H
#define UNIT_H


#include "GlobalAI.h"

class CUNIT {
	public:
		CUNIT(AIClasses* ai);
		virtual ~CUNIT();

		// misc. info
		float3 pos();
		float Health();
		int category();

		const UnitDef* def();
		int myid;
		// 0: mine, 1: allied, 2: enemy -1: non-existant
		int owner();

		// the attackgroup that the unit might belong to
		// (added to support some AttackHandler/group features)
		int groupID;

		bool CanAttack(int otherUnit);
		bool CanAttackMe(int otherUnit);

		// used in groups when units don't respond to move orders
		int stuckCounter;
		float3 earlierPosition;
		// used in groups to microcontrol units to their max range of the chosen target
		int maneuverCounter;

		// specialized
		bool ReclaimBest(bool metal, float radius = 1000);

		// construction
		bool Build_ClosestSite(const UnitDef* unitdef, float3 targetpos, int separation = DEFCBS_SEPARATION, float radius = DEFCBS_RADIUS);
		bool FactoryBuild(const UnitDef* built);
		// added by Kloot
		bool HubBuild(const UnitDef* built);
		bool isHub;


		// target-based abilities
		bool Attack(int target);
		bool Capture(int target);
		bool Guard(int target);
		bool Load(int target);
		bool Reclaim(int target);
		bool Repair(int target);
		bool Ressurect(int target);

		// location point abilities
		bool Build(float3 pos, const UnitDef* unit);
		bool Move(float3 pos);
		bool MoveShift(float3 pos);
		bool Patrol(float3 pos);
		bool PatrolShift(float3 pos);

		// radius abilities
		bool Attack(float3 pos, float radius);
		bool Ressurect(float3 pos, float radius);
		bool Reclaim(float3 pos, float radius);
		bool Capture(float3 pos, float radius);
		bool Restore(float3 pos, float radius);
		bool Load(float3 pos, float radius);
		bool Unload(float3 pos, float radius);

		// toggable abilities
		bool Cloaking(bool on);
		bool OnOff(bool on);

		// special abilities
		bool SelfDestruct();
		bool SetFiringMode(int mode);
		bool Stop();
		bool SetMaxSpeed(float speed);


	private:
		AIClasses *ai;

		// command generators
		Command MakePosCommand(int id, float3 pos, float radius = 0);
		Command MakeIntCommand(int id, int number, int maxnum = 4999);
};


#endif

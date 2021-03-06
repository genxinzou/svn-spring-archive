#ifndef GLOBALAIHANDLER_H
#define GLOBALAIHANDLER_H

#include "Object.h"
#include "Platform/ConfigHandler.h"
#include <map>
#include <string>

class CUnit;
class CGlobalAI;
struct WeaponDef;
struct Command;

class CGlobalAIHandler :
	public CObject
{
public:
	static bool CatchException();

	CGlobalAIHandler(void);
	~CGlobalAIHandler(void);

	void Update(void);
	void PreDestroy ();

	void UnitEnteredLos(CUnit* unit,int allyteam);
	void UnitLeftLos(CUnit* unit,int allyteam);
	void UnitEnteredRadar(CUnit* unit,int allyteam);
	void UnitLeftRadar(CUnit* unit,int allyteam);

	void UnitIdle(CUnit* unit);
	void UnitCreated(CUnit* unit);
	void UnitFinished(CUnit* unit);
	void UnitDestroyed(CUnit* unit, CUnit *attacker);
	void UnitMoveFailed(CUnit* unit);
	void UnitTaken(CUnit* unit, int newteam);
	void UnitGiven(CUnit* unit, int oldteam);
	void WeaponFired(CUnit* unit,WeaponDef* def);
	void PlayerCommandGiven(std::vector<int>& selectedunits,Command& c,int player);
	CGlobalAI* ais[MAX_TEAMS];
	bool hasAI;

	struct AIMemBuffer{
		char* mem;
		int usage;
	};
	std::map<std::string,AIMemBuffer> memBuffers[MAX_TEAMS];

	bool CreateGlobalAI(int team, const char* dll);
	void* GetAIBuffer(int team, std::string name, int length);
	void ReleaseAIBuffer(int team, std::string name);
	void GotChatMsg(const char* msg, int player);
	void UnitDamaged(CUnit* attacked,CUnit* attacker,float damage);
};

extern CGlobalAIHandler* globalAI;

#endif

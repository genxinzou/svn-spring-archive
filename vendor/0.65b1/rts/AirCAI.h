#pragma once
#include "commandai.h"

class CAirCAI :
	public CCommandAI
{
public:
	CAirCAI(CUnit* owner);
	~CAirCAI(void);

	int GetDefaultCmd(CUnit* pointed,CFeature* feature);
	void SlowUpdate();
	void GiveCommand(Command &c);
	void DrawCommands(void);
	void AddUnit(CUnit* unit);
	void FinishCommand(void);

	float3 goalPos;
	float3 patrolGoal;

	float3 basePos;
	float3 baseDir;

	bool tempOrder;

	int activeCommand;
	int targetAge;
	unsigned int patrolTime;
	void BuggerOff(float3 pos, float radius);
};

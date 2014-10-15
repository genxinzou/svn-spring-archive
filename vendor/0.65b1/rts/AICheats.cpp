#include "stdafx.h"
#include ".\aicheats.h"
#include <vector>
#include "globalai.h"
#include "unit.h"
#include "quadfield.h"
#include "unithandler.h"
#include "unitloader.h"
#include "team.h"
//#include "mmgr.h"

using namespace std;

CAICheats::CAICheats(CGlobalAI* ai)
:	ai(ai)
{
}

CAICheats::~CAICheats(void)
{
}

void CAICheats::SetMyHandicap(float handicap)
{
	gs->teams[ai->team]->handicap=1+handicap/100;
}

void CAICheats::GiveMeMetal(float amount)
{
	gs->teams[ai->team]->metal+=amount;
}

void CAICheats::GiveMeEnergy(float amount)
{
	gs->teams[ai->team]->energy+=amount;
}

int CAICheats::CreateUnit(const char* name,float3 pos)
{
	CUnit* u=unitLoader.LoadUnit(name,pos,ai->team,false);
	if(u)
		return u->id;
	return 0;
}

const UnitDef* CAICheats::GetUnitDef(int unitid)
{
	CUnit* unit=uh->units[unitid];
	if(unit){
		return unit->unitDef;
	}
	return 0;

}

float3 CAICheats::GetUnitPos(int unitid)
{
	CUnit* unit=uh->units[unitid];
	if(unit){
		return unit->pos;
	}
	return ZeroVector;

}

int CAICheats::GetEnemyUnits(int *units)
{
	list<CUnit*>::iterator ui;
	int a=0;

	for(list<CUnit*>::iterator ui=uh->activeUnits.begin();ui!=uh->activeUnits.end();++ui){
		if(!gs->allies[(*ui)->allyteam][gs->team2allyteam[ai->team]]){
			units[a++]=(*ui)->id;
		}
	}
	return a;
}

int CAICheats::GetEnemyUnits(int *units,const float3& pos,float radius)
{
	vector<CUnit*> unit=qf->GetUnitsExact(pos,radius);

	vector<CUnit*>::iterator ui;
	int a=0;

	for(ui=unit.begin();ui!=unit.end();++ui){
		if(!gs->allies[(*ui)->allyteam][gs->team2allyteam[ai->team]]){
			units[a]=(*ui)->id;
			++a;
		}
	}
	return a;

}

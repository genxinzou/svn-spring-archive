// Weapon.cpp: implementation of the CWeapon class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Weapon.h"
#include "Sim/Units/Unit.h"
#include "Game/GameHelper.h"
#include "Game/Team.h"
#include "Sim/Units/COB/CobInstance.h"
#include "Sim/Units/COB/CobFile.h"
#include "myMath.h"
#include "Game/UI/InfoConsole.h"
#include "Rendering/UnitModels/3DOParser.h"
#include "SyncTracer.h"
#include "WeaponDefHandler.h"
#include "Sim/Projectiles/WeaponProjectile.h"
#include "Sim/Misc/InterceptHandler.h"
#include "Sim/Units/CommandAI/CommandAI.h"
#include "Sim/Map/Ground.h"
#include "Game/Camera.h"
#include "Game/Player.h"
#include "Sim/Misc/LosHandler.h"
#include "Sim/Misc/GeometricObjects.h"
#include "mmgr.h"
#include "Sim/MoveTypes/TAAirMoveType.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

static void ScriptCallback(int retCode,void* p1,void* p2)
{
	if(retCode==1)
		((CWeapon*)p1)->ScriptReady();
}

CWeapon::CWeapon(CUnit* owner)
:	targetType(Target_None),
	owner(owner),
	range(1),
	reloadTime(1),
	reloadStatus(0),
	salvoLeft(0),
	salvoDelay(0),
	salvoSize(1),
	nextSalvo(0),
	predict(0),
	targetUnit(0),
	accuracy(0),
	projectileSpeed(1),
	predictSpeedMod(1),
	metalFireCost(0),
	energyFireCost(0),
	targetPos(1,1,1),
	angleGood(false),
	wantedDir(0,1,0),
	lastRequestedDir(0,-1,0),
	haveUserTarget(false),
	subClassReady(true),
	onlyForward(false),
	weaponPos(0,0,0),
	lastRequest(0),
	relWeaponPos(0,1,0),
	muzzleFlareSize(1),
	lastTargetRetry(-100),
	areaOfEffect(1),
	badTargetCategory(0),
	onlyTargetCategory(0xffffffff),
	weaponDef(0),
	buildPercent(0),
	numStockpiled(0),
	numStockpileQued(0),
	interceptTarget(0),
	salvoError(0,0,0),
	sprayangle(0),
	useWeaponPosForAim(0),
	errorVector(ZeroVector),
	errorVectorAdd(ZeroVector),
	lastErrorVectorUpdate(0),
	slavedTo(0),
	mainDir(0,0,1),
	maxMainDirAngleDif(-1)
{
}

CWeapon::~CWeapon()
{
	if(weaponDef->interceptor)
		interceptHandler.RemoveInterceptorWeapon(this);
}

void CWeapon::Update()
{
	// landed aircraft can't shoot
	if( dynamic_cast<CTAAirMoveType*> (owner->moveType) ) {
		CTAAirMoveType *amt = (CTAAirMoveType *)owner->moveType;
		if (amt->aircraftState == CTAAirMoveType::AIRCRAFT_LANDED)
			return;
	}

	if(targetType==Target_Unit){
	// landed aircraft can't shoot
	if( dynamic_cast<CTAAirMoveType*> (owner->moveType) ) {
		CTAAirMoveType *amt = (CTAAirMoveType *)owner->moveType;
		if (amt->aircraftState == CTAAirMoveType::AIRCRAFT_LANDED)
			return;
	}

		if(lastErrorVectorUpdate<gs->frameNum-16){
			float3 newErrorVector(gs->randVector());
			errorVectorAdd=(newErrorVector-errorVector)*(1.0f/16.0f);
			lastErrorVectorUpdate=gs->frameNum;
		}
		errorVector+=errorVectorAdd;
		if(weaponDef->selfExplode){	//assumes that only flakker like units that need to hit aircrafts has this,change to a separate tag later
			targetPos=helper->GetUnitErrorPos(targetUnit,owner->allyteam)+targetUnit->speed*(0.5+predictSpeedMod*0.5)*predict;
		} else {
			targetPos=helper->GetUnitErrorPos(targetUnit,owner->allyteam)+targetUnit->speed*predictSpeedMod*predict;
		}
		targetPos+=errorVector*(weaponDef->targetMoveError*30*targetUnit->speed.Length()*(1.0-owner->limExperience));
		if(!weaponDef->waterweapon && targetPos.y<1)
			targetPos.y=1;
	}

	if(weaponDef->interceptor)
		CheckIntercept();
	if(targetType!=Target_None){
		if(onlyForward){
			float3 goaldir=targetPos-owner->pos;
			goaldir.Normalize();
			angleGood=owner->frontdir.dot(goaldir) > maxAngleDif;
		} else if(lastRequestedDir.dot(wantedDir)<maxAngleDif || lastRequest+15<gs->frameNum){
			angleGood=false;
			lastRequestedDir=wantedDir;
			lastRequest=gs->frameNum;

			short int heading=GetHeadingFromVector(wantedDir.x,wantedDir.z);
			short int pitch=(short int) (asin(wantedDir.dot(owner->updir))*(32768/PI));
			std::vector<long> args;
			args.push_back(short(heading-owner->heading));
			args.push_back(pitch);
			owner->cob->Call(COBFN_AimPrimary+weaponNum,args,ScriptCallback,this,0);
		}
	}
	if(weaponDef->stockpile && numStockpileQued){
		float p=1.0/reloadTime;
		if(gs->Team(owner->team)->metal>=metalFireCost*p && gs->Team(owner->team)->energy>=energyFireCost*p){
			owner->UseEnergy(energyFireCost*p);
			owner->UseMetal(metalFireCost*p);
			buildPercent+=p;
		}
		if(buildPercent>=1){
			buildPercent=0;
			numStockpileQued--;
			numStockpiled++;
			owner->commandAI->StockpileChanged(this);
		}
	}

	if(salvoLeft==0 
#ifdef DIRECT_CONTROL_ALLOWED
	&& (!owner->directControl || owner->directControl->mouse1 || owner->directControl->mouse2)
#endif
	&& targetType!=Target_None
	&& angleGood 
	&& subClassReady 
	&& reloadStatus<=gs->frameNum
	&& (weaponDef->stockpile || (gs->Team(owner->team)->metal>=metalFireCost && gs->Team(owner->team)->energy>=energyFireCost))
	&& (!weaponDef->stockpile || numStockpiled)
	&& (weaponDef->waterweapon || weaponPos.y>0)
	){
		std::vector<long> args;
		args.push_back(0);
		owner->cob->Call(COBFN_QueryPrimary+weaponNum,args);
		relWeaponPos=owner->localmodel->GetPiecePos(args[0]);
		weaponPos=owner->pos+owner->frontdir*relWeaponPos.z+owner->updir*relWeaponPos.y+owner->rightdir*relWeaponPos.x;
		useWeaponPosForAim=reloadTime/16+8;

		if(TryTarget(targetPos,haveUserTarget,targetUnit)){
			if(weaponDef->stockpile){
				numStockpiled--;
				owner->commandAI->StockpileChanged(this);
			} else {
				owner->UseEnergy(energyFireCost);
				owner->UseMetal(metalFireCost);
			}
			if(weaponDef->stockpile)
				reloadStatus=gs->frameNum+60;
			else
				reloadStatus=gs->frameNum+(int)(reloadTime/owner->reloadSpeed);

			salvoLeft=salvoSize;
			nextSalvo=gs->frameNum;
			salvoError=gs->randVector()*(owner->isMoving?weaponDef->movingAccuracy:accuracy);
			if(targetType==Target_Pos || (targetType==Target_Unit && !(targetUnit->losStatus[owner->allyteam] & LOS_INLOS)))		//area firing stuff is to effective at radar firing...
				salvoError*=1.3;

			owner->lastMuzzleFlameSize=muzzleFlareSize;
			owner->lastMuzzleFlameDir=wantedDir;
			owner->cob->Call(COBFN_FirePrimary+weaponNum);
		}
	}

	if(salvoLeft && nextSalvo<=gs->frameNum){
		salvoLeft--;
		nextSalvo=gs->frameNum+salvoDelay;
		owner->lastFireWeapon=gs->frameNum;

		std::vector<long> args;
		args.push_back(0);
		owner->cob->Call(/*COBFN_AimFromPrimary+weaponNum/*/COBFN_QueryPrimary+weaponNum/**/,args);
		relWeaponPos=owner->localmodel->GetPiecePos(args[0]);
		weaponPos=owner->pos+owner->frontdir*relWeaponPos.z+owner->updir*relWeaponPos.y+owner->rightdir*relWeaponPos.x;

//		info->AddLine("RelPosFire %f %f %f",relWeaponPos.x,relWeaponPos.y,relWeaponPos.z);

		owner->isCloaked=false;
		owner->curCloakTimeout=gs->frameNum+owner->cloakTimeout;

		Fire();

		//Rock the unit in the direction of the fireing
		float3 rockDir = wantedDir;
		rockDir.y = 0;
		rockDir = -rockDir.Normalize();
		std::vector<long> rockAngles;
		rockAngles.push_back((long)(500 * rockDir.z));
		rockAngles.push_back((long)(500 * rockDir.x));
		owner->cob->Call(COBFN_RockUnit,  rockAngles);		

		owner->commandAI->WeaponFired(this);

		if(salvoLeft==0){
			owner->cob->Call(COBFN_EndBurst+weaponNum);		
		}
#ifdef TRACE_SYNC
	tracefile << "Weapon fire: ";
	tracefile << weaponPos.x << " " << weaponPos.y << " " << weaponPos.z << " " << targetPos.x << " " << targetPos.y << " " << targetPos.z << "\n";
#endif
	}
}

bool CWeapon::AttackGround(float3 pos,bool userTarget)
{
	if((!userTarget && weaponDef->noAutoTarget))
		return false;
	if(weaponDef->interceptor || weaponDef->onlyTargetCategory!=0xffffffff)
		return false;

	if(!weaponDef->waterweapon && pos.y<1)
		pos.y=1;
	weaponPos=owner->pos+owner->frontdir*relWeaponPos.z+owner->updir*relWeaponPos.y+owner->rightdir*relWeaponPos.x;
	if(weaponPos.y<ground->GetHeight2(weaponPos.x,weaponPos.z))
		weaponPos=owner->pos+10;		//hope that we are underground because we are a popup weapon and will come above ground later

	if(!TryTarget(pos,userTarget,0))
		return false;
	if(targetUnit){
		DeleteDeathDependence(targetUnit);
		targetUnit=0;
	}
	haveUserTarget=userTarget;
	targetType=Target_Pos;
	targetPos=pos;
	return true;
}

bool CWeapon::AttackUnit(CUnit *unit,bool userTarget)
{
	if((!userTarget && weaponDef->noAutoTarget))
		return false;
	if(weaponDef->interceptor)
		return false;

	weaponPos=owner->pos+owner->frontdir*relWeaponPos.z+owner->updir*relWeaponPos.y+owner->rightdir*relWeaponPos.x;
	if(weaponPos.y<ground->GetHeight2(weaponPos.x,weaponPos.z))
		weaponPos=owner->pos+10;		//hope that we are underground because we are a popup weapon and will come above ground later

	if(!unit){
		if(targetType!=Target_Unit)			//make the unit be more likely to keep the current target if user start to move it
			targetType=Target_None;
		haveUserTarget=false;
		return false;
	}
	float3 targetPos(helper->GetUnitErrorPos(unit,owner->allyteam));
	targetPos+=errorVector*(weaponDef->targetMoveError*30*unit->speed.Length()*(1.0-owner->limExperience));
	if(!TryTarget(targetPos,userTarget,unit))
		return false;

	if(targetUnit){
		DeleteDeathDependence(targetUnit);
		targetUnit=0;
	}
	haveUserTarget=userTarget;
	targetType=Target_Unit;
	targetUnit=unit;
	targetPos=unit->midPos+float3(0,0.3,0)*unit->radius;
	AddDeathDependence(targetUnit);
	return true;
}

void CWeapon::HoldFire()
{
	if(targetUnit){
		DeleteDeathDependence(targetUnit);
		targetUnit=0;
	}
	targetType=Target_None;
	haveUserTarget=false;
}

void CWeapon::SlowUpdate()
{
#ifdef TRACE_SYNC
	tracefile << "Weapon slow update: ";
	tracefile << owner->id << " " << weaponNum <<  "\n";
#endif

	std::vector<long> args;
	args.push_back(0);
//	owner->cob->Call("AimFromPrimary",args);
	if(useWeaponPosForAim){
		owner->cob->Call(/*COBFN_AimFromPrimary+weaponNum/*/COBFN_QueryPrimary+weaponNum/**/,args);
		if(useWeaponPosForAim>1)
			useWeaponPosForAim--;
	} else {
		owner->cob->Call(COBFN_AimFromPrimary+weaponNum/*/COBFN_QueryPrimary+weaponNum/**/,args);
	}
	relWeaponPos=owner->localmodel->GetPiecePos(args[0]);
	weaponPos=owner->pos+owner->frontdir*relWeaponPos.z+owner->updir*relWeaponPos.y+owner->rightdir*relWeaponPos.x;
	if(weaponPos.y<ground->GetHeight2(weaponPos.x,weaponPos.z))
		weaponPos=owner->pos+10;		//hope that we are underground because we are a popup weapon and will come above ground later

	predictSpeedMod=1+(gs->randFloat()-0.5)*2*(1-owner->limExperience);

	if(targetType!=Target_None && !TryTarget(targetPos,haveUserTarget,targetUnit)){
		HoldFire();
	}
	if(targetType==Target_Unit && targetUnit->isCloaked && !(targetUnit->losStatus[owner->allyteam] & (LOS_INLOS | LOS_INRADAR)))
		HoldFire();

	if(slavedTo){	//use targets from the thing we are slaved to
		if(targetUnit){
			DeleteDeathDependence(targetUnit);
			targetUnit=0;
		}
		targetType=Target_None;
		if(slavedTo->targetType==Target_Unit){
			float3 tp=helper->GetUnitErrorPos(slavedTo->targetUnit,owner->allyteam);
			tp+=errorVector*(weaponDef->targetMoveError*30*slavedTo->targetUnit->speed.Length()*(1.0-owner->limExperience));
			if(TryTarget(tp,false,slavedTo->targetUnit)){
				targetType=Target_Unit;
				targetUnit=slavedTo->targetUnit;
				targetPos=tp;
				AddDeathDependence(targetUnit);
			}
		} else if(slavedTo->targetType==Target_Pos){
			if(TryTarget(slavedTo->targetPos,false,0)){
				targetType=Target_Pos;
				targetPos=slavedTo->targetPos;
			}
		}
		return;
	}

	if(!weaponDef->noAutoTarget){
		if(owner->fireState==2 && !haveUserTarget && (targetType==Target_None || (targetType==Target_Unit && (targetUnit->category & badTargetCategory)) || gs->frameNum>lastTargetRetry+65)){
			std::map<float,CUnit*> targets;
			helper->GenerateTargets(this,targetUnit,targets);

			for(std::map<float,CUnit*>::iterator ti=targets.begin();ti!=targets.end();++ti){
				if(targetUnit && (ti->second->category & badTargetCategory))
					continue;
				float3 tp(ti->second->midPos);
				tp+=errorVector*(weaponDef->targetMoveError*30*ti->second->speed.Length()*(1.0-owner->limExperience));
				if(TryTarget(tp,false,ti->second)){
					if(targetUnit){
						DeleteDeathDependence(targetUnit);
					}
					targetType=Target_Unit;
					targetUnit=ti->second;
					targetPos=tp;
					AddDeathDependence(targetUnit);
					break;
				}
			}
		}
	}
	if(targetType!=Target_None){
		owner->haveTarget=true;
		if(haveUserTarget)
			owner->haveUserTarget=true;
	} else {	//if we cant target anything try switching aim point
		if(useWeaponPosForAim && useWeaponPosForAim==1){
			useWeaponPosForAim--;
		} else {
			useWeaponPosForAim=1;
		}
	}
}

void CWeapon::DependentDied(CObject *o)
{
	if(o==targetUnit){
		targetUnit=0;
		if(targetType==Target_Unit){
			targetType=Target_None;
			haveUserTarget=false;
		}
	}
	if(weaponDef->interceptor){
		incoming.remove((CWeaponProjectile*)o);
	}
}

bool CWeapon::TryTarget(const float3 &pos,bool userTarget,CUnit* unit)
{
	if(unit && !(onlyTargetCategory&unit->category))
		return false;

	if(weaponDef->stockpile && !numStockpiled)
		return false;

	float3 dif=pos-weaponPos;

	float r=range+(owner->pos.y-pos.y)*heightMod;
	if(dif.SqLength2D()>r*r)
		return false;

	if(maxMainDirAngleDif>-0.999f){
		dif.Normalize();
		float3 modMainDir=owner->frontdir*mainDir.z+owner->rightdir*mainDir.x+owner->updir*mainDir.y;
		
//		geometricObjects->AddLine(weaponPos,weaponPos+modMainDir*50,3,0,16);
		if(modMainDir.dot(dif)<maxMainDirAngleDif)
			return false;
	}
	return true;
}

void CWeapon::Init(void)
{
	std::vector<long> args;
	args.push_back(0);
	owner->cob->Call(COBFN_AimFromPrimary+weaponNum,args);
	relWeaponPos=owner->localmodel->GetPiecePos(args[0]);
	weaponPos=owner->pos+owner->frontdir*relWeaponPos.z+owner->updir*relWeaponPos.y+owner->rightdir*relWeaponPos.x;
//	info->AddLine("RelPos %f %f %f",relWeaponPos.x,relWeaponPos.y,relWeaponPos.z);

	if(range>owner->maxRange)
		owner->maxRange=range;

	muzzleFlareSize=min(areaOfEffect*0.2,min(1500.f,damages[0])*0.003);

	if(weaponDef->interceptor){
		interceptHandler.AddInterceptorWeapon(this);
		if(weaponNum==0)	//only do this if its primary weapon
			owner->unitDef->noChaseCategory=0xffffffff;		//prevent anti nuke type units from chasing enemies, might have to change if one has a unit with both interceptors and other weapons
	}

	if(weaponDef->stockpile){
		owner->stockpileWeapon=this;
		owner->commandAI->AddStockpileWeapon(this);
	}
}

void CWeapon::ScriptReady(void)
{
	angleGood=true;
}

void CWeapon::CheckIntercept(void)
{
	targetType=Target_None;

	for(std::list<CWeaponProjectile*>::iterator pi=incoming.begin();pi!=incoming.end();++pi){
		if((*pi)->targeted)
			continue;
		targetType=Target_Intercept;
		interceptTarget=*pi;
		targetPos=(*pi)->pos;

		break;
	}
}

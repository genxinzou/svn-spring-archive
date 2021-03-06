#include "StdAfx.h"
// Builder.cpp: implementation of the CBuilder class.
//
//////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <algorithm>
#include "Builder.h"
#include "Building.h"
#include "myMath.h"
#include "Sound.h"
#include "LogOutput.h"
#include "Game/GameHelper.h"
#include "Game/Team.h"
#include "Map/Ground.h"
#include "Map/MapDamage.h"
#include "Rendering/UnitModels/3DOParser.h"
#include "Sim/Misc/Feature.h"
#include "Sim/Misc/FeatureHandler.h"
#include "Sim/ModInfo.h"
#include "Sim/Projectiles/GfxProjectile.h"
#include "Sim/Units/COB/CobInstance.h"
#include "Sim/Units/UnitDefHandler.h"
#include "Sim/Units/UnitHandler.h"
#include "Sim/Units/UnitLoader.h"
#include "mmgr.h"

using namespace std;


CR_BIND_DERIVED(CBuilder, CUnit, );

CR_REG_METADATA(CBuilder, (
				CR_MEMBER(buildDistance),
				CR_MEMBER(buildSpeed),
				CR_MEMBER(repairSpeed),
				CR_MEMBER(reclaimSpeed),
				CR_MEMBER(resurrectSpeed),
				CR_MEMBER(captureSpeed),
				CR_MEMBER(terraformSpeed),
				CR_MEMBER(curResurrect),
				CR_MEMBER(lastResurrected),
				CR_MEMBER(curBuild),
				CR_MEMBER(curCapture),
				CR_MEMBER(curReclaim),
				CR_MEMBER(helpTerraform),
				CR_MEMBER(terraforming),
				CR_MEMBER(terraformLeft),
				CR_MEMBER(terraformHelp),
				CR_MEMBER(tx1), CR_MEMBER(tx2), CR_MEMBER(tz1), CR_MEMBER(tz2),
				CR_MEMBER(terraformCenter),
				CR_MEMBER(terraformRadius),
				CR_MEMBER(nextBuildType),
				CR_MEMBER(nextBuildPos)));


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBuilder::CBuilder()
:	buildDistance(16),
  buildSpeed(100),
  repairSpeed(100),
  reclaimSpeed(100),
  resurrectSpeed(100),
  captureSpeed(100),
  terraformSpeed(100),
  curBuild(0),
  curReclaim(0),
  terraforming(false),
  terraformLeft(0),
  terraformType(Terraform_Building),
  tx1(0),
  tx2(0),
  tz1(0),
  tz2(0),
  terraformCenter(0,0,0),
  terraformRadius(0),
  nextBuildPos(0,0,0),
  terraformHelp(0),
  helpTerraform(0),
  curResurrect(0),
  lastResurrected(0),
  curCapture(0)
{
}


CBuilder::~CBuilder()
{
}


void CBuilder::UnitInit(UnitDef* def, int team, const float3& position)
{
	buildDistance  = def->buildDistance;

	const float scale = (1.0f / 32.0f);
	buildSpeed     = scale * def->buildSpeed;
	repairSpeed    = scale * def->repairSpeed;
	reclaimSpeed   = scale * def->reclaimSpeed;
	resurrectSpeed = scale * def->resurrectSpeed;
	captureSpeed   = scale * def->captureSpeed;
	terraformSpeed = scale * def->terraformSpeed;

	CUnit::UnitInit (def, team, position);
}


void CBuilder::Update()
{
	if (beingBuilt) {
		return;
	}

	if(terraforming && inBuildStance){
		if(terraformLeft>0){
			float* heightmap = readmap->GetHeightmap();
			assert(!mapDamage->disabled); // The map should not be deformed in the first place.
			float terraformScale = (terraformSpeed + terraformHelp) / terraformLeft;
			terraformLeft -= (terraformSpeed + terraformHelp);
			terraformHelp = 0;
			if (terraformScale > 1.0f) {
				terraformScale = 1.0f;
			}
			switch(terraformType){
			case Terraform_Building:
				if(curBuild)
					curBuild->AddBuildPower(0.001f,this); //prevent building from timing out while terraforming for it
				for(int z=tz1; z<=tz2; z++){
					for(int x=tx1; x<=tx2; x++){
						float ch=heightmap[z*(gs->mapx+1)+x];
						heightmap[z*(gs->mapx+1)+x] += (nextBuildPos.y-ch) * terraformScale;
					}
				}
				break;
			case Terraform_Restore:
				for(int z=tz1; z<=tz2; z++){
					for(int x=tx1; x<=tx2; x++){
						float ch=heightmap[z*(gs->mapx+1)+x];
						float oh=readmap->orgheightmap[z*(gs->mapx+1)+x];
						heightmap[z*(gs->mapx+1)+x] += (oh-ch) * terraformScale;
					}
				}
				break;
			}
			for(int z=tz1; z<=tz2; z++){		//smooth the borders x
				for(int x=1; x<=3; x++){
					if(tx1-3>=0){
						float ch3=heightmap[z*(gs->mapx+1)+tx1];
						float ch=heightmap[z*(gs->mapx+1)+tx1-x];
						float ch2=heightmap[z*(gs->mapx+1)+tx1-3];
						heightmap[z*(gs->mapx+1)+tx1-x] += ((ch3*(3-x)+ch2*x)/3-ch) * terraformScale;
					}
					if(tx2+3<gs->mapx){
						float ch3=heightmap[z*(gs->mapx+1)+tx2];
						float ch=heightmap[z*(gs->mapx+1)+tx2+x];
						float ch2=heightmap[z*(gs->mapx+1)+tx2+3];
						heightmap[z*(gs->mapx+1)+tx2+x] += ((ch3*(3-x)+ch2*x)/3-ch) * terraformScale;
					}
				}
			}
			for(int z=1; z<=3; z++){		//smooth the borders z
				for(int x=tx1; x<=tx2; x++){
					if(tz1-3>=0){
						float ch3=heightmap[(tz1)*(gs->mapx+1)+x];
						float ch=heightmap[(tz1-z)*(gs->mapx+1)+x];
						float ch2=heightmap[(tz1-3)*(gs->mapx+1)+x];
						heightmap[(tz1-z)*(gs->mapx+1)+x] += ((ch3*(3-z)+ch2*z)/3-ch) * terraformScale;
					}
					if(tz2+3<gs->mapy){
						float ch3=heightmap[(tz2)*(gs->mapx+1)+x];
						float ch=heightmap[(tz2+z)*(gs->mapx+1)+x];
						float ch2=heightmap[(tz2+3)*(gs->mapx+1)+x];
						heightmap[(tz2+z)*(gs->mapx+1)+x] += ((ch3*(3-z)+ch2*z)/3-ch) * terraformScale;
					}
				}
			}
			CreateNanoParticle(terraformCenter,terraformRadius*0.5f,false);
		}
		if(terraformLeft<=0){
			terraforming=false;
			mapDamage->RecalcArea(tx1,tx2,tz1,tz2);
			if(terraformType==Terraform_Building){
//				if(curBuild)
//					SetBuildStanceToward(curBuild->midPos);
			} else {
				StopBuild();
			}
		}
	}
	else if (helpTerraform && inBuildStance) {
		if (helpTerraform->terraforming) {
			helpTerraform->terraformHelp += terraformSpeed;
			CreateNanoParticle(helpTerraform->terraformCenter,helpTerraform->terraformRadius*0.5f,false);
		} else {
			DeleteDeathDependence(helpTerraform);
			helpTerraform=0;
			StopBuild(true);
		}
	}
	else if (curBuild && curBuild->pos.distance2D(pos) < buildDistance + curBuild->radius) {
		if (curBuild->soloBuilder && (curBuild->soloBuilder != this)) {
			StopBuild();
		} else {
			if (!inBuildStance) {
				curBuild->AddBuildPower(0.001f, this); //prevent building timing out
			} else {
  			isCloaked = false;
  			curCloakTimeout = gs->frameNum + cloakTimeout;

  			float adjBuildSpeed; // adjusted build speed
  			if (curBuild->buildProgress < 1.0f) {
  				adjBuildSpeed = buildSpeed;  // new build
  			} else {
  				adjBuildSpeed = repairSpeed; // repair
				}

				if (curBuild->AddBuildPower(adjBuildSpeed, this)) {
					CreateNanoParticle(curBuild->midPos, curBuild->radius * 0.5f, false);
				} else {
					if(!curBuild->beingBuilt && curBuild->health >= curBuild->maxHealth) {
						StopBuild();
					}
				}
			}
		}
	}
	else if(curReclaim && curReclaim->pos.distance2D(pos)<buildDistance+curReclaim->radius && inBuildStance){
		isCloaked=false;
		curCloakTimeout=gs->frameNum+cloakTimeout;
		if(curReclaim->AddBuildPower(-reclaimSpeed, this)){
			CreateNanoParticle(curReclaim->midPos,curReclaim->radius*0.7f,true);
		}
	}
	else if(curResurrect && curResurrect->pos.distance2D(pos)<buildDistance+curResurrect->radius && inBuildStance){
		UnitDef* ud=unitDefHandler->GetUnitByName(curResurrect->createdFromUnit);
		if(ud){
			if( modInfo->reclaimMethod != 1 && curResurrect->reclaimLeft < 1)
			{
				// This corpse has been reclaimed a little, need to restore the resources
				// before we can let the player resurrect it.
				curResurrect->AddBuildPower(repairSpeed, this);
			}
			else
			{
				// Corpse has been restored, begin resurrection
				if(UseEnergy(ud->energyCost*resurrectSpeed/ud->buildTime*0.5f)){
					curResurrect->resurrectProgress+=resurrectSpeed/ud->buildTime;
					CreateNanoParticle(curResurrect->midPos,curResurrect->radius*0.7f,gs->randInt()&1);
				}
				if(curResurrect->resurrectProgress>1){		//resurrect finished
					curResurrect->UnBlock();
					CUnit* u = unitLoader.LoadUnit(curResurrect->createdFromUnit, curResurrect->pos,
					                               team, false, curResurrect->buildFacing, this);
					if (!this->unitDef->canBeAssisted) {
						u->soloBuilder = this;
						u->AddDeathDependence(this);
					}
					u->health*=0.05f;
					u->lineage = this->lineage;
					lastResurrected=u->id;
					curResurrect->resurrectProgress=0;
					featureHandler->DeleteFeature(curResurrect);
					StopBuild(true);
				}
			}
		} else {
			StopBuild(true);
		}
	}
	else if(curCapture && curCapture->pos.distance2D(pos)<buildDistance+curCapture->radius && inBuildStance){
		if(curCapture->team!=team){
			curCapture->captureProgress+=1.0f/(150+curCapture->buildTime/captureSpeed*(curCapture->health+curCapture->maxHealth)/curCapture->maxHealth*0.4f);
			CreateNanoParticle(curCapture->midPos,curCapture->radius*0.7f,false);
			if(curCapture->captureProgress > 1.0f){
				if (!curCapture->ChangeTeam(team, CUnit::ChangeCaptured)) {
					// capture failed
					ENTER_MIXED;
					if (team == gu->myTeam) {
						logOutput.Print("%s: Capture failed, unit type limit reached", unitDef->humanName.c_str());
						logOutput.SetLastMsgPos(pos);
					}
					ENTER_SYNCED;
				} else {
					// capture succesful
					int oldLineage = curCapture->lineage;
					curCapture->lineage = this->lineage;
					gs->Team(oldLineage)->LeftLineage(curCapture);
				}
				curCapture->captureProgress=0.5f;	//make units somewhat easier to capture back after first capture
				StopBuild(true);
			}
		} else {
			StopBuild(true);
		}
	}
	CUnit::Update();
}


void CBuilder::SlowUpdate(void)
{
	if(terraforming){
		mapDamage->RecalcArea(tx1,tx2,tz1,tz2);
	}
	CUnit::SlowUpdate();
}


void CBuilder::SetRepairTarget(CUnit* target)
{
	if(target==curBuild)
		return;

	StopBuild(false);
	TempHoldFire();

	curBuild=target;
	AddDeathDependence(curBuild);

	SetBuildStanceToward(target->pos);
}


void CBuilder::SetReclaimTarget(CSolidObject* target)
{
	if(dynamic_cast<CFeature*>(target) && !((CFeature*) target)->def->reclaimable){
		return;
	}

	if(dynamic_cast<CUnit*>(target) && !((CUnit*) target)->unitDef->reclaimable){
		return;
	}

	if(curReclaim==target || this == target){
		return;
	}

	StopBuild(false);
	TempHoldFire();

	curReclaim=target;
	AddDeathDependence(curReclaim);

	SetBuildStanceToward(target->pos);
}


void CBuilder::SetResurrectTarget(CFeature* target)
{
	if(curResurrect==target || target->createdFromUnit=="")
		return;

	StopBuild(false);
	TempHoldFire();

	curResurrect=target;
	AddDeathDependence(curResurrect);

	SetBuildStanceToward(target->pos);
}


void CBuilder::SetCaptureTarget(CUnit* target)
{
	if(target==curCapture)
		return;

	StopBuild(false);
	TempHoldFire();

	curCapture=target;
	AddDeathDependence(curCapture);

	SetBuildStanceToward(target->pos);
}


void CBuilder::StartRestore(float3 centerPos, float radius)
{
	StopBuild(false);
	TempHoldFire();

	terraforming=true;
	terraformType=Terraform_Restore;
	terraformCenter=centerPos;
	terraformRadius=radius;

	tx1 = (int)max((float)0,(centerPos.x-radius)/SQUARE_SIZE);
	tx2 = (int)min((float)gs->mapx,(centerPos.x+radius)/SQUARE_SIZE);
	tz1 = (int)max((float)0,(centerPos.z-radius)/SQUARE_SIZE);
	tz2 = (int)min((float)gs->mapy,(centerPos.z+radius)/SQUARE_SIZE);

	float tcost=0;
	float* heightmap = readmap->GetHeightmap();
	for(int z=tz1; z<=tz2; z++){
		for(int x=tx1; x<=tx2; x++){
			float delta=readmap->orgheightmap[z*(gs->mapx+1)+x]-heightmap[z*(gs->mapx+1)+x];
			tcost+=fabs(delta);
		}
	}
	terraformLeft=tcost;

	SetBuildStanceToward(centerPos);
}


void CBuilder::StopBuild(bool callScript)
{
	if(curBuild)
		DeleteDeathDependence(curBuild);
	if(curReclaim)
		DeleteDeathDependence(curReclaim);
	if(helpTerraform)
		DeleteDeathDependence(helpTerraform);
	if(curResurrect)
		DeleteDeathDependence(curResurrect);
	if(curCapture)
		DeleteDeathDependence(curCapture);
	curBuild=0;
	curReclaim=0;
	helpTerraform=0;
	curResurrect=0;
	curCapture=0;
	terraforming=false;
	if(callScript)
		cob->Call("StopBuilding");
	ReleaseTempHoldFire();
//	logOutput.Print("stop build");
}


bool CBuilder::StartBuild(BuildInfo& buildInfo)
{
	StopBuild(false);
//	logOutput.Print("start build");

	buildInfo.pos=helper->Pos2BuildPos(buildInfo);

	CFeature* feature;
	int canBuild=uh->TestUnitBuildSquare(buildInfo, feature, allyteam);
	if(canBuild<2){
		CUnit* u=helper->GetClosestFriendlyUnit(buildInfo.pos,5,allyteam);
		if(u && u->unitDef==buildInfo.def){
			curBuild=u;
			AddDeathDependence(u);
			SetBuildStanceToward(buildInfo.pos);
			return true;
		}
		return false;
	}
	if(feature)
		return false;

	const UnitDef* unitDef = buildInfo.def;
	if (mapDamage->disabled || !unitDef->levelGround || unitDef->floater ||
	    (unitDef->canmove && (unitDef->speed > 0.0f))) {
		// skip the terraforming job.
		terraformLeft = 0;
	}
	else {
		CalculateBuildTerraformCost(buildInfo);

		terraforming=true;
		terraformType=Terraform_Building;
		terraformCenter=buildInfo.pos;
		terraformRadius=(tx2-tx1)*SQUARE_SIZE;
	}

	SetBuildStanceToward(buildInfo.pos);

	nextBuildType=buildInfo.def->name;
	nextBuildPos=buildInfo.pos;

	CUnit* b = unitLoader.LoadUnit(nextBuildType, nextBuildPos, team,
	                               true, buildInfo.buildFacing, this);
	if (!this->unitDef->canBeAssisted) {
		b->soloBuilder = this;
		b->AddDeathDependence(this);
	}
	b->lineage = this->lineage;
	AddDeathDependence(b);
	curBuild=b;
	if (mapDamage->disabled && !(curBuild->floatOnWater)) {
		/* The ground isn't going to be terraformed.
		 * When the building is completed, it'll 'pop'
		 * into the correct height for the (un-flattened)
		 * terrain it's on.
		 *
		 * To prevent this visual artifact, put the building
		 * at the 'right' height to begin with.
		 *
		 * Duplicated from CMoveType::SlowUpdate(), which
		 * is why we use the regular code for floating things.
		 */
		curBuild->pos.y=ground->GetHeight2(curBuild->pos.x,curBuild->pos.z);
		curBuild->midPos.y=curBuild->pos.y+curBuild->relMidPos.y;
	}
	else {
		float d=nextBuildPos.y-curBuild->pos.y;
		curBuild->pos.y+=d;
		curBuild->midPos.y+=d;
	}

	return true;
}


void CBuilder::CalculateBuildTerraformCost(BuildInfo& buildInfo)
{
	float3& buildPos=buildInfo.pos;
	const UnitDef *unitdef=buildInfo.def;
	tx1 = (int)max((float)0,(buildPos.x-(buildInfo.GetXSize()*0.5f*SQUARE_SIZE))/SQUARE_SIZE);
	tx2 = min(gs->mapx,tx1+buildInfo.GetXSize());
	tz1 = (int)max((float)0,(buildPos.z-(buildInfo.GetYSize()*0.5f*SQUARE_SIZE))/SQUARE_SIZE);
	tz2 = min(gs->mapy,tz1+buildInfo.GetYSize());

	float tcost=0;
	float* heightmap = readmap->GetHeightmap();
	for(int z=tz1; z<=tz2; z++){
		for(int x=tx1; x<=tx2; x++){
			float delta=buildPos.y-heightmap[z*(gs->mapx+1)+x];
			float cost;
			if(delta>0){
				cost=max(3.f,heightmap[z*(gs->mapx+1)+x]-readmap->orgheightmap[z*(gs->mapx+1)+x]+delta*0.5f);
			} else {
				cost=max(3.f,readmap->orgheightmap[z*(gs->mapx+1)+x]-heightmap[z*(gs->mapx+1)+x]-delta*0.5f);
			}
			tcost+=fabs(delta)*cost;
		}
	}

	terraformLeft=tcost;
}


void CBuilder::DependentDied(CObject *o)
{
	if(o==curBuild){
		curBuild=0;
		StopBuild();
	}
	if(o==curReclaim){
		curReclaim=0;
		StopBuild();
	}
	if(o==helpTerraform){
		helpTerraform=0;
		StopBuild();
	}
	if(o==curResurrect){
		curResurrect=0;
		StopBuild();
	}
	if(o==curCapture){
		curCapture=0;
		StopBuild();
	}
	CUnit::DependentDied(o);
}


void CBuilder::SetBuildStanceToward(float3 pos)
{
	float3 wantedDir=(pos-this->pos).Normalize();
	short int h=GetHeadingFromVector(wantedDir.x,wantedDir.z);

	std::vector<int> args;
	args.push_back(short(h-heading));
	cob->Call("StartBuilding",args);

	if(unitDef->sounds.build.id)
		sound->PlaySample(unitDef->sounds.build.id, pos, unitDef->sounds.build.volume);
}


void CBuilder::HelpTerraform(CBuilder* unit)
{
	if(helpTerraform==unit)
		return;

	StopBuild(false);

	helpTerraform=unit;
	AddDeathDependence(helpTerraform);

	SetBuildStanceToward(unit->terraformCenter);
}


void CBuilder::CreateNanoParticle(float3 goal, float radius, bool inverse)
{
	std::vector<int> args;
	args.push_back(0);
	cob->Call("QueryNanoPiece",args);

	if(!unitDef->showNanoSpray)
        return;

	float3 relWeaponFirePos=localmodel->GetPiecePos(args[0]);
	float3 weaponPos=pos + frontdir*relWeaponFirePos.z + updir*relWeaponFirePos.y + rightdir*relWeaponFirePos.x;

	float3 dif=goal-weaponPos;
	float l=dif.Length();
	dif/=l;
	float3 error=gs->randVector()*(radius/l);

	float3 color= unitDef->nanoColor;
	if(gu->teamNanospray){
		unsigned char* tcol=gs->Team(team)->color;
		color = float3(tcol[0]*(1.f/255.f),tcol[1]*(1.f/255.f),tcol[2]*(1.f/255.f));
	}

	if(inverse)
		SAFE_NEW CGfxProjectile(weaponPos+(dif+error)*l,-(dif+error)*3,(int)(l/3),color);
	else
		SAFE_NEW CGfxProjectile(weaponPos,(dif+error)*3,(int)(l/3),color);
}

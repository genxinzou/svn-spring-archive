// UnitLoader.cpp: implementation of the CUnitLoader class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Rendering/GL/myGL.h"
#include "mmgr.h"

#include "UnitLoader.h"
#include "Unit.h"
#include "UnitDefHandler.h"
#include "UnitTypes/Builder.h"
#include "UnitTypes/ExtractorBuilding.h"
#include "UnitTypes/Factory.h"
#include "UnitTypes/TransportUnit.h"
#include "COB/CobEngine.h"
#include "COB/CobFile.h"
#include "COB/CobInstance.h"
#include "CommandAI/AirCAI.h"
#include "CommandAI/BuilderCAI.h"
#include "CommandAI/CommandAI.h"
#include "CommandAI/FactoryCAI.h"
#include "CommandAI/MobileCAI.h"
#include "CommandAI/TransportCAI.h"
#include "Game/GameHelper.h"
#include "Map/Ground.h"
#include "Map/MapDamage.h"
#include "Map/MapInfo.h"
#include "Map/ReadMap.h"
#include "Platform/errorhandler.h"
#include "Rendering/UnitModels/IModelParser.h"
#include "Sim/Misc/CollisionVolume.h"
#include "Sim/MoveTypes/AirMoveType.h"
#include "Sim/MoveTypes/GroundMoveType.h"
#include "Sim/MoveTypes/TAAirMoveType.h"
#include "Sim/Weapons/BeamLaser.h"
#include "Sim/Weapons/bombdropper.h"
#include "Sim/Weapons/Cannon.h"
#include "Sim/Weapons/DGunWeapon.h"
#include "Sim/Weapons/EmgCannon.h"
#include "Sim/Weapons/FlameThrower.h"
#include "Sim/Weapons/LaserCannon.h"
#include "Sim/Weapons/LightningCannon.h"
#include "Sim/Weapons/MeleeWeapon.h"
#include "Sim/Weapons/MissileLauncher.h"
#include "Sim/Weapons/NoWeapon.h"
#include "Sim/Weapons/PlasmaRepulser.h"
#include "Sim/Weapons/Rifle.h"
#include "Sim/Weapons/StarburstLauncher.h"
#include "Sim/Weapons/TorpedoLauncher.h"
#include "Sim/Weapons/WeaponDefHandler.h"
#include "Sound.h"
#include "myMath.h"
#include "LogOutput.h"
#include "Exceptions.h"
#include "TimeProfiler.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUnitLoader unitLoader;

CUnitLoader::CUnitLoader()
{
	CGroundMoveType::CreateLineTable();
}

CUnitLoader::~CUnitLoader()
{
	CGroundMoveType::DeleteLineTable();
}

CUnit* CUnitLoader::LoadUnit(const string& name, float3 pos, int team,
                             bool build, int facing, const CUnit* builder)
{
//	GML_RECMUTEX_LOCK(unit); // LoadUnit. Unitinit puts unit in the quadfield and activeUnits -
	GML_RECMUTEX_LOCK(sel); // LoadUnit. For anti deadlock purposes.
	GML_RECMUTEX_LOCK(quad); // LoadUnit. - make sure other threads cannot access an incomplete unit

	CUnit* unit;

	SCOPED_TIMER("Unit loader");

	const UnitDef* ud = unitDefHandler->GetUnitByName(name);
	if (!ud) {
		throw content_error("Couldn't find unittype " +  name);
	}

	string type = ud->type;

	if (!build) {
		pos.y = ground->GetHeight2(pos.x, pos.z);
		if (ud->floater && pos.y < 0.0f) {
			// adjust to waterline iif we are submerged
 			pos.y = -ud->waterline;
		}
	}

	if (team < 0) {
		team = MAX_TEAMS - 1; // FIXME use gs->gaiaTeamID ?  (once it is always enabled)
	}

	if (type == "GroundUnit") {
		unit = SAFE_NEW CUnit;
	} else if (type == "Transport") {
		unit = SAFE_NEW CTransportUnit;
	} else if (type == "Building") {
		unit = SAFE_NEW CBuilding;
	} else if (type == "Factory") {
		unit = SAFE_NEW CFactory;
	} else if (type == "Builder") {
		unit = SAFE_NEW CBuilder;
	} else if (type == "Bomber" || type == "Fighter") {
		unit = SAFE_NEW CUnit;
	} else if (type == "MetalExtractor") {
		unit = SAFE_NEW CExtractorBuilding;
	} else {
		logOutput << "Unknown unit type " << type.c_str() << "\n";
		return NULL;
	}

	unit->UnitInit(ud, team, pos);

	unit->beingBuilt = build;

	unit->xsize = ((facing & 1) == 0) ? ud->xsize : ud->zsize;
	unit->zsize = ((facing & 1) == 1) ? ud->xsize : ud->zsize;
	unit->buildFacing = facing;
	unit->power = ud->power;
	unit->maxHealth = ud->health;
	unit->health = ud->health;
	//unit->metalUpkeep = ud->metalUpkeep*16.0f/GAME_SPEED;
	//unit->energyUpkeep = ud->energyUpkeep*16.0f/GAME_SPEED;
	unit->controlRadius = (int)(ud->controlRadius / SQUARE_SIZE);
	unit->losHeight = ud->losHeight;
	unit->metalCost = ud->metalCost;
	unit->energyCost = ud->energyCost;
	unit->buildTime = ud->buildTime;
	unit->aihint = ud->aihint;
	unit->tooltip = ud->humanName + " - " + ud->tooltip;
	unit->armoredMultiple = std::max(0.0001f, ud->armoredMultiple);		//armored multiple of 0 will crash spring
	unit->wreckName = ud->wreckName;

	unit->realLosRadius = (int) (ud->losRadius);
	unit->realAirLosRadius = (int) (ud->airLosRadius);
	unit->upright = ud->upright;
	unit->radarRadius      = ud->radarRadius    / (SQUARE_SIZE * 8);
	unit->sonarRadius      = ud->sonarRadius    / (SQUARE_SIZE * 8);
	unit->jammerRadius     = ud->jammerRadius   / (SQUARE_SIZE * 8);
	unit->sonarJamRadius   = ud->sonarJamRadius / (SQUARE_SIZE * 8);
	unit->seismicRadius    = ud->seismicRadius  / (SQUARE_SIZE * 8);
	unit->seismicSignature = ud->seismicSignature;
	unit->hasRadarCapacity = unit->radarRadius  || unit->sonarRadius    ||
	                         unit->jammerRadius || unit->sonarJamRadius ||
	                         unit->seismicRadius;
	unit->stealth = ud->stealth;
	unit->sonarStealth = ud->sonarStealth;
	unit->category = ud->category;
	unit->armorType = ud->armorType;
	unit->floatOnWater =
		ud->floater || (ud->movedata && ((ud->movedata->moveType == MoveData::Hover_Move) ||
		                                 (ud->movedata->moveType == MoveData::Ship_Move)));
	unit->maxSpeed = ud->speed / 30.0f;
	unit->decloakDistance = ud->decloakDistance;

	unit->flankingBonusMode        = ud->flankingBonusMode;
	unit->flankingBonusDir         = ud->flankingBonusDir;
	unit->flankingBonusMobility    = ud->flankingBonusMobilityAdd * 1000;
	unit->flankingBonusMobilityAdd = ud->flankingBonusMobilityAdd;
	unit->flankingBonusAvgDamage = (ud->flankingBonusMax + ud->flankingBonusMin) * 0.5f;
	unit->flankingBonusDifDamage = (ud->flankingBonusMax - ud->flankingBonusMin) * 0.5f;


	if(ud->highTrajectoryType==1)
		unit->useHighTrajectory=true;

	if(ud->fireState >= 0)
		unit->fireState = ud->fireState;

	if(build){
		unit->ChangeLos(1,1);
		unit->health=0.1f;
	} else {
		unit->ChangeLos((int)(ud->losRadius),(int)(ud->airLosRadius));
	}

	if (type == "GroundUnit") {
		SAFE_NEW CMobileCAI(unit);
	}
	else if (type == "Transport") {
		SAFE_NEW CTransportCAI(unit);
	}
	else if (type == "Factory") {
		SAFE_NEW CFactoryCAI(unit);
	}
	else if (type == "Builder") {
		SAFE_NEW CBuilderCAI(unit);
	}
	else if (type == "Bomber") {
		if (ud->hoverAttack) {
			SAFE_NEW CMobileCAI(unit);
		} else {
			SAFE_NEW CAirCAI(unit);
		}
	}
	else if(type == "Fighter"){
		if (ud->hoverAttack) {
			SAFE_NEW CMobileCAI(unit);
		} else {
			SAFE_NEW CAirCAI(unit);
		}
	}
	else {
		SAFE_NEW CCommandAI(unit);
	}

	if (ud->canmove && !ud->canfly && (type != "Factory")) {
		CGroundMoveType* mt = SAFE_NEW CGroundMoveType(unit);
		mt->maxSpeed = ud->speed / GAME_SPEED;
		mt->maxWantedSpeed = ud->speed / GAME_SPEED;
		mt->turnRate = ud->turnRate;
		mt->baseTurnRate = ud->turnRate;

		if (!mt->accRate) {
			logOutput << "acceleration of " << ud->name.c_str() << " is zero!!\n";
		}

		mt->moveType = ud->moveType;
		mt->accRate = ud->maxAcc;
		mt->decRate = ud->maxDec;
		mt->floatOnWater = (ud->movedata->moveType == MoveData::Hover_Move ||
		                    ud->movedata->moveType == MoveData::Ship_Move);

		if (!unit->beingBuilt) {
			// otherwise set this when finished building instead
			unit->mass = ud->mass;
		}
		unit->moveType = mt;

		// Ground-mobility
		unit->mobility = SAFE_NEW MoveData(ud->movedata, GAME_SPEED);

	} else if (ud->canfly) {
		// Air-mobility
		unit->mobility = SAFE_NEW MoveData(ud->movedata, GAME_SPEED);

		if (!unit->beingBuilt) {
			// otherwise set this when finished building instead
			unit->mass = ud->mass;
		}

		if ((type == "Builder") || ud->hoverAttack || ud->transportCapacity) {
			CTAAirMoveType* mt = SAFE_NEW CTAAirMoveType(unit);

			mt->turnRate = ud->turnRate;
			mt->maxSpeed = ud->speed / GAME_SPEED;
			mt->accRate = ud->maxAcc;
			mt->decRate = ud->maxDec;
			mt->wantedHeight = ud->wantedHeight + gs->randFloat() * 5;
			mt->orgWantedHeight = mt->wantedHeight;
			mt->dontLand = ud->DontLand();
			mt->collide = ud->collide;
			mt->altitudeRate = ud->verticalSpeed;
			mt->bankingAllowed = ud->bankingAllowed;

			unit->moveType = mt;
		}
		else {
			CAirMoveType *mt = SAFE_NEW CAirMoveType(unit);

			if(type=="Fighter")
				mt->isFighter=true;

			mt->collide = ud->collide;

			mt->wingAngle = ud->wingAngle;
			mt->crashDrag = 1 - ud->crashDrag;
			mt->invDrag = 1 - ud->drag;
			mt->frontToSpeed = ud->frontToSpeed;
			mt->speedToFront = ud->speedToFront;
			mt->myGravity = ud->myGravity;

			mt->maxBank = ud->maxBank;
			mt->maxPitch = ud->maxPitch;
			mt->turnRadius = ud->turnRadius;
			mt->wantedHeight = (ud->wantedHeight * 1.5f) +
			                   ((gs->randFloat() - 0.3f) * 15 * (mt->isFighter ? 2 : 1));

			mt->maxAcc = ud->maxAcc;
			mt->maxAileron = ud->maxAileron;
			mt->maxElevator = ud->maxElevator;
			mt->maxRudder = ud->maxRudder;

			unit->moveType = mt;
		}
	} else {
		unit->moveType = SAFE_NEW CMoveType(unit);
		unit->upright = true;
	}

	unit->energyTickMake = ud->energyMake;

	if (ud->tidalGenerator > 0)
		unit->energyTickMake += ud->tidalGenerator * mapInfo->map.tidalStrength;


	unit->model = LoadModel(ud);
	unit->SetRadius(unit->model->radius);

	// copy the UnitDef volume archetype data
	unit->collisionVolume = SAFE_NEW CollisionVolume(ud->collisionVolume);

	// if no "collisionVolumeScales" tag was defined in UnitDef,
	// the default scale for this volume will be a ZeroVector
	if (unit->collisionVolume->GetScale(COLVOL_AXIS_X) <= 1.0f &&
		unit->collisionVolume->GetScale(COLVOL_AXIS_Y) <= 1.0f &&
		unit->collisionVolume->GetScale(COLVOL_AXIS_Z) <= 1.0f) {
		// aircraft still get half-size spheres for coldet purposes
		// if no custom volume is defined (unit->model->radius and
		// unit->radius themselves are no longer altered)
		const float scaleFactor = (ud->canfly)? 0.5f: 1.0f;
		unit->collisionVolume->SetDefaultScale(unit->model->radius * scaleFactor);

		if (unit->collisionVolume->GetBoundingRadius() <= 30.0f) {
			// the interval-based method fails too easily for units
			// with small default volumes, force use of raytracing
			unit->collisionVolume->SetTestType(COLVOL_TEST_CONT);
		}
	}

	if (ud->floater) {
		// restrict our depth to our waterline
		unit->pos.y = std::max(-ud->waterline, ground->GetHeight2(unit->pos.x, unit->pos.z));
	} else {
		unit->pos.y = ground->GetHeight2(unit->pos.x, unit->pos.z);
	}

	modelParser->CreateLocalModel(unit);
	unit->cob = SAFE_NEW CCobInstance(GCobEngine.GetCobFile(ud->scriptPath), unit);

	unit->weapons.reserve(ud->weapons.size());
	for (unsigned int i = 0; i < ud->weapons.size(); i++) {
		unit->weapons.push_back(LoadWeapon(ud->weapons[i].def, unit, &ud->weapons[i]));
	}

	// Calculate the max() of the available weapon reloadtimes
	int relMax = 0;
	for (vector<CWeapon*>::iterator i = unit->weapons.begin(); i != unit->weapons.end(); ++i) {
		if ((*i)->reloadTime > relMax)
			relMax = (*i)->reloadTime;
		if (dynamic_cast<CBeamLaser*>(*i))
			relMax = 150;
	}

	// convert ticks to milliseconds
	relMax *= 30;

	// TA does some special handling depending on weapon count
	if (unit->weapons.size() > 1) {
		relMax = std::max(relMax, 3000);
	}

	// Call initializing script functions
	unit->cob->Call(COBFN_Create);
	unit->cob->Call("SetMaxReloadTime", relMax);

	unit->heading = GetHeadingFromFacing(facing);
	unit->frontdir = GetVectorFromHeading(unit->heading);
	unit->updir = UpVector;
	unit->rightdir = unit->frontdir.cross(unit->updir);

	unit->yardMap = ud->yardmaps[facing];

	unit->Init(builder);

	if (!build) {
		unit->FinishedBuilding();
	}
	return unit;
}

CWeapon* CUnitLoader::LoadWeapon(const WeaponDef *weapondef, CUnit* owner, const UnitDef::UnitDefWeapon* udw)
{
	CWeapon* weapon;

	if (!weapondef) {
		logOutput.Print("Error: No weapon def?");
	}

	if (udw->name == "NOWEAPON") {
		weapon = SAFE_NEW CNoWeapon(owner);
	} else if (weapondef->type == "Cannon") {
		weapon = SAFE_NEW CCannon(owner);
		((CCannon*)weapon)->selfExplode = weapondef->selfExplode;
	} else if (weapondef->type == "Rifle") {
		weapon = SAFE_NEW CRifle(owner);
	} else if (weapondef->type == "Melee") {
		weapon = SAFE_NEW CMeleeWeapon(owner);
	} else if (weapondef->type == "AircraftBomb") {
		weapon = SAFE_NEW CBombDropper(owner, false);
	} else if (weapondef->type == "Shield") {
		weapon = SAFE_NEW CPlasmaRepulser(owner);
	} else if (weapondef->type == "Flame") {
		weapon = SAFE_NEW CFlameThrower(owner);
	} else if (weapondef->type == "MissileLauncher") {
		weapon = SAFE_NEW CMissileLauncher(owner);
	} else if (weapondef->type == "TorpedoLauncher") {
		if (owner->unitDef->canfly && !weapondef->submissile) {
			weapon = SAFE_NEW CBombDropper(owner, true);
			if (weapondef->tracks)
				((CBombDropper*) weapon)->tracking = weapondef->turnrate;
			((CBombDropper*) weapon)->bombMoveRange = weapondef->range;
		} else {
			weapon = SAFE_NEW CTorpedoLauncher(owner);
			if (weapondef->tracks)
				((CTorpedoLauncher*) weapon)->tracking = weapondef->turnrate;
		}
	} else if (weapondef->type == "LaserCannon") {
		weapon = SAFE_NEW CLaserCannon(owner);
		((CLaserCannon*) weapon)->color = weapondef->visuals.color;
	} else if (weapondef->type == "BeamLaser") {
		weapon = SAFE_NEW CBeamLaser(owner);
		((CBeamLaser*) weapon)->color = weapondef->visuals.color;
	} else if (weapondef->type == "LightningCannon") {
		weapon = SAFE_NEW CLightningCannon(owner);
		((CLightningCannon*) weapon)->color = weapondef->visuals.color;
	} else if (weapondef->type == "EmgCannon") {
		weapon = SAFE_NEW CEmgCannon(owner);
	} else if (weapondef->type == "DGun") {
		weapon = SAFE_NEW CDGunWeapon(owner);
	} else if (weapondef->type == "StarburstLauncher"){
		weapon = SAFE_NEW CStarburstLauncher(owner);
		if (weapondef->tracks)
			((CStarburstLauncher*) weapon)->tracking = weapondef->turnrate;
		else
			((CStarburstLauncher*) weapon)->tracking = 0;
		((CStarburstLauncher*) weapon)->uptime = weapondef->uptime * GAME_SPEED;
	} else {
		logOutput << "Unknown weapon type " << weapondef->type.c_str() << "\n";
		return 0;
	}
	weapon->weaponDef = weapondef;

	weapon->reloadTime = (int) (weapondef->reload * GAME_SPEED);
	if (weapon->reloadTime == 0)
		weapon->reloadTime = 1;
	weapon->range = weapondef->range;
//	weapon->baseRange = weapondef->range;
	weapon->heightMod = weapondef->heightmod;
	weapon->projectileSpeed = weapondef->projectilespeed;
//	weapon->baseProjectileSpeed = weapondef->projectilespeed / GAME_SPEED;

	weapon->areaOfEffect = weapondef->areaOfEffect;
	weapon->accuracy = weapondef->accuracy;
	weapon->sprayAngle = weapondef->sprayAngle;

	weapon->stockpileTime = (int) (weapondef->stockpileTime * GAME_SPEED);

	weapon->salvoSize = weapondef->salvosize;
	weapon->salvoDelay = (int) (weapondef->salvodelay * GAME_SPEED);
	weapon->projectilesPerShot = weapondef->projectilespershot;

	weapon->metalFireCost = weapondef->metalcost;
	weapon->energyFireCost = weapondef->energycost;

	weapon->fireSoundId = weapondef->firesound.getID(0);
	weapon->fireSoundVolume = weapondef->firesound.getVolume(0);

	weapon->onlyForward = weapondef->onlyForward;
	if (owner->unitDef->type == "Fighter" && !owner->unitDef->hoverAttack) {
		// fighter aircraft have too big tolerance in TA
		weapon->maxAngleDif = cos(weapondef->maxAngle * 0.4f / 180 * PI);
	} else {
		weapon->maxAngleDif = cos(weapondef->maxAngle / 180 * PI);
	}

	weapon->SetWeaponNum(owner->weapons.size());

	weapon->badTargetCategory = udw->badTargetCat;
	weapon->onlyTargetCategory = weapondef->onlyTargetCategory & udw->onlyTargetCat;

	if (udw->slavedTo) {
		const int index = (udw->slavedTo - 1);
		if ((index < 0) || (index >= owner->weapons.size())) {
			throw content_error("Bad weapon slave in " + owner->unitDef->name);
		}
		weapon->slavedTo = owner->weapons[index];
	}

	weapon->mainDir = udw->mainDir;
	weapon->maxMainDirAngleDif = udw->maxAngleDif;

	weapon->fuelUsage = udw->fuelUsage;
	weapon->avoidFriendly = weapondef->avoidFriendly;
	weapon->avoidFeature = weapondef->avoidFeature;
	weapon->avoidNeutral = weapondef->avoidNeutral;
	weapon->targetBorder = weapondef->targetBorder;
	weapon->cylinderTargetting = weapondef->cylinderTargetting;
	weapon->minIntensity = weapondef->minIntensity;
	weapon->heightBoostFactor = weapondef->heightBoostFactor;
	weapon->collisionFlags = weapondef->collisionFlags;
	weapon->Init();

	return weapon;
}


void CUnitLoader::FlattenGround(const CUnit* unit)
{
	const UnitDef* unitDef = unit->unitDef;
	const float groundheight = ground->GetHeight2(unit->pos.x, unit->pos.z);

	if (!mapDamage->disabled && unitDef->levelGround &&
		!(unitDef->floater && groundheight <= 0) &&
		!(unitDef->canmove && (unitDef->speed > 0.0f))) {
		// if we are float-capable, only flatten
		// if the terrain here is above sea level

		BuildInfo bi(unitDef, unit->pos, unit->buildFacing);
		bi.pos = helper->Pos2BuildPos(bi);
		const float hss = 0.5f * SQUARE_SIZE;
		const int tx1 = (int) std::max(0.0f ,(bi.pos.x - (bi.GetXSize() * hss)) / SQUARE_SIZE);
		const int tz1 = (int) std::max(0.0f ,(bi.pos.z - (bi.GetZSize() * hss)) / SQUARE_SIZE);
		const int tx2 = std::min(gs->mapx, tx1 + bi.GetXSize());
		const int tz2 = std::min(gs->mapy, tz1 + bi.GetZSize());

		for (int z = tz1; z <= tz2; z++) {
			for (int x = tx1; x <= tx2; x++) {
				readmap->SetHeight(z * (gs->mapx + 1) + x, bi.pos.y);
			}
		}

		mapDamage->RecalcArea(tx1, tx2, tz1, tz2);
	}
}

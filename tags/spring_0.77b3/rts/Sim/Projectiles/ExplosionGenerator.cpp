#include "StdAfx.h"
#include <fstream>
#include <stdexcept>
#include <SDL_types.h>
#include "mmgr.h"

#include "creg/VarTypes.h"
#include "ExplosionGenerator.h"
#include "FileSystem/FileHandler.h"
#include "Game/Camera.h"
#include "LogOutput.h"
#include "Map/Ground.h"
#include "Platform/ConfigHandler.h"
#include "Rendering/GL/myGL.h"
#include "Rendering/GroundFlash.h"
#include "Rendering/Textures/ColorMap.h"
#include "Sim/Projectiles/ProjectileHandler.h"
#include "Sim/Projectiles/Unsynced/BubbleProjectile.h"
#include "Sim/Projectiles/Unsynced/DirtProjectile.h"
#include "Sim/Projectiles/Unsynced/ExploSpikeProjectile.h"
#include "Sim/Projectiles/Unsynced/HeatCloudProjectile.h"
#include "Sim/Projectiles/Unsynced/SmokeProjectile2.h"
#include "Sim/Projectiles/Unsynced/SpherePartProjectile.h"
#include "Sim/Projectiles/Unsynced/WakeProjectile.h"
#include "Sim/Projectiles/Unsynced/WreckProjectile.h"
#include <assert.h>
#include "System/Exceptions.h"

using std::min;
using std::max;

CR_BIND_DERIVED_INTERFACE(CExpGenSpawnable, CWorldObject);

CR_REG_METADATA(CExpGenSpawnable,
);

CExplosionGeneratorHandler* explGenHandler = NULL;


// -------------------------------------------------------------------------------
// ClassAliasList: Finds C++ classes with class aliases
// -------------------------------------------------------------------------------


ClassAliasList::ClassAliasList() {}


void ClassAliasList::Load(const LuaTable& aliasTable)
{
	map<string, string> aliasList;
	aliasTable.GetMap(aliasList);
	aliases.insert(aliasList.begin(), aliasList.end());
}


creg::Class* ClassAliasList::GetClass(const string& name)
{
	string n = name;
	for (;;) {
		map<string,string>::iterator i = aliases.find(n);
		if (i == aliases.end()) {
			break;
		}
		n = i->second;
	}
	creg::Class *cls = creg::System::GetClass(n);
	if (!cls) {
		throw content_error("Unknown class: " + name);
	}
	return cls;
}

string ClassAliasList::FindAlias(const string& className)
{
	for (map<string,string>::iterator i = aliases.begin(); i != aliases.end(); ++i)
		if (i->second == className) return i->first;
	return className;
}


// -------------------------------------------------------------------------------
// Explosion generator handler: loads and stores a list of explosion generators
// -------------------------------------------------------------------------------

CExplosionGeneratorHandler::CExplosionGeneratorHandler()
: luaParser("gamedata/explosions.lua",
	          SPRING_VFS_MOD_BASE, SPRING_VFS_ZIP)

{
	LuaParser aliasParser("gamedata/explosion_alias.lua",
	                      SPRING_VFS_MOD_BASE, SPRING_VFS_ZIP);
	if (!aliasParser.Execute()) {
		logOutput.Print(aliasParser.GetErrorLog());
	} else {
		const LuaTable root = aliasParser.GetRoot();
		projectileClasses.Load(root.SubTable("projectiles"));
		generatorClasses.Load(root.SubTable("generators"));
	}

	if (!luaParser.Execute()) {
		logOutput.Print(luaParser.GetErrorLog());
	} else {
		luaTable = luaParser.GetRoot();
	}
}


CExplosionGenerator* CExplosionGeneratorHandler::LoadGenerator(const string& tag)
{
	string klass;
	string::size_type seppos = tag.find(':');
	if (seppos == string::npos) {
		klass = tag;
	} else {
		klass = tag.substr(0, seppos);
	}

	creg::Class *cls = generatorClasses.GetClass(klass);
	if (!cls->IsSubclassOf(CExplosionGenerator::StaticClass())) {
		throw content_error(klass + " is not a subclass of CExplosionGenerator");
	}

	CExplosionGenerator* eg = (CExplosionGenerator *)cls->CreateInstance();

	if (seppos != string::npos) {
		eg->Load(this, tag.substr(seppos + 1));
	}

	return eg;
}


// -------------------------------------------------------------------------------
// Base explosion generator class
// -------------------------------------------------------------------------------

CR_BIND_INTERFACE(CExplosionGenerator);


CExplosionGenerator::CExplosionGenerator()
{}


CExplosionGenerator::~CExplosionGenerator()
{}


// -------------------------------------------------------------------------------
// Default explosion generator: everything is calculated from damage and radius
// -------------------------------------------------------------------------------

CR_BIND_DERIVED(CStdExplosionGenerator, CExplosionGenerator, );


CStdExplosionGenerator::CStdExplosionGenerator()
{}


CStdExplosionGenerator::~CStdExplosionGenerator()
{}


void CStdExplosionGenerator::Load(CExplosionGeneratorHandler *h, const string& tag)
{
}


void CStdExplosionGenerator::Explosion(const float3 &pos, float damage,
                                       float radius, CUnit *owner,float gfxMod,
                                       CUnit *hit, const float3 &dir)
{
	PUSH_CODE_MODE;
	ENTER_MIXED;
	float h2=ground->GetHeight2(pos.x,pos.z);

	float height=pos.y-h2;
	if (height < 0.0f) {
		height = 0.0f;
	}

	bool waterExplosion = h2 < -3;
	bool uwExplosion = pos.y < -15;
	bool airExplosion = pos.y - max((float)0, h2) > 20;

	damage=damage/20;
	if (damage>radius*1.5f) //limit the visual effects based on the radius
		damage=radius*1.5f;
	damage*=gfxMod;
	for (int a=0;a<1;++a) {
//		float3 speed((gs->randFloat()-0.5f)*(radius*0.04f),0.05f+(gs->randFloat())*(radius*0.007f),(gs->randFloat()-0.5f)*(radius*0.04f));
		float3 speed(0,0.3f,0);
		float3 camVect=camera->pos-pos;
		float camLength=camVect.Length();
		camVect/=camLength;
		float moveLength=radius*0.03f;
		if (camLength<moveLength+2)
			moveLength=camLength-2;
		float3 npos=pos+camVect*moveLength;

		SAFE_NEW CHeatCloudProjectile(npos,speed,8+sqrt(damage)*0.5f,7+damage*2.8f,owner);
	}
	if (ph->particleSaturation<1) {		//turn off lots of graphic only particles when we have more particles than we want
		float smokeDamage=damage;
		if (uwExplosion)
			smokeDamage*=0.3f;
		if (airExplosion || waterExplosion)
			smokeDamage*=0.6f;
		float invSqrtsmokeDamage=1/(sqrt(smokeDamage)*0.35f);
		for (int a=0;a<smokeDamage*0.6f;++a) {
			float3 speed(-0.1f+gu->usRandFloat()*0.2f,(0.1f+gu->usRandFloat()*0.3f)*invSqrtsmokeDamage,-0.1f+gu->usRandFloat()*0.2f);
			float3 npos(pos+gu->usRandVector()*(smokeDamage*1.0f));
			float h=ground->GetApproximateHeight(npos.x,npos.z);
			if (npos.y<h)
				npos.y=h;
			float time=(40+sqrt(smokeDamage)*15)*(0.8f+gu->usRandFloat()*0.7f);
			SAFE_NEW CSmokeProjectile2(pos,npos,speed,time,sqrt(smokeDamage)*4,0.4f,owner,0.6f);
		}
		if (!airExplosion && !uwExplosion && !waterExplosion) {
			int numDirt=(int)min(20.f,damage*0.8f);
			float3 color(0.15f,0.1f,0.05f);
			for (int a=0;a<numDirt;++a) {
				float3 speed((0.5f-gu->usRandFloat())*1.5f,1.7f+gu->usRandFloat()*1.6f,(0.5f-gu->usRandFloat())*1.5f);
				speed*=0.7f+min((float)30,damage)/30;
				float3 npos(pos.x-(0.5f-gu->usRandFloat())*(radius*0.6f),pos.y-2.0f-damage*0.2f,pos.z-(0.5f-gu->usRandFloat())*(radius*0.6f));
				SAFE_NEW CDirtProjectile(npos,speed,90+damage*2,2.0f+sqrt(damage)*1.5f,0.4f,0.999f,owner,color);
			}
		}
		if (!airExplosion && !uwExplosion && waterExplosion) {
			int numDirt=(int)min(40.f,damage*0.8f);
			float3 color(1,1,1);
			for (int a=0;a<numDirt;++a) {
				float3 speed((0.5f-gu->usRandFloat())*0.2f,a*0.1f+gu->usRandFloat()*0.8f,(0.5f-gu->usRandFloat())*0.2f);
				speed*=0.7f+min((float)30,damage)/30;
				float3 npos(pos.x-(0.5f-gu->usRandFloat())*(radius*0.2f),pos.y-2.0f-sqrt(damage)*2.0f,pos.z-(0.5f-gu->usRandFloat())*(radius*0.2f));
				SAFE_NEW CDirtProjectile(npos,speed,90+damage*2,2.0f+sqrt(damage)*2.0f,0.3f,0.99f,owner,color);
			}
		}
		if (damage>=20 && !uwExplosion && !airExplosion) {
			int numDebris=gu->usRandInt()%6;
			if (numDebris>0)
				numDebris+=3+(int)(damage*0.04f);
			for (int a=0;a<numDebris;++a) {
				float3 speed;
				if (height<4)
					speed=float3((0.5f-gu->usRandFloat())*2.0f,1.8f+gu->usRandFloat()*1.8f,(0.5f-gu->usRandFloat())*2.0f);
				else
					speed=float3(gu->usRandVector()*2);
				speed*=0.7f+min((float)30,damage)/23;
				float3 npos(pos.x-(0.5f-gu->usRandFloat())*(radius*1),pos.y,pos.z-(0.5f-gu->usRandFloat())*(radius*1));
				SAFE_NEW CWreckProjectile(npos,speed,90+damage*2,owner);
			}
		}
		if (uwExplosion) {
			int numBubbles=(int)(damage*0.7f);
			for (int a=0;a<numBubbles;++a) {
				SAFE_NEW CBubbleProjectile(pos+gu->usRandVector()*radius*0.5f,gu->usRandVector()*0.2f+float3(0,0.2f,0),damage*2+gu->usRandFloat()*damage,1+gu->usRandFloat()*2,0.02f,owner,0.5f+gu->usRandFloat()*0.3f);
			}
		}
		if (waterExplosion && !uwExplosion && !airExplosion) {
			int numWake=(int)(damage*0.5f);
			for (int a=0;a<numWake;++a) {
				SAFE_NEW CWakeProjectile(pos+gu->usRandVector()*radius*0.2f,gu->usRandVector()*radius*0.003f,sqrt(damage)*4,damage*0.03f,owner,0.3f+gu->usRandFloat()*0.2f,0.8f/(sqrt(damage)*3+50+gu->usRandFloat()*90),1);
			}
		}
		if (radius>10 && damage>4) {
			int numSpike=(int)sqrt(damage)+8;
			for (int a=0;a<numSpike;++a) {
				float3 speed=gu->usRandVector();
				speed.Normalize();
				speed*=(8+damage*3.0f)/(9+sqrt(damage)*0.7f)*0.35f;
				if (!airExplosion && !waterExplosion && speed.y<0)
					speed.y=-speed.y;
				SAFE_NEW CExploSpikeProjectile(pos+speed,speed*(0.9f+gu->usRandFloat()*0.4f),radius*0.1f,radius*0.1f,0.6f,0.8f/(8+sqrt(damage)),owner);
			}
		}
	}

	if (radius > 20 && damage > 6 && height < radius * 0.7f) {
		float modSize=max(radius,damage*2);
		float circleAlpha=0;
		float circleGrowth=0;
		float ttl=8+sqrt(damage)*0.8f;
		if (radius>40 && damage>12) {
			circleAlpha=min(0.5f,damage*0.01f);
			circleGrowth=(8+damage*2.5f)/(9+sqrt(damage)*0.7f)*0.55f;
		}
		float flashSize=modSize;
		float flashAlpha=min(0.8f,damage*0.01f);
		SAFE_NEW CStandardGroundFlash(pos,circleAlpha,flashAlpha,flashSize,circleGrowth,ttl);
	}

	if (radius > 40 && damage > 12) {
		CSpherePartProjectile::CreateSphere(pos,min(0.7f,damage*0.02f),5+(int)(sqrt(damage)*0.7f),(8+damage*2.5f)/(9+sqrt(damage)*0.7f)*0.5f,owner);
	}
	POP_CODE_MODE;
}

// -------------------------------------------------------------------------------
// CCustomExplosionGenerator: Uses explosion info from a TDF file
// -------------------------------------------------------------------------------

CR_BIND_DERIVED(CCustomExplosionGenerator, CStdExplosionGenerator, );

#define SPW_WATER 1
#define SPW_GROUND 2
#define SPW_AIR 4
#define SPW_UNDERWATER 8
#define SPW_UNIT 16 // only execute when the explosion hits a unit
#define SPW_NO_UNIT 32 // only execute when the explosion doesn't hit a unit (environment)

CCustomExplosionGenerator::CCustomExplosionGenerator()
{
	currentCEG = 0;
}

CCustomExplosionGenerator::~CCustomExplosionGenerator()
{
}


#define OP_END		 0
#define OP_STOREI	 1 // int
#define OP_STOREF	 2 // float
#define OP_STOREC	 3 // char
#define OP_ADD		 4
#define OP_RAND		 5
#define OP_DAMAGE	 6
#define OP_INDEX	 7
#define OP_LOADP	 8 // load a void* into the pointer register
#define OP_STOREP	 9 // store the pointer register into a void*
#define OP_DIR		10 // store the float3 direction
#define OP_SAWTOOTH	11 // Performs a modulo to create a sawtooth wave
#define OP_DISCRETE	12 // Floors the value to a multiple of its parameter
#define OP_SINE		13 // Uses val as the phase of a sine wave
#define OP_YANK     14 // Moves the input value into a buffer, returns zero
#define OP_MULTIPLY 15 // Multiplies with buffer value
#define OP_ADDBUFF  16 // Adds buffer value
#define OP_POW      17 // Power with code as exponent
#define OP_POWBUFF  18 // Power with buffer as exponent

void CCustomExplosionGenerator::ExecuteExplosionCode(const char* code, float damage, char* instance, int spawnIndex, const float3 &dir)
{
	float val = 0.0f;
	void* ptr = NULL;
	float buffer[16];

	for (;;) {
		switch (*(code++)) {
			case OP_END: {
				return;
			}
			case OP_STOREI: {
				Uint16 offset = *(Uint16*) code;
				code += 2;
				*(int*) (instance + offset) = (int) val;
				val = 0.0f;
				break;
			}
			case OP_STOREF: {
				Uint16 offset = *(Uint16*) code;
				code += 2;
				*(float*) (instance + offset) = val;
				val = 0.0f;
				break;
			}
			case OP_STOREC: {
				Uint16 offset = *(Uint16*) code;
				code += 2;
				*(unsigned char*) (instance + offset) = (int) val;
				val = 0.0f;
				break;
			}
			case OP_ADD: {
				val += *(float*) code;
				code += 4;
				break;
			}
			case OP_RAND: {
				val += gu->usRandFloat() * (*(float*) code);
				code += 4;
				break;
			}
			case OP_DAMAGE: {
				val += damage * (*(float*) code);
				code += 4;
				break;
			}
			case OP_INDEX: {
				val += spawnIndex * (*(float*) code);
				code += 4;
				break;
			}
			case OP_LOADP: {
				ptr = *(void**) code;
				code += sizeof(void*);
				break;
			}
			case OP_STOREP: {
				Uint16 offset = *(Uint16*) code;
				code += 2;
				*(void**) (instance + offset) = ptr;
				ptr = NULL;
				break;
			}
			case OP_DIR: {
				Uint16 offset = *(Uint16*) code;
				code += 2;
				*(float3*) (instance + offset) = dir;
				break;
			}
			case OP_SAWTOOTH: {
				// this translates to modulo except it works with floats
				val -= (*(float*) code) * floor(val / (*(float*) code));
				code += 4;
				break;
			}
			case OP_DISCRETE: {
				val = (*(float*) code) * floor(val / (*(float*) code));
				code += 4;
				break;
			}
			case OP_SINE: {
				val = (*(float*) code) * sin(val);
				code += 4;
				break;
			}
			case OP_YANK: {
				buffer[(*(int*) code)] = val;
				val = 0;
				code += 4;
				break;
			}
			case OP_MULTIPLY: {
				val *= buffer[(*(int*) code)];
				code += 4;
				break;
			}
			case OP_ADDBUFF: {
				val += buffer[(*(int*) code)];
				code += 4;
				break;
			}
			case OP_POW: {
				val = pow(val, (*(float*) code));
				code += 4;
				break;
			}
			case OP_POWBUFF: {
				val = pow(val, buffer[(*(int*) code)]);
				code += 4;
				break;
			}
			default: {
				assert(false);
				break;
			}
		}
	}
}


void CCustomExplosionGenerator::ParseExplosionCode(
	CCustomExplosionGenerator::ProjectileSpawnInfo* psi,
	int offset, boost::shared_ptr<creg::IType> type, const string& script, string& code)
{

	string::size_type end = script.find(';', 0);
	string vastr = script.substr(0, end);


	if (vastr == "dir") {//first see if we can match any keywords
		//if the user uses a keyword assume he knows that it is put on the right datatype for now
		code += OP_DIR;
		Uint16 ofs = offset;
		code.append((char*) &ofs, (char*) &ofs + 2);
	}
	else if (dynamic_cast<creg::BasicType*>(type.get())) {
		creg::BasicType *bt = (creg::BasicType*)type.get();

		if (bt->id != creg::crInt && bt->id != creg::crFloat && bt->id != creg::crUChar && bt->id != creg::crBool) {
			throw content_error("Projectile properties other than int, float and uchar, are not supported (" + script + ")");
			return;
		}

		int p = 0;
		while (p < script.length()) {
			char opcode;
			char c;
			do { c = script[p++]; } while(c == ' ');

			bool useInt=false;

			if (c == 'i')      opcode = OP_INDEX;
			else if (c == 'r') opcode = OP_RAND;
			else if (c == 'd') opcode = OP_DAMAGE;
			else if (c == 'm') opcode = OP_SAWTOOTH;
			else if (c == 'k') opcode = OP_DISCRETE;
			else if (c == 's') opcode = OP_SINE;
			else if (c == 'y') {opcode = OP_YANK; useInt=true;}
			else if (c == 'x') {opcode = OP_MULTIPLY; useInt=true;}
			else if (c == 'a') {opcode = OP_ADDBUFF; useInt=true;}
			else if (c == 'p') opcode = OP_POW;
			else if (c == 'q') {opcode = OP_POWBUFF; useInt=true;}
			else if (isdigit(c) || c == '.' || c == '-') { opcode = OP_ADD; p--; }
			else throw content_error("Explosion script error: \"" + script + "\"  : \'" + string(1, c) + "\' is unknown opcode.");

			char* endp;
			if(!useInt) {
				float v = (float)strtod(&script[p], &endp);
				p += endp - &script[p];
				code += opcode;
				code.append((char*) &v, ((char*) &v) + 4);
			}
			else {
				int v = (int)strtol(&script[p], &endp, 10);
				if (v < 0 || v > 16) throw content_error("Explosion script error: \"" + script + "\"  : Buffer index is out of bounds.");
				p += endp - &script[p];
				code += opcode;
				code.append((char*) &v, ((char*) &v) + 4);
			}
		}

		switch (bt->id) {
			case creg::crInt: code.push_back(OP_STOREI); break;
			case creg::crBool: code.push_back(OP_STOREI); break;
			case creg::crFloat: code.push_back(OP_STOREF); break;
			case creg::crUChar: code.push_back(OP_STOREC); break;
			default:
				throw content_error("Explosion script variable is of unsupported type. "
					"Contact the Spring team to fix this.");
					break;
		}
		Uint16 ofs = offset;
		code.append((char*)&ofs, (char*)&ofs + 2);
	}
	else if (dynamic_cast<creg::ObjectInstanceType*>(type.get())) {
		creg::ObjectInstanceType *oit = (creg::ObjectInstanceType *)type.get();

		string::size_type start = 0;
		for (creg::Class* c = oit->objectClass; c; c=c->base) {
			for (int a=0;a<c->members.size();a++) {
				string::size_type end = script.find(',', start+1);
				ParseExplosionCode(psi, offset + c->members [a]->offset, c->members[a]->type, script.substr(start,end-start), code);
				start = end+1;
				if (start >= script.length()) break;
			}
			if (start >= script.length()) break;
		}
	}
	else if (dynamic_cast<creg::StaticArrayBaseType*>(type.get())) {
		creg::StaticArrayBaseType *sat = (creg::StaticArrayBaseType*)type.get();

		string::size_type start = 0;
		for (unsigned int i=0; i < sat->size; i++) {
			string::size_type end = script.find(',', start+1);
			ParseExplosionCode(psi, offset + sat->elemSize * i, sat->elemType, script.substr(start,end-start), code);
			start = end+1;
			if (start >= script.length()) break;
		}
	}
	else {
		if (type->GetName() == "AtlasedTexture*") {
			string::size_type end = script.find(';', 0);
			string texname = script.substr(0, end);
			void* tex = ph->textureAtlas->GetTexturePtr(texname);
			code += OP_LOADP;
			code.append((char*)(&tex), ((char*)(&tex)) + sizeof(void*));
			code += OP_STOREP;
			Uint16 ofs = offset;
			code.append((char*)&ofs, (char*)&ofs + 2);
		}
		else if (type->GetName() == "GroundFXTexture*") {
			string::size_type end = script.find(';', 0);
			string texname = script.substr(0, end);
			void* tex = ph->groundFXAtlas->GetTexturePtr(texname);
			code += OP_LOADP;
			code.append((char*)(&tex), ((char*)(&tex)) + sizeof(void*));
			code += OP_STOREP;
			Uint16 ofs = offset;
			code.append((char*)&ofs, (char*)&ofs + 2);
		}
		else if (type->GetName() == "CColorMap*") {
			string::size_type end = script.find(';', 0);
			string colorstring = script.substr(0, end);
			void* colormap = CColorMap::LoadFromDefString(colorstring);
			code += OP_LOADP;
			code.append((char*)(&colormap), ((char*)(&colormap)) + sizeof(void*));
			code += OP_STOREP;
			Uint16 ofs = offset;
			code.append((char*)&ofs, (char*)&ofs + 2);
		}
		else if (type->GetName() == "CExplosionGenerator*") {
			string::size_type end = script.find(';', 0);
			string name = script.substr(0, end);
			void* explgen = explGenHandler->LoadGenerator(name);
			code += OP_LOADP;
			code.append((char*)(&explgen), ((char*)(&explgen)) + sizeof(void*));
			code += OP_STOREP;
			Uint16 ofs = offset;
			code.append((char*)&ofs, (char*)&ofs + 2);
		}
	}
}


void CCustomExplosionGenerator::Load(CExplosionGeneratorHandler* h, const string& tag)
{
	static std::map<string, CEGData> cachedCEGs;
	std::map<string, CEGData>::iterator it = cachedCEGs.find(tag);

	if (it == cachedCEGs.end()) {
		CEGData cegData;

		const LuaTable& root = h->GetTable();
		const LuaTable expTable = root.SubTable(tag);
		if (!expTable.IsValid()) {
			throw content_error("Explosion info for " + tag + " not found.");
		}

		vector<string> spawns;
		expTable.GetKeys(spawns);

		for (vector<string>::iterator si = spawns.begin(); si != spawns.end(); ++si) {
			ProjectileSpawnInfo psi;

			const string& spawnName = *si;
			const LuaTable spawnTable = expTable.SubTable(spawnName);

			if (!spawnTable.IsValid() || spawnName == "groundflash") {
				continue;
			}

			const string className = spawnTable.GetString("class", spawnName);
			unsigned int flags = 0;

			if (spawnTable.GetBool("ground",     false)) { flags |= SPW_GROUND;     }
			if (spawnTable.GetBool("water",      false)) { flags |= SPW_WATER;      }
			if (spawnTable.GetBool("air",        false)) { flags |= SPW_AIR;        }
			if (spawnTable.GetBool("underwater", false)) { flags |= SPW_UNDERWATER; }
			if (spawnTable.GetBool("unit",       false)) { flags |= SPW_UNIT;       }
			if (spawnTable.GetBool("nounit",     false)) { flags |= SPW_NO_UNIT;    }

			psi.projectileClass = h->projectileClasses.GetClass(className);
			psi.flags = flags;
			psi.count = spawnTable.GetInt("count", 1);

			string code;
			map<string, string> props;
			map<string, string>::const_iterator propIt;
			spawnTable.SubTable("properties").GetMap(props);

			for (propIt = props.begin(); propIt != props.end(); ++propIt) {
				creg::Class::Member* m = psi.projectileClass->FindMember(propIt->first.c_str());
				if (m && (m->flags & creg::CM_Config)) {
					ParseExplosionCode(&psi, m->offset, m->type, propIt->second, code);
				}
			}

			code += (char)OP_END;
			psi.code.resize(code.size());
			copy(code.begin(), code.end(), psi.code.begin());

			cegData.projectileSpawn.push_back(psi);
		}

		const LuaTable gndTable = expTable.SubTable("groundflash");
		const int ttl = gndTable.GetInt("ttl", 0);
		if (ttl > 0) {
			cegData.groundFlash.circleAlpha  = gndTable.GetFloat("circleAlpha",  0.0f);
			cegData.groundFlash.flashSize    = gndTable.GetFloat("flashSize",    0.0f);
			cegData.groundFlash.flashAlpha   = gndTable.GetFloat("flashAlpha",   0.0f);
			cegData.groundFlash.circleGrowth = gndTable.GetFloat("circleGrowth", 0.0f);
			cegData.groundFlash.color        = gndTable.GetFloat3("color", float3(1.0f, 1.0f, 0.8f));

			unsigned int flags = SPW_GROUND;
			if (gndTable.GetBool("ground",     false)) { flags |= SPW_GROUND;     }
			if (gndTable.GetBool("water",      false)) { flags |= SPW_WATER;      }
			if (gndTable.GetBool("air",        false)) { flags |= SPW_AIR;        }
			if (gndTable.GetBool("underwater", false)) { flags |= SPW_UNDERWATER; }
			if (gndTable.GetBool("unit",       false)) { flags |= SPW_UNIT;       }
			if (gndTable.GetBool("nounit",     false)) { flags |= SPW_NO_UNIT;    }

			cegData.groundFlash.flags = flags;
			cegData.groundFlash.ttl = ttl;
		}

		cegData.useDefaultExplosions = expTable.GetBool("useDefaultExplosions", false);

		cachedCEGs[tag] = cegData;
		currentCEG = &cachedCEGs[tag];
	} else {
		currentCEG = &(it->second);
	}
}


void CCustomExplosionGenerator::Explosion(const float3& pos, float damage, float radius, CUnit* owner, float gfxMod, CUnit* hit, const float3& dir)
{
	if (currentCEG == 0) {
		// Explosion() called before Load()'ing a CEG
		return;
	}

	float h2 = ground->GetHeight2(pos.x, pos.z);
	unsigned int flags = 0;

	if (pos.y - max(0.0f, h2) > 20) flags = SPW_AIR;
	else if (h2 < -3)               flags = SPW_WATER;
	else if (pos.y < -15)           flags = SPW_UNDERWATER;
	else                            flags = SPW_GROUND;

	if (hit) flags |= SPW_UNIT;
	else     flags |= SPW_NO_UNIT;

	for (int a = 0; a < currentCEG->projectileSpawn.size(); a++) {
		ProjectileSpawnInfo* psi = &currentCEG->projectileSpawn[a];

		if (!(psi->flags & flags)) {
			continue;
		}

		for (int c = 0; c < psi->count; c++) {
			CExpGenSpawnable* projectile = (CExpGenSpawnable*) psi->projectileClass->CreateInstance();

			ExecuteExplosionCode(&psi->code[0], damage, (char*) projectile, c, dir);
			projectile->Init(pos, owner);
		}
	}

	const GroundFlashInfo& groundFlash = currentCEG->groundFlash;

	if ((flags & SPW_GROUND) && groundFlash.ttl > 0) {
		SAFE_NEW CStandardGroundFlash(pos, groundFlash.circleAlpha, groundFlash.flashAlpha,
			groundFlash.flashSize, groundFlash.circleGrowth, groundFlash.ttl, groundFlash.color);
	}

	if (currentCEG->useDefaultExplosions) {
		CStdExplosionGenerator::Explosion(pos, damage, radius, owner, gfxMod, hit, dir);
	}
}


void CCustomExplosionGenerator::OutputProjectileClassInfo()
{
	const vector<creg::Class*>& classes = creg::System::GetClasses();
	std::ofstream fs("projectiles.txt");
	CExplosionGeneratorHandler egh;

	if (fs.bad() || !fs.is_open()) {
		return;
	}

	for (vector<creg::Class*>::const_iterator ci = classes.begin(); ci != classes.end(); ++ci) {
		if (!(*ci)->IsSubclassOf (CExpGenSpawnable::StaticClass()) || (*ci) == CExpGenSpawnable::StaticClass()) {
			continue;
		}

		creg::Class *klass = *ci;
		fs << "Class: " << klass->name << ".  Scriptname: " << egh.projectileClasses.FindAlias(klass->name) << std::endl;
		for (; klass; klass = klass->base) {
			for (unsigned int a = 0; a < klass->members.size(); a++) {
				if (klass->members[a]->flags & creg::CM_Config) {
					fs << "\t" << klass->members[a]->name << ": " << klass->members[a]->type->GetName() << "\n";
				}
			}
		}
		fs << "\n\n";
	}
}

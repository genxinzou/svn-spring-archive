#include "StdAfx.h"
#include "LaserProjectile.h"
#include "Game/Camera.h"
#include "Rendering/GL/VertexArray.h"
#include "Sim/Weapons/WeaponDefHandler.h"
#include "Game/UI/InfoConsole.h"
#include "mmgr.h"

CLaserProjectile::CLaserProjectile(const float3& pos,const float3& speed,CUnit* owner,const DamageArray& damages,float length,const float3& color,float intensity, WeaponDef *weaponDef, int ttl)
: CWeaponProjectile(pos,speed,owner,0,ZeroVector,weaponDef,damages,0),
	ttl(ttl),
	color(color),
	length(length),
	curLength(0),
	intensity(intensity),
	intensityFalloff(intensity*0.1)
{
	dir=speed;
	dir.Normalize();
	speedf=speed.Length();

	SetRadius(0.5);
	drawRadius=length;
#ifdef TRACE_SYNC
	tracefile << "New laser: ";
	tracefile << pos.x << " " << pos.y << " " << pos.z << " " << speed.x << " " << speed.y << " " << speed.z << "\n";
#endif
}

CLaserProjectile::~CLaserProjectile(void)
{
}

void CLaserProjectile::Update(void)
{
	pos+=speed;
	if(checkCol){	//normal;
		curLength+=speedf;
		if(curLength>length)
			curLength=length;
	} else {	//fading out after hit
		curLength-=speedf;
		if(curLength<=0){
			deleteMe=true;
			curLength=0;
		}
	}
	ttl--;

	if(ttl<5){
		intensity-=intensityFalloff;
		if(intensity<=0){
			deleteMe=true;
			intensity=0;
		}
	}
}

void CLaserProjectile::Collision(CUnit* unit)
{
	float3 oldPos=pos;
	CWeaponProjectile::Collision(unit);

	deleteMe=false;	//we will fade out over some time
	checkCol=false;
	speed=ZeroVector;
	pos=oldPos;
}

void CLaserProjectile::Collision(CFeature* feature)
{
	float3 oldPos=pos;
	CWeaponProjectile::Collision(feature);

	deleteMe=false;	//we will fade out over some time
	checkCol=false;
	speed=ZeroVector;
	pos=oldPos;
}

void CLaserProjectile::Collision()
{
	float3 oldPos=pos;
	CWeaponProjectile::Collision();

	deleteMe=false;	//we will fade out over some time
	checkCol=false;
	speed=ZeroVector;
	pos=oldPos;
}


void CLaserProjectile::Draw(void)
{
	if(s3domodel)	//dont draw if a 3d model has been defined for us
		return;

	inArray=true;
	float3 dif(pos-camera->pos);
	float camDist=dif.Length();
	dif/=camDist;
	float3 dir1(dif.cross(dir));
	dir1.Normalize();
	float3 dir2(dif.cross(dir1));

	unsigned char col[4];
	col[0]=(unsigned char) (color.x*intensity*255);
	col[1]=(unsigned char) (color.y*intensity*255);
	col[2]=(unsigned char) (color.z*intensity*255);
	col[3]=1;//intensity*255;

	float size=weaponDef->thickness;

	if(camDist<1000){
		float3 pos1=pos+speed*gu->timeOffset;
		float3 pos2=pos1-dir*curLength;

		va->AddVertexTC(pos1-dir1*size,					  15.0/16,0,    col);
		va->AddVertexTC(pos1+dir1*size,					  15.0/16,1.0/8,col);
		va->AddVertexTC(pos1+dir1*size-dir2*size, 14.0/16,1.0/8,col);
		va->AddVertexTC(pos1-dir1*size-dir2*size, 14.0/16,0		,col);

		va->AddVertexTC(pos1-dir1*size,11.0/16,0,    col);
		va->AddVertexTC(pos1+dir1*size,11.0/16,1.0/8,col);
		va->AddVertexTC(pos2+dir1*size,11.0/16,1.0/8,col);
		va->AddVertexTC(pos2-dir1*size,11.0/16,0    ,col);

		va->AddVertexTC(pos2-dir1*size,					  15.0/16,0,    col);
		va->AddVertexTC(pos2+dir1*size,					  15.0/16,1.0/8,col);
		va->AddVertexTC(pos2+dir1*size+dir2*size, 14.0/16,1.0/8,col);
		va->AddVertexTC(pos2-dir1*size+dir2*size, 14.0/16,0		,col);
	} else {
		float3 pos1=pos+speed*gu->timeOffset+dir*(size*0.5);
		float3 pos2=pos1-dir*(curLength+size);

		va->AddVertexTC(pos1-dir1*size,11.0/16,0,    col);
		va->AddVertexTC(pos1+dir1*size,11.0/16,1.0/8,col);
		va->AddVertexTC(pos2+dir1*size,11.0/16,1.0/8,col);
		va->AddVertexTC(pos2-dir1*size,11.0/16,0    ,col);
	}
}

int CLaserProjectile::ShieldRepulse(CPlasmaRepulser* shield,float3 shieldPos, float shieldForce, float shieldMaxSpeed)
{
	float3 sdir=pos-shieldPos;
	sdir.Normalize();
	if(sdir.dot(speed)<0){
		speed-=sdir*sdir.dot(speed)*2;
		dir=speed;
		dir.Normalize();
		return 1;
	}
	return 0;
}

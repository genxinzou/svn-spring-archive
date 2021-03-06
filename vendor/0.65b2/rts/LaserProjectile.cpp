#include "StdAfx.h"
#include ".\laserprojectile.h"
#include "projectilehandler.h"
#include "unit.h"
#include "mygl.h"
#include "camera.h"
#include "vertexarray.h"
#include "synctracer.h"
#include "weapondefhandler.h"
//#include "mmgr.h"

CLaserProjectile::CLaserProjectile(const float3& pos,const float3& speed,CUnit* owner,const DamageArray& damages,float length,const float3& color,float intensity, WeaponDef *weaponDef, int ttl)
: CWeaponProjectile(pos,speed,owner,0,ZeroVector,weaponDef,0),
	damages(damages),
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
	curLength+=speedf;
	if(curLength>length)
		curLength=length;
	ttl--;

	if(ttl<7){
		intensity-=intensityFalloff;
		if(intensity<=0){
			deleteMe=true;
			intensity=0;
		}
	}
}

void CLaserProjectile::Collision(CUnit* unit)
{
//	unit->DoDamage(damages,owner);

	CWeaponProjectile::Collision(unit);
}

void CLaserProjectile::Draw(void)
{
	if(isUnitPart)	//dont draw if a 3d model has been defined for us
		return;

	inArray=true;
	float3 dif(pos-camera->pos);
	float camDist=dif.Length();
	dif/=camDist;
	float3 dir1(dif.cross(dir));
	dir1.Normalize();
	float3 dir2(dif.cross(dir1));

	unsigned char col[4];
	col[0]=color.x*intensity*255;
	col[1]=color.y*intensity*255;
	col[2]=color.z*intensity*255;
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

#include "StdAfx.h"
#include "GenericParticleProjectile.h"
#include "GlobalStuff.h"
#include "Rendering/GL/VertexArray.h"
#include "Game/Camera.h"
#include "ProjectileHandler.h"
#include "Rendering/Textures/ColorMap.h"

CR_BIND_DERIVED(CGenericParticleProjectile, CProjectile, (float3(0,0,0),float3(0,0,0),NULL));

CR_REG_METADATA(CGenericParticleProjectile,(
	CR_MEMBER(gravity),
	CR_MEMBER(texture),
	CR_MEMBER(colorMap),
	CR_MEMBER(directional),
	CR_MEMBER(life),
	CR_MEMBER(decayrate),
	CR_MEMBER(size),
	CR_MEMBER(airdrag),
	CR_MEMBER(sizeGrowth),
	CR_MEMBER(sizeMod)
	));

CGenericParticleProjectile::CGenericParticleProjectile(const float3& pos,const float3& speed,CUnit* owner) :
	CProjectile(pos, speed, owner)
{
	deleteMe=false;
	checkCol=false;
	useAirLos=true;
}

CGenericParticleProjectile::~CGenericParticleProjectile(void)
{
}

void CGenericParticleProjectile::Update()
{
	pos += speed;
	life +=decayrate;
	speed += gravity;
	speed *= airdrag;
	size = size*sizeMod + sizeGrowth;

	if(life>1.0f)
	{
		deleteMe = true;
	}
}

void CGenericParticleProjectile::Draw()
{
	inArray=true;

	if(directional)
	{
		float3 dif(pos-camera->pos);
		float camDist=dif.Length();
		dif/=camDist;
		float3 dir1(dif.cross(speed));
		dir1.Normalize();
		float3 dir2(dif.cross(dir1));

		unsigned char color[4];

		colorMap->GetColor(color, life);
		float3 interPos=pos+speed*gu->timeOffset;

		va->AddVertexTC(interPos-dir1*size-dir2*size,texture->xstart,texture->ystart,color);
		va->AddVertexTC(interPos-dir1*size+dir2*size,texture->xend ,texture->ystart,color);
		va->AddVertexTC(interPos+dir1*size+dir2*size,texture->xend ,texture->yend ,color);
		va->AddVertexTC(interPos+dir1*size-dir2*size,texture->xstart,texture->yend ,color);
	}
	else
	{
		unsigned char color[4];

		colorMap->GetColor(color, life);
		float3 interPos=pos+speed*gu->timeOffset;

		va->AddVertexTC(interPos-camera->right*size-camera->up*size,texture->xstart,texture->ystart,color);
		va->AddVertexTC(interPos+camera->right*size-camera->up*size,texture->xend ,texture->ystart,color);
		va->AddVertexTC(interPos+camera->right*size+camera->up*size,texture->xend ,texture->yend ,color);
		va->AddVertexTC(interPos-camera->right*size+camera->up*size,texture->xstart,texture->yend ,color);
	}
}

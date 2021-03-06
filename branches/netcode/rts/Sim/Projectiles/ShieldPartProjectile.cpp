#include "StdAfx.h"
#include "ShieldPartProjectile.h"
#include "Rendering/GL/myGL.h"
#include "Rendering/GL/VertexArray.h"
#include "Sim/Projectiles/ProjectileHandler.h"
#include "Rendering/Textures/TextureAtlas.h"
#include "mmgr.h"

using namespace std;

CR_BIND_DERIVED(CShieldPartProjectile, CProjectile, (float3(0,0,0),0,0,0,float3(0,0,0),0,NULL,NULL));

CR_REG_METADATA(CShieldPartProjectile,(
	CR_MEMBER(centerPos),
	CR_MEMBER(vectors),
	CR_MEMBER(texCoords),
	CR_MEMBER(sphereSize),
	CR_MEMBER(baseAlpha),
	CR_MEMBER(color)
	));

CShieldPartProjectile::CShieldPartProjectile(const float3& centerPos,int xpart,int ypart,float sphereSize,float3 color,float alpha,AtlasedTexture *texture,CUnit* owner)
: CProjectile(centerPos,ZeroVector,owner, false),
	centerPos(centerPos),
	sphereSize(sphereSize),
	baseAlpha(alpha),
	color(color)
{
	checkCol=false;

	AtlasedTexture* tex= texture;

	for(int y=0;y<5;++y){
		float yp=(y+ypart)/16.0f*PI-PI/2;
		for(int x=0;x<5;++x){
			float xp=(x+xpart)/32.0f*2*PI;
			vectors[y*5+x]=float3(sin(xp)*cos(yp),sin(yp),cos(xp)*cos(yp));
			texCoords[y*5+x].x=(vectors[y*5+x].x*(2-fabs(vectors[y*5+x].y)))*((tex->xend-tex->xstart)*0.25f)+((tex->xstart+tex->xend)*0.5f);
			texCoords[y*5+x].y=(vectors[y*5+x].z*(2-fabs(vectors[y*5+x].y)))*((tex->yend-tex->ystart)*0.25f)+((tex->ystart+tex->yend)*0.5f);
		}
	}
	pos=centerPos+vectors[12]*sphereSize;

	alwaysVisible=false;
	useAirLos=true;
	drawRadius=sphereSize*0.4f;

	ph->numPerlinProjectiles++;
}

CShieldPartProjectile::~CShieldPartProjectile(void)
{
	ph->numPerlinProjectiles--;
}

void CShieldPartProjectile::Update(void)
{
	pos=centerPos+vectors[12]*sphereSize;
}

void CShieldPartProjectile::Draw(void)
{
	if (baseAlpha <= 0.0f) {
		return;
	}

	inArray=true;
	unsigned char col[4];

	float interSize=sphereSize;
	for(int y=0;y<4;++y){
		for(int x=0;x<4;++x){
			float alpha=baseAlpha*255;

			col[0]=(unsigned char) (color.x*alpha);
			col[1]=(unsigned char) (color.y*alpha);
			col[2]=(unsigned char) (color.z*alpha);
			col[3]=(unsigned char) (alpha);
			va->AddVertexTC(centerPos+vectors[y*5+x]      *interSize,texCoords[y*5+x].x      ,texCoords[y*5+x].y      ,col);
			va->AddVertexTC(centerPos+vectors[y*5+x+1]    *interSize,texCoords[y*5+x+1].x    ,texCoords[y*5+x+1].y    ,col);
			va->AddVertexTC(centerPos+vectors[(y+1)*5+x+1]*interSize,texCoords[(y+1)*5+x+1].x,texCoords[(y+1)*5+x+1].y,col);
			va->AddVertexTC(centerPos+vectors[(y+1)*5+x]  *interSize,texCoords[(y+1)*5+x].x  ,texCoords[(y+1)*5+x].y  ,col);
		}
	}
}

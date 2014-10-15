// SmokeProjectile.h: interface for the CSmokeProjectile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SMOKEPROJECTILE_H__8E8E19C3_6BF3_11D5_AD55_0080ADA84DE3__INCLUDED_)
#define AFX_SMOKEPROJECTILE_H__8E8E19C3_6BF3_11D5_AD55_0080ADA84DE3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Projectile.h"

class CSmokeProjectile2 : public CProjectile  
{
public:
	void Update();
	void Draw();
	CSmokeProjectile2(float3 pos,float3 wantedPos,float3 speed,float ttl,float startSize,float sizeExpansion, CUnit* owner,float color=0.7f);
	virtual ~CSmokeProjectile2();

	float color;
	float age;
	float ageSpeed;
	float size;
	float startSize;
	float sizeExpansion;
	int textureNum;
	float3 wantedPos;
	float glowFalloff;
};

#endif // !defined(AFX_SMOKEPROJECTILE_H__8E8E19C3_6BF3_11D5_AD55_0080ADA84DE3__INCLUDED_)

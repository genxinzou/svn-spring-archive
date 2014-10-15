// HeatCloudProjectile.h: interface for the CHeatCloudProjectile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HEATCLOUDPROJECTILE_H__23CBBD23_8FBE_11D4_AD55_0080ADA84DE3__INCLUDED_)
#define AFX_HEATCLOUDPROJECTILE_H__23CBBD23_8FBE_11D4_AD55_0080ADA84DE3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Projectile.h"

class CHeatCloudProjectile : public CProjectile  
{
public:
public:
	virtual void Draw();
	virtual void Update();
	CHeatCloudProjectile(const float3 pos,const float3 speed,const float temperature,const float size, CUnit* owner);  //projectile start at size 0 and ends at size size
	CHeatCloudProjectile(const float3 pos,const float3 speed,const float temperature,const float size, float sizegrowth, CUnit* owner);  //size is initial size and sizegrowth is size increase per tick
	virtual ~CHeatCloudProjectile();

	float heat;
	float maxheat;
	float heatFalloff;
	float size;
	float sizeGrowth;
	float sizemod;
	float sizemodmod;
};

#endif // !defined(AFX_HEATCLOUDPROJECTILE_H__23CBBD23_8FBE_11D4_AD55_0080ADA84DE3__INCLUDED_)

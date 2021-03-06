#pragma once
#include "projectile.h"
#include <vector>

class CFlareProjectile :
	public CProjectile
{
public:
	CFlareProjectile(const float3& pos,const float3& speed,CUnit* owner,int activateFrame);
	~CFlareProjectile();
	void Update(void);
	void Draw(void);

	int activateFrame;
	int deathFrame;

	int numSub;
	int lastSub;
	std::vector<float3> subPos;
	std::vector<float3> subSpeed;
	float alphaFalloff;
};

#ifndef SPHEREPARTPROJECTILE_H
#define SPHEREPARTPROJECTILE_H

#include "Projectile.h"

class CSpherePartProjectile :
	public CProjectile
{
public:
	CSpherePartProjectile(const float3& centerPos,int xpart,int ypart,float expansionSpeed,float alpha,int ttl,CUnit* owner,float3 &color);
	~CSpherePartProjectile(void);

	float3 centerPos;
	float3 vectors[25];
	float3 color;

	float sphereSize;
	float expansionSpeed;

	int xbase;
	int ybase;

	float baseAlpha;
	int age;
	int ttl;
	float texx;
	float texy;
	void Draw(void);
	void Update(void);
	static void CreateSphere(float3 pos, float alpha, int ttl, float expansionSpeed , CUnit* owner, float3 color=float3(0.8,0.8,0.6));
};

//class to make a spherepartprojectile via the explosiongenerator
class CSpherePartSpawner : CProjectile
{
	CR_DECLARE(CSpherePartSpawner);
public:
	float alpha;
	int ttl;
	float expansionSpeed;
	float3 color;

	CSpherePartSpawner();
	~CSpherePartSpawner();

	virtual void Init(const float3& pos, CUnit *owner);
};

#endif /* SPHEREPARTPROJECTILE_H */

#pragma once

class CUnit;
#include <set>
#include <vector>
#include <list>
#include <stack>
#include <string>
#include <map>

class CVertexArray;
struct S3DOModel;
struct UnitDef;

class CUnitDrawer
{
public:
	CUnitDrawer(void);
	~CUnitDrawer(void);

	void Update(void);
	void Draw(bool drawReflection);

	void DrawCloakedUnits(void);		//cloaked units must be drawn after all others;
	void DrawShadowPass(void);
	void SetupForUnitDrawing(void);
	void CleanUpUnitDrawing(void);

	float unitDrawDist;

	unsigned int whiteTex;
	unsigned int radarBlippTex;

	std::vector<CUnit*> drawCloaked;
	CVertexArray* va;
	inline void DrawFar(CUnit* unit);
	unsigned int unitVP;
	unsigned int unitShadowVP;
	unsigned int unitFP;
	unsigned int boxtex;
	unsigned int specularTex;

	int updateFace;

	float3 unitAmbientColor;
	float3 unitSunColor;
	float unitShadowDensity;

	struct TempDrawUnit{
		UnitDef* unitdef;
		int team;
		float3 pos;
		float rot;
		bool drawBorder;
	};
	std::multimap<int,TempDrawUnit> tempDrawUnits;
	std::multimap<int,TempDrawUnit> tempTransperentDrawUnits;

	struct GhostBuilding {
		float3 pos;
		S3DOModel* model;
	};
	std::list<GhostBuilding> ghostBuildings;	//these are buildings that where in LOS_PREVLOS when they died and havent been in los since then

	bool showHealthBars;

	float3 camNorm;		//used by drawfar

#ifdef DIRECT_CONTROL_ALLOWED
	CUnit* playerControlledUnit;		
#endif
	void CreateSpecularFace(unsigned int gltype, int size, float3 baseDir, float3 xdif, float3 ydif, float3 sundir, float exponent,float3 suncolor);
	void UpdateReflectTex(void);
	void CreateReflectionFace(unsigned int gltype, float3 camdir);
};

extern CUnitDrawer* unitDrawer;

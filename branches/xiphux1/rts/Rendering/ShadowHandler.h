#ifndef SHADOWHANDLER_H
#define SHADOWHANDLER_H

#include "Matrix44f.h"
#include "GL/myGL.h"
#include <vector>

class IFramebuffer;

class CShadowHandler
{
public:
	CShadowHandler(void);
	~CShadowHandler(void);
	void CreateShadows(void);

	int shadowMapSize;
	unsigned int shadowTexture;

	bool drawShadows;
	bool inShadowPass;
	bool showShadowMap;
	bool firstDraw;
	bool useFPShadows;

	float3 centerPos;
	float3 cross1;
	float3 cross2;

	float x1,x2,y1,y2;

	CMatrix44f shadowMatrix;
	void DrawShadowTex(void);
	void CalcMinMaxView(void);

protected:
	void GetFrustumSide(float3& side,bool upside);
	struct fline {
		float base;
		float dir;
		int left;
		float minz;
		float maxz;
	};
	std::vector<fline> left;
	IFramebuffer *fb;
};

extern CShadowHandler* shadowHandler;

#endif /* SHADOWHANDLER_H */

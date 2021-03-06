#include "StdAfx.h"
#include "mmgr.h"

#include "BaseTreeDrawer.h"
#include "BasicTreeDrawer.h"
#include "AdvTreeDrawer.h"
#include "Rendering/GL/myGL.h"
#include "Platform/ConfigHandler.h"
#include "Game/Camera.h"
#include "Game/GlobalConstants.h"

CBaseTreeDrawer* treeDrawer=0;

CBaseTreeDrawer::CBaseTreeDrawer(void)
{
	drawTrees=true;
	baseTreeDistance=configHandler.GetInt("TreeRadius",(unsigned int) (5.5f*256))/256.0f;
}

CBaseTreeDrawer::~CBaseTreeDrawer(void)
{}

CBaseTreeDrawer* CBaseTreeDrawer::GetTreeDrawer(void)
{
	CBaseTreeDrawer* td;
	if(GLEW_ARB_vertex_program && configHandler.GetInt("3DTrees",1)){
		GLint maxTexel;
		glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB,&maxTexel);
		if(maxTexel>=4){
			td=SAFE_NEW CAdvTreeDrawer;
			return td;
		}
	}
	td=SAFE_NEW CBasicTreeDrawer;
	return td;
}

int CBaseTreeDrawer::AddFallingTree(float3 pos, float3 dir, int type)
{
	return 0;
}

void CBaseTreeDrawer::DrawShadowPass(void)
{
}

void CBaseTreeDrawer::Draw (bool drawReflection)
{
	float zoom=45/camera->GetFov();
	float treeDistance=baseTreeDistance*sqrt((zoom));
	if(treeDistance>MAX_VIEW_RANGE/(SQUARE_SIZE*TREE_SQUARE_SIZE))
		treeDistance=MAX_VIEW_RANGE/(SQUARE_SIZE*TREE_SQUARE_SIZE);

	Draw (treeDistance, drawReflection);
}

void CBaseTreeDrawer::UpdateDraw() {
	GML_STDMUTEX_LOCK(tree); // UpdateDraw
	for(std::vector<GLuint>::iterator i=delDispLists.begin(); i!=delDispLists.end(); ++i)
		glDeleteLists(*i, 1);
	delDispLists.clear();
}


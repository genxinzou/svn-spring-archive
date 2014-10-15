#include "stdafx.h"
#include "basesky.h"
#include "basicsky.h"
#include "advsky.h"
#include "mygl.h"
#include "reghandler.h"
#include ".\basesky.h"
#include "skybox.h"
#include "readmap.h"

//#include "mmgr.h"

CBaseSky* sky=0;

CBaseSky::CBaseSky(void)
{
}

CBaseSky::~CBaseSky(void)
{
}

CBaseSky* CBaseSky::GetSky()
{

	if(!readmap->skyBox.empty())
		return new CSkyBox("maps\\" + readmap->skyBox);
	else if(GLEW_ARB_fragment_program && regHandler.GetInt("AdvSky",1) && ProgramStringIsNative(GL_FRAGMENT_PROGRAM_ARB,"clouds.fp"))
		return new CAdvSky();
	else
		return new CBasicSky();
}

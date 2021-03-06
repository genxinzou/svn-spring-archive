#include "StdAfx.h"
// DrawWater.cpp: implementation of the CBasicWater class.
//
//////////////////////////////////////////////////////////////////////

#include "BasicWater.h"
#include "Rendering/GL/myGL.h"
#include <GL/glu.h>

#include "Rendering/Textures/Bitmap.h"
#include "Map/ReadMap.h"
#include "mmgr.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBasicWater::CBasicWater()
{
	glGenTextures(1, &texture);
	CBitmap pic;
	if (!pic.Load(readmap->waterTexture))
		throw content_error("Could not read water texture from file " + readmap->waterTexture);

	// create mipmapped texture
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGBA8 ,pic.xsize, pic.ysize, GL_RGBA, GL_UNSIGNED_BYTE, pic.mem);

	displist=0;

	textureWidth = pic.xsize;
	textureHeight = pic.ysize;
}

CBasicWater::~CBasicWater()
{
	glDeleteTextures (1, &texture);
}

void CBasicWater::Draw()
{
	if(readmap->minheight > 10)
		return;

	if(displist == 0) {
		displist=glGenLists(1);
		glNewList(displist, GL_COMPILE);
		glDisable(GL_ALPHA_TEST);
		glDepthMask(0);
		glColor4f(0.7f,0.7f,0.7f,0.5f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);
		glBegin(GL_QUADS);

		float mapSizeX=gs->mapx*SQUARE_SIZE;
		float mapSizeY=gs->mapy*SQUARE_SIZE;

		// Calculate number of times texture should repeat over the map,
		// taking aspect ratio into account.
		float repeatX = 65536.0f / gs->mapx;
		float repeatY = 65536.0f / gs->mapy * textureWidth / textureHeight;

		// Use better repeat setting of 1 repeat per 4096 mapx/mapy for the new
		// ocean.jpg while retaining backward compatibility with old maps relying
		// on 1 repeat per 1024 mapx/mapy. (changed 16/05/2007)
		if (readmap->waterTexture == "bitmaps/ocean.jpg") {
			repeatX /= 4;
			repeatY /= 4;
		}

		readmap->mapDefParser.GetTDef(repeatX, repeatX, "MAP\\WATER\\WaterRepeatX");
		readmap->mapDefParser.GetTDef(repeatY, repeatY, "MAP\\WATER\\WaterRepeatY");
		repeatX /= 16;
		repeatY /= 16;

		for(int y=0;y<16;y++){
			for(int x=0;x<16;x++){
				glTexCoord2f(x*repeatX,y*repeatY);
				glVertex3f(x*mapSizeX/16,0,y*mapSizeY/16);
				glTexCoord2f(x*repeatX,(y+1)*repeatY);
				glVertex3f(x*mapSizeX/16,0,(y+1)*mapSizeY/16);
				glTexCoord2f((x+1)*repeatX,(y+1)*repeatY);
				glVertex3f((x+1)*mapSizeX/16,0,(y+1)*mapSizeY/16);
				glTexCoord2f((x+1)*repeatX,y*repeatY);
				glVertex3f((x+1)*mapSizeX/16,0,y*mapSizeY/16);
			}
		}

		glEnd();
		glDisable(GL_TEXTURE_2D);
		glDepthMask(1);
		glEndList();
	}
	glCallList(displist);
}

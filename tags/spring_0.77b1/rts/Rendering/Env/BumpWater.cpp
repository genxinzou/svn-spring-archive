/**
 * @file BumpWater.cpp
 * @brief extended bumpmapping water shader
 * @author jK
 *
 * Copyright (C) 2008.  Licensed under the terms of the
 * GNU GPL, v2 or later.
 */

#include "StdAfx.h"
#include "Rendering/GL/myGL.h"
#include <boost/format.hpp>
#include "mmgr.h"

#include "bitops.h"
#include "BumpWater.h"
#include "BaseSky.h"
#include "Game/Game.h"
#include "Game/Camera.h"
#include "Rendering/GL/VertexArray.h"
#include "Rendering/Textures/Bitmap.h"
#include "Rendering/UnitModels/UnitDrawer.h"
#include "Map/MapInfo.h"
#include "Map/ReadMap.h"
#include "Map/BaseGroundDrawer.h"
#include "Sim/Projectiles/ProjectileHandler.h"
#include "Sim/Features/FeatureHandler.h"
#include "Sim/Misc/Wind.h"
#include "FileSystem/FileHandler.h"
#include "System/FastMath.h"
#include "System/EventHandler.h"
#include "System/Platform/ConfigHandler.h"
#include "TimeProfiler.h"
#include "LogOutput.h"
#include "System/Exceptions.h"

using std::string;
using std::vector;
using std::min;
using std::max;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// HELPER FUNCTIONS
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void PrintShaderLog(GLuint obj, const string& type)
{
	/** WAS COMPILATION SUCCESSFUL ? **/
	GLint compiled;
	if(glIsShader(obj))
		glGetShaderiv(obj,GL_COMPILE_STATUS,&compiled);
	else
		glGetProgramiv(obj,GL_LINK_STATUS,&compiled);

	if (compiled) return;

	/** GET INFOLOG **/
	int infologLength = 0;
	int maxLength;

	if(glIsShader(obj))
		glGetShaderiv(obj,GL_INFO_LOG_LENGTH,&maxLength);
	else
		glGetProgramiv(obj,GL_INFO_LOG_LENGTH,&maxLength);

	char* infoLog = SAFE_NEW char[maxLength];

	if (glIsShader(obj))
		glGetShaderInfoLog(obj, maxLength, &infologLength, infoLog);
	else
		glGetProgramInfoLog(obj, maxLength, &infologLength, infoLog);

	if (infologLength > 0) {
		string str(infoLog, infologLength);
		delete[] infoLog;
		//! string size is limited with content_error()
		logOutput.Print("BumpWater shader error (" + type  + "): " + str);
		throw content_error(string("BumpWater shader error!"));
	}
	delete[] infoLog;
}


static void PrintFboError(string fbo, GLenum error)
{
	switch(error) {
		case GL_FRAMEBUFFER_COMPLETE_EXT:
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
			logOutput.Print("BumpWater-FBO: ("+fbo+")has no images/buffers attached!");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
			logOutput.Print("BumpWater-FBO: ("+fbo+")missing a required image/buffer attachment!");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
			logOutput.Print("BumpWater-FBO: ("+fbo+")has mismatched image/buffer dimensions!");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
			logOutput.Print("BumpWater-FBO: ("+fbo+")colorbuffer attachments have different types!");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
			logOutput.Print("BumpWater-FBO: ("+fbo+")incomplete draw buffers!");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
			logOutput.Print("BumpWater-FBO: ("+fbo+")trying to read from a non-attached color buffer!");
			break;
		case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
			logOutput.Print("BumpWater-FBO: ("+fbo+")format is not supported by current graphics card/driver!");
			break;
		default:
			logOutput.Print("BumpWater-FBO: ("+fbo+")*UNKNOWN ERROR*");
			break;
	}
}


static string LoadShaderSource(const string& file)
{
	CFileHandler fh(file);
	if (!fh.FileExists())
		throw content_error("Can't load shader " + file);
	string text;
	text.resize(fh.FileSize());
	fh.Read(&text[0], text.length());
	return text;
}


static void GLSLDefineConst4f(string& str, const string& name, const float x, const float y, const float z, const float w)
{
	str += boost::str(boost::format(string("#define ")+name+" vec4(%1$.12f,%2$.12f,%3$.12f,%4$.12f)\n") % (x) % (y) % (z) % (w));
}


static void GLSLDefineConstf4(string& str, const string& name, const float3& v, const float& alpha)
{
	str += boost::str(boost::format(string("#define ")+name+" vec4(%1$.12f,%2$.12f,%3$.12f,%4$.12f)\n") % (v.x) % (v.y) % (v.z) % (alpha));
}


static void GLSLDefineConstf3(string& str, const string& name, const float3& v)
{
	str += boost::str(boost::format(string("#define ")+name+" vec3(%1$.12f,%2$.12f,%3$.12f)\n") % (v.x) % (v.y) % (v.z));
}


static void GLSLDefineConstf2(string& str, const string& name, const float& x, const float& y)
{
	str += boost::str(boost::format(string("#define ")+name+" vec2(%1$.12f,%2$.12f)\n") % x % y);
}


static void GLSLDefineConstf1(string& str, const string& name, const float& x)
{
	str += boost::str(boost::format(string("#define ")+name+" %1$.12f\n") % x);
}


static GLuint LoadTexture(const string& filename, const float anisotropy = 0.0f, int* sizeX = NULL, int* sizeY = NULL)
{
	GLuint texID;
	CBitmap bm;
	if (!bm.Load(filename))
		throw content_error("Could not load texture from file "+filename);

	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	if (anisotropy > 0.0f)
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropy);
	glBuildMipmaps(GL_TEXTURE_2D, GL_RGBA8, bm.xsize, bm.ysize, GL_RGBA, GL_UNSIGNED_BYTE, bm.mem);

	if (sizeY != NULL) {
		*sizeX = bm.xsize;
		*sizeY = bm.ysize;
	}

	return texID;
}


static void DrawRadialDisc()
{
	//! SAME ALGORITHM AS FOR WATERPLANE IN BFGroundDrawer.cpp!
	const float xsize = (gs->mapx * SQUARE_SIZE) >> 2;
	const float ysize = (gs->mapy * SQUARE_SIZE) >> 2;

	CVertexArray *va = GetVertexArray();
	va->Initialize();
	va->CheckInitSize(4*34*2*VA_SIZE_0);

	const float alphainc = fastmath::PI2 / 32;
	const float alphaincpi2 = alphainc + fastmath::PI2;	
	float alpha,r1,r2;
	float3 p(0.0f,0.0f,0.0f);
	const float size = std::min(xsize,ysize);
	for (int n = 0; n < 4 ; ++n) {
		r1 = n*n * size;
		if (n==3) {
			r2 = (n+0.5)*(n+0.5) * size;
		}else{
			r2 = (n+1)*(n+1) * size;
		}
		for (alpha = 0.0f; alpha < alphaincpi2 ; alpha+=alphainc) {
			p.x = r1 * fastmath::sin(alpha) + 2 * xsize;
			p.z = r1 * fastmath::cos(alpha) + 2 * ysize;
			va->AddVertexQ0(p.x,p.y,p.z);
			p.x = r2 * fastmath::sin(alpha) + 2 * xsize;
			p.z = r2 * fastmath::cos(alpha) + 2 * ysize;
			va->AddVertexQ0(p.x,p.y,p.z);
		}
	}

	va->DrawArray0(GL_TRIANGLE_STRIP);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// (DE-)CONSTRUCTOR
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CBumpWater::CBumpWater()
{
	/** LOAD USER CONFIGS **/
	reflTexSize  = next_power_of_2(configHandler.GetInt("BumpWaterTexSizeReflection", 512));
	reflection   = !!configHandler.GetInt("BumpWaterReflection", 1);
	refraction   = configHandler.GetInt("BumpWaterRefraction", 1);  /// 0:=off, 1:=screencopy, 2:=own rendering cycle
	anisotropy   = atof(configHandler.GetString("BumpWaterAnisotropy", "0.0").c_str());
	depthCopy    = !!configHandler.GetInt("BumpWaterUseDepthTexture", 1);
	depthBits    = configHandler.GetInt("BumpWaterDepthBits", (gu->atiHacks)?16:24);
	blurRefl     = !!configHandler.GetInt("BumpWaterBlurReflection", 0);
	shoreWaves   = (!!configHandler.GetInt("BumpWaterShoreWaves", 1)) && mapInfo->water.shoreWaves;
	endlessOcean = (!!configHandler.GetInt("BumpWaterEndlessOcean", 1)) && mapInfo->hasWaterPlane;
	dynWaves     = (!!configHandler.GetInt("BumpWaterDynamicWaves", 1)) && (mapInfo->water.numTiles>1);

	if (refraction>1)
		drawSolid = true;


	/** CHECK HARDWARE **/
	if (!GL_ARB_shading_language_100)
		throw content_error("BumpWater: your hardware/driver setup does not support GLSL.");

	shoreWaves = shoreWaves && (GLEW_EXT_framebuffer_object);
	dynWaves   = dynWaves && (GLEW_EXT_framebuffer_object && GLEW_ARB_imaging);


	/** LOAD TEXTURES **/
	foamTexture     = LoadTexture( mapInfo->water.foamTexture );
	normalTexture   = LoadTexture( mapInfo->water.normalTexture , anisotropy , &normalTextureX, &normalTextureY);

	//! caustic textures
	const vector<string>& causticNames = mapInfo->water.causticTextures;
	if (causticNames.size() <= 0) {
		throw content_error("no caustic textures");
	}
	for (int i = 0; i < (int)causticNames.size(); ++i) {
		caustTextures.push_back(LoadTexture(causticNames[i]));
	}


	/** SHOREWAVES **/
	if (shoreWaves) {
		pboID = 0;
		if (GLEW_EXT_pixel_buffer_object) {
			glGenBuffers(1, &pboID);
		}

		coastmapNeedUpload=false;
		coastmapNeedUpdate=false;
		coastUploadX1 = (int)1e9;
		coastUploadX2 = -1;
		coastUploadZ1 = (int)1e9;
		coastUploadZ2 = -1;

		waveRandTexture = LoadTexture( "bitmaps/shorewaverand.bmp" );

		glGenTextures(2, coastTexture);
		glBindTexture(GL_TEXTURE_2D, coastTexture[0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F_ARB, gs->mapx, gs->mapy, 0, GL_RGBA, GL_FLOAT, NULL);
		glGenerateMipmapEXT(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, coastTexture[1]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F_ARB, gs->mapx, gs->mapy, 0, GL_RGBA, GL_FLOAT, NULL);

		const string fsSource = LoadShaderSource("shaders/bumpWaterCoastBlurFS.glsl");
		const GLchar* fsSourceStr = fsSource.c_str();
		blurFP = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(blurFP, 1, &fsSourceStr, NULL);
		glCompileShader(blurFP);
		PrintShaderLog(blurFP, "blurFP");

		blurShader = glCreateProgram();
		glAttachShader(blurShader, blurFP);
		glLinkProgram(blurShader);
		PrintShaderLog(blurShader, "blurShader");
		blurDirLoc = glGetUniformLocation(blurShader, "blurDir");
		blurTexLoc = glGetUniformLocation(blurShader, "texture");

		glGenFramebuffersEXT(1,&coastFBO);
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, coastFBO);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, coastTexture[0], 0);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_TEXTURE_2D, coastTexture[1], 0);
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

		//! initialize
		UploadCoastline(0, 0, gs->mapx, gs->mapy);
		UpdateCoastmap(0, 0, gs->mapx, gs->mapy);
	}


	/** CREATE TEXTURES **/
	if ((refraction>0) || depthCopy) {
		//! ati's don't have glsl support for texrects
		if (GLEW_ARB_texture_rectangle && !GLEW_ATI_envmap_bumpmap) {
			target = GL_TEXTURE_RECTANGLE_ARB;
			screenTextureX = gu->viewSizeX;
			screenTextureY = gu->viewSizeY;
		}else{
			target = GL_TEXTURE_2D;
			screenTextureX = next_power_of_2(gu->viewSizeX);
			screenTextureY = next_power_of_2(gu->viewSizeY);
		}
	}

	if (refraction>0) {
		//! CREATE REFRACTION TEXTURE
		glGenTextures(1, &refractTexture);
		glBindTexture(target, refractTexture);
		glTexParameteri(target,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameteri(target,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		if (GLEW_EXT_texture_edge_clamp) {
			glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		} else {
			glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP);
		}
		glTexImage2D(target, 0, GL_RGBA8, screenTextureX, screenTextureY, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	}

	if (reflection) {
		//! CREATE REFLECTION TEXTURE
		glGenTextures(1, &reflectTexture);
		glBindTexture(GL_TEXTURE_2D, reflectTexture);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		if (GLEW_EXT_texture_edge_clamp) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		} else {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		}
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, reflTexSize, reflTexSize, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	}

	if (depthCopy) {
		//! CREATE DEPTH TEXTURE
		glGenTextures(1, &depthTexture);
		glBindTexture(target, depthTexture);
		glTexParameteri(target,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameteri(target,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		GLuint depthFormat = GL_DEPTH_COMPONENT; 
		switch (gu->depthBufferBits) { /// use same depth as screen framebuffer
			case 16: depthFormat = GL_DEPTH_COMPONENT16; break;
			case 24: depthFormat = GL_DEPTH_COMPONENT24; break;
			case 32: depthFormat = GL_DEPTH_COMPONENT32; break;
		}
		glTexImage2D(target, 0, depthFormat, screenTextureX, screenTextureY, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	}

	if (dynWaves) {
		//! SETUP DYNAMIC WAVES
		tileOffsets = SAFE_NEW unsigned char[mapInfo->water.numTiles * mapInfo->water.numTiles];

		normalTexture2 = normalTexture;
		glBindTexture(GL_TEXTURE_2D, normalTexture2);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 0.0);

		glGenTextures(1, &normalTexture);
		glBindTexture(GL_TEXTURE_2D, normalTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		if (anisotropy>0.0f)
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropy);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, normalTextureX, normalTextureY, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glGenerateMipmapEXT(GL_TEXTURE_2D);
	}

	/** CREATE FBOs **/
	if (GLEW_EXT_framebuffer_object) {
		GLuint depthRBOFormat = GL_DEPTH_COMPONENT;
		switch (depthBits) {
			case 16: depthRBOFormat = GL_DEPTH_COMPONENT16; break;
			case 24: depthRBOFormat = GL_DEPTH_COMPONENT24; break;
			case 32: depthRBOFormat = GL_DEPTH_COMPONENT32; break;
		}

		if (reflection) {
			glGenRenderbuffersEXT(1, &reflectRBO);
			glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, reflectRBO);
			glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, depthRBOFormat, reflTexSize, reflTexSize);

			glGenFramebuffersEXT(1,&reflectFBO);
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, reflectFBO);
			glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, reflectRBO);
			glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, reflectTexture, 0);
			GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
			if (status != GL_FRAMEBUFFER_COMPLETE_EXT) {
				PrintFboError("reflection",status);
				glDeleteRenderbuffersEXT(1, &reflectRBO);
				glDeleteFramebuffersEXT(1,  &reflectFBO);
				reflectFBO = 0;
			}
		}

		if (refraction>0) {
			glGenRenderbuffersEXT(1, &refractRBO);
			glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, refractRBO);
			glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, depthRBOFormat, screenTextureX, screenTextureY);

			glGenFramebuffersEXT(1,&refractFBO);
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, refractFBO);
			glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, refractRBO);
			glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, target, refractTexture, 0);
			GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
			if (status != GL_FRAMEBUFFER_COMPLETE_EXT) {
				PrintFboError("refraction",status);
				glDeleteRenderbuffersEXT(1, &refractRBO);
				glDeleteFramebuffersEXT(1,  &refractFBO);
				refractFBO = 0;
			}
		}

		if (dynWaves) {
			glGenFramebuffersEXT(1,&dynWavesFBO);
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, dynWavesFBO);
			glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, normalTexture, 0);
			GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
			if (status != GL_FRAMEBUFFER_COMPLETE_EXT) {
				PrintFboError("dynamicWaves",status);
				glDeleteFramebuffersEXT(1,  &dynWavesFBO);
				dynWavesFBO = 0;
			}else{
				UpdateDynWaves(true); //! initialize
			}
		}

	}


	/** DEFINE SOME SHADER RUNTIME CONSTANTS (I don't use Uniforms for that, because the glsl compiler can't optimize those!) **/
	string definitions;
	if (reflection)   definitions += "#define use_reflection\n";
	if (refraction>0) definitions += "#define use_refraction\n";
	if (shoreWaves)   definitions += "#define use_shorewaves\n";
	if (depthCopy)    definitions += "#define use_depth\n";
	if (blurRefl)     definitions += "#define blur_reflection\n";
	if (target==GL_TEXTURE_RECTANGLE_ARB) definitions += "#define use_texrect\n";
	GLSLDefineConstf4(definitions, "SurfaceColor",   mapInfo->water.surfaceColor*0.4, mapInfo->water.surfaceAlpha );
	GLSLDefineConstf4(definitions, "PlaneColor",     mapInfo->water.planeColor*0.4, mapInfo->water.surfaceAlpha );
	GLSLDefineConstf3(definitions, "DiffuseColor",   mapInfo->water.diffuseColor );
	GLSLDefineConstf3(definitions, "SpecularColor",  mapInfo->water.specularColor );
	GLSLDefineConstf1(definitions, "SpecularPower",  mapInfo->water.specularPower );
	GLSLDefineConstf1(definitions, "SpecularFactor", mapInfo->water.specularFactor );
	GLSLDefineConstf1(definitions, "AmbientFactor",  mapInfo->water.ambientFactor );
	GLSLDefineConstf1(definitions, "DiffuseFactor",  mapInfo->water.diffuseFactor*15.0f );
	GLSLDefineConstf3(definitions, "SunDir",         mapInfo->light.sunDir );
	GLSLDefineConstf3(definitions, "MapMid",         float3(readmap->width*SQUARE_SIZE*0.5f,0.0f,readmap->height*SQUARE_SIZE*0.5f) );
	GLSLDefineConstf2(definitions, "ScreenInverse",  1.0f/gu->viewSizeX, 1.0f/gu->viewSizeY );
	GLSLDefineConstf2(definitions, "ScreenTextureSizeInverse",  1.0f/screenTextureX, 1.0f/screenTextureY );
	GLSLDefineConstf2(definitions, "ViewPos",        gu->viewPosX,gu->viewPosY );
	GLSLDefineConstf1(definitions, "FresnelMin",     mapInfo->water.fresnelMin);
	GLSLDefineConstf1(definitions, "FresnelMax",     mapInfo->water.fresnelMax);
	GLSLDefineConstf1(definitions, "FresnelPower",   mapInfo->water.fresnelPower);
	GLSLDefineConstf1(definitions, "ReflDistortion", mapInfo->water.reflDistortion);
	GLSLDefineConstf2(definitions, "BlurBase",       0.0f,mapInfo->water.blurBase/gu->viewSizeY);
	GLSLDefineConstf1(definitions, "BlurExponent",   mapInfo->water.blurExponent);
	GLSLDefineConstf1(definitions, "PerlinStartFreq",  mapInfo->water.perlinStartFreq);
	GLSLDefineConstf1(definitions, "PerlinLacunarity", mapInfo->water.perlinLacunarity);
	GLSLDefineConstf1(definitions, "PerlinAmp",        mapInfo->water.perlinAmplitude);

	{
		const int mapX = readmap->width  * SQUARE_SIZE;
		const int mapZ = readmap->height * SQUARE_SIZE;
		const float shadingX = (float)gs->mapx / gs->pwr2mapx;
		const float shadingZ = (float)gs->mapy / gs->pwr2mapy;

		GLSLDefineConst4f(definitions, "TexGenPlane", 1.0f/mapX, 1.0f/mapZ, shadingX/mapX, shadingZ/mapZ);
	}

	/** LOAD SHADERS **/
	string vsSource = LoadShaderSource("shaders/bumpWaterVS.glsl");
	string fsSource = LoadShaderSource("shaders/bumpWaterFS.glsl");

	vector<GLint> lengths(2);
	vector<const GLchar*> strings(2);
	lengths[0] = definitions.length();
	strings[0] = definitions.c_str();

	waterVP = glCreateShader(GL_VERTEX_SHADER);
	lengths[1] = vsSource.length();
	strings[1] = vsSource.c_str();
	glShaderSource(waterVP, strings.size(), &strings.front(), &lengths.front());
	glCompileShader(waterVP);
	PrintShaderLog(waterVP, "waterVP");

	waterFP = glCreateShader(GL_FRAGMENT_SHADER);
	lengths[1] = fsSource.length();
	strings[1] = fsSource.c_str();
	glShaderSource(waterFP, strings.size(), &strings.front(), &lengths.front());
	glCompileShader(waterFP);
	PrintShaderLog(waterFP, "waterFP");

	waterShader = glCreateProgram();
	glAttachShader(waterShader, waterVP);
	glAttachShader(waterShader, waterFP);
	glLinkProgram(waterShader);
	PrintShaderLog(waterShader, "waterShader");


	/** BIND TEXTURE UNIFORMS **/
	glUseProgram(waterShader);
		eyePosLoc     = glGetUniformLocation(waterShader, "eyePos");
		frameLoc      = glGetUniformLocation(waterShader, "frame");

		GLuint normalmapLoc  = glGetUniformLocation(waterShader, "normalmap");
		GLuint heightmapLoc  = glGetUniformLocation(waterShader, "heightmap");
		GLuint causticLoc    = glGetUniformLocation(waterShader, "caustic");
		GLuint foamLoc       = glGetUniformLocation(waterShader, "foam");
		GLuint reflectionLoc = glGetUniformLocation(waterShader, "reflection");
		GLuint refractionLoc = glGetUniformLocation(waterShader, "refraction");
		GLuint depthmapLoc   = glGetUniformLocation(waterShader, "depthmap");
		GLuint coastmapLoc   = glGetUniformLocation(waterShader, "coastmap");
		GLuint waverandLoc   = glGetUniformLocation(waterShader, "waverand");

		glUniform1i(normalmapLoc, 0);
		glUniform1i(heightmapLoc, 1);
		glUniform1i(causticLoc, 2);
		glUniform1i(foamLoc, 3);
		glUniform1i(reflectionLoc, 4);
		glUniform1i(refractionLoc, 5);
		glUniform1i(coastmapLoc, 6);
		glUniform1i(depthmapLoc, 7);
		glUniform1i(waverandLoc, 8);
	glUseProgram(0);


	/** CREATE DISPLAYLIST **/
	displayList = glGenLists(1);
	glNewList(displayList,GL_COMPILE);
	if (endlessOcean) {
		DrawRadialDisc();
	}else{
		const int mapX = readmap->width  * SQUARE_SIZE;
		const int mapZ = readmap->height * SQUARE_SIZE;
		glBegin(GL_QUADS);
		glVertex3i(   0, 0, 0);
		glVertex3i(   0, 0, mapZ);
		glVertex3i(mapX, 0, mapZ);
		glVertex3i(mapX, 0, 0);
		glEnd();
	}
	glEndList();
}


CBumpWater::~CBumpWater()
{
	if (reflection)
		glDeleteTextures(1, &reflectTexture);
	if (refraction>0)
		glDeleteTextures(1, &refractTexture);
	if (depthCopy)
		glDeleteTextures(1, &depthTexture);

	if (reflectFBO) {
		glDeleteRenderbuffersEXT(1, &reflectRBO);
		glDeleteFramebuffersEXT(1,  &reflectFBO);
	}

	if (refractFBO) {
		glDeleteRenderbuffersEXT(1, &refractRBO);
		glDeleteFramebuffersEXT(1,  &refractFBO);
	}

	glDeleteTextures(1, &foamTexture);
	glDeleteTextures(1, &normalTexture);
	for (int i = 0; i < (int)caustTextures.size(); ++i) {
		glDeleteTextures(1, &caustTextures[i]);
	}

	glDeleteShader(waterVP);
	glDeleteShader(waterFP);
	glDeleteProgram(waterShader);

	glDeleteLists(displayList,1);

	if (shoreWaves) {
		glDeleteTextures(2, coastTexture);
		glDeleteTextures(1, &waveRandTexture);

		if (pboID)
			glDeleteBuffers(1, &pboID);

		if (coastFBO)
			glDeleteFramebuffersEXT(1, &coastFBO);

		glDeleteShader(blurFP);
		glDeleteProgram(blurShader);
	}

	if (dynWaves) {
		glDeleteTextures(1, &normalTexture2);

		delete[] tileOffsets;

		if (dynWavesFBO)
			glDeleteFramebuffersEXT(1, &dynWavesFBO);
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///  UPDATE
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CBumpWater::Update()
{
	if ((!mapInfo->water.forceRendering && readmap->currMinHeight > 1.0f) || mapInfo->map.voidWater)
		return;

	float3 w = wind.GetCurrentWind();
	texcoord1.x += w.x*0.001f;
	texcoord1.y += w.z*0.001f;

	if (dynWaves)
		UpdateDynWaves();

	if (!shoreWaves)
		return;

	SCOPED_TIMER("Coastmap");

	if (coastmapNeedUpload && ((gs->frameNum + 1) % 15)==0)
	{
		UploadCoastline(coastUploadX1, coastUploadZ1, coastUploadX2, coastUploadZ2);

		coastUpdateX1 = coastUploadX1;
		coastUpdateX2 = coastUploadX2;
		coastUpdateZ1 = coastUploadZ1;
		coastUpdateZ2 = coastUploadZ2;
		coastmapNeedUpdate = true;

		coastUploadX1 = gs->mapx+1;
		coastUploadX2 = -1;
		coastUploadZ1 = gs->mapy+1;
		coastUploadZ2 = -1;
		coastmapNeedUpload = false;

	}

	if (coastmapNeedUpdate && ((gs->frameNum + 14) % 15)==0)
	{
		UpdateCoastmap(coastUpdateX1, coastUpdateZ1, coastUpdateX2, coastUpdateZ2);
		coastmapNeedUpdate = false;
	}
}


void CBumpWater::UpdateWater(CGame* game)
{
	if ((!mapInfo->water.forceRendering && readmap->currMinHeight > 1.0f) || mapInfo->map.voidWater)
		return;

	if (refraction>1) DrawRefraction(game);
	if (reflection)   DrawReflection(game);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///  SHOREWAVES/COASTMAP
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CBumpWater::HeightmapChanged(const int x1, const int y1, const int x2, const int y2)
{
	if (!shoreWaves)
		return;

	if (x1<coastUploadX1) coastUploadX1=x1;
	if (x2>coastUploadX2) coastUploadX2=x2;
	if (y1<coastUploadZ1) coastUploadZ1=y1;
	if (y2>coastUploadZ2) coastUploadZ2=y2;

	coastmapNeedUpload = true;
}


void CBumpWater::UploadCoastline(const int x1, const int y1, const int x2, const int y2)
{
	GLfloat* coastmap=NULL;
	bool usedPBO = false;

	if (pboID) {
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pboID);
		glBufferData(GL_PIXEL_UNPACK_BUFFER, gs->mapx*gs->mapy*4*4, 0, GL_STREAM_DRAW);
		coastmap = (GLfloat*)glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
		usedPBO = true;
	}

	if(coastmap == NULL) {
		coastmap = SAFE_NEW GLfloat[gs->mapx*gs->mapy*4];
		usedPBO = false;
	}

	/*for (int y = 0; y < gs->mapy; y++) {
		for (int x = 0; x < gs->mapx; x++) {
			float3& normal = readmap->facenormals[(y*gs->mapx+x)*2];
			normalmap[(y*gs->mapx+x)*4]   = normal.x;
			normalmap[(y*gs->mapx+x)*4+1] = normal.y;
			normalmap[(y*gs->mapx+x)*4+2] = normal.z;
			normalmap[(y*gs->mapx+x)*4+3] = 1;
		}
	}*/

	int xmin = max(x1 - 10*2,0);
	int xmax = min(x2 + 10*2,gs->mapx);
	int ymin = max(y1 - 10*2,0);
	int ymax = min(y2 + 10*2,gs->mapy);
	int xsize = xmax - xmin;
	int ysize = ymax - ymin;

	const float* heightMap = readmap->GetHeightmap();
	for (int y = ymin; y < ymax; ++y) {
		int yindex  = y*(gs->mapx + 1);
		int yindex2 = (y-ymin)*xsize;
		for (int x = xmin; x < xmax; ++x) {
			int index  = yindex + x;
			int index2 = (yindex2 + (x-xmin)) << 2;
			const float& height = heightMap[index];
			coastmap[index2]   = (height>0.0f)?1:0;
			coastmap[index2+1] = (height>0.0f)?1:0;
			coastmap[index2+2] = (height>0.0f)?1:0;
			coastmap[index2+3] = height;
		}
	}


	if (usedPBO) {
		glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
		glBindTexture(GL_TEXTURE_2D, coastTexture[1]);
		glTexSubImage2D(GL_TEXTURE_2D, 0, xmin, ymin, xsize,ysize, GL_RGBA, GL_FLOAT, 0);
		glBufferData(GL_PIXEL_UNPACK_BUFFER, 0, 0, GL_STREAM_DRAW); //free it
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
	}else{
		glBindTexture(GL_TEXTURE_2D, coastTexture[1]);
		glTexSubImage2D(GL_TEXTURE_2D, 0, xmin, ymin, xsize,ysize, GL_RGBA, GL_FLOAT, coastmap);
		delete[] coastmap;
	}
}


void CBumpWater::UpdateCoastmap(const int x1, const int y1, const int x2, const int y2)
{
	glDisable(GL_BLEND);
	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, coastFBO);
	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if (status == GL_FRAMEBUFFER_COMPLETE_EXT) {
		glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, coastTexture[1]);
		glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, coastTexture[0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glMatrixMode(GL_TEXTURE);
			glPushMatrix();
			glLoadIdentity();
			glScalef(1.0f/gs->mapx, 1.0f/gs->mapy, 1);
		glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();
			glOrtho(0,1,0,1,-1,1);
		glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();
			glScalef(1.0f/gs->mapx, 1.0f/gs->mapy, 1);

		glViewport(0,0,gs->mapx, gs->mapy);
		glUseProgram(blurShader);

		int xmin = max(x1 - 10*2,0);
		int xmax = min(x2 + 10*2,gs->mapx);
		int ymin = max(y1 - 10*2,0);
		int ymax = min(y2 + 10*2,gs->mapy);
		//int xsize = xmax - xmin;
		//int ysize = ymax - ymin;

		glUniform2f(blurDirLoc,1.0f/gs->mapx,0.0f);
		glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
		glUniform1i(blurTexLoc,1);

		glBegin(GL_QUADS);
			glTexCoord2f(xmin,ymin); glVertex2f(xmin,ymin);
			glTexCoord2f(xmin,ymax); glVertex2f(xmin,ymax);
			glTexCoord2f(xmax,ymax); glVertex2f(xmax,ymax);
			glTexCoord2f(xmax,ymin); glVertex2f(xmax,ymin);
		glEnd();

		for (int i=0; i<5; ++i){
			glUniform2f(blurDirLoc,0.0f,1.0f/gs->mapy);
			glDrawBuffer(GL_COLOR_ATTACHMENT1_EXT);
			glUniform1i(blurTexLoc,0);

			glBegin(GL_QUADS);
				glTexCoord2f(xmin,ymin); glVertex2f(xmin,ymin);
				glTexCoord2f(xmin,ymax); glVertex2f(xmin,ymax);
				glTexCoord2f(xmax,ymax); glVertex2f(xmax,ymax);
				glTexCoord2f(xmax,ymin); glVertex2f(xmax,ymin);
			glEnd();

			glUniform2f(blurDirLoc,1.0f/gs->mapx,0.0f);
			glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
			glUniform1i(blurTexLoc,1);

			glBegin(GL_QUADS);
				glTexCoord2f(xmin,ymin); glVertex2f(xmin,ymin);
				glTexCoord2f(xmin,ymax); glVertex2f(xmin,ymax);
				glTexCoord2f(xmax,ymax); glVertex2f(xmax,ymax);
				glTexCoord2f(xmax,ymin); glVertex2f(xmax,ymin);
			glEnd();
		}

		glMatrixMode(GL_TEXTURE);
			glPopMatrix();
		glMatrixMode(GL_PROJECTION);
			glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
			glPopMatrix();

		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, coastTexture[0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glGenerateMipmapEXT(GL_TEXTURE_2D);

		glUseProgram(0);
		glViewport(gu->viewPosX,0,gu->viewSizeX,gu->viewSizeY);
	}else PrintFboError("coast",status);

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///  DYNAMIC WAVES
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CBumpWater::UpdateDynWaves(const bool initialize)
{
	if (!dynWaves || !dynWavesFBO)
		return;

	static const unsigned char tiles  = mapInfo->water.numTiles; //! (numTiles <= 16)
	static const unsigned char ntiles = mapInfo->water.numTiles * mapInfo->water.numTiles;
	static const float tilesize = 1.0f/mapInfo->water.numTiles;

	const int f = (gs->frameNum+1) % 60;

	if ( f == 0) {
		for (unsigned char i=0; i<ntiles; ++i) {
			do {
				tileOffsets[i] = (unsigned char)(gu->usRandFloat()*ntiles);
			} while(tileOffsets[i] == i);
		}
	}

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, dynWavesFBO);
	glBindTexture(GL_TEXTURE_2D, normalTexture2);
	glBlendFunc(GL_CONSTANT_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA);
	glBlendColor(1.0f,1.0f,1.0f, (initialize) ? 1.0f : (f + 1)/600.0f );
	glColor4f(1.0f,1.0f,1.0f,1.0f);

	glViewport(0,0, normalTextureX, normalTextureY);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0,1,0,1,-1,1);
	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glLoadIdentity();

	glBegin(GL_QUADS);
	unsigned char offset,tx,ty;
	unsigned char *to=tileOffsets;
	unsigned char yts=0;
	for (unsigned char y=0; y<tiles; ++y) {
		unsigned char yts1=yts+tilesize;
		unsigned char xts=0;
		for (unsigned char x=0; x<tiles; ++x) {
			unsigned char xts1=xts+tilesize;
			offset = *to++;
			tx = offset % tiles;
			ty = ((offset - tx)/tiles)*tilesize;
			tx*=tilesize;
			glTexCoord2f(tx,          ty         ); glVertex2f(xts,  yts );
			glTexCoord2f(tx,          ty+tilesize); glVertex2f(xts,  yts1);
			glTexCoord2f(tx+tilesize, ty+tilesize); glVertex2f(xts1, yts1);
			glTexCoord2f(tx+tilesize, ty         ); glVertex2f(xts1, yts );
			xts=xts1;
		}
		yts=yts1;
	}
	glEnd();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glPopMatrix();
	glMatrixMode(GL_PROJECTION);
		glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
	glViewport(gu->viewPosX,0,gu->viewSizeX,gu->viewSizeY);

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	glBindTexture(GL_TEXTURE_2D, normalTexture);
	glGenerateMipmapEXT(GL_TEXTURE_2D);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///  DRAW FUNCTIONS
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CBumpWater::Draw()
{
	if (!mapInfo->water.forceRendering && readmap->currMinHeight > 1.0f)
		return;

	if (refraction == 1) {
		//! _SCREENCOPY_ REFRACT TEXTURE
		glBindTexture(target, refractTexture);
		glEnable(target);
		glCopyTexSubImage2D(target, 0, 0, 0, gu->viewPosX, 0, gu->viewSizeX, gu->viewSizeY);
		glDisable(target);
	}

	if (depthCopy) {
		//! _SCREENCOPY_ DEPTH TEXTURE
		glBindTexture(target, depthTexture);
		glEnable(target);
		glCopyTexSubImage2D(target, 0, 0, 0, gu->viewPosX, 0, gu->viewSizeX, gu->viewSizeY);
		glDisable(target);
	}

	glDisable(GL_ALPHA_TEST);
	if (refraction<2)
		glDepthMask(0);
	if (refraction>0)
		glDisable(GL_BLEND);

	const int causticTexNum = (gs->frameNum % caustTextures.size());
	glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, readmap->GetShadingTexture());
	glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, caustTextures[causticTexNum]);
	glActiveTexture(GL_TEXTURE3); glBindTexture(GL_TEXTURE_2D, foamTexture);
	glActiveTexture(GL_TEXTURE4); glBindTexture(GL_TEXTURE_2D, reflectTexture);
	glActiveTexture(GL_TEXTURE5); glBindTexture(target,        refractTexture);
	glActiveTexture(GL_TEXTURE6); glBindTexture(GL_TEXTURE_2D, coastTexture[0]);
	glActiveTexture(GL_TEXTURE7); glBindTexture(target,        depthTexture);
	glActiveTexture(GL_TEXTURE8); glBindTexture(GL_TEXTURE_2D, waveRandTexture);
	glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, normalTexture);

	glUseProgram(waterShader);
	glUniform1f(frameLoc,  gs->frameNum / 15000.0f);
	glUniformf3(eyePosLoc, camera->pos);

	glCallList(displayList);

	glUseProgram(0);

	if (refraction<2)
		glDepthMask(1);
	if (refraction>0)
		glEnable(GL_BLEND);
}


void CBumpWater::DrawRefraction(CGame* game)
{
	//! _RENDER_ REFRACTION TEXTURE
	if (refractFBO)
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, refractFBO);

	camera->Update(false);
	glViewport(0,0,screenTextureX,screenTextureY);

	glClear(GL_DEPTH_BUFFER_BIT);
	glDisable(GL_FOG);

	float3 oldsun=unitDrawer->unitSunColor;
	float3 oldambient=unitDrawer->unitAmbientColor;

	unitDrawer->unitSunColor*=float3(0.5f,0.7f,0.9f);
	unitDrawer->unitAmbientColor*=float3(0.6f,0.8f,1.0f);

	game->SetDrawMode(CGame::refractionDraw);
	drawRefraction=true;

	glEnable(GL_CLIP_PLANE2);
	const double plane[4]={0,-1,0,5};
	glClipPlane(GL_CLIP_PLANE2 ,plane);

	readmap->GetGroundDrawer()->Draw();
	unitDrawer->Draw(false,true);
	featureHandler->Draw();
	unitDrawer->DrawCloakedUnits();
	ph->Draw(false,true);
	eventHandler.DrawWorldRefraction();

	glDisable(GL_CLIP_PLANE2);
	game->SetDrawMode(CGame::normalDraw);
	drawRefraction=false;

	if (refractFBO) {
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	}else{
		glBindTexture(target, refractTexture);
		glEnable(target);
		glCopyTexSubImage2D(target,0,0,0,0,0,screenTextureX,screenTextureY);
		glDisable(target);
	}

	glViewport(gu->viewPosX,0,gu->viewSizeX,gu->viewSizeY);
	glEnable(GL_FOG);

	unitDrawer->unitSunColor=oldsun;
	unitDrawer->unitAmbientColor=oldambient;
}


void CBumpWater::DrawReflection(CGame* game)
{
	//! CREATE REFLECTION TEXTURE
	if (reflectFBO)
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, reflectFBO);

	CCamera *realCam = camera;
	camera = new CCamera(*realCam);
	camera->up.x=0;
	camera->up.y=1;
	camera->up.z=0;
	camera->forward.y*=-1;
	camera->pos.y*=-1;
	camera->Update(false);

	glViewport(0,0,reflTexSize,reflTexSize);
	glClear(GL_DEPTH_BUFFER_BIT);

	game->SetDrawMode(CGame::reflectionDraw);
	sky->Draw();

	glEnable(GL_CLIP_PLANE2);
	const double plane[4]={0,1,0,1};
	glClipPlane(GL_CLIP_PLANE2 ,plane);
	drawReflection=true;

	readmap->GetGroundDrawer()->Draw(true);
	unitDrawer->Draw(true);
	featureHandler->Draw();
	ph->Draw(true);
	eventHandler.DrawWorldReflection();

	game->SetDrawMode(CGame::normalDraw);
	drawReflection=false;
	glDisable(GL_CLIP_PLANE2);

	if (reflectFBO) {
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	}else{
		glBindTexture(GL_TEXTURE_2D, reflectTexture);
		glEnable(GL_TEXTURE_2D);
		glCopyTexSubImage2D(GL_TEXTURE_2D,0,0,0,0,0,reflTexSize, reflTexSize);
		glDisable(GL_TEXTURE_2D);
	}

	glViewport(gu->viewPosX,0,gu->viewSizeX,gu->viewSizeY);

	delete camera;
	camera = realCam;
	camera->Update(false);
}

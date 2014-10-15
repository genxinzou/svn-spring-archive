// s3oParser.cpp: implementation of the Cs3oParser class.
//
//////////////////////////////////////////////////////////////////////
//#include "StdAfx.h"
#include "StdAfx.h"
#include "s3oParser.h"
#include <fstream>
#include "Rendering/GL/myGL.h"
#include <algorithm>
#include <locale>
#include <cctype>
#include "FileSystem/FileHandler.h"
#include "s3o.h"
#include "Rendering/FartextureHandler.h"
#include "Sim/Units/COB/CobInstance.h"
#include "Rendering/Textures/TextureHandler.h"
#include "Platform/byteorder.h"
#include "Platform/errorhandler.h"
#include <stdexcept>
#include "mmgr.h"

void SS3O::DrawStatic()
{
	glPushMatrix();
	glTranslatef(offset.x,offset.y,offset.z);
	glCallList(displist);
	for(unsigned int i=0; i<childs.size(); i++)
		childs[i]->DrawStatic();
	glPopMatrix();
}

SS3O::~SS3O()
{
	glDeleteLists(displist,1);
}


CS3OParser::CS3OParser()
{

}

CS3OParser::~CS3OParser()
{
	map<string,CModel*>::iterator ui;
	for(ui=units.begin();ui!=units.end();++ui){
		DeleteSS3O(ui->second->rootobjects3o);
		delete ui->second;
	}
}

void CS3OParser::DeleteSS3O(SS3O* o)
{
	for(std::vector<SS3O*>::iterator di=o->childs.begin();di!=o->childs.end();di++){
		DeleteSS3O(*di);
	}
	delete o;
}

CModel* CS3OParser::Load3DO(string name,float scale,int side)
{
	if(name.find(".")==string::npos)
		name+=".s3o";

	StringToLowerInPlace(name);

	map<string,CModel*>::iterator ui;
	if((ui=units.find(name))!=units.end()){
		return ui->second;
	}

	PUSH_CODE_MODE;
	ENTER_SYNCED;

	CFileHandler file(name);
	if(!file.FileExists()){
		POP_CODE_MODE;
		throw content_error("File not found: "+name);
	}
	unsigned char* fileBuf=new unsigned char[file.FileSize()];
	file.Read(fileBuf, file.FileSize());
	S3OHeader header;
	memcpy(&header,fileBuf,sizeof(header));
	
	CModel *model = new CModel;
	model->numobjects=0;
	SS3O* object=LoadPiece(fileBuf,header.rootPiece,model,-1);
	model->rootobjects3o=object;
	model->rootobject3do=0;
	object->isEmpty=true;
	model->name=name;
	model->textureType=texturehandler->LoadS3OTexture((char*)&fileBuf[header.texture1],(char*)&fileBuf[header.texture2]);

	FindMinMax(object);

	units[name]=model;

	CreateLists(object);

	model->radius = header.radius*scale;		//this is a hack to make aircrafts less likely to collide and get hit by nontracking weapons
	model->height = header.height;
	model->relMidPos.x=header.midx;
	model->relMidPos.y=header.midy;
	model->relMidPos.z=header.midz;
	if(model->relMidPos.y<1)
		model->relMidPos.y=1;

//	info->AddLine("%s has height %f",name,model->height);
	fartextureHandler->CreateFarTexture(model);

	model->maxx=model->rootobjects3o->maxx;
	model->maxy=model->rootobjects3o->maxy;
	model->maxz=model->rootobjects3o->maxz;

	model->minx=model->rootobjects3o->minx;
	model->miny=model->rootobjects3o->miny;
	model->minz=model->rootobjects3o->minz;

	delete[] fileBuf;
	POP_CODE_MODE;
	return model;
}

CModelInstance* CS3OParser::CreateLocalModel(CModel *model, IModelAnimator *anim)
{
	CModelInstance *lmodel = new CModelInstance;
	lmodel->numpieces = model->numobjects;

	int piecenum=0;
	lmodel->pieces = new LocalS3DO[model->numobjects];
	lmodel->pieces->parent = NULL;

	CreateLocalModel(model->rootobjects3o, lmodel, &piecenum);

	return lmodel;
}

void CS3OParser::CreateLocalModel(SS3O *model, CModelInstance *lmodel, int *piecenum)
{
	PUSH_CODE_MODE;
	ENTER_SYNCED;
	lmodel->pieces[*piecenum].displist = model->displist;
	lmodel->pieces[*piecenum].offset = model->offset;
	lmodel->pieces[*piecenum].name = model->name;
	lmodel->pieces[*piecenum].originals3o = model;
	lmodel->pieces[*piecenum].original3do = 0;
	lmodel->pieces[*piecenum].pieceIndex = *piecenum;

	int thispiece = *piecenum;
	for(unsigned int i=0; i<model->childs.size(); i++)
	{
		(*piecenum)++;
		lmodel->pieces[thispiece].childs.push_back(&lmodel->pieces[*piecenum]);
		lmodel->pieces[*piecenum].parent = &lmodel->pieces[thispiece];
		CreateLocalModel(model->childs[i], lmodel, piecenum);
	}
	POP_CODE_MODE;
}

SS3O* CS3OParser::LoadPiece(unsigned char* buf, int offset,CModel* model, int parent)
{
	int cur = model->numobjects++;

	SS3O* piece=new SS3O;

	Piece* fp=(Piece*)&buf[offset];

	piece->offset.x=fp->xoffset;
	piece->offset.y=fp->yoffset;
	piece->offset.z=fp->zoffset;
	piece->primitiveType=fp->primitiveType;
	piece->name=(char*)&buf[fp->name];

	ModelPieceInfo mpi;
	mpi.name = piece->name;
	// ModelPieceInfo::offset is in non-TA style coordinates, 
	// so the animation doesn't have to mess around with negating z
	mpi.offset = float3(piece->offset.x, piece->offset.y, -piece->offset.z);
	mpi.parent = parent;
	model->pieceInfo.push_back(mpi);

	int vertexPointer=fp->vertices;
	for(int a=0;a<fp->numVertices;++a){
		piece->vertices.push_back(*(SS3OVertex*)&buf[vertexPointer]);
/*		piece->vertices.back().normal.x=piece->vertices.back().pos.x;
		piece->vertices.back().normal.y=piece->vertices.back().pos.y;
		piece->vertices.back().normal.z=piece->vertices.back().pos.z;
		piece->vertices.back().normal.Normalize();*/
		vertexPointer+=sizeof(Vertex);
	}
	int vertexTablePointer=fp->vertexTable;
	for(int a=0;a<fp->vertexTableSize;++a){
		int num=*(int*)&buf[vertexTablePointer];
		piece->vertexDrawOrder.push_back(num);
		vertexTablePointer+=sizeof(int);

		if(num==-1 && a!=fp->vertexTableSize-1){		//for triangle strips
			piece->vertexDrawOrder.push_back(num);

			num=*(int*)&buf[vertexTablePointer];
			piece->vertexDrawOrder.push_back(num);				
		}
	}
	int childPointer=fp->childs;
	for(int a=0;a<fp->numChilds;++a){
		piece->childs.push_back(LoadPiece(buf,*(int*)&buf[childPointer],model,cur));
		childPointer+=sizeof(int);
	}

	return piece;
}

void CS3OParser::FindMinMax(SS3O *object)
{
	std::vector<SS3O*>::iterator si;
	for(si=object->childs.begin();si!=object->childs.end();++si){
		FindMinMax(*si);
	}

	float maxx=-1000,maxy=-1000,maxz=-1000;
	float minx=10000,miny=10000,minz=10000;

	std::vector<SS3OVertex>::iterator vi;
	for(vi=object->vertices.begin();vi!=object->vertices.end();++vi){
		maxx=max(maxx,vi->pos.x);
		maxy=max(maxy,vi->pos.y);
		maxz=max(maxz,vi->pos.z);

		minx=min(minx,vi->pos.x);
		miny=min(miny,vi->pos.y);
		minz=min(minz,vi->pos.z);
	}
	for(si=object->childs.begin();si!=object->childs.end();++si){
		maxx=max(maxx,(*si)->offset.x+(*si)->maxx);
		maxy=max(maxy,(*si)->offset.y+(*si)->maxy);
		maxz=max(maxz,(*si)->offset.z+(*si)->maxz);

		minx=min(minx,(*si)->offset.x+(*si)->minx);
		miny=min(miny,(*si)->offset.y+(*si)->miny);
		minz=min(minz,(*si)->offset.z+(*si)->minz);
	}
	object->maxx=maxx;
	object->maxy=maxy;
	object->maxz=maxz;

	object->minx=minx;
	object->miny=miny;
	object->minz=minz;
}

void CS3OParser::DrawSub(SS3O* o)
{
	glVertexPointer(3,GL_FLOAT,sizeof(SS3OVertex),&o->vertices[0].pos.x);
	glTexCoordPointer(2,GL_FLOAT,sizeof(SS3OVertex),&o->vertices[0].textureX);
	glNormalPointer(GL_FLOAT,sizeof(SS3OVertex),&o->vertices[0].normal.x);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	
	switch(o->primitiveType){
	case 0:
		glDrawElements(GL_TRIANGLES,o->vertexDrawOrder.size(),GL_UNSIGNED_INT,&o->vertexDrawOrder[0]);
		break;
	case 1:
		glDrawElements(GL_TRIANGLE_STRIP,o->vertexDrawOrder.size(),GL_UNSIGNED_INT,&o->vertexDrawOrder[0]);
		break;
	case 2:
		glDrawElements(GL_QUADS,o->vertexDrawOrder.size(),GL_UNSIGNED_INT,&o->vertexDrawOrder[0]);
		break;
	}
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);						
	glDisableClientState(GL_NORMAL_ARRAY);

}


void CS3OParser::CreateLists(SS3O *o)
{
	o->displist=glGenLists(1);
	PUSH_CODE_MODE;
	ENTER_MIXED;
	glNewList(o->displist,GL_COMPILE);
	DrawSub(o);
	glEndList();
	POP_CODE_MODE;

	for(std::vector<SS3O*>::iterator bs=o->childs.begin();bs!=o->childs.end();bs++){
		CreateLists(*bs);
	}
}

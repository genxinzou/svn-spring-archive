#include "stdafx.h"
#include ".\moveinfo.h"
#include "filehandler.h"
#include "sunparser.h"
#include "infoconsole.h"
#include "readmap.h"
//#include "mmgr.h"

CMoveInfo* moveinfo;
using namespace std;

CMoveInfo::CMoveInfo(void)
{
	sunparser=new CSunParser;
	sunparser->LoadFile("gamedata\\moveinfo.tdf");

	moveInfoChecksum=0;

	for(int num=0;ClassExists(num);++num){
		char c[100];
		sprintf(c,"class%i",num);
		string className=c;
	
		MoveData* md=new MoveData;
		string name=sunparser->SGetValueDef("",className+"\\name");

		md->maxSlope=1;
		md->depth=0;
		md->pathType=num;
		md->crushStrength = atof(sunparser->SGetValueDef("10",className+"\\CrushStrength").c_str());
		if(name.find("BOAT")!=string::npos || name.find("SHIP")!=string::npos){
			md->moveType=MoveData::Ship_Move;
			md->depth=atof(sunparser->SGetValueDef("10",className+"\\MinWaterDepth").c_str());
//			info->AddLine("class %i %s boat",num,name.c_str());
			md->moveFamily=3;
		} else if(name.find("HOVER")!=string::npos){
			md->moveType=MoveData::Hover_Move;
			md->maxSlope=1-cos(atof(sunparser->SGetValueDef("10",className+"\\MaxSlope").c_str())*1.5*PI/180);
			md->moveFamily=2;
//			info->AddLine("class %i %s hover",num,name.c_str());
		} else {
			md->moveType=MoveData::Ground_Move;	
			md->depthMod=0.1;
			md->depth=atof(sunparser->SGetValueDef("0",className+"\\MaxWaterDepth").c_str());
			md->maxSlope=1-cos(atof(sunparser->SGetValueDef("60",className+"\\MaxSlope").c_str())*1.5*PI/180);
//			info->AddLine("class %i %s ground",num,name.c_str());
			if(name.find("TANK")!=string::npos)
				md->moveFamily=0;
			else
				md->moveFamily=1;
		}
		md->slopeMod=4/(md->maxSlope+0.001);
		md->size=max(2,min(8,atoi(sunparser->SGetValueDef("1", className+"\\FootprintX").c_str())*2));//ta has only half our res so multiply size with 2
//		info->AddLine("%f %i",md->slopeMod,md->size);
		moveInfoChecksum+=md->size;
		moveInfoChecksum^=*(unsigned int*)&md->slopeMod;
		moveInfoChecksum+=*(unsigned int*)&md->depth;

		moveData.push_back(md);
		name2moveData[name]=md->pathType;
	}

	for(int a=0;a<256;++a){
		terrainType2MoveFamilySpeed[a][0]=readmap->terrainTypes[a].tankSpeed;
		terrainType2MoveFamilySpeed[a][1]=readmap->terrainTypes[a].kbotSpeed;
		terrainType2MoveFamilySpeed[a][2]=readmap->terrainTypes[a].hoverSpeed;
		terrainType2MoveFamilySpeed[a][3]=readmap->terrainTypes[a].shipSpeed;
	}
	delete sunparser;
}

CMoveInfo::~CMoveInfo(void)
{
	while(!moveData.empty()){
		delete moveData.back();
		moveData.pop_back();
	}
}

MoveData* CMoveInfo::GetMoveDataFromName(std::string name)
{
	return moveData[name2moveData[name]];
}

bool CMoveInfo::ClassExists(int num)
{
	char className[100];
	sprintf(className,"class%i",num);
	
	std::vector<string> sections=sunparser->GetSectionList("");
	for(vector<string>::iterator si=sections.begin();si!=sections.end();++si){
		if((*si)==className)
			return true;
	}
	return false;
}

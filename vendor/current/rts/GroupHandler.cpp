#include "stdafx.h"
// GroupHandler.cpp: implementation of the CGroupHandler class.
//
//////////////////////////////////////////////////////////////////////

#include "GroupHandler.h"
#include <windows.h>
#include <io.h>
#include "group.h"
#include "igroupai.h"
#include "infoconsole.h"
#include "selectedunits.h"
#include "timeprofiler.h"
#include "unit.h"
#include ".\grouphandler.h"
#include "mousehandler.h"
#include "cameracontroller.h"
//#include "mmgr.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGroupHandler* grouphandler;
extern bool	keys[256];

CGroupHandler::CGroupHandler(int team)
: firstUnusedGroup(10),
	team(team)
{
	availableAI["default"]="default";
	FindDlls();

	for(int a=0;a<10;++a){
		groups.push_back(new CGroup("default",a,this));
	}
}

CGroupHandler::~CGroupHandler()
{
	for(int a=0;a<firstUnusedGroup;++a)
		delete groups[a];
}

void CGroupHandler::Update()
{
START_TIME_PROFILE;
	for(std::vector<CGroup*>::iterator ai=groups.begin();ai!=groups.end();++ai)
		if((*ai)!=0)
			(*ai)->Update();
END_TIME_PROFILE("Group AI");
}

void CGroupHandler::TestDll(string name)
{
	typedef int (WINAPI* GETGROUPAIVERSION)();
	typedef void (WINAPI* GETAINAME)(char* c);
	
	HINSTANCE m_hDLL;
	GETGROUPAIVERSION GetGroupAiVersion;
	GETAINAME GetAiName;

	m_hDLL=LoadLibrary(name.c_str());
	if (m_hDLL==0){
		MessageBox(NULL,name.c_str(),"Cant load dll",MB_OK|MB_ICONEXCLAMATION);
		return;
	}

	GetGroupAiVersion = (GETGROUPAIVERSION)GetProcAddress(m_hDLL,"GetGroupAiVersion");
	if (GetGroupAiVersion==0){
		MessageBox(NULL,name.c_str(),"Incorrect AI dll",MB_OK|MB_ICONEXCLAMATION);
		return;
	}

	int i=GetGroupAiVersion();

	if (i!=AI_INTERFACE_VERSION){
		MessageBox(NULL,name.c_str(),"Incorrect AI dll version",MB_OK|MB_ICONEXCLAMATION);
		return;
	}
	
	GetAiName = (GETAINAME)GetProcAddress(m_hDLL,"GetAiName");
	if (GetAiName==0){
		MessageBox(NULL,name.c_str(),"Incorrect AI dll",MB_OK|MB_ICONEXCLAMATION);
		return;
	}

	char c[500];
	GetAiName(c);

	availableAI[name]=c;
//	(*info) << name.c_str() << " " << c << "\n";
	FreeLibrary(m_hDLL);
}

void CGroupHandler::GroupCommand(int num)
{
	if(keys[VK_CONTROL]){
		if(!keys[VK_SHIFT])
			groups[num]->ClearUnits();
		set<CUnit*>::iterator ui;
		for(ui=selectedUnits.selectedUnits.begin();ui!=selectedUnits.selectedUnits.end();++ui){
			(*ui)->SetGroup(groups[num]);
		}
	}
	if(selectedUnits.selectedGroup==num && !groups[num]->units.empty()){
		float3 p(0,0,0);
		for(std::set<CUnit*>::iterator gi=groups[num]->units.begin();gi!=groups[num]->units.end();++gi){
			p+=(*gi)->pos;
		}
		p/=groups[num]->units.size();
		mouse->currentCamController->SetPos(p);
	}
	selectedUnits.SelectGroup(num);
}

void CGroupHandler::FindDlls(void)
{
	struct _finddata_t files;    
	long hFile;
	int morefiles=0;

	if( (hFile = _findfirst( "aidll\\*.dll", &files )) == -1L ){
		morefiles=-1;
	}

	int numfiles=0;
	while(morefiles==0){
//		info->AddLine("Testing %s",files.name);
		TestDll(string("aidll\\")+files.name);
		
		morefiles=_findnext( hFile, &files ); 
	}

	if(availableAI.empty()){
		MessageBox(0,"Fatal error","Need at least one valid ai dll in ./aidll",0);
		exit(0);
	}
}

CGroup* CGroupHandler::CreateNewGroup(string ainame)
{
	if(freeGroups.empty()){
		CGroup* group=new CGroup(ainame,firstUnusedGroup++,this);
		groups.push_back(group);
		if(group!=groups[group->id]){
			MessageBox(0,"Id error when creating group","Error",0);
		}
		return group;
	} else {
		int id=freeGroups.back();
		freeGroups.pop_back();
		CGroup* group=new CGroup(ainame,id,this);
		groups[id]=group;
		return group;
	}
}

void CGroupHandler::RemoveGroup(CGroup* group)
{
	if(group->id<10){
		info->AddLine("Warning trying to remove hotkey group %i",group->id);
		return;
	}
	if(selectedUnits.selectedGroup==group->id)
		selectedUnits.ClearSelected();
	groups[group->id]=0;
	freeGroups.push_back(group->id);
	delete group;
}

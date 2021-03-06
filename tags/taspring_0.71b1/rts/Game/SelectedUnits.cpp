// SelectedUnits.cpp: implementation of the CSelectedUnits class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SelectedUnits.h"
#include "Sim/Units/Unit.h"
#include <map>
#include "Rendering/GL/myGL.h"
#include "Sim/Units/UnitHandler.h"
#include "Net.h"
#include "ExternalAI/GroupHandler.h"
#include "ExternalAI/Group.h"
#include "UI/InfoConsole.h"
#include "Camera.h"
#include "Rendering/UnitModels/3DOParser.h"
#include "Sim/Units/CommandAI/CommandAI.h"
#include "SelectedUnitsAI.h"
#include "Sound.h"
#include "Sim/Units/UnitDef.h"
#include "Player.h"
#include "mmgr.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSelectedUnits selectedUnits;

CSelectedUnits::CSelectedUnits()
: selectionChanged(false),
	possibleCommandsChanged(true),
	selectedGroup(-1)
{

}

CSelectedUnits::~CSelectedUnits()
{

}

CSelectedUnits::AvailableCommandsStruct CSelectedUnits::GetAvailableCommands()
{
	possibleCommandsChanged=false;

	if(selectedGroup!=-1 && grouphandler->groups[selectedGroup]->ai){
		AvailableCommandsStruct ac;
		ac.commandPage=grouphandler->groups[selectedGroup]->lastCommandPage;
		ac.commands=grouphandler->groups[selectedGroup]->GetPossibleCommands();

		CommandDescription c;			//make sure we can clear the group even when selected
		c.id=CMD_GROUPCLEAR;
		c.type=CMDTYPE_ICON;
		c.name="Clear group";
		c.tooltip="Removes the units from any group they belong to";
		c.key='Q';
		c.switchKeys=SHIFT_KEY;
		ac.commands.push_back(c);

		return ac;
	}

	int commandPage=1000;
	int foundGroup=-2;
	int foundGroup2=-2;
	map<int,int> count;

	for(set<CUnit*>::iterator ui=selectedUnits.begin();ui!=selectedUnits.end();++ui){
		vector<CommandDescription>* c=&(*ui)->commandAI->GetPossibleCommands();
		vector<CommandDescription>::iterator ci;
		for(ci=c->begin();ci!=c->end();++ci)
			count[ci->id]=1;
		if((*ui)->commandAI->lastSelectedCommandPage<commandPage)
			commandPage=(*ui)->commandAI->lastSelectedCommandPage;

		if(foundGroup==-2 && (*ui)->group)
			foundGroup=(*ui)->group->id;
		if(!(*ui)->group || foundGroup!=(*ui)->group->id)
			foundGroup=-1;

		if(foundGroup2==-2 && (*ui)->group)
			foundGroup2=(*ui)->group->id;
		if(foundGroup2>=0 && (*ui)->group && (*ui)->group->id!=foundGroup2)
			foundGroup2=-1;
	}

	vector<CommandDescription> commands ;
	for(set<CUnit*>::iterator ui=selectedUnits.begin();ui!=selectedUnits.end();++ui){
		vector<CommandDescription>* c=&(*ui)->commandAI->GetPossibleCommands();
		vector<CommandDescription>::iterator ci;
		for(ci=c->begin();ci!=c->end();++ci){
			if(ci->showUnique && selectedUnits.size()>1)
				continue;
			if(count[ci->id]>0){
				commands.push_back(*ci);
				count[ci->id]=0;
			}
		}
	}

	if(foundGroup!=-2){			//create a new group
		CommandDescription c;
		c.id=CMD_AISELECT;
		c.type=CMDTYPE_COMBO_BOX;
		c.name="Select AI";
		c.tooltip="Create a new group using the selected units and with the ai selected";
		c.key='Q';
		c.switchKeys=CONTROL_KEY;

		c.params.push_back("0");
		c.params.push_back("None");
		map<string,string>::iterator aai;
		for(aai=grouphandler->availableAI.begin();aai!=grouphandler->availableAI.end();++aai){
			c.params.push_back(aai->second.c_str());
		}
		commands.push_back(c);
	}

	if(foundGroup<0 && foundGroup2>=0){			//add the selected units to a previous group (that at least one unit is also selected from)
		CommandDescription c;
		c.id=CMD_GROUPADD;
		c.type=CMDTYPE_ICON;
		c.name="Add to group";
		c.tooltip="Adds the selected to an existing group (of which one or more units is already selected)";
		c.key='Q';
		commands.push_back(c);
	}

	if(foundGroup>=0){				//select the group to which the units belong
		CommandDescription c;

		c.id=CMD_GROUPSELECT;
		c.type=CMDTYPE_ICON;
		c.name="Select group";
		c.tooltip="Select the group that these units belong to";
		c.key='Q';
		commands.push_back(c);
	}

	if(foundGroup2!=-2){				//remove all selected units from any groups they belong to
		CommandDescription c;

		c.id=CMD_GROUPCLEAR;
		c.type=CMDTYPE_ICON;
		c.name="Clear group";
		c.tooltip="Removes the units from any group they belong to";
		c.key='Q';
		c.switchKeys=SHIFT_KEY;
		commands.push_back(c);
	}

	AvailableCommandsStruct ac;
	ac.commandPage=commandPage;
	ac.commands=commands;
	return ac;
}

void CSelectedUnits::GiveCommand(Command c,bool fromUser)
{
//	info->AddLine("Command given %i",c.id);
	if(gu->spectating || selectedUnits.empty())
		return;

	if(fromUser){		//add some statistics
		gs->players[gu->myPlayerNum]->currentStats->numCommands++;
		if(selectedGroup!=-1){
			gs->players[gu->myPlayerNum]->currentStats->unitCommands+=grouphandler->groups[selectedGroup]->units.size();
		} else {
			gs->players[gu->myPlayerNum]->currentStats->unitCommands+=selectedUnits.size();
		}
	}

	if(c.id==CMD_GROUPCLEAR){
		for(set<CUnit*>::iterator ui=selectedUnits.begin();ui!=selectedUnits.end();++ui){
			if((*ui)->group)
				(*ui)->SetGroup(0);
		}
		return;
	}

	if(selectedGroup!=-1 && (grouphandler->groups[selectedGroup]->ai || c.id==CMD_AISELECT)){
		grouphandler->groups[selectedGroup]->GiveCommand(c);
		return;
	}

	if(c.id==CMD_GROUPSELECT){
		SelectGroup((*selectedUnits.begin())->group->id);
		return;
	}

	if(c.id==CMD_GROUPADD){
		CGroup* group=0;
		for(set<CUnit*>::iterator ui=selectedUnits.begin();ui!=selectedUnits.end();++ui){
			if((*ui)->group){
				group=(*ui)->group;
				break;
			}
		}
		if(group){
			for(set<CUnit*>::iterator ui=selectedUnits.begin();ui!=selectedUnits.end();++ui){
				if(!(*ui)->group)
					(*ui)->SetGroup(group);
			}	
			SelectGroup(group->id);
		}
		return;
	}

	if(c.id==CMD_AISELECT){
		if(c.params[0]!=0){
			map<string,string>::iterator aai;
			int a=0;
			for(aai=grouphandler->availableAI.begin();aai!=grouphandler->availableAI.end() && a<c.params[0]-1;++aai){
				a++;
			}
			CGroup* group=grouphandler->CreateNewGroup(aai->first);

			for(set<CUnit*>::iterator ui=selectedUnits.begin();ui!=selectedUnits.end();++ui){
				(*ui)->SetGroup(group);
			}
			SelectGroup(group->id);
		}
		return;
	}
//	selectedUnitsAI.GiveCommand(c);

	SendCommand(c);

	if(!selectedUnits.empty()){
		set<CUnit*>::iterator ui = selectedUnits.begin();
		if((*ui)->unitDef->sounds.ok.id)
			sound->PlayUnitReply((*ui)->unitDef->sounds.ok.id, (*ui), (*ui)->unitDef->sounds.ok.volume, true);
	}
}

void CSelectedUnits::AddUnit(CUnit* unit)
{
	selectedUnits.insert(unit);
	AddDeathDependence(unit);
	selectionChanged=true;
	possibleCommandsChanged=true;
	selectedGroup=-1;
	PUSH_CODE_MODE;
	ENTER_MIXED;
	unit->commandAI->selected=true;
	POP_CODE_MODE;
}

void CSelectedUnits::RemoveUnit(CUnit* unit)
{
	selectedUnits.erase(unit);
	DeleteDeathDependence(unit);
	selectionChanged=true;
	possibleCommandsChanged=true;
	selectedGroup=-1;
	PUSH_CODE_MODE;
	ENTER_MIXED;
	unit->commandAI->selected=false;
	POP_CODE_MODE;
}

void CSelectedUnits::ClearSelected()
{
	set<CUnit*>::iterator ui;
	ENTER_MIXED;
	for(ui=selectedUnits.begin();ui!=selectedUnits.end();++ui){
		(*ui)->commandAI->selected=false;
		DeleteDeathDependence(*ui);
	}
	ENTER_UNSYNCED;

	selectedUnits.clear();
	selectionChanged=true;
	possibleCommandsChanged=true;
	selectedGroup=-1;
}

void CSelectedUnits::SelectGroup(int num)
{
	ClearSelected();
	selectedGroup=num;
	CGroup* group=grouphandler->groups[num];

	set<CUnit*>::iterator ui;
	ENTER_MIXED;
	for(ui=group->units.begin();ui!=group->units.end();++ui){
		(*ui)->commandAI->selected=true;
		selectedUnits.insert(*ui);
		AddDeathDependence(*ui);
	}
	ENTER_UNSYNCED;

	possibleCommandsChanged=true;
	selectionChanged=true;
}

void CSelectedUnits::Draw()
{
	glDisable(GL_DEPTH_TEST );
	//glEnable(GL_BLEND);
	//glColor4f(0,0.8f,0,0.4f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(0,1.0f,0);
	set<CUnit*>::iterator ui;
	glBegin(GL_QUADS);
	if(selectedGroup!=-1){
		for(ui=grouphandler->groups[selectedGroup]->units.begin();ui!=grouphandler->groups[selectedGroup]->units.end();++ui){
			float3 pos((*ui)->pos+(*ui)->speed*gu->timeOffset);
			glVertexf3(pos+float3((*ui)->xsize*4,0,(*ui)->ysize*4));
			glVertexf3(pos+float3(-(*ui)->xsize*4,0,(*ui)->ysize*4));
			glVertexf3(pos+float3(-(*ui)->xsize*4,0,-(*ui)->ysize*4));
			glVertexf3(pos+float3((*ui)->xsize*4,0,-(*ui)->ysize*4));
		}
	} else {
		for(ui=selectedUnits.begin();ui!=selectedUnits.end();++ui){
			float3 pos((*ui)->pos+(*ui)->speed*gu->timeOffset);
			glVertexf3(pos+float3((*ui)->xsize*4,0,(*ui)->ysize*4));
			glVertexf3(pos+float3(-(*ui)->xsize*4,0,(*ui)->ysize*4));
			glVertexf3(pos+float3(-(*ui)->xsize*4,0,-(*ui)->ysize*4));
			glVertexf3(pos+float3((*ui)->xsize*4,0,-(*ui)->ysize*4));
		}
	}
	glEnd();
	glEnable(GL_DEPTH_TEST );
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glDisable(GL_BLEND);
}

void CSelectedUnits::DependentDied(CObject *o)
{
	selectedUnits.erase((CUnit*)o);
	selectionChanged=true;
	possibleCommandsChanged=true;
}

void CSelectedUnits::NetSelect(vector<int>& s,int player)
{
	netSelected[player]=s;
}

void CSelectedUnits::NetOrder(Command &c, int player)
{
	selectedUnitsAI.GiveCommandNet(c,player);

/*	if(!(c.options & CONTROL_KEY) && c.params.size()==3) {//fix: some better way to detect if its a map position
		float3 oldPos(0,0,0);
		vector<int>::iterator ui;
		for(ui=netSelected[player].begin();ui!=netSelected[player].end();++ui){
			oldPos+=uh->units[*ui]->pos;
		}
		oldPos/=netSelected[player].size();
		float3 newPos(c.params[0],c.params[1],c.params[2]);
		float3 dif=newPos-oldPos;
		for(ui=netSelected[player].begin();ui!=netSelected[player].end();++ui){
			newPos=uh->units[*ui]->pos+dif;
			c.params[0]=newPos.x;
			c.params[1]=newPos.y;
			c.params[2]=newPos.z;
			uh->units[*ui]->commandAI->GiveCommand(c);
			uh->units[*ui]->commandAI->lastUserCommand=gs->frameNum;
		}
	} else {
		vector<int>::iterator ui;
		for(ui=netSelected[player].begin();ui!=netSelected[player].end();++ui){
			if(uh->units[*ui]!=0){
				uh->units[*ui]->commandAI->GiveCommand(c);
				uh->units[*ui]->commandAI->lastUserCommand=gs->frameNum;
			}
		}
	}*/
}

bool CSelectedUnits::CommandsChanged()
{
	return possibleCommandsChanged;
}

int CSelectedUnits::GetDefaultCmd(CUnit *unit,CFeature* feature)
{
	if(selectedGroup!=-1 && grouphandler->groups[selectedGroup]->ai){
		return grouphandler->groups[selectedGroup]->GetDefaultCmd(unit,feature);
	}
	int cmd=CMD_STOP;
	int lowestHint=10000;//find better way to decide
	CUnit* selected=0;
	set<CUnit*>::iterator ui;
	for(ui=selectedUnits.begin();ui!=selectedUnits.end();++ui){
		if((*ui)->aihint<lowestHint){
			selected=*ui;
			lowestHint=(*ui)->aihint;
		}
	}
	if(selected)
		cmd=selected->commandAI->GetDefaultCmd(unit,feature);
	return cmd;
}

void CSelectedUnits::AiOrder(int unitid, Command &c)
{
	if(uh->units[unitid]!=0)
		uh->units[unitid]->commandAI->GiveCommand(c);	
}


void CSelectedUnits::PossibleCommandChange(CUnit* sender)
{
	if(sender==0 || selectedUnits.find(sender)!=selectedUnits.end())
		possibleCommandsChanged=true;
}

void CSelectedUnits::DrawCommands(void)
{
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	set<CUnit*>::iterator ui;
	if(selectedGroup!=-1){
		for(ui=grouphandler->groups[selectedGroup]->units.begin();ui!=grouphandler->groups[selectedGroup]->units.end();++ui){
			(*ui)->commandAI->DrawCommands();
		}
	} else {
		for(ui=selectedUnits.begin();ui!=selectedUnits.end();++ui){
			(*ui)->commandAI->DrawCommands();
		}
	}
	glEnable(GL_DEPTH_TEST);
}

std::string CSelectedUnits::GetTooltip(void)
{
	std::string s;
	if(selectedGroup!=-1 && grouphandler->groups[selectedGroup]->ai){
		s="Group selected";
	} else if(!selectedUnits.empty()){
		s=(*selectedUnits.begin())->tooltip;
	}
	if(selectedUnits.empty()){
		return s;
	}
	char tmp[500];
	float maxHealth=0,curHealth=0,cost=0,exp=0,range=0,metalMake=0,metalUse=0,energyMake=0,energyUse=0;
	for(set<CUnit*>::iterator ui=selectedUnits.begin();ui!=selectedUnits.end();++ui){
		maxHealth+=(*ui)->maxHealth;
		curHealth+=(*ui)->health;
		exp+=(*ui)->experience;
		cost+=(*ui)->metalCost+(*ui)->energyCost/60;
		range+=(*ui)->maxRange;
		metalMake+=(*ui)->metalMake;
		metalUse+=(*ui)->metalUse;
		energyMake+=(*ui)->energyMake;
		energyUse+=(*ui)->energyUse;

	}
	float num=selectedUnits.size();
	sprintf(tmp,"\nHealth %.0f/%.0f \nExperience %.2f Cost %.0f Range %.0f \n\xff\xd3\xdb\xffMetal: \xff\x50\xff\x50%.1f\xff\x90\x90\x90/\xff\xff\x50\x01-%.1f\xff\xd3\xdb\xff Energy: \xff\x50\xff\x50%.1f\xff\x90\x90\x90/\xff\xff\x50\x01-%.1f",
			curHealth,maxHealth,exp/num,cost,range/num, metalMake, metalUse, energyMake, energyUse);
	s+=tmp;

	return s;
}

void CSelectedUnits::SetCommandPage(int page)
{
	if(selectedGroup!=-1 && grouphandler->groups[selectedGroup]->ai){
		grouphandler->groups[selectedGroup]->lastCommandPage=page;
	}
	if(!selectedUnits.empty())
		(*selectedUnits.begin())->commandAI->lastSelectedCommandPage=page;
}

void CSelectedUnits::SendSelection(void)
{
	// first, convert CUnit* to unit IDs.
	std::vector<short> selectedUnitIDs(selectedUnits.size());
	std::vector<short>::iterator i = selectedUnitIDs.begin();
	std::set<CUnit*>::const_iterator ui = selectedUnits.begin();
	for(; ui != selectedUnits.end(); ++i, ++ui) *i = (*ui)->id;
	net->SendSTLData<unsigned char, std::vector<short> >(NETMSG_SELECT, gu->myPlayerNum, selectedUnitIDs);
	selectionChanged=false;
}

void CSelectedUnits::SendCommand(Command& c)
{
	if(selectionChanged){		//send new selection
		SendSelection();
	}
	net->SendSTLData<unsigned char, int, unsigned char, std::vector<float> >(
			NETMSG_COMMAND, gu->myPlayerNum, c.id, c.options, c.params);
}

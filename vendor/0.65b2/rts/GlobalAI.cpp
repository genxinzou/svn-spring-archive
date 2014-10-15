#include ".\globalai.h"
#include "iglobalai.h"
#include "globalaicallback.h"
#include "grouphandler.h"
//#include "mmgr.h"

CGlobalAI::CGlobalAI(int team, const char* dll)
: team(team)
{
	ai=0;
	m_hDLL=0;

	m_hDLL=LoadLibrary(dll);
	if (m_hDLL==0){
		MessageBox(NULL,dll,"Could not find AI dll",MB_OK|MB_ICONEXCLAMATION);
		return;
	}
	
	GetGlobalAiVersion = (GETGLOBALAIVERSION)GetProcAddress(m_hDLL,"GetGlobalAiVersion");
	if (GetGlobalAiVersion==0){
		MessageBox(NULL,dll,"Incorrect Global AI dll",MB_OK|MB_ICONEXCLAMATION);
		return;
	}
	
	int i=GetGlobalAiVersion();

	if (i!=GLOBAL_AI_INTERFACE_VERSION){
		MessageBox(NULL,dll,"Incorrect Global AI dll version",MB_OK|MB_ICONEXCLAMATION);
		return;
	}

	GetNewAI = (GETNEWAI)GetProcAddress(m_hDLL,"GetNewAI");
	ReleaseAI = (RELEASEAI)GetProcAddress(m_hDLL,"ReleaseAI");

	ai=GetNewAI();
	callback=new CGlobalAICallback(this);
	gh=new CGroupHandler(team);
	ai->InitAI(callback,team);
}

void CGlobalAI::PreDestroy ()
{
	callback->noMessages = true;
}

CGlobalAI::~CGlobalAI(void)
{
	if(ai){
		ReleaseAI(ai);
		delete callback;
		delete gh;
	}
}

void CGlobalAI::Update(void)
{
	gh->Update();
	ai->Update();
}

#pragma once

#include <windows.h>
#include "object.h"

class IGlobalAI;
class CGlobalAICallback;
class CGroupHandler;

class CGlobalAI :
	public CObject
{
public:
	CGlobalAI(int team, const char* dll);
	~CGlobalAI(void);

	void Update(void);
	void PreDestroy (); // called just before all the units are destroyed

	int team;

	IGlobalAI* ai;
	CGlobalAICallback* callback;
	CGroupHandler* gh;

	HINSTANCE m_hDLL;

	typedef int (WINAPI* GETGLOBALAIVERSION)();
	typedef IGlobalAI* (WINAPI* GETNEWAI)();
	typedef void (WINAPI* RELEASEAI)(IGlobalAI* i);
	
	GETGLOBALAIVERSION GetGlobalAiVersion;
	GETNEWAI GetNewAI;
	RELEASEAI ReleaseAI;
};

// SyncTracer.cpp: implementation of the CSyncTracer class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SyncTracer.h"
#include <stdio.h>
#include "Game/UI/InfoConsole.h"
#include "Platform/FileSystem.h"
#include "mmgr.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSyncTracer tracefile;

bool CSyncTracer::init()
{
#ifdef TRACE_SYNC
	if (logfile == 0) {
		char c[100];
		sprintf(c, "trace%i.log", gu->myTeam);
		logfile = filesystem.new_ofstream(c);
	}
#endif
	return logfile != 0;
}

CSyncTracer::CSyncTracer()
{
	file = 0;
	logfile = 0;
	nowActive = 0;
	firstActive = 0;
}

CSyncTracer::~CSyncTracer()
{
#ifdef TRACE_SYNC
	delete file;
	delete logfile;
#endif
}

void CSyncTracer::Commit()
{
#ifdef TRACE_SYNC
	if(file == 0){
		char c[100];
		sprintf(c, "trace%i.txt", gu->myTeam);
		file = filesystem.new_ofstream(c);
	}
#endif

	if (!file)
		return;

	(*file) << traces[firstActive].c_str();
	while(nowActive!=firstActive){
		firstActive++;
		if(firstActive==10)
			firstActive=0;
		(*file) << traces[firstActive].c_str();
	}
	traces[nowActive]="";
}

void CSyncTracer::NewInterval()
{
	nowActive++;
	if(nowActive==10)
		nowActive=0;
	traces[nowActive]="";
}

void CSyncTracer::DeleteInterval()
{
	if(firstActive!=nowActive)
		firstActive++;
	if(firstActive==10)
		firstActive=0;
}

CSyncTracer& CSyncTracer::operator<<(const char* c)
{
	traces[nowActive]+=c;
	if (init()) (*logfile) << c;
	return *this;
}

CSyncTracer& CSyncTracer::operator<<(const int i)
{
	char t[20];
	sprintf(t,"%d",i);
	traces[nowActive]+=t;
	if (init()) (*logfile) << i;
	return *this;
}

CSyncTracer& CSyncTracer::operator<<(const float f)
{
	char t[50];
	sprintf(t,"%f",f);
	traces[nowActive]+=t;
	if (init()) (*logfile) << f;
	return *this;
}


#pragma once
#include "inputreceiver.h"

class CTooltipConsole :
	public CInputReceiver
{
public:
	CTooltipConsole(void);
	~CTooltipConsole(void);
	void Draw(void);
	bool IsAbove(int x,int y);
};

extern CTooltipConsole* tooltip;
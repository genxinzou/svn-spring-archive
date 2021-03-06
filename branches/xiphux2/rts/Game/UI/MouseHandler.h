#ifndef MOUSEHANDLER_H
#define MOUSEHANDLER_H
// MouseHandler.h: interface for the CMouseHandler class.
//
//////////////////////////////////////////////////////////////////////

#include <string>
#include <map>

#include "MouseCursor.h"

#define NUM_BUTTONS 5

class CInputReceiver;
class CCameraController;

class CMouseHandler  
{
public:
	void UpdateCam();
	void ToggleState(bool shift);
	void HideMouse();
	void ShowMouse();
	void Draw();
	void MouseRelease(int x,int y,int button);
	void MousePress(int x,int y,int button);
	void MouseMove(int x,int y);
	CMouseHandler();
	virtual ~CMouseHandler();

	int lastx;  
	int lasty;  
	bool hide;
	bool locked;
	bool inStateTransit;
	bool invertMouse;
	double transitSpeed;

	struct ButtonPress{
		bool pressed;
		int x;
		int y;
		float3 camPos;
		float3 dir;
		double time;
		double lastRelease;
		int movement;
	};

	ButtonPress buttons[NUM_BUTTONS + 1]; /* One-bottomed. */
	float3 dir;

	CInputReceiver* activeReceiver;
	int activeButton;

	unsigned int cursorTex;
	std::string cursorText;
	std::string cursorTextRight;
	void DrawCursor(void);
	std::string GetCurrentTooltip(void);

	map<std::string, CMouseCursor *> cursors;
	//CMouseCursor *mc;

	CCameraController* currentCamController;
	std::vector<CCameraController*> camControllers;
	int currentCamControllerNum;

	int xButtonCounter;		//incremented with xbutton2 decremented with xbutton1, used for spacing buildings in guihandler
protected:
	int soundMultiselID;
public:
	void EmptyMsgQueUpdate(void);
};

extern CMouseHandler* mouse;

#endif /* MOUSEHANDLER_H */

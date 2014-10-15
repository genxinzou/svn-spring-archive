#include "GUIinput.h"
#include <string>
#include "GUIfont.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#include "guicontroller.h"

GUIinput::GUIinput(const int x1, const int y1, int w1, int h1, Functor1<const string&> s):GUIframe(x1,y1,w1, h1), enter(s)
{
	caption="";
}

GUIinput::~GUIinput()
{
}

void GUIinput::PrivateDraw()
{
	static GLuint tex=0;

	if(!tex)
		tex=Texture("bitmaps\\pane.bmp");

	glBindTexture(GL_TEXTURE_2D, tex);
	
	glPushAttrib(GL_CURRENT_BIT);
	glColor4f(1, 1, 1, 0.3);	
	DrawThemeRect(20, 128, w, h);	
	glPopAttrib();

	guifont->Print(0, (h-guifont->GetHeight())/2.0, caption);

	guifont->Print(guifont->GetWidth(caption)+5, (h-guifont->GetHeight())/2.0, userInput);
}

extern bool keys[256];
bool GUIinput::KeyAction(const int key)
{
	if(isHidden())
		return false;

	return true;
}

bool GUIinput::CharacterAction(const char key)
{
	if(isHidden())
		return false;

	if(key==VK_RETURN)
	{
		// send to delegate
		enter(userInput);
		userInput="";
	}
	else if(key==VK_ESCAPE)
	{
		userInput="";
	}	
	else if(key==VK_BACK)
	{
		if(userInput.size())
			userInput=userInput.substr(0, userInput.size()-1);
	}
	else
		userInput+=key;
	return true;
}


bool GUIinput::EventAction(const std::string& event)
{
	if(event=="activate")
		TakeFocus();
	else if(event=="deactivate")
		ReleaseFocus();
	else if(event=="togglefocus")
	{
		if(HasFocus())
		{
			ReleaseFocus();
		}
		else
		{
			TakeFocus();
		}
	}
	else if(event=="togglehidden")
	{
		if(isHidden())
			TakeFocus();
		GUIframe::EventAction(event);
	}
	else GUIframe::EventAction(event);

	return false;
}


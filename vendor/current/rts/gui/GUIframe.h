// GUIframe.h: interface for the GUIframe class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GUIFRAME_H__CC73D151_5BEB_49EC_9581_6715FB94464E__INCLUDED_)
#define AFX_GUIFRAME_H__CC73D151_5BEB_49EC_9581_6715FB94464E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <list>
#include <string>
#include <vector>

#include "globalstuff.h"
#include "mygl.h"

using namespace std;

class GUIframe;

#define GUI_TRANS 0.4f

class GUIresponder
{
public:
	virtual bool MouseDown(int x1, int y1, int button)=0;
	virtual bool MouseUp(int x1, int y1, int button)=0;
	virtual bool MouseMove(int x1, int y1, int xrel, int yrel, int buttons)=0;
	
	virtual bool KeyDown(const int key)=0;
	
	virtual bool Event(const std::string& event)=0;
};

class GUIdrawer
{
public:
	virtual void Draw()=0;
};

class GUIcaption
{
public:
	virtual void SetCaption(const string& caption)=0;
};

class GUIframe : public GUIresponder, public GUIdrawer
{
public:
	GUIframe(const int x1, const int y1, const int w1, const int h1);
	GUIframe();

	static void InitMainFrame();

	virtual ~GUIframe();

	virtual void AddChild(GUIframe *baby);
	virtual void RemoveChild(GUIframe *item);
	void AddSibling(GUIframe *sister);

	void Draw();

	bool MouseDown(int x1, int y1, int button);
	bool MouseUp(int x1, int y1, int button);
	bool KeyDown(const int key);
	bool Character(const char c);
	bool MouseMove(int x1, int y1, int xrel, int yrel, int buttons);
	bool Event(const std::string& event);
	
	GUIframe* ControlAtPos(int x1, int y1);

	void TakeFocus();
	void ReleaseFocus();
	bool HasFocus();

	// these are the coordinates relative to the parent frame
	int x,y,w,h;
	GUIframe *parent;

	bool isTopmost();
	void Raise()
	{
		if(parent)
			parent->RaiseChild(this);
	}
	virtual void RaiseChild(const GUIframe *child);

	bool isHidden() { return hidden; }
	void ToggleHidden();

	void SetIdentifier(const std::string& id) { identifier = id; }
	const string& Identifier() { return identifier; }

	virtual bool EventAction(const std::string& event);

	void SetTooltip(const std::string& id) { tooltip = id; };
	virtual string Tooltip();
	
	virtual void SizeChanged();
	
	virtual void SelectCursor() {};

protected:

	std::string identifier;
	std::string tooltip;

	std::list<GUIframe*> children;
	
	char* name;

	bool isMainFrame;
	bool hidden;

	void ChangeParent(GUIframe *mommy);

	virtual void PrivateDraw();

	virtual bool MouseDownAction(int x, int y, int button);
	virtual bool MouseUpAction(int x, int y, int button);
	virtual bool MouseMoveAction(int x, int y, int xrel, int yrel, int buttons);

	virtual bool KeyAction(const int key);
	virtual bool CharacterAction(const char c);

	
	bool IsInside(int x1, int y1)
	{
		if(x1<x||y1<y||x1>(x+w)||y1>(y+h))
			return false;
		return true;
	}
	
	bool LocalInside(int x1, int y1)
	{
		if(x1<0||y1<0||x1>w||y1>h)
			return false;
		return true;
	}

	GUIframe *inputFocus;

	void MakeChildFocus(GUIframe *child);
	
	struct Extent
	{
		int x;
		int y;
		int w;
		int h;
	};
};

extern GUIframe *mainFrame;

GLuint Texture(const std::string& name, const vector<PALETTEENTRY>* pvTransparentColors = NULL);
void DrawThemeRect(int edge, int size, int w, int h);


#endif // !defined(AFX_GUIFRAME_H__CC73D151_5BEB_49EC_9581_6715FB94464E__INCLUDED_)

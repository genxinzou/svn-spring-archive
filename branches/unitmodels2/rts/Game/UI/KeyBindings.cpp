#include "StdAfx.h"
// CKeyBindings.cpp: implementation of the CKeyBindings class.
//
//////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <cctype>
#include "KeyBindings.h"
#include "KeyCodes.h"
#include "SDL_keysym.h"
#include "Sim/Units/UnitDef.h"
#include "Sim/Units/UnitDefHandler.h"
#include "System/Platform/errorhandler.h"
#include "System/FileSystem/FileHandler.h"
#include "InfoConsole.h"


CKeyBindings* keyBindings = NULL;


static const struct DefaultBinding {
	const char* key;
	const char* action;
}
defaultBindings[] = {

	{ "esc",       "quit"  },
	{ "Shift+esc", "quit"  },

	{ "pause", "pause" },

	{ "b",   "debug"                 },
	{ "i",   "gameinfo"              },
	{ "o",   "singlestep"            },
	{ "c",   "controlunit"           },
	{ "h",   "sharedialog"           },
	{ "tab", "toggleoverview"        },
	{ "l",   "togglelos"             },
	{ ";",   "toggleradarandjammer"  },

	{ "Any+j",     "mouse2"     },
	{ "backspace", "mousestate" },
	{ "Ctrl+h",    "mousestate" }, // CTRL+BACKSPACE returns CTRL+h

	{ "enter",       "chat"           },
	{ "Alt+enter",   "chatally"       },
	{ "Alt+enter",   "chatswitchally" },
	{ "Shift+enter", "chatspec"       },
	{ "Shift+enter", "chatswitchspec" },
	{ "Ctrl+m",      "chatall"        },
	{ "Ctrl+m",      "chatswitchall"  }, // CTRL+ENTER returns CTRL+m

	{ "home", "increaseViewRadius"  },
	{ "end",  "decreaseViewRadius"  },

	{ "insert",  "speedup"   },
	{ "delete",  "slowdown"  },
	{ "=",       "speedup"   },
	{ "+",       "speedup"   },
	{ "Shift++", "speedup"   },
	{ "-",       "slowdown"  },

	{ "Shift+>", "incguiopacity"  },
	{ "Shift+<", "decguiopacity"  },

	{ "Any+0", "group0"  },
	{ "Any+1", "group1"  },
	{ "Any+2", "group2"  },
	{ "Any+3", "group3"  },
	{ "Any+4", "group4"  },
	{ "Any+5", "group5"  },
	{ "Any+6", "group6"  },
	{ "Any+7", "group7"  },
	{ "Any+8", "group8"  },
	{ "Any+9", "group9"  },

	{ ",", "prevmenu" },
	{ ".", "nextmenu" },

	{ "Ctrl+insert", "hotbind"   },
	{ "Ctrl+delete", "hotunbind" },
	
	{ "c", "controlunit"   },
	
	{ "[",     "incbuildfacing"  },
	{ "]",     "decbuildfacing"  },
	{ "Any+z", "incbuildspacing" },
	{ "Any+x", "decbuildspacing" },

	{ "s",       "stop"          },
	{ "w",       "wait"          },
	{ "m",       "move"          },
	{ "a",       "attack"        },
	{ "Alt+a",   "areaattack"    },
	{ "f",       "fight"         },
	{ "p",       "patrol"        },
	{ "g",       "guard"         },
	{ "d",       "dgun"          },
	{ "e",       "reclaim"       },
	{ "r",       "repair"        },
	{ "Alt+c",   "capture"       },
	{ "Ctrl+d",  "selfd"         },
	{ "l",       "loadunits"     },
	{ "u",       "unloadunits"   },
	{ "k",       "cloak"         },
	{ "x",       "onoff"         },

	{ "q",       "groupselect"   },
	{ "q",       "groupadd"      },
	{ "Ctrl+q",  "aiselect"      },
	{ "Shift+q", "groupclear"    },

	// for RadarAI
	{ "m", "minimapalert" },
	{ "t", "textalert"    },

	// Numpad -> Dev Playground
	{ "numpad0", "repeat"       },
	{ "numpad1", "movestate 0"  },
	{ "numpad2", "movestate 1"  },
	{ "numpad3", "movestate 2"  },
	{ "numpad4", "firestate 0"  },
	{ "numpad5", "firestate 1"  },
	{ "numpad6", "firestate 2"  },
	{ "numpad7", "onoff"        },
	{ "numpad8", "cloak"        },
	{ "numpad9", "trajectory"   },
	{ "numpad.", "showcommands" },
	{ "C+numpad7", "aiselect Simple formation"  },
	{ "C+numpad8", "aiselect Radar AI"          },
	{ "C+numpad4", "aiselect MexUpgrader AI"    },
	{ "C+numpad9", "aiselect Central build AI"  },
	{ "C+numpad5", "aiselect default"           },
	{ "C+numpad6", "aiselect None"              },
	{ "C+numpad0", "groupclear"                 },

	{ "t",       "track"       },
	{ "Ctrl+t",  "trackmode"   },
	{ "Shift+t", "trackoff"    },

	{ "Ctrl+f1",  "viewfps"    },
	{ "Ctrl+f2",  "viewta"     },
	{ "Ctrl+f3",  "viewtw"     },
	{ "Ctrl+f4",  "viewrot"    },
	
	{ "f1",  "showElevation"   },
	{ "f2",  "ShowPathMap"     },
	{ "f3",  "LastMsgPos"      },
	{ "f4",  "ShowMetalMap"    },
	{ "f5",  "hideinterface"   },
	{ "f6",  "NoSound"         },
	{ "f7",  "dynamicSky"      },
	{ "f8",  "savegame"        },
	{ "f9",  "showhealthbars"  },
	{ "f10", "createvideo"     },
	{ "f11", "screenshot png"  },
	{ "f12", "screenshot"      },

	{    "Alt+`", "drawlabel"  },
	
	// NOTE: Up bindings are currently converted to press bindings
	//       (see KeySet.cpp / DISALLOW_RELEASE_BINDINGS)
	
	{    "Any+`", "drawinmap"  },
	{ "Up+Any+`", "drawinmap"  },

	{    "Any+up",       "moveforward"  },
	{ "Up+Any+up",       "moveforward"  },
	{    "Any+down",     "moveback"     },
	{ "Up+Any+down",     "moveback"     },
	{    "Any+right",    "moveright"    },
	{ "Up+Any+right",    "moveright"    },
	{    "Any+left",     "moveleft"     },
	{ "Up+Any+left",     "moveleft"     },
	{    "Any+pageup",   "moveup"       },
	{ "Up+Any+pageup",   "moveup"       },
	{    "Any+pagedown", "movedown"     },
	{ "Up+Any+pagedown", "movedown"     },

	{    "Any+ctrl",     "moveslow"     },
	{ "Up+Any+ctrl",     "moveslow"     },
	{    "Any+shift",    "movefast"     },
	{ "Up+Any+shift",    "movefast"     }
};


/******************************************************************************/
//
// Parsing Routines
//

static int lineNumber = 0;
static bool inComment = false; // /* text */ comments are not implemented


// returns next line (without newlines)
static string GetLine(CFileHandler& fh)
{
	lineNumber++;
	char a;
	string s = "";
	while (true) {
		a = fh.Peek();
		if (a == EOF)  { break; }
		fh.Read(&a, 1);
		if (a == '\n') { break; }
		if (a != '\r') { s += a; }
	}
	return s;
}


// returns next non-blank line (without newlines or comments)
static string GetCleanLine(CFileHandler& fh)
{
	string::size_type pos;
	while (true) {
		if (fh.Eof()) {
			return ""; // end of file
		}
		string line = GetLine(fh);

		pos = line.find_first_not_of(" \t");
		if (pos == string::npos) {
			continue; // blank line
		}

		pos = line.find("//");
		if (pos != string::npos) {
			line.erase(pos);
			pos = line.find_first_not_of(" \t");
			if (pos == string::npos) {
				continue; // blank line (after removing comments)
			}
		}

		return line;
	}
}


static vector<string> Tokenize(const string& line, int minWords = 0)
{
	vector<string> words;
	string::size_type start;
	string::size_type end = 0;
	while (true) {
		start = line.find_first_not_of(" \t", end);
		if (start == string::npos) {
			break;
		}
		string word;
		if ((minWords > 0) && (words.size() >= minWords)) {
			word = line.substr(start);
			// strip trailing whitespace
			string::size_type pos = word.find_last_not_of(" \t");
			if (pos != (word.size() - 1)) {
				word.resize(pos + 1);
			}
			end = string::npos;
		}
		else {
			end = line.find_first_of(" \t", start);
			if (end == string::npos) {
				word = line.substr(start);
			} else {
				word = line.substr(start, end - start);
			}
		}
		words.push_back(word);
		if (end == string::npos) {
			break;
		}
	}
	
	return words;
}


/******************************************************************************/

CKeyBindings::Action::Action(const string& line)
{
	rawline = line;
	command = "";
	extra = "";
	vector<string> words = Tokenize(line, 1);
	if (words.size() > 0) {
		command = StringToLower(words[0]);
	}
	if (words.size() > 1) {
		extra = words[1];
	}
}


/******************************************************************************/
//
// CKeyBindings
//

CKeyBindings::CKeyBindings()
{
	debug = 0;
	fakeMetaKey = -1;
	userCommand = true;

	statefulCommands.insert("drawinmap");
	statefulCommands.insert("moveforward");
	statefulCommands.insert("moveback");
	statefulCommands.insert("moveright");
	statefulCommands.insert("moveleft");
	statefulCommands.insert("moveup");
	statefulCommands.insert("movedown");
	statefulCommands.insert("moveslow");
	statefulCommands.insert("movefast");
}


CKeyBindings::~CKeyBindings()
{
}


/******************************************************************************/

void CKeyBindings::OutputDebug(const char* msg) const
{
	printf("%s\n", msg);
	if (debug >= 2) {
		info->AddLine(string(msg));
	}
}


const CKeyBindings::ActionList&
	CKeyBindings::GetActionList(const CKeySet& ks) const
{
	static const ActionList empty;
	const ActionList* alPtr = NULL;

	if (ks.AnyMod()) {
		KeyMap::const_iterator it = bindings.find(ks);
		if (it == bindings.end()) {
			alPtr = &empty;
		} else {
			alPtr = &(it->second);
		}
	}
	else {
		// have to check for an AnyMod keyset as well as the normal one
		CKeySet anyMod = ks;
		anyMod.SetAnyBit();
		KeyMap::const_iterator nit = bindings.find(ks);
		KeyMap::const_iterator ait = bindings.find(anyMod);
		const bool haveNormal = (nit != bindings.end());
		const bool haveAnyMod = (ait != bindings.end());
		if (!haveNormal && !haveAnyMod) {
			alPtr = &empty;
		}
		else if (haveNormal && !haveAnyMod) {
			alPtr = &(nit->second);
		}
		else if (!haveNormal && haveAnyMod) {
			alPtr = &(ait->second);
		}
		else {
			// combine the two lists (normal first)
			static ActionList merged;
			merged = nit->second;
			const ActionList& aal = ait->second;
			for (int i = 0; i < (int)aal.size(); ++i) {
				merged.push_back(aal[i]);
			}
			alPtr = &merged;
		}
	}

	if (debug > 0) {
		char buf[256];
		SNPRINTF(buf, sizeof(buf), "GetAction: %s (0x%03X)",
		         ks.GetString(false).c_str(), ks.Key());
		if (alPtr == &empty) {
			strncat(buf, "  EMPTY", sizeof(buf));
			OutputDebug(buf);
		}
		else {	
			OutputDebug(buf);
			const ActionList& al = *alPtr;
			for (int i = 0; i < (int)al.size(); ++i) {
				SNPRINTF(buf, sizeof(buf), "  %s  \"%s\"",
				         al[i].command.c_str(), al[i].rawline.c_str());
				OutputDebug(buf);
			}
		}
	}

	return *alPtr;
}


const CKeyBindings::HotkeyList&
	CKeyBindings::GetHotkeys(const string& action) const
{
	ActionMap::const_iterator it = hotkeys.find(action);
	if (it == hotkeys.end()) {
		static HotkeyList empty;
		return empty;
	}
	const HotkeyList& hl = it->second;
	return hl;
}


/******************************************************************************/

bool CKeyBindings::Bind(const string& keystr, const string& line)
{
	CKeySet ks;
	if (!ParseKeySet(keystr, ks)) {
		printf("Bind: could not parse key: %s\n", keystr.c_str());
		return false;
	}
	Action action(line);
	action.boundWith = keystr;
	if (action.command.empty()) {
		printf("Bind: empty action: %s\n", line.c_str());
		return false;
	}
	
	// Try to be safe, force AnyMod mode for stateful commands
	if (statefulCommands.find(action.command) != statefulCommands.end()) {
		ks.SetAnyBit();
	}

	KeyMap::iterator it = bindings.find(ks);
	if (it == bindings.end()) {
		// new entry, push it
		ActionList& al = bindings[ks];
		al.push_back(action);
	} else {
		if (it->first != ks) {
			// not a match, push it
			ActionList& al = it->second;
			al.push_back(action);
		}
		else {
			// an exact keyset match, check the command
			ActionList& al = it->second;
			int i;
			for (i = 0; i < (int)al.size(); i++) {
				if (action.command == al[i].command) {
					break;
				}
			}
			if (i == (int)al.size()) {
				// not a match, push it
				al.push_back(action);
			}
		}
	}
		
	return true;
}


bool CKeyBindings::UnBind(const string& keystr, const string& command)
{
	CKeySet ks;
	if (!ParseKeySet(keystr, ks)) {
		printf("UnBind: could not parse key: %s\n", keystr.c_str());
		return false;
	}
	bool success = false;

	KeyMap::iterator it = bindings.find(ks);
	if (it != bindings.end()) {
		ActionList& al = it->second;
		success = RemoveCommandFromList(al, command);
		if (al.size() <= 0) {
			bindings.erase(it);
		}
	}
	return success;
}


bool CKeyBindings::UnBindKeyset(const string& keystr)
{
	CKeySet ks;
	if (!ParseKeySet(keystr, ks)) {
		printf("UnBindKeyset: could not parse key: %s\n", keystr.c_str());
		return false;
	}
	bool success = false;

	KeyMap::iterator it = bindings.find(ks);
	if (it != bindings.end()) {
		bindings.erase(it);
		success = true;
	}
	return success;
}


bool CKeyBindings::UnBindAction(const string& command)
{
	bool success = false;

	KeyMap::iterator it = bindings.begin();
	while (it != bindings.end()) {
		ActionList& al = it->second;
		if (RemoveCommandFromList(al, command)) {
			success = true;
		}
		if (al.size() <= 0) {
			KeyMap::iterator it_next = it;
			it_next++;
			bindings.erase(it);
			it = it_next;
		} else {
			it++;
		}
	}
	return success;
}


bool CKeyBindings::SetFakeMetaKey(const string& keystr)
{
	CKeySet ks;
	if (StringToLower(keystr) == "none") {
		fakeMetaKey = -1;
		return true;
	}
	if (!ks.Parse(keystr)) {
		printf("SetFakeMetaKey: could not parse key: %s\n", keystr.c_str());
		return false;
	}
	fakeMetaKey = ks.Key();
	return true;
}

bool CKeyBindings::AddKeySymbol(const string& keysym, const string& code)
{
	CKeySet ks;
	if (!ks.Parse(code)) {
		printf("AddKeySymbol: could not parse key: %s\n", code.c_str());
		return false;
	}
	if (!keyCodes->AddKeySymbol(keysym, ks.Key())) {
		printf("AddKeySymbol: could not add: %s\n", keysym.c_str());
		return false;
	}
	return true;
}


bool CKeyBindings::AddNamedKeySet(const string& name, const string& keystr)
{
	CKeySet ks;
	if (!ks.Parse(keystr)) {
		printf("AddNamedKeySet: could not parse keyset: %s\n", keystr.c_str());
		return false;
	}
	if ((ks.Key() < 0) || !CKeyCodes::IsValidLabel(name)) {
		printf("AddNamedKeySet: bad custom keyset name: %s\n", name.c_str());
		return false;
	}
	namedKeySets[name] = ks;
}


bool CKeyBindings::RemoveCommandFromList(ActionList& al, const string& command)
{
	bool success = false;
	for (int i = 0; i < (int)al.size(); ++i) {
		if (al[i].command == command) {
			success = true;
			for (int j = (i + 1); j < (int)al.size(); ++j) {
				al[j - 1] = al[j];
			}
			al.resize(al.size() - 1);
		}
	}
	return success;
}


bool CKeyBindings::ParseKeySet(const string& keystr, CKeySet& ks) const
{
	if (keystr[0] != NamedKeySetChar) {
		return ks.Parse(keystr);
	}
	else {
		const string keysetName = keystr.substr(1);
		NamedKeySetMap::const_iterator it = namedKeySets.find(keysetName);
		if (it !=  namedKeySets.end()) {
			ks = it->second;
		} else {
			return false;
		}
	}
	return true;
}


/******************************************************************************/

void CKeyBindings::LoadDefaults()
{
	const int count = sizeof(defaultBindings) / sizeof(defaultBindings[0]);
	for (int i = 0; i < count; ++i) {
		Bind(defaultBindings[i].key, defaultBindings[i].action);
	}
}


bool CKeyBindings::Command(const string& line)
{
	vector<string> words = Tokenize(line, 2);

	if (words.size() <= 0) {
		return false;
	}
	const string command = StringToLower(words[0]);
	
	if ((command == "fakemeta") && (words.size() > 1)) {
		if (!SetFakeMetaKey(words[1])) { return false; }
	}
	else if ((command == "keyset") && (words.size() > 2)) {
		if (!AddNamedKeySet(words[1], words[2])) { return false; }
	}
	else if ((command == "keysym") && (words.size() > 2)) {
		if (!AddKeySymbol(words[1], words[2])) { return false; }
	}
	else if ((command == "bind") && (words.size() > 2)) {
		if (!Bind(words[1], words[2])) { return false; }
	}
	else if ((command == "unbind") && (words.size() > 2)) {
		if (!UnBind(words[1], words[2])) { return false; }
	}
	else if ((command == "unbindaction") && (words.size() > 1)) {
		if (!UnBindAction(words[1])) { return false; }
	}
	else if ((command == "unbindkeyset") && (words.size() > 1)) {
		if (!UnBindKeyset(words[1])) { return false; }
	}
	else if (command == "unbindall") {
		bindings.clear();
		keyCodes->Reset();
		namedKeySets.clear();
		Bind("enter", "chat"); // bare minimum
	}
	else {
		return false;
	}
	
	if (userCommand) {
		Sanitize();
	}

	return false;
}


bool CKeyBindings::Load(const string& filename)
{
	inComment = false;
	lineNumber = 0;
	CFileHandler ifs(filename);
	
	userCommand = false; // temporarily disable Sanitize() calls
	
	LoadDefaults();
	
	while (true) {
		const string line = GetCleanLine(ifs);
		if (line.empty()) {
			break;
		}
		Command(line);
	}
	
	Sanitize();
	
	userCommand = true; // re-enable Sanitize() calls

	return true;
}


void CKeyBindings::Sanitize()
{
	// FIXME -- do something extra here?
	//          (seems as though removing the Up states fixed most of the problems)
	BuildHotkeyMap();
}


void CKeyBindings::BuildHotkeyMap()
{
	hotkeys.clear();

	KeyMap::const_iterator kit;
	for (kit = bindings.begin(); kit != bindings.end(); ++kit) {
		const CKeySet ks = kit->first;
		const string keystr = ks.GetString(true);
		const ActionList& al = kit->second;
		for (int i = 0; i < (int)al.size(); ++i) {
			HotkeyList& hl = hotkeys[al[i].command];
			int j;
			for (j = 0; j < (int)hl.size(); ++j) {
				if (hl[j] == keystr) {
					break;
				}
			}
			if (j == (int)hl.size()) {
				hl.push_back(keystr);
			}
		}
	}
}


/******************************************************************************/

void CKeyBindings::Print() const
{
	FileSave(stdout);
}


bool CKeyBindings::Save(const string& filename) const
{
	FILE* out = fopen(filename.c_str(), "wt");
	if (out == NULL) {
		return false;
	}

	const bool success = FileSave(out);

	fclose(out);
	
	return success;
}


bool CKeyBindings::FileSave(FILE* out) const
{
	if (out == NULL) {
		return false;
	}
	
	// clear the defaults
	fprintf(out, "\n");
	fprintf(out, "unbindall          // clear the defaults\n");
	fprintf(out, "unbind enter chat  // clear the defaults\n");
	fprintf(out, "\n");

	// save the user defined key symbols
	keyCodes->SaveUserKeySymbols(out);
	
	// save the fake meta key (if it has been defined)
	if (fakeMetaKey >= 0) {
		fprintf(out, "fakemeta  %s\n\n", keyCodes->GetName(fakeMetaKey).c_str());
	}

	// save the named keysets	
	NamedKeySetMap::const_iterator ks_it;
	for (ks_it = namedKeySets.begin(); ks_it != namedKeySets.end(); ++ks_it) {
		fprintf(out, "keyset  %-15s  %s\n",
		        ks_it->first.c_str(), ks_it->second.GetString(false).c_str());
	}
	if (!namedKeySets.empty()) {
		fprintf(out, "\n");
	}
	
	// save the bindings
	KeyMap::const_iterator it;
	for (it = bindings.begin(); it != bindings.end(); ++it) {
		const ActionList& al = it->second;
		for (int i = 0; i < (int)al.size(); ++i) {
			const Action& action = al[i];
			string comment;
			if (unitDefHandler && (action.command.find("buildunit_") == 0)) {
				const string unitName = action.command.substr(10);
				const UnitDef* unitDef = unitDefHandler->GetUnitByName(unitName);
				if (unitDef) {
					comment = "  // " + unitDef->humanName + " :: " + unitDef->tooltip;
				}
			}
			if (comment.empty()) {
				fprintf(out, "bind %18s  %s\n", 
				        action.boundWith.c_str(),
				        action.rawline.c_str());
			} else {
				fprintf(out, "bind %18s  %-20s%s\n", 
				        action.boundWith.c_str(),
				        action.rawline.c_str(), comment.c_str());
			}
		}
	}
	return true;
}


/******************************************************************************/

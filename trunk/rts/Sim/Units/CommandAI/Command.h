#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <vector>
#include <limits.h> // for INT_MAX
#include "creg/creg_cond.h"

// cmds lower than 0 is reserved for build options (cmd -x = unitdefs[x])
#define CMD_STOP                   0
#define CMD_INSERT                 1
#define CMD_REMOVE                 2
#define CMD_WAIT                   5
#define CMD_TIMEWAIT               6
#define CMD_DEATHWAIT              7
#define CMD_SQUADWAIT              8
#define CMD_GATHERWAIT             9
#define CMD_MOVE                  10
#define CMD_PATROL                15
#define CMD_FIGHT                 16
#define CMD_ATTACK                20
#define CMD_AREA_ATTACK           21
#define CMD_GUARD                 25
#define CMD_AISELECT              30
#define CMD_GROUPSELECT           35
#define CMD_GROUPADD              36
#define CMD_GROUPCLEAR            37
#define CMD_REPAIR                40
#define CMD_FIRE_STATE            45
#define CMD_MOVE_STATE            50
#define CMD_SETBASE               55
#define CMD_INTERNAL              60
#define CMD_SELFD                 65
#define CMD_SET_WANTED_MAX_SPEED  70
#define CMD_LOAD_UNITS            75
#define CMD_LOAD_ONTO             76
#define CMD_UNLOAD_UNITS          80
#define CMD_UNLOAD_UNIT           81
#define CMD_ONOFF                 85
#define CMD_RECLAIM               90
#define CMD_CLOAK                 95
#define CMD_STOCKPILE            100
#define CMD_DGUN                 105
#define CMD_RESTORE              110
#define CMD_REPEAT               115
#define CMD_TRAJECTORY           120
#define CMD_RESURRECT            125
#define CMD_CAPTURE              130
#define CMD_AUTOREPAIRLEVEL      135
#define CMD_LOOPBACKATTACK       140
#define CMD_IDLEMODE             145

#define CMDTYPE_ICON                        0  // expect 0 parameters in return
#define CMDTYPE_ICON_MODE                   5  // expect 1 parameter in return (number selected mode)
#define CMDTYPE_ICON_MAP                   10  // expect 3 parameters in return (mappos)
#define CMDTYPE_ICON_AREA                  11  // expect 4 parameters in return (mappos+radius)
#define CMDTYPE_ICON_UNIT                  12  // expect 1 parameters in return (unitid)
#define CMDTYPE_ICON_UNIT_OR_MAP           13  // expect 1 parameters in return (unitid) or 3 parameters in return (mappos)
#define CMDTYPE_ICON_FRONT                 14  // expect 3 or 6 parameters in return (middle of front and right side of front if a front was defined)
#define CMDTYPE_COMBO_BOX                  15  // expect 1 parameter in return (number selected option)
#define CMDTYPE_ICON_UNIT_OR_AREA          16  // expect 1 parameter in return (unitid) or 4 parameters in return (mappos+radius)
#define CMDTYPE_NEXT                       17  // used with CMD_INTERNAL
#define CMDTYPE_PREV                       18  // used with CMD_INTERNAL
#define CMDTYPE_ICON_UNIT_FEATURE_OR_AREA  19  // expect 1 parameter in return (unitid or featureid+MAX_UNITS (id>MAX_UNITS=feature)) or 4 parameters in return (mappos+radius)
#define CMDTYPE_ICON_BUILDING              20  // expect 3 parameters in return (mappos)
#define CMDTYPE_CUSTOM                     21  // used with CMD_INTERNAL
#define CMDTYPE_ICON_UNIT_OR_RECTANGLE     22  // expect 1 parameter in return (unitid)
                                               //     or 3 parameters in return (mappos)
                                               //     or 6 parameters in return (startpos+endpos)
#define CMDTYPE_NUMBER                     23  // expect 1 parameter in return (number)


// wait codes
#define CMD_WAITCODE_TIMEWAIT    1.0f
#define CMD_WAITCODE_DEATHWAIT   2.0f
#define CMD_WAITCODE_SQUADWAIT   3.0f
#define CMD_WAITCODE_GATHERWAIT  4.0f


// bits for the option field of Command
#define DONT_REPEAT     (1 << 3) //   8
#define RIGHT_MOUSE_KEY (1 << 4) //  16
#define SHIFT_KEY       (1 << 5) //  32
#define CONTROL_KEY     (1 << 6) //  64
#define ALT_KEY         (1 << 7) // 128


#define INTERNAL_ORDER  (DONT_REPEAT)


struct Command
{
	CR_DECLARE_STRUCT(Command);

	Command()
	: options(0), tag(0), timeOut(INT_MAX) {};

	void AddParam(float par) {params.push_back(par);};
	int id;                     // CMD_xxx code  (custom codes can also be used)
	unsigned char options;      // option bits
	std::vector<float> params;  // command parameters

	unsigned int tag;  // unique id within a CCommandQueue

	int timeOut;  // remove this command after this frame
	              // can only be set locally, not sent over net
	              // (used for temporary orders)
};


struct CommandDescription {
	CR_DECLARE_STRUCT(CommandDescription);

	CommandDescription()
	: hidden(false),
	  disabled(false),
	  showUnique(false),
	  onlyTexture(false) {}

	int id;    // CMD_xxx     code  (custom codes can also be used)
	int type;  // CMDTYPE_xxx code 

	std::string name;       // command name
	std::string action;     // the associated command action binding name
	std::string iconname;   // button texture
	std::string mouseicon;  // mouse cursor
	std::string tooltip;    // tooltip text

	bool hidden;       // if true dont show a button for the command
	bool disabled;     // for greying-out commands
	bool showUnique;   // command only applies to single units
	bool onlyTexture;  // do not draw the name if the texture is available

	std::vector<std::string> params;
};


#endif /* COMMAND_H */
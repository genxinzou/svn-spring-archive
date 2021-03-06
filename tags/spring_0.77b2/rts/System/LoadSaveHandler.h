#ifndef LOADSAVEHANDLER_H
#define LOADSAVEHANDLER_H

#include <string>
#include <fstream>

class CLoadInterface;

class CLoadSaveHandler
{
public:
	CLoadSaveHandler(void);
	~CLoadSaveHandler(void);
	void SaveGame(std::string file);
	void LoadGameStartInfo(std::string file); // load things such as map/mod, needed to fire up the engine
	void LoadGame(); 
	std::string FindSaveFile(const char* name);

	std::string scriptText;
	std::string mapName;
	std::string modName;
protected:
	std::ifstream *ifs;
};


#endif /* LOADSAVEHANDLER_H */

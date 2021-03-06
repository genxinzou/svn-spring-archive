#ifndef __END_GAME_BOX_H__
#define __END_GAME_BOX_H__

#include "InputReceiver.h"
#include <list>
#include <vector>

class CEndGameBox :
	public CInputReceiver
{
public:
	CEndGameBox(void);
	~CEndGameBox(void);

	virtual bool MousePress(int x, int y, int button);
	virtual void MouseMove(int x, int y, int dx,int dy, int button);
	virtual void MouseRelease(int x, int y, int button);
	virtual void Draw();
	virtual bool IsAbove(int x, int y);
	virtual std::string GetTooltip(int x,int y);

protected:
	void FillTeamStats();
	ContainerBox box;

	ContainerBox exitBox;

	ContainerBox playerBox;
	ContainerBox sumBox;
	ContainerBox difBox;

	bool moveBox;

	int dispMode;

	int stat1;
	int stat2;

	struct TeamStat {
	};

	struct Stat {
		Stat(std::string s):max(1),maxdif(1),name(s){}

		void AddStat(int team,float value){
			if(value>max)
				max=value;
			if(values[team].size()>0 && fabs(value-values[team].back())>maxdif)
				maxdif=fabs(value-values[team].back());

			values[team].push_back(value);
		}
		std::string name;
		float max;
		float maxdif;

		std::vector<float> values[MAX_TEAMS];
	};

	std::vector<Stat> stats;
	unsigned int graphTex;
};

#endif // __END_GAME_BOX_H__

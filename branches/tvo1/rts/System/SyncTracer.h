#ifndef SYNCTRACER_H
#define SYNCTRACER_H
// SyncTracer.h: interface for the CSyncTracer class.
//
//////////////////////////////////////////////////////////////////////

#define TRACE_SYNC


#include <fstream>
#include <deque>

using namespace std;

class CSyncTracer  
{
	bool init();
public:
	void DeleteInterval();
	void NewInterval();
	void Commit();
	CSyncTracer();
	virtual ~CSyncTracer();

	CSyncTracer& operator<<(const char* c);
	CSyncTracer& operator<<(const int i);
	CSyncTracer& operator<<(const float f);
	CSyncTracer& operator<<(const float3& v);

	ofstream* file;
	ofstream* logfile;
	std::string traces[10];
	int firstActive;
	int nowActive;
};

#ifdef TRACE_SYNC
extern CSyncTracer tracefile;
#endif

#endif /* SYNCTRACER_H */

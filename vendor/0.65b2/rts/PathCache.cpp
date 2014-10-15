#include "stdafx.h"
#include ".\pathcache.h"
#include <algorithm>
#include "infoconsole.h"
//#include "mmgr.h"

using namespace std;

CPathCache::CPathCache(int blocksX,int blocksZ)
: blocksX(blocksX),
	blocksZ(blocksZ)
{
	numCacheHits=0;
	numCacheMisses=0;
}

CPathCache::~CPathCache(void)
{
	info->AddLine("Path cache hits %i %.0f%%",numCacheHits,float(numCacheHits)/float(numCacheHits+numCacheMisses)*100);
	for(std::map<unsigned int,CacheItem*>::iterator ci=cachedPaths.begin();ci!=cachedPaths.end();++ci)
		delete ci->second;
}

void CPathCache::AddPath(IPath::Path* path, IPath::SearchResult result, int2 startBlock,int2 goalBlock,float goalRadius,int pathType)
{
	if(cacheQue.size()>100)
		RemoveFrontQueItem();

	unsigned int hash=((((goalBlock.y)*blocksX+goalBlock.x)*blocksZ+startBlock.y)*blocksX)+startBlock.x*(pathType+1)*max(1.0f,goalRadius);

	if(cachedPaths.find(hash)!=cachedPaths.end()){
		return;		
	}

	CacheItem* ci=new CacheItem;
	ci->path=*path;
	ci->result=result;
	ci->startBlock=startBlock;
	ci->goalBlock=goalBlock;
	ci->goalRadius=goalRadius;
	ci->pathType=pathType;

	cachedPaths[hash]=ci;

	CacheQue cq;
	cq.hash=hash;
	cq.timeout=gs->frameNum+200;

	cacheQue.push_back(cq);
}

CPathCache::CacheItem* CPathCache::GetCachedPath(int2 startBlock,int2 goalBlock,float goalRadius,int pathType)
{
	unsigned int hash=((((goalBlock.y)*blocksX+goalBlock.x)*blocksZ+startBlock.y)*blocksX)+startBlock.x*(pathType+1)*max(1.0f,goalRadius);

	std::map<unsigned int,CacheItem*>::iterator ci=cachedPaths.find(hash);
	if(ci!=cachedPaths.end() && ci->second->startBlock.x==startBlock.x && ci->second->startBlock.y==startBlock.y && ci->second->goalBlock.x==goalBlock.x && ci->second->pathType==pathType){
		++numCacheHits;
		return ci->second;
	}
	++numCacheMisses;
	return 0;
}

void CPathCache::Update(void)
{
	while(!cacheQue.empty() && cacheQue.front().timeout<gs->frameNum)
		RemoveFrontQueItem();
}

void CPathCache::RemoveFrontQueItem(void)
{
	delete cachedPaths[cacheQue.front().hash];
	cachedPaths.erase(cacheQue.front().hash);
	cacheQue.pop_front();
}
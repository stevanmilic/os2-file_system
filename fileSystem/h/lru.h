//File: lru.h
#ifndef _lru_h_
#define _lru_h_
#include "cacheblock.h"
#include "part.h"
#include <windows.h>

class LRU{
	ClusterNo cacheSize = 16;
	CacheBlock **cbs;
	CacheBlock *firstAccessed = nullptr, *lastAccessed = nullptr;
	ClusterNo victim = 0;
	Partition *part;
	CRITICAL_SECTION csBlock;

public:
	LRU(CacheBlock**,Partition*);
	~LRU();

	//TO DO: write copy and move constructor

	void loadPage(ClusterNo);
	CacheBlock* hitPage(ClusterNo, char);
};
#endif

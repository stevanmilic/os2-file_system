//File: cache.h
#ifndef _cache_h_
#define _cache_h_
#include "hashtable.h"
#include "part.h"
#include "lru.h"
#include "fs.h"

class Cache{
//TO DO: implementation:
//Writing and Reading could be done(delayed) in sepate thread
//size of cache depends on size of file, formula?
	
	Partition *part;
	CacheBlock** cbs;
	LRU* partLRU;
	HashTable<LRU*,EntryNum> lt;
	Directory dir;
	char* bitVector;

	ClusterNo bvSize();
public:
	Cache(Partition*);
	~Cache();

	Directory* getDir(){
		return &dir;
	}

	char* getBitVector(){
		return bitVector;
	}

	char* getBlock(LRU*,ClusterNo,char,EntryNum=0);
	char* cacheBlock(EntryNum,ClusterNo,char);
	char* partBlock(ClusterNo,char);
	void clearBitVector();
	void clearDir();
	void newFileCache(EntryNum);
	void closeFileCache(EntryNum);
	ClusterNo findFreeBlock();
};
#endif

//File: cache.h
#ifndef _cache_h_
#define _cache_h_
#include "hashtable.h"
#include "part.h"
#include "lru.h"
#include "fs.h"

typedef unsigned long ID;
class Cache{
//TO DO: implementation:
//Writing and Reading could be done(delayed) in sepate thread
//size of cache depends on size of file, formula?
	
	Partition *part;
	CacheBlock** cbs;
	LRU* partLRU;
	HashTable<LRU*,ID> lt;
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

	bool readBlock(char*,ID,ClusterNo);
	bool writeBlock(char*,ID,ClusterNo);
	void readWriteBitVector();
	void readWriteDir(char);
	void clearBitVector();
	void clearDir();
	void newFileCache(ID);
	void closeFileCache(ID);
	ClusterNo findFreeBlock();
};
#endif

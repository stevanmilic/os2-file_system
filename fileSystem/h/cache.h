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
	HashTable<LRU*> lt;
	Directory dir;
	char* bitVector;

	ClusterNo bvSize();
public:
	Cache(Partition*);

	Directory* getDir(){
		return &dir;
	}

	char* getBitVector(){
		return bitVector;
	}

	bool readBlock(char*,ID,ClusterNo);
	bool writeBlock(char*,ID,ClusterNo);
	void readBitVector();
	void readDir();
	void clearBitVector();
	void clearDir();
	void newFileCache(ID);
	void closeFileCache(ID);
	ClusterNo findFreeBlock();
};
#endif

//File: cache.h
#ifndef _cache_h_
#define _cache_h_
#include "hashtable.h"
#include "part.h"
#include "lru.h"


typedef unsigned long ID;
class Cache{
//TO DO: implementation:
//LRU cache!
//Communicates with partition for writing and reading clusters
//Cache gets free clusters from partition
//Has data and index clusters, every partion has cache
//Has bitvector
//Cache holds valuable data in program memory, so it's easy to access clusters
//Writing and Reading could be done(delayed) in sepate thread
//size of cache depends on size of file, formula?
	
	HashTable<LRU*> lt(64);
	CacheBlock** cbs;
	Directory dir;
	char* bitVector;
	ClusterNo numOfBlocks;
	Partition *part;

	ClusterNo bvSize();
public:
	Cache(Partition*);

	Directory getDir(){
		return dir;
	}

	char* getBitVector(){
		return bitVector;
	}

	void readBlock(char*,ID,ClusterNo);
	void writeBlock(char*,ID,ClusterNo);
	void readBitVector();
	void readDir(Directory)
	void clearBitVector();
	void clearDir();
	void newFileCache(ID);
	void closeFileCache(ID);
	ClusterNo findFreeBlock();
};
#endif

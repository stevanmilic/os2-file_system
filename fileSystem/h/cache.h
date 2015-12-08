//File: cache.h
#ifndef _cache_h_
#define _cache_h_
#include "part.h"
#include "hashtable.h"
#include "lru.h"

struct tagMem{
	ClusterNo index;
}

struct dataMem{
	char* data;
}

struct set{
	tagMem tm;
	dataMem dm;
	char valid;
	char dirty;
};

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
	Directory dir;

	Partition *part;
	char* bitVector;
	ClusterNo dirIndex;

	ClusterNo bvClustersNum(){
public:
	Cache(Partition *part);

	Directory* getDir(){
		return &dir;
	}

	bool readFromCluster(char*,ID,ClusterNo);
	bool writeToCluster(char*,ID,ClusterNo);
	ClusterNo nextCluster();
	void clearBitVector();
	void clearDir();
	void newFileCache(ID);
};
#endif

//File: cache.cpp
#include "cache.h"

Cache::Cache(Partition *part){
	this->part = part;
	bitVector = new char[bvSize()];
}

ClusterNo Cache::bvSize(){
	ClusterNo partSize = part->getNumOfClusters();
	ClusterNo bvSize = ClusterSize*8; 
	return ClusterSize*(partSize/bvSize + partSize % bvSize == partSize ? 0 : 1);
}

void Cache::newFileCache(ID fcbid){
	LRU* lru = new LRU(10);
	lt.insertKey(fcbid,lru);
}





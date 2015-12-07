//File: cache.h
#ifndef _cache_h_
#define _cache_h_
#include "part.h"

class Cache{
//TO DO: implementation:
//Communicates with partition for writing and reading clusters
//Cache gets free clusters from partition
//Has data and index clusters, every partion has cache
//Has bitvector
//Cache holds valuable data in program memory, so it's easy to access clusters
//Writing and Reading could be done(delayed) in sepate thread
//size of cache depends on size of file, formula?

Partition *part;
ClusterNo dirIndex;

public:
	Cache(Partition *part){
		this->part = part;
	}

	ClusterNo bvClustersNum(){
		return part->getNumOfClusters()/ClusterSize + part->getNumOfClusters() % ClusterSize ? 1 : 0;//if number of clusters exceeds clustersize
	}

	ClusterNo getDir(){
		return dirIndex;
	}
};
#endif

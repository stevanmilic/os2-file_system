#ifndef _partinter_h_
#define _partinter_h_
#include "part.h"

class PartInterface{
	Partition* part;
	ClusterNo dirIndex;
public:
	PartInterface(Partition* part){
		this->part = part;
	}
	
	void setDir(ClusterNo dirIndex){
		this->dirIndex = dirIndex;
	}

	Partition* getPart(){
		return part;
	}

	ClusterNo getDir(){
		return dirIndex;
	}

	ClusterNo bvClustersNum(){
		return part->getNumOfClusters()/ClusterSize + part->getNumOfClusters() % ClusterSize ? 1 : 0;//if number of clusters exceeds clustersize
	}

	static char toNumber(char partName){
		return partName - 'A';
	}
};
#endif

//File: partwrapper.h
#ifndef _partwrapper_h_
#define _partwrapper_h_
#include "part.h"
#include "cache.h"
//TO DO:
//use cache wisely :)

class PartWrapper{
	Cache* cache;
	Partition *part;
	
	static char posID;//default 0
	char id = posID++;
	char name = 'A' + id;
public:
	PartWrapper(Partition* part){
		this->part = part;
		cache = new Cache(part);
	}
	
	char getID(){
		return id;
	}

	char getName(){
		return name;
	}

	static char toNumber(char partName){
		return partName - 'A';
	}

	//get partition name from fpath
	static char parseName(char *fpath){
		if(fpath == nullptr)
			return '0';
		return fpath[0] - 'A';
	}

	void clear(){
		cache->clearBitVector();
		cache->clearDir();
		//TO DO :cache->clearCacheBlocks();
	}

	Directory* rootDir(){
		return cache->getDir();
	}

	void raed(char* buffer, ID fcbID, ClusterNo readCluster){
		cache->readBlock(buffer,fcbID,readCluster);	
	}

	ClusterNo write(char* buffer,ID fcbID){
		ClusterNo writtenCluster = cache->findFreeBlock();
		cache->writeBlock(buffer,fcbID,writtenCluster);
		return writtenCluster;
	}

	ClusterNo cluster(){
		return cache->findFreeBlock();
	}

	void fopen(ID fcbID){
		cache->newFileCache(fcbID);
	}
};
#endif

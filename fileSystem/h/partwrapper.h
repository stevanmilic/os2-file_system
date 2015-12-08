//File: partwrapper.h
#ifndef _partwrapper_h_
#define _partwrapper_h_
#include "cache.h"
//TO DO:
//use cache wisely :)

class PartWrapper{
	Cache* cache;
	
	static char posID;//default 0
	char id = posID++;
	char name = 'A' + id;
public:
	PartWrapper(Cache* cache){
		this->cache = cache;
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
	}

	Directory* rootDir(){
		return cache->getDir();
	}

	bool raed(char* buffer){
		return cache->readFromCluster(buffer);
	}

	bool write(char* buffer){
		return cache->writeToCluster(buffer);
	}

	ClusterNo cluster(){
		return cache->nextCluster();
	}

	void fopen(unsigned long fcbid){
		cache->newFileCache(fcbid);
	}
};
#endif

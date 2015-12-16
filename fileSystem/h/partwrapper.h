//File: partwrapper.h
#ifndef _partwrapper_h_
#define _partwrapper_h_
#include "part.h"
#include "cache.h"

class PartWrapper{
	Cache* cache;
	Partition *part;
	
	static char posID;//default 0
	char id = posID++;
	char name = 'A' + id;
	char format = 0;
public:
	PartWrapper(Partition*);
	~PartWrapper();	

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

	void clear();
	Directory* rootDir();
	void read(char*,EntryNum,ClusterNo);
	ClusterNo write(char*,EntryNum);
	ClusterNo cluster();
	void fopen(EntryNum);
	void fclose(EntryNum);
	bool getFormat();
	void setStartCluster(EntryNum,ClusterNo);
	ClusterNo getStartCluster(EntryNum);
};
#endif

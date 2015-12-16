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
	void read(void*,BytesCnt,BytesCnt,EntryNum,ClusterNo);
	ClusterNo write(void*,BytesCnt,BytesCnt, EntryNum,ClusterNo = 0);
	ClusterNo cluster();
	void fopen(EntryNum);
	void fclose(EntryNum);
	bool getFormat();
	ClusterNo getStartCluster(EntryNum);
	BytesCnt getFileSize(EntryNum);
	void setFileSize(EntryNum,BytesCnt size);
};
#endif

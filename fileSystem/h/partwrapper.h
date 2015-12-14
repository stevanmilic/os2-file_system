//File: partwrapper.h
#ifndef _partwrapper_h_
#define _partwrapper_h_
#include "part.h"
#include "cache.h"
#include "rw.h"
//TO DO:
//use cache wisely :)

class PartWrapper{
	Cache* cache;
	Partition *part;
	HashTable<ReadersWriters*,EntryNum> rwt;
	CRITICAL_SECTION csPart;
	
	static char posID;//default 0
	char id = posID++;
	char name = 'A' + id;
	char format = 0;
public:
	PartWrapper(Partition* part) : rwt(64){
		this->part = part;
		cache = new Cache(part);
		InitializeCriticalSection(&csPart);
	}

	~PartWrapper(){
		EnterCriticalSection(&csPart);
		LeaveCriticalSection(&csPart);
		DeleteCriticalSection(&csPart);
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

	void clear();
	Directory* rootDir();
	void raed(char*,ID,ClusterNo);
	ClusterNo write(char*,ID);
	ClusterNo cluster();
	void fopen(ID fcbID);
	void fclose(ID fcbID);
	bool getFormat();
	void startReading(EntryNum);
	void stopReading(EntryNum);
	void startWriting(EntryNum);
	void stopWriting(EntryNum);
};
#endif

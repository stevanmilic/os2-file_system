//File: partwrapper.h
#ifndef _partwrapper_h_
#define _partwrapper_h_
#include "part.h"
//TO DO:
//use cache wisely :)

class PartWrapper{
	Partition* part;
	Cache *cache;
	
	static char pos_id;//default 0
	char id = posID++;
	char name = 'A' + id;
public:
	PartWrapper(Partition* part){
		this->part = part;
	}
	
	Partition* getPart(){
		return part;
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

	static char getPartName(char *fpath){
		if(fpath == nullptr)
			return '0';
		return fpath[0];
	}
};
#endif

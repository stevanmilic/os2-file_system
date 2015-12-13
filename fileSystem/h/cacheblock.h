//File: cacheblock.h
#ifndef _cacheblock_h_
#define _cacheblock_h_
#include "part.h"
#include <cstring>

class LRU;

class CacheBlock{
	friend class LRU;
	Partition *part;
	char* data;
	char dirty;
	char valid;
	ClusterNo blockNo;
	CacheBlock *prev,*next;

	void writeToPartition(){
		//TO DO:write in a new thread
		part->writeCluster(blockNo,data);
	}

	void deleteData(){
		delete [] data;
	}

	void initData(){
		data = new char[ClusterSize];
		//TO DO:read in a new thread
		part->readCluster(this->blockNo = blockNo,data);
	}

	CacheBlock(ClusterNo blockNo,Partition *part){
		this->part = part;
		initData();
	}
public:
	~CacheBlock(){
		if(dirty)
			writeToPartition();
		deleteData();
	}

	//write copy and move constructor

	void setData(char* buffer){
		std::memcpy(data,buffer,ClusterSize);
	}

	void readData(char* buffer){
		std::memcpy(buffer, data, ClusterSize);
	}

	char* getData(){
		return data;
	}
};
#endif

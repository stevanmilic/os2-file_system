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

	void initData(ClusterNo blockNo){
		data = new char[ClusterSize]();
		//TO DO:read in a new thread
		part->readCluster(this->blockNo = blockNo,data);
	}

	CacheBlock(ClusterNo blockNo,Partition *part){
		this->part = part;
		initData(blockNo);
	}
public:
	~CacheBlock(){
		if(dirty)
			writeToPartition();
		deleteData();
	}

	//write copy and move constructor

	void setData(void* buffer,unsigned long size){
		std::memcpy(data,buffer,size);
	}

	void readData(void* buffer,unsigned long size){
		std::memcpy(buffer, data, size);
	}

	char* getData(){
		return data;
	}
};
#endif

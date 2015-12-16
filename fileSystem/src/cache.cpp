//File: cache.cpp
#include "cache.h"

Cache::Cache(Partition* part) : lt(ENTRYCNT){
	this->part = part;
	cbs = new CacheBlock*[part->getNumOfClusters()];
	partLRU = new LRU(cbs,part);
	readWriteBitVector();
	readWriteDir(0);//read dir from part
}

Cache::~Cache(){
	readWriteDir(1);//write dir to part
	delete partLRU;
	//hashtable deletes all file cache lru's, and writes cache blocks to part
}

/*ClusterNo Cache::bvSize(){
	ClusterNo bvSize = ClusterSize*8; 
	return partSize/bvSize + partSize % bvSize == partSize ? 0 : 1;
}*/

void Cache::newFileCache(EntryNum entry){
	LRU *lru = lt.findKey(entry);
	if(lru == 0)
		lru = new LRU(cbs,part);
	lt.insertKey(entry,lru);
}

ClusterNo Cache::findFreeBlock(){
	//TO DO: write better search algortithm!
	ClusterNo blockNo;
	for(ClusterNo i = 0;i <= part->getNumOfClusters(); i++)
		if(bitVector[i] == 0){
			blockNo = i;
			bitVector[i] = 1;//project specification says otherwise? :)
		}
	return blockNo;
}

bool Cache::writeBlock(char* buffer, EntryNum entry, ClusterNo blockNo){
	LRU* lru = lt.findKey(entry);
	if(lru == 0)
		return 0;//file not found?

	CacheBlock* block = lru->hitPage(blockNo,1);

	if(block == nullptr){
		lru->loadPage(blockNo);
		block = lru->hitPage(blockNo,1);
	}
		
	block->setData(buffer);
	return 1;
}

bool Cache::readBlock(char* buffer, EntryNum entry, ClusterNo blockNo){
	LRU *lru = lt.findKey(entry);
	if(lru == 0)
		return 0;//file not found?


	CacheBlock* block = lru->hitPage(blockNo,0);
	if(block == nullptr){
		lru->loadPage(blockNo);
		block = lru->hitPage(blockNo,0);
	}
	
	block->readData(buffer);
	return 1;
}

void Cache::clearBitVector(){
	memset(bitVector,0,ClusterSize);
}

void Cache::clearDir(){
	memset(dir,0,ClusterSize);
}

void Cache::readWriteBitVector(){
	ClusterNo blockNo = 0;//bitVector position
	CacheBlock* block = partLRU->hitPage(blockNo,1);
	if(block == nullptr){
		partLRU->loadPage(blockNo);
		block = partLRU->hitPage(blockNo,1);
	}
	bitVector = block->getData();
}

void Cache::readWriteDir(char write){
	ClusterNo blockNo = 1;//dir position
	CacheBlock* block = partLRU->hitPage(blockNo,write);
	if(block == nullptr){
		partLRU->loadPage(blockNo);
		block = partLRU->hitPage(blockNo,write);
	}

	if(!write)
		memcpy(dir,block->getData(),ClusterSize);
	else
		memcpy(block->getData(),dir,ClusterSize);
}

void Cache::closeFileCache(EntryNum entry){
	lt.deleteKey(entry);
}

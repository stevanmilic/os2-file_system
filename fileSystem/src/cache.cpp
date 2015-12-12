//File: cache.cpp
#include "cache.h"

Cache::Cache(Partition* part){
	this->part = part;
	cbs = new (CacheBlock*)[part->numOfBlocks() = numOfBlocks];
	readBitVector();
	readDir();
}

/*ClusterNo Cache::bvSize(){
	ClusterNo bvSize = ClusterSize*8; 
	return partSize/bvSize + partSize % bvSize == partSize ? 0 : 1;
}*/

void Cache::newFileCache(ID fcbid){
	LRU* lru = new LRU(cbs,part);
	lt.insertKey(fcbid,lru);
}

ClusterNo Cache::findFreeBlock(){
	//TO DO: write better search algortithm!
	ClusterNo blockNo;
	for(ClusterNo i = 0;i <= numOfBlocks; i++)
		if(bitVector[i] == 0){
			blockNo = i;
			bitVector[i] = 1;//project specification says otherwise? :)
		}
	return blockNo;
}

void Cache::writeBlock(char* buffer, ID fcbID, ClusterNo blockNo){
	LRU* lru = lt.findKey(fcbID);
	if(lru == 0)
		return -1;//file not found?

	CacheBlock* block = lru->hitPage(blockNo);

	if(block == nullptr){
		lru->loadPage(blockNo);
		block = lru->hitPage(blockNo,1);
	}
		
	block->setData(buffer);
}

void Cache::readBlock(char* buffer, ID fcbID, ClusterNo blockNo){
	LRU *lru = lt.findKey(fcbID);
	if(lru == 0)
		return -1;//file not found?


	CacheBlock* block = lru->hitPage(blockNo,0);
	if(block == nullptr){
		lru->loadPage(blockNo);
		block = lru->hitPage(blockNo,0);
	}
	
	block->readData(buffer);
}

void Cache::clearBitVector(){
	memset(bitVector,0,sizeof bitVector);
}

void Cache::clearDir(){
	memset(dir,0,sizeof dir);
}

void Cache::readBitVector(){
	Cluster blockNo = 0;//bitVector position
	CacheBlock* block = lru->hitPage(blockNo,0);
	if(block == nullptr){
		lru->loadPage(blockNo);
		block = lru->hitPage(blockNo,0);
	}

	bitVector = block->getData();
}

void Cache::readDir(){
	Cluster blockNo = 1;//bitVector position
	CacheBlock* block = lru->hitPage(blockNo,0);
	if(block == nullptr){
		lru->loadPage(blockNo);
		block = lru->hitPage(blockNo,0);
	}

	//TO DO: char to dir(ENTRY[64])
	//dir = block->getData();
}

void Cache::closeFileCache(ID fcbID){
	lt.deleteKey(fcbID);
}

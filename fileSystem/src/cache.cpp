//File: cache.cpp
#include "cache.h"

Cache::Cache(Partition* part) : lt(ENTRYCNT){
	this->part = part;
	cbs = new CacheBlock*[part->getNumOfClusters()];
	partLRU = new LRU(cbs,part);
	bitVector = partBlock(0,1);//read blockNo = 1, and make it dirty
	memcpy(dir,partBlock(1,0),sizeof dir);
	readWriteDir(0);//read dir from part
}

Cache::~Cache(){
	memcpy(dir,partBlock(1,1),sizeof dir);
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
	for(ClusterNo i = 0;i <= part->getNumOfClusters(); i++)
		if(bitVector[i] == 0){
			bitVector[i] = 1;//project specification says otherwise? :)
			return i;
			
		}
	return 0;
}


char* Cache:: partBlock(ClusterNo blockNo,char write){
	return getBlock(partLRU,blockNo,write);
}

char* Cache::cacheBlock(EntryNum entry, ClusterNo blockNo, char write){
	LRU* lru = lt.findKey(entry);
	if(lru == 0)
		return 0;//file not found?
	return getBlock(lru,blockNo,write,entry);
}

char* Cache::getBlock(LRU *lru,ClusterNo blockNo, char write, EntryNum entry){
	CacheBlock* block = lru->hitPage(blockNo,write);

	if(block == nullptr){
		lru->loadPage(blockNo);
		block = lru->hitPage(blockNo,write);
	}
		
	return block->getData();
}

void Cache::clearBitVector(){
	memset(bitVector,0,ClusterSize);
}

void Cache::clearDir(){
	memset(dir,0,ClusterSize);
}

void Cache::closeFileCache(EntryNum entry){
	lt.deleteKey(entry);
}

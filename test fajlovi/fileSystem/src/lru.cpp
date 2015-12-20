#include "lru.h"

LRU::LRU(CacheBlock **cbs, Partition *part){
	this->cbs = cbs;
	this->part = part;
}

LRU::~LRU(){
	while(firstAccessed != nullptr){
		CacheBlock* old = firstAccessed;
		firstAccessed = firstAccessed->next;
		ClusterNo oldBlock = old->blockNo;
		delete old;
		cbs[oldBlock] = nullptr;
	}
}

void LRU::loadPage(ClusterNo blockNo){
	if(victim < cacheSize)
		victim++;
	else{
		if (lastAccessed == firstAccessed)
			firstAccessed = nullptr;
		lastAccessed->valid = 0;
		if(lastAccessed->dirty){
			//write block to partition
			lastAccessed->writeToPartition();
			lastAccessed->dirty = 0;
		}
		lastAccessed->deleteData();
		
		lastAccessed = lastAccessed->prev;
		if(lastAccessed)
			lastAccessed->next = nullptr;
	}

	//get current block from partition(GETNODE)
	if(cbs[blockNo] == nullptr)
		cbs[blockNo] = new CacheBlock(blockNo,part);
	else
		cbs[blockNo]->initData(blockNo);

	cbs[blockNo]->next = firstAccessed;

	// If queue is empty, change both front and rear pointers
	if (lastAccessed == nullptr)
		firstAccessed = lastAccessed = cbs[blockNo];
	else  // Else change the front
	{
		firstAccessed->prev = cbs[blockNo];
		firstAccessed = cbs[blockNo];
	}
	firstAccessed->dirty = 0;
	firstAccessed->valid = 1;
}

CacheBlock* LRU::hitPage(ClusterNo blockNo, char write){
	if(cbs[blockNo] != nullptr && cbs[blockNo]->valid){
		if(write)
			cbs[blockNo]->dirty = 1;
		if(firstAccessed != cbs[blockNo]){
			if (cbs[blockNo]->prev == nullptr) return nullptr;
			cbs[blockNo]->prev->next = cbs[blockNo]->next;
			if (cbs[blockNo]->next)
				cbs[blockNo]->next->prev = cbs[blockNo]->prev;
			if (lastAccessed == cbs[blockNo]) {
				lastAccessed = cbs[blockNo]->prev;
				lastAccessed->next = nullptr;
			}
			cbs[blockNo]->next = firstAccessed;
			cbs[blockNo]->prev = nullptr;
			cbs[blockNo]->next->prev = cbs[blockNo];
			firstAccessed = cbs[blockNo];
		}
		return cbs[blockNo];
	}
	return nullptr;

}

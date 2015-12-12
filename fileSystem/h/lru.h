//File: lru.h
#ifndef _lru_h_
#define _lru_h_
#include "cacheblock.h"
#include "part.h"

class LRU{
	ClusterNo cacheSize = 10;
	CacheBlock **cbs;
	CacheBlock *firstAccessed = nullptr, *lastAccessed = nullptr;
	ClusterNo victim = 0;
	Partition *part;

public:
	LRU(CacheBlock **cbs,Partition *part)
		this->cbs = cbs;
		this->part = part;
	}

	//TO DO: write copy and move constructor, and destructor

	void loadPage(ClusterNo blockNo){
		if(victim < cacheSize)
			victim++;
		else{
			lastAccessed->valid = 0;
			if(lastAccessed->dirty){
				//write block to partition
				lastAccessed->writeToPartition();
				lastAccessed->dirty = 0;
			}
			lastAccessed->deleteData();
			lastAccessed = lastAccessed->prev;
				
			lastAccessed->next = nullptr;
		}

		//get current block from partition
		if(cbs[blockNo] == nullptr)
			cbs[blockNo] = new CacheBlock(blockNo,part);
		else
			cbs[blockNo]->initData();

		cbs[blockNo]->next = firstAccessed;
		firstAccessed->prev = cbs[blockNo];
		firstAccessed = firstAccessed->prev;
		firstAccessed->prev = nullptr;
		firstAccessed->dirty = 0;
		firstAccessed->valid = 1;
	}


	CacheBlock* hitPage(ClusterNo blockNo, char write){
		if(cbs[blockNo]->valid){
			if(write)
				cbs[blockNo]->dirty = 1;
			if(firstAccessed != cbs[blockNo]){
				if(cbs[blockNo]->next)
					cbs[blockNo]->next->prev = cbs[blockNo]->prev;
				else
					lastAccessed = lastAccessed->prev;
				cbs[blockNo]->prev->next = cbs[blockNo]->next;
				firstAccessed->prev = cbs[blockNo];
				cbs[blockNo]->next = firstAccessed;
				firstAccessed = firstAccessed->prev;
				firstAccessed->prev = nullptr;
			}
			return cbs[blockNo];
		}
		return nullptr;
	}
};
#endif

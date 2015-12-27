//File: cache.cpp
#include "cache.h"

Cache::Cache(Partition* part) : lt(ENTRYCNT) {
	this->part = part;
	cbs = new CacheBlock*[part->getNumOfClusters()]();
	partLRU = new LRU(cbs, part);
	memcpy(dir, partBlock(1, 0), sizeof dir);
	bitVector = new BitVector(partBlock(0, 1));
	(*bitVector)[0] = true;
	(*bitVector)[1] = true;
}

Cache::~Cache() {
	memcpy(partBlock(1, 1), dir, sizeof dir);
	lt.deleteTable();
	delete partLRU;// crashes on delete buffer, when using 2mb input?!
	delete[] cbs;
}

void Cache::newFileCache(EntryNum entry) {
	LRU* lru = lt.findKey(entry);
	if (lru == 0) {
		LRU* lru = new LRU(cbs, part);
		lt.insertKey(entry, lru);
	}
}

ClusterNo Cache::findFreeBlock() {
	//TO DO: write better search algortithm!
	for (ClusterNo i = 0; i < part->getNumOfClusters(); i++)
		if ((*bitVector)[i] == 0) {
			(*bitVector)[i] = 1;//project specification says otherwise? :)
			return i;
		}
	return 0;
}

void Cache::clearBlock(ClusterNo blockNo) {
	(*bitVector)[blockNo] = 0;
}

char* Cache::partBlock(ClusterNo blockNo, char write) {
	return getBlock(partLRU, blockNo, write);
}

char* Cache::cacheBlock(EntryNum entry, ClusterNo blockNo, char write) {
	LRU* lru = lt.findKey(entry);
	if (lru == 0)
		return 0;//file not found?
	return getBlock(lru, blockNo, write, entry);
}

char* Cache::getBlock(LRU *lru, ClusterNo blockNo, char write, EntryNum entry) {
	CacheBlock* block = lru->hitPage(blockNo, write);

	if (block == nullptr) {
		lru->loadPage(blockNo);
		block = lru->hitPage(blockNo, write);
	}

	return block->getData();
}

void Cache::clearBitVector() {
	bitVector->clearBitVector();
	(*bitVector)[0] = 1;
	(*bitVector)[1] = 1;
}

void Cache::clearDir() {
	memset(dir, 0, sizeof dir);
}

void Cache::closeFileCache(EntryNum entry) {
	lt.deleteKey(entry);
}

bool Cache::hasData() {
	if (lt.fillRatio() > 0.0)
		return true;
	return false;
}

//Fiel: partwrapper.cpp
#include "partwrapper.h"

char PartWrapper::posID = 0;

PartWrapper::PartWrapper(Partition* part){
	this->part = part;
	cache = new Cache(part);
	InitializeCriticalSection(&csPart);
}

PartWrapper::~PartWrapper(){
	unmount = true;
	EnterCriticalSection(&csPart);
	delete cache;
	LeaveCriticalSection(&csPart);

	DeleteCriticalSection(&csPart);
}
void PartWrapper::clear(){
	if (!unmount) {
		format = true;
		EnterCriticalSection(&csPart);
		cache->clearBitVector();
		cache->clearDir();
		//TO DO :cache->clearCacheBlocks();
		LeaveCriticalSection(&csPart);
		format = false;
	}
}

Directory& PartWrapper::rootDir(){
	return cache->getDir();
}

void PartWrapper::read(void *buffer,BytesCnt buffSize,BytesCnt offset, EntryNum entry, ClusterNo readCluster){
	std::memcpy(buffer,cache->cacheBlock(entry,readCluster,0) + offset,buffSize);
}

ClusterNo PartWrapper::write(void* buffer,BytesCnt buffSize,BytesCnt offset, EntryNum entry,ClusterNo& writeCluster){
	if(writeCluster == 0)
		writeCluster = cache->findFreeBlock();
	std::memcpy(cache->cacheBlock(entry,writeCluster,1) + offset,buffer,buffSize);
	return writeCluster;
}

ClusterNo PartWrapper::cluster(){
	return cache->findFreeBlock();
}

void PartWrapper::fopen(EntryNum entry){
	cache->newFileCache(entry);
	if (!cacheUsed && cache->hasData()) {
		cacheUsed = true;
		EnterCriticalSection(&csPart);
	}
}

void PartWrapper::fclose(EntryNum entry){
	cache->closeFileCache(entry);
	if (cacheUsed && !cache->hasData()) {
		LeaveCriticalSection(&csPart);
		cacheUsed = false;
	}
}

bool PartWrapper::getFormat(){
	return format;
}

ClusterNo PartWrapper::getStartCluster(EntryNum entry){
	Directory& dir = rootDir();
	return dir[entry].indexCluster;
}

BytesCnt PartWrapper::getFileSize(EntryNum entry){
	Directory& dir = rootDir();
	return dir[entry].size;
}

void PartWrapper::setFileSize(EntryNum entry,BytesCnt size){
	Directory& dir = rootDir();
	dir[entry].size = size;
}

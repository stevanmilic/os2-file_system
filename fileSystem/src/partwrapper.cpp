//Fiel: partwrapper.cpp
#include "partwrapper.h"

char PartWrapper::posID = 0;

PartWrapper::PartWrapper(Partition* part){
	this->part = part;
	cache = new Cache(part);
}

PartWrapper::~PartWrapper(){
	delete cache;
}
void PartWrapper::clear(){
	format = 1;
	cache->clearBitVector();
	cache->clearDir();
	//TO DO :cache->clearCacheBlocks();
	format = 0;
}

Directory* PartWrapper::rootDir(){
	return cache->getDir();
}

void PartWrapper::read(void *buffer,BytesCnt buffSize,BytesCnt offset, EntryNum entry, ClusterNo readCluster){
	std::memcpy(buffer,cache->cacheBlock(entry,readCluster,0) + offset,buffSize);
}

ClusterNo PartWrapper::write(void* buffer,BytesCnt buffSize,BytesCnt offset, EntryNum entry,ClusterNo writeCluster){
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
}

void PartWrapper::fclose(EntryNum entry){
	cache->closeFileCache(entry);
}

bool PartWrapper::getFormat(){
	return format;
}

ClusterNo PartWrapper::getStartCluster(EntryNum entry){
	Directory *dir = rootDir();
	return dir[entry]->indexCluster;
}

BytesCnt PartWrapper::getFileSize(EntryNum entry){
	Directory *dir = rootDir();
	return dir[entry]->size;
}

void PartWrapper::setFileSize(EntryNum entry,BytesCnt size){
	Directory *dir = rootDir();
	dir[entry]->size = size;
}

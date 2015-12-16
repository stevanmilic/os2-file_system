//Fiel: partwrapper.cpp
#include "partwrapper.h"

PartWrapper::PartWrapper(Partition* part){
	this->part = part;
	cache = new Cache(part);
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

void PartWrapper::read(char* buffer, EntryNum entry, ClusterNo readCluster){
	cache->readBlock(buffer,entry,readCluster);	
}

ClusterNo PartWrapper::write(char* buffer,EntryNum entry){
	ClusterNo writtenCluster = cache->findFreeBlock();
	cache->writeBlock(buffer,entry,writtenCluster);
	return writtenCluster;
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

void PartWrapper::setStartCluster(EntryNum entry, ClusterNo startIndex){
	Directory *dir = rootDir();
	dir[entry]->indexCluster = startIndex;
}

ClusterNo PartWrapper::getStartCluster(EntryNum entry){
	Directory *dir = rootDir();
	return dir[entry]->indexCluster;
}

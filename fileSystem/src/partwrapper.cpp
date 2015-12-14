//Fiel: partwrapper.cpp
#include "partwrapper.h"

void PartWrapper::clear(){
	format = 1;
	EnterCriticalSection(&csPart);
	cache->clearBitVector();
	cache->clearDir();
	//TO DO :cache->clearCacheBlocks();
	LeaveCriticalSection(&csPart);
	format = 0;
}

Directory* PartWrapper::rootDir(){
	return cache->getDir();
}

void PartWrapper::raed(char* buffer, ID fcbID, ClusterNo readCluster){
	cache->readBlock(buffer,fcbID,readCluster);	
}

ClusterNo PartWrapper::write(char* buffer,ID fcbID){
	ClusterNo writtenCluster = cache->findFreeBlock();
	cache->writeBlock(buffer,fcbID,writtenCluster);
	return writtenCluster;
}

ClusterNo PartWrapper::cluster(){
	return cache->findFreeBlock();
}

void PartWrapper::fopen(ID fcbID){
	cache->newFileCache(fcbID);
	if(rwt.fillRatio() > 0.0)
		EnterCriticalSection(&csPart);
		;
}

void PartWrapper::fclose(ID fcbID){
	cache->closeFileCache(fcbID);
	if(rwt.fillRatio() == 0.0)
		LeaveCriticalSection(&csPart);
		;
}

bool PartWrapper::getFormat(){
	return format;
}

void PartWrapper::startReading(EntryNum entry){
	ReadersWriters* rw = rwt.findKey(entry);
	if(rw == 0){
		rw = new ReadersWriters();
		rwt.insertKey(entry,rw);
	}
	rw->startRead();
}

void PartWrapper::stopReading(EntryNum entry){
	ReadersWriters* rw = rwt.findKey(entry);
	rw->stopRead();
}

void PartWrapper::startWriting(EntryNum entry){
	ReadersWriters* rw = rwt.findKey(entry);
	if(rw == 0){
		rw = new ReadersWriters();
		rwt.insertKey(entry,rw);
	}
	rw->startWrite();
}

void PartWrapper::stopWriting(EntryNum entry){
	ReadersWriters* rw = rwt.findKey(entry);
	rw->stopWrite();
}

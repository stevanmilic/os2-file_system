#include "index.h"
#include "iterator.h"

IndexAlloc::IndexAlloc(PartWrapper* pw,EntryNum entry){
	this->pw = pw;
	this->entry = entry;
	index = new ClusterNo*[secondLevel + 1];
	index[firstLevel] = new ClusterNo[numOfIndex];
	index[secondLevel] = new ClusterNo[numOfIndex];
}

IndexAlloc::~IndexAlloc(){
	delete index[firstLevel];
	delete index[secondLevel];
	delete [] index;
}

void IndexAlloc::loadIndex(){
	pw->read(index[firstLevel],ClusterSize,0,entry,pw->getStartCluster(entry));
	if(indexCluster == secondLevel && dataCluster ==numOfIndex - 1){
		pw->read(index[secondLevel],ClusterSize,0,entry,index[firstLevel][secondLevelIndex - 1]);
	}
	else if (indexCluster == secondLevel && dataCluster != numOfIndex - 1) {
		pw->read(index[secondLevel], ClusterSize, 0, entry, index[firstLevel][secondLevelIndex]);
	}
}

void IndexAlloc::load(BytesCnt len,char* buffer){
	if (buffer != nullptr) {
		this->buffer = buffer;
	}
	else {
		indexCluster = firstLevel;//first level or second level index cl.
		dataCluster = 0;//specific data cluster on first or second level
		secondLevelIndex = numOfDataIndexF;//current second level index
		currOffset = 0;
		currSize = ClusterSize;
	}
	this->len = len;
}

Iterator* IndexAlloc::createIterator(char mode){
	switch(mode){
		case 'r':
			return new ReadIterator(this);
			break;
		case 'w':
			return new WriteIterator(this);
			break;
		case 's':
			return new SeekIterator(this);
			break;
		default:
			return nullptr;
	}
}


void IndexAlloc::readIndex(BytesCnt current){
	if(indexCluster == firstLevel && dataCluster == numOfDataIndexF - 1){
		++indexCluster;
		pw->read(index[secondLevel],ClusterSize,0,entry,index[firstLevel][secondLevelIndex++]);
		dataCluster = 0;
	}
	else if(dataCluster == numOfIndex - 1){
		pw->read(index[secondLevel],ClusterSize,0,entry,index[firstLevel][secondLevelIndex++]);
		dataCluster = 0;
	}
	if (readCluster += currSize >= ClusterSize) {
		dataCluster++;
		readCluster = 0;
	}
	pw->read(buffer + current,currSize,currOffset,entry,index[indexCluster][dataCluster]);
}

void IndexAlloc::seekIndex(BytesCnt current){
	if(indexCluster == firstLevel && dataCluster == numOfDataIndexF - 1){
		++indexCluster;
		pw->read(index[secondLevel],ClusterSize,0,entry,index[firstLevel][secondLevelIndex++]);
		dataCluster = 0;
	}
	else if(dataCluster == numOfIndex - 1){
		pw->read(index[secondLevel],ClusterSize,0,entry,index[firstLevel][secondLevelIndex++]);
		dataCluster = 0;
	}
	if (readCluster += currSize >= ClusterSize) {
		dataCluster++;
		readCluster = 0;
	}
}

void IndexAlloc::writeIndex(BytesCnt current){
	if(indexCluster == firstLevel && dataCluster == (numOfDataIndexF - 1)){
		++indexCluster;
		dataCluster = 0;
	}
	//write to second level indexes
	else if(dataCluster == (numOfIndex - 1)){
		writeTo = 0;
		index[firstLevel][secondLevelIndex++] = pw->write(index[secondLevel],ClusterSize,0,entry,writeTo);	
		memset(index[secondLevel],0,ClusterSize);
		dataCluster = 0;
	}
	if (readCluster += currSize >= ClusterSize) {
		dataCluster++;
		readCluster = 0;
		writeTo = 0;
	}
	index[indexCluster][dataCluster] = pw->write(buffer + current,currSize,currOffset,entry,writeTo);
}

void IndexAlloc::writeLast(){
	if (indexCluster == secondLevel && dataCluster != numOfIndex - 1){
		writeTo = 0;
		index[firstLevel][secondLevelIndex] = pw->write(index[secondLevel], ClusterSize, 0, entry, writeTo);
	}
	
	//update first levle cluster,TO DO: check if it's updated?!
	writeTo = pw->getStartCluster(entry);
	pw->write(index[firstLevel],ClusterSize,entry,0,writeTo);
}

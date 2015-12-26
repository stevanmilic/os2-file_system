//File: index.h
#ifndef _index_h_
#define _index_h_
#include "partwrapper.h"

const	ClusterNo numOfIndex = ClusterSize/sizeof(ClusterNo);
const	ClusterNo numOfDataIndexF = numOfIndex/2;
const char firstLevel = 0;
const char secondLevel = 1;

class Iterator;

class IndexAlloc{
	char indexCluster = firstLevel;//first level or second level index cl.
	ClusterNo dataCluster = 0;//specific data cluster on first or second level
	ClusterNo secondLevelIndex = numOfDataIndexF;//current second level index

	BytesCnt currOffset = 0;
	BytesCnt currSize = ClusterSize;
	BytesCnt readCluster = 0;
	BytesCnt writeTo = 0;

	PartWrapper *pw;
	EntryNum entry;
	ClusterNo index[secondLevel + 1][numOfIndex] = { { 0 }, { 0 } };
	char* buffer;
	BytesCnt len;
	char lastMode = '\0';

public:
	
	IndexAlloc(PartWrapper*,EntryNum);
	~IndexAlloc() = default;
	void loadIndex(char='r');
	void load(BytesCnt,char* =nullptr);//argument: size of buffer
	Iterator* createIterator(char);
	BytesCnt getLen() const{
		return len;
	}
	BytesCnt getCurrOffset() const{
		return currOffset;
	}
	BytesCnt getCurrSize() const{
		return currSize;
	}
	void setCurrOffset(BytesCnt currOffset){
		this->currOffset = currOffset;
	}
	void setCurrSize(BytesCnt currSize){
		this->currSize = currSize;
	}
	void readIndex(BytesCnt);
	void seekIndex(BytesCnt);
	void writeIndex(BytesCnt);
	void writeLast();

};
#endif

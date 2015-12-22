#include "iterator.h"
#include "index.h"

void Iterator::onFirst(){
	if(index->getLen() < index->getCurrSize()){
		index->setCurrSize(index->getLen());
		smallFile = true;
	}
	current = 0;
}

void Iterator::next(){
	BytesCnt len = index->getLen();
	current += index->getCurrSize();;
	if(current < len && current == index->getCurrSize()){
		index->setCurrSize(ClusterSize);
		index->setCurrOffset(0);
	}
	else if(current < len && current + ClusterSize > len)
		index->setCurrSize(len - current);
}

bool Iterator::done(){
	if(current >= index->getLen()){
		if(smallFile)
			index->setCurrOffset(index->getCurrSize() + index->getCurrOffset());
		else
			index->setCurrOffset(index->getCurrSize());
		index->setCurrSize(ClusterSize - index->getCurrOffset());
		return true;
	}
	return false;
}

void ReadIterator::next(){
	index->readIndex(current);
	Iterator::next();
}

void WriteIterator::next(){
	index->writeIndex(current);
	Iterator::next();
}

bool WriteIterator::done(){
	if(current >= index->getLen()){
		if(smallFile)
			index->setCurrOffset(index->getCurrSize() + index->getCurrOffset());
		else
			index->setCurrOffset(index->getCurrSize());
		index->setCurrSize(ClusterSize - index->getCurrOffset());
		index->writeLast();
		return true;
	}
	return false;
}

void SeekIterator::next(){
	index->seekIndex(current);
	Iterator::next();
}

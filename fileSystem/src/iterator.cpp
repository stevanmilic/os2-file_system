#include "iterator.h"
#include "index.h"

void Iterator::onFirst() {
	if (index->getLen() < index->getCurrSize()) {
		index->setCurrSize(index->getLen());
		smallFile = true;
	}
	current = 0;
}

void Iterator::next() {
	BytesCnt len = index->getLen();
	current += index->getCurrSize();;

	if (current < len && current + index->getCurrSize() > len)
		index->setCurrSize(len - current);
	else if (current < len && current % ClusterSize == 0) {
		index->setCurrSize(ClusterSize);
		//index->setCurrOffset(0);
	}
}

bool Iterator::done() {
	if (current >= index->getLen()) {
		index->setCurrOffset(index->getCurrSize() + index->getCurrOffset());
		if (index->getCurrOffset() >= ClusterSize)
			index->setCurrOffset(0);
		index->setCurrSize(ClusterSize - index->getCurrOffset());
		return true;
	}
	return false;
}

void ReadIterator::next() {
	index->readIndex(current);
	Iterator::next();
}

void WriteIterator::next() {
	index->writeIndex(current);
	Iterator::next();
}

bool WriteIterator::done() {
	if (current >= index->getLen()) {
		index->setCurrOffset(index->getCurrSize() + index->getCurrOffset());
		if (index->getCurrOffset() >= ClusterSize)
			index->setCurrOffset(0);
		index->setCurrSize(ClusterSize - index->getCurrOffset());
		index->writeLast();
		return true;
	}
	return false;
}

void SeekIterator::next() {
	index->seekIndex(current);
	Iterator::next();
}

void DeleteIterator::next() {
	index->deleteIndex(current);
	Iterator::next();
}

bool DeleteIterator::done() {
	if (current >= index->getLen()) {
		index->setCurrOffset(index->getCurrSize() + index->getCurrOffset());
		if (index->getCurrOffset() >= ClusterSize)
			index->setCurrOffset(0);
		index->setCurrSize(ClusterSize - index->getCurrOffset());
		index->deleteLast();
		return true;
	}
	return false;
}

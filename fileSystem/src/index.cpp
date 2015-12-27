#include "index.h"
#include "iterator.h"

IndexAlloc::IndexAlloc(PartWrapper* pw, EntryNum entry) {
	this->pw = pw;
	this->entry = entry;
}

void IndexAlloc::loadIndex(char mode) {
	pw->read(index[firstLevel], ClusterSize, 0, entry, pw->getStartCluster(entry));
	if (indexCluster == secondLevel) {
		if (mode == 'r') {
			if (lastMode == 'w')
				secondLevelIndex++;
			pw->read(index[secondLevel], ClusterSize, 0, entry, index[firstLevel][secondLevelIndex - 1]);
		}
		else {
			if (lastMode == 'r')
				secondLevelIndex--;
			pw->read(index[secondLevel], ClusterSize, 0, entry, index[firstLevel][secondLevelIndex]);
		}
	}
	lastMode = mode;
}

void IndexAlloc::load(BytesCnt len, char* buffer) {
	if (buffer != nullptr) {
		this->buffer = buffer;
	}
	else {
		indexCluster = firstLevel;//first level or second level index cl.
		dataCluster = 0;//specific data cluster on first or second level
		secondLevelIndex = numOfDataIndexF;//current second level index
		currOffset = 0;
		currSize = ClusterSize;
		readCluster = 0;
	}
	this->len = len;
}

Iterator* IndexAlloc::createIterator(char mode) {
	switch (mode) {
	case 'r':
		return new ReadIterator(this);
		break;
	case 'w':
		return new WriteIterator(this);
		break;
	case 's':
		return new SeekIterator(this);
		break;
	case 'd':
		return new DeleteIterator(this);
		break;
	default:
		return nullptr;
	}
}

void IndexAlloc::readIndex(BytesCnt current) {
	if (indexCluster == firstLevel && dataCluster == numOfDataIndexF) {
		++indexCluster;
		pw->read(index[secondLevel], ClusterSize, 0, entry, index[firstLevel][secondLevelIndex++]);
		dataCluster = 0;
	}
	else if (dataCluster == numOfIndex - 1) {
		pw->read(index[secondLevel], ClusterSize, 0, entry, index[firstLevel][secondLevelIndex++]);
		dataCluster = 0;
	}

	pw->read(buffer + current, currSize, currOffset, entry, index[indexCluster][dataCluster]);
	readCluster += currSize;
	if (readCluster >= ClusterSize) {
		readCluster = 0;
		dataCluster++;
	}
}

void IndexAlloc::seekIndex(BytesCnt current) {
	if (indexCluster == firstLevel && dataCluster == numOfDataIndexF) {
		++indexCluster;
		pw->read(index[secondLevel], ClusterSize, 0, entry, index[firstLevel][secondLevelIndex++]);
		dataCluster = 0;
	}
	else if (dataCluster == numOfIndex - 1) {
		pw->read(index[secondLevel], ClusterSize, 0, entry, index[firstLevel][secondLevelIndex++]);
		dataCluster = 0;
	}
	readCluster += currSize;
	if (readCluster >= ClusterSize) {
		readCluster = 0;
		dataCluster++;
	}
}

void IndexAlloc::deleteIndex(BytesCnt current) {
	if (indexCluster == firstLevel && dataCluster == numOfDataIndexF) {
		++indexCluster;
		pw->read(index[secondLevel], ClusterSize, 0, entry, index[firstLevel][secondLevelIndex++]);
		dataCluster = 0;
	}
	else if (dataCluster == numOfIndex - 1) {
		pw->delCluster(index[firstLevel][secondLevelIndex - 1]);
		pw->read(index[secondLevel], ClusterSize, 0, entry, index[firstLevel][secondLevelIndex++]);
		dataCluster = 0;
	}

	pw->delCluster(index[indexCluster][dataCluster]);
	readCluster += currSize;
	if (readCluster >= ClusterSize) {
		readCluster = 0;
		dataCluster++;
	}
}

void IndexAlloc::deleteLast() {
	if (indexCluster == secondLevel && dataCluster != numOfIndex - 1)
		pw->delCluster(index[firstLevel][secondLevelIndex - 1]);
	pw->delCluster(pw->getStartCluster(entry));
}

void IndexAlloc::writeIndex(BytesCnt current) {
	if (indexCluster == firstLevel && dataCluster == (numOfDataIndexF)) {
		++indexCluster;
		dataCluster = 0;
		memset(index[secondLevel], 0, ClusterSize);
	}
	//write to second level indexes
	else if (dataCluster == (numOfIndex - 1)) {
		writeTo = 0;
		index[firstLevel][secondLevelIndex++] = pw->write(index[secondLevel], ClusterSize, 0, entry, writeTo);
		memset(index[secondLevel], 0, ClusterSize);
		dataCluster = 0;
	}

	writeTo = index[indexCluster][dataCluster];
	index[indexCluster][dataCluster] = pw->write(buffer + current, currSize, currOffset, entry, writeTo);
	readCluster += currSize;
	if (readCluster >= ClusterSize) {
		readCluster = 0;
		dataCluster++;
	}
}

void IndexAlloc::writeLast() {
	if (indexCluster == secondLevel && dataCluster != numOfIndex - 1) {
		writeTo = index[firstLevel][secondLevelIndex];
		index[firstLevel][secondLevelIndex] = pw->write(index[secondLevel], ClusterSize, 0, entry, writeTo);
	}

	//update first levle cluster,TO DO: check if it's updated?!
	writeTo = pw->getStartCluster(entry);
	pw->write(index[firstLevel], ClusterSize, 0, entry, writeTo);
}

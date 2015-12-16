#include "kernelfile.h"

//---------------------- UGLHHY -----------------------------------
//TO DO : make a class for index operations!!!!

char KernelFile::kwrite(BytesCnt len, char *writeBuffer){
	//get right fcb and pw
	FCB *fcb = KernelFS::ft.findKey(id);
	if(id.getMode() == 'r')
		return 0;
	PartWrapper* pw = KernelFS::pt.findKey(PartWrapper::toNumber(fcb->getPart()));

	ClusterNo** index = new ClusterNo*[secondLevel + 1];
	index[firstLevel] = new ClusterNo[numOfIndex]();

	if(getFileSize()){
		pw->read(index[firstLevel],ClusterSize,0,fcb->getEntry(),pw->getStartCluster(fcb->getEntry()));
		if(indexCluster == secondLevel){
			index[secondLevel] = new ClusterNo[numOfIndex]();	
			pw->read(index[secondLevel],ClusterSize,0,fcb->getEntry(),index[firstLevel][secondLevelIndex - 1]);
		}
		currByte += len;
		if(currByte > getFileSize())
			pw->setFileSize(fcb->getEntry(),currByte);
		else
			currByte = getFileSize();
	}

	BytesCnt currWrite = 0;
	char small = 0;
	if(len < currSize){
		currSize = len;
		small = 1;
	}

	while(currWrite < len){
		//if first level index use only first 256 inputs for data clusters
		//rest is for index level two
		if(indexCluster == firstLevel && dataCluster == (numOfDataIndexF - 1)){
			++indexCluster;
			index[secondLevel] = new ClusterNo[numOfIndex]();	
			dataCluster = 0;
		}
		//write to second level indexes
		else if(dataCluster == (numOfIndex - 1)){
			index[firstLevel][secondLevelIndex++] = pw->write(index[secondLevel],ClusterSize,0,fcb->getEntry());	
			memset(index[secondLevel],0,ClusterSize);
			dataCluster = 0;
		}

		index[indexCluster][dataCluster++] = pw->write(writeBuffer + currWrite,currSize,currOffset,fcb->getEntry());

		currWrite += currSize;
		if(currWrite < len && currWrite == currSize){
			currSize = ClusterSize;
			currOffset = 0;
		}

		if(currWrite < len && currWrite + ClusterSize > len){
			currSize = len - currWrite;
		}
	}

	if(small)
		currOffset += currSize;
	else
		currOffset = currSize;
	currSize = ClusterSize - currOffset;
	
	if(indexCluster == secondLevel && dataCluster != numOfIndex - 1)
		index[firstLevel][secondLevelIndex] = pw->write(index[secondLevel],ClusterSize,0,fcb->getEntry());
	
	//update first levle cluster,TO DO: check if it's updated?!
	pw->write(index[firstLevel],ClusterSize,fcb->getEntry(),pw->getStartCluster(fcb->getEntry()));

	for(ClusterNo i = 0; i < indexCluster + 1; i++)
		delete index[i];
	delete [] index;
	return 1;
}

BytesCnt KernelFile::kread(BytesCnt len, char *readBuffer){
	FCB *fcb = KernelFS::ft.findKey(id);
	if(id.getMode() == 'w')
		return 0;
	PartWrapper* pw = KernelFS::pt.findKey(PartWrapper::toNumber(fcb->getPart()));
	ClusterNo** index = new ClusterNo*[secondLevel + 1];
	index[firstLevel] = new ClusterNo[numOfIndex];

	pw->read(index[firstLevel],ClusterSize,0,fcb->getEntry(),pw->getStartCluster(fcb->getEntry()));

	if(getFileSize()){
		if(currByte + len > getFileSize())
			len = getFileSize() - currByte;
	}
	else
		return 0;

	BytesCnt currRead = 0;
	char small = 0;
	if(len < currSize){
		currSize = len;
		small = 1;
	}

	while(currRead < len){

		if(indexCluster == firstLevel && dataCluster == numOfDataIndexF - 1){
			++indexCluster;
			index[secondLevel] = new ClusterNo[numOfIndex]();	
			pw->read(index[secondLevel],ClusterSize,0,fcb->getEntry(),index[firstLevel][secondLevelIndex++]);
			dataCluster = 0;
		}
		else if(dataCluster == numOfIndex - 1){
			pw->read(index[secondLevel],ClusterSize,0,fcb->getEntry(),index[firstLevel][secondLevelIndex++]);
			dataCluster = 0;
		}

		pw->read(readBuffer + currRead,currSize,currOffset,fcb->getEntry(),index[indexCluster][dataCluster++]);
		
		currRead += currSize;
		if(currRead < len && currRead == currSize){
			currSize = ClusterSize;
			currOffset = 0;
		}

		if(currRead < len && currRead + ClusterSize > len){
			currSize = len - currRead;
		}
	}

	if(small)
		currOffset += currSize;
	else
		currOffset = currSize;
	currSize = ClusterSize - currOffset;

	currByte += len;
	for(ClusterNo i = 0; i < indexCluster; i++)
		delete index[i];
	delete [] index;
	return currRead;
}

char KernelFile::seek(BytesCnt len){
	FCB *fcb = KernelFS::ft.findKey(id);
	if(id.getMode() == 'w')
		return 0;
	PartWrapper* pw = KernelFS::pt.findKey(PartWrapper::toNumber(fcb->getPart()));
	ClusterNo** index = new ClusterNo*[secondLevel + 1];
	index[firstLevel] = new ClusterNo[numOfIndex];

	pw->read(index[firstLevel],ClusterSize,0,fcb->getEntry(),pw->getStartCluster(fcb->getEntry()));

	if(getFileSize()){
		if(currByte + len > getFileSize())
			len = getFileSize() - currByte;
	}
	else
		return 0;

	BytesCnt currRead = 0;
	BytesCnt currSize = ClusterSize;	
	if(len < ClusterSize)
		currSize = len;

	while(currRead < len){

		if(indexCluster == firstLevel && dataCluster == numOfDataIndexF - 1){
			++indexCluster;
			index[secondLevel] = new ClusterNo[numOfIndex]();	
			pw->read(index[secondLevel],ClusterSize,0,fcb->getEntry(),index[firstLevel][secondLevelIndex++]);
			dataCluster = 0;
		}
		else if(dataCluster == numOfIndex - 1){
			pw->read(index[secondLevel],ClusterSize,0,fcb->getEntry(),index[firstLevel][secondLevelIndex++]);
			dataCluster = 0;
		}
		dataCluster++;

		currRead += currSize;
		if(currRead + ClusterSize > len)
			currSize = len - currRead;
	}
	currOffset = ClusterSize - currSize;
	currByte += currRead;
	for(ClusterNo i = 0; i < secondLevel + 1; i++)
		delete index[i];
	delete [] index;
	return currRead;
}

BytesCnt KernelFile::filePos(){
	return currByte;
}

char KernelFile::eof(){
	if(currByte == getFileSize())
		return 1;
	return 0;
}

BytesCnt KernelFile::getFileSize(){
	FCB *fcb = KernelFS::ft.findKey(id);
	PartWrapper* pw = KernelFS::pt.findKey(PartWrapper::toNumber(fcb->getPart()));
	return pw->getFileSize(fcb->getEntry());
}

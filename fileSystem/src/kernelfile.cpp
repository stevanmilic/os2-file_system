#include "kernelfile.h"

char KernelFile::kwrite(BytesCnt len, char *writeBuffer){
	//get right fcb and pw
	FCB *fcb = KernelFS::ft.findKey(id);
	if(id.mode == 'r')
		return 0;
	PartWrapper* pw = KernelFS::pt.findKey(PartWrapper::toNumber(fcb->getPart()));

	//define and initialze buffer and indexes for writing
	char *buffer = new char[ClusterSize]();

	ClusterNo** index = new ClusterNo*[secondLevel + 1];
	index[firstLevel] = new ClusterNo[numOfIndex]();

	if(getFileSize()){
		pw->read(buffer,fcb->getEntry(),pw->getStartCluster(fcb->getEntry()));
		memcpy(index[firstLevel],buffer,ClusterSize);
		if(indexCluster == secondLevel){
			pw->read(buffer,fcb->getEntry(),currentCluster);
			index[secondLevel] = new ClusterNo[numOfIndex]();	
			memcpy(index[secondLevel],buffer,ClusterSize);
		}
	}
	BytesCnt currWrite = 0;
	while(currWrite < len){
		//if first level index use only first 256 inputs for data clusters
		//rest is for index level two
		if(indexCluster == firstLevel && dataCluster == (numOfDataIndexF - 1)){
			++indexCluster;
			index[secondLevel] = new ClusterNo[numOfIndex]();	
			dataCluster = 0;
		}

		//write to second level indexes
		if(dataCluster == (numOfIndex - 1)){
			memcpy(buffer,index[secondLevel],ClusterSize);
			currentCluster = index[firstLevel][secondLevelIndex++] = pw->write(buffer,fcb->getEntry());	
			memset(index[secondLevel],0,ClusterSize);
			dataCluster = 0;
		}

		index[indexCluster][dataCluster++] = pw->write(writeBuffer + currWrite,fcb->getEntry());
		if(currWrite + ClusterSize > len)
			currWrite += len - currWrite;
		else
			currWrite += ClusterSize;
	}
	
	currByte += currWrite;
	
	//write to first level which is also first cluster to start
	//update dir for specific entry, also
	memcpy(buffer,index[firstLevel],ClusterSize);
	pw->setStartCluster(fcb->getEntry(),pw->write(buffer,fcb->getEntry()));

	for(ClusterNo i = 0; i < secondLevel + 1; i++)
		delete index[i];
	delete [] index;
	delete [] buffer;
	return 1;
}

BytesCnt KernelFile::kread(BytesCnt len, char *readBuffer){
	FCB *fcb = KernelFS::ft.findKey(id);
	if(id.mode != 'r')
		return 0;
	PartWrapper* pw = KernelFS::pt.findKey(PartWrapper::toNumber(fcb->getPart()));
	char *buffer = new char[ClusterSize]();
	ClusterNo** index = new ClusterNo*[secondLevel + 1];
	index[firstLevel] = new ClusterNo[numOfIndex];

	pw->read(buffer,fcb->getEntry(),pw->getStartCluster(fcb->getEntry()));
	memcpy(index[firstLevel],buffer,ClusterSize);
	currByte = 0;
	

	while(currByte <= len){

		if(indexCluster == firstLevel && dataCluster == numOfDataIndexF - 1){
			++indexCluster;
			dataCluster = 0;
		}
		else if(dataCluster == numOfIndex - 1){
			pw->read(buffer,fcb->getEntry(),index[firstLevel][secondLevelIndex++]);
			memcpy(index[secondLevel],buffer,ClusterSize);
			dataCluster = 0;
		}

		pw->read(buffer,fcb->getEntry(),index[indexCluster][dataCluster++]);
		memcpy(readBuffer + currByte, buffer, ClusterSize);
		
		if(currByte + ClusterSize > len)
			currByte = len - currByte;
		else
			currByte += ClusterSize;
	}
	for(ClusterNo i = 0; i < secondLevel + 1; i++)
		delete index[i];
	delete [] index;
	delete [] buffer;
	return currByte;
}

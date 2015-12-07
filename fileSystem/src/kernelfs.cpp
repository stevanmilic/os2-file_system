//File: kernelfs.cpp
#include "kernelfs.h"
#include "kernelfile.h"
#include <iostream>

KernelFS* KernelFS::onlySample = nullptr;

KernelFS* KernelFS::sample(){
	if(onlySample == nullptr) 
		onlySample = new KernelFS();
	return onlySample;
}

char KernelFS::kmount(Partition *part){
	if(pt.fillRatio() == 1)
		return '0';//exception: full on part
	PartWrapper* pw = new PartWrapper(part);
	pt.insertKey(pw->getID(), pw);
	return pw->getName();
}

char KernelFS::kunmount(char part){
	if(pt.findKey(PartWrapper::toNumber(part) == 0)
		return 0;//excep: part with this part name doesn't exist
	enterCriticalSection(part);
	pt.deleteKey(PartWrapper::toNumber(part));
	return 1;
}

char KernelFS::kformat(char part){
	pw = pt.findKey(PartWrapper::toNumber(part);
	if(pw == 0)
		return 0;//excep: part with this part name doesn't exist
	enterCriticalSection(part);
	pw->clearBitVector();
	pw->clearDir();
	return 1;
	/*use this for cache implementation
	Partition *part = partInter[partIndex].part;
	std::memset(buffer,1,ClusterSize);
	unsigned long bvClusters = part->getNumOfClusters()/ClusterSize + part->getNumOfClusters() % ClusterSize ? 1 : 0;//if number of clusters exceeds clustersize
	partInter[partIndex].dirIndex = bvClusters + 1;// + 1 for dir index
	std::memset(buffer,0,partInter[partIndex].dirIndex);
	for(unsigned long i = 0; i < partInter[partIndex].dirIndex; i++){
		part->writeCluster(i, buffer);
		std::memset(buffer,1,ClusterSize);
	}
	std::memset(buffer,0,ClusterSize);
	std::memcpy(buffer,myDir,sizeof myDir);
	part->writeCluster(partInter[partIndex].dirIndex,buffer);// if write = 1*/
}

char KernelFS::kexist(char* fname){
	//TO DO:
	//implement with partWrapper
	std::memset(buffer,0,ClusterSize);
	for(int i = 0; i < partCounter; i++){
		partInter[i].part->readCluster(partInter[i].dirIndex,buffer);
		std::memcpy(myDir,buffer,sizeof myDir);
		for(int j = 0;j < ENTRYCNT && myDir[j].name != 0; j++)
			if(myDir[j].name == fname)
				return 1;
	}
	return 0;
}

char KernelFS::kreadRootDir(char partName, EntryNum entryNum,Directory &dir){
	//TO DO:
	//implement with partWrapper
	PartNum partIndex = partName - 'A';
	Partition* part = partInter[partIndex].part;
	std::memset(buffer,0,ClusterSize);
	part->readCluster(partInter[partIndex].dirIndex,buffer);
	std::memcpy(dir,buffer,ClusterSize);
	char counter = 0;
	//TO DO: faster search -> log saerch
	for(int i = entryNum; i < ENTRYCNT; i++)
		if(myDir[i].name[0] != '\0')
			dir[counter++] = myDir[i];
	if(counter < 64)
		return 0;
	return 1;
}

File* KernelFS::kopen(char* fpath, char mode){
	//TO DO:
	//implement with partWrapper
	//implement with FCBtable
	PartNum partIndex;
	char *fname;
	Partition *part = nullptr;
	char index = -1;
	File *file;
	FCB* newFCB;
	switch(mode){
		case 'r':
			partIndex = fpath[0] - 'A';
			fname = getFileName(fpath);
			part = partInter[partIndex].part;
			enterCriticalSection(partIndex,fname);
			std::memset(buffer,0,ClusterSize);
			part->readCluster(partInter[partIndex].dirIndex, buffer);
			std::memcpy(myDir,buffer,sizeof myDir);
			for(char i = 0; i < ENTRYCNT; i++)
				if(myDir[i].name == fname)
					index = i;
			if(index < 0)
				return nullptr;
			file = new File();
			newFCB = new FCB(myDir[index],partIndex,index,0);
			openedFiles.add(newFCB,fcbCounter);
			file->myImpl->setID(fcbCounter++);
			break;
		case 'w':
			//start creating a file
			//TO DO: 1)findEntry,2)OpenAnewEntry,3)loadAtributesOfFile,4)checkRights
			//5)findFileSomeSpace!
			//1)findEntry
			partIndex = fpath[0] - 'A';
			fname = getFileName(fpath);
			part = partInter[partIndex].part;
			enterCriticalSection(partIndex,fname);
			std::memset(buffer,0,ClusterSize);
			part->readCluster(partInter[partIndex].dirIndex, buffer);
			std::memcpy(myDir,buffer,sizeof myDir);
			for(char i = 0; i < ENTRYCNT; i++)
				//2)findEntry
				if(myDir[i].name[0] == '\0'){
					//3)loadAtributes
					strcpy(myDir[index = i].name, getName(fname));
					strcpy(myDir[index].ext, getExt(fname));
					myDir[index].indexCluster = partInter[partIndex].dirIndex + 1;
					myDir[index].size = ClusterSize;
				}
				else if(myDir[i].name == fname){
					kdelete(fpath);
					//myDir[index].name[0] = '\0';
					return nullptr;
					}
			if(index < 0)
				return nullptr;
			std::memcpy(buffer,myDir,sizeof myDir);
			part->writeCluster(partInter[partIndex].dirIndex,buffer);
			file = new File();
			newFCB = new FCB(myDir[index],partIndex,index,0);
			openedFiles.add(newFCB,fcbCounter);
			file->myImpl->setID(fcbCounter++);
			break;
		case 'a':
			partIndex = fpath[0] - 'A';
			fname = getFileName(fpath);
			part = partInter[partIndex].part;
			enterCriticalSection(partIndex,fname);
			std::memset(buffer,0,ClusterSize);
			part->readCluster(partInter[partIndex].dirIndex, buffer);
			std::memcpy(myDir,buffer,sizeof myDir);
			for(char i = 0; i < ENTRYCNT; i++)
				if(myDir[i].name == fname)
					index = i;
			if(index < 0)
				return nullptr;
			file = new File();
			newFCB = new FCB(myDir[index],partIndex,index,0);
			openedFiles.add(newFCB,fcbCounter);
			file->myImpl->setID(fcbCounter++);
			break;
		default:
			return nullptr;
	}
	return file;
}

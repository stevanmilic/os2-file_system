//File: kernelfs.cpp
#include "kernelfs.h"
#include "kernelfile.h"
#include "cache.h"
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
	Cache c = new Cache(part);
	PartWrapper* pw = new PartWrapper(cache);
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
	PartWrapper*  pw = pt.findKey(PartWrapper::toNumber(part);
	if(pw == 0)
		return 0;//excep: part with this part name doesn't exist
	enterCriticalSection(part);
	pw->clear();
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
	PartWrapper* pw = pt.findKey(PartWrapper::parseName(fname));
	if(pw == 0)
		return -1;//excep: partition doesn't exist
	Directory* myDir = pw->readDir();
	fname = FCB::parseName();	
	for(int j = 0;j < ENTRYCNT; j++)
		if(*myDir[j].name == fname)
			return j;//file found with given index
	return -2;//file not found
	/*use for cache implemention
	std::memset(buffer,0,ClusterSize);
	for(int i = 0; i < partCounter; i++){
		partInter[i].part->readCluster(partInter[i].dirIndex,buffer);
		std::memcpy(myDir,buffer,sizeof myDir);
		for(int j = 0;j < ENTRYCNT && myDir[j].name != 0; j++)
			if(myDir[j].name == fname)
				return 1;
	}
	return 0;*/
}

char KernelFS::kreadRootDir(char part, EntryNum entryNum,Directory &dir){
	PartWrapper* pw = pt.findKey(PartWrapper::toName(part));
	if(pw == 0)
		return 0;//excep: partition doesn't exist
	Directory* myDir = pw->rootDir();	

	char counter = 0;
	for(int i = entryNum; i < ENTRYCNT; i++)
		if(*myDir[i].name[0] != '\0')
			dir[counter++] = *myDir[i];
	if(counter < 64)
		return 0;
	return 1;
	/*use for cache implementation
	Partition* part = partInter[partIndex].part;
	std::memset(buffer,0,ClusterSize);
	part->readCluster(partInter[partIndex].dirIndex,buffer);
	std::memcpy(dir,buffer,ClusterSize);
	//TO DO: faster search -> log saerch
	for(int i = entryNum; i < ENTRYCNT; i++)
		if(myDir[i].name[0] != '\0')
			dir[counter++] = myDir[i];
	if(counter < 64)
		return 0;
	return 1;*/
}

File* KernelFS::newFileOpened(PartWrapper* pw, char* fpath, char index, char mode){
	FCB* newFCB = new FCB(PartWrapper::parseName(fpath), index, mode);
	file->myImpl->addFCB_ID(newFCB::getID());
	ft->insertKey(newFCB->getID(), newFCB);
	pw->fopen(newFCB->getID());
}

File* KernelFS::startReading(char *fpath,char mode){
	char index = kexists(fpath);
	if(index < 0)
		return nullptr;//excep: file not found or no part with given name
	PartWrapper* pw = pt.findKey(PartWrapper::toName(part));
	return newFileOpened(pw, fpath, index mode);
}

File* KernelFS::startWriting(char* fpath){
	char index = kexists(fpath);
	if(index == -1)
		return nullptr;//excep:: no partition with given name
	else if(index >= 0){
		kdelete(fpath);
		return nullptr;//excep: file exists and it's deleted
	}
		
	PartWrapper* pw = pt.findKey(PartWrapper::toName(part));
	Directory* myDir = pw->rootDir();	
	
	for(char i = 0; i < ENTRYCNT; i++)
		if(*myDir[i].name[0] == '\0'){
			strcpy(*myDir[index = i].name, FCB::parseName(fpath));
			strcpy(*myDir[index].ext, FCB::parseExt(fname));
			*myDir[index].indexCluster = pw->cluster();
			*myDir[index].size = ClusterSize;
		}

	return newFileOpened(pw, fpath, index, 'w');
	
}

File* KernelFS::kopen(char* fpath, char mode){
	switch(mode){
		case 'r':
			enterCriticalSection(partIndex,fname);
			return startReading(fpath,mode);
			/*use for cache implementation
			std::memset(buffer,0,ClusterSize);
			part->readCluster(partInter[partIndex].dirIndex, buffer);
			std::memcpy(myDir,buffer,sizeof myDir);*/
			break;
		case 'w':
			enterCriticalSection(partIndex,fname);
			return startWriting(fname);
			/*use for cache implementation
			std::memcpy(buffer,myDir,sizeof myDir);
			part->writeCluster(partInter[partIndex].dirIndex,buffer);*/
			break;
		case 'a':
			enterCriticalSection(partIndex,fname);
			return startReading(fname,mode);
			break;
		default:
			return nullptr;
	}
}

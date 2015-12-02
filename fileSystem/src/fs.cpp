#include "fs.h"
#include "kernelfs.h"
KernelFS* FS::myImpl = nullptr;

char FS::mount(Partition* partition){
	myImpl = KernelFS::sample();
	return myImpl->kmount(partition);
}

char FS::unmount(char part){
	return myImpl->kunmount(part);
}

char FS::format(char part){
	return myImpl->kformat();
}

char FS::readRootDir(char part, EntryNum n,Directory &d){
	return myImpl->kreadRootDir(char* fname)
}

char FS::doesExist(char* fname){
	return myImpl->kexist(fname);
}

File* FS::open(char* fname, char mode){
	return myImpl->kopen(fname,mode);
}

char FS::deleteFile(char* fname){
	return myImpl->kdelete(fname);
}

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
	return myImpl->kformat(part);
}

char FS::readRootDir(char part, EntryNum n,Directory &d){
	//ako se var d prenosi po referenci onda bi trebalo dalje da pokazuje kao pointer... implementirati :)
	return myImpl->kreadRootDir(part,n,d);
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

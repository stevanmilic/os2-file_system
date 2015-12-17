#include "kernelfile.h"
#include "file.h"

File::File(){
	myImpl = new KernelFile();
}

File::~File(){
	delete myImpl;
}

char File::write(BytesCnt size, char* buffer){
	return myImpl->kwrite(size,buffer);
}

BytesCnt File::read(BytesCnt size, char* buffer){
	return myImpl->kread(size,buffer);
}

char File::seek(BytesCnt seekByte){
	return myImpl->seek(seekByte);
}

BytesCnt File::filePos(){
	return myImpl->filePos();
}

char File::eof(){
	return myImpl->eof();
}

BytesCnt File::getFileSize(){
	return myImpl->getFileSize();
}

char File::truncate(){
	return myImpl->truncate();
}

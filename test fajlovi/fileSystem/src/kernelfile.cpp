#include "kernelfile.h"
#include "iterator.h"

KernelFile::KernelFile(){
	/*FCB *fcb = KernelFS::ft.findKey(id);
	PartWrapper* pw = KernelFS::pt.findKey(PartWrapper::toNumber(fcb->getPart()));
	index = new IndexAlloc(pw,fcb->getEntry());*/
}

KernelFile::~KernelFile(){
	FCB *fcb = KernelFS::ft.findKey(id);
	fcb->closeMode(id.getMode());
	if(!fcb->fileOpened()){
		PartWrapper* pw = KernelFS::pt.findKey(fcb->getPart());
		pw->fclose(fcb->getEntry());
		KernelFS::ft.deleteKey(id);
	}
}

void KernelFile::addFCBid(FCBid id){
	this->id = id;
	FCB *fcb = KernelFS::ft.findKey(id);
	PartWrapper* pw = KernelFS::pt.findKey(fcb->getPart());
	index = new IndexAlloc(pw, fcb->getEntry());
}

char KernelFile::kwrite(BytesCnt len, char *writeBuffer){
	if(id.getMode() == 'r')
		return 0;

	//if file already has data written
	if(getFileSize())
		index->loadIndex();

	index->load(len,writeBuffer);

	Iterator* iter = index->createIterator('w');
	for (iter->onFirst(); !iter->done(); iter->next());
	currByte += iter->curr();;

	if(currByte > getFileSize()){
		FCB *fcb = KernelFS::ft.findKey(id);
		PartWrapper* pw = KernelFS::pt.findKey(fcb->getPart());
		pw->setFileSize(fcb->getEntry(),currByte);
	}
	else
		currByte = getFileSize();
	delete iter;
	return 1;
}

BytesCnt KernelFile::kread(BytesCnt len, char *readBuffer){
	/*if(id.getMode() == 'w')
		return 0;*/
	
	if(getFileSize()){
		index->loadIndex();
		if(currByte + len > getFileSize())
			len = getFileSize() - currByte;
	}

	index->load(len,readBuffer);

	Iterator* iter = index->createIterator('r');
	for(iter->onFirst();!iter->done();iter->next());
	currByte += iter->curr();
	delete iter;
	return iter->curr();
}


char KernelFile::seek(BytesCnt len){
	if(getFileSize()){
		index->loadIndex();
		currByte = 0;
		if(currByte + len > getFileSize())
			len = getFileSize() - currByte;
	}

	index->load(len);

	Iterator* iter = index->createIterator('s');
	for(iter->onFirst();!iter->done();iter->next());
	currByte += iter->curr();
	delete iter;
	return 1;
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
	PartWrapper* pw = KernelFS::pt.findKey(fcb->getPart());
	return pw->getFileSize(fcb->getEntry());
}

char KernelFile::truncate(){
	//TO DO: implement this :)
	return 1;
}

//File : kernelfile.h
#ifndef _kernelfile_h_
#define _kernelfile_h_
#include "partwrapper.h"
#include "fcb.h"
#include "kernelfs.h"

const	ClusterNo numOfIndex = ClusterSize/sizeof(ClusterNo);
const	ClusterNo numOfDataIndexF = numOfIndex/2;
const char firstLevel = 0;
const char secondLevel = 1;

class KernelFile{
	private:
		friend class File;

		FCBid id;

		char indexCluster = firstLevel;//first level or second level index cl.
		ClusterNo dataCluster = 0;//specific data cluster on first or second level
		ClusterNo secondLevelIndex = numOfDataIndexF;//current second level index

		BytesCnt currByte = 0;//current byte position in file
		BytesCnt currOffset = 0;
		BytesCnt currSize = ClusterSize;

//		KernelFile();
		
		char kwrite(BytesCnt,char*);
		BytesCnt kread(BytesCnt,char*);
		char seek(BytesCnt);
		BytesCnt filePos();
		char eof();
		BytesCnt getFileSize();
		char truncate();
	public:
		void addFCBid(FCBid);
		~KernelFile();
};
#endif

//File : kernelfile.h
#ifndef _kernelfile_h_
#define _kernelfile_h_
#include "fcb.h"
#include "kernelfs.h"
#include "index.h"
//#include "partwrapper.h"

class KernelFile{
	friend class File;

	FCBid id;
	IndexAlloc* index;
	BytesCnt currByte = 0;//current byte position in file

	/*char indexCluster = firstLevel;//first level or second level index cl.
	ClusterNo dataCluster = 0;//specific data cluster on first or second level
	ClusterNo secondLevelIndex = numOfDataIndexF;//current second level index

	BytesCnt currOffset = 0;
	BytesCnt currSize = ClusterSize;*/

	KernelFile();
	
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

//File : kernelfile.h
#ifndef _kernelfile_h_
#define _kernelfile_h_
#include "file.h"
#include "mutex.h"
#include "windows.h"

#define signal(x) ReleaseSemaphore(x,1,NULL)
#define wait(x) WaitForSingleObject(x,INFINITE)

class KernelFile{
	private:
		friend class File;

		struct IndexClusterFirst{
			ClusterNo dataCluster[1024];
			ClusterNo indexCluster[1024];
		};

		struct IndexClusterSecond{
			ClusterNo dataCluster[ClusterSize];
		};

		HANDLE sem;

		KernelFile();
		
		char kwrite(BytesCnt,char*);
		BytesCnt kread(BytesCnt,char*);
		char seek(BytesCnt);
		BytesCnt filePos();
		char eof();
		BytesCnt getFileSize();
		char truncate();
		void setID(unsigned long);

		friend char KernelFS::kopen(char*,char);

	public:
		~KernelFile();
};
#endif

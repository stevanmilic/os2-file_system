//File : kernelfile.h
#ifndef _kernelfile_h_
#define _kernelfile_h_
#include "file.h"
#include "mutex.h"
#include "windows.h"

class KernelFile{
	private:
		friend class File;

		struct IndexClusterFirst{
			ClusterNo dataCluster[1024];
			ClusterNo indexCluster[1024];
		}

		struct IndexClusterSecond{
			ClusterNo dataCluster[ClusterSize];
		}

		HANDLE sem;

		KernelFile();
		
		char kwrite(BytesCnt,char*);
		BytesCnt kread(BytesCnt,char*);
		char seek(BytesCnt);
		BytesCnt filePos();
		char eof();
		BytesCnt getFileSize();
		char truncate();

	public:
		~KernelFile();
};
#endif

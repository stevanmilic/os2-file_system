//File : kernelfile.h
#ifndef _kernelfile_h_
#define _kernelfile_h_
#include "fs.h"
#include "mutex.h"
#include "windows.h"

class KernelFile{
	private:
		friend class File;

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

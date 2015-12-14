//File : kernelfile.h
#ifndef _kernelfile_h_
#define _kernelfile_h_
#include "kernelfs.h"

typedef unsigned long FCB_ID;

class KernelFile{
	private:
		friend class File;
		friend class KernelFS;

		FCB_ID id;

		KernelFile();
		
		char kwrite(BytesCnt,char*);
		BytesCnt kread(BytesCnt,char*);
		char seek(BytesCnt);
		BytesCnt filePos();
		char eof();
		BytesCnt getFileSize();
		char truncate();
		void addFCB_ID(FCB_ID);
	public:
		~KernelFile();
};
#endif

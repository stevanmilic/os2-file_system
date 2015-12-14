//File : kernelfs.h
#ifndef _kernelfs_h_
#define _kernelfs_h_
#include "fs.h"
#include "file.h"
#include "partwrapper.h"
#include "fcb.h"

const char ALPHASIZE = 26;

class KernelFS{
	protected:
		KernelFS();
	private:
		friend class FS;

		HashTable<PartWrapper*,char> pt;
		HashTable<FCB*,unsigned long> ft;

		static KernelFS* onlySample;
		static KernelFS* sample();

		char kmount(Partition*);
		char kunmount(char);
		char kformat(char);
		EntryNum kexist(char*);
		char kreadRootDir(char,EntryNum,Directory&);
		File* kopen(char*,char);
		char kdelete(char*); 

		File* newFileOpened(PartWrapper*,char*,char,char);
		File* startReading(char*,char);
		File* startWriting(char*);
	public:
		~KernelFS();
};
#endif

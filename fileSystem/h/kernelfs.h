//File : kernelfs.h
#ifndef _kernelfs_h_
#define _kernelfs_h_
#include "fs.h"
#include "file.h"
#include "windows.h"
#include "partwrapper.h"
#include "fcb.h"


#define signal(x) ReleaseSemaphore(x,1,NULL)
#define wait(x) WaitForSingleObject(x,INFINITE)

const char ALPHASIZE = 26;


class KernelFS{
	protected:
		KernelFS();
	private:
		friend class FS;

		HANDLE sem;

		HashTable<PartWrapper*> pt(ALPHASIZE);
		HashTable<FCB*> ft(ENTRYCNT);

		static KernelFS* onlySample;
		static KernelFS* sample();

		char kmount(Partition*);
		char kunmount(char);
		char kformat(char);
		char kexist(char*);
		char kreadRootDir(char,EntryNum,Directory&);
		File* kopen(char*,char);
		char kdelete(char*); 
		char kclose(fcbID);

		File* newFileOpened(PartWrapper*,char*,char index,char);
		File* startReading(char*,char);
		File* startWriting(char*);

		void enterCriticalSection(PartNum,char* fname = nullptr);
	public:
		~KernelFS();
};
#endif

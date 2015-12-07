//File : kernelfs.h
#ifndef _kernelfs_h_
#define _kernelfs_h_
#include "fs.h"
#include "file.h"
#include "windows.h"
#include "list.h"

#define signal(x) ReleaseSemaphore(x,1,NULL)
#define wait(x) WaitForSingleObject(x,INFINITE)

typedef unsigned long fcbID;
const PartNum alphabetSize = 26;

struct FCB{
	Entry entry;
	PartNum partNum;
	EntryNum entryNum;
	BytesCnt cursor;
	FCB(Entry e, PartNum pn,EntryNum en, BytesCnt bc) : entry(e), partNum(pn), entryNum(en), cursor(bc) {}
};


class KernelFS{
	protected:
		KernelFS();
	private:
		friend class FS;

		HANDLE sem;

		static PartWrapper *pw;
		fcbID fcbCounter;
		Directory myDir;
		List<FCB*> openedFiles;

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

		void enterCriticalSection(PartNum,char* fname = nullptr);
		char* getFileName(char*);
		char* getName(char*);
		char* getExt(char*);

	public:
		~KernelFS();
};
#endif

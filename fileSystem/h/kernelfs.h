//File : kernelfs.h
#ifndef _kernelfs_h_
#define _kernelfs_h_
#include "fs.h"
#include "mutex.h"
#include "windows.h"
#include "list.h"
#include "part.h"

typedef unsigned long fcbID;
typedef char PartNum;

class KernelFS{
	protected:
		KernelFS();
	private:
		friend class FS;

		struct FCB{
			Entry entry;
			PartNum partNum;
			EntryNum entryNum;
			BytesCnt cursor;
			FCB(Entry e, PartNum pn,EntryNum en, BytesCnt bc) : entry(e), partNum(pn), entryNum(en), cursor(bc) {}
		};
		
		struct PartitionInterface{
			Partition* part;
			ClusterNo dirIndex;
		};

		
		const PartNum alphabetSize = 26;
		HANDLE sem;

		PartitionInterface partInter[alphabetSize];
		PartNum partCounter;
		char *buffer;
		fcbID fcbCounter;
		Directory myDir;
		List<FCB*>* openedFiles;

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

		void enterCriticalSection(char partName,char* fname = nullptr);
		char* getFileName(char*);
		char* getName(char*);
		char* getExt(char*);
	public:
		~KernelFS();
};
#endif

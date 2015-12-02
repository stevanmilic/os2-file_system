//File : kernelfs.h
#ifndef _kernelfs_h_
#define _kernelfs_h_
#include "fs.h"
#include "mutex.h"
#include "windows.h"

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
			ByteCnt cursor;
			FCB(Entry e, PartNum pn,EntryNum en, ByteCnt bc) : entry(e), partNum(pn), entryNum(en), cursor(bc) {}
		}
		
		struct PartitionInterface{
			Partion* part;
			ClusterNo dirIndex;
			~PartitionInterface(){
				part = nullptr;
			}
		}

		struct IndexClusterFirst{
			ClusterNo dataCluster[1024];
			ClusterNo indexCluster[1024];
		}

		struct IndexClusterSecond{
			ClusterNo dataCluster[ClusterSize];
		}
		
		const PartNum alphabetSize = 26;
		HANDLE sem;

		PartitionInterface *partInter;
		PartNum partCounter;
		char *buffer;
		unsigned long fcbCounter;
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

		void enterCriticalSection(char partName,char* fname = nullptr);
		char* getFileName(char*);
		char* getName(char*);
		char* getExt(char*);
		ClusterNo getFreeCluster(Partition*);
	  ClusterNo findLastWrittenCluster(ClusterNo);
		ByteCnt findLastWrittenByte(ClusterNo);
	public:
		~KernelFS();
};
#endif

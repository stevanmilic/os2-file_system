//File : kernelfs.h
#ifndef _kernelfs_h_
#define _kernelfs_h_
#include "fs.h"
#include "file.h"
#include "windows.h"
#include "list.h"
#include "part.h"
#include "hashtable.h"

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

class PartitionWrapper{
	//TO DO:
	//ima hash tabelu koja sadrzi trenutno montirane(mount) particije
	//moze da se doda,uzme particija, direktiva oko dirIndexa(prosledj se partName)
	//moze da se upisuje, da se cita sa particije ->memset, memcpy -> buffer
	//prosledjuse fpath, vrati ime particije
	//vraca broj potreban za bitvektor klastere... dirIndex...
	//vraca prvu prazni klaster za otvoreni fajl,ili za nest sto treba fajl-u?
	//da se koristi partitionIntreface i od kernelfile.h :)
	class PartInterface{
		Partition* part;
		ClusterNo dirIndex;
		static char partCounter;
	public:
		PartInterface(Partition* part){
			this->part = part;
		}
		
		void setDir(ClusterNo dirIndex){
			this->dirIndex = dirIndex;
		}

		Partition* getPart(){
			return part;
		}

		ClusterNo getDir(){
			return dirIndex;
		}

		ClusterNo bvClustersSize(){
			return part->getNumOfClusters()/ClusterSize + part->getNumOfClusters() % ClusterSize ? 1 : 0;//if number of clusters exceeds clustersize
		}

		static char toName(char partCounter){
			return 'A' + partCounter;
		}

		static char toNumber(char partName){
			return partName - 'A';
		}

		static getCounter(){
			return this->partCounter;
		}
		
		static incCounter(){
			return this->partCounter++;
		}
	};
	HashTable<PartInterface*> pt(alphabetSize);
	char *buffer;

public:
	char mountPart(Partition *part){
		if(ht.fillRatio() == 1)
			return '0'
		PartInterface* pi = new PartInterface(part);
		ht.insertKey(PartInterface::getCounter(), pi);
		return PartInterface::toName(PartInterface::incCounter());
	}

	bool checkMount(char part){
		if(ht.findKey(PartInterface::toNumber(part)))
			return 1;
		return 0;
	}

	char unMountPart(char part){
		ht.deleteKey(PartInterface::toNumber(part));
	}


	PartInterface* operator [](char part){
		PartInterface* pi = pt.findKey(PartInterface::toNumber(part));
		if(pi == 0)
			return nullptr; //Exception
		return pi;
	}
};

class KernelFS{
	protected:
		KernelFS();
	private:
		friend class FS;

		struct PartitionInterface{
			Partition* part;
			ClusterNo dirIndex;
		};
		
		HANDLE sem;

		PartitionInterface partInter[alphabetSize];
		PartNum partCounter;
		char *buffer;
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

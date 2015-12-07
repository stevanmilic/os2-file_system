//File: partwrapper.h
#ifndef _partwrapper_h_
#define _partwrapper_h_
#include "part.h"
#include "partinter.h"
#include "hashtable.h"

class PartWrapper{
	//TO DO:
	//ima hash tabelu koja sadrzi trenutno montirane(mount) particije
	//moze da se doda,uzme particija, direktiva oko dirIndexa(prosledj se partName)
	//moze da se upisuje, da se cita sa particije ->memset, memcpy -> buffer
	//prosledjuse fpath, vrati ime particije
	//vraca broj potreban za bitvektor klastere... dirIndex...
	//vraca prvu prazni klaster za otvoreni fajl,ili za nest sto treba fajl-u?
	//da se koristi partitionIntreface i od kernelfile.h :)

	HashTable<PartInterface*> pt(alphabetSize);
	char partName = 'A';
	char *buffer;

	static PartWrapper* onlySample;
	static partwrapper* sample();
	

public:

	PartWrapper* sample(){
		if(onlySample == nullptr)
			onlySample = new PartWrapper();
		return onlySample;
	}

	char mountPart(Partition *part){
		if(ht.fillRatio() == 1)
			return '0'
		PartInterface* pi = new PartInterface(part);
		ht.insertKey(PartInterface::toNumber(partName), pi);
		return partName++;
	}

	bool checkMount(char part){
		if(ht.findKey(PartInterface::toNumber(part)))
			return 1;
		return 0;
	}

	char unMountPart(char part){
		ht.deleteKey(PartInterface::toNumber(part));
	}

	void formatPart(char part){

	}

	PartInterface* operator [](char part){
		PartInterface* pi = pt.findKey(PartInterface::toNumber(part));
		if(pi == 0)
			return nullptr; //Exception
		return pi;
	}
};
#endif

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
	//prosledjuse fpath, vrati ime particije
	//vraca prvu prazni klaster za otvoreni fajl,ili za nest sto treba fajl-u?
	//moze da se upisuje, da se cita sa particije ->memset, memcpy -> buffer
	//-->> trebalo bi napravati cache koji dovlaci klastere sa particije i cuva ih tako
	//da lako i efikasno moze da se pristupa podacima, nije lose imati cache i zai
	//data klastere i za indexne klastere(ne moraju svi indexi, niti svi podaci za fajl)
	//velicina cache je zavisna od velicine fajla, formula?
	//svaki kes za jedan PartInterface? ili samo jedan univerzalni u partWrapper-u?

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

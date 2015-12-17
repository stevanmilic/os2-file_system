#include "index.h"
#include "iterator.h"

IndexAlloc::IndexAlloc(PartWrapper* pw,EntryNum entry){
	this->pw = pw;
	this->entry = entry;
	index = new ClusterNo*[secondLevel + 1];
	index[firstLevel] = new ClusterNo[numOfIndex];
	index[secondLevel] = new ClusterNo[numOfIndex];
}

IndexAlloc::~IndexAlloc(){
	delete index[firstLevel];
	delete index[secondLevel];
	delete [] index;
}

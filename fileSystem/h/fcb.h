//File: fcb.h
#ifndef _fcb_h_
#define _fcb_h_
#include "fs.h"

typedef unsigned long ID;

class FCB{
	EntryNum entryNum;
	char part;
	char mode;//write,read,append

	static ID posID;//default 0
	ID id = posID++;

	public:
		FCB(char part, EntryNum entryNum, char mode){
			this->entry = entry;
			this->part = part;
			this->entryNum = entryNum;
			this->mode = mode;
		}

		ID getID(){
			return id;
		}

		static char* parseName(char *fpath);//get name of the file from absolute path
		static char* parseExt(char *fpath);//get ext of the file from absolute path

};
#endif

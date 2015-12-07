//File: fcb.h
#ifndef _fcb_h_
#define _fcb_h_

typedef unsigned long ID;

class FCB{
	Entry entry;
	EntryNum entryNum;
	char part;
	char mode;//write,read,append

	static ID posId;//default 0
	ID id = posID++;

	public:
		FCB(Entry entry, char part,EntryNum entryNum, char mode){
			this->entry = entry;
			this->part = part;
			this->entryNum = entryNum;
			this->mode = mode;
		}

		ID getID(){
			return id;
		}

		void setNameAndExt(char *fpath){
			//get file name and ext, from absolute path(fpath)
			entry->name = /*...*/;
			entry->ext = /*...*/;
		}

		char* getExt(){
			return entry->ext;
		}

		char* getName(){
			return entry->name;
		}

};
#endif

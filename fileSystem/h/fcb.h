//File: fcb.h
#ifndef _fcb_h_
#define _fcb_h_
//#include "rw.h"
#include <cstring>

typedef unsigned long EntryNum;

class FCB;

class FCBid{
	friend class FCB;
	EntryNum entry;
	char part;
	char mode;
public:

	FCBid(){
		entry = 0;
		part = -1;
	}

	FCBid(EntryNum entry,char part,char mode){
		this->entry = entry;
		this->part = part;
		this->mode = mode;
	}

	char getMode(){
		return mode;
	}

	operator int() const;

	friend bool operator==(const FCBid& lhs, const FCBid& rhs);
	friend bool operator!=(const FCBid& lhs, const FCBid& rhs);
};

class FCB{
	FCBid id;
	//ReadersWriters rw;
	unsigned long filesOp = 0;

public:
	FCB(FCBid id){
		this->id = id;
	}

	void startMode(char mode);

	void closeMode(char mode);

	unsigned long fileOpened(){
		return filesOp;
	}

	FCBid getID(){
		return id;
	}

	EntryNum getEntry(){
		return id.entry;
	}

	char getPart(){
		return id.part;
	}

	static void parseName(char *fpath, char *name);
	static void parseExt(char *fpath,char *ext);
};
#endif

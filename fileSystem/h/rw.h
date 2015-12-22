#ifndef _rw_h_
#define _rw_h_
#include "windows.h"

class ReadersWriters{
	CRITICAL_SECTION csWriter,csReader;
	HANDLE readClear;
	int readcount = 0;

public:

ReadersWriters();
~ReadersWriters();

void startWrite();
void stopWrite();
void startRead();
void stopRead();
};

#endif

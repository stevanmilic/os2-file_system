#include "rw.h"

ReadersWriters::ReadersWriters() {
	InitializeCriticalSection(&csWriter);
	InitializeCriticalSection(&csReader);
	readClear = CreateEvent(NULL, TRUE, TRUE, NULL);
}

ReadersWriters::~ReadersWriters() {
	WaitForSingleObject(readClear, INFINITE);
	CloseHandle(readClear);
	DeleteCriticalSection(&csWriter);
	DeleteCriticalSection(&csReader);
}
void ReadersWriters::startWrite() {
	EnterCriticalSection(&csWriter);
	WaitForSingleObject(readClear, INFINITE);
}

void ReadersWriters::stopWrite() {
	LeaveCriticalSection(&csWriter);
}

void ReadersWriters::startRead() {
	EnterCriticalSection(&csWriter);
	EnterCriticalSection(&csReader);
	if (++readcount == 1)
		ResetEvent(readClear);
	LeaveCriticalSection(&csReader);
	LeaveCriticalSection(&csWriter);
}

void ReadersWriters::stopRead() {
	EnterCriticalSection(&csReader);
	if (--readcount == 0)
		SetEvent(readClear);
	LeaveCriticalSection(&csReader);
}

//File: mutex.h
#include "windows.h"

#define signal(x) ReleaseSemaphore(x,1,NULL)
#define wait(x) WaitForSingleObject(x,INFINITE)

class Mutex{
	public:
		Mutex(HANDLE*);
		~Mutex();
	private:
		HANDLE *sem;
};

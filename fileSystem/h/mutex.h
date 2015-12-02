//File: mutex.h
#include "windows.h"

class Mutex{
	public:
		Mutex(HANDLE*);
		~Mutex();
	private:
		HANDLE *sem;
};

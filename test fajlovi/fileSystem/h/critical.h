//File: critical.h
#ifndef _critical_h_
#define _critical_h_
#include "windows.h"

class CriticalSection{
	public:
		CriticalSection(CRITICAL_SECTION*);
		~CriticalSection();
	private:
		CRITICAL_SECTION* cs;
};
#endif

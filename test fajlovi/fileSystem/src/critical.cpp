//File : critical.cpp
#include "critical.h"

CriticalSection::CriticalSection(CRITICAL_SECTION* s) : cs(s){
	EnterCriticalSection(cs);
}

CriticalSection::~CriticalSection(){
	LeaveCriticalSection(cs);
}

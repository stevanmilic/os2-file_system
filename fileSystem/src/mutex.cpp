//File : mutex.cpp
#include "mutex.h"

Mutex::Mutex(Handle* s) : sem(s){
	if(sem){
		wait(sem);
	}
}

Mutex::~Mutex(){
	if(sem){
		signal(sem);
	}
}

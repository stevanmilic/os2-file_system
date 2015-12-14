#include <iostream>
#include <cstring>
using namespace std;

typedef unsigned long BytesCnt;
typedef unsigned long EntryNum;
const unsigned long ENTRYCNT=64;
const unsigned int FNAMELEN=8;
const unsigned int FEXTLEN=3;
const unsigned long ClusterSize = 2048;

struct Entry {
	char name[FNAMELEN];
	char ext[FEXTLEN];
	char reserved;
	unsigned long indexCluster;
	unsigned long size;
};

typedef Entry Directory[ENTRYCNT];

class A{
	Directory dir;
public:
	Directory* getDir(){
		return &dir;
	}

	void setDir(char *buffer){
		memcpy(dir,buffer,ClusterSize);
	}

	void setbuffer(char *buffer){
		memcpy(buffer,dir,ClusterSize);
	}

	void print(){
		cout << dir[0].name << endl;
	}
};

int main(void){
	A a;
	char *buffer = new char[ClusterSize];
	memset(buffer,0,ClusterSize);
	a.setDir(buffer);	
	Directory* myDir = a.getDir();
	for(EntryNum i = 0; i < ENTRYCNT; i++)
		if(myDir[i]->name[0] == '\0'){
			strcpy(myDir[i]->name,"stevan\0" );
			strcpy(myDir[i]->ext,"dat\0" );
			myDir[i]->indexCluster = 0;
			myDir[i]->size = 0;
			break;
		}
	a.setbuffer(buffer);
	//----------------------------
	a.setDir(buffer);
	a.print();
}

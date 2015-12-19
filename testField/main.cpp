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
	Directory& getDir(){
		return dir;
	}

	void setDir(char *buffer){
		memcpy(dir,buffer,sizeof dir);
	}

	void setbuffer(char *buffer){
		memcpy(buffer,dir,sizeof dir);
	}

	void print(){
		cout << dir[0].name << endl;
		cout << dir[0].ext << endl;
	}
};

void parseName(char *fpath,char*name){
	char p_len = 3;
	char *pch = strchr(fpath,'.');
	strncpy(name,fpath + p_len,pch - fpath - p_len);
}

void parseExt(char *fpath,char*ext){
	char *pch = strchr(fpath,'.');
	strncpy(ext,fpath + (pch -fpath + 1),3);
}

int main(void){
	A a;
	char *buffer = new char[ClusterSize];
	memset(buffer,0,ClusterSize);
	a.setDir(buffer);	
	char filepath[]="1:\\fajl1.dat";
	Directory& myDir = a.getDir();
	for(EntryNum i = 0; i < ENTRYCNT; i++)
		if(myDir[i].name[0] == '\0'){
			parseName(filepath,myDir[i].name);
			parseExt(filepath,myDir[i].ext);
			myDir[i].indexCluster = 0;
			myDir[i].size = 0;
			break;
		}
	char* name = new char[FNAMELEN];
	parseName(filepath,name);
	for(EntryNum j = 0;j < ENTRYCNT; j++)
		if(myDir[j].name[0] != '\0' && strcmp(myDir[j].name,name) == 0)
			cout << " WUHU " << endl;
		
	a.setbuffer(buffer);
	//----------------------------
	a.setDir(buffer);
	a.print();
}

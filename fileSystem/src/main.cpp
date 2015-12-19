#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>
#include "fs.h"
#include "file.h"
#include "part.h"

#include<fstream>
#include<cstdlib>
#include<windows.h>
#include<ctime>
using namespace std;
int main(void){
	int ulazSize;
	char* ulazBuffer;
	{
		FILE *f=fopen("ulaz.dat","rb");
		if(f==0){
			cout<<"GRESKA: Nije nadjen ulazni fajl 'ulaz.dat' u os domacinu!"<<endl;
			system("PAUSE");
			return 0;//exit program
		}
		ulazBuffer=new char[32*1024*1024];//32MB
		ulazSize=fread(ulazBuffer, 1, 32*1024*1024, f);
		fclose(f);
	}
	Partition *partition1 = new Partition("p1.ini");
	char p1=FS::mount(partition1);
	FS::format(p1);
	char filepath[]="1:\\fajl1.dat";
	filepath[0]=p1;
	File *f=FS::open(filepath,'w');
	f->write(ulazSize,ulazBuffer);
	f->seek(0);
	char *toWr=new char[ulazSize+1];
	ofstream fout("izlaz2.dat", ios::out | ios::binary);
	f->read(0x7FFFFFFF,toWr);//namerno veci broj od velicine fajla
	fout.write(toWr, f->getFileSize());
	delete [] toWr;
	fout.close();
	delete f;
	return 0;
}


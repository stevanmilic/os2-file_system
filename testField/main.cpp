#include <iostream>
#include "bitvector.h"
using namespace std;


int main(void){
	char *buffer = new char[2048];
	BitVector *bitVector = new BitVector(buffer);
	(*bitVector)[0] = true;
	(*bitVector)[1] = true;
	(*bitVector)[256] = true;
	if((*bitVector)[256])
		cout << "Bit vektor je kul!" << endl;
	if((*bitVector)[13] != true)
		cout << "I dalje je kul" << endl;
}

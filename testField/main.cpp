#include <iostream>
using namespace std;

struct A{
	char name[8];
};

int main(void){
	A a;
	if(a.name[0] == '\0')
		cout << "Aham";
	else
		cout << "Nope";
}

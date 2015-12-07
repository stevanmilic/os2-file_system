#ifndef _quadratichashing_h_
#define _quadratichashing_h_

#include "addressfunction.h"

class QuadraticHashing : public AddressFunction{
	//For m = 2n, a good choice for the constants are c1 = c2 = 1/2, as the values of h(k,i) for i in [0,m-1] are all distinct.
	//For prime m > 2, most choices of c1 and c2 will make h(k,i) distinct for i in [0, (m-1)/2]. Such choices include c1 = c2 = 1/2, c1 = c2 = 1, and c1 = 0, c2 = 1
  double c1, c2;

public:
  QuadraticHashing(int n){
		if(n !=1 && !(n & (n -1)))
			c1 = c2 = 0.5;
		else{
			c1 = 1;
			c2 = 0;
		}
	}

  Address getAddress(Key k, Address a, Attempt i) const override{
    return a + (int)(c1*i) + (int)(c2*(i*i));
  } 
};
#endif

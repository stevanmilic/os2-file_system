#ifndef _addressfunction_h_
#define _addressfunction_h_

#include <iostream>
using namespace std;
using Address = int;
using Key = int;
using Attempt = int;
class AddressFunction{
public:
  virtual ~AddressFunction() {}

  virtual Address getAddress(Key k, Address a, Attempt i) const = 0;
}; 
#endif

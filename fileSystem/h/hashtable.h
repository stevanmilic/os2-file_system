#ifndef _hashtable_h_
#define _hashtable_h_

#include "quadratichashing.h"
#include <iostream>
using namespace std;
using Key = int;
enum State{Empty, Deleted, Full};

template <class T> class HashTable{
  struct Elem{
    Key k;
    T dat;
    State a = Empty;
    Elem() {}
    Elem(Key k, T dat,State a){
      this->k = k;
      this->dat = dat;
      this->a = a;
    } 
		Elem(Elem* newElem){
			Elem(newElem->k, newElem->dat, newElem->a);
		}
  }; 
  Elem **table;
	//For m = 2^n, a good choice for the constants are c1 = c2 = 1/2, as the values of h(k,i) for i in [0,m-1] are all distinct.
  int n;
  int keysInserted = 0;
  AddressFunction *address;

  void write(ostream& it) const;
  void copy(const HashTable& ht);
  void move(HashTable& ht);
  void deleteTable();
  int h(Key k);
  
public:
  HashTable(int n){
    this->n = n;
    table = new Elem*[n];
    for(int i = 0;i < n;i++)
      table[i] = new Elem;
		address = new QuadraticHashing(n);
  } 

  ~HashTable(){
    deleteTable();
  } 

  HashTable(const HashTable& ht){
    copy(ht);
  } 

  HashTable(HashTable&& ht){
    move(ht);
  } 

  HashTable& operator=(const HashTable& ht){
    if(this != &ht){
      deleteTable();
      copy(ht);
    } 
    return *this;
  } 

  HashTable& operator=(HashTable&& ht){
    if(this != &ht){
      deleteTable();
      move(ht);
    } 
    return *this;
  } 
  
  T findKey(Key);
  int insertKey(Key, T);
  bool deleteKey(Key);
  void clear();
  int keyCount() const;
  int tableSize() const;
	double fillRatio() const;
  friend ostream& operator<<(ostream& it, const HashTable& ht){
    ht.write(it);
    return it;
  } 
};

#include "addressfunction.h"

template <class T> void HashTable<T>::write(ostream& it) const{
  for(int i = 0; i < n;i++)
    if(table[i]->a == Full)
      it << table[i]->k << " -> " << table[i]->s << endl;
    else
      it << "EMPTY" << endl;
} 

template <class T> void HashTable<T>::copy(const HashTable<T>& ht){
  n = ht.n;
  table = new Elem*[n];
  for(int i = 0;i < n;i++)
    table[i] = new Elem(ht.table[i]);
} 

template <class T> void HashTable<T>::move(HashTable<T>& ht){
  table = ht.table;
  n = ht.n;
  ht.table = nullptr;
} 

template <class T> void HashTable<T>::deleteTable(){
  for(int i = 0; i < n; i++)
    delete table[i];
  delete [] table;
  table = nullptr;
} 

template <class T> int HashTable<T>::h(Key k){
  return k % n;
} 

template <class T> T HashTable<T>::findKey(Key k){
  int i = 0, j, t = 0;
  int a = h(k);
  do{
    j = address->getAddress(k, a, i);
    if(j >= n){
      i = 0;
      a = h(j);
      j = address->getAddress(k, a, i);
    } 
    if(table[j]->k == k){
      return table[j]->dat;
    } 
    else{
      i++;
      t++;
    } 
  }while((table[j]->a != Empty || table[j]->a == Deleted) && t != n);
  return 0;
} 

template <class T> Key HashTable<T>::insertKey(Key k, T d){
  int i = 0, j, t = 0;
  int a = h(k);
  do{
    j = address->getAddress(k, a, i);
    if(j >= n){
      i = 0;
      a = h(j);
      j = address->getAddress(k, a, i);
    } 
    if(table[j]->a == Empty){
      keysInserted++;
      table[j] = new Elem(k, d, Full);
      return 1;
    } 
    else if(table[j]->k == k)
      return -1;
    else{
      t++;
      i++;
    } 
  }while(t != n);
  return 0;
} 

template <class T> bool HashTable<T>::deleteKey(Key k){
  int i = 0, j, t = 0;
  int a = h(k);
  do{
    j = address->getAddress(k, a, i);
    if(j >= n){
      i = 0;
      a = h(j);
      j = address->getAddress(k, a, i);
    } 
    if(table[j]->k == k){
      table[j]->a = Deleted;
      return 1;
    } 
    else{
      i++;
      t++;
    } 
  }while((table[j]->a != Empty || table[j]->a != Deleted) && t != n);
  return 0;
} 

template <class T> void HashTable<T>::clear(){
  for(int i = 0; i < n; i++)
    table[i]->a = Empty;
} 

template <class T> int HashTable<T>::keyCount() const{
  return keysInserted;
} 

template <class T> int HashTable<T>::tableSize() const{
  return n;
} 

template <class T> double HashTable<T>::fillRatio() const{
  return (double)keysInserted/n;
} 
#endif

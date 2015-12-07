/*#include "hashtable.h"
#include "addressfunction.h"

void HashTable::write(ostream& it) const{
  for(int i = 0; i < n;i++)
    if(table[i]->a == Full)
      it << table[i]->k << " -> " << table[i]->s << endl;
    else
      it << "EMPTY" << endl;
} 

void HashTable::copy(const HashTable& ht){
  n = ht.n;
  table = new Elem*[n];
  for(int i = 0;i < n;i++)
    table[i] = new Elem(ht.table[i]);
} 

void HashTable::move(HashTable& ht){
  table = ht.table;
  n = ht.n;
  ht.table = nullptr;
} 

void HashTable::deleteTable(){
  for(int i = 0; i < n; i++)
    delete table[i];
  delete [] table;
  table = nullptr;
} 

int HashTable::h(Key k){
  return k % n;
} 

string* HashTable::findKey(Key k){
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
      return &table[j]->s;
    } 
    else{
      i++;
      t++;
    } 
  }while((table[j]->a != Empty || table[j]->a == Deleted) && t != n);
  return nullptr;
} 

bool HashTable::insertKey(Key k, string s){
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
      table[j] = new Elem(k, s, Full);
      return 1;
    } 
    else if(table[j]->k == k)
      return 0;
    else{
      t++;
      i++;
    } 
  }while(t != n);
  return 0;
} 

bool HashTable::deleteKey(Key k){
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

void HashTable::clear(){
  for(int i = 0; i < n; i++)
    table[i]->a = Empty;
} 

int HashTable::keyCount() const{
  return keysInserted;
} 

int HashTable::tableSize() const{
  return n;
} 

double HashTable::fillRatio() const{
  return (double)keysInserted/n;
} */

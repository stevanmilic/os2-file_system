#ifndef _hashtable_h_
#define _hashtable_h_

#include "quadratichashing.h"
enum State{Empty, Deleted, Full};

template <class T,class K> class HashTable{
  using Key = K;
  struct Elem{
    Key k;
    T dat = nullptr;
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

	
	void deleteData() {
		if (dat != nullptr) {
			delete dat;
			dat = nullptr;
		}
	}

	~Elem() {
		deleteData();
	}
  }; 
  Elem **table;
	//Good choice for n is, n = 2^m, or n is a prime number
  int n;
  int keysInserted = 0;
  AddressFunction *address;

  void write(ostream& it) const;
  void copy(const HashTable& ht);
  void move(HashTable& ht);
  int h(int k);
  
public:
  HashTable(int n){
    this->n = n;
    table = new Elem*[n];
    for(int i = 0;i < n;i++)
      table[i] = new Elem;
	address = new QuadraticHashing(n);
  } 

  void deleteTable();

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

template <class T,class K> void HashTable<T,K>::write(ostream& it) const{
  for(int i = 0; i < n;i++)
    if(table[i]->a == Full)
      it << table[i]->k << " -> " << table[i]->s << endl;
    else
      it << "EMPTY" << endl;
} 

template <class T,class K> void HashTable<T,K>::copy(const HashTable<T,K>& ht){
  n = ht.n;
  table = new Elem*[n];
  for(int i = 0;i < n;i++)
    table[i] = new Elem(ht.table[i]);
} 

template <class T,class K> void HashTable<T,K>::move(HashTable<T,K>& ht){
  table = ht.table;
  n = ht.n;
  ht.table = nullptr;
} 

template <class T,class K> void HashTable<T,K>::deleteTable(){
	if (table != nullptr) {
		for (int i = 0; i < n; i++)
			delete table[i];
		delete[] table;
		table = nullptr;
	}
} 

template <class T,class K> int HashTable<T,K>::h(int k){
  return k % n;
} 

template <class T,class K> T HashTable<T,K>::findKey(Key k){
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

template <class T,class K> Key HashTable<T,K>::insertKey(Key k, T d){
  int i = 0, j, t = 0;
  int a = h(k);
  do{
    j = address->getAddress(k, a, i);
    if(j >= n){
      i = 0;
      a = h(j);
      j = address->getAddress(k, a, i);
    } 
    if(table[j]->a == Empty || table[j]->a == Deleted){//deleted novo!
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

template <class T,class K> bool HashTable<T,K>::deleteKey(Key k){
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
	  keysInserted--;
      //table[j]->a = Deleted;
	  table[j]->a = Empty;
	  table[j]->deleteData();
      return 1;
    } 
    else{
      i++;
      t++;
    } 
  }while((table[j]->a != Empty || table[j]->a != Deleted) && t != n);
  return 0;
} 

template <class T,class K> void HashTable<T,K>::clear(){
  for(int i = 0; i < n; i++)
    table[i]->a = Empty;
} 

template <class T,class K> int HashTable<T,K>::keyCount() const{
  return keysInserted;
} 

template <class T,class K> int HashTable<T,K>::tableSize() const{
  return n;
} 

template <class T,class K> double HashTable<T,K>::fillRatio() const{
  return (double)keysInserted/n;
} 
#endif

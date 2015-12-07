#include <iostream>
#include <random>
#include <time.h>
#include <string>
#include <fstream>
#include "hashtable.h"
using namespace std;

/*void tabOfStatisticData(HashTable& ht, Key *k, int n, int numSearch){
  ht.resetStatistics();
  int min,max;
  string x = "x";
  if(k[0] >= k[1]){
    min = k[1];
    max = k[0];
  } 
  else{
    min = k[0];
    max = k[1];
  } 
  for(int i = 0;i < n;i++){
    if(k[i] > max)
      max = k[i];
    if(k[i] < min)
      min = k[i];
    ht.insertKey(k[i], x);
  } 
  cout << ht;
  for(int i = 0;i < numSearch;i++)
    ht.findKey((rand() % (max - min)) + min);
  cout << "Statistics: " << "\nSuccess Rate: " << ht.avgAccessSuccess() << "\nUnSuccess Rate: " << ht.avgAccessUnsuccess() << "\nEstimated Unsuccess Rate: " << ht.avgAccessUnsuccessEst() << endl;
} 

int main(){
  int k;
  srand(time(NULL));
  cout << "Set table size: ";
  cin >> k;
  HashTable ht(k);
  while(true){
    cout << "\nMenu:\n1.Find Key\n2.Insert Key\n3.Delete Key\n4.Avarage Access Success\n5.Average Access Unsuccess\n6.Average Access Unsuccess Estimated\n7.Reset Statistics\n8.Clear Table\n9.Get number of keys inserted\n10.Get Table Size\n11.Get Fill Ratio\n12.Get Statistic Data\n13.Print Table\n14.Quit"<<endl;
    int choice, *keys, num;
    string s, *sp;
    ifstream dat;
    cin >> choice;
    switch(choice){
      case 1:
        cout << "Enter key you want to search: ";
        cin >> k;
        sp = ht.findKey(k);
        if(sp)
          cout << "Key found!\nValue: " << *sp;
        else
          cout << "Key not found!";
        break;
      case 2:
        cout << "\n1.Insert from standard input\n2.Insert from file"<<endl;
        cin >> choice;
        switch(choice){
          case 1:
            cout << "Insert Key and Value: ";
            cin >> k >> s;
            if(ht.insertKey(k, s))
              cout << "Key inserted!";
            else
              cout << "Key not inserted!";
            break;
         case 2:
            cout << "Insert name of file: ";
            cin >> s;
            dat.open(s);
            while(dat >> s >> k)
              ht.insertKey(k, s);
            break;
        } 
        break;
      case 3:
        cout << "Enter key you want to delete: ";
        cin >> k;
        if(ht.deleteKey(k))
          cout << "Key deleted!";
        else
          cout << "Key not deleted!";
        break;
      case 4:
        cout << "Value: " << ht.avgAccessSuccess();
        break;
      case 5:
        cout << "Value: " << ht.avgAccessUnsuccess();
        break;
      case 6:
        cout << "Value: " << ht.avgAccessUnsuccessEst();
        break;
      case 7:
        ht.resetStatistics();
        cout << "Statistics reseted!";
        break;
      case 8:
        ht.clear();
        cout << "Hash-Table cleared!";
        break;
      case 9:
        cout << "Value: " << ht.keyCount();
        break;
      case 10:
        cout << "Value: " << ht.tableSize();
        break;
      case 11:
        cout << "Value: " << ht.fillRatio();
        break;
      case 12:
        tabOfStatisticData(ht, keys, k, num);
        break;
      case 13:
        cout << ht;
        break;
      case 14:
        cout << "Exiting Program..."<<endl;
        return 0;
        break;
    } 
  } 
} */

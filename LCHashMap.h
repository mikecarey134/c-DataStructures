#ifndef LCHashMap_H
#define LCHashMap_H

//////////////////////////////////
//LCHashMapMap.h                //
//By:Michael Carey && Dan Brown //
//////////////////////////////////

#include<vector>
#include<math.h>//for sqrt()
#include<functional>
#include <iostream>

template<class K, class V, class Hashfn, class Equator = std::equal_to<K> >

class LCHashMap
{
 public:

  LCHashMap(int capacity = INITIAL_CAP);
  LCHashMap(const LCHashMap<K,V,Hashfn,Equator>& other);
  ~LCHashMap() {}

  //HashEntry types
  enum EntryType{ACTIVE,EMPTY,DELETED};

  //public functions
  bool in       (const K& key)                 { return isActive(findPos(key)); }
  bool insert   (const K& key, const V& value);
  bool erase    (const K& key);
  void clear    ();
  int  size     ()                             { return currentSize_; }
  bool empty    ()                             { return currentSize_ == 0; }
  
  //operators
  V&                             operator[] (const K& key);
  const V&                       operator[] (const K& key)const;
  LCHashMap<K,V,Hashfn,Equator>& operator=  (const LCHashMap<K,V,Hashfn,Equator>& other);

 private:

  //private struct HashEntry
  //contains our data &&  key assiciated
  //and its current state 
  struct HashEntry
  {
    K key;
    V element;
    EntryType info;
    HashEntry(const K& k = K(), const V& e = V(), EntryType i = LCHashMap::EMPTY):
        key(k), element(e), info(i){}
  };

 private:

  std::vector<HashEntry> table_;//our hash table
  size_t capacity_;
  size_t currentSize_;
  Hashfn hashfn_;
  Equator equals_;
  const static double lambda_ = 0.5;
  const static int INITIAL_CAP = 101;

  //private functions
  bool    isActive (int pos)const;
  int     findPos  (const K& key)const;
  int     nextPrime(int num);
  bool    isPrime  (int num)const;
  void    rehash   ();
  size_t  hash     (const K& object)const;

};

template<class K, class V, class Hashfn, class Equator>
LCHashMap<K,V,Hashfn,Equator>::LCHashMap(int capacity): 
  capacity_(capacity), currentSize_(0)
{
  //If user inputs non prime starting capacity, 
  //resize it with nextPrime
  if(!isPrime(capacity_))
    capacity_ = nextPrime(capacity_);

  table_.resize(capacity_);//resize our vector to the initial capacity
  clear();                 //setup all the .info values in our table to EMPTY
}

template<class K, class V, class Hashfn, class Equator>
int LCHashMap<K,V,Hashfn,Equator>::nextPrime(int num)
{
  if(num % 2 == 0)//its even so make it odd
  {
    ++num;
  }

  while(!isPrime(num))//keep adding to the number until it becomes prime
  {
    num += 2;
  }

  return num;
}

template<class K, class V, class Hashfn, class Equator>
bool LCHashMap<K,V,Hashfn,Equator>::isPrime(int num)const
{
  //check all nums
  int sq_rt = sqrt(num);
  int fact = 2;
  while(fact <= sq_rt && num % fact !=0) { ++fact; }

  return fact > sq_rt;
  //return true if the computed val is prime

}

template<class K, class V, class Hashfn, class Equator>
bool LCHashMap<K,V,Hashfn,Equator>::isActive(int pos)const
{
  return table_[pos].info == ACTIVE; //return if the current position in our table is active
}

//quadratic findpos
template<class K, class V, class Hashfn, class Equator>
int LCHashMap<K,V,Hashfn,Equator>::findPos(const K& key) const
{
    int keyHash = hashfn_(key);
    int index = keyHash % capacity_;
    int collisions = 0;

    while(table_[index].info != EMPTY 
	    && !(equals_(table_[index].key, key)))//while we are in an empty slot 
    {                                            //and the current index's key also does not match 
      ++collisions;      
      index = (keyHash + (collisions * collisions)) % capacity_;
    }
    return index;
}

template<class K, class V, class Hashfn, class Equator>
bool LCHashMap<K,V,Hashfn,Equator>::insert(const K& key, const V& value)
{
    bool retValue = false;
    int currentPos = findPos(key);//find where the key passed in should go

    if(!isActive(currentPos))//if that space is active 
      {                      //add the key and associated vals
         table_[currentPos].key = key;
         table_[currentPos].element = value;
         table_[currentPos].info = ACTIVE;
         ++currentSize_;
         retValue = true;
    }
    //if our current amount of entries
    //divided by the capacity  > than lambda
    //then rehash our table
    if( (double)currentSize_ / (double)capacity_ > lambda_){
       rehash();
    }

    return retValue;
}

template<class K, class V, class Hashfn, class Equator>
bool LCHashMap<K,V,Hashfn,Equator>::erase(const K& key)
{
  int curPos = findPos(key);//find where our item should be
  bool retval = false;

  if(isActive(curPos))         
  {                               //if the item is there
    table_[curPos].info = DELETED;//mark item as deleted
    retval = true;
  }

  return retval;

}

template<class K, class V, class Hashfn, class Equator>
void LCHashMap<K,V,Hashfn,Equator>::rehash()
{
  //reset the size before copying
  currentSize_ = 0;

  std::vector<HashEntry> oldTable = table_;

  //resize our table to its next logical prime
  capacity_ = nextPrime(capacity_*2);
  table_.resize(capacity_);

  //set our internal tables entries to EMPTY
  for (int i = 0; i < capacity_; ++i)
    table_[i].info = EMPTY;

  //re-insert all the old values
  for (int i = 0; i < oldTable.size(); ++i)
  {
    if (oldTable[i].info == ACTIVE)
      insert(oldTable[i].key, oldTable[i].element);
  }

}

template<class K, class V, class Hashfn, class Equator>
void LCHashMap<K,V,Hashfn,Equator>::clear()
{
    //reset our table to a table filled
    //with empty entries
    HashEntry emptyEntry;
    for (int i = 0; i < capacity_; ++i)
        table_[i] = emptyEntry;
    
    currentSize_ = 0;//set the size back to 0
}

//operator[]
template<class K, class V, class Hashfn, class Equator>
V& LCHashMap<K,V,Hashfn,Equator>::operator[] (const K& key)
{
    int index = findPos(key);
    return table_[index].element;
}

//const operator[]

template<class K, class V, class Hashfn, class Equator>
const V& LCHashMap<K,V,Hashfn,Equator>::operator[] (const K& key)const
{
    int index = findPos(key);
    return table_[index].element;
}


//copy constructor
template<class K, class V, class Hashfn, class Equator>
LCHashMap<K,V,Hashfn,Equator>::LCHashMap(const LCHashMap<K,V,Hashfn,Equator>& other):
  table_(other.table_), capacity_(other.capacity_),currentSize_(other.currentSize_){}

//operator=
template<class K, class V, class Hashfn, class Equator>
LCHashMap<K,V,Hashfn,Equator>& LCHashMap<K,V,Hashfn,Equator>::operator=(const LCHashMap<K,V,Hashfn,Equator>& other)
{
  table_ = other.table_;
  capacity_ = other.capacity_; 
  currentSize_ = other.currentSize_; 

  return *this;
}

#endif//LCHashMap_H

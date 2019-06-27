/**********************************************************
    LCVector.h
        vector class used to store items and compare them

        by: Mike Carey && Dan Brown

***********************************************************/

#ifndef LCVECTOR_H_INCLUDED
#define LCVECTOR_H_INCLUDED

#include <cstdlib>
#include <iostream>
#include <exception>

template <typename T>
class LCVector
{

  enum{INITIAL_SIZE = 16};//initial capicity for our vector
public:

  class OutOfRangeExp : public std::exception//out of range exception to handle out of range calls
  {
    virtual const char* what() const throw()
    {
      return "Vector Subscript Out Of Range";
    }
  };


 LCVector();
 LCVector(const LCVector& other);
 ~LCVector();

 void             push_back   (const T& thingToAdd);
 void             pop_back    ();
 void             insertAt    (const T& thingToAdd, size_t index);
 void             removeAt    (size_t index);
 void             clear       ();
 void             sort        ();
 size_t           size        ()const{return size_;}
 size_t           capacity    ()const{return capacity_;}
 bool             empty       ()const{return size_ == 0;}
 bool             find        (const T& item) const;

 //operators
 T&               operator[]  (size_t index);
 const T&         operator[]  (size_t index)const;
 const LCVector&  operator=   (const LCVector& other);
 bool             operator==  (const LCVector& other);


 private:

 void doubleCapicity();//private in order to protect function call

 T* contents_;
 size_t size_;
 size_t capacity_;
 OutOfRangeExp _exp;
};



template <typename T>
LCVector<T>::LCVector():size_(0),capacity_(INITIAL_SIZE)
{
  contents_ = new T[INITIAL_SIZE];
}

template <typename T>
LCVector<T>::LCVector(const LCVector& other):size_(other.size()),capacity_(other.capacity())
{

  contents_ = new T[capacity_];

  for(int i = 0; i < size_; i++)
    {
      contents_[i] = other[i];

    }

}

template <typename T>
LCVector<T>::~LCVector()
{
  delete[] contents_;

}

template <typename T>
void LCVector<T>::push_back(const T& thingToAdd)
{

  if (size_ == capacity_)
    {
      doubleCapicity();
    }

  contents_[size_]= thingToAdd;
  size_++;

}

template <typename T>
void LCVector<T>::pop_back()
{
  if (size_ > 0)
    size_--;
}

template <typename T>
void LCVector<T>::clear()
{
  size_ = 0;//set the size to zero
  //makes it so we can just overwrite the old data

}


template <typename T>
void LCVector<T>::doubleCapicity()
{
  capacity_ *= 2;                 //make the capicity double

  T* temp = new T[capacity_];     //create a temp array

  for (int i = 0; i< size_;++i)
    {
      temp[i] = contents_[i];     //copy the old memory
    }
  delete [] contents_;            //delete the old memory

  contents_ = temp;               //point to the new memory

}

template <typename T>
T& LCVector<T>::operator[](size_t index)//non const operator[]
{
  if (index < 0 || index >= size_) //if outside the valid bounds of <o or > size_
      throw _exp;//throw out of range exception

  return contents_[index];//otherwise return the content of said index
}

template <typename T>
const T& LCVector<T>::operator[](size_t index)const//const operator[]
{
  if  (index < 0 || index >= size_) //if outside the valid bounds of <o or > size_
      throw _exp;//throw out of range exception

  return contents_[index];//otherwise return the content of said index
}


template <typename T>
const LCVector<T>& LCVector<T>::operator=(const LCVector<T>& other)
{
  size_     = other.size();       //set private members
  capacity_ = other.capacity();

  delete[]contents_;              //delete the array of data

  contents_ = new T[capacity_];   //allocate new memory

  for(int i = 0; i < size_ ;i++)
    {
      contents_[i] = other[i];    //copy the others memory
    }

  return *this;                   //return a pointer to the lhs

}

  template <typename T>
  bool LCVector<T>::operator==(const LCVector<T>& other)
  {
    int checkSum = 0;

	int i = 0;
	while (i < size_ && i < other.size())
    {
      if (contents_[i] == other[i])
	  {
		++checkSum;//increment checksum only if the contents of both arrays are ==
	  }
      else
		i = size_;//breaks out of the for loop if contents are not the same

	  ++i;
    }

    return (checkSum == other.size() && checkSum == size_); //if the checksum == the other vectors size it means all the content and the size are equal
    //thus == will return true

  }

template <typename T>
void LCVector<T>::insertAt(const T& thingToAdd, size_t index)
{
	if (index < 0 || index <= size_)
	{
		if (size_ == capacity_)
			doubleCapicity();

		T * temp = new T [capacity_];
		for (int i = 0; i < index; i++)
		{
		  temp[i] = contents_[i];
		}
		temp[index] = thingToAdd;
	
		for (int i = index+1; i <= size_; ++ i)
		{
		  temp[i]=contents_[i-1];
		}
		++size_;

		delete [] contents_;
		contents_ = temp;
	}
}


template <typename T>
void LCVector<T>::removeAt(size_t index)
{
  if (size_ > 0)
  {
      for (int i = index; i < size_; ++ i)
        contents_[i] = contents_[i+1];

      --size_;
  }
}

template <typename T>
void LCVector<T>::sort()
{
  T smallest;
  T tmp;
  int swapIndex;
  for (int i = 0; i < size_; ++i)
    {
      smallest = contents_[i];
      for (int j = i; j < size_; ++j)
	{
	  if (contents_[j] < smallest)	//Find the next smallest thing
	    {
	      smallest = contents_[j];
	      swapIndex = j;
	    }
	}
      tmp = contents_[i];			//Swap whatever is in the next spot in the contents
      contents_[i] = smallest;	//with the next smallest thing
      contents_[swapIndex] = tmp;
    }
}

template <typename T>
bool LCVector<T>::find(const T& item) const
{
    int i = 0;
    while(contents_[i] != item && i < size_)
        ++i;

    return (contents_[i] == item && size_ > 0);
}
#endif // LCVECTOR_H_INCLUDED

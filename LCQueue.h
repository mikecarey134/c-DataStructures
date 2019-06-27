/*****************************************************
    LCQueue.h
    purpose: queue implementation for card encryption
    by: Michael Carey && Dan Brown

*****************************************************/

#ifndef LCQUEUE_H
#define LCQUEUE_H

#include <exception>

template <typename T>
class LCQueue
{

public:

    class RangeExp : public std::exception
    {                                  //out of range exception to handle out of range calls
     public:
        virtual const char* what() const throw()
        {
            return "Queue Subscript Out Of Range";
        }
    };

	LCQueue(int capacity = INITIAL_CAPACITY):
        theSize_(0),capacity_(capacity),first_(capacity_/INITIAL_CAPACITY),last_(first_-1) { data_ = new T[capacity_]; }

	LCQueue(const LCQueue& other);

	~LCQueue(){delete [] data_;}

	void    push      (const T& item);
	T       pop       ();
	T       front     ();
	bool    empty     () { return theSize_ == 0; }

	const size_t& size        () { return theSize_; }
	const size_t& getCapacity () { return capacity_; }

	//operators
	const LCQueue& operator=(const LCQueue& other);

private:
    void doubleCap ();

	// default capacity, if none is specified
	const static int INITIAL_CAPACITY = 2;
	size_t theSize_;
	size_t capacity_;
	size_t first_;
	size_t last_;
	T* data_;
};

template<typename T>
LCQueue<T>::LCQueue(const LCQueue& other):theSize_(other.theSize_),
 capacity_(other.capacity_), first_(other.first_), last_(other.last_),data_(0)
{
    T* tmp = new T[other.capacity_];//make a tmp array to copy other's items

    size_t i = first_;

    while (i != last_)
    {
        if (i == capacity_)
            i = 0;
        tmp[i] = other.data_[i];

        ++i;
        if (i == capacity_)
            i = 0;
    }

    tmp[last_] = other.data_[last_];
    delete [] data_;//delete old data
    data_ = tmp;//point to new memory
}

template <typename T>
void LCQueue<T>::push(const T& item)
{
    if(theSize_ == capacity_)
        doubleCap();

    last_++;
    if(last_ == capacity_)
        last_ = 0;

    data_[last_] = item;
    theSize_++;
}

template <typename T>
T LCQueue<T>::pop()
{
    if (theSize_ == 0)
    {
        RangeExp exp_;
        throw exp_;
    }

    theSize_--;
    int index = first_;   //if first is equal to capacity
    first_++;

    if (first_ == capacity_)//reset to 0 and keep track of the
        first_ = 0;         //correct index to return for when this happens

    return data_[index];
}

template <typename T>
T LCQueue<T>::front()//dispay the first item in the vector
{
    if(theSize_ > 0)
        return data_[first_];
    else
    {
        RangeExp exp_;
        throw exp_;
    }
}

template <typename T>
void LCQueue<T>::doubleCap()
{
    T* tmp = new T[capacity_* INITIAL_CAPACITY];

    size_t i = first_;
    for (; i < capacity_; ++i)
    {
        tmp[i] = data_[i];
    }

    for (size_t j = 0; j < first_; ++j)
    {
        tmp[i] = data_[j];
        ++i;
    }

    delete [] data_;
    data_ = tmp;
    last_ = first_+capacity_-1;
    capacity_ *= INITIAL_CAPACITY;
}


template<typename T>
const LCQueue<T>& LCQueue<T>::operator=(const LCQueue<T>& other)
{
    capacity_ = other.capacity_;
    theSize_ = other.theSize_;
    first_ = other.first_;
    last_ = other.last_;

    T* tmp = new T[capacity_];//make a tmp array to copy other's items

    size_t i = first_;
    while (i <= last_)
    {
        if (i == capacity_)
            i = 0;
        tmp[i] = other.data_[i];

        ++i;
    }
    delete [] data_;//delete old data

    data_ = tmp;//point to new memory

    return *this;
}



#endif

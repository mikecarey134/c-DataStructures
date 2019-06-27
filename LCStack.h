/*****************************************************
    LCStack.h
    purpose: stack implementation for card encryption
    by: Michael Carey && Dan Brown

*****************************************************/

#ifndef LCSTACK_H_INCLUDED
#define LCSTACK_H_INCLUDED

template<typename T>
class LCStack
{

private:
    struct node
    {
        node(const T& d = T(),node* n = NULL):data(d),next(n){}
        T data;
        node* next;
    };

    void      erase (node* theNode);//in private so it cannot be called outside the class
    void      copy  (node* theNode)
    {
        if (theNode!= NULL)
        {
            push(theNode->data);
            copy(theNode->next);
        }
    }
    node* phead;
    size_t size_;

public:
    LCStack         ():phead(NULL),size_(0) {}
    LCStack         (const LCStack& other);

    virtual~LCStack ();

    class RangeExp : public std::exception
    {                                  //out of range exception to handle out of range calls
    public:
        virtual const char* what() const throw()
        {
            return "Stack Subscript Out Of Range";
        }
    };

    void      push      (const T& item);
    T         pop       ();
    const T&  top       ();
    bool      empty     ()               {return phead == NULL;}

    LCStack   operator= (const LCStack& other);
};


template<typename T>
LCStack<T>::LCStack (const LCStack<T>& other): phead(0),size_(other.size_)
{
    copy(other.phead);
}

template <typename T>
LCStack<T> LCStack<T>::operator= (const LCStack<T>& other)
{
    phead = 0;
    size_ = other.size_;

    copy(other.phead);

    return *this;
}

template<typename T>
void LCStack<T>::erase(node* theNode)
{
    if(theNode!=NULL)
    {
        erase(theNode->next);
        delete theNode;
    }
}

template<typename T>
LCStack<T>::~LCStack()
{
    erase(phead);
}

template<typename T>
void LCStack<T>::push(const T& item)
{
    node* temp = new node(item, phead);
    phead = temp;

    ++size_;
}

template<typename T>
const T& LCStack<T>::top()
{
    if (empty())
    {
        RangeExp exp_;
        throw exp_;
    }

    return phead->data;
}

template<typename T>
T LCStack<T>::pop()
{
    if (empty())
    {
        RangeExp exp_;
        throw exp_;
    }

    T retval = phead->data;

    node* tmp = phead;
    phead = phead->next;
    delete tmp;

    --size_;

  return retval;
}


#endif // LCSTACK_H_INCLUDED

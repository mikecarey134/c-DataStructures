/*
LCMap.h
By: Mike Carey && Dan Brown
Purpose: Provide the map Data Structure for the game "Wheel Of Lies"

*/


#ifndef LCMAP_H_INCLUDED
#define LCMAP_H_INCLUDED

#include<functional>//for std::less
#include<list>
#include<exception>

template<class K,class V,class Comparator = std::less<K> >//defaulted comparator is std::less
class LCMap
{
private:

  struct BNode//node class that stores the value associated with a key
    {          //as well as the node's left and right children
        K key_;
        V value_;
        BNode* left_;
        BNode* right_;
        //constructor
        BNode(K key = K(), V value = V(), BNode* l=NULL, BNode* r=NULL):
            key_(key),value_(value),left_(l),right_(r)
            {
            }
    };


public:
    LCMap(Comparator c = Comparator()):
        root_(NULL), islessThan_(c), size_(-1)
        {
        }

    LCMap(const LCMap<K,V,Comparator>& other);
    virtual~LCMap() { deleteAllChildren(root_); }


    bool insert (const K &key,const V &value);
    bool erase  (const K& key);


    void          clear     ();
    std::list<K>  keys      ();
    bool          in        (const K& key);
    bool          empty     () const        { return root_ == NULL; }

    BNode* findMin(BNode* node)const;

    //operators
    V& operator[] (const K& k);
    const V& operator[] (const K& k) const;

    LCMap<K,V,Comparator>& operator= (const LCMap<K,V,Comparator>&);


private:

//Data Members
    BNode* root_;
    Comparator islessThan_;
    int size_;

//Recusive helper functions
    void   deleteAllChildren (BNode*& node);
    BNode* erase         (BNode*& node,const K& key);
    void   keysListMaker (std::list<K>& keysList, BNode* node);
    void   removeNode    (BNode*& node);
    //BNode* insert        (BNode*& node, const K &key, const V& value);
    bool   in            (BNode* node, const K& key);
    BNode* clone         (BNode* node);
    BNode* bracketHelper (const K& key, BNode* node);
    BNode* lookup        (const K& key, BNode* node);
};

template<class K,class V,class Comparator>
bool LCMap<K,V,Comparator>::in(const K& key)//use the recursive in to return true if a particular key is in the tree
{
    return in(root_, key);
}

template<class K,class V,class Comparator>
bool LCMap<K,V,Comparator>::in(BNode* node, const K& key)
{
    bool retval = true;

    if(node == NULL)//base case
        retval = false;
    else if(islessThan_(key,node->key_))
        retval = in(node->left_,key);//go left
    else if(islessThan_(node->key_,key))
        retval = in(node->right_,key);//go right

    return retval;//return our extracted boolean
}

template<class K,class V,class Comparator>
std::list<K> LCMap<K,V,Comparator>::keys()
{
    std::list<K> keysList;
    if (root_ != NULL)
        keysListMaker(keysList, root_);//push back the root
                                        //push all its children
    return keysList;
}

template<class K,class V,class Comparator>
void LCMap<K,V,Comparator>::keysListMaker(std::list<K>& keysList, BNode* node)
{
    if (node != NULL)
    {
        keysListMaker(keysList, node->left_);
        keysList.push_back(node->key_);
        keysListMaker(keysList, node->right_);
    }
}
/*
template<class K,class V,class Comparator>
void LCMap<K,V,Comparator>::keysListMaker(std::list<K>& keysList, BNode* leftNode, BNode* rightNode)
{
    if (leftNode != NULL)
    {
        keysList.push_back(leftNode->key_);
        keysListMaker(keysList, leftNode->left_, leftNode->right_);
    }
    if (rightNode != NULL)
    {
        keysList.push_back(rightNode->key_);
        keysListMaker(keysList, rightNode->left_, rightNode->right_);
    }
}
*/
template<class K,class V,class Comparator>
LCMap<K,V,Comparator>::LCMap(const LCMap<K,V,Comparator>& other):
    root_(NULL), islessThan_(other.islessThan_)
{
    root_ = clone(other.root_);//copy all from other's root using recursive clone helper
    size_ = other.size_;
}

template<class K,class V,class Comparator>
typename LCMap<K,V,Comparator>::BNode* LCMap<K,V,Comparator>::clone(BNode* node)
{
    BNode* newNode = NULL;//if a node doesnt have a right or left this will stay null
    if (node != NULL)
    {
        newNode = new BNode(node->key_, node->value_, clone(node->left_),
            clone(node->right_));
    }
    return newNode;
}

template<class K,class V,class Comparator>
bool LCMap<K,V,Comparator>::insert(const K &key,const V &value)
{
    BNode* insertCusor = lookup(root_, key, value);

    bool isInserted = (insertCusor != NULL);
    if (!isInserted)
    {
        insertCusor = new BNode(key, value, NULL, NULL);
        ++size_;
    }
    return !isInserted;
    /*
    if (!inTree)//if the value is not in the tree
    {
        insert(root_, key, value);//we put the value into our tree
        ++size_;
    }
    return !inTree;
    */
}
/*
template<class K,class V,class Comparator>
typename LCMap<K,V,Comparator>::BNode* LCMap<K,V,Comparator>::insert(BNode*& node, const K &key, const V& value)
{
    if(islessThan_(key,node->key_))
        node = insert(node->left_, key, value);
    else if(islessThan_(node->key_,key))
        node = insert(node->right_, key, value);

    return node;
}
*/
template<class K,class V,class Comparator>
bool LCMap<K,V,Comparator>::erase(const K& key)
{/*
    bool inTree = in(key); //Return value used to remember if we went to remove something
    if (inTree)
    {
        erase(root_,key);
        --size_;
    }
    return inTree;
    */
    BNode* tmp = erase(root_, key);
    bool inTree = (tmp != NULL);
    if (inTree)
    {
        removeNode(tmp);
        --size_;
    }
    return inTree;

}

template<class K,class V,class Comparator>
typename LCMap<K,V,Comparator>::BNode* LCMap<K,V,Comparator>::erase(BNode*& node, const K& key)
{/*
    if (islessThan_(key, node->key_))       //Looks for the key we want to remove
        node = erase(node->left_, key);     //and returns a node pointer to it
    else if (islessThan_(node->key_, key))  //
        node = erase(node->right_, key);
        */
    node = lookup(node, key);

    if (node->right_ != NULL)
    {
        BNode* tmp = findMin(node->right_);
        node->key_ = tmp->key_;
        node->value_ = tmp->value_;

        node = erase(node->right_, node->key_);
    }
    return node;
}

template<class K,class V,class Comparator>
typename LCMap<K,V,Comparator>::BNode* lookup (const K& key, BNode* node)
{
    if(islessThan_(key,node->key_))
        node = insert(node->left_, key, value);
    else if(islessThan_(node->key_,key))
        node = insert(node->right_, key, value);

    return node;
}

template<class K,class V,class Comparator>
void LCMap<K,V,Comparator>::clear()
{
    deleteAllChildren(root_);
    size_ = 0;
}

template<class K,class V,class Comparator>
void LCMap<K,V,Comparator>::deleteAllChildren(BNode*& node)
{
    if (node != NULL)
    {
        deleteAllChildren(node->left_);
        deleteAllChildren(node->right_);
        delete node;
    }
    node = NULL;
}

template<class K,class V,class Comparator>
void LCMap<K,V,Comparator>::removeNode(BNode*& node)
{
    if(node!=NULL)
        delete node;

    node = NULL;
}

template<class K,class V,class Comparator>
typename LCMap<K,V,Comparator>::BNode* LCMap<K,V,Comparator>::findMin(BNode* node) const
{
    if(node != NULL && node->left_ != NULL)
        node = findMin(node->left_);

    return node;
}

template<class K,class V,class Comparator>
V& LCMap<K,V,Comparator>::operator[](const K& key)
{/*
    if (!in(key))
        insert(key, V());

    BNode* node = root_;

    if (islessThan_(node->key_, key))
        node = bracketHelper(key, node->right_);
    else if (islessThan_(key, node->key_))
        node = bracketHelper(key, node->left_);

    return node->value_;
    */

    BNode* retNode = bracketHelper(key, root_);
    if (retNode == NULL)
    {
        retNode = new BNode(key, V(), NULL, NULL);
        ++size_;
    }

    return retNode->value_;
}

template<class K,class V,class Comparator>
const V& LCMap<K,V,Comparator>::operator[](const K& key) const
{
/*
    if (!in(key))
        insert(key, V());

    BNode* node = root_;

    if (islessThan_(node->key_, key))
        node = bracketHelper(key, node->right_);
    else if (islessThan_(key, node->key_))
        node = bracketHelper(key, node->left_);

    return node->value_;
    */
    BNode* retNode = bracketHelper(key, root_);
    if (retNode == NULL)
    {
        retNode = new BNode(key, V(), NULL, NULL);
        ++size_;
    }
    return retNode->value_;
}

template<class K,class V,class Comparator>
typename LCMap<K,V,Comparator>::BNode* LCMap<K,V,Comparator>::bracketHelper(const K& key, BNode* node)
{
    if (islessThan_(node->key_, key))
        node = bracketHelper(key, node->right_);
    else if (islessThan_(key, node->key_))
        node = bracketHelper(key, node->left_);

    return node;
}

template<class K,class V,class Comparator>
LCMap<K,V,Comparator>& LCMap<K,V,Comparator>::operator=(const LCMap<K,V,Comparator>& other)
{
    root_ = clone(other.root_);
    islessThan_ = other.islessThan_;

    return *this;
}

#endif // LCMAP_H_INCLUDED

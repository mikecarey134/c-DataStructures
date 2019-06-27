/****************************
*       SpellChecker        *
*            by             *
* Daniel Brown & Mike Carey *
*****************************/

#include "Trie.h"

using namespace std;

Trie::Trie()
{
    root_= new TrieNode();
}

Trie::~Trie()
{
     eraseTrieNodeAndChildren(root_);
}

void Trie::insert(string word)
{
    TrieNode* node = root_;
    string wordRecord;

    //Go down the Trie, creating new nodes as necessary until word is inserted
    for (size_t i = 0; i < word.length(); ++i)
    {
        wordRecord += word[i];

        if (node->children_.count(word[i]) == 0)
        {
            TrieNode* tmp = new TrieNode(word[i]);
            node->children_[word[i]] = tmp;
        }
        node = node->children_[word[i]];
    }

    //Set the position the node gets to as having a completed word
    node->completeWord_ = (word == wordRecord);
}

//Traverses the Trie until it finds desired word,
//Returns NULL if string is not found
Trie::TrieNode* Trie::find(string word)
{
    TrieNode* retNode = root_;

    //If we are not looking for root
    if (word[0] != rootChar_)
    {
        int stringPos = 0;
        string searchString;
        while (!retNode->children_.empty() && retNode->children_.count(word[stringPos]) > 0)
        {
            retNode = retNode->children_[word[stringPos]];
            searchString += word[stringPos];
            ++stringPos;
        }
        if (searchString != word)
            retNode = NULL;
    }

    return retNode;
}

//Will return a set of a given string's children
//Using set because iterator goes through in sorted order automatically
set<string> Trie::getStringChildren(string word)
{
    TrieNode* node = find(word);
    set<string> words;

	if (node != NULL)
	{
	    //Add each of the node's children to the set
		for (map<char,TrieNode*>::iterator it = node->children_.begin();
			 it != node->children_.end();
			 ++it)
			 {
				string addWord;
				if (word == getRootCharAsString()) //If we are getting root's children
                    addWord = it->first;            //ignore its char, otherwise put together
                else                                //the passed in word and the child char
                    addWord = word + it->first;     //then insert that
				words.insert(addWord);
			 }
	}
    return words;
}

//Returns the node's variable saying whether or not it is a real word
bool Trie::isCompletedWord(std::string word)
{
    TrieNode* node = find(word);

    return (node != NULL && node->completeWord_);
}

//Recursive helper for destructor
void Trie::eraseTrieNodeAndChildren(TrieNode* node)
{
    //Deletes all of a node's children if any, then deletes the node
    if(!node->children_.empty())
    {
        for(map<char,TrieNode*>::iterator it = node->children_.begin();
            it != node->children_.end();
            ++it)
            {
                eraseTrieNodeAndChildren(it->second);
            }
    }
    delete node;
}

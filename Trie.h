/****************************
*       SpellChecker        *
*            by             *
* Daniel Brown & Mike Carey *
*****************************/

#ifndef TRIE_H
#define TRIE_H

#include <map>
#include <string>
#include <set>

class Trie
{
private:

    //TrieNode stores the edge character, if the word is an actual word,
    //and a map of all its children
    struct TrieNode
    {
        TrieNode(char edge = rootChar_, bool completed = false): edge_(edge), completeWord_(completed)
        {
        }

        char edge_;
        bool completeWord_;
        std::map<char,TrieNode*> children_;
    };

public:
    Trie();
    virtual ~Trie();

    void insert(std::string word);
    std::set<std::string> getStringChildren(std::string word);
    bool isCompletedWord(std::string word);
    const char getRootChar() { return rootChar_; }
    const std::string getRootCharAsString() { std::string root; root += rootChar_; return root; }

private:
    TrieNode* root_;
    static const char rootChar_ = '_';

    TrieNode* find(std::string word);
    void eraseTrieNodeAndChildren(TrieNode* node);
};

#endif // TRIE_H

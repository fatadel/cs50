// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// include the dictionary's funcionality declaring header file
#include "dictionary.h"
// include the declaration of the trie node
#include "node.h"

// defines the size of a dictionary loaded
// we don't want to count that number each time we invoke size()
// that's why shall we make it global
// count it on load (set to zero on unload)
// and just give this value when size() is invoked
unsigned int dictionarySize = 0;
// remember the trie root globally, so that in each function we know
// where to start traversing from
node *root = NULL;

// the functions that freeds the trie node
void freed(node *n);
// the functions that initializes the trie node
// "initializes" means we make every children to point to NULL, so we don't have any garbage values,
// and setting isWord to false, so we don't accidently have true
// the function is used when a node is created
void initializeNode(node *n);

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // TODO
    // crete a node to traverse the trie
    node *trav = root;
    // iterate over each character in a word from text
    for (int i = 0; word[i] != '\0'; i++)
    {
        // "unifying" a letter to lowercase
        char letter = tolower(word[i]);
        // count the letter's position in the children array of a node
        // the zeroth is the apostrophe, the rest are the letters according to their position in the real alphabet
        int pos = (letter == '\'') ? letter - '\'' : letter - 'a' + 1;
        // check if particular letter points somewhere not null
        // else there is no this word in the dictionary
        if (trav->children[pos] == NULL)
        {
            return false;
        }
        // if a letter points to another node, proceed to that node
        trav = trav->children[pos];
    }
    // when we reached the last node, check if the path we made composes a word
    if (trav->isWord)
    {
        return true;
    }
    return false;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // TODO
    // create a file pointer to read the dictionary
    FILE *dict = fopen(dictionary, "r");
    // check for errors
    if (!dict)
    {
        return false;
    }
    // a string to store words from dictionary
    // size is equal to maximum length of a possible word + '\0'
    char *word = malloc(LENGTH + 1);
    // check for errors
    if (!word)
    {
        return false;
    }
    // create the trie root node
    root = malloc(sizeof(node));
    // check for errors
    if (!root)
    {
        return false;
    }
    // initialize the root node
    initializeNode(root);
    // scan the dictionary until end of file
    // writing each sting into the word variable
    while (fscanf(dict, "%s", word) != EOF)
    {
        // start traversing from root
        node *trav = root;
        // iterate over each letter in the word from dictinary
        for (int i = 0; word[i] != '\0'; i++)
        {
            // calculate position of a particular letter in the children array
            // the zeroth is the apostrophe, the rest are the letters according to their position in the real alphabet
            int pos = (word[i] == '\'') ? word[i] - '\'' : word[i] - 'a' + 1;
            // make a new path if there is no
            if (trav->children[pos] == NULL)
            {
                // create a new node
                node *newNode = malloc(sizeof(node));
                // check for memory errors
                if (!newNode)
                {
                    return false;
                }
                // initialize the node
                initializeNode(newNode);
                // make a path from current node to the one created now
                trav->children[pos] = newNode;
            }
            // proceed to the new node
            trav = trav->children[pos];
        }
        // now the path we made composes a word
        trav->isWord = true;
        // increase dictionary size
        dictionarySize++;
    }
    // freed the memory allocated for 'word' string
    free(word);
    // close the dictinary file
    fclose(dict);
    // succefully loaded the dictinary into our memory
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    // return the dictionary size (global variable composed on load/unload)
    return dictionarySize;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // TODO
    // freed the trie
    freed(root);
    // set to zero the dictionary size
    dictionarySize = 0;
    // return true
    return true;
    // Actually I have no idea why the staff made this function bool, since free() is a void function not bool, and the only way
    // to check if a dictionary was indeed unloaded is to use valgrind to check memory. However this fuction will always return
    // true, no matter if we succeded or not with dictionary unloading.
}

// the recursive function freeing a trie
// from the node n it goes down as possible
// frees the deepest node
// than comes back up and continues
// so when we pass the root node, all the tries becomes freed
void freed(node *n)
{
    // for every children in the node
    for (int i = 0; i < CHILDREN_SIZE; i++)
    {
        // go down to a node if possible
        if (n->children[i] != NULL)
        {
            // start over with that node
            freed(n->children[i]);
        }
    }
    // free the node when there is no way down from its children
    free(n);
}
// the function initializing a node
void initializeNode(node *n)
{
    // iterate over its children
    for (int i = 0; i < CHILDREN_SIZE; i++)
    {
        // make them point to null, so that we don't accidently point to a garbage value
        n->children[i] = NULL;
    }
    // make isWord flag false, so that we don't accidently have it true
    n->isWord = false;
}

// So this was the original implementation that passed check50 and got 1197th place in the Big Board with the following values:
// Rank  Name   Time        Load    Check   Size    Unload  Memory  Heap    Stack
// 1197 fatadel 14.872 s    6.256 s 5.056 s 0.000 s 3.560 s 82.2 MB 82.2 MB 4.7 kB

// Now let's copy this as dictionary_original.c and play around with dictionary.c to go up in the Big Board!
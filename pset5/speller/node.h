// Header file, defining a node of a trie

#include <stdbool.h>

// define the number of ways from one node
// in our case is equal to the size of the alphabet used in the dictionary (26)
// plus the apostrophe symbol
#define CHILDREN_SIZE 27

typedef struct node
{
    // the boolean showing whether the ways that led to this node compose a word
    bool isWord;
    // ways to go from one particular node
    struct node *children[CHILDREN_SIZE];
}
node;
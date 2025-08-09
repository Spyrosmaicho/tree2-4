#ifndef TREE_H_
#define TREE_H_

typedef struct t24 Tree24;
typedef int Item;
typedef int Key;

/*Function that initializes the root of the tree*/
void init();

/*Function to count the number of items in the tree*/
int count();

/*Function to insert an Item into the tree*/
void insert(Item x);

/*Function to select the k-th element in the tree*/
Item select(int k);

/*Function to search for a key in the tree*/
Item search(Key key);

/*Function to visit each node in sorted order*/
void sort(void (*visit)(Item));

/*Function that deletes a node from the tree*/
void delete(Item x);

/*Function that destroys the Tree24*/
void destroy_tree();

#endif
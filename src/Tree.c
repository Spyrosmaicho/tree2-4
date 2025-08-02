#include <stdlib.h>
#include "Tree.h"
#include <assert.h>
#include <stdbool.h>
#include <limits.h>
#include <stdio.h>

Tree24 *root = NULL; //Global variable to hold the root of the tree

struct t24 
{
    int Count;
    Tree24 *parent;
    Item items[3];
    Tree24 *children[4];
    int N[4];
};


/*Creates a new empty node*/
Tree24 *create_empty_node() 
{
    Tree24 *node = malloc(sizeof(Tree24));
    assert(node != NULL);
    node->Count = 0;
    node->parent = NULL;
    for (int i = 0; i < 4; i++) {
        node->children[i] = NULL;
        node->N[i] = 0;
    }
    return node;
}

/*Creates a new node with initial item*/
Tree24 *create_node(Item val)
 {
    Tree24 *node = create_empty_node();
    node->Count = 1;
    node->items[0] = val;
    return node;
}

/*Function that initializes the root of the tree*/
void init() 
{
   root = NULL;
}


/*Helper function to check if a node is a leaf*/
bool leaf(Tree24* node) 
{
    return node ? node->children[0] == NULL : true;
}

/*Helper recursive function for count function*/
int real_count(Tree24 *node) 
{
    if (!node) return 0;
    int cnt = node->Count;
    //add elements of all subtrees
    for (int i = 0; i <= node->Count; i++) cnt += real_count(node->children[i]); 
    return cnt;
}

/*Function to count the number of items in the tree*/
int count() 
{
    return root ? real_count(root) : 0;
}

/*Helper function to update the N array of the parent nodes*/
void update_subtree_counts(Tree24 *node) 
{
    if (!node) return;
    
    //Calculate sizes for each child subtree
    for (int i = 0; i <= node->Count; i++) node->N[i] = node->children[i] ? real_count(node->children[i]) : 0;
    
    //Propagate the changes up to the parent
    if (node->parent) update_subtree_counts(node->parent);
}

/*Helper function to split a full node*/
void split(Tree24 *node, int index) 
{
    Tree24 *full = node->children[index];
    Tree24 *right = create_node(full->items[2]); //create right sibling
    assert(right != NULL);
    
    Item mid = full->items[1]; //Middle item to promote
    right->Count = full->Count = 1;

    //For non-leaf nodes, move children
    if (!leaf(full)) 
    {
        right->children[0] = full->children[2];
        right->children[1] = full->children[3];
        //Update parent pointers
        if (right->children[0]) right->children[0]->parent = right;
        if (right->children[1]) right->children[1]->parent = right;
        //Transfer subtree counts
        right->N[0] = full->N[2];
        right->N[1] = full->N[3];
        //Clear transferred children
        full->children[2] = full->children[3] = NULL;
        full->N[2] = full->N[3] = 0;
    }

    //Make space in parent
    for (int i = node->Count; i > index; i--) 
    {
        node->items[i] = node->items[i-1];
        node->children[i+1] = node->children[i];
        node->N[i+1] = node->N[i];
    }

    // Insert promoted item
    node->items[index] = mid;
    node->children[index+1] = right;
    right->parent = node;
    node->Count++;
    
    update_subtree_counts(node); //Update the N array
}

/*Helper function to insert into a non full node*/
void insert_not_full(Tree24 *node, Item x) 
{
    int index = node->Count - 1;
    //Leaf node insertion
    if (leaf(node)) 
    {
        //Check for duplicates
        for (int i = 0; i < node->Count; i++) if (node->items[i] == x) return;
        //Find position and shift items
        while (index >= 0 && x < node->items[index]) 
        {
            node->items[index+1] = node->items[index];
            node->N[index+1] = node->N[index];
            index--;
        }
        
        //Insert new item
        node->items[index+1] = x; 
        node->N[index+1] = 0; //Leaves have no children
        node->Count++;
        update_subtree_counts(node->parent);
    } 
    //Internal node
    else 
    {
        //Find appropriate child
        while (index >= 0 && x < node->items[index]) index--;
        index++;
        
        //Handle NULL child
        if (!node->children[index]) 
        {
            node->children[index] = create_node(x);
            node->children[index]->parent = node;
            node->N[index] = 1;
            return;
        }

        //Split full children
        if (node->children[index]->Count == 3) 
        {
            split(node, index);
            if (x > node->items[index]) index++;
        }
        
        insert_not_full(node->children[index], x);
    }
}

/*Function to insert an Item into the tree*/
void insert(Item x) 
{
    if (!root) 
    {
        root = create_node(x);
        return;
    }

    //full root
    if (root->Count == 3) 
    {
        //Create new empty root
        Tree24 *new_root = create_empty_node();
        new_root->children[0] = root;
        root->parent = new_root;
        split(new_root, 0); //split old root
        root = new_root; 
    }

    insert_not_full(root, x);
}

/*Helper function to search for a key in the tree*/
Item real_search(Tree24 *node, Key key) 
{
    if (!node) return -1;

    //Check if key is in the current node
    for (int i = 0; i < node->Count; i++) 
    {
        if (node->items[i] == key) return node->items[i];
        if (key < node->items[i]) return node->children[i] ? real_search(node->children[i], key) : -1;
    }
    //Check the last child
    return node->children[node->Count] ? real_search(node->children[node->Count], key) : -1;
}

/*Function to search for a key in the tree*/
Item search(Key key) 
{
    return root ? real_search(root, key) : -1;
}

/*Helper function to select the k-th element in the tree*/
Item real_select(Tree24 *node, int key) \
{
    if (!node) return -1;

    for (int i = 0; i < node->Count; i++) 
    {
        //Calculate the total number of elements in the left subtree
        int left_size = node->children[i] ? node->N[i] : 0;
        
        //If the key is in the left subtree
        if (key <= left_size) return real_select(node->children[i], key);
        
        //Adjust the key for current node items
        key -= left_size;
        
        //If the key points to current item
        if (key == 1) return node->items[i];
        
        //Move to the space between items
        key--;
    }
    
    //Check the rightmost child
    return real_select(node->children[node->Count], key);
}

/* Function to select the k-th element in the tree */
Item select(int k) 
{
    if (!root || k < 1 || k > count()) return -1;  // Invalid cases
    return real_select(root, k);
}

/*Helper function to visit each node in sorted order*/
void real_sort(Tree24 *node, void (*visit)(Item)) 
{
    if (!node) return;

    for (int i = 0; i < node->Count; i++) 
    {
        real_sort(node->children[i], visit);
        visit(node->items[i]);
    }
    real_sort(node->children[node->Count], visit);
}

/*Function to visit each node in sorted order*/
void sort(void (*visit)(Item)) 
{
    real_sort(root, visit);
}

/*Helper function to get the predecessor of an item*/
Item predecessor(Tree24* node, int index) 
{
    //Check for validation
    if (!node || !node->children[index]) return -1;
    //Find the rightmost child
    Tree24* current = node->children[index];
    //Traverse to the rightmost child
    while (!leaf(current)) current = current->children[current->Count];
    return current->items[current->Count - 1];
}


/*Helper function to get the successor of an item*/
Item successor(Tree24* node, int index) 
{
    //Check for validation
    if (!node || !node->children[index+1]) return -1;
    //Find the leftmost child
    Tree24* current = node->children[index+1];
    //Traverse to the leftmost child
    while (!leaf(current)) current = current->children[0];
    //Return the leftmost item
    return current->items[0];
}

/*Function that removes an item from a leaf node*/
void remove_from_leaf(Tree24* node, int index) 
{
    //Validate input and check if node is actually a leaf
    if (!node || !leaf(node)) return;
    
    //Shift all items after the deleted one to the left
    for (int i = index + 1; i < node->Count; i++) node->items[i-1] = node->items[i];
    //Decrease item count
    node->Count--;
    update_subtree_counts(node->parent); //Update parent's counters
}

/*Function that borrows an item from the left sibling*/
void get_from_left(Tree24* parent, int index) 
{
    //Validate parent and index
    if (!parent || index <= 0) return;
    
    Tree24* child = parent->children[index];
    Tree24* sibling = parent->children[index-1];
    
    //Check if both children exist
    if (!child || !sibling) return;

    //Make space in child by shifting items right
    for (int i = child->Count; i > 0; i--) child->items[i] = child->items[i-1];
    //Move item from parent to child
    child->items[0] = parent->items[index-1];
    
    //If not leaf move children as well
    if (!leaf(child)) 
    {
        // Shift child pointers right
        for (int i = child->Count + 1; i > 0; i--) child->children[i] = child->children[i-1];
        //Move last child from sibling
        child->children[0] = sibling->children[sibling->Count];
        if (child->children[0]) child->children[0]->parent = child;
    }

    //Move item from sibling to parent
    parent->items[index-1] = sibling->items[sibling->Count-1];
    child->Count++;
    sibling->Count--;
    
    update_subtree_counts(parent); //Update size counters
}

/*Function that borrows an item from the right sibling*/
void get_from_right(Tree24* parent, int index) 
{
    //Validate parent and index
    if (!parent || index >= parent->Count) return;
    
    Tree24* child = parent->children[index];
    Tree24* sibling = parent->children[index+1];
    //Check if both children exist
    if (!child || !sibling) return;

    //Move item from parent to child
    child->items[child->Count] = parent->items[index];
    
    //If not leaf move children as well
    if (!leaf(child)) 
    {
        //Move first child from sibling
        child->children[child->Count+1] = sibling->children[0];
        if (child->children[child->Count+1]) child->children[child->Count+1]->parent = child;
    }

    //Move item from sibling to parent
    parent->items[index] = sibling->items[0];
    
    //Shift remaining items in sibling left
    for (int i = 1; i < sibling->Count; i++) sibling->items[i-1] = sibling->items[i];
    
    //If not leaf shift children as well
    if (!leaf(sibling)) 
    {
        for (int i = 1; i <= sibling->Count; i++) sibling->children[i-1] = sibling->children[i];
    }

    child->Count++;
    sibling->Count--;
    
    update_subtree_counts(parent); //Update size counters
}

/*Function that merges two children nodes*/
void merge(Tree24* parent, int index) 
{
    //Validate parent and index
    if (!parent || index < 0 || index >= parent->Count) return;
    
    Tree24* child = parent->children[index];
    Tree24* sibling = parent->children[index+1];
    //Check if both children exist
    if (!child || !sibling) return;

    //Move item from parent to child
    child->items[1] = parent->items[index];
    child->Count++;

    //Copy all items from sibling
    for (int i = 0; i < sibling->Count; i++) child->items[child->Count + i] = sibling->items[i];

    //If not leaf copy children as well
    if (!leaf(sibling)) 
    {
        for (int i = 0; i <= sibling->Count; i++) 
        {
            child->children[child->Count + i] = sibling->children[i];
            if (child->children[child->Count + i]) child->children[child->Count + i]->parent = child;
        }
    }

    child->Count += sibling->Count;

    //Remove the merged item from parent
    for (int i = index + 1; i < parent->Count; i++) parent->items[i-1] = parent->items[i];
    //Shift children pointers
    for (int i = index + 2; i <= parent->Count; i++) parent->children[i-1] = parent->children[i];

    parent->Count--;
    free(sibling); //Free the merged sibling
    update_subtree_counts(parent); //Update size counters
}

/*Function that fixes underflow in a node by borrowing or merging */
void fix(Tree24* node, int index) 
{
    if (!node) return;
    
    //First try to borrow from left sibling
    if (index > 0 && node->children[index-1] && node->children[index-1]->Count > 1) get_from_left(node, index);
    //Then try to borrow from right sibling
    else if (index < node->Count && node->children[index+1] && node->children[index+1]->Count > 1) get_from_right(node, index);
    //If both siblings have minimum items merge
    else 
    {
        if (index > 0) merge(node, index-1);
        else merge(node, index);
    }
}

/*Function that finds the index of an item in a node*/
int find(Tree24* node, Item item) 
{
    if (!node) return -1;
    
    //Linear search through items
    for (int i = 0; i < node->Count; i++) 
    {
        if (node->items[i] == item) return i;
        if (item < node->items[i]) return -1;
    }
    return -1;
}

/*Function that recursive deletes from a node */
void delete_from_node(Tree24* node, Item item) 
{
    if (!node) return;
    
    int index = find(node, item);
    //Item found in this node
    if (index != -1) 
    { 
        if (leaf(node)) remove_from_leaf(node, index); //Simple removal
        else 
        {
            //Replace with predecessor if left child has enough items
            if (node->children[index] && node->children[index]->Count > 1) {

                Item pred = predecessor(node, index);
                node->items[index] = pred;
                delete_from_node(node->children[index], pred);
            } 
            //Otherwise try successor from right child
            else if (node->children[index+1] && node->children[index+1]->Count > 1) \
            {
                Item succ = successor(node, index);
                node->items[index] = succ;
                delete_from_node(node->children[index+1], succ);
            } 
            //If both children have minimum items, merge them
            else 
            {
                if (node->children[index] && node->children[index+1]) 
                {
                    merge(node, index);
                    delete_from_node(node->children[index], item);
                }
            }
        }
    }
    //Item not in this node 
    else 
    {
        if (leaf(node)) return; //Item not found

        //Find appropriate child to continue search
        int child_index = 0;
        while (child_index < node->Count && item > node->items[child_index]) child_index++;

        //Ensure child has enough items before recursing
        if (child_index <= node->Count && node->children[child_index]) 
        {
            if (node->children[child_index]->Count < 2) 
            {
                fix(node, child_index); //Fix underflow
                //Recalculate child_index after fix
                child_index = 0;
                while (child_index < node->Count && item > node->items[child_index]) child_index++;
            }
            delete_from_node(node->children[child_index], item);
        }
    }
}

/*Function that deletes a node from the tree*/
void delete(Item x) 
{
    if (!root) return; //Empty tree

    delete_from_node(root, x);

    //Handle empty root case
    if (root->Count == 0) 
    {
        Tree24* tmp = root;
        if (leaf(root)) root = NULL; //Tree becomes empty
        else 
        {
            //Promote only child to root
            root = root->children[0];
            if (root) root->parent = NULL;
        }
        free(tmp);
    }
}

/*Function that recursively destroys a subtree*/
void real_destroy(Tree24 *node)
{
    if (!node) return;
    
    //First real_destroy all children
    for (int i = 0; i <= node->Count; i++) real_destroy(node->children[i]);
    
    //Then free the node itself
    free(node);
}

/*Function that destroys the Tree24*/
void destroy_tree() 
{
    real_destroy(root);
    root = NULL; //Reset root pointer
}

/*Function that validates tree structure*/
bool validate_tree(Tree24 *node) 
{
    if (!node) return true;
    
    //Check item count is within bounds
    if (node->Count < 1 || node->Count > 3) return false;
    
    //Check items are properly ordered
    for (int i = 1; i < node->Count; i++) if (node->items[i] <= node->items[i-1]) return false;
    
    //Validate all subtrees
    for (int i = 0; i <= node->Count; i++) 
    {
        if (node->children[i]) 
        {
            //Check subtree size matches counter
            if (node->N[i] != real_count(node->children[i])) return false;
            //Check parent pointer is correct
            if (node->children[i]->parent != node) return false;
            //Recursively validate subtree
            if (!validate_tree(node->children[i])) return false;
        }
        //Leaf child should have size 0
        else if (node->N[i] != 0) return false;
    }
    
    return true;
}

/*Validation helper function*/
void validate() 
{
    if (!validate_tree(root)) 
    {
        printf("Not a valid tree!\n");
        exit(1);
    }
}
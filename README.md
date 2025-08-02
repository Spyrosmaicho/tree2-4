# Tree 2-4

This assignment implements a 2-4 tree along with some basic operations based on the interface and functions described in the problem statement.

## Core Program Functions

- `insert(Item)`: Inserts nodes into the tree

- `delete(Item)`: Deletes nodes from the tree

- `search(Key)`: Searches for keys in the tree

- `init()`: Initializes the root of the tree

- `int count()`: Returns the number of elements in the tree

- `Item select(int)`: Returns the k-th smallest element of the tree

- `void sort(void (*visit)(Item))`: Visits the tree elements in ascending order

## Running Instructions
To run the program simply use the command
```sh
./run.sh
```

If compilation or execution fails initially, first run:
```sh
chmod +x run.sh
```

I created an input.txt file where you provide a value that determines how many nodes will be created for the implementation of a 2-4 tree. This is used below in a test for performance and correctness of the implemented insert functions.


## Explanation of Function Usage/Utility

1) Creation & Destruction

- `init()`: Initializes an empty tree.

Use: Called at the start to set root to NULL.

- `create_empty_node()`: Creates an empty node (without keys).

Use: Helper for insertions/splits.

- `create_node(val)`: Creates a node with initial value val.

Use: Used when inserting the very first element in the tree.

- `destroy_tree()`: Frees all tree memory.

Use: Called at the end of the program to avoid memory leaks.

2) Insertion & Deletion

- `insert(x)`: Inserts element x into the tree, maintaining balance.

Behavior: If root is full (3 keys), it splits and creates a new root.

- `split(node, index)`: Splits a full node into two smaller nodes.

Use: Important for maintaining tree balance during insertion.

- `delete(x)`: Deletes element x from the tree.

Important detail: If a node shrinks to 1 key, merge or borrow from siblings occurs.

- `fix(node, index)`: Restores balance after deletion.

Two cases: Borrow from siblings or merge nodes.

3) Search & Traversal

- `search(key)`: Returns the element with key (or -1 if not found).

Behavior: Recursively searches from root to leaves.

- `select(k)`: Returns the k-th smallest element (in ascending order).

Note: Based on array `N[]` that counts elements in each subtree.

- `sort(visit)`: Visits all elements in ascending order.

Use: For printing or processing data.

4) Helper Functions

- `leaf(node)`: Checks if node is a leaf (no children).

- `update_subtree_counts(node)`: Updates subtree sizes up the tree.

- `validate()`: Checks tree correctness (for debugging).


## Example Execution
Here is an example run of the program:
```
sh
$ cat input.txt
1000
$ ./run.sh
gcc -Wall -Wextra -g -std=c11 -c main.c -o main.o
gcc -Wall -Wextra -g -std=c11 -c Tree.c -o Tree.o
gcc -Wall -Wextra -g -std=c11 -o main main.o Tree.o
==259209== Memcheck, a memory error detector
==259209== Copyright (C) 2002-2017, and GNU GPL d, by Julian Seward et al.
==259209== Using Valgrind-3.15.0 and LibVEX; rerun with -h for copyright info
==259209== Command: ./main
==259209== 
                Tests for Tree 2-4
            ------------------------

1) Basic Operations Test

After inserting 42:
Search 42: Item Found
Count: 1

After inserting 12,34,25,51,44:
Sorted: 12 25 34 42 44 51 
Count: 6

Search test:
Search 12: Item Found
Search 34: Item Found
Search 44: Item Found

╔══════════════════════════════╗
║         Test passed!         ║
╚══════════════════════════════╝


2) Deletion Test

Initial tree (15 elements): 15 20 25 30 35 40 45 50 55 60 65 70 75 80 85 
Count: 15

After deleting 85: 15 20 25 30 35 40 45 50 55 60 65 70 75 80 
Count: 14

After deleting 30: 15 20 25 35 40 45 50 55 60 65 70 75 80 
Count: 13

After deleting 55: 15 20 25 35 40 45 50 60 65 70 75 80 
Count: 12

Attempt to delete 99: Should not crash.

╔══════════════════════════════╗
║         Test passed!         ║
╚══════════════════════════════╝


3) Selection Test

Select tests on 100 elements:
1st element: 1 (expected: 1)
50th element: 50 (expected: 50)
100th element: 100 (expected: 100)
Invalid selection (0): -1 (expected: -1)
Invalid selection (101): -1 (expected: -1)

╔══════════════════════════════╗
║         Test passed!         ║
╚══════════════════════════════╝


4) Edge Case Test

Empty tree tests:
Count: 0
Search 42: -1
Select 1: -1
Sort (should do nothing): 

Single element tree:
Count: 1
Search 42: 42
After deletion, count: 0

Duplicate insertions:
Count (should be 1): 1

╔══════════════════════════════╗
║         Test passed!         ║
╚══════════════════════════════╝


5) Stress Test

Type the number of random operations to perform: 
Inserting 1000 random numbers
Count: 773
Insert time: 0.16 seconds

Verifying sort order
Sort order: Valid

Random deletions  
Count after deletions: 500
Delete time: 0.16 seconds

╔══════════════════════════════╗
║         Test passed!         ║
╚══════════════════════════════╝


==259209== 
==259209== HEAP SUMMARY:
==259209==     in use at exit: 0 bytes in 0 blocks
==259209==   total heap usage: 595 allocs, 595 frees, 56,656 bytes allocated
==259209== 
==259209== All heap blocks were freed -- no leaks are possible
==259209== 
==259209== For lists of detected and suppressed errors, rerun with: -s
==259209== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
rm -f main.o Tree.o main
```
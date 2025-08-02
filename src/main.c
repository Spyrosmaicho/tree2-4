#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Tree.h"
#include <limits.h>
#include <string.h>

extern Tree24 *root;
//Global variables for test5
int last=-1;
int flag;


//Function that prints a message with a beautiful way
void my_print(char *str)
{
    int len = strlen(str);
    if (len > 30) str[30] = '\0'; //Truncate to 30 characters
    int length = 30; //Max length of banner
    printf("╔");
    for (int i = 0; i < length; i++) printf("═");
    printf("╗\n");

    printf("║");
    int padding = (length - (int)strlen(str)) / 2;
    for (int i = 0; i < padding; i++) printf(" ");
    printf("%s", str);
    for (int i = 0; i < padding; i++) printf(" ");
    if ((strlen(str) % 2) != 0) printf(" "); 
    printf("║\n");

    printf("╚");
    for (int i = 0; i < length; i++) printf("═");
    printf("╝\n");
}


//Helper function to print an item
void print_items(Item item) 
{
    printf("%d ", item);
}

/*Function to verify the order of items in the tree*/
/*This function checks if the items are in sorted order */
void verify_order(Item item) 
{
    if (item <= last) flag = 1;
    last = item;
}


//Test insertion and search functionality
void test1() 
{
    printf("1) Basic Operations Test\n\n");
    init();
    
    //Insert single element
    insert(42);
    printf("After inserting 42:\n");
    printf("Search 42: %s\n", search(42) == 42 ? "Item Found" : "Item Not found");
    printf("Count: %d\n", count());
    //Insert many nums
    int nums[] = {12, 34, 25, 51, 44};
    for (int i = 0; i < 5; i++) insert(nums[i]);
    printf("\nAfter inserting 12,34,25,51,44:\n");
    printf("Sorted: ");
    sort(print_items);
    printf("\nCount: %d\n", count());
    
    //Search test
    printf("\nSearch test:\n");
    //Search root
    printf("Search 12: %s\n", search(12) == 12 ? "Item Found" : "Item Not found");
    //Search internal
    printf("Search 34: %s\n", search(34) == 34 ? "Item Found" : "Item Not found");
    //Search leaf
    printf("Search 44: %s\n", search(44) == 44 ? "Item Found" : "Item Not found");
    
    printf("\n");
    destroy_tree();
    my_print("Test passed!");
    printf("\n\n");
}

//Test deletion
void test2() 
{
    printf("2) Deletion Test\n\n");
    init();

    //Build a known tree structure
    int elements[] = {50, 30, 70, 20, 40, 60, 80, 15, 25, 35, 45, 55, 65, 75, 85};
    for (int i = 0; i < 15; i++) insert(elements[i]);

    printf("Initial tree (15 elements): ");
    sort(print_items);
    printf("\nCount: %d\n", count());

    //Delete leaf from multi-key node
    delete(85);
    printf("\nAfter deleting 85: ");
    sort(print_items);
    printf("\nCount: %d\n", count());

    //Delete a internal key from a non-leaf node
    delete(30);
    printf("\nAfter deleting 30: ");
    sort(print_items);
    printf("\nCount: %d\n", count());

    //Delete a key that causes a merge
    delete(55);
    printf("\nAfter deleting 55: ");
    sort(print_items);
    printf("\nCount: %d\n", count());

    //Delete a non-existent element
    printf("\nAttempt to delete 99: ");
    delete(99);
    printf("Should not crash.\n");

    printf("\n");
    destroy_tree();
    my_print("Test passed!");
    printf("\n\n");
}

//Test selection functionality
void test3() 
{
    printf("3) Selection Test\n\n");
    init();
    

    //Insert numbers 1-100
    srand(time(NULL));
    int nums[100];
    for (int i = 0; i < 100; i++) nums[i] = i+1;

    //Swap first 50 with last 50
    //This is to test the select function
    for(int i = 0;i<50;i++)
    {
        int k = 99-i;
        nums[i] = nums[i] ^nums[k];
        nums[k] = nums[i] ^nums[k];
        nums[i] = nums[i] ^nums[k];
    }
    
    
    for (int i = 0; i < 100; i++)  insert(nums[i]);
    
    printf("Select tests on 100 elements:\n");
    printf("1st element: %d (expected: 1)\n", select(1));
    printf("50th element: %d (expected: 50)\n", select(50));
    printf("100th element: %d (expected: 100)\n", select(100));
    printf("Invalid selection (0): %d (expected: -1)\n", select(0));
    printf("Invalid selection (101): %d (expected: -1)\n", select(101));
    
    printf("\n");
    destroy_tree();
    my_print("Test passed!");
    printf("\n\n");
}

//Stress test with random operations
void test5() 
{
    printf("5) Stress Test\n\n");
    init();
    

    printf("Type the number of random operations to perform: ");
    int size;
    if(scanf("%d",&size)!=1) return;
    putchar('\n');
    printf("Inserting %d random numbers\n", size);
    
    clock_t start = clock();
    for (int i = 0; i < size; i++) insert(rand() % size);
    double insert_time = (double)(clock() - start) / CLOCKS_PER_SEC;
    
    //Count will be <= size due to duplicates
    printf("Count: %d\n", count());
    printf("Insert time: %.2f seconds\n\n", insert_time);
    
    //Verify sorted order
    int flag = 0;
    printf("Verifying sort order\n");
    sort(verify_order);
    printf("Sort order: %s\n", flag ? "Invalid" : "Valid");
    
    printf("\n");
    //Random deletions
    printf("Random deletions  \n");
    start = clock();
    for (int i = 0; i < size/2; i++) delete(rand() % size);
    double delete_time = (double)(clock() - start) / CLOCKS_PER_SEC;
    
    printf("Count after deletions: %d\n", count());
    printf("Delete time: %.2f seconds\n", delete_time);
    
    printf("\n");
    destroy_tree();
    my_print("Test passed!");
    printf("\n\n");
}

// Test edge cases
void test4() 
{
    printf("4) Edge Case Test\n\n");
    init();

    printf("Empty tree tests:\n");
    printf("Count: %d\n", count());
    printf("Search 42: %d\n", search(42));
    printf("Select 1: %d\n", select(1));
    printf("Sort (should do nothing): ");
    sort(print_items);
    printf("\n");
    
    //Insert and delete single element
    printf("\nSingle element tree:\n");
    insert(42);
    printf("Count: %d\n", count());
    printf("Search 42: %d\n", search(42));
    delete(42);
    printf("After deletion, count: %d\n", count());
    
    // Duplicate insertions
    printf("\nDuplicate insertions:\n");
    for (int i = 0; i < 5; i++) insert(10);
    printf("Count (should be 1): %d\n", count());
    
    printf("\n");
    destroy_tree();
    my_print("Test passed!");
    printf("\n\n");
}

int main() 
{
    printf("\t\tTests for Tree 2-4\n");
    printf("\t    ------------------------\n\n");    
    test1();
    test2();
    test3();
    test4();
    test5();

    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include "avl.h"

int main() {
    AVLNode *root = NULL;

    // Insert values
    root = insert(root, 10);
    root = insert(root, 20);
    root = insert(root, 30);
    root = insert(root, 40);
    root = insert(root, 50);
    root = insert(root, 25);

    // Print in-order traversal of the AVL tree
    printf("In-order traversal of the AVL tree is:\n");
    in_order(root);
    printf("\n");

    // Free the tree
    free_tree(root);

    return 0;
}
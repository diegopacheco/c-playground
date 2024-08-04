#ifndef AVL_H
#define AVL_H

#include <stdlib.h>

typedef struct AVLNode {
    int value;
    struct AVLNode *left;
    struct AVLNode *right;
    int height;
} AVLNode;

int height(AVLNode *node);
AVLNode* create_node(int value);
AVLNode* right_rotate(AVLNode *y);
AVLNode* left_rotate(AVLNode *x);
int get_balance(AVLNode *node);
AVLNode* insert(AVLNode* node, int value);
void in_order(AVLNode *root);
void free_tree(AVLNode *root);

#endif
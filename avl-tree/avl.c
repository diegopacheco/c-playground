#include <stdio.h>
#include <stdlib.h>
#include "avl.h"

int height(AVLNode *node) {
    if (node == NULL) {
        return 0;
    }
    return node->height;
}

AVLNode* create_node(int value) {
    AVLNode *node = (AVLNode *)malloc(sizeof(AVLNode));
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    node->height = 1; // New node is initially added at leaf
    return node;
}

AVLNode* right_rotate(AVLNode *y) {
    AVLNode *x = y->left;
    AVLNode *T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights
    y->height = 1 + (height(y->left) > height(y->right) ? height(y->left) : height(y->right));
    x->height = 1 + (height(x->left) > height(x->right) ? height(x->left) : height(x->right));

    // Return new root
    return x;
}

AVLNode* left_rotate(AVLNode *x) {
    AVLNode *y = x->right;
    AVLNode *T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    // Update heights
    x->height = 1 + (height(x->left) > height(x->right) ? height(x->left) : height(x->right));
    y->height = 1 + (height(y->left) > height(y->right) ? height(y->left) : height(y->right));

    // Return new root
    return y;
}

int get_balance(AVLNode *node) {
    if (node == NULL) {
        return 0;
    }
    return height(node->left) - height(node->right);
}

AVLNode* insert(AVLNode* node, int value) {
    // 1. Perform the normal BST insertion
    if (node == NULL) {
        return create_node(value);
    }

    if (value < node->value) {
        node->left = insert(node->left, value);
    } else if (value > node->value) {
        node->right = insert(node->right, value);
    } else {
        // Equal values are not allowed in BST
        return node;
    }

    // 2. Update height of this ancestor node
    node->height = 1 + (height(node->left) > height(node->right) ? height(node->left) : height(node->right));

    // 3. Get the balance factor of this ancestor node to check whether this node became unbalanced
    int balance = get_balance(node);

    // If this node becomes unbalanced, then there are 4 cases

    // Left Left Case
    if (balance > 1 && value < node->left->value) {
        return right_rotate(node);
    }

    // Right Right Case
    if (balance < -1 && value > node->right->value) {
        return left_rotate(node);
    }

    // Left Right Case
    if (balance > 1 && value > node->left->value) {
        node->left = left_rotate(node->left);
        return right_rotate(node);
    }

    // Right Left Case
    if (balance < -1 && value < node->right->value) {
        node->right = right_rotate(node->right);
        return left_rotate(node);
    }

    // Return the (unchanged) node pointer
    return node;
}

void in_order(AVLNode *root) {
    if (root != NULL) {
        in_order(root->left);
        printf("%d ", root->value);
        in_order(root->right);
    }
}

void free_tree(AVLNode *root) {
    if (root != NULL) {
        free_tree(root->left);
        free_tree(root->right);
        free(root);
    }
}

#include <stdio.h>
#include <stdlib.h>

typedef enum { RED, BLACK } Color;

typedef struct Node {
    int data;
    Color color;
    struct Node *left;
    struct Node *right;
    struct Node *parent;
} Node;

typedef struct {
    Node *root;
    Node *nil;
} RBTree;

Node* create_node(RBTree *tree, int data) {
    Node *node = malloc(sizeof(Node));
    node->data = data;
    node->color = RED;
    node->left = tree->nil;
    node->right = tree->nil;
    node->parent = tree->nil;
    return node;
}

RBTree* create_tree() {
    RBTree *tree = malloc(sizeof(RBTree));
    tree->nil = malloc(sizeof(Node));
    tree->nil->color = BLACK;
    tree->nil->left = NULL;
    tree->nil->right = NULL;
    tree->nil->parent = NULL;
    tree->root = tree->nil;
    return tree;
}

void left_rotate(RBTree *tree, Node *x) {
    Node *y = x->right;
    x->right = y->left;
    if (y->left != tree->nil) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == tree->nil) {
        tree->root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

void right_rotate(RBTree *tree, Node *x) {
    Node *y = x->left;
    x->left = y->right;
    if (y->right != tree->nil) {
        y->right->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == tree->nil) {
        tree->root = y;
    } else if (x == x->parent->right) {
        x->parent->right = y;
    } else {
        x->parent->left = y;
    }
    y->right = x;
    x->parent = y;
}

void insert_fixup(RBTree *tree, Node *z) {
    while (z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            Node *y = z->parent->parent->right;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    left_rotate(tree, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                right_rotate(tree, z->parent->parent);
            }
        } else {
            Node *y = z->parent->parent->left;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    right_rotate(tree, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                left_rotate(tree, z->parent->parent);
            }
        }
    }
    tree->root->color = BLACK;
}

void insert(RBTree *tree, int data) {
    Node *z = create_node(tree, data);
    Node *y = tree->nil;
    Node *x = tree->root;

    while (x != tree->nil) {
        y = x;
        if (z->data < x->data) {
            x = x->left;
        } else {
            x = x->right;
        }
    }

    z->parent = y;
    if (y == tree->nil) {
        tree->root = z;
    } else if (z->data < y->data) {
        y->left = z;
    } else {
        y->right = z;
    }

    z->left = tree->nil;
    z->right = tree->nil;
    z->color = RED;

    insert_fixup(tree, z);
}

Node* search(RBTree *tree, int data) {
    Node *current = tree->root;
    while (current != tree->nil) {
        if (data == current->data) {
            return current;
        } else if (data < current->data) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    return tree->nil;
}

void inorder(RBTree *tree, Node *node) {
    if (node != tree->nil) {
        inorder(tree, node->left);
        printf("%d(%s) ", node->data, node->color == RED ? "R" : "B");
        inorder(tree, node->right);
    }
}

void print_tree_helper(RBTree *tree, Node *node, int space) {
    if (node == tree->nil) {
        return;
    }

    space += 10;
    print_tree_helper(tree, node->right, space);

    printf("\n");
    for (int i = 10; i < space; i++) {
        printf(" ");
    }
    printf("%d(%s)\n", node->data, node->color == RED ? "R" : "B");

    print_tree_helper(tree, node->left, space);
}

void print_tree(RBTree *tree) {
    print_tree_helper(tree, tree->root, 0);
}

int black_height(RBTree *tree, Node *node) {
    if (node == tree->nil) {
        return 0;
    }
    int left_height = black_height(tree, node->left);
    int right_height = black_height(tree, node->right);
    int add = (node->color == BLACK) ? 1 : 0;
    return left_height + add;
}

int main() {
    RBTree *tree = create_tree();

    printf("Inserting: 10, 20, 30, 15, 25, 5, 1\n\n");
    insert(tree, 10);
    insert(tree, 20);
    insert(tree, 30);
    insert(tree, 15);
    insert(tree, 25);
    insert(tree, 5);
    insert(tree, 1);

    printf("Tree structure:\n");
    print_tree(tree);

    printf("\nInorder traversal: ");
    inorder(tree, tree->root);
    printf("\n");

    printf("\nSearching for 15: ");
    Node *result = search(tree, 15);
    if (result != tree->nil) {
        printf("Found %d (color: %s)\n", result->data, result->color == RED ? "RED" : "BLACK");
    } else {
        printf("Not found\n");
    }

    printf("Searching for 100: ");
    result = search(tree, 100);
    if (result != tree->nil) {
        printf("Found %d\n", result->data);
    } else {
        printf("Not found\n");
    }

    printf("\nBlack height: %d\n", black_height(tree, tree->root));

    return 0;
}

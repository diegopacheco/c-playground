#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_LEVEL 16
#define P 0.5

typedef struct Node {
    int key;
    int value;
    struct Node **forward;
} Node;

typedef struct {
    int level;
    Node *header;
} SkipList;

int random_level() {
    int lvl = 1;
    while ((float)rand() / RAND_MAX < P && lvl < MAX_LEVEL) {
        lvl++;
    }
    return lvl;
}

Node* create_node(int key, int value, int level) {
    Node *node = malloc(sizeof(Node));
    node->key = key;
    node->value = value;
    node->forward = malloc(sizeof(Node*) * (level + 1));
    for (int i = 0; i <= level; i++) {
        node->forward[i] = NULL;
    }
    return node;
}

SkipList* create_skiplist() {
    SkipList *list = malloc(sizeof(SkipList));
    list->level = 0;
    list->header = create_node(-1, -1, MAX_LEVEL);
    return list;
}

void insert(SkipList *list, int key, int value) {
    Node *update[MAX_LEVEL + 1];
    Node *current = list->header;

    for (int i = list->level; i >= 0; i--) {
        while (current->forward[i] != NULL && current->forward[i]->key < key) {
            current = current->forward[i];
        }
        update[i] = current;
    }

    current = current->forward[0];

    if (current == NULL || current->key != key) {
        int new_level = random_level();

        if (new_level > list->level) {
            for (int i = list->level + 1; i <= new_level; i++) {
                update[i] = list->header;
            }
            list->level = new_level;
        }

        Node *new_node = create_node(key, value, new_level);

        for (int i = 0; i <= new_level; i++) {
            new_node->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = new_node;
        }
    } else {
        current->value = value;
    }
}

int search(SkipList *list, int key, int *value) {
    Node *current = list->header;

    for (int i = list->level; i >= 0; i--) {
        while (current->forward[i] != NULL && current->forward[i]->key < key) {
            current = current->forward[i];
        }
    }

    current = current->forward[0];

    if (current != NULL && current->key == key) {
        *value = current->value;
        return 1;
    }
    return 0;
}

void delete(SkipList *list, int key) {
    Node *update[MAX_LEVEL + 1];
    Node *current = list->header;

    for (int i = list->level; i >= 0; i--) {
        while (current->forward[i] != NULL && current->forward[i]->key < key) {
            current = current->forward[i];
        }
        update[i] = current;
    }

    current = current->forward[0];

    if (current != NULL && current->key == key) {
        for (int i = 0; i <= list->level; i++) {
            if (update[i]->forward[i] != current) {
                break;
            }
            update[i]->forward[i] = current->forward[i];
        }

        free(current->forward);
        free(current);

        while (list->level > 0 && list->header->forward[list->level] == NULL) {
            list->level--;
        }
    }
}

void print_list(SkipList *list) {
    printf("Skip List (level %d):\n", list->level);
    for (int i = list->level; i >= 0; i--) {
        Node *node = list->header->forward[i];
        printf("Level %d: ", i);
        while (node != NULL) {
            printf("%d:%d ", node->key, node->value);
            node = node->forward[i];
        }
        printf("\n");
    }
}

void free_skiplist(SkipList *list) {
    Node *current = list->header->forward[0];
    while (current != NULL) {
        Node *temp = current;
        current = current->forward[0];
        free(temp->forward);
        free(temp);
    }
    free(list->header->forward);
    free(list->header);
    free(list);
}

int main() {
    srand(time(NULL));

    SkipList *list = create_skiplist();

    printf("Inserting elements...\n");
    insert(list, 3, 300);
    insert(list, 6, 600);
    insert(list, 7, 700);
    insert(list, 9, 900);
    insert(list, 12, 1200);
    insert(list, 19, 1900);
    insert(list, 17, 1700);
    insert(list, 26, 2600);
    insert(list, 21, 2100);
    insert(list, 25, 2500);

    print_list(list);

    int value;
    printf("\nSearching for key 19: ");
    if (search(list, 19, &value)) {
        printf("Found value %d\n", value);
    } else {
        printf("Not found\n");
    }

    printf("Searching for key 15: ");
    if (search(list, 15, &value)) {
        printf("Found value %d\n", value);
    } else {
        printf("Not found\n");
    }

    printf("\nDeleting key 19...\n");
    delete(list, 19);

    print_list(list);

    printf("\nSearching for key 19 after deletion: ");
    if (search(list, 19, &value)) {
        printf("Found value %d\n", value);
    } else {
        printf("Not found\n");
    }

    free_skiplist(list);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CACHE_SIZE 5
#define HASH_SIZE 10

typedef struct Node {
    int key;
    int value;
    struct Node *prev;
    struct Node *next;
} Node;

typedef struct {
    Node *head;
    Node *tail;
    Node **hash_table;
    int capacity;
    int size;
} LRUCache;

Node* create_node(int key, int value) {
    Node *node = malloc(sizeof(Node));
    node->key = key;
    node->value = value;
    node->prev = NULL;
    node->next = NULL;
    return node;
}

LRUCache* create_cache(int capacity) {
    LRUCache *cache = malloc(sizeof(LRUCache));
    cache->capacity = capacity;
    cache->size = 0;
    cache->head = create_node(-1, -1);
    cache->tail = create_node(-1, -1);
    cache->head->next = cache->tail;
    cache->tail->prev = cache->head;
    cache->hash_table = calloc(HASH_SIZE, sizeof(Node*));
    return cache;
}

void remove_node(Node *node) {
    node->prev->next = node->next;
    node->next->prev = node->prev;
}

void add_to_head(LRUCache *cache, Node *node) {
    node->next = cache->head->next;
    node->prev = cache->head;
    cache->head->next->prev = node;
    cache->head->next = node;
}

void move_to_head(LRUCache *cache, Node *node) {
    remove_node(node);
    add_to_head(cache, node);
}

Node* remove_tail(LRUCache *cache) {
    Node *node = cache->tail->prev;
    remove_node(node);
    return node;
}

int hash(int key) {
    return abs(key) % HASH_SIZE;
}

int get(LRUCache *cache, int key) {
    int idx = hash(key);
    Node *node = cache->hash_table[idx];

    while (node != NULL) {
        if (node->key == key) {
            move_to_head(cache, node);
            return node->value;
        }
        node = node->next;
    }
    return -1;
}

void put(LRUCache *cache, int key, int value) {
    int idx = hash(key);
    Node *node = cache->hash_table[idx];
    Node *prev_node = NULL;

    while (node != NULL) {
        if (node->key == key) {
            node->value = value;
            move_to_head(cache, node);
            return;
        }
        prev_node = node;
        node = node->next;
    }

    Node *new_node = create_node(key, value);
    add_to_head(cache, new_node);

    if (cache->hash_table[idx] == NULL) {
        cache->hash_table[idx] = new_node;
    } else {
        Node *temp = cache->hash_table[idx];
        while (temp->next != NULL && temp->next != cache->head && temp->next != cache->tail) {
            temp = temp->next;
        }
    }

    cache->size++;

    if (cache->size > cache->capacity) {
        Node *tail = remove_tail(cache);
        int tail_idx = hash(tail->key);
        Node *hash_node = cache->hash_table[tail_idx];
        Node *hash_prev = NULL;

        while (hash_node != NULL) {
            if (hash_node->key == tail->key) {
                if (hash_prev == NULL) {
                    cache->hash_table[tail_idx] = NULL;
                } else {
                    hash_prev->next = hash_node->next;
                }
                break;
            }
            hash_prev = hash_node;
            hash_node = hash_node->next;
        }

        free(tail);
        cache->size--;
    }
}

void print_cache(LRUCache *cache) {
    printf("Cache (size=%d): ", cache->size);
    Node *node = cache->head->next;
    while (node != cache->tail) {
        printf("[%d:%d] ", node->key, node->value);
        node = node->next;
    }
    printf("\n");
}

void free_cache(LRUCache *cache) {
    Node *node = cache->head;
    while (node != NULL) {
        Node *temp = node;
        node = node->next;
        free(temp);
    }
    free(cache->hash_table);
    free(cache);
}

int main() {
    LRUCache *cache = create_cache(CACHE_SIZE);

    printf("LRU Cache with capacity %d\n\n", CACHE_SIZE);

    printf("Put(1, 100)\n");
    put(cache, 1, 100);
    print_cache(cache);

    printf("Put(2, 200)\n");
    put(cache, 2, 200);
    print_cache(cache);

    printf("Put(3, 300)\n");
    put(cache, 3, 300);
    print_cache(cache);

    printf("Get(1) = %d\n", get(cache, 1));
    print_cache(cache);

    printf("Put(4, 400)\n");
    put(cache, 4, 400);
    print_cache(cache);

    printf("Put(5, 500)\n");
    put(cache, 5, 500);
    print_cache(cache);

    printf("Put(6, 600) - should evict LRU\n");
    put(cache, 6, 600);
    print_cache(cache);

    printf("Get(2) = %d (should be -1, evicted)\n", get(cache, 2));
    printf("Get(3) = %d\n", get(cache, 3));
    print_cache(cache);

    printf("Put(7, 700) - should evict LRU\n");
    put(cache, 7, 700);
    print_cache(cache);

    free_cache(cache);

    return 0;
}

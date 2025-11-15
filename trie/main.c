#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ALPHABET_SIZE 26

typedef struct TrieNode {
    struct TrieNode *children[ALPHABET_SIZE];
    bool is_end_of_word;
} TrieNode;

typedef struct {
    TrieNode *root;
} Trie;

TrieNode* create_node() {
    TrieNode *node = malloc(sizeof(TrieNode));
    node->is_end_of_word = false;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        node->children[i] = NULL;
    }
    return node;
}

Trie* create_trie() {
    Trie *trie = malloc(sizeof(Trie));
    trie->root = create_node();
    return trie;
}

void insert(Trie *trie, const char *word) {
    TrieNode *current = trie->root;
    int len = strlen(word);

    for (int i = 0; i < len; i++) {
        int index = word[i] - 'a';
        if (index < 0 || index >= ALPHABET_SIZE) {
            continue;
        }

        if (current->children[index] == NULL) {
            current->children[index] = create_node();
        }
        current = current->children[index];
    }
    current->is_end_of_word = true;
}

bool search(Trie *trie, const char *word) {
    TrieNode *current = trie->root;
    int len = strlen(word);

    for (int i = 0; i < len; i++) {
        int index = word[i] - 'a';
        if (index < 0 || index >= ALPHABET_SIZE || current->children[index] == NULL) {
            return false;
        }
        current = current->children[index];
    }
    return current != NULL && current->is_end_of_word;
}

bool starts_with(Trie *trie, const char *prefix) {
    TrieNode *current = trie->root;
    int len = strlen(prefix);

    for (int i = 0; i < len; i++) {
        int index = prefix[i] - 'a';
        if (index < 0 || index >= ALPHABET_SIZE || current->children[index] == NULL) {
            return false;
        }
        current = current->children[index];
    }
    return current != NULL;
}

void collect_words_helper(TrieNode *node, char *prefix, int depth, char **results, int *count, int max_results) {
    if (*count >= max_results) {
        return;
    }

    if (node->is_end_of_word) {
        prefix[depth] = '\0';
        results[*count] = malloc(strlen(prefix) + 1);
        strcpy(results[*count], prefix);
        (*count)++;
    }

    for (int i = 0; i < ALPHABET_SIZE && *count < max_results; i++) {
        if (node->children[i] != NULL) {
            prefix[depth] = 'a' + i;
            collect_words_helper(node->children[i], prefix, depth + 1, results, count, max_results);
        }
    }
}

int autocomplete(Trie *trie, const char *prefix, char **results, int max_results) {
    TrieNode *current = trie->root;
    int len = strlen(prefix);

    for (int i = 0; i < len; i++) {
        int index = prefix[i] - 'a';
        if (index < 0 || index >= ALPHABET_SIZE || current->children[index] == NULL) {
            return 0;
        }
        current = current->children[index];
    }

    char buffer[100];
    strcpy(buffer, prefix);
    int count = 0;
    collect_words_helper(current, buffer, len, results, &count, max_results);
    return count;
}

bool delete_helper(TrieNode *node, const char *word, int depth) {
    if (node == NULL) {
        return false;
    }

    if (depth == strlen(word)) {
        if (node->is_end_of_word) {
            node->is_end_of_word = false;

            for (int i = 0; i < ALPHABET_SIZE; i++) {
                if (node->children[i] != NULL) {
                    return false;
                }
            }
            return true;
        }
        return false;
    }

    int index = word[depth] - 'a';
    if (index < 0 || index >= ALPHABET_SIZE) {
        return false;
    }

    bool should_delete = delete_helper(node->children[index], word, depth + 1);

    if (should_delete) {
        free(node->children[index]);
        node->children[index] = NULL;

        if (!node->is_end_of_word) {
            for (int i = 0; i < ALPHABET_SIZE; i++) {
                if (node->children[i] != NULL) {
                    return false;
                }
            }
            return true;
        }
    }

    return false;
}

void delete(Trie *trie, const char *word) {
    delete_helper(trie->root, word, 0);
}

int main() {
    Trie *trie = create_trie();

    printf("Inserting words: apple, app, application, banana, band, bandana, cat, car\n\n");
    insert(trie, "apple");
    insert(trie, "app");
    insert(trie, "application");
    insert(trie, "banana");
    insert(trie, "band");
    insert(trie, "bandana");
    insert(trie, "cat");
    insert(trie, "car");

    printf("Search 'apple': %s\n", search(trie, "apple") ? "found" : "not found");
    printf("Search 'app': %s\n", search(trie, "app") ? "found" : "not found");
    printf("Search 'appl': %s\n", search(trie, "appl") ? "found" : "not found");
    printf("Search 'banana': %s\n", search(trie, "banana") ? "found" : "not found");
    printf("Search 'dog': %s\n\n", search(trie, "dog") ? "found" : "not found");

    printf("Starts with 'app': %s\n", starts_with(trie, "app") ? "yes" : "no");
    printf("Starts with 'ban': %s\n", starts_with(trie, "ban") ? "yes" : "no");
    printf("Starts with 'dog': %s\n\n", starts_with(trie, "dog") ? "yes" : "no");

    char *results[10];
    int count = autocomplete(trie, "app", results, 10);
    printf("Autocomplete for 'app' (%d results):\n", count);
    for (int i = 0; i < count; i++) {
        printf("  %s\n", results[i]);
        free(results[i]);
    }

    printf("\n");
    count = autocomplete(trie, "ban", results, 10);
    printf("Autocomplete for 'ban' (%d results):\n", count);
    for (int i = 0; i < count; i++) {
        printf("  %s\n", results[i]);
        free(results[i]);
    }

    printf("\nDeleting 'app'...\n");
    delete(trie, "app");
    printf("Search 'app' after deletion: %s\n", search(trie, "app") ? "found" : "not found");
    printf("Search 'apple' after deletion: %s\n", search(trie, "apple") ? "found" : "not found");

    return 0;
}

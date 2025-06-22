#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define HEAP_SIZE 4096
#define ALIGNMENT 8
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~(ALIGNMENT-1))

typedef struct block {
    size_t size;
    bool marked;
    bool free;
    struct block *next;
    struct block *prev;
} block_t;

typedef struct {
    void *heap_start;
    void *heap_end;
    block_t *free_list;
    block_t *allocated_list;
    size_t total_allocated;
    size_t gc_threshold;
} allocator_t;

static allocator_t gc_allocator = {0};

bool gc_init(void) {
    gc_allocator.heap_start = malloc(HEAP_SIZE);
    if (!gc_allocator.heap_start) {
        return false;
    }
    
    gc_allocator.heap_end = (char*)gc_allocator.heap_start + HEAP_SIZE;
    gc_allocator.free_list = (block_t*)gc_allocator.heap_start;
    gc_allocator.allocated_list = NULL;
    gc_allocator.total_allocated = 0;
    gc_allocator.gc_threshold = HEAP_SIZE / 2;
    
    gc_allocator.free_list->size = HEAP_SIZE - sizeof(block_t);
    gc_allocator.free_list->marked = false;
    gc_allocator.free_list->free = true;
    gc_allocator.free_list->next = NULL;
    gc_allocator.free_list->prev = NULL;
    
    return true;
}

static void split_block(block_t *block, size_t size) {
    if (block->size >= size + sizeof(block_t) + ALIGNMENT) {
        block_t *new_block = (block_t*)((char*)block + sizeof(block_t) + size);
        new_block->size = block->size - size - sizeof(block_t);
        new_block->free = true;
        new_block->marked = false;
        new_block->next = block->next;
        new_block->prev = block;
        
        if (block->next) {
            block->next->prev = new_block;
        }
        block->next = new_block;
        block->size = size;
    }
}

static void remove_from_free_list(block_t *block) {
    if (block->prev) {
        block->prev->next = block->next;
    } else {
        gc_allocator.free_list = block->next;
    }
    
    if (block->next) {
        block->next->prev = block->prev;
    }
}

static void add_to_allocated_list(block_t *block) {
    block->next = gc_allocator.allocated_list;
    block->prev = NULL;
    
    if (gc_allocator.allocated_list) {
        gc_allocator.allocated_list->prev = block;
    }
    
    gc_allocator.allocated_list = block;
}

static block_t* find_block_for_ptr(void *ptr) {
    if (!ptr || ptr < gc_allocator.heap_start || ptr >= gc_allocator.heap_end) {
        return NULL;
    }
    
    block_t *block = (block_t*)((char*)ptr - sizeof(block_t));
    
    if ((void*)block >= gc_allocator.heap_start && 
        (void*)block < gc_allocator.heap_end &&
        !block->free) {
        return block;
    }
    
    return NULL;
}

static void mark_reachable(void *ptr) {
    static void* mark_stack[1024];
    int stack_top = 0;
    
    mark_stack[stack_top++] = ptr;
    
    while (stack_top > 0) {
        void *current_ptr = mark_stack[--stack_top];
        
        if (!current_ptr || current_ptr < gc_allocator.heap_start || current_ptr >= gc_allocator.heap_end) {
            continue;
        }
        
        block_t *block = find_block_for_ptr(current_ptr);
        if (block && !block->marked) {
            block->marked = true;
            
            uintptr_t *scan_ptr = (uintptr_t*)((char*)block + sizeof(block_t));
            size_t scan_words = block->size / sizeof(uintptr_t);
            
            for (size_t i = 0; i < scan_words && stack_top < 1023; i++) {
                mark_stack[stack_top++] = (void*)scan_ptr[i];
            }
        }
    }
}

static void sweep(void) {
    block_t *current = gc_allocator.allocated_list;
    block_t *prev = NULL;
    
    while (current) {
        block_t *next = current->next;
        
        if (!current->marked) {
            if (prev) {
                prev->next = next;
            } else {
                gc_allocator.allocated_list = next;
            }
            
            if (next) {
                next->prev = prev;
            }
            
            current->free = true;
            current->marked = false;
            current->next = gc_allocator.free_list;
            current->prev = NULL;
            
            if (gc_allocator.free_list) {
                gc_allocator.free_list->prev = current;
            }
            gc_allocator.free_list = current;
            gc_allocator.total_allocated -= current->size;
            
            current = next;
        } else {
            current->marked = false;
            prev = current;
            current = next;
        }
    }
}

void gc_collect(void **roots, size_t root_count) {
    for (size_t i = 0; i < root_count; i++) {
        if (roots[i]) {
            mark_reachable(roots[i]);
        }
    }
    
    sweep();
}

void* gc_malloc(size_t size) {
    if (size == 0) return NULL;
    
    size = ALIGN(size);
    block_t *best_fit = NULL;
    block_t *current = gc_allocator.free_list;
    
    while (current) {
        if (current->size >= size) {
            if (!best_fit || current->size < best_fit->size) {
                best_fit = current;
                if (current->size == size) break;
            }
        }
        current = current->next;
    }
    
    if (best_fit) {
        split_block(best_fit, size);
        remove_from_free_list(best_fit);
        add_to_allocated_list(best_fit);
        
        best_fit->free = false;
        best_fit->marked = false;
        gc_allocator.total_allocated += size;
        
        return (char*)best_fit + sizeof(block_t);
    }
    
    return NULL;
}

void gc_stats(void) {
    printf("Heap Statistics:\n");
    printf("Total allocated: %zu bytes\n", gc_allocator.total_allocated);
    printf("GC threshold: %zu bytes\n", gc_allocator.gc_threshold);
    
    size_t free_blocks = 0, free_bytes = 0;
    block_t *current = gc_allocator.free_list;
    while (current) {
        free_blocks++;
        free_bytes += current->size;
        current = current->next;
    }
    
    size_t allocated_blocks = 0;
    current = gc_allocator.allocated_list;
    while (current) {
        allocated_blocks++;
        current = current->next;
    }
    
    printf("Free blocks: %zu (%zu bytes)\n", free_blocks, free_bytes);
    printf("Allocated blocks: %zu\n", allocated_blocks);
}

void gc_cleanup(void) {
    if (gc_allocator.heap_start) {
        free(gc_allocator.heap_start);
        memset(&gc_allocator, 0, sizeof(gc_allocator));
    }
}

int main() {
    if (!gc_init()) {
        printf("Failed to initialize garbage collector\n");
        return 1;
    }
    
    printf("Garbage Collector Demo\n");
    printf("======================\n");
    
    void *ptr1 = gc_malloc(100);
    void *ptr2 = gc_malloc(200);
    void *ptr3 = gc_malloc(150);
    
    printf("Allocated 3 blocks\n");
    gc_stats();
    
    void *roots[] = {ptr1, ptr3};
    
    gc_collect(roots, 2);
    gc_stats();
    
    void *ptr4 = gc_malloc(300);
    printf("Allocated another block\n");
    gc_stats();
    
    gc_cleanup();
    return 0;
}
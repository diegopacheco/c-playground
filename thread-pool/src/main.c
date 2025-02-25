#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

typedef struct Task {
    void (*function)(void *);
    void *argument;
    struct Task *next;
} Task;

typedef struct ThreadPool {
    pthread_mutex_t lock;
    pthread_cond_t notify;
    pthread_t *threads;
    Task *queue_head;
    Task *queue_tail;
    int thread_count;
    int task_count;
    bool shutdown;
} ThreadPool;

int threadpool_destroy(ThreadPool *pool);

static void *worker_thread(void *arg) {
    ThreadPool *pool = (ThreadPool *)arg;
    Task *task;
    
    while (1) {
        pthread_mutex_lock(&(pool->lock));
        
        // Wait for a task or shutdown signal
        while (pool->task_count == 0 && !pool->shutdown) {
            pthread_cond_wait(&(pool->notify), &(pool->lock));
        }
        
        if (pool->shutdown) {
            pthread_mutex_unlock(&(pool->lock));
            pthread_exit(NULL);
        }
        
        // Get the first task from the queue
        task = pool->queue_head;
        if (task != NULL) {
            pool->queue_head = task->next;
            if (pool->queue_head == NULL) {
                pool->queue_tail = NULL;
            }
            pool->task_count--;
        }
        
        pthread_mutex_unlock(&(pool->lock));
        if (task != NULL) {
            // Execute the task
            (*(task->function))(task->argument);
            free(task);
        }
    }
    return NULL;
}

ThreadPool *threadpool_create(int thread_count) {
    if (thread_count <= 0)
        return NULL;
    
    ThreadPool *pool = (ThreadPool *)calloc(1, sizeof(ThreadPool));
    if (pool == NULL)
        return NULL;
    
    // Initialize the pool
    pool->thread_count = thread_count;
    pool->task_count = 0;
    pool->queue_head = NULL;
    pool->queue_tail = NULL;
    pool->shutdown = false;
    
    // Initialize mutex and condition variable
    pthread_mutex_init(&(pool->lock), NULL);
    pthread_cond_init(&(pool->notify), NULL);
    
    // Create worker threads
    pool->threads = (pthread_t *)malloc(thread_count * sizeof(pthread_t));
    if (pool->threads == NULL) {
        free(pool);
        return NULL;
    }
    
    for (int i = 0; i < thread_count; i++) {
        if (pthread_create(&(pool->threads[i]), NULL, worker_thread, pool) != 0) {
            threadpool_destroy(pool);
            return NULL;
        }
    }
    
    return pool;
}

int threadpool_add_task(ThreadPool *pool, void (*function)(void *), void *argument) {
    if (pool == NULL || function == NULL)
        return -1;
    
    Task *task = (Task *)malloc(sizeof(Task));
    if (task == NULL)
        return -1;
    
    task->function = function;
    task->argument = argument;
    task->next = NULL;
    
    pthread_mutex_lock(&(pool->lock));
    
    // Add task to queue
    if (pool->queue_head == NULL) {
        pool->queue_head = task;
        pool->queue_tail = task;
    } else {
        pool->queue_tail->next = task;
        pool->queue_tail = task;
    }
    
    pool->task_count++;
    
    // Signal a waiting thread that there's work
    pthread_cond_signal(&(pool->notify));
    pthread_mutex_unlock(&(pool->lock));
    
    return 0;
}

int threadpool_destroy(ThreadPool *pool) {
    if (pool == NULL)
        return -1;
    
    pthread_mutex_lock(&(pool->lock));
    pool->shutdown = true;
    pthread_cond_broadcast(&(pool->notify));
    pthread_mutex_unlock(&(pool->lock));
    
    // Wait for all threads to exit
    for (int i = 0; i < pool->thread_count; i++) {
        pthread_join(pool->threads[i], NULL);
    }
    
    // Clean up
    free(pool->threads);
    
    // Clean up remaining tasks
    Task *task;
    while (pool->queue_head != NULL) {
        task = pool->queue_head;
        pool->queue_head = task->next;
        free(task);
    }
    
    pthread_mutex_destroy(&(pool->lock));
    pthread_cond_destroy(&(pool->notify));
    free(pool);
    
    return 0;
}

void example_task(void *arg) {
    int num = *(int *)arg;
    printf("Task %d executing\n", num);
    sleep(1);
    printf("Task %d completed\n", num);
    free(arg);
}

int main() {
    ThreadPool *pool = threadpool_create(4);
    if (pool == NULL) {
        fprintf(stderr, "Failed to create thread pool\n");
        return 1;
    }
    
    for (int i = 0; i < 10; i++) {
        int *arg = malloc(sizeof(int));
        *arg = i;
        threadpool_add_task(pool, example_task, arg);
    }
    
    sleep(5);
    threadpool_destroy(pool);
    return 0;
}
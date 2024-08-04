#include <stdio.h>
#include "queue.h"

int main() {
    Queue *q = initQueue();
    enqueue(q, 10);
    enqueue(q, 20);
    enqueue(q, 30);

    printf("Front element is %d\n", peek(q));
    printf("Dequeued element is %d\n", dequeue(q));
    printf("Dequeued element is %d\n", dequeue(q));
    printf("Dequeued element is %d\n", dequeue(q));
    printf("Dequeued element is %d\n", dequeue(q)); // Should indicate the queue is empty

    freeQueue(q);
    return 0;
}
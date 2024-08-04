#include <stdio.h>
#include "stack.h"

int main() {
    Stack s;
    initialize(&s);

    push(&s, 10);
    push(&s, 20);
    push(&s, 30);

    printf("Top element is %d\n", peek(&s));
    printf("Popped element is %d\n", pop(&s));
    printf("Popped element is %d\n", pop(&s));
    printf("Popped element is %d\n", pop(&s));
    printf("Popped element is %d\n", pop(&s)); // This will show stack is empty

    return 0;
}

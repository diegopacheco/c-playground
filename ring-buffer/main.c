#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct {
    int *buffer;
    int capacity;
    int size;
    int read_pos;
    int write_pos;
} RingBuffer;

RingBuffer* create_ring_buffer(int capacity) {
    RingBuffer *rb = malloc(sizeof(RingBuffer));
    rb->buffer = malloc(sizeof(int) * capacity);
    rb->capacity = capacity;
    rb->size = 0;
    rb->read_pos = 0;
    rb->write_pos = 0;
    return rb;
}

bool is_full(RingBuffer *rb) {
    return rb->size == rb->capacity;
}

bool is_empty(RingBuffer *rb) {
    return rb->size == 0;
}

int get_size(RingBuffer *rb) {
    return rb->size;
}

int get_available_space(RingBuffer *rb) {
    return rb->capacity - rb->size;
}

bool write_item(RingBuffer *rb, int value) {
    if (is_full(rb)) {
        return false;
    }

    rb->buffer[rb->write_pos] = value;
    rb->write_pos = (rb->write_pos + 1) % rb->capacity;
    rb->size++;
    return true;
}

bool read_item(RingBuffer *rb, int *value) {
    if (is_empty(rb)) {
        return false;
    }

    *value = rb->buffer[rb->read_pos];
    rb->read_pos = (rb->read_pos + 1) % rb->capacity;
    rb->size--;
    return true;
}

bool peek_item(RingBuffer *rb, int *value) {
    if (is_empty(rb)) {
        return false;
    }

    *value = rb->buffer[rb->read_pos];
    return true;
}

void write_overwrite(RingBuffer *rb, int value) {
    if (is_full(rb)) {
        rb->read_pos = (rb->read_pos + 1) % rb->capacity;
        rb->size--;
    }

    rb->buffer[rb->write_pos] = value;
    rb->write_pos = (rb->write_pos + 1) % rb->capacity;
    rb->size++;
}

int write_batch(RingBuffer *rb, int *values, int count) {
    int written = 0;
    for (int i = 0; i < count; i++) {
        if (write_item(rb, values[i])) {
            written++;
        } else {
            break;
        }
    }
    return written;
}

int read_batch(RingBuffer *rb, int *values, int count) {
    int read = 0;
    for (int i = 0; i < count; i++) {
        if (read_item(rb, &values[i])) {
            read++;
        } else {
            break;
        }
    }
    return read;
}

void clear_buffer(RingBuffer *rb) {
    rb->size = 0;
    rb->read_pos = 0;
    rb->write_pos = 0;
}

void print_buffer(RingBuffer *rb) {
    printf("Ring Buffer (size=%d, capacity=%d): [", rb->size, rb->capacity);

    int pos = rb->read_pos;
    for (int i = 0; i < rb->size; i++) {
        printf("%d", rb->buffer[pos]);
        if (i < rb->size - 1) printf(", ");
        pos = (pos + 1) % rb->capacity;
    }
    printf("]\n");
}

void free_ring_buffer(RingBuffer *rb) {
    free(rb->buffer);
    free(rb);
}

int main() {
    RingBuffer *rb = create_ring_buffer(5);

    printf("Created ring buffer with capacity 5\n\n");

    printf("Writing 1, 2, 3\n");
    write_item(rb, 1);
    write_item(rb, 2);
    write_item(rb, 3);
    print_buffer(rb);

    int value;
    printf("\nReading one item: ");
    if (read_item(rb, &value)) {
        printf("%d\n", value);
    }
    print_buffer(rb);

    printf("\nWriting 4, 5, 6\n");
    write_item(rb, 4);
    write_item(rb, 5);
    if (!write_item(rb, 6)) {
        printf("Buffer full! Cannot write 6\n");
    }
    print_buffer(rb);

    printf("\nPeeking at front: ");
    if (peek_item(rb, &value)) {
        printf("%d\n", value);
    }
    print_buffer(rb);

    printf("\nReading two items: ");
    read_item(rb, &value);
    printf("%d ", value);
    read_item(rb, &value);
    printf("%d\n", value);
    print_buffer(rb);

    printf("\nWriting with overwrite mode: 6, 7, 8\n");
    write_overwrite(rb, 6);
    write_overwrite(rb, 7);
    write_overwrite(rb, 8);
    print_buffer(rb);

    printf("\nBatch write [9, 10, 11]: ");
    int batch_write[] = {9, 10, 11};
    int written = write_batch(rb, batch_write, 3);
    printf("%d items written\n", written);
    print_buffer(rb);

    printf("\nBatch read 3 items: ");
    int batch_read[3];
    int read_count = read_batch(rb, batch_read, 3);
    printf("%d items read: ", read_count);
    for (int i = 0; i < read_count; i++) {
        printf("%d ", batch_read[i]);
    }
    printf("\n");
    print_buffer(rb);

    printf("\nClearing buffer\n");
    clear_buffer(rb);
    print_buffer(rb);

    printf("\nIs empty: %s\n", is_empty(rb) ? "yes" : "no");
    printf("Is full: %s\n", is_full(rb) ? "yes" : "no");

    free_ring_buffer(rb);

    return 0;
}

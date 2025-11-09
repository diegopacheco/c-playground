#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

#define MAX_VERTICES 100
#define INF INT_MAX

typedef struct {
    int vertex;
    int weight;
} Edge;

typedef struct {
    Edge *edges;
    int count;
    int capacity;
} AdjList;

typedef struct {
    AdjList *adj;
    int num_vertices;
} Graph;

typedef struct {
    int vertex;
    int distance;
} MinHeapNode;

typedef struct {
    MinHeapNode *nodes;
    int *pos;
    int size;
    int capacity;
} MinHeap;

Graph* create_graph(int vertices) {
    Graph *graph = malloc(sizeof(Graph));
    graph->num_vertices = vertices;
    graph->adj = malloc(sizeof(AdjList) * vertices);

    for (int i = 0; i < vertices; i++) {
        graph->adj[i].capacity = 4;
        graph->adj[i].count = 0;
        graph->adj[i].edges = malloc(sizeof(Edge) * graph->adj[i].capacity);
    }
    return graph;
}

void add_edge(Graph *graph, int src, int dest, int weight) {
    if (graph->adj[src].count >= graph->adj[src].capacity) {
        graph->adj[src].capacity *= 2;
        graph->adj[src].edges = realloc(graph->adj[src].edges, sizeof(Edge) * graph->adj[src].capacity);
    }
    graph->adj[src].edges[graph->adj[src].count].vertex = dest;
    graph->adj[src].edges[graph->adj[src].count].weight = weight;
    graph->adj[src].count++;
}

MinHeap* create_min_heap(int capacity) {
    MinHeap *heap = malloc(sizeof(MinHeap));
    heap->size = 0;
    heap->capacity = capacity;
    heap->nodes = malloc(sizeof(MinHeapNode) * capacity);
    heap->pos = malloc(sizeof(int) * capacity);
    return heap;
}

void swap_nodes(MinHeapNode *a, MinHeapNode *b) {
    MinHeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void min_heapify(MinHeap *heap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < heap->size && heap->nodes[left].distance < heap->nodes[smallest].distance) {
        smallest = left;
    }

    if (right < heap->size && heap->nodes[right].distance < heap->nodes[smallest].distance) {
        smallest = right;
    }

    if (smallest != idx) {
        heap->pos[heap->nodes[smallest].vertex] = idx;
        heap->pos[heap->nodes[idx].vertex] = smallest;
        swap_nodes(&heap->nodes[smallest], &heap->nodes[idx]);
        min_heapify(heap, smallest);
    }
}

MinHeapNode extract_min(MinHeap *heap) {
    if (heap->size == 0) {
        MinHeapNode empty = {-1, INF};
        return empty;
    }

    MinHeapNode root = heap->nodes[0];
    MinHeapNode last = heap->nodes[heap->size - 1];
    heap->nodes[0] = last;

    heap->pos[root.vertex] = heap->size - 1;
    heap->pos[last.vertex] = 0;

    heap->size--;
    min_heapify(heap, 0);

    return root;
}

void decrease_key(MinHeap *heap, int vertex, int distance) {
    int i = heap->pos[vertex];
    heap->nodes[i].distance = distance;

    while (i && heap->nodes[i].distance < heap->nodes[(i - 1) / 2].distance) {
        heap->pos[heap->nodes[i].vertex] = (i - 1) / 2;
        heap->pos[heap->nodes[(i - 1) / 2].vertex] = i;
        swap_nodes(&heap->nodes[i], &heap->nodes[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

bool is_in_heap(MinHeap *heap, int vertex) {
    return heap->pos[vertex] < heap->size;
}

void dijkstra(Graph *graph, int src, int *dist, int *prev) {
    MinHeap *heap = create_min_heap(graph->num_vertices);

    for (int v = 0; v < graph->num_vertices; v++) {
        dist[v] = INF;
        prev[v] = -1;
        heap->nodes[v].vertex = v;
        heap->nodes[v].distance = INF;
        heap->pos[v] = v;
    }

    dist[src] = 0;
    heap->nodes[src].distance = 0;
    decrease_key(heap, src, 0);
    heap->size = graph->num_vertices;

    while (heap->size != 0) {
        MinHeapNode min_node = extract_min(heap);
        int u = min_node.vertex;

        for (int i = 0; i < graph->adj[u].count; i++) {
            int v = graph->adj[u].edges[i].vertex;
            int weight = graph->adj[u].edges[i].weight;

            if (is_in_heap(heap, v) && dist[u] != INF && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                prev[v] = u;
                decrease_key(heap, v, dist[v]);
            }
        }
    }

    free(heap->nodes);
    free(heap->pos);
    free(heap);
}

void print_path(int *prev, int vertex) {
    if (prev[vertex] == -1) {
        printf("%d", vertex);
        return;
    }
    print_path(prev, prev[vertex]);
    printf(" -> %d", vertex);
}

void print_solution(int *dist, int *prev, int num_vertices, int src) {
    printf("Vertex   Distance   Path\n");
    for (int i = 0; i < num_vertices; i++) {
        printf("%d        ", i);
        if (dist[i] == INF) {
            printf("INF        -\n");
        } else {
            printf("%-10d ", dist[i]);
            print_path(prev, i);
            printf("\n");
        }
    }
}

int main() {
    int num_vertices = 9;
    Graph *graph = create_graph(num_vertices);

    add_edge(graph, 0, 1, 4);
    add_edge(graph, 0, 7, 8);
    add_edge(graph, 1, 2, 8);
    add_edge(graph, 1, 7, 11);
    add_edge(graph, 2, 3, 7);
    add_edge(graph, 2, 8, 2);
    add_edge(graph, 2, 5, 4);
    add_edge(graph, 3, 4, 9);
    add_edge(graph, 3, 5, 14);
    add_edge(graph, 4, 5, 10);
    add_edge(graph, 5, 6, 2);
    add_edge(graph, 6, 7, 1);
    add_edge(graph, 6, 8, 6);
    add_edge(graph, 7, 8, 7);

    int dist[MAX_VERTICES];
    int prev[MAX_VERTICES];

    int src = 0;
    printf("Dijkstra's Shortest Path Algorithm\n");
    printf("Source vertex: %d\n\n", src);

    dijkstra(graph, src, dist, prev);
    print_solution(dist, prev, num_vertices, src);

    return 0;
}

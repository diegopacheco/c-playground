#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#define MAX_CITIES 20
#define INF INT_MAX

int tsp_brute_force(int graph[][MAX_CITIES], int n, int pos, int visited, int start) {
    if (visited == (1 << n) - 1) {
        return graph[pos][start];
    }
    
    int min_cost = INF;
    for (int city = 0; city < n; city++) {
        if (!(visited & (1 << city)) && graph[pos][city] != INF) {
            int cost = graph[pos][city] + 
                      tsp_brute_force(graph, n, city, visited | (1 << city), start);
            if (cost < min_cost) {
                min_cost = cost;
            }
        }
    }
    return min_cost;
}

int dp[MAX_CITIES][1 << MAX_CITIES];

int tsp_dp(int graph[][MAX_CITIES], int n, int pos, int mask) {
    if (mask == (1 << n) - 1) {
        return graph[pos][0];
    }
    
    if (dp[pos][mask] != -1) {
        return dp[pos][mask];
    }
    
    int min_cost = INF;
    for (int city = 0; city < n; city++) {
        if (!(mask & (1 << city)) && graph[pos][city] != INF) {
            int cost = graph[pos][city] + 
                      tsp_dp(graph, n, city, mask | (1 << city));
            if (cost < min_cost) {
                min_cost = cost;
            }
        }
    }
    
    return dp[pos][mask] = min_cost;
}

int tsp_nearest_neighbor(int graph[][MAX_CITIES], int n, int start) {
    int visited[MAX_CITIES] = {0};
    int current = start;
    int total_cost = 0;
    visited[current] = 1;
    
    for (int count = 1; count < n; count++) {
        int nearest = -1;
        int min_dist = INF;
        
        for (int city = 0; city < n; city++) {
            if (!visited[city] && graph[current][city] < min_dist) {
                min_dist = graph[current][city];
                nearest = city;
            }
        }
        
        if (nearest != -1) {
            visited[nearest] = 1;
            total_cost += min_dist;
            current = nearest;
        }
    }
    
    total_cost += graph[current][start];
    return total_cost;
}

int main() {
    int n = 4;
    int graph[MAX_CITIES][MAX_CITIES] = {
        {0, 10, 15, 20},
        {10, 0, 35, 25},
        {15, 35, 0, 30},
        {20, 25, 30, 0}
    };
    
    printf("TSP Solutions for %d cities:\n", n);
    
    int brute_result = tsp_brute_force(graph, n, 0, 1, 0);
    printf("Brute Force: %d\n", brute_result);
    
    memset(dp, -1, sizeof(dp));
    int dp_result = tsp_dp(graph, n, 0, 1);
    printf("Dynamic Programming: %d\n", dp_result);
    
    int nn_result = tsp_nearest_neighbor(graph, n, 0);
    printf("Nearest Neighbor Heuristic: %d\n", nn_result);
    
    return 0;
}
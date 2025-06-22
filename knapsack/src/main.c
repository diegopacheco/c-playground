#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ITEMS 100
#define MAX_WEIGHT 1000

int knapsack_recursive(int weights[], int values[], int n, int capacity) {
    if (n == 0 || capacity == 0) {
        return 0;
    }
    
    if (weights[n-1] > capacity) {
        return knapsack_recursive(weights, values, n-1, capacity);
    }
    
    int include = values[n-1] + knapsack_recursive(weights, values, n-1, capacity - weights[n-1]);
    int exclude = knapsack_recursive(weights, values, n-1, capacity);
    
    return (include > exclude) ? include : exclude;
}

int knapsack_dp(int weights[], int values[], int n, int capacity) {
    int dp[n+1][capacity+1];
    
    for (int i = 0; i <= n; i++) {
        for (int w = 0; w <= capacity; w++) {
            if (i == 0 || w == 0) {
                dp[i][w] = 0;
            } else if (weights[i-1] <= w) {
                int include = values[i-1] + dp[i-1][w - weights[i-1]];
                int exclude = dp[i-1][w];
                dp[i][w] = (include > exclude) ? include : exclude;
            } else {
                dp[i][w] = dp[i-1][w];
            }
        }
    }
    
    return dp[n][capacity];
}

int knapsack_optimized(int weights[], int values[], int n, int capacity) {
    int dp[capacity + 1];
    memset(dp, 0, sizeof(dp));
    
    for (int i = 0; i < n; i++) {
        for (int w = capacity; w >= weights[i]; w--) {
            dp[w] = (dp[w] > dp[w - weights[i]] + values[i]) ? 
                    dp[w] : dp[w - weights[i]] + values[i];
        }
    }
    
    return dp[capacity];
}

typedef struct {
    int value;
    int weight;
    double ratio;
    int index;
} Item;

int compare_items(const void *a, const void *b) {
    Item *itemA = (Item*)a;
    Item *itemB = (Item*)b;
    return (itemB->ratio > itemA->ratio) ? 1 : -1;
}

int knapsack_greedy(int weights[], int values[], int n, int capacity) {
    Item items[n];
    
    for (int i = 0; i < n; i++) {
        items[i].value = values[i];
        items[i].weight = weights[i];
        items[i].ratio = (double)values[i] / weights[i];
        items[i].index = i;
    }
    
    qsort(items, n, sizeof(Item), compare_items);
    
    int total_value = 0;
    int current_weight = 0;
    
    for (int i = 0; i < n; i++) {
        if (current_weight + items[i].weight <= capacity) {
            total_value += items[i].value;
            current_weight += items[i].weight;
            printf("Taking item %d (weight: %d, value: %d, ratio: %.2f)\n", 
                   items[i].index, items[i].weight, items[i].value, items[i].ratio);
        }
    }
    
    return total_value;
}

int main() {
    int values[] = {60, 100, 120};
    int weights[] = {10, 20, 30};
    int n = sizeof(values) / sizeof(values[0]);
    int capacity = 50;
    
    printf("Knapsack Problem (capacity: %d)\n", capacity);
    printf("Items: ");
    for (int i = 0; i < n; i++) {
        printf("(w:%d, v:%d) ", weights[i], values[i]);
    }
    printf("\n\n");
    
    if (n <= 10) {
        int recursive_result = knapsack_recursive(weights, values, n, capacity);
        printf("Recursive solution: %d\n", recursive_result);
    }
    
    int dp_result = knapsack_dp(weights, values, n, capacity);
    printf("Dynamic Programming: %d\n", dp_result);
    
    int optimized_result = knapsack_optimized(weights, values, n, capacity);
    printf("Space-optimized DP: %d\n", optimized_result);
    
    printf("\nGreedy approximation (by value/weight ratio):\n");
    int greedy_result = knapsack_greedy(weights, values, n, capacity);
    printf("Greedy result: %d\n", greedy_result);
    
    return 0;
}
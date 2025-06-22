#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_SIZE 100

int memo[MAX_SIZE][10000];
bool memo_init = false;

int subset_sum_memo(int arr[], int n, int sum) {
    if (!memo_init) {
        for (int i = 0; i < MAX_SIZE; i++) {
            for (int j = 0; j < 10000; j++) {
                memo[i][j] = -1;
            }
        }
        memo_init = true;
    }
    
    if (sum == 0) return 1;
    if (n == 0) return 0;
    
    if (memo[n][sum] != -1) {
        return memo[n][sum];
    }
    
    if (arr[n-1] > sum) {
        memo[n][sum] = subset_sum_memo(arr, n-1, sum);
    } else {
        memo[n][sum] = subset_sum_memo(arr, n-1, sum) || 
                       subset_sum_memo(arr, n-1, sum - arr[n-1]);
    }
    
    return memo[n][sum];
}

bool subset_sum_dp(int arr[], int n, int sum) {
    bool dp[n+1][sum+1];
    
    for (int i = 0; i <= n; i++) {
        dp[i][0] = true;
    }
    
    for (int j = 1; j <= sum; j++) {
        dp[0][j] = false;
    }
    
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= sum; j++) {
            if (arr[i-1] > j) {
                dp[i][j] = dp[i-1][j];
            } else {
                dp[i][j] = dp[i-1][j] || dp[i-1][j - arr[i-1]];
            }
        }
    }
    
    return dp[n][sum];
}

bool find_subset_backtrack(int arr[], int n, int sum, int subset[], int *subset_size) {
    if (sum == 0) {
        return true;
    }
    if (n == 0) {
        return false;
    }
    
    if (arr[n-1] <= sum) {
        subset[*subset_size] = arr[n-1];
        (*subset_size)++;
        
        if (find_subset_backtrack(arr, n-1, sum - arr[n-1], subset, subset_size)) {
            return true;
        }
        
        (*subset_size)--;
    }
    
    return find_subset_backtrack(arr, n-1, sum, subset, subset_size);
}

void find_all_subsets(int arr[], int n, int sum, int current_subset[], 
                     int current_size, int start_index) {
    if (sum == 0) {
        printf("Subset found: { ");
        for (int i = 0; i < current_size; i++) {
            printf("%d ", current_subset[i]);
        }
        printf("}\n");
        return;
    }
    
    if (sum < 0 || start_index >= n) {
        return;
    }
    
    for (int i = start_index; i < n; i++) {
        current_subset[current_size] = arr[i];
        find_all_subsets(arr, n, sum - arr[i], current_subset, 
                        current_size + 1, i + 1);
    }
}

bool subset_sum_optimized(int arr[], int n, int sum) {
    bool dp[sum + 1];
    
    for (int i = 0; i <= sum; i++) {
        dp[i] = false;
    }
    dp[0] = true;
    
    for (int i = 0; i < n; i++) {
        for (int j = sum; j >= arr[i]; j--) {
            dp[j] = dp[j] || dp[j - arr[i]];
        }
    }
    
    return dp[sum];
}

int subset_sum_greedy_approx(int arr[], int n, int target_sum) {
    for (int i = 0; i < n-1; i++) {
        for (int j = i+1; j < n; j++) {
            if (arr[i] < arr[j]) {
                int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }
    
    int current_sum = 0;
    printf("Greedy approximation: ");
    
    for (int i = 0; i < n; i++) {
        if (current_sum + arr[i] <= target_sum) {
            current_sum += arr[i];
            printf("%d ", arr[i]);
        }
    }
    
    printf("\nSum achieved: %d (target: %d)\n", current_sum, target_sum);
    return current_sum;
}

int main() {
    int arr[] = {3, 34, 4, 12, 5, 2};
    int n = sizeof(arr) / sizeof(arr[0]);
    int sum = 9;
    
    printf("Subset Sum Problem\n");
    printf("Array: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\nTarget sum: %d\n\n", sum);
    
    printf("=== Testing if subset exists ===\n");
    
    if (subset_sum_memo(arr, n, sum)) {
        printf("Memoized recursive: Subset exists\n");
    } else {
        printf("Memoized recursive: Subset does not exist\n");
    }
    
    if (subset_sum_dp(arr, n, sum)) {
        printf("Dynamic Programming: Subset exists\n");
    } else {
        printf("Dynamic Programming: Subset does not exist\n");
    }
    
    if (subset_sum_optimized(arr, n, sum)) {
        printf("Space-optimized DP: Subset exists\n");
    } else {
        printf("Space-optimized DP: Subset does not exist\n");
    }
    
    printf("\n=== Finding actual subset ===\n");
    int subset[MAX_SIZE];
    int subset_size = 0;
    
    if (find_subset_backtrack(arr, n, sum, subset, &subset_size)) {
        printf("One possible subset: { ");
        for (int i = 0; i < subset_size; i++) {
            printf("%d ", subset[i]);
        }
        printf("}\n");
    } else {
        printf("No subset found\n");
    }
    
    printf("\n=== Finding all subsets ===\n");
    int current_subset[MAX_SIZE];
    find_all_subsets(arr, n, sum, current_subset, 0, 0);
    
    printf("\n=== Greedy approximation ===\n");
    int arr_copy[n];
    for (int i = 0; i < n; i++) {
        arr_copy[i] = arr[i];
    }
    subset_sum_greedy_approx(arr_copy, n, sum);
    
    return 0;
}
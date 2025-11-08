#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define ALPHABET_SIZE 256

void compute_bad_char_table(const char *pattern, int m, int bad_char[ALPHABET_SIZE]) {
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        bad_char[i] = -1;
    }
    for (int i = 0; i < m; i++) {
        bad_char[(unsigned char)pattern[i]] = i;
    }
}

void compute_good_suffix_table(const char *pattern, int m, int *good_suffix) {
    int *suffix = malloc(sizeof(int) * m);
    int f = 0;
    int g = m - 1;

    suffix[m - 1] = m;

    for (int i = m - 2; i >= 0; i--) {
        if (i > g && suffix[i + m - 1 - f] < i - g) {
            suffix[i] = suffix[i + m - 1 - f];
        } else {
            if (i < g) {
                g = i;
            }
            f = i;
            while (g >= 0 && pattern[g] == pattern[g + m - 1 - f]) {
                g--;
            }
            suffix[i] = f - g;
        }
    }

    for (int i = 0; i < m; i++) {
        good_suffix[i] = m;
    }

    int j = 0;
    for (int i = m - 1; i >= 0; i--) {
        if (suffix[i] == i + 1) {
            for (; j < m - 1 - i; j++) {
                if (good_suffix[j] == m) {
                    good_suffix[j] = m - 1 - i;
                }
            }
        }
    }

    for (int i = 0; i <= m - 2; i++) {
        good_suffix[m - 1 - suffix[i]] = m - 1 - i;
    }

    free(suffix);
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

void boyer_moore_search(const char *text, const char *pattern) {
    int n = strlen(text);
    int m = strlen(pattern);

    if (m == 0 || n == 0 || m > n) {
        printf("Pattern not found\n");
        return;
    }

    int bad_char[ALPHABET_SIZE];
    int *good_suffix = malloc(sizeof(int) * m);

    compute_bad_char_table(pattern, m, bad_char);
    compute_good_suffix_table(pattern, m, good_suffix);

    int shift = 0;
    int matches = 0;

    while (shift <= n - m) {
        int j = m - 1;

        while (j >= 0 && pattern[j] == text[shift + j]) {
            j--;
        }

        if (j < 0) {
            printf("Pattern found at index %d\n", shift);
            matches++;
            shift += good_suffix[0];
        } else {
            int bad_char_shift = j - bad_char[(unsigned char)text[shift + j]];
            int good_suffix_shift = good_suffix[j];
            shift += max(bad_char_shift, good_suffix_shift);
        }
    }

    if (matches == 0) {
        printf("Pattern not found\n");
    } else {
        printf("Total matches: %d\n", matches);
    }

    free(good_suffix);
}

int main() {
    const char *text1 = "AABAACAADAABAABA";
    const char *pattern1 = "AABA";

    printf("Text: %s\n", text1);
    printf("Pattern: %s\n", pattern1);
    boyer_moore_search(text1, pattern1);

    printf("\n");

    const char *text2 = "THIS IS A TEST TEXT FOR PATTERN MATCHING WITH BOYER-MOORE ALGORITHM";
    const char *pattern2 = "PATTERN";

    printf("Text: %s\n", text2);
    printf("Pattern: %s\n", pattern2);
    boyer_moore_search(text2, pattern2);

    printf("\n");

    const char *text3 = "abcdefghijklmnopqrstuvwxyz";
    const char *pattern3 = "xyz";

    printf("Text: %s\n", text3);
    printf("Pattern: %s\n", pattern3);
    boyer_moore_search(text3, pattern3);

    printf("\n");

    const char *text4 = "The quick brown fox jumps over the lazy dog";
    const char *pattern4 = "cat";

    printf("Text: %s\n", text4);
    printf("Pattern: %s\n", pattern4);
    boyer_moore_search(text4, pattern4);

    printf("\n");

    const char *text5 = "ababababab";
    const char *pattern5 = "abab";

    printf("Text: %s\n", text5);
    printf("Pattern: %s\n", pattern5);
    boyer_moore_search(text5, pattern5);

    return 0;
}

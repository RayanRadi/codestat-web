
#include <stdio.h>

void print_pairs(int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("(%d,%d)\n", i, j);
        }
    }
}

int count_duplicates(int arr[], int n) {
    int count = 0;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (arr[i] == arr[j]) count++;
        }
    }
    return count;
}

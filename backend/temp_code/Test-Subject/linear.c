
#include <stdio.h>

void print_n_times(int n) {
    for (int i = 0; i < n; i++) {
        printf("%d\n", i);
    }
}

int sum_array(int arr[], int n) {
    int total = 0;
    for (int i = 0; i < n; i++) {
        total += arr[i];
    }
    return total;
}

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define SIZE 100

void parallel_bubble_sort(int arr[], int n) {
    int i, j, temp;
    #pragma omp parallel shared(arr, n) private(i, j, temp)
    for(i = 0; i < n; i++) {
        #pragma omp for schedule(static)
        for(j = 0; j < n-i-1; j++) {
            if(arr[j] > arr[j+1]) {
                temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}



int main() {
    int arr[SIZE];
    int i;
    // initialize array with random values
    for(i = 0; i < SIZE; i++) {
        arr[i] = rand() % 20;
    }

    printf("Original array:\n");
    for(i = 0; i < SIZE; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    // sort the array in parallel
    parallel_bubble_sort(arr, SIZE);

    printf("Sorted array:\n");
    for(i = 0; i < SIZE; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    return 0;
}


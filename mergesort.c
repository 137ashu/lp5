// #include <omp.h> // Include OpenMP library
// #include <stdio.h>
// #include <stdlib.h>

// // Merge two sorted sub-arrays into one sorted array
// void merge(int arr[], int l, int m, int r) {
//     int i, j, k;
//     int n1 = m - l + 1;
//     int n2 = r - m;
//     int L[n1], R[n2];

//     // Copy data to temporary arrays L[] and R[]
//     for (i = 0; i < n1; i++)
//         L[i] = arr[l + i];
//     for (j = 0; j < n2; j++)
//         R[j] = arr[m + 1 + j];

//     // Merge the temporary arrays back into arr[l..r]
//     i = 0;
//     j = 0;
//     k = l;

//     while (i < n1 && j < n2) {
//         if (L[i] <= R[j]) {
//             arr[k] = L[i];
//             i++;
//         }
//         else {
//             arr[k] = R[j];
//             j++;
//         }
//         k++;
//     }

//     // Copy the remaining elements of L[], if there are any
//     while (i < n1) {
//         arr[k] = L[i];
//         i++;
//         k++;
//     }

//     // Copy the remaining elements of R[], if there are any
//     while (j < n2) {
//         arr[k] = R[j];
//         j++;
//         k++;
//     }
// }

// // Sorts an array in ascending order using mergesort algorithm
// void mergesort(int arr[], int l, int r) {
//     // Base case: If there is only one element, it is already sorted
//     if (l < r) {
//         // Calculate the middle index
//         int m = l + (r - l) / 2;

//         // Parallelize the two halves of the array using OpenMP sections
//         #pragma omp parallel sections
//         {
//             #pragma omp section
//             {
//                 mergesort(arr, l, m); // Sort the left half
//             }

//             #pragma omp section
//             {
//                 mergesort(arr, m + 1, r); // Sort the right half
//             }
//         }

//         // Merge the two sorted halves into one sorted array
//         merge(arr, l, m, r);
//     }
// }

// int main() {
//     int arr[] = { 12, 11, 13, 5, 6, 7 };
//     int n = sizeof(arr) / sizeof(arr[0]);

//     printf("Given array is \n");
//     for (int i = 0; i < n; i++)
//         printf("%d ", arr[i]);

//     // Sort the array in parallel using OpenMP
//     #pragma omp parallel
//     {
//         // Execute mergesort function in a single thread to avoid race conditions
//         #pragma omp single nowait
//         mergesort(arr, 0, n - 1);
//     }

//     printf("\nSorted array is \n");
//     for (int i = 0; i < n; i++)
//         printf("%d ", arr[i]);
//     printf("\n");

//     return 0;
// }





#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;

// Merge two sorted sub-arrays of arr[]
void merge(vector<int>& arr, int left, int mid, int right) {
    vector<int> left_arr(mid - left + 1); // create a temporary array to hold the left sub-array
    vector<int> right_arr(right - mid); // create a temporary array to hold the right sub-array

    // Copy the left and right sub-arrays into the temporary arrays
    for(int i = 0; i < left_arr.size(); i++) {
        left_arr[i] = arr[left + i];
    }
    for(int j = 0; j < right_arr.size(); j++) {
        right_arr[j] = arr[mid + 1 + j];
    }

    // Merge the two sorted sub-arrays into the original array
    int i = 0, j = 0, k = left;
    while(i < left_arr.size() && j < right_arr.size()) {
        if(left_arr[i] <= right_arr[j]) {
            arr[k] = left_arr[i];
            i++;
        }
        else {
            arr[k] = right_arr[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of left_arr[] if any
    while(i < left_arr.size()) {
        arr[k] = left_arr[i];
        i++;
        k++;
    }

    // Copy the remaining elements of right_arr[] if any
    while(j < right_arr.size()) {
        arr[k] = right_arr[j];
        j++;
        k++;
    }
}

// Parallel mergesort algorithm
void mergesort(vector<int>& arr, int left, int right) {
    if(left < right) {
        // Compute the middle index of the array
        int mid = left + (right - left) / 2;

        // Recursively sort the left and right halves of the array in parallel
        #pragma omp parallel sections
        {
            #pragma omp section
            {
                mergesort(arr, left, mid);
            }
            #pragma omp section
            {
                mergesort(arr, mid + 1, right);
            }
        }

        // Merge the sorted left and right halves
        merge(arr, left, mid, right);
    }
}

int main() {
    // Test the parallel mergesort algorithm
    vector<int> arr = {5, 3, 8, 6, 2, 7, 1, 4};

    // Sort the array using parallel mergesort
    mergesort(arr, 0, arr.size() - 1);

    // Print the sorted array
    for(int i = 0; i < arr.size(); i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    return 0;
}

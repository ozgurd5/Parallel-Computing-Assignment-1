#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int serial_main(const int argc, char** argv)
{
    srand(time(NULL));
    double start;
    double end;

    printf("Array size (N): %d\n", N);

    int* original_array = (int*)malloc(N * sizeof(int));

    for (int i = 0; i < N; i++)
    {
        original_array[i] = rand() % 1000000;
    }

    printf("--- Quick Sort ---\n");
    start = get_time();
    int* quick_sorted = quick_sort(original_array, N);
    end = get_time();
    printf("Time: %.6f seconds\n\n", end - start);
    free(quick_sorted);

    printf("--- Merge Sort ---\n");
    start = get_time();
    int* merge_sorted = merge_sort(original_array, N);
    end = get_time();
    printf("Time: %.6f seconds\n\n", end - start);
    free(merge_sorted);

    printf("--- Bitonic Sort ---\n");
    start = get_time();
    int* bitonic_sorted = bitonic_sort(original_array, N);
    end = get_time();
    printf("Time: %.6f seconds\n\n", end - start);
    free(bitonic_sorted);

    printf("--- Selection Sort ---\n");
    start = get_time();
    int* selection_sorted = selection_sort(original_array, N);
    end = get_time();
    printf("Time: %.6f seconds\n\n", end - start);
    free(selection_sorted);

    printf("--- Bubble Sort ---\n");
    start = get_time();
    int* bubble_sorted = bubble_sort(original_array, N);
    end = get_time();
    printf("Time: %.6f seconds\n\n", end - start);
    free(bubble_sorted);

    free(original_array);
    return 0;
}
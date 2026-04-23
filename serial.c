#include "common.h"
#include <stdio.h>
#include <stdlib.h>

int serial_main(const int argc, char** argv, const int* original_array)
{
    double start;
    double end;

    printf("Array size (N): %d\n", N);

    printf("--- Quick Sort ---\n");
    start = get_time();
    int* quick_sorted = quick_sort(original_array, N);
    end = get_time();
    printf("Time: %.6f seconds\n", end - start);
    printf("Sorted: %s\n\n", is_sorted(quick_sorted, N) ? "YES" : "NO");
    free(quick_sorted);

    printf("--- Merge Sort ---\n");
    start = get_time();
    int* merge_sorted = merge_sort(original_array, N);
    end = get_time();
    printf("Time: %.6f seconds\n", end - start);
    printf("Sorted: %s\n\n", is_sorted(merge_sorted, N) ? "YES" : "NO");
    free(merge_sorted);

    printf("--- Bitonic Sort ---\n");
    start = get_time();
    int* bitonic_sorted = bitonic_sort(original_array, N);
    end = get_time();
    printf("Time: %.6f seconds\n", end - start);
    printf("Sorted: %s\n\n", is_sorted(bitonic_sorted, N) ? "YES" : "NO");
    free(bitonic_sorted);

    printf("--- Selection Sort ---\n");
    start = get_time();
    int* selection_sorted = selection_sort(original_array, N);
    end = get_time();
    printf("Time: %.6f seconds\n", end - start);
    printf("Sorted: %s\n\n", is_sorted(selection_sorted, N) ? "YES" : "NO");
    free(selection_sorted);

    printf("--- Bubble Sort ---\n");
    start = get_time();
    int* bubble_sorted = bubble_sort(original_array, N);
    end = get_time();
    printf("Time: %.6f seconds\n", end - start);
    printf("Sorted: %s\n\n", is_sorted(bubble_sorted, N) ? "YES" : "NO");
    free(bubble_sorted);

    return 0;
}

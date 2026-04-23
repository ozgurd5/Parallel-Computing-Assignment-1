#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "common.h"

int parallel_main(const int argc, char** argv, const int* original_array)
{
    MPI_Init(&argc, &argv);

    int rank;
    int size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (N % size != 0)
    {
        if (rank == 0)
        {
            printf("Error: N (%d) must be divisible by processor size (%d).\n", N, size);
        }

        MPI_Finalize();
        return 1;
    }

    const int local_n = N / size;
    int* gathered_array = NULL;

    if (rank == 0)
    {
        printf("--- MPI PARALLEL SORTING STARTING ---\n");
        printf("Total processors: %d\n", size);
        printf("Array size (N): %d\n\n", N);
        fflush(stdout);

        gathered_array = (int*)malloc(N * sizeof(int));
    }

    int* local_array = (int*)malloc(local_n * sizeof(int));

    double start_time;
    double end_time;
    double local_elapsed;
    double max_elapsed;

    // ---------------- 1. QUICK SORT (PARALLEL) ----------------
    start_time = MPI_Wtime();
    MPI_Scatter(original_array, local_n, MPI_INT, local_array,
        local_n, MPI_INT, 0, MPI_COMM_WORLD);

    int* local_sorted_quick = quick_sort(local_array, local_n);

    MPI_Gather(local_sorted_quick, local_n, MPI_INT, gathered_array,
        local_n, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        for (int i = 1; i < size; i++)
        {
            merge(gathered_array, 0, i * local_n - 1, (i + 1) * local_n - 1);
        }
    }

    end_time = MPI_Wtime();
    local_elapsed = end_time - start_time;
    MPI_Reduce(&local_elapsed, &max_elapsed, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        printf("Parallel Quick Sort Time: %.6f seconds\n", max_elapsed);
        printf("Sorted: %s\n\n", is_sorted(gathered_array, N) ? "YES" : "NO");
        fflush(stdout);
    }
    free(local_sorted_quick);

    // ---------------- 2. MERGE SORT (PARALLEL) ----------------
    MPI_Barrier(MPI_COMM_WORLD);
    start_time = MPI_Wtime();

    MPI_Scatter(original_array, local_n, MPI_INT, local_array,
        local_n, MPI_INT, 0, MPI_COMM_WORLD);

    int* local_sorted_merge = merge_sort(local_array, local_n);

    MPI_Gather(local_sorted_merge, local_n, MPI_INT, gathered_array,
        local_n, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        for (int i = 1; i < size; i++)
        {
            merge(gathered_array, 0, i * local_n - 1, (i + 1) * local_n - 1);
        }
    }

    end_time = MPI_Wtime();
    local_elapsed = end_time - start_time;
    MPI_Reduce(&local_elapsed, &max_elapsed, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        printf("Parallel Merge Sort Time: %.6f seconds\n", max_elapsed);
        printf("Sorted: %s\n\n", is_sorted(gathered_array, N) ? "YES" : "NO");
        fflush(stdout);
    }
    free(local_sorted_merge);

    // ---------------- 3. BITONIC SORT (PARALLEL) ----------------
    MPI_Barrier(MPI_COMM_WORLD);
    start_time = MPI_Wtime();

    MPI_Scatter(original_array, local_n, MPI_INT, local_array,
        local_n, MPI_INT, 0, MPI_COMM_WORLD);

    int* local_sorted_bitonic = bitonic_sort(local_array, local_n);

    MPI_Gather(local_sorted_bitonic, local_n, MPI_INT, gathered_array,
        local_n, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        for (int i = 1; i < size; i++)
        {
            merge(gathered_array, 0, i * local_n - 1, (i + 1) * local_n - 1);
        }
    }

    end_time = MPI_Wtime();
    local_elapsed = end_time - start_time;
    MPI_Reduce(&local_elapsed, &max_elapsed, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        printf("Parallel Bitonic Sort Time: %.6f seconds\n", max_elapsed);
        printf("Sorted: %s\n\n", is_sorted(gathered_array, N) ? "YES" : "NO");
        fflush(stdout);
    }
    free(local_sorted_bitonic);

    // ---------------- 4. SELECTION SORT (PARALLEL) ----------------
    MPI_Barrier(MPI_COMM_WORLD);
    start_time = MPI_Wtime();

    MPI_Scatter(original_array, local_n, MPI_INT, local_array,
        local_n, MPI_INT, 0, MPI_COMM_WORLD);

    int* local_sorted_selection = selection_sort(local_array, local_n);

    MPI_Gather(local_sorted_selection, local_n, MPI_INT, gathered_array,
        local_n, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        for (int i = 1; i < size; i++)
        {
            merge(gathered_array, 0, i * local_n - 1, (i + 1) * local_n - 1);
        }
    }

    end_time = MPI_Wtime();
    local_elapsed = end_time - start_time;
    MPI_Reduce(&local_elapsed, &max_elapsed, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        printf("Parallel Selection Sort Time: %.6f seconds\n", max_elapsed);
        printf("Sorted: %s\n\n", is_sorted(gathered_array, N) ? "YES" : "NO");
        fflush(stdout);
    }
    free(local_sorted_selection);

    // ---------------- 5. BUBBLE SORT (PARALLEL) ----------------
    MPI_Barrier(MPI_COMM_WORLD);
    start_time = MPI_Wtime();

    MPI_Scatter(original_array, local_n, MPI_INT, local_array, local_n, MPI_INT, 0, MPI_COMM_WORLD);

    int* local_sorted_bubble = bubble_sort(local_array, local_n);

    MPI_Gather(local_sorted_bubble, local_n, MPI_INT, gathered_array, local_n, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        for (int i = 1; i < size; i++)
        {
            merge(gathered_array, 0, i * local_n - 1, (i + 1) * local_n - 1);
        }
    }

    end_time = MPI_Wtime();
    local_elapsed = end_time - start_time;
    MPI_Reduce(&local_elapsed, &max_elapsed, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        printf("Parallel Bubble Sort Time: %.6f seconds\n", max_elapsed);
        printf("Sorted: %s\n\n", is_sorted(gathered_array, N) ? "YES" : "NO");
        fflush(stdout);
    }
    free(local_sorted_bubble);

    free(local_array);
    if (rank == 0)
    {
        free(gathered_array);
    }

    MPI_Finalize();
    return 0;
}

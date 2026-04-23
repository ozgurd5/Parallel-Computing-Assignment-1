#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "common.h"

typedef int* (*sort_func)(const int*, int);

void run_parallel_sort(const char* name,
                              const sort_func sort_fn,
                              const int* original_array,
                              int* local_array,
                              int* gathered_array,
                              const int local_n,
                              const int size,
                              const int rank)
{
    MPI_Barrier(MPI_COMM_WORLD);
    const double start_time = MPI_Wtime();

    MPI_Scatter(original_array, local_n, MPI_INT, local_array,
        local_n, MPI_INT, 0, MPI_COMM_WORLD);

    int* local_sorted = sort_fn(local_array, local_n);

    MPI_Gather(local_sorted, local_n, MPI_INT, gathered_array,
        local_n, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        for (int i = 1; i < size; i++)
        {
            merge(gathered_array, 0, i * local_n - 1, (i + 1) * local_n - 1);
        }
    }

    const double end_time = MPI_Wtime();
    const double local_elapsed = end_time - start_time;
    double max_elapsed;
    MPI_Reduce(&local_elapsed, &max_elapsed, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        printf("--- Parallel %s ---\n", name);
        printf("Time: %.6f seconds\n", max_elapsed);
        printf("Sorted: %s\n\n", is_sorted(gathered_array, N) ? "YES" : "NO");
        fflush(stdout);
    }

    free(local_sorted);
}

void run_parallel_sort_tree(const char* name,
                            const sort_func sort_fn,
                            const int* original_array,
                            int* local_array,
                            const int local_n,
                            const int size,
                            const int rank)
{
    MPI_Barrier(MPI_COMM_WORLD);
    const double start_time = MPI_Wtime();

    MPI_Scatter(original_array, local_n, MPI_INT, local_array,
        local_n, MPI_INT, 0, MPI_COMM_WORLD);

    int* sorted = sort_fn(local_array, local_n);
    int current_size = local_n;

    int step = 1;
    int active = 1;

    while (step < size && active)
    {
        if (rank % (2 * step) == 0)
        {
            if (rank + step < size)
            {
                int recv_size;
                MPI_Recv(&recv_size, 1, MPI_INT, rank + step, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

                int* recv_buf = (int*)malloc(recv_size * sizeof(int));
                MPI_Recv(recv_buf, recv_size, MPI_INT, rank + step, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

                const int merged_size = current_size + recv_size;
                int* merged = (int*)malloc(merged_size * sizeof(int));

                int i = 0;
                int j = 0;
                int k = 0;

                while (i < current_size && j < recv_size)
                {
                    if (sorted[i] <= recv_buf[j])
                    {
                        merged[k++] = sorted[i++];
                    }
                    else
                    {
                        merged[k++] = recv_buf[j++];
                    }
                }

                while (i < current_size)
                {
                    merged[k++] = sorted[i++];
                }

                while (j < recv_size)
                {
                    merged[k++] = recv_buf[j++];
                }

                free(sorted);
                free(recv_buf);
                sorted = merged;
                current_size = merged_size;
            }
        }
        else
        {
            MPI_Send(&current_size, 1, MPI_INT, rank - step, 0, MPI_COMM_WORLD);
            MPI_Send(sorted, current_size, MPI_INT, rank - step, 0, MPI_COMM_WORLD);
            active = 0;
        }

        step *= 2;
    }

    const double end_time = MPI_Wtime();
    const double local_elapsed = end_time - start_time;
    double max_elapsed;
    MPI_Reduce(&local_elapsed, &max_elapsed, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        printf("--- Parallel %s (Tree Merge) ---\n", name);
        printf("Time: %.6f seconds\n", max_elapsed);
        printf("Sorted: %s\n\n", is_sorted(sorted, N) ? "YES" : "NO");
        fflush(stdout);
    }

    free(sorted);
}

int parallel_main(const int argc, char** argv, const int* original_array)
{
    printf("Running parallel algorithms\n");

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

    if (rank == 0)
    {
        printf("--- MPI PARALLEL SORTING STARTING ---\n");
        printf("Total processors: %d\n", size);
        printf("Array size (N): %d\n\n", N);
        fflush(stdout);
    }

    int* local_array = (int*)malloc(local_n * sizeof(int));

    run_parallel_sort_tree("Quick Sort", quick_sort, original_array,
        local_array, local_n, size, rank);

    run_parallel_sort_tree("Merge Sort", merge_sort, original_array,
        local_array, local_n, size, rank);

    run_parallel_sort_tree("Bitonic Sort", bitonic_sort, original_array,
        local_array, local_n, size, rank);

    run_parallel_sort_tree("Selection Sort", selection_sort, original_array,
        local_array, local_n, size, rank);

    run_parallel_sort_tree("Bubble Sort", bubble_sort, original_array,
        local_array, local_n, size, rank);

    free(local_array);

    MPI_Finalize();
    return 0;
}

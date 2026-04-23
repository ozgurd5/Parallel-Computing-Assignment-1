#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include "common.h"
#include "serial.h"
#include "parallel.h"

int main(int argc, char** argv)
{
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int* original_array = NULL;

    if (rank == 0)
    {
        srand(time(NULL));
        original_array = (int*)malloc(N * sizeof(int));

        for (int i = 0; i < N; i++)
        {
            original_array[i] = rand() % 1000000;
        }

        serial_main(argc, argv, original_array);
    }

    parallel_main(argc, argv, original_array);

    if (rank == 0)
    {
        free(original_array);
    }

    MPI_Finalize();
    return 0;
}

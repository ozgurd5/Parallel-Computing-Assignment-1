#include <stdlib.h>
#include <time.h>
#include "common.h"
#include "serial.h"
#include "parallel.h"

int main(const int argc, char** argv)
{
    srand(time(NULL));

    int* original_array = (int*)malloc(N * sizeof(int));

    for (int i = 0; i < N; i++)
    {
        original_array[i] = rand() % 1000000;
    }

    serial_main(argc, argv, original_array);
    parallel_main(argc, argv, original_array);

    free(original_array);
    return 0;
}

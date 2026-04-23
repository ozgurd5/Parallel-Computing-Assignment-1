#include "serial.h"
#include "parallel.h"

int main(const int argc, char** argv)
{
    // serial_main(argv, argv);
    parallel_main(argc, argv);

    return 0;
}

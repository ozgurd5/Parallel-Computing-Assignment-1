cmake_minimum_required(VERSION 4.2)
project(Parallel_Computing_Assignment_1 C)

set(CMAKE_C_STANDARD 17)

include_directories("C:/Program Files (x86)/Microsoft SDKs/MPI/Include")

link_directories("C:/Program Files (x86)/Microsoft SDKs/MPI/Lib/x64")

add_executable(Parallel_Computing_Assignment_1
        main.c
        common.c
        common.h
        serial.c
        serial.h
        parallel.c
        parallel.h)

target_link_libraries(Parallel_Computing_Assignment_1 msmpi)
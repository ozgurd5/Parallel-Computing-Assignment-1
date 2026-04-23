cmake_minimum_required(VERSION 4.2)
project(Parallel_Computing_Assignment C)

set(CMAKE_C_STANDARD 17)

add_executable(Parallel_Computing_Assignment
        main.c
        parallel.c
        serial.c
        serial.h)
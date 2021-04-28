cmake_minimum_required(VERSION 3.17)
project(Coursework_2_c C)

set(CMAKE_C_STANDARD 11)

add_executable(Coursework_2_c
        main.c
        grid.h grid.c
        helpers.h helpers.c
        cluster_finder.h cluster_finder.c
        pos.c pos.h)

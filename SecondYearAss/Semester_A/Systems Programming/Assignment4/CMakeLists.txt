cmake_minimum_required(VERSION 3.27)
project(Assignment4 C)

set(CMAKE_C_STANDARD 11)

add_executable(Assignment4 encodemain.c
        compress.h
        compress.c)

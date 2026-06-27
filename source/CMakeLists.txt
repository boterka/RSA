cmake_minimum_required(VERSION 3.26)
project(NagyHazi C)

set(CMAKE_C_STANDARD 99)

add_executable(NagyHazi main.c
        mtwister.c
        Funcs.c
        Funcs.h
        debugmalloc.h
)

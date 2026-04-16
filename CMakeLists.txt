cmake_minimum_required(VERSION 3.30)
project(vector C)

set(CMAKE_C_STANDARD 23)

add_library(cvec STATIC src/cvec.c
        include/cvec/cvec.h
        src/unlikely.h
        examples/basic_usage/main.c)

target_include_directories(cvec PUBLIC include)
target_include_directories(cvec PRIVATE src)
add_subdirectory(examples)
cmake_minimum_required(VERSION 3.30)
project(vector C)

set(CMAKE_C_STANDARD 23)

add_library(vector STATIC src/library.c)

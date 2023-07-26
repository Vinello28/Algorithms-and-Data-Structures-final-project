cmake_minimum_required(VERSION 3.25)
project(Progetto C)

set(CMAKE_C_STANDARD 11)
add_executable(Progetto main.c)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -std=gnu11 -O2 -pipe -g ")
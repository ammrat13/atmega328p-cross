cmake_minimum_required(VERSION 3.21.0)

project(main)

add_avr_executable(main main.c)
add_compile_options(-Wall -Wextra)

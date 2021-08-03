cmake_minimum_required(VERSION 3.21.0)

project("atmega328p-cross" LANGUAGES CXX)


add_subdirectory("main"   "main/build")
add_subdirectory("serial" "serial/build")

add_dependencies("main" "serial")

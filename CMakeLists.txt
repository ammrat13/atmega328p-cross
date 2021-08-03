cmake_minimum_required(VERSION 3.21.0)

project("atmega328p-cross" LANGUAGES CXX)


add_subdirectory("src/main")
add_subdirectory("src/serial")

cmake_minimum_required(VERSION 3.21.0)

project("atmega328p-cross" LANGUAGES CXX)

# If we need debug options, add them to all binaries
if( (CMAKE_BUILD_TYPE STREQUAL "Debug")
 OR (CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo"))
    add_compile_options("-g")
endif()


add_subdirectory("src/main")

add_subdirectory("src/serial")
add_subdirectory("src/ymodem")

cmake_minimum_required(VERSION 3.21.0)

project("serial" LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

add_library(
    "serial"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/serial_error.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/serial_config.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/serial_put.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/serial_get.cpp"
)
target_compile_options(
    "serial" PRIVATE
    "-Os"
    "-Wall" "-Wextra" "-Werror"
    "-ffunction-sections" "-fdata-sections"
)

target_include_directories(
    "serial" PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include/"
    "${CMAKE_CURRENT_BINARY_DIR}/include/"
)

set_target_properties(
    "serial" PROPERTIES
    PUBLIC_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/include/serial.hpp"
)
install(TARGETS "serial")

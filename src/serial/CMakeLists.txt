cmake_minimum_required(VERSION 3.21.0)

project("serial" LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)


add_compile_options("-Os")
add_compile_options("-Wall" "-Wextra" "-Werror")
add_compile_options("-ffunction-sections" "-fdata-sections")


add_library(
    "serial"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/serial_error.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/serial_config.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/serial_put.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/serial_get.cpp"
)
target_include_directories(
    "serial"
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/"
)

set_target_properties(
    "serial" PROPERTIES
    PUBLIC_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/include/serial.hpp"
)
install(TARGETS "serial")

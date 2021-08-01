cmake_minimum_required(VERSION 3.21.0)

project("Main" LANGUAGES CXX)


add_compile_options("-Os")
add_compile_options("-Wall" "-Wextra" "-Werror")
add_compile_options("-ffunction-sections" "-fdata-sections")


add_avr_executable("main" "main.cpp")

find_path(   LIBSERIAL_INCLUDE_PATH "serial.hpp" REQUIRED)
find_library(LIBSERIAL_LIBRARY_PATH "serial"     REQUIRED)
target_include_directories("main" PRIVATE "${LIBSERIAL_INCLUDE_PATH}")
target_link_libraries(     "main" PRIVATE "${LIBSERIAL_LIBRARY_PATH}")

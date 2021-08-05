cmake_minimum_required(VERSION 3.21.0)

project("main" LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)


add_compile_options("-Os")
add_compile_options("-Wall" "-Wextra" "-Werror")
add_compile_options("-ffunction-sections" "-fdata-sections")


add_avr_executable("main" "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")

target_link_libraries("main" "serial")

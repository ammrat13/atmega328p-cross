cmake_minimum_required(VERSION 3.21.0)

project("serial" LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)


add_compile_options("-Os")
add_compile_options("-Wall" "-Wextra" "-Werror")
add_compile_options("-ffunction-sections" "-fdata-sections")


add_library(
    "serial"
    "src/serial.cpp"
)
target_include_directories(
    "serial"
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/"
)
install(TARGETS "serial")

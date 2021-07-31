cmake_minimum_required(VERSION 3.21.0)

project("libserial")


add_compile_options("-Os")
add_compile_options("-Wall" "-Wextra" "-Werror")
add_compile_options("-ffunction-sections" "-fdata-sections")


add_library(
    "serial"
    "serial.cpp"
)
set_target_properties(
    "serial" PROPERTIES
    PUBLIC_HEADER "serial.hpp"
)
install(TARGETS "serial")

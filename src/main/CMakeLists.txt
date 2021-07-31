cmake_minimum_required(VERSION 3.21.0)

project("main")


add_compile_options("-Os")
add_compile_options("-Wall" "-Wextra" "-Werror")
add_compile_options("-ffunction-sections" "-fdata-sections")

add_link_options("-Wl,-gc-sections")


add_avr_executable("main" "main.c")

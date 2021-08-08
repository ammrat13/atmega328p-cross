cmake_minimum_required(VERSION 3.21.0)
include(GNUInstallDirs)

project("main" LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)


add_avr_executable("main" "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")
target_compile_options(
    "main" PRIVATE
    "-Os"
    "-Wall" "-Wextra" "-Werror"
    "-ffunction-sections" "-fdata-sections"
)
# Garbage collecting sections can mask errors
# So only do this if we are releasing
if( (CMAKE_BUILD_TYPE STREQUAL "Release")
 OR (CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
 OR (CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo"))
    target_link_options(
        "main" PRIVATE
        "-Wl,-gc-sections"
    )
endif()

target_link_libraries("main" "serial")

cmake_minimum_required(VERSION 3.21.0)
include(GNUInstallDirs)

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
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

set_target_properties(
    "serial" PROPERTIES
    PUBLIC_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/include/serial.hpp"
)
install(
    TARGETS "serial"
    EXPORT "serial-export"
    CONFIGURATIONS "Release" "MinSizeRel" "RelWithDebInfo"
)
install(
    EXPORT "serial-export"
    CONFIGURATIONS "Release" "MinSizeRel" "RelWithDebInfo"
    FILE "serial-config.cmake"
    NAMESPACE "serial::"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/serial/"
)

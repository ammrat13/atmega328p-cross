cmake_minimum_required(VERSION 3.21.0)
include(GNUInstallDirs)

project("serial" LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)


add_library(
    "serial"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/error.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/config.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/put.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/get.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/flush.cpp"
)
target_compile_options(
    "serial" PRIVATE
    "-Os"
    "-Wall" "-Wextra" "-Werror"
    "-ffunction-sections" "-fdata-sections"
)
# Have separate directories for build and install
# Needed because we'll be installing a CMake configuration too
target_include_directories(
    "serial" PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)


# Install the includes along with the library
# Have to do this before the "install"
set_target_properties(
    "serial" PROPERTIES
    PUBLIC_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/include/serial.hpp"
    PRIVATE_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/include/registers.hpp"
)
install(
    TARGETS "serial"
    EXPORT "serial-export"
    CONFIGURATIONS "Release" "MinSizeRel" "RelWithDebInfo"
)
# Export the target so we can use it in other CMake projects
# Have to do this after the "install"
install(
    EXPORT "serial-export"
    CONFIGURATIONS "Release" "MinSizeRel" "RelWithDebInfo"
    FILE "serial-config.cmake"
    NAMESPACE "serial::"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/serial/"
)

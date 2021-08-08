cmake_minimum_required(VERSION 3.21.0)
include(GNUInstallDirs)

project("ymodem" LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)


add_library(
    "ymodem"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/ymodem.cpp"
)
target_compile_options(
    "ymodem" PRIVATE
    "-Os"
    "-Wall" "-Wextra" "-Werror"
    "-ffunction-sections" "-fdata-sections"
)
# Have separate directories for build and install
# Needed because we'll be installing a CMake configuration too
target_include_directories(
    "ymodem" PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)
target_link_libraries(
    "ymodem" PUBLIC
    "serial"
)


# Install the includes along with the library
# Have to do this before the "install"
set_target_properties(
    "ymodem" PROPERTIES
    PUBLIC_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/include/ymodem.hpp"
)
install(
    TARGETS "ymodem"
    EXPORT "ymodem-export"
    CONFIGURATIONS "Release" "MinSizeRel" "RelWithDebInfo"
)
# Export the target so we can use it in other CMake projects
# Have to do this after the "install"
install(
    EXPORT "ymodem-export"
    CONFIGURATIONS "Release" "MinSizeRel" "RelWithDebInfo"
    FILE "ymodem-config.cmake"
    NAMESPACE "ymodem::"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/ymodem/"
)


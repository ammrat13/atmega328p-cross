# Generic CMake toolchain for AVR targets
#
# Expected to be included by a particular platform's toolchain file. Requires
# the following variables to be set:
# * LINKER_NAME: The name to pass to `gcc` and `ld`
# * UPLOAD_NAME: The name to pass to `avrdude`
# * UPLOAD_PROGRAMMER: The programmer for `avrdude` to use
# * CPU_FREQ: The CPU's frequency (in MHz)


# Use Generic for bare-metal targets
set(CMAKE_SYSTEM_NAME      "Generic")
set(CMAKE_SYSTEM_PROCESSOR "avr")


# Control where CMake looks for programs and libraries
# The user must specify CMAKE_FIND_ROOT_PATH if they want to use it
set(
    CMAKE_FIND_ROOT_PATH "/usr/lib/gcc/avr/11.1.0" CACHE STRING
    "A semicolon-separated list of directories to use when searching for includes and libraries"
)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM BOTH)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)


# Find the required programs
# Allow the user to change the compiler prefix
set(
    AVR_CROSS_PROGRAM_PREFIX "avr-" CACHE STRING
    "The string to be prepended to the names of the toolchain's programs"
)
find_program(CMAKE_AS           "${AVR_CROSS_PROGRAM_PREFIX}as"      REQUIRED)
find_program(CMAKE_C_COMPILER   "${AVR_CROSS_PROGRAM_PREFIX}gcc"     REQUIRED)
find_program(CMAKE_CXX_COMPILER "${AVR_CROSS_PROGRAM_PREFIX}g++"     REQUIRED)
find_program(CMAKE_LINKER       "${AVR_CROSS_PROGRAM_PREFIX}gcc"     REQUIRED)
find_program(CMAKE_OBJCOPY      "${AVR_CROSS_PROGRAM_PREFIX}objcopy" REQUIRED)
find_program(CMAKE_AR           "${AVR_CROSS_PROGRAM_PREFIX}ar"      REQUIRED)
find_program(CMAKE_RANLIB       "${AVR_CROSS_PROGRAM_PREFIX}ranlib"  REQUIRED)

# Don't compile dynamic libraries
set(BUILD_SHARED_LIBS OFF)

# Set required compilation and linker flags
# We need some of these for AVR-LibC
add_compile_options("-mmcu=${LINKER_NAME}")
add_compile_options("-Os")
add_definitions("-DF_CPU=${CPU_FREQ}")
add_link_options("-mmcu=${LINKER_NAME}")


# Program and flags to upload code
# Must be avrdude with this configuration
find_program(AVR_UPLOAD       "avrdude")
set(
    AVR_UPLOAD_FLAGS "-p" "${UPLOAD_NAME}" "-c" "${UPLOAD_PROGRAMMER}"
)
set(
    AVR_UPLOAD_PORT "/dev/ttyACM0" CACHE FILEPATH
    "The serial port to use for uploading programs to the device"
)


# Add a command for generating an AVR "executable"
# It compiles the ELF then OBJCOPYs it to an IHEX
function(add_avr_executable executable)

    # Start by compiling the ELF file normally
    add_executable("${executable}" ${ARGN})
    set_target_properties("${executable}" PROPERTIES SUFFIX ".elf")

    # Generate the IHEX file with OBJCOPY
    add_custom_command(
        OUTPUT "${executable}.hex"
        COMMAND ${CMAKE_OBJCOPY} -O ihex
                                "$<TARGET_FILE:${executable}>"
                                "${executable}.hex"
        DEPENDS "$<TARGET_FILE:${executable}>"
        VERBATIM
    )

    # Add a target to upload it
    add_custom_target(
        "${executable}_upload"
        COMMAND ${AVR_UPLOAD} ${AVR_UPLOAD_FLAGS}
                              -P "${AVR_UPLOAD_PORT}"
                              -U "flash:w:${executable}.hex:i"
        DEPENDS "${executable}.hex"
    )

endfunction()

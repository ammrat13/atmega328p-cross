# CMake Toolchain for ATmega328P

# Use Generic for bare-metal targets
set(CMAKE_SYSTEM_NAME      "Generic")
set(CMAKE_SYSTEM_PROCESSOR "avr")


# Control where CMake looks for programs and libraries
set(CMAKE_FIND_ROOT_PATH "/usr/lib/gcc/avr/11.1.0/")
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)


# Find the required programs
# Have a variable to control different prefixes
set(TARGET "avr-")
find_program(CMAKE_AS           "${TARGET}as"      REQUIRED)
find_program(CMAKE_C_COMPILER   "${TARGET}gcc"     REQUIRED)
find_program(CMAKE_CXX_COMPILER "${TARGET}g++"     REQUIRED)
find_program(CMAKE_LINKER       "${TARGET}gcc"     REQUIRED)
find_program(CMAKE_OBJCOPY      "${TARGET}objcopy" REQUIRED)
find_program(CMAKE_AR           "${TARGET}ar"      REQUIRED)
find_program(CMAKE_RANLIB       "${TARGET}ranlib"  REQUIRED)

# Don't compile dynamic libraries
set(BUILD_SHARED_LIBS OFF)

# Set required compilation flags
# Make sure to at least optimize for size, otherwise avr-libc fails
add_compile_options("-mmcu=atmega328p")
add_compile_options("-Os")

# Set required definitions
add_definitions("-DF_CPU=16000000")


# Program to upload code
# Must be avrdude with this configuration
find_program(AVR_UPLOAD "avrdude")

# Settings for the uploader
set(AVR_UPLOAD_PORT  "/dev/ttyACM0")
set(AVR_UPLOAD_FLAGS "-p" "m328p" "-c" "arduino")


# Add a command for generating an AVR "executable"
# It compiles the ELF then OBJCOPYs it to an IHEX
function(add_avr_executable executable)

    # Start by compiling the ELF file normally
    add_executable("${executable}.elf" ${ARGN})

    # Generate the IHEX file with OBJCOPY
    add_custom_command(
        OUTPUT "${executable}.hex"
        COMMAND ${CMAKE_OBJCOPY} -O ihex
                                "$<TARGET_FILE:${executable}.elf>"
                                "${executable}.hex"
        DEPENDS "${executable}.elf"
        VERBATIM
    )
    add_custom_target(
        "${executable}" ALL
        DEPENDS "${executable}.hex"
        VERBATIM
    )

    # Add a target to upload it
    add_custom_target(
        "${executable}_upload"
        COMMAND ${AVR_UPLOAD} ${AVR_UPLOAD_FLAGS}
                              -P ${AVR_UPLOAD_PORT}
                              -U "flash:w:${executable}.hex:i"
        DEPENDS "${executable}"
    )

endfunction()

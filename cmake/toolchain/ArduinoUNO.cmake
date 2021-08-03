# CMake toolchain for the Arduino UNO

set(LINKER_NAME       "atmega328p")
set(UPLOAD_NAME       "m328p")
set(UPLOAD_PROGRAMMER "arduino")
set(CPU_FREQ          "16000000")

include("${CMAKE_CURRENT_LIST_DIR}/AVR.cmake")

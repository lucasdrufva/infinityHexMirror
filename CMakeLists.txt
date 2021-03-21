# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# initalize pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH "/Users/lucas/Documents/pico/pico-sdk")

# Pull in Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(mirror C CXX ASM)

set(PICO_CXX_ENABLE_EXCEPTIONS 1)

set(PICO_CXX_ENABLE_RTTI 1)

# Initialise the Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(mirror mirror.c )
pico_generate_pio_header(mirror ${CMAKE_CURRENT_LIST_DIR}/ws2812.pio)

pico_set_program_name(mirror "mirror")
pico_set_program_version(mirror "0.1")

pico_enable_stdio_uart(mirror 0)
pico_enable_stdio_usb(mirror 1)

# Add the standard library to the build
target_link_libraries(mirror PRIVATE pico_stdlib hardware_pwm pico_servo hardware_pio)

pico_add_extra_outputs(mirror)

add_subdirectory(servo)


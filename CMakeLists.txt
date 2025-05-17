# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(internet C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()
add_subdirectory(core0/libs/build_in/one_wire)
# Add executable. Default name is the project name, version 0.1

add_executable(internet 
    internet.cpp

    )

pico_set_program_name(internet "internet")
pico_set_program_version(internet "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(internet 0)
pico_enable_stdio_usb(internet 1)

# Add the standard library to the build
target_link_libraries(internet
        pico_stdlib)

# Add the standard include files to the build
target_include_directories(internet PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

# Add any user requested libraries
target_link_libraries(internet 
        hardware_i2c
        hardware_watchdog
        pico_multicore
        pico_one_wire
        pico_stdlib
        hardware_gpio
        pico_cyw43_arch_lwip_threadsafe_background
        pico_lwip
        hardware_rtc
        )

pico_add_extra_outputs(internet)


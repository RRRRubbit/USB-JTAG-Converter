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
set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD seeed_xiao_rp2040 CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(USB-JTAG-Converter C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(USB-JTAG-Converter 
        USB-JTAG-Converter.c 
        led.c 
        usb_task.c 
        jtag_task.c
        #get_serial.c
        #usb_descriptors.c
)

pico_set_program_name(USB-JTAG-Converter "USB-JTAG-Converter")
pico_set_program_version(USB-JTAG-Converter "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(USB-JTAG-Converter 0)
pico_enable_stdio_usb(USB-JTAG-Converter 0)

# Add the standard library to the build
target_link_libraries(USB-JTAG-Converter
        pico_stdlib)

# Add the standard include files to the build
target_include_directories(USB-JTAG-Converter PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
)

pico_add_extra_outputs(USB-JTAG-Converter)
pico_enable_stdio_usb(${PROJECT_NAME} 1) # 启用 USB
target_compile_definitions(${PROJECT_NAME} PRIVATE CFG_TUD_CDC=1)


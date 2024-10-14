# == DO NEVER EDIT THE NEXT LINES for Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.0.0)
set(toolchainVersion 13_2_Rel1)
set(picotoolVersion 2.0.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
# Generated CMake Pico project file
cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.0.0)
set(toolchainVersion 13_2_Rel1)
set(picotoolVersion 2.0.0)
include(${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)

set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(umor_pico C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1
add_executable(umor_pico umor_pico.cpp
    /Users/tilman/pico-sdk/lib/RadioLib/src/modules/SX126x/SX1262.cpp
    /Users/tilman/pico-sdk/lib/RadioLib/src/modules/SX126x/SX126x.cpp
    /Users/tilman/pico-sdk/lib/RadioLib/src/protocols/PhysicalLayer/PhysicalLayer.cpp
    /Users/tilman/pico-sdk/lib/RadioLib/src/Module.cpp
    /Users/tilman/pico-sdk/lib/RadioLib/src/modules/SX126x/SX126x_LR_FHSS.cpp
    /Users/tilman/pico-sdk/lib/RadioLib/src/utils/CRC.cpp
    /Users/tilman/pico-sdk/lib/RadioLib/src/utils/FEC.cpp
)

pico_set_program_name(umor_pico "umor_pico")
pico_set_program_version(umor_pico "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(umor_pico 0)
pico_enable_stdio_usb(umor_pico 1)


# Add the LoRa lib
target_link_libraries(umor_pico pico_stdlib
                    /Users/tilman/pico-sdk/lib/RadioLib/build/libRadioLib.a
)

target_include_directories(umor_pico PRIVATE /Users/tilman/pico-sdk/lib/RadioLib/src)

# Add the standard include files to the build
target_include_directories(umor_pico PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/.. # for any other standard includes, if required
)

# Ensure these directories are included
target_include_directories(umor_pico PRIVATE
  ${PICO_SDK_PATH}/src/common/pico_stdlib
  ${PICO_SDK_PATH}/src/boards/include
  ${PICO_SDK_PATH}/src/rp2040/hardware_gpio/include
  ${PICO_SDK_PATH}/src/rp2040/hardware_i2c/include
  ${PICO_SDK_PATH}/src/rp2040/hardware_timer/include
  ${PICO_SDK_PATH}/src/rp2040/hardware_watchdog/include
  ${PICO_SDK_PATH}/src/rp2040/hardware_clocks/include
  /Users/tilman/pico-sdk/lib/lwip/src/include # Added for lwipopts.h
  /Users/tilman/pico-sdk/lib/RadioLib/examples/NonArduino/Pico/ # Added for PicoHal.h
  /Users/tilman/pico-sdk/src/rp2_common/hardware_pwm/include # Added to get all the hardware libs of rp2040
  /Users/tilman/pico-sdk/src/rp2_common/pico_multicore/include
)

target_sources(umor_pico PRIVATE /Users/tilman/pico-sdk/lib/RadioLib/src/Hal.cpp
/Users/tilman/pico-sdk/lib/RadioLib/src/modules/SX126x/SX1262.h        
)



# Add any user requested libraries
target_link_libraries(umor_pico 
        hardware_i2c
        hardware_interp
        hardware_timer
        hardware_watchdog
        hardware_clocks
        hardware_spi
        hardware_pwm
        pico_multicore
)

if (PICO_CYW43_SUPPORTED)
    target_link_libraries(umor_pico pico_cyw43_arch_none)
endif()
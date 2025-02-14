cmake_minimum_required(VERSION 3.28)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

set(CMAKE_PROJECT_NAME devcontainer-stm32-cubemx-eeprom)

include("cmake/gcc-arm-none-eabi.cmake")

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

enable_language(C CXX ASM)

project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

add_executable(${CMAKE_PROJECT_NAME})

add_subdirectory(cmake/stm32cubemx)

target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
)

target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    "Core/Src/system_clock.c"
    "Core/Src/stm32l4xx_it.c"
    "Core/Src/stm32l4xx_hal_msp.c"
    "Core/Src/system_stm32l4xx.c"
    "Core/Src/sysmem.c"
    "Core/Src/syscalls.c"
    "Core/Src/main.cpp"
    "Core/Src/usart.c"
    "Core/Inc/utility.hpp"
    "Core/Inc/i2c_device.hpp"
    "Core/Inc/spi_device.hpp"
    "Core/Src/i2c_device.cpp"
    "Core/Src/spi_device.cpp"
)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    "Core/Inc"
)

target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx
)

target_compile_options(${CMAKE_PROJECT_NAME} PUBLIC
    -std=c++23
    -Wall
    -Wextra
    -Wconversion
    -Wshadow
    -Wpedantic
    -Wnarrowing
    -Wimplicit
    -Waddress
    -pedantic
    -Wdeprecated
    -Wregister
    -Wnon-virtual-dtor
    -Wpessimizing-move
    -Wsign-conversion
    -Wduplicated-cond
    -Wduplicated-branches
    -Wlogical-op
    -Wnull-dereference
    -Wuseless-cast
    -Wdouble-promotion
    -Wimplicit-fallthrough
    -Wcast-align
    -fconcepts
)
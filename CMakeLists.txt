cmake_minimum_required(VERSION 3.16)

set(PICO_SDK_FETCH_FROM_GIT on)

include(pico_sdk_import.cmake)

project(MT)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(MT
src/MT.cpp
)

target_link_libraries(MT
pico_stdlib
pico_cxx_options
pico_printf
pico_standard_link
)

pico_add_extra_outputs(MT)

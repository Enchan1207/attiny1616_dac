cmake_minimum_required(VERSION 3.19)

project(attiny1616_dac
    VERSION 0.1.0
    DESCRIPTION "DAC function test for ATtiny1616"
    LANGUAGES C
)

add_compile_options(
    -Wall
)

add_subdirectory(src)

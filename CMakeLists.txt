cmake_minimum_required(VERSION 3.19)

project(tinysynth
    VERSION 0.1.0
    DESCRIPTION "Tiny synthesizer on ATtiny1616"
    LANGUAGES C
)

add_compile_options(
    -Wall
)

include_directories(BEFORE
    ${PROJECT_SOURCE_DIR}/include
)

add_subdirectory(src)

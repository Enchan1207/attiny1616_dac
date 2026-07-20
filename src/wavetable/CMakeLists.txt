cmake_minimum_required(VERSION 3.19)

add_library(tinysynth_wavetables)

target_sources(tinysynth_wavetables PRIVATE
    pulse.c
    sine.c
)

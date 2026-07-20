cmake_minimum_required(VERSION 3.19)

add_library(tinysynth_modules)

target_sources(tinysynth_modules PRIVATE
    audio.c
    oscillator.c
    voice.c
    envelope.c
    filter.c
    lfo.c
)

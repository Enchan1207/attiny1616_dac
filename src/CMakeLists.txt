cmake_minimum_required(VERSION 3.19)

add_executable(attiny1616_dac)

target_sources(attiny1616_dac PRIVATE
    main.c
    hardware/dac.c
    hardware/oscillator.c
)

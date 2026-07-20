cmake_minimum_required(VERSION 3.19)

add_executable(attiny1616_dac)

target_sources(attiny1616_dac PRIVATE
    main.c
    hardware/tca0.c
    hardware/dac0.c
    hardware/rotenc.c
    hardware/usart.c
    wavetable/pulse.c
    wavetable/sine.c
    module/oscillator.c
    module/voice.c
)

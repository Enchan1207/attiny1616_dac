cmake_minimum_required(VERSION 3.19)

add_executable(tinysynth)

target_sources(tinysynth PRIVATE
    main.c
    hardware/tca0.c
    hardware/dac0.c
    hardware/rotenc.c
    hardware/usart.c
    wavetable/pulse.c
    wavetable/sine.c
    module/oscillator.c
    module/voice.c
    app/synth.c
    module/envelope.c
)

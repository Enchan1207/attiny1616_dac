cmake_minimum_required(VERSION 3.19)

add_library(tinysynth_hardware)

target_sources(tinysynth_hardware PRIVATE
    tca0.c
    dac0.c
    rotenc.c
    usart.c
)

target_compile_definitions(tinysynth_hardware PRIVATE
    $<$<BOOL:${TINYSYNTH_VISUALIZE_ISR_RATE}>:VISUALIZE_ISR_RATE>
)

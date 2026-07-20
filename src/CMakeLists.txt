cmake_minimum_required(VERSION 3.19)

add_subdirectory(hardware)
add_subdirectory(module)
add_subdirectory(wavetable)

add_executable(tinysynth)

target_sources(tinysynth PRIVATE
    main.c
    app/synth.c
)

target_link_libraries(tinysynth PRIVATE
    tinysynth_hardware
    tinysynth_modules
    tinysynth_wavetables
)

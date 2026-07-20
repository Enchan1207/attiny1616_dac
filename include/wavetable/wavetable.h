#ifndef MODULE_WAVETABLE_H
#define MODULE_WAVETABLE_H

#include <stdint.h>

#include "module/audio.h"

/// @brief 波形テーブル
typedef struct {
    const audio_sample_t data[256];
} wavetable_t;

#endif /* MODULE_WAVETABLE_H */

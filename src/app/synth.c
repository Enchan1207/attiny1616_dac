#include "app/synth.h"

#include <stddef.h>

#include "hardware/dac0.h"
#include "hardware/tca0.h"
#include "module/voice.h"

static synthesizer_ctx_t* active_synth;

synthesizer_ctx_t synth0_internal;

synthesizer_ctx_t* synth0 = &synth0_internal;

static void synthesizer_step() {
    if (active_synth == NULL) {
        return;
    }

    audio_sample_t voice = voice_step(&active_synth->voice);
    audio_sample_t filtered = filter_step(&active_synth->filter, voice);
    uint8_t dac_value = (uint8_t)((int16_t)filtered + 0x80);

    dac0_set(dac_value);
}

void synthesizer_begin() {
    dac0_init();
    tca0_init();
    tca0_set_overflow_handler(synthesizer_step);
}

void synthesizer_init(synthesizer_ctx_t* ctx) {
    voice_init(&ctx->voice);
    filter_init(&ctx->filter);
}

void synthesizer_set_active_synth(synthesizer_ctx_t* ctx) {
    active_synth = ctx;
}

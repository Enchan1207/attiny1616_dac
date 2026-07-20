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

    uint8_t value = voice_step(&active_synth->voice);
    dac0_set(value);
}

void synthesizer_begin() {
    dac0_init();
    tca0_init();
    tca0_set_overflow_handler(synthesizer_step);
}

void synthesizer_init(synthesizer_ctx_t* ctx) {
    voice_init(&ctx->voice);
}

void synthesizer_set_active_synth(synthesizer_ctx_t* ctx) {
    active_synth = ctx;
}

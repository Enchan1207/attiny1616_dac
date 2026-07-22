#include "app/synth.h"

#include <stdbool.h>
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

    lfo_result_t result = {
        .updated = false,
        .value = 0,
    };
    lfo_step(&active_synth->modulation_lfo, &result);

    if (result.updated) {
        int16_t pitch_modulation = ((int16_t)result.value * active_synth->lfo_depth) >> 7;
        voice_set_pitch_modulation(&active_synth->voice, pitch_modulation);
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
    lfo_init(&ctx->modulation_lfo);
    lfo_set_waveform(&ctx->modulation_lfo, LFO_WAVEFORM_TRIANGLE);

    ctx->lfo_depth = 0x00;
}

void synthesizer_set_active_synth(synthesizer_ctx_t* ctx) {
    active_synth = ctx;
}

void synthesizer_reset_lfo_phase(synthesizer_ctx_t* ctx) {
    lfo_reset_phase(&ctx->modulation_lfo);
}

void synthesizer_set_lfo_depth(synthesizer_ctx_t* ctx, uint8_t depth) {
    ctx->lfo_depth = depth;
}

void synthesizer_set_lfo_frequency(synthesizer_ctx_t* ctx, uint16_t frequency) {
    lfo_set_frequency(&ctx->modulation_lfo, frequency);
}

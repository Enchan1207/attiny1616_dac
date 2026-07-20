#include "module/oscillator.h"

#include <stdbool.h>
#include <stddef.h>

#include "module/audio.h"

void oscillator_init(oscillator_ctx_t* ctx) {
    ctx->phase = 0;
    ctx->step = 0;
    ctx->table = NULL;
}

void oscillator_set_wavetable(oscillator_ctx_t* ctx, const wavetable_t* table) {
    ctx->table = table;
}

void oscillator_set_step(oscillator_ctx_t* ctx, uint16_t step) {
    ctx->step = step;
}

audio_sample_t oscillator_step(oscillator_ctx_t* ctx) {
    if (ctx->table == NULL) {
        return 0;
    }

    // 値を取得してphaseを進める
    audio_sample_t value = ctx->table->data[ctx->phase >> 8];
    ctx->phase += ctx->step;
    return value;
}

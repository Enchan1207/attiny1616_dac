#include "module/lfo.h"

#include "module/audio.h"

const uint8_t LFO_DIVIDER = 32;

void lfo_init(lfo_ctx_t* ctx) {
    ctx->phase = 0;
    ctx->step = 0;
    ctx->divider_counter = 0;
    ctx->current_output = 0;
    ctx->waveform = LFO_WAVEFORM_SQUARE;
}

void lfo_set_waveform(lfo_ctx_t* ctx, lfo_waveform_t wfm) {
    ctx->waveform = wfm;
}

void lfo_set_frequency(lfo_ctx_t* ctx, uint16_t freq) {
    uint16_t step = (uint32_t)freq * 65536UL / AUDIO_SAMPLE_RATE;
    ctx->step = step;
}

static inline int8_t calculate_lfo_output(lfo_ctx_t* ctx) {
    switch (ctx->waveform) {
        case LFO_WAVEFORM_TRIANGLE:
            uint8_t phase = ctx->phase >> 8;

            if (phase < 128) {
                return (int8_t)((int16_t)phase * 2 - 128);
            }

            return (int8_t)(127 - ((int16_t)(phase - 128) * 2));

        case LFO_WAVEFORM_SQUARE:
            return (ctx->phase & 0x8000) ? -127 : 127;

        default:
            return 0;
    }
}

int8_t lfo_step(lfo_ctx_t* ctx) {
    // LFO_DIVIDER回呼ばれるまで前の値を返し続ける
    ctx->divider_counter++;
    if (ctx->divider_counter < LFO_DIVIDER) {
        return ctx->current_output;
    }
    ctx->divider_counter = 0;

    // 値を計算してphaseを進める
    int8_t value = calculate_lfo_output(ctx);
    ctx->phase += ctx->step;
    ctx->current_output = value;
    return value;
}

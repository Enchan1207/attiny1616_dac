#include "module/filter.h"

void filter_init(filter_ctx_t* ctx) {
    ctx->coefficient = 0xFF;
    ctx->state_q8 = 0;
}

audio_sample_t filter_step(filter_ctx_t* ctx, audio_sample_t input) {
    int32_t input_q8 = (int32_t)input * 256;
    int32_t diff = input_q8 - ctx->state_q8;

    ctx->state_q8 += (diff * ctx->coefficient) >> 8;

    return (audio_sample_t)(ctx->state_q8 >> 8);
}

void filter_set_coefficient(filter_ctx_t* ctx, uint8_t coefficient) {
    ctx->coefficient = coefficient;
}

uint8_t filter_get_coefficient(filter_ctx_t* ctx) {
    return ctx->coefficient;
}

void filter_reset(filter_ctx_t* ctx, audio_sample_t value) {
    ctx->state_q8 = (int16_t)value << 8;
}

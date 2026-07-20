#include "module/filter.h"

/// @brief カットオフ値テーブル
static const uint16_t cutoff_table[] = {
    128,
    157,
    191,
    234,
    285,
    348,
    424,
    517,
    631,
    769,
    936,
    1140,
    1387,
    1685,
    2047,
    2482,
    3005,
    3632,
    4380,
    5267,
    6313,
    7536,
    8954,
    10578,
    12412,
    14447,
    16659,
    19001,
    21405,
    23779,
    26017,
    28008,
    29662,
};

void filter_init(filter_ctx_t* ctx) {
    ctx->coefficient_q15 = 0xFFFF;
    ctx->state_q8 = 0;
}

audio_sample_t filter_step(filter_ctx_t* ctx, audio_sample_t input) {
    int32_t input_q8 = (int32_t)input * 256;
    int32_t diff = input_q8 - ctx->state_q8;

    ctx->state_q8 += (diff * ctx->coefficient_q15) >> 15;

    return (audio_sample_t)(ctx->state_q8 >> 8);
}

void filter_set_cutoff(filter_ctx_t* ctx, uint8_t cutoff) {
    uint8_t index = cutoff >> 3;
    uint8_t fraction = cutoff & 0x07;

    uint16_t lower = cutoff_table[index];
    uint16_t upper = cutoff_table[index + 1];
    int32_t diff = (int32_t)upper - lower;

    ctx->cutoff = cutoff;
    ctx->coefficient_q15 = (uint16_t)(lower + ((diff * fraction) >> 3));
}

uint8_t filter_get_cutoff(filter_ctx_t* ctx) {
    return ctx->cutoff;
}

void filter_reset(filter_ctx_t* ctx, audio_sample_t value) {
    ctx->state_q8 = (int16_t)value << 8;
}

#include "module/oscillator.h"

#include <stdbool.h>
#include <stddef.h>

void oscillator_init(oscillator_ctx_t* ctx) {
    ctx->phase = 0;
    ctx->step = 0;
    ctx->table = NULL;
}

void oscillator_set_wavetable(oscillator_ctx_t* ctx, const wavetable_t* table) {
    ctx->table = table;
}

void oscillator_set_frequency(oscillator_ctx_t* ctx, uint16_t frequency) {
    // 周波数を65536分率に変換してステップとする
    uint16_t step = (uint32_t)frequency * 65536UL / 32000;
    ctx->step = step;
}

uint8_t oscillator_step(oscillator_ctx_t* ctx) {
    if (ctx->table == NULL) {
        return 0x80;
    }

    // 値を取得してphaseを進める
    uint8_t value = ctx->table->data[ctx->phase >> 8];
    ctx->phase += ctx->step;
    return value;
}

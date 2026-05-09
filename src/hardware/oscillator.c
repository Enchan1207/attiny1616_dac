#include "hardware/oscillator.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <stddef.h>

struct oscillator_ctx_t {
    volatile bool enabled;
    volatile uint16_t phase;
    volatile uint16_t step;
    volatile const wavetable_t* table;
};

struct oscillator_ctx_t osc0_internal = {
    .enabled = false,
    .phase = 0,
    .step = 0,
    .table = NULL,
};

oscillator_ctx_t* osc0 = &osc0_internal;

// TCA0オーバーフロー割り込み
ISR(TCA0_OVF_vect) {
    // 割込みフラグをクリア
    TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;

    if (!(osc0->enabled)) {
        DAC0.DATA = 0x80;
        return;
    }

    if (osc0->table == NULL) {
        return;
    }

    DAC0.DATA = osc0->table->data[osc0->phase >> 8];
    osc0->phase += osc0->step;
}

void oscillator_init() {
    cli();

    // 参照電圧を4.3Vに設定し、DACを有効化
    VREF.CTRLA = (VREF.CTRLA & ~VREF_DAC0REFSEL_gm) | (VREF_DAC0REFSEL_4V34_gc << VREF_DAC0REFSEL_gp);
    DAC0.DATA = 0x80;
    DAC0.CTRLA |= DAC_ENABLE_bm | DAC_OUTEN_bm;

    // TCA0を初期化

    // 20MHz, x1 = 50ns/clk
    // 20MHz, x2 = 25ns/clk

    // 8kHz = 125us = 125000ns
    // 32kHz = 31.25us = 31250ns
    // 48kHz = 20.83us = 2083ns

    // 125000ns / 50ns = 2500, 31250ns / 50ns = 625, 2083ns/50ns = 416, ...

    TCA0.SINGLE.PER = (625 - 1);
    TCA0.SINGLE.CTRLA |= TCA_SINGLE_CLKSEL_DIV1_gc | TCA_SINGLE_ENABLE_bm;
    TCA0.SINGLE.INTCTRL |= TCA_SINGLE_OVF_bm << TCA_SINGLE_OVF_bp;
}

void oscillator_set_wavetable(oscillator_ctx_t* ctx, const wavetable_t* table) {
    ctx->table = table;
}

void oscillator_set_frequency(oscillator_ctx_t* ctx, uint16_t frequency) {
    // 周波数を65536分率に変換してステップとする
    uint16_t step = (uint32_t)frequency * 65536UL / 32000;
    ctx->step = step;
}

void oscillator_enable(oscillator_ctx_t* ctx) {
    ctx->enabled = true;
}

void oscillator_disable(oscillator_ctx_t* ctx) {
    ctx->enabled = false;
}

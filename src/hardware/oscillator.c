#include "hardware/oscillator.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stddef.h>

struct oscillator_ctx_t {
    uint16_t frequency;
    wavetable_t* waveTable;
};

struct oscillator_ctx_t oscillator_internal = {
    .frequency = 0,
    .waveTable = NULL,
};

oscillator_ctx_t* oscillator = &oscillator_internal;

// TCA0オーバーフロー割り込み
ISR(TCA0_OVF_vect) {
    // 割込みフラグをクリア
    TCA0.SINGLE.INTFLAGS |= TCA_SINGLE_OVF_bm << TCA_SINGLE_OVF_bp;

    // TODO: 適切にテーブルを呼んで反映させる
    DAC0.DATA = 0x00;
}

void oscillator_init(oscillator_ctx_t* ctx) {
    cli();

    // 参照電圧を4.3Vに設定し、DACを有効化
    VREF.CTRLA = (VREF.CTRLA & ~VREF_DAC0REFSEL_gm) | (VREF_DAC0REFSEL_4V34_gc << VREF_DAC0REFSEL_gp);
    DAC0.DATA = 0x80;
    DAC0.CTRLA |= DAC_ENABLE_bm | DAC_OUTEN_bm;

    // 8kHzで割込むようTCA0を初期化 20MHz, 1倍 -> 50ns/clk * 2500 = 125us/clk = 8kHz
    TCA0.SINGLE.PER = (2500 - 1);
    TCA0.SINGLE.CTRLA |= (TCA_SINGLE_CLKSEL_DIV1_gc << TCA_SINGLE_CLKSEL_gp) | (TCA_SINGLE_ENABLE_bm | TCA_SINGLE_ENABLE_bp);
    TCA0.SINGLE.INTCTRL |= TCA_SINGLE_OVF_bm << TCA_SINGLE_OVF_bp;

    sei();
}

void oscillator_set_wavetable(oscillator_ctx_t* ctx, const wavetable_t* table) {
}

void oscillator_set_frequency(oscillator_ctx_t* ctx, uint16_t frequency) {
}

void oscillator_enable(oscillator_ctx_t* ctx) {
}

void oscillator_disable(oscillator_ctx_t* ctx) {
}

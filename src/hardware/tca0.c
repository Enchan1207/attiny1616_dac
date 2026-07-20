#include "hardware/tca0.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stddef.h>

static volatile tca0_overflow_handler_t overflow_handler = NULL;

// TCA0オーバーフロー割り込み
ISR(TCA0_OVF_vect) {
    // 割込みフラグをクリア
    TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;

    if (overflow_handler == NULL) {
        return;
    }

    overflow_handler();
}

void tca0_init() {
    cli();

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

void tca0_set_overflow_handler(tca0_overflow_handler_t handler) {
    uint8_t sreg = SREG;
    cli();
    overflow_handler = handler;
    SREG = sreg;
}

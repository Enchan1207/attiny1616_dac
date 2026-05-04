#include "hardware/dac.h"

#include <avr/io.h>

struct dac_ctx_t {
    DAC_t* const dac;

    volatile uint8_t* vrefRegister;
    uint8_t vrefGroupMask;
    uint8_t vrefGroupShift;
};

static dac_ctx_t dac0_internal = {
    .dac = &DAC0,

    .vrefRegister = &VREF.CTRLA,
    .vrefGroupMask = VREF_DAC0REFSEL_gm,
    .vrefGroupShift = VREF_DAC0REFSEL_gp,
};

dac_ctx_t* dac0 = &dac0_internal;

void dac_init(dac_ctx_t* ctx, VREF_VOLTAGE refVoltage) {
    uint8_t current = *ctx->vrefRegister;
    *ctx->vrefRegister = (current & ~ctx->vrefGroupMask) | (refVoltage << ctx->vrefGroupShift);

    ctx->dac->CTRLA |= DAC_ENABLE_bm;
}

void dac_output_enable(dac_ctx_t* ctx) {
    ctx->dac->CTRLA |= DAC_OUTEN_bm;
}

void dac_output_disable(dac_ctx_t* ctx) {
    ctx->dac->CTRLA &= ~DAC_OUTEN_bm;
}

void dac_set_value(dac_ctx_t* ctx, uint8_t value) {
    ctx->dac->DATA = value;
}

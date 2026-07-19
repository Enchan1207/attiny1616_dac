#include "hardware/dac0.h"

#include <avr/io.h>

void dac0_init() {
    // 参照電圧を4.3Vに設定し、DACを有効化
    VREF.CTRLA = (VREF.CTRLA & ~VREF_DAC0REFSEL_gm) | (VREF_DAC0REFSEL_4V34_gc << VREF_DAC0REFSEL_gp);
    DAC0.DATA = 0x80;
    DAC0.CTRLA |= DAC_ENABLE_bm | DAC_OUTEN_bm;
}

inline void dac0_set(uint8_t value) {
    DAC0.DATA = value;
}

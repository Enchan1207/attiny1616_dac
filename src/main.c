#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

#include "hardware/dac.h"

int main() {
    CCP = CCP_IOREG_gc;
    CLKCTRL_MCLKCTRLB &= ~CLKCTRL_PEN_bm;

    PORTA.DIRSET = 1 << 6;

    dac_init(dac0, VREF_4V3);

    dac_output_enable(dac0);

    uint8_t value = 0;

    while (1) {
        dac_set_value(dac0, value);
        _delay_us(10);
        value++;
    }
}

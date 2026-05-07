#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

#include "hardware/oscillator.h"

int main() {
    CCP = CCP_IOREG_gc;
    CLKCTRL_MCLKCTRLB &= ~CLKCTRL_PEN_bm;

    PORTA.DIRSET = 1 << 6;

    oscillator_init(oscillator);

    sei();

    while (1) {
        //
    }
}

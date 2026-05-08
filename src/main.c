#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

#include "hardware/oscillator.h"
#include "wavetable/pulse.h"

int main() {
    CCP = CCP_IOREG_gc;
    CLKCTRL_MCLKCTRLB &= ~CLKCTRL_PEN_bm;

    PORTA.DIRSET = 1 << 6;

    oscillator_init(osc0);

    oscillator_set_wavetable(osc0, &pulseTable);
    oscillator_set_frequency(osc0, 440);

    oscillator_enable(osc0);

    sei();

    while (1) {
        //
    }
}

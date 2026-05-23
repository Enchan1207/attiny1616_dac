#include <avr/interrupt.h>
#include <avr/io.h>
#include <stddef.h>
#include <stdint.h>
#include <util/delay.h>

#include "hardware/oscillator.h"
#include "wavetable/pulse.h"
#include "wavetable/sine.h"

int main() {
    // クロック設定 (プリスケーラ1倍, 無効)
    _PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, 0x00);

    oscillator_init();

    oscillator_set_frequency(osc0, 440);
    oscillator_set_wavetable(osc0, &sineTable);

    oscillator_enable(osc0);

    sei();

    while (1) {
        //
    }
}

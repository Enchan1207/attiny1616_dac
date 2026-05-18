#include <avr/interrupt.h>
#include <avr/io.h>
#include <stddef.h>
#include <stdint.h>
#include <util/delay.h>

#include "hardware/oscillator.h"
#include "wavetable/pulse.h"
#include "wavetable/sine.h"

volatile int tableIndex = 0;

ISR(PORTB_PORT_vect) {
    PORTB.INTFLAGS = PIN0_bm;

    // PC0を読んでCW/CCWを判断
    if (PORTC.IN & 0b00000001) {
        tableIndex = 1;
    } else {
        tableIndex = 2;
    }
}

int main() {
    // クロック設定 (プリスケーラ1倍, 無効)
    _PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, 0x00);

    PORTA.DIRSET = 1 << 6;

    // PB0, PC0を入力に設定し、PB0の立下りエッジ割り込みを有効化
    PORTB.DIRCLR = 1 << 0;
    PORTC.DIRCLR = 1 << 0;
    PORTB.PIN0CTRL = (PORTB.PIN0CTRL & ~PORT_ISC_gm) | (PORT_ISC_FALLING_gc << PORT_ISC_gp);

    oscillator_init();

    oscillator_set_frequency(osc0, 440);

    oscillator_enable(osc0);

    sei();

    while (1) {
        if (tableIndex > 0) {
            const wavetable_t* ptr = tableIndex == 1 ? &sineTable : &pulseTable;
            oscillator_set_wavetable(osc0, ptr);
            tableIndex = 0;
        }
    }
}

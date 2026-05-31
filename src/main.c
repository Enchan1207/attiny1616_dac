#include <avr/interrupt.h>
#include <avr/io.h>
#include <stddef.h>
#include <stdint.h>
#include <util/delay.h>

#include "hardware/oscillator.h"
#include "hardware/rotenc.h"
#include "hardware/usart.h"
#include "wavetable/pulse.h"
#include "wavetable/sine.h"

int main() {
    // クロック設定 (プリスケーラ1倍, 無効)
    _PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, 0x00);

    // オシレータ
    oscillator_init();
    oscillator_set_frequency(osc0, 440);
    oscillator_set_wavetable(osc0, &sineTable);
    oscillator_enable(osc0);

    // ロータリエンコーダ
    rotenc_init();

    // USART (Tx: PA1 Rx: PA2)
    PORTMUX_CTRLB |= PORTMUX_USART0_ALTERNATE_gc;
    PORTA.DIRSET = PIN1_bm;
    PORTA.DIRCLR = PIN2_bm;
    usart_begin(usart0);

    sei();

    usart_print(usart0, "Hello from ATtiny1616\n");

    rotenc_ctx_t* const encoders[] = {rotenc0, rotenc1, rotenc2};
    uint8_t scores[] = {0x00, 0x00, 0x00};
    bool isRotated = false;

    while (1) {
        for (size_t i = 0; i < 3; i++) {
            rotenc_ctx_t* enc = encoders[i];
            rotenc_dir_t dir = rotenc_get_direction(enc);
            if (dir == ROTATION_NONE) {
                continue;
            }

            scores[i] += dir == ROTATION_CW ? 1 : -1;
            isRotated = true;
        }

        if (isRotated) {
            for (size_t i = 0; i < 3; i++) {
                uint8_t score_str[2] = {0};
                itoa(scores[i], (char*)score_str, 16);

                usart_write(usart0, score_str, 2);
                usart_print(usart0, ", ");
            }
            usart_print(usart0, "\r\n");

            isRotated = false;
        }
    }
}

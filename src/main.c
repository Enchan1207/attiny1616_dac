#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>

#include "app/synth.h"
#include "hardware/rotenc.h"
#include "hardware/usart.h"

static uint8_t clamp_filter_coefficient(uint8_t coefficient, rotenc_dir_t direction) {
    if (direction == ROTATION_CW && coefficient < UINT8_MAX) {
        return coefficient + 1;
    }

    if (direction == ROTATION_CCW && coefficient > 0) {
        return coefficient - 1;
    }

    return coefficient;
}

static void print_filter_coefficient(uint8_t coefficient) {
    char message[] = "filter coefficient: 255\r\n";

    message[20] = (char)('0' + (coefficient / 100));
    message[21] = (char)('0' + ((coefficient / 10) % 10));
    message[22] = (char)('0' + (coefficient % 10));

    usart_print(usart0, message);
}

// ポートC割込み
ISR(PORTC_PORT_vect) {
    uint8_t input = PORTC.IN;
    PORTC.INTFLAGS = 0b00000010;

    if (!(input & PIN1_bm)) {
        voice_note_on(&synth0->voice, VOICE_NOTE_C);
    }

    if (input & PIN1_bm) {
        voice_note_off(&synth0->voice);
    }
}

int main() {
    uint8_t filter_coefficient = 0xFF;

    // クロック設定 (プリスケーラ1倍, 無効)
    _PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, 0x00);

    // シンセサイザ
    synthesizer_begin();
    synthesizer_init(synth0);
    synthesizer_set_active_synth(synth0);

    voice_set_waveform(&synth0->voice, VOICE_WAVEFORM_PULSE);
    envelope_set_attack_ms(&synth0->voice.evp_ctx, 20);
    envelope_set_decay_ms(&synth0->voice.evp_ctx, 80);
    envelope_set_sustain(&synth0->voice.evp_ctx, 0xA0A0);
    envelope_set_release_ms(&synth0->voice.evp_ctx, 120);
    filter_set_coefficient(&synth0->filter, filter_coefficient);

    // ロータリエンコーダ
    rotenc_init();

    // USART (Tx: PA1 Rx: PA2)
    PORTMUX_CTRLB |= PORTMUX_USART0_ALTERNATE_gc;
    PORTA.DIRSET = PIN1_bm;
    PORTA.DIRCLR = PIN2_bm;
    usart_begin(usart0);

    // ノートボタン (PC1に仮設)
    PORTC.DIRCLR = 0b00000010;
    PORTC.PIN1CTRL = (PORTC.PIN1CTRL & ~PORT_ISC_gm) | PORT_ISC_BOTHEDGES_gc;

    sei();

    while (1) {
        rotenc_dir_t direction = rotenc_get_direction(rotenc0);

        if (direction == ROTATION_NONE) {
            continue;
        }

        uint8_t next_coefficient = clamp_filter_coefficient(filter_coefficient, direction);
        if (next_coefficient == filter_coefficient) {
            continue;
        }

        filter_coefficient = next_coefficient;
        filter_set_coefficient(&synth0->filter, filter_coefficient);
        print_filter_coefficient(filter_coefficient);
    }
}

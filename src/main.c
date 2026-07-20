#include <avr/interrupt.h>
#include <avr/io.h>
#include <stddef.h>
#include <stdint.h>
#include <util/delay.h>

#include "app/synth.h"

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

    // ノートボタン (PC1に仮設)
    PORTC.DIRCLR = 0b00000010;
    PORTC.PIN1CTRL = (PORTC.PIN1CTRL & ~PORT_ISC_gm) | PORT_ISC_BOTHEDGES_gc;

    sei();

    while (1) {
    }
}

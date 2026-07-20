#include "module/voice.h"

#include <stddef.h>

#include "hardware/dac0.h"
#include "hardware/tca0.h"
#include "module/oscillator.h"
#include "wavetable/pulse.h"
#include "wavetable/sine.h"

static uint16_t calculate_frequency(voice_note_t note) {
    switch (note) {
        case VOICE_NOTE_C:
            return 262;
        case VOICE_NOTE_CS:
            return 277;
        case VOICE_NOTE_D:
            return 294;
        case VOICE_NOTE_DS:
            return 311;
        case VOICE_NOTE_E:
            return 330;
        case VOICE_NOTE_F:
            return 349;
        case VOICE_NOTE_FS:
            return 370;
        case VOICE_NOTE_G:
            return 392;
        case VOICE_NOTE_GS:
            return 415;
        case VOICE_NOTE_A:
            return 440;
        case VOICE_NOTE_AS:
            return 466;
        case VOICE_NOTE_B:
            return 494;

        default:
            return 0;
    }
}

void voice_init(voice_ctx_t* ctx) {
    oscillator_init(&ctx->osc_ctx);
}

uint8_t voice_sample_next(voice_ctx_t* ctx) {
    uint8_t osc_value = oscillator_step(&ctx->osc_ctx);
    // TODO: envelope?
    return osc_value;
}

void voice_note_on(voice_ctx_t* ctx, voice_note_t note) {
    oscillator_set_frequency(&ctx->osc_ctx, calculate_frequency(note));
    // TODO: envelope
    oscillator_enable(&ctx->osc_ctx);
}

void voice_note_off(voice_ctx_t* ctx) {
    // TODO: envelope
    oscillator_disable(&ctx->osc_ctx);
}

void voice_set_waveform(voice_ctx_t* ctx, voice_waveform_t wfm) {
    switch (wfm) {
        case VOICE_WAVEFORM_PULSE:
            oscillator_set_wavetable(&ctx->osc_ctx, &pulseTable);
            break;

        case VOICE_WAVEFORM_SINE:
            oscillator_set_wavetable(&ctx->osc_ctx, &sineTable);
            break;

        default:
            break;
    }
}

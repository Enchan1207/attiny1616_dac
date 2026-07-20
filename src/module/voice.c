#include "module/voice.h"

#include <stddef.h>

#include "hardware/dac0.h"
#include "hardware/tca0.h"
#include "module/envelope.h"
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
    envelope_init(&ctx->evp_ctx);

    ctx->base_step = 0;
}

audio_sample_t voice_step(voice_ctx_t* ctx) {
    audio_sample_t osc_value = oscillator_step(&ctx->osc_ctx);
    uint8_t evp_value = envelope_step(&ctx->evp_ctx);
    int16_t scaled = ((int16_t)osc_value * evp_value) >> 8;

    return (audio_sample_t)scaled;
}

void voice_note_on(voice_ctx_t* ctx, voice_note_t note) {
    uint16_t frequency = calculate_frequency(note);

    uint16_t step = (uint32_t)frequency * 65536UL / AUDIO_SAMPLE_RATE;
    oscillator_set_step(&ctx->osc_ctx, step);

    ctx->base_step = step;

    envelope_note_on(&ctx->evp_ctx);
}

void voice_note_off(voice_ctx_t* ctx) {
    envelope_note_off(&ctx->evp_ctx);
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

void voice_set_pitch_modulation(voice_ctx_t* ctx, int16_t modulation) {
    int32_t increment = ctx->base_step + modulation;

    if (increment < 1) {
        increment = 1;
    } else if (increment > UINT16_MAX) {
        increment = UINT16_MAX;
    }

    oscillator_set_step(&ctx->osc_ctx, (uint16_t)increment);
}

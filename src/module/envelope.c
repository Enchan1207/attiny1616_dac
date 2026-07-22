#include "module/envelope.h"

#include <util/atomic.h>

#include "module/audio.h"

const uint16_t ENVELOPE_MAX_LEVEL = 65535;

/// @brief 時間をステップ数に換算する
/// @param range
/// @param ms
/// @return
static uint16_t envelope_calc_step(uint32_t range, uint16_t ms) {
    uint32_t samples;
    uint32_t step;

    if (range == 0) {
        return 0;
    }

    if (ms == 0) {
        return (uint16_t)range;
    }

    samples = ((uint32_t)AUDIO_SAMPLE_RATE * ms) / 1000UL;
    if (samples == 0) {
        samples = 1;
    }

    step = (range + samples - 1) / samples;
    if (step == 0) {
        step = 1;
    }

    if (step > 0xFFFFUL) {
        step = 0xFFFFUL;
    }

    return (uint16_t)step;
}

void envelope_init(envelope_ctx_t* ctx) {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        ctx->params.attack_step = 0;
        ctx->params.decay_ms = 0;
        ctx->params.decay_step = 0;
        ctx->params.sustain = 0;
        ctx->params.release_ms = 0;
        ctx->runtime.release_step = 0;
        ctx->runtime.state = ENVELOPE_STATE_STOP;
        ctx->runtime.current_level = 0;
    }
}

void envelope_set_attack_ms(envelope_ctx_t* ctx, uint16_t ms) {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        ctx->params.attack_step = envelope_calc_step(ENVELOPE_MAX_LEVEL, ms);
    }
}

void envelope_set_decay_ms(envelope_ctx_t* ctx, uint16_t ms) {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        ctx->params.decay_ms = ms;
        ctx->params.decay_step = envelope_calc_step(ENVELOPE_MAX_LEVEL - ctx->params.sustain, ms);
    }
}

void envelope_set_sustain(envelope_ctx_t* ctx, uint16_t value) {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        ctx->params.sustain = value;

        if (ctx->runtime.state != ENVELOPE_STATE_DECAY) {
            return;
        }

        uint16_t sustain_level = ctx->params.sustain;
        uint32_t range = 0;

        if (ctx->runtime.current_level > sustain_level) {
            range = ctx->runtime.current_level - sustain_level;
        }

        ctx->params.decay_step = envelope_calc_step(range, ctx->params.decay_ms);
    }
}

void envelope_set_release_ms(envelope_ctx_t* ctx, uint16_t ms) {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        ctx->params.release_ms = ms;
    }
}

void envelope_note_on(envelope_ctx_t* ctx) {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        ctx->runtime.state = ENVELOPE_STATE_ATTACK;
    }
}

void envelope_note_off(envelope_ctx_t* ctx) {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        ctx->runtime.release_step = envelope_calc_step(ctx->runtime.current_level, ctx->params.release_ms);
        ctx->runtime.state = ENVELOPE_STATE_RELEASE;
    }
}

uint8_t envelope_step(envelope_ctx_t* ctx) {
    switch (ctx->runtime.state) {
        case ENVELOPE_STATE_ATTACK: {
            uint32_t new_level = (uint32_t)ctx->runtime.current_level + ctx->params.attack_step;

            if (new_level >= ENVELOPE_MAX_LEVEL) {
                ctx->runtime.state = ENVELOPE_STATE_DECAY;
                ctx->runtime.current_level = (uint16_t)ENVELOPE_MAX_LEVEL;
                ctx->params.decay_step =
                    envelope_calc_step(ENVELOPE_MAX_LEVEL - ctx->params.sustain, ctx->params.decay_ms);
                break;
            }

            ctx->runtime.current_level = (uint16_t)new_level;
            break;
        }

        case ENVELOPE_STATE_DECAY:
            if (ctx->runtime.current_level <= ctx->params.sustain + ctx->params.decay_step) {
                ctx->runtime.state = ENVELOPE_STATE_SUSTAIN;
                ctx->runtime.current_level = ctx->params.sustain;
                break;
            }

            ctx->runtime.current_level -= ctx->params.decay_step;
            break;

        case ENVELOPE_STATE_SUSTAIN:
            // do nothing
            break;

        case ENVELOPE_STATE_RELEASE:
            if (ctx->runtime.current_level <= ctx->runtime.release_step) {
                ctx->runtime.state = ENVELOPE_STATE_STOP;
                ctx->runtime.current_level = 0;
                break;
            }

            ctx->runtime.current_level -= ctx->runtime.release_step;
            break;

        default:
            break;
    }

    return (uint8_t)(ctx->runtime.current_level >> 8);
}

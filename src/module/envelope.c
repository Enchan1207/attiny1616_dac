#include "module/envelope.h"

const uint16_t ENVELOPE_SAMPLE_RATE = 32000;
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

    samples = ((uint32_t)ENVELOPE_SAMPLE_RATE * ms) / 1000UL;
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
    ctx->attack_step = 0;
    ctx->decay_ms = 0;
    ctx->decay_step = 0;
    ctx->sustain = 0;
    ctx->release_ms = 0;
    ctx->release_step = 0;
    ctx->state = ENVELOPE_STATE_STOP;
    ctx->current_level = 0;
}

void envelope_set_attack_ms(envelope_ctx_t* ctx, uint16_t ms) {
    ctx->attack_step = envelope_calc_step(ENVELOPE_MAX_LEVEL, ms);
}

void envelope_set_decay_ms(envelope_ctx_t* ctx, uint16_t ms) {
    ctx->decay_ms = ms;
    ctx->decay_step = envelope_calc_step(ENVELOPE_MAX_LEVEL - ctx->sustain, ms);
}

void envelope_set_sustain(envelope_ctx_t* ctx, uint16_t value) {
    ctx->sustain = value;

    if (ctx->state != ENVELOPE_STATE_DECAY) {
        return;
    }

    uint16_t sustain_level = ctx->sustain;
    uint32_t range = 0;

    if (ctx->current_level > sustain_level) {
        range = ctx->current_level - sustain_level;
    }

    ctx->decay_step = envelope_calc_step(range, ctx->decay_ms);
}

void envelope_set_release_ms(envelope_ctx_t* ctx, uint16_t ms) {
    ctx->release_ms = ms;
}

void envelope_note_on(envelope_ctx_t* ctx) {
    ctx->state = ENVELOPE_STATE_ATTACK;
}

void envelope_note_off(envelope_ctx_t* ctx) {
    ctx->release_step = envelope_calc_step(ctx->current_level, ctx->release_ms);
    ctx->state = ENVELOPE_STATE_RELEASE;
}

uint8_t envelope_step(envelope_ctx_t* ctx) {
    switch (ctx->state) {
        case ENVELOPE_STATE_ATTACK: {
            uint32_t new_level = (uint32_t)ctx->current_level + ctx->attack_step;

            if (new_level >= ENVELOPE_MAX_LEVEL) {
                ctx->state = ENVELOPE_STATE_DECAY;
                ctx->current_level = (uint16_t)ENVELOPE_MAX_LEVEL;
                ctx->decay_step = envelope_calc_step(ENVELOPE_MAX_LEVEL - ctx->sustain, ctx->decay_ms);
                break;
            }

            ctx->current_level = (uint16_t)new_level;
            break;
        }

        case ENVELOPE_STATE_DECAY:
            if (ctx->current_level <= ctx->sustain + ctx->decay_step) {
                ctx->state = ENVELOPE_STATE_SUSTAIN;
                ctx->current_level = ctx->sustain;
                break;
            }

            ctx->current_level -= ctx->decay_step;
            break;

        case ENVELOPE_STATE_SUSTAIN:
            // do nothing
            break;

        case ENVELOPE_STATE_RELEASE:
            if (ctx->current_level <= ctx->release_step) {
                ctx->state = ENVELOPE_STATE_STOP;
                ctx->current_level = 0;
                break;
            }

            ctx->current_level -= ctx->release_step;
            break;

        default:
            break;
    }

    return (uint8_t)(ctx->current_level >> 8);
}

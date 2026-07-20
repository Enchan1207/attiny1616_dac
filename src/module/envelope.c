#include "module/envelope.h"

void envelope_init(envelope_ctx_t* ctx) {
    ctx->attack = 0;
    ctx->decay = 0;
    ctx->sustain = 0;
    ctx->release = 0;
    ctx->state = ENVELOPE_STATE_STOP;
    ctx->current_volume = 0;
}

uint8_t envelope_get(envelope_ctx_t* ctx, envelope_param_t param) {
    switch (param) {
        case ENVELOPE_PARAM_ATTACK:
            return ctx->attack;

        case ENVELOPE_PARAM_DECAY:
            return ctx->decay;

        case ENVELOPE_PARAM_SUSTAIN:
            return ctx->sustain;

        case ENVELOPE_PARAM_RELEASE:
            return ctx->release;

        default:
            return 0;
    }
}

void envelope_set(envelope_ctx_t* ctx, envelope_param_t param, uint8_t value) {
    switch (param) {
        case ENVELOPE_PARAM_ATTACK:
            ctx->attack = value;
            break;

        case ENVELOPE_PARAM_DECAY:
            ctx->decay = value;
            break;

        case ENVELOPE_PARAM_SUSTAIN:
            ctx->sustain = value;
            break;

        case ENVELOPE_PARAM_RELEASE:
            ctx->release = value;
            break;

        default:
            return;
    }
}

void envelope_note_on(envelope_ctx_t* ctx) {
    ctx->state = ENVELOPE_STATE_ATTACK;
}

void envelope_note_off(envelope_ctx_t* ctx) {
    ctx->state = ENVELOPE_STATE_RELEASE;
}

uint8_t envelope_step(envelope_ctx_t* ctx) {
    switch (ctx->state) {
        case ENVELOPE_STATE_ATTACK: {
            uint16_t new_volume = ctx->current_volume + ctx->attack;

            if (new_volume > 0xFF) {
                ctx->state = ENVELOPE_STATE_DECAY;
                ctx->current_volume = 0xFF;
                break;
            }

            ctx->current_volume = (uint8_t)new_volume;
            break;
        }

        case ENVELOPE_STATE_DECAY: {
            int16_t new_volume = (int16_t)ctx->current_volume - ctx->decay;

            if (new_volume <= ctx->sustain) {
                ctx->state = ENVELOPE_STATE_SUSTAIN;
                ctx->current_volume = ctx->sustain;
                break;
            }

            ctx->current_volume = (uint8_t)new_volume;
            break;
        }

        case ENVELOPE_STATE_SUSTAIN:
            // do nothing
            break;

        case ENVELOPE_STATE_RELEASE: {
            int16_t new_volume = (int16_t)ctx->current_volume - ctx->release;

            if (new_volume <= 0) {
                ctx->state = ENVELOPE_STATE_STOP;
                ctx->current_volume = 0;
                break;
            }

            ctx->current_volume = (uint8_t)new_volume;
            break;
        }

        default:
            break;
    }

    return ctx->current_volume;
}

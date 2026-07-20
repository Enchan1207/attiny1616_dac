#include "module/envelope.h"

void envelope_init(envelope_ctx_t* ctx) {
    ctx->attack = 0;
    ctx->decay = 0;
    ctx->sustain = 0;
    ctx->release = 0;
    ctx->state = ENVELOPE_STATE_STOP;
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
    if (ctx->state != ENVELOPE_STATE_STOP) {
        return;
    }

    ctx->state = ENVELOPE_STATE_ATTACK;
}

void envelope_note_off(envelope_ctx_t* ctx) {
    if (ctx->state != ENVELOPE_STATE_SUSTAIN) {
        return;
    }

    ctx->state = ENVELOPE_STATE_RELEASE;
}

uint8_t envelope_step(envelope_ctx_t* ctx) {
    // TODO
    return 0;
}

#include "hardware/rotenc.h"

#include <avr/interrupt.h>
#include <stdint.h>

struct rotenc_ctx_t {
    volatile rotenc_dir_t direction;
};

struct rotenc_ctx_t rotenc0_internal = {
    .direction = ROTATION_NONE,
};
rotenc_ctx_t* rotenc0 = &rotenc0_internal;

struct rotenc_ctx_t rotenc1_internal = {
    .direction = ROTATION_NONE,
};
rotenc_ctx_t* rotenc1 = &rotenc1_internal;

struct rotenc_ctx_t rotenc2_internal = {
    .direction = ROTATION_NONE,
};
rotenc_ctx_t* rotenc2 = &rotenc2_internal;

// ポートB割込み
ISR(PORTB_PORT_vect) {
    uint8_t input = PORTB.IN;
    uint8_t flags = PORTB.INTFLAGS;
    PORTB.INTFLAGS = 0b00010101;

    if (flags & PIN0_bm) {
        rotenc0->direction = input & PIN1_bm ? ROTATION_CW : ROTATION_CCW;
    }

    if (flags & PIN2_bm) {
        rotenc1->direction = input & PIN3_bm ? ROTATION_CW : ROTATION_CCW;
    }

    if (flags & PIN4_bm) {
        rotenc2->direction = input & PIN5_bm ? ROTATION_CW : ROTATION_CCW;
    }
}

void rotenc_init() {
    cli();

    // PB0~5を入力に設定し、0,2,4の立下りエッジ割込みを有効化
    PORTB.DIRCLR = 0b00111111;
    PORTB.PIN0CTRL = (PORTB.PIN0CTRL & ~PORT_ISC_gm) | PORT_ISC_FALLING_gc;
    PORTB.PIN2CTRL = (PORTB.PIN2CTRL & ~PORT_ISC_gm) | PORT_ISC_FALLING_gc;
    PORTB.PIN4CTRL = (PORTB.PIN4CTRL & ~PORT_ISC_gm) | PORT_ISC_FALLING_gc;
}

rotenc_dir_t rotenc_get_direction(rotenc_ctx_t* ctx) {
    // 本来は読出し -> 書込みの間は割込みを止めるべき(cli-sei)だが、
    // AVRではこの命令にそこまで時間がかからないこと、オシレータの処理を止めたくないことから、
    // 許容する
    rotenc_dir_t dir = ctx->direction;
    ctx->direction = ROTATION_NONE;
    return dir;
}

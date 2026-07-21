#include <avr/interrupt.h>
#include <avr/io.h>
#include <hardware/usart.h>
#include <stddef.h>
#include <util/atomic.h>

struct usart_ctx_t {
    USART_t* const usart;

    const uint8_t* volatile txDataPtr;
    volatile size_t txRemaining;

    volatile uint8_t rxBuffer[USART_RX_BUFSIZE];
    volatile uint8_t rxHead;
    volatile uint8_t rxTail;
};

static usart_ctx_t usart0_internal = {
    .usart = &USART0,

    .txDataPtr = NULL,
    .txRemaining = 0,

    .rxBuffer = {0},
    .rxHead = 0,
    .rxTail = 0,
};

usart_ctx_t* usart0 = &usart0_internal;

// USART0データエンプティ割込み
ISR(USART0_DRE_vect) {
    if (usart0->txRemaining > 0) {
        usart0->usart->TXDATAL = *usart0->txDataPtr++;
        usart0->txRemaining--;
    } else {
        usart0->usart->CTRLA &= ~USART_DREIE_bm;
    }
}

// USART0受信完了割込み
ISR(USART0_RXC_vect) {
    uint8_t data = usart0->usart->RXDATAL;
    uint8_t nextTail = (usart0->rxTail + 1) & (USART_RX_BUFSIZE - 1);

    // バッファがいっぱいの場合は読み捨て
    if (nextTail == usart0->rxHead) {
        PORTA.OUTSET = 1 << 7;
        return;
    }

    usart0->rxBuffer[usart0->rxTail] = data;
    usart0->rxTail = nextTail;
}

void usart_begin(usart_ctx_t* ctx) {
    cli();

    ctx->usart->BAUD = (uint16_t)((float)(F_CPU * 64 / (16 * USART_BAUD_RATE)) + 0.5);
    ctx->usart->CTRLB = USART_TXEN_bm | USART_RXEN_bm;
    ctx->usart->CTRLA |= USART_RXCIE_bm;
}

void usart_write(usart_ctx_t* ctx, const uint8_t* const data, size_t len) {
    usart_write_noblock(ctx, data, len);

    while (ctx->txRemaining > 0);
}

void usart_write_noblock(usart_ctx_t* ctx, const uint8_t* const data, size_t len) {
    while (ctx->txRemaining > 0);

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        ctx->txDataPtr = data;
        ctx->txRemaining = len;
        ctx->usart->CTRLA |= USART_DREIE_bm;
    }
}

void usart_print(usart_ctx_t* ctx, const char* const str) {
    usart_print_noblock(ctx, str);

    while (ctx->txRemaining > 0);
}

void usart_print_noblock(usart_ctx_t* ctx, const char* const str) {
    size_t len = 0;
    while (str[len] != '\0') {
        len++;
    }

    usart_write_noblock(ctx, (const uint8_t*)str, len);
}

uint8_t usart_read(usart_ctx_t* ctx) {
    while (ctx->rxHead == ctx->rxTail);

    uint8_t data = ctx->rxBuffer[ctx->rxHead];
    ctx->rxHead = (ctx->rxHead + 1) & (USART_RX_BUFSIZE - 1);

    return data;
}

bool usart_available(usart_ctx_t* ctx) {
    return ctx->rxHead != ctx->rxTail;
}

#ifndef USART_H
#define USART_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#ifndef USART_RX_BUFSIZE
/// @brief 受信バッファサイズ
#define USART_RX_BUFSIZE 32
#endif

#if (USART_RX_BUFSIZE & (USART_RX_BUFSIZE - 1)) != 0
#error "USART_RX_BUFSIZE must be a power of 2"
#endif

#ifndef USART_BAUD_RATE
/// @brief USARTボーレート
#define USART_BAUD_RATE 115200
#endif

/// @brief USARTコンテキスト
typedef struct usart_ctx_t usart_ctx_t;

/// @brief USART0
extern usart_ctx_t* usart0;

/// @brief USART通信開始
/// @param ctx
/// @note ボーレートはマクロ USART_BAUD_RATE により設定します。フォーマットは 8/N/1 です(MCUリセット直後のまま)。
void usart_begin(usart_ctx_t* ctx);

/// @brief USART送信 (バイナリデータ)
/// @param ctx
/// @param data
/// @param len
/// @note 与えられたデータの送信が完了するまでブロックします。
void usart_write(usart_ctx_t* ctx, const uint8_t* const data, size_t len);

/// @brief USART送信 (バイナリデータ, ノンブロッキング)
/// @param ctx
/// @param data
/// @param len
/// @note 現在実行中の送信がある場合は、完了するまでブロックします。
void usart_write_noblock(usart_ctx_t* ctx, const uint8_t* const data, size_t len);

/// @brief USART送信 (文字列)
/// @param ctx
/// @param str
/// @note 与えられたデータの送信が完了するまでブロックします。
void usart_print(usart_ctx_t* ctx, const char* const str);

/// @brief USART送信 (文字列, ノンブロッキング)
/// @param ctx
/// @param str
/// @note 現在実行中の送信がある場合は、完了するまでブロックします。
void usart_print_noblock(usart_ctx_t* ctx, const char* const str);

/// @brief USART受信
/// @return 受信結果
/// @note データを受信するまでブロックします。
uint8_t usart_read(usart_ctx_t* ctx);

/// @brief 受信バッファ状態確認
/// @return データを受信していれば true が返ります。
bool usart_available(usart_ctx_t* ctx);

#endif /* USART_H */

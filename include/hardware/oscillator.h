#ifndef OSCILLATOR_H
#define OSCILLATOR_H

#include <stdint.h>

/// @brief 波形テーブル
typedef struct {
    int8_t data[256];
} wavetable_t;

/// @brief オシレータコンテキスト
typedef struct oscillator_ctx_t oscillator_ctx_t;

/// @brief (開発用) デフォルトオシレータ
extern oscillator_ctx_t* oscillator;

/// @brief オシレータモジュールを初期化する
/// @param ctx
void oscillator_init(oscillator_ctx_t* ctx);

/// @brief 波形テーブルを設定する
/// @param ctx
/// @param table
void oscillator_set_wavetable(oscillator_ctx_t* ctx, const wavetable_t* table);

/// @brief 周波数を設定する
/// @param ctx
/// @param frequency
void oscillator_set_frequency(oscillator_ctx_t* ctx, uint16_t frequency);

/// @brief オシレータを有効にする
/// @param ctx
void oscillator_enable(oscillator_ctx_t* ctx);

/// @brief オシレータを無効にする
/// @param ctx
void oscillator_disable(oscillator_ctx_t* ctx);

#endif /* OSCILLATOR_H */

#ifndef MODULE_OSCILLATOR_H
#define MODULE_OSCILLATOR_H

#include <stdbool.h>
#include <stdint.h>

#include "wavetable/wavetable.h"

/// @brief オシレータコンテキスト
typedef struct {
    volatile bool enabled;
    volatile uint16_t phase;
    volatile uint16_t step;
    volatile const wavetable_t* table;
} oscillator_ctx_t;

/// @brief オシレータを初期化する
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

/// @brief オシレータの位相を進める
/// @param ctx
/// @return 位相を進める前の時点でDACに出力されるべき値
uint8_t oscillator_step(oscillator_ctx_t* ctx);

#endif /* MODULE_OSCILLATOR_H */

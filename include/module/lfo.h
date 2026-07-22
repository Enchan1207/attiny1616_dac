#ifndef MODULE_LFO_H
#define MODULE_LFO_H

#include <stdbool.h>
#include <stdint.h>

/// @brief LFO波形
typedef enum {
    LFO_WAVEFORM_SQUARE,
    LFO_WAVEFORM_TRIANGLE,
} lfo_waveform_t;

/// @brief LFOコンテキスト
typedef struct {
    uint16_t phase;
    volatile uint16_t step;

    uint8_t divider_counter;
    int8_t current_output;

    lfo_waveform_t waveform;
} lfo_ctx_t;

/// @brief LFO処理結果
typedef struct {
    int8_t value;
    bool updated;
} lfo_result_t;

/// @brief LFOを初期化する
/// @param ctx
void lfo_init(lfo_ctx_t* ctx);

/// @brief LFOの状態を進める
/// @param ctx
/// @return
void lfo_step(lfo_ctx_t* ctx, lfo_result_t* result);

/// @brief LFOの波形を設定する
/// @param ctx
/// @param wfm
void lfo_set_waveform(lfo_ctx_t* ctx, lfo_waveform_t wfm);

/// @brief LFOの周波数を設定する
/// @param ctx
/// @param freq 周波数 (mHz)
void lfo_set_frequency(lfo_ctx_t* ctx, uint16_t freq);

/// @brief LFOの位相をリセットする
/// @param ctx
void lfo_reset_phase(lfo_ctx_t* ctx);

#endif /* MODULE_LFO_H */

#ifndef MODULE_FILTER_H
#define MODULE_FILTER_H

#include <stdint.h>

#include "module/audio.h"

/// @brief フィルタコンテキスト
typedef struct {
    int32_t state_q8;
    uint8_t cutoff;
    uint16_t coefficient_q15;
} filter_ctx_t;

/// @brief フィルタを初期化する
/// @param ctx
void filter_init(filter_ctx_t* ctx);

/// @brief フィルタへ値を入力し、結果を得る
/// @param ctx
/// @return
audio_sample_t filter_step(filter_ctx_t* ctx, audio_sample_t input);

/// @brief フィルタのカットオフパラメータを設定する
/// @param ctx
/// @param cutoff 係数 (0: カットオフ低 / 255: カットオフ高)
void filter_set_cutoff(filter_ctx_t* ctx, uint8_t cutoff);

/// @brief フィルタのカットオフパラメータを取得する
/// @param ctx
/// @return
uint8_t filter_get_cutoff(filter_ctx_t* ctx);

/// @brief フィルタの内部状態を指定したサンプル値へリセットする
/// @param ctx
/// @param value
void filter_reset(filter_ctx_t* ctx, audio_sample_t value);

#endif /* MODULE_FILTER_H */

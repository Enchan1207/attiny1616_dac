#ifndef APP_SYNTH_H
#define APP_SYNTH_H

#include <stdbool.h>
#include <stdint.h>

#include "module/filter.h"
#include "module/lfo.h"
#include "module/voice.h"

/// @brief シンセサイザコンテキスト
typedef struct {
    voice_ctx_t voice;
    filter_ctx_t filter;

    lfo_ctx_t modulation_lfo;
    uint8_t lfo_depth;
} synthesizer_ctx_t;

/// @brief シンセサイザインスタンス0
extern synthesizer_ctx_t* synth0;

/// @brief シンセサイザの利用を開始する
void synthesizer_begin();

/// @brief シンセサイザを初期化する
/// @param ctx
void synthesizer_init(synthesizer_ctx_t* ctx);

/// @brief 現在アクティブなシンセサイザを設定する
/// @param ctx
void synthesizer_set_active_synth(synthesizer_ctx_t* ctx);

/// @brief シンセサイザのLFO深度を設定する
/// @param ctx
/// @param depth
void synthesizer_set_lfo_depth(synthesizer_ctx_t* ctx, uint8_t depth);

#endif /* APP_SYNTH_H */

#ifndef APP_SYNTH_H
#define APP_SYNTH_H

#include <stdbool.h>
#include <stdint.h>

#include "module/filter.h"
#include "module/voice.h"

/// @brief シンセサイザコンテキスト
typedef struct {
    voice_ctx_t voice;
    filter_ctx_t filter;
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

#endif /* APP_SYNTH_H */

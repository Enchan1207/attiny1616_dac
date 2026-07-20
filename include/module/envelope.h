#ifndef MODULE_ENVELOPE_H
#define MODULE_ENVELOPE_H

#include <stdint.h>

#include "module/audio.h"

/// @brief エンベロープの状態
typedef enum {
    ENVELOPE_STATE_STOP,
    ENVELOPE_STATE_ATTACK,
    ENVELOPE_STATE_DECAY,
    ENVELOPE_STATE_SUSTAIN,
    ENVELOPE_STATE_RELEASE,
} envelope_state_t;

/// @brief エンベロープコンテキスト
typedef struct {
    volatile envelope_state_t state;

    volatile uint16_t attack_step;

    volatile uint16_t decay_ms;
    volatile uint16_t decay_step;

    volatile uint16_t sustain;

    volatile uint16_t release_ms;
    volatile uint16_t release_step;

    volatile uint16_t current_level;
} envelope_ctx_t;

/// @brief エンベロープを初期化する
/// @param ctx
void envelope_init(envelope_ctx_t* ctx);

/// @brief Attack時間を設定する
/// @param ctx
/// @param ms
void envelope_set_attack_ms(envelope_ctx_t* ctx, uint16_t ms);

/// @brief Decay時間を設定する
/// @param ctx
/// @param ms
void envelope_set_decay_ms(envelope_ctx_t* ctx, uint16_t ms);

/// @brief Sustainレベルを設定する
/// @param ctx
/// @param value
void envelope_set_sustain(envelope_ctx_t* ctx, uint16_t value);

/// @brief Release時間を設定する
/// @param ctx
/// @param ms
void envelope_set_release_ms(envelope_ctx_t* ctx, uint16_t ms);

/// @brief エンベロープをattack状態に遷移させる
/// @param ctx
void envelope_note_on(envelope_ctx_t* ctx);

/// @brief エンベロープをrelease状態に遷移させる
/// @param ctx
void envelope_note_off(envelope_ctx_t* ctx);

/// @brief エンベロープの状態を進める
/// @param ctx
/// @return 状態を進める前の時点でオシレータに乗算されるべき値
audio_sample_t envelope_step(envelope_ctx_t* ctx);

#endif /* MODULE_ENVELOPE_H */

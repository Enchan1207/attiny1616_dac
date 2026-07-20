#ifndef MODULE_ENVELOPE_H
#define MODULE_ENVELOPE_H

#include <stdint.h>

/// @brief エンベロープのパラメータ
typedef enum {
    ENVELOPE_PARAM_ATTACK,
    ENVELOPE_PARAM_DECAY,
    ENVELOPE_PARAM_SUSTAIN,
    ENVELOPE_PARAM_RELEASE,
} envelope_param_t;

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

    volatile uint8_t attack;
    volatile uint8_t decay;
    volatile uint8_t sustain;
    volatile uint8_t release;
} envelope_ctx_t;

/// @brief エンベロープを初期化する
/// @param ctx
void envelope_init(envelope_ctx_t* ctx);

/// @brief エンベロープのパラメータを取得する
/// @param ctx
/// @param param
/// @return
uint8_t envelope_get(envelope_ctx_t* ctx, envelope_param_t param);

/// @brief エンベロープのパラメータを設定する
/// @param ctx
/// @param param
/// @param value
void envelope_set(envelope_ctx_t* ctx, envelope_param_t param, uint8_t value);

/// @brief エンベロープをattack状態に遷移させる
/// @param ctx
void envelope_note_on(envelope_ctx_t* ctx);

/// @brief エンベロープをrelease状態に遷移させる
/// @param ctx
void envelope_note_off(envelope_ctx_t* ctx);

/// @brief エンベロープの状態を進める
/// @param ctx
/// @return 状態を進める前の時点でオシレータに乗算されるべき値
uint8_t envelope_step(envelope_ctx_t* ctx);

#endif /* MODULE_ENVELOPE_H */

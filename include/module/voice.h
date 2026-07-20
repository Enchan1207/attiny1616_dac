#ifndef MODULE_VOICE_H
#define MODULE_VOICE_H

#include <stdbool.h>
#include <stdint.h>

#include "module/audio.h"
#include "module/envelope.h"
#include "module/oscillator.h"

/// @brief ボイスコンテキスト
typedef struct {
    oscillator_ctx_t osc_ctx;
    envelope_ctx_t evp_ctx;

    uint16_t base_step;
} voice_ctx_t;

/// @brief ボイスの波形
typedef enum {
    VOICE_WAVEFORM_SINE,
    VOICE_WAVEFORM_PULSE,
} voice_waveform_t;

/// @brief ボイスの音階
typedef enum {
    VOICE_NOTE_C,
    VOICE_NOTE_CS,
    VOICE_NOTE_D,
    VOICE_NOTE_DS,
    VOICE_NOTE_E,
    VOICE_NOTE_F,
    VOICE_NOTE_FS,
    VOICE_NOTE_G,
    VOICE_NOTE_GS,
    VOICE_NOTE_A,
    VOICE_NOTE_AS,
    VOICE_NOTE_B,
} voice_note_t;

/// @brief ボイスを初期化する
/// @param ctx
void voice_init(voice_ctx_t* ctx);

/// @brief ボイスのフェーズを進め、サンプルを取得する
/// @param ctx
/// @return このボイスが発声している音
audio_sample_t voice_step(voice_ctx_t* ctx);

/// @brief ボイスの発声を開始する
/// @param ctx
/// @param note
void voice_note_on(voice_ctx_t* ctx, voice_note_t note);

/// @brief ボイスの発声を停止する
/// @param ctx
void voice_note_off(voice_ctx_t* ctx);

/// @brief ボイスの波形を設定する
/// @param ctx
/// @param wfm
void voice_set_waveform(voice_ctx_t* ctx, voice_waveform_t wfm);

/// @brief ボイスのピッチ変調を設定する
/// @param ctx
/// @param modulation
void voice_set_pitch_modulation(voice_ctx_t* ctx, int16_t modulation);

#endif /* MODULE_VOICE_H */

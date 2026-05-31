#ifndef ROTENC_H
#define ROTENC_H

/// @brief ロータリエンコーダの回転方向
typedef enum {
    /// @brief 回転していない
    ROTATION_NONE = 0,

    /// @brief 時計回り
    ROTATION_CW = 1,

    /// @brief 反時計回り
    ROTATION_CCW = 2,
} rotenc_dir_t;

/// @brief ロータリエンコーダコンテキスト
typedef struct rotenc_ctx_t rotenc_ctx_t;

/// @brief ロータリエンコーダインスタンス0 (PB0, 1)
extern rotenc_ctx_t* rotenc0;

/// @brief ロータリエンコーダインスタンス1 (PB2, 3)
extern rotenc_ctx_t* rotenc1;

/// @brief ロータリエンコーダインスタンス2 (PB4, 5)
extern rotenc_ctx_t* rotenc2;

/// @brief ロータリエンコーダモジュールを初期化する
void rotenc_init();

/// @brief ロータリエンコーダの回転状態を取得する
/// @param ctx
/// @return
/// @note この関数を呼び出すと、インスタンスの回転状態はクリア(NONEにセット)されます。
rotenc_dir_t rotenc_get_direction(rotenc_ctx_t* ctx);

#endif /* ROTENC_H */

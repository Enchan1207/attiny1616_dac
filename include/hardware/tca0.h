#ifndef HARDWARE_TCA0_H
#define HARDWARE_TCA0_H

typedef void (*tca0_overflow_handler_t)(void);

/// @brief TCA0 (オシレータ制御用タイマ) を初期化する
void tca0_init();

/// @brief TCA0オーバーフロー時に呼ばれるハンドラを設定する
/// @param handler
void tca0_set_overflow_handler(tca0_overflow_handler_t handler);

#endif /* HARDWARE_TCA0_H */

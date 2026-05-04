#ifndef DAC_H
#define DAC_H

#include <stdint.h>

/// @brief DACコンテキスト
typedef struct dac_ctx_t dac_ctx_t;

/// @brief DAC0
extern dac_ctx_t* dac0;

/// @brief DAC参照電圧
typedef enum {
    VREF_0V55 = 0x0,
    VREF_1V1 = 0x1,
    VREF_2V5 = 0x2,
    VREF_4V3 = 0x3,
    VREF_1V5 = 0x4,
} VREF_VOLTAGE;

/// @brief DACモジュールを初期化する
/// @param ctx
/// @param refVoltage
void dac_init(dac_ctx_t* ctx, VREF_VOLTAGE refVoltage);

/// @brief DACの出力を有効にする
/// @param ctx
void dac_output_enable(dac_ctx_t* ctx);

/// @brief DACの出力を無効にする
/// @param ctx
void dac_output_disable(dac_ctx_t* ctx);

/// @brief DACの出力を設定する
/// @param ctx
/// @param value
void dac_set_value(dac_ctx_t* ctx, uint8_t value);

#endif /* DAC_H */

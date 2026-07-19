#ifndef HARDWARE_DAC0_H
#define HARDWARE_DAC0_H

#include <stdint.h>

/// @brief DAC0 (オシレータ出力用DAC) を初期化する
void dac0_init();

/// @brief DAC0の出力を設定する
/// @param value
void dac0_set(uint8_t value);

#endif /* HARDWARE_DAC0_H */

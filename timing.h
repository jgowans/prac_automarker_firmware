#define STM32F051

#include "stm32f0xx.h"
#include <stdint.h>

int32_t timing_between_patterns(uint8_t pattern0, uint8_t pattern1);
int32_t timing_transition(uint8_t pattern0, uint8_t pattern1);

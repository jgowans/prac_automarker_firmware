#include "timing.h"

/* The function takes two patterns which it will look for on the LEDs
 * It will use a timer counter to measure the time between toggling the patterns
 * It will start looking for a case where pattern0 changes into pattern1, and will start timing
 * When pattern1 then changes back into pattern0 it will return the number of cycles between the change
 * It will return -1 if the change does not occur withing 2 seconds */
int32_t timing_between_patterns(uint8_t pattern0, uint8_t pattern1) {
  // pin should already be configured and be an input
  // using TIM1 as it has a 32-bit CNT
  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
  TIM2->CR1 &= ~TIM_CR1_CEN; // force ensure the time is not counting
  TIM2->CNT = 0;
  TIM2->ARR = 0xFFFFFFFF; // maximum value to count to. About 94 second worth of counting at full resolution!
  TIM2->CR1 |= TIM_CR1_CEN;  // start counting for timeout reasons
  // detect a change from pattern0 to pattern1
  // this is done by hanging while the previous pattern is not pattern 0 or the current pattern is not pattern 1
  uint8_t previous = (uint8_t)(GPIOB->IDR & 0xFF);
  uint8_t current = (uint8_t)(GPIOB->IDR & 0xFF);
  while ( previous != pattern0 || current != pattern1) { //edge not detected - update
    previous = current;
    current = (uint8_t)(GPIOB->IDR & 0xFF);
    if (TIM2->CNT > 96000000) { return -1; } // timeout, 2 seconds
  }
  TIM2->CNT = 0; // reset counter after edge
  // hang until pattern1 changes into pattern0 again
  while ( previous != pattern1 || current != pattern0) { //edge not detected - update
    previous = current;
    current = (uint8_t)(GPIOB->IDR & 0xFF);
    if (TIM2->CNT > 96000000) { return -1; } // timeout, 2 seconds
  }
  // stop timer and return value
  TIM2->CR1 &= ~TIM_CR1_CEN; // force the CEN bit to 0
  uint32_t ticks = TIM2->CNT;
  return ticks;
}


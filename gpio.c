#include "gpio.h"

void gpio_init_inputs(void){
  RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
  GPIOB->PUPDR |= 0xAAAA; // B0 - B7 to pull down
}

void gpio_init_outputs(void) {
  RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
//  GPIOA->MODER |= 0x55; // A0 - A3 to outputs
}

void gpio_init_nrst(void) {
  RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
}

uint8_t gpio_read(void) {
  return (uint8_t)(GPIOB->IDR & 0xFF);
}

void gpio_pin_set(int32_t pin_to_set) {
  // move the setting bit to the correct pin
  GPIOA->BSRR |= 0x1 << pin_to_set;
  // Now going to set pins to outputs
  // multiply the pin number by 2 to get the position which the bits
  // must be at, seeing as each pin is represented by two bits
  // Then, shift a pattern which select those two bits up.
  uint32_t mask_pattern = 0b01 << (pin_to_set << 1);
  GPIOA->MODER |= mask_pattern;
}

void gpio_pin_clear(int32_t pin_to_clear) {
  GPIOA->BSRR |= 0x10000 << pin_to_clear; // the upper half word is for the reset bits
  // Now going to set pins to outputs
  uint32_t mask_pattern = 0b01 << (pin_to_clear << 1);
  GPIOA->MODER |= mask_pattern;
}

void gpio_pin_highz(int32_t pin_to_highz) {
  // Now going to set pins to inputs
  uint32_t mask_pattern = 0b11 << (pin_to_highz << 1);
  GPIOA->MODER &= ~mask_pattern; // mask out all but those pins
}

void gpio_nrst_set(int32_t level) {
  if (level == 0) {
    GPIOC->MODER |= GPIO_MODER_MODER13_0;  // set PC13 to output
    GPIOC->BSRR |= GPIO_BSRR_BR_13;  // pull NRST low
  } else {
    GPIOC->MODER &= ~GPIO_MODER_MODER13_0;  // clear the output bit, making it an input 
  }
}

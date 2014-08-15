#define STM32F051

#include <stdint.h>
#include "stm32f0xx.h"
#include <stdio.h>
#include <stdlib.h>

#include "serial_terminal.h"
#include "gpio.h"

extern int write(int, char*, int);

void main(void) {
  // initialise USART and enable interrupt which will process incomming chars
  serial_terminal_init();
  printf("Serial Terminal Initialised\r\n");
  gpio_init_inputs();
  gpio_init_outputs();
  gpio_init_nrst();
  printf("System initialised. Waiting for input\r\n");
  printf("> ");
  uint8_t *inst[32];
  uint32_t num;
  for(;;) {
  }
}

void WWDG_IRQHandler(void) {
  for(;;);
}

void HardFault_Handler(void) {
  for(;;);
}

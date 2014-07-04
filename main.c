#define STM32F051

#include <stdint.h>
#include "stm32f0xx.h"

#include "serial_terminal.h"

void main(void) {
  // initialise USART and enable interrupt which will process incomming chars
  serial_terminal_init();
  for(;;);
}

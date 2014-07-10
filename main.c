#define STM32F051

#include <stdint.h>
#include "stm32f0xx.h"
#include <stdio.h>
#include <stdlib.h>

#include "serial_terminal.h"

extern int write(int, char*, int);

void main(void) {
  // initialise USART and enable interrupt which will process incomming chars
  serial_terminal_init();
  printf("System initialised. Waiting for input\r\n");
  printf("> ");
  for(;;);
}

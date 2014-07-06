#include "serial_terminal.h"
#include <string.h>
#include <stdio.h>

#define BUFFER_SIZE 64

extern int write(int file, char *ptr, int len);

// static function prototypes
static void process_command(uint8_t *rx_buffer);

void serial_terminal_init(void) {
  // clock to USART1
  RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
  // clock to GPIOA
  RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
  // PA9 and PA10 to AF
  GPIOA->MODER |= GPIO_MODER_MODER9_1;
  GPIOA->MODER |= GPIO_MODER_MODER10_1;
  // remap to correct AF
  GPIOA->AFR[1] |= (1 << (1*4)); // remap pin 9 to AF1
  GPIOA->AFR[1] |= (1 << (2*4)); // remap pin 10 to AF1

  // BRR = fclk / baud = fclk / 115200
  SystemCoreClockUpdate();
  USART1->BRR = SystemCoreClock/115200;
  USART1->CR1 |= USART_CR1_RE; // receive enable
  USART1->CR1 |= USART_CR1_RXNEIE; // receive not empty interrupt enable
  USART1->CR1 |= USART_CR1_TE; // transmit enable
  USART1->CR1 |= USART_CR1_UE; // usart enable

  NVIC_EnableIRQ(USART1_IRQn);
}

void USART1_IRQHandler(void) {
  static uint8_t rx_buffer[BUFFER_SIZE];
  static uint32_t buffer_pointer = 0;
  
  // ack the interrput by reading the received data
  uint8_t received_char = USART1->RDR;
  if (received_char = '\n') { //"Enter"
    rx_buffer[buffer_pointer] = 0; // finalise the string by terminating it with a 0
    process_command(rx_buffer);
    buffer_pointer = 0; // new string to buffer
  } else { // new char arrived. Add it to the buffer
    rx_buffer[buffer_pointer++] = received_char;
  }
}

static void process_command(uint8_t *rx_buffer) {
  if (strcmp("PING", rx_buffer)) {
    write(0, "PONG! from write", 16);
    printf("PONG! from printf");
  }
}




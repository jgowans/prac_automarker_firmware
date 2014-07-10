#include "serial_terminal.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#define BUFFER_SIZE 64

// static function prototypes
static void process_command(uint8_t *rx_buffer);

void serial_terminal_init(void) {
  // disable buffering to prevent having to send a newline to flush
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);

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
  write(0, &received_char, 1);
  if (received_char == '\r') { //"Enter"
    write(0, "\n", 1); 
    rx_buffer[buffer_pointer] = 0; // finalise the string by terminating it with a 0
    process_command(rx_buffer);
    buffer_pointer = 0; // ready to buffer fresh string
    write(0, "> ", 2);  // ready for next command
  } else if (received_char == '\b') { //backspace
    rx_buffer[--buffer_pointer] = 0; // clear the previous char
    write(0, " \b", 3); // overwrite with space and then move back
  }
  else { // new char arrived. Add it to the buffer
    rx_buffer[buffer_pointer++] = received_char;
  }
}

static void process_command(uint8_t *rx_buffer) {
  if (strcmp("PING", rx_buffer) == 0) {
    printf("PONG!\r\n");
  } else {
    printf("ERROR 1: no such command: \"%s\"\r\n", rx_buffer);
  }
}



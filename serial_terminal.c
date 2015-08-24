#include "serial_terminal.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "gpio.h"
#include "timing.h"
#include "mcp4921.h"

#define BUFFER_SIZE 64

// static function prototypes
static uint32_t process_command(char *rx_buffer);

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
  static int8_t rx_buffer[BUFFER_SIZE];
  static uint32_t buffer_pointer = 0;
  
  // ack the interrput by reading the received data
  int8_t received_char = USART1->RDR;
  write(0, &received_char, 1);
  if (received_char == '\r') { //"Enter"
    write(0, "\n", 1); 
    rx_buffer[buffer_pointer] = 0; // finalise the string by terminating it with a 0
    buffer_pointer = 0; // ready to buffer fresh string
    process_command((char*)rx_buffer);
  } else if (received_char == '\b') { //backspac
    if (buffer_pointer > 0) {
      rx_buffer[--buffer_pointer] = 0; // clear the previous char
      write(0, " \b", 3); // overwrite with space and then move back
    }
  }
  else { // new char arrived. Add it to the buffer
    rx_buffer[buffer_pointer++] = received_char;
  }
  // check for impending overrun
  if (buffer_pointer >= BUFFER_SIZE) {
    printf("ERROR 3: buffer overrun\r\n");
    buffer_pointer = 0;
  }
}

static uint32_t process_command(char *rx_buffer) {
  char instruction[BUFFER_SIZE];
  int operand;
  if (sscanf(rx_buffer, "%s %i", instruction, &operand) != 2) {
    printf("ERROR 1: invalid command syntax: \"%s\"\r\n", rx_buffer);
    return -1;
  }
  if (strcmp("PING", instruction) == 0) {
    for (uint32_t i = 0; i < operand; i++) {
      printf("PONG!\r\n");
    }
  } 
  else if (strcmp("GPIO_READ", instruction) == 0) {
    printf("INPUTS: %X\r\n", gpio_read());
  } 
  else if (strcmp("GPIO_SET", instruction) == 0) {
    gpio_pin_set(operand);
  } 
  else if (strcmp("GPIO_CLEAR", instruction) == 0) {
    gpio_pin_clear(operand);
  } 
  else if (strcmp("GPIO_HIGHZ", instruction) == 0) {
    gpio_pin_highz(operand);
  } 
  else if (strcmp("PATTERN_TIMING", instruction) == 0) {
    int cycles = timing_between_patterns((uint8_t)(operand & 0xFF), (uint8_t)((operand >> 8) & 0xFF));
    printf("TIMING: %i\r\n", cycles);
  } 
  else if (strcmp("PATTERN_TRANSITION", instruction) == 0) {
    int cycles = timing_transition((uint8_t)(operand & 0xFF), (uint8_t)((operand >> 8) & 0xFF));
    printf("TRANSITION: %i\r\n", cycles);
  } 
  else if (strcmp("NRST", instruction) == 0) {
    gpio_nrst_set((int8_t)operand);
  } 
  else if (strcmp("DAC", instruction) == 0) {
    mcp4921_set_value(((uint8_t)((operand >> 8) & 0xFF)), ((uint8_t)((operand) & 0xFF)));
  } 
  else {
    printf("ERROR 2: no such command: \"%s\"\r\n", rx_buffer);
    return -1; // prevent printing of OK
  }
  printf("OK\r\n");
  return 0;
}


#define STM32F051

#include <stdint.h>
#include "stm32f0xx.h"
#include <stdio.h>
#include <stdlib.h>

#include "serial_terminal.h"
#include "gpio.h"
#include "mcp4921.h"

extern int write(int, char*, int);

int main(void) {
    // initialise USART and enable interrupt which will process incomming chars
    serial_terminal_init();
    printf("Serial Terminal Initialised\r\n");
    gpio_init_inputs();
    gpio_init_outputs();
    gpio_init_nrst();
    mcp4921_init();
    printf("An int is %u bytes big\n", sizeof(int));
    printf("System initialised. Waiting for input\r\n");
    for(;;) {
    }
    return 0;
}

void WWDG_IRQHandler(void) {
    for(;;);
}

void HardFault_Handler(void) {
    for(;;);
}

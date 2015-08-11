#include "dac.h"

void dac_init(void) {
    RCC->APB1ENR |= RCC_APB1ENR_DACEN;
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    GPIOA->MODER |= GPIO_MODER_MODER4;// PA4 as analogue
    DAC->CR |= DAC_CR_EN1;
    DAC->CR |= DAC_CR_BOFF1; //disable the buffer to increase voltage swing
    dac_drive_select_init();
}

void dac_drive_select_init(void) {
    // set PA7 and PA11 to outputs
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    GPIOA->MODER |= GPIO_MODER_MODER7_0;
    GPIOA->MODER |= GPIO_MODER_MODER11_0;
    // default DAC channels off
    GPIOA->BSRR = GPIO_BSRR_BR_7;
    GPIOA->BSRR = GPIO_BSRR_BR_11;

}

void dac_drive_select(uint8_t channel, uint8_t state) {
    if (channel == 0) { //PA7
        if (state == 0) {
            GPIOA->BSRR = GPIO_BSRR_BR_7;
        } else {
            GPIOA->BSRR = GPIO_BSRR_BS_7;
        }
    } 
    if (channel == 1) {  // PA11
        if (state == 0) {
            GPIOA->BSRR = GPIO_BSRR_BR_11;
        } else {
            GPIOA->BSRR = GPIO_BSRR_BS_11;
        }
    }
}

void dac_write(uint8_t to_output) {
    DAC->DHR8R1 = to_output;
}

void dac_buffer(uint32_t state) {
    if (state == 0) {
        DAC->CR |= DAC_CR_BOFF1;
    }
    else {
        DAC->CR &= ~(DAC_CR_BOFF1);
    }
}

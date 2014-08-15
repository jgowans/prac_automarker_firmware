#include <stdint.h>
#include "stm32f0xx.h"

void gpio_init_inputs(void);
void gpio_init_outputs(void);

/* Provides clock to GPIOC, necessary for NRST control pin */
void gpio_init_nrst(void);
uint8_t gpio_read(void);
void gpio_pin_set(int32_t pin_to_set);
void gpio_pin_clear(int32_t pin_to_clear);
void gpio_pin_highz(int32_t pin_to_highz);

/* Set level to 0 to pull pin low, else pin becomes an input */
void gpio_nrst_set(int32_t level);

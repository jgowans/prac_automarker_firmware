#include <stdint.h>
#include "stm32f0xx.h"

/* enables clock to port and peripheral
 * sets pin to analogue mode
 * sets up peripheral
 */
void dac_init(void);

/* places a value on the DAC output pin */
void dac_write(uint8_t to_output);

void dac_drive_select_init(void);

void dac_drive_select(uint8_t channel, uint8_t state);

/* state = 0: buffer off. state = 1: buffer on */
void dac_buffer(uint32_t state);

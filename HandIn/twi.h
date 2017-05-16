#ifndef TWI_H_
#define TWI_H_

#include <avr/io.h>

uint8_t twi_init(long sclFrequency);
void twi_write(uint8_t device, uint8_t address, uint8_t *data, int len, void (*callback)(uint8_t*));
void twi_read(uint8_t device, uint8_t address, int len, void (*callback)(uint8_t*));

#endif /* TWI_H_ */
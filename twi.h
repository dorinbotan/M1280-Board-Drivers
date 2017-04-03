#ifndef TWI_H_
#define TWI_H_

#include <avr/interrupt.h>
#include <util/twi.h>
#include "mcu.h"

#define TWI_SendStart()		(TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN) | (1 << TWIE))
#define TWI_SendStop()		(TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN) | (1 << TWIE))
#define TWI_SendTransmit()	(TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE))
#define TWI_SendACK()		(TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE) | (1 << TWEA))
#define TWI_SendNACK()		(TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE))

typedef enum {
	Sending,
	Receiving
} TWIMode;
TWIMode mode;

uint8_t deviceId;
uint8_t reg;
uint8_t *TXBuffer;
volatile int TXIndex;
int TXSize;

uint8_t twi_init(long sclFrequency);
void twi_write(uint8_t device, uint8_t address, uint8_t *data, int len);
uint8_t* twi_read(uint8_t device, uint8_t address, int len);

#endif /* TWI_H_ */
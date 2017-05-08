#include "twi.h"

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
void (*callbackFunction)(uint8_t*);

ISR(TWI_vect)
{
	switch(TW_STATUS)
	{
		case TW_START:
			//usart0_send_arr("S", 1);
			TWDR = (deviceId << 1);
			TWI_SendTransmit();
			break;
		case TW_REP_START:
			//usart0_send_arr("2", 1);
			TWDR = (deviceId << 1) | 0x01;
			TWI_SendTransmit();
		case TW_MT_SLA_ACK:
			//usart0_send_arr("3", 1);
			TWDR = reg;
			TWI_SendTransmit();
			break;
		case TW_MR_SLA_ACK:
			//usart0_send_arr("4", 1);
			TWI_SendACK();
			break;
		case TW_MT_DATA_ACK:
			//usart0_send_arr("5", 1);
			if(mode == Sending)	{
				if(TXIndex < TXSize) {
					TWDR = TXBuffer[TXIndex++];
					TWI_SendTransmit();
				} else {
					TWI_SendStop();
					if(callbackFunction)
						callbackFunction(0);
				}
			} else {
				TWI_SendStart();
			}
			break;
		case TW_MR_DATA_ACK:
			//usart0_send_arr("6", 1);
			TXBuffer[TXIndex++] = TWDR;
			if(TXIndex < TXSize) {
				TWI_SendACK();
			} else {
				TWI_SendNACK();
				while((TWCR & (1 << TWINT)) == 0);
				TWI_SendStop();
				if(callbackFunction)
					callbackFunction(TXBuffer);
				free(TXBuffer);
			}
			break;
		// Loosing Arbitration (wail a sec and try again)
		case TW_MT_ARB_LOST:
		// Trying to access non-existing device here !!!
		case TW_MT_SLA_NACK:
		case TW_MR_SLA_NACK:
		// Data not transmitted here !!!
		case TW_MT_DATA_NACK:
		case TW_MR_DATA_NACK:
			break;
	}
}

uint8_t twi_init(long sclFrequency)
{
	// Set no pre-scaling
	TWSR = !((1 << TWPS0) | (1<< TWPS1));
	// Set bit rate
	TWBR = (F_CPU / sclFrequency - 16) / 2;
	// Enable TWI and interrupt
	TWCR = (1 << TWEN) | (1 << TWIE);
	return TWBR;
}

void twi_write(uint8_t device, uint8_t address, uint8_t *data, int len, void (*callback)(uint8_t*))
{
	//usart0_send_arr(" W-", 3);
	mode = Sending;
	deviceId = device;
	reg = address;
	TXBuffer = data;
	TXIndex = 0;
	TXSize = len;
	callbackFunction = callback;

	TWI_SendStart();
}

void twi_read(uint8_t device, uint8_t address, int len, void (*callback)(uint8_t*))
{
	//usart0_send_arr(" R-", 3);
	mode = Receiving;
	deviceId = device;
	reg = address;
	TXBuffer = (uint8_t*)malloc(len);
	TXIndex = 0;
	TXSize = len;
	callbackFunction = callback;

	TWI_SendStart();
}
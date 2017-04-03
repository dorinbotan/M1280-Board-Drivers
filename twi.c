#include "twi.h"

ISR(TWI_vect)
{
	switch(TW_STATUS)
	{
		case TW_START:
			TWDR = (deviceId << 1);
			TWI_SendTransmit();
			break;
		case TW_REP_START:
			TWDR = (deviceId << 1) | 0x01;
			TWI_SendTransmit();
		case TW_MT_SLA_ACK:
			TWDR = reg;
			TWI_SendTransmit();
			break;
		case TW_MR_SLA_ACK:
			// Error here ???
			TWI_SendACK();
			break;
		case TW_MT_DATA_ACK:
			switch(mode)
			{
				case Sending:
					if(TXIndex < TXSize)
					{
						TWDR = TXBuffer[TXIndex++];
						TWI_SendTransmit();
					}
					else
					{
						TWI_SendStop();
					}
					break;
				case Receiving:
					TWI_SendStart();
					break;
			}
			break;
		case TW_MR_DATA_ACK:
			TXBuffer[TXIndex++] = TWDR;
			if(TXIndex < TXSize)
			{
				TWI_SendACK();
				PORTH = 0xFF;
			}
			else
			{
				TWI_SendNACK();
				TWI_SendStop();
				free(TXBuffer);
			}
			break;		
			// Loosing Arbitration (wail a sec and try again)
		case TW_MT_ARB_LOST:
			// Trying to access non-existing device here !!!
		case TW_MT_SLA_NACK:
		case TW_MR_SLA_NACK:
			break;
			// Data not transmitted here !!!
		case TW_MT_DATA_NACK:
		case TW_MR_DATA_NACK:
			break;
		break;
	}
}

uint8_t twi_init(long sclFrequency)
{
	// Set no pre-scaling
	TWSR = !((1 << TWPS0) | (1 << TWPS1));
	// Set bit rate
	TWBR = ((F_CPU / sclFrequency) - 16) / 2;
	// Enable TWI and interrupt
	TWCR = (1 << TWEN) | (1 << TWIE);
	return TWBR;
}

void twi_write(uint8_t device, uint8_t address, uint8_t *data, int len)
{
	mode = Sending;
	deviceId = device;
	reg = address;
	TXBuffer = data;
	TXIndex = 0;
	TXSize = len;

	TWI_SendStart();
}

uint8_t* twi_read(uint8_t device, uint8_t address, int len)
{
	mode = Receiving;
	deviceId = device;
	reg = address;
	TXBuffer = (uint8_t*)malloc(len);
	TXIndex = 0;
	TXSize = len;

	TWI_SendStart();
}
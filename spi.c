#include "spi.h"
#include <avr/interrupt.h>

void (*callbackFunction)(uint8_t*);

ISR(SPI_STC_vect)
{
	callbackFunction(SPDR);
}

void spi_init(uint8_t data_order,
			  uint8_t mode,
			  uint8_t polarity,
			  uint8_t phase)
{
	//			MOSI		 SCK		  SS
	DDRB |= _BV(PINB2) | _BV(PINB1) | _BV(PINB0);

	SPCR = (
			_BV(SPIE) |
			_BV(SPE) |
			(data_order << DORD) |
			(mode << MSTR) |
			(polarity << CPOL) |
			(phase << CPHA) |
			_BV(SPR1) |
			_BV(SPR0)
		   );
}

void spi_send_byte(uint8_t value, void (*callback)(uint8_t*))
{
	callbackFunction = callback;
	SPDR = value;
}
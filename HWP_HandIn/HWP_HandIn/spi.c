#include "spi.h"
#include <avr/interrupt.h>

void spi_init(uint8_t data_order,
			  uint8_t mode,
			  uint8_t polarity,
			  uint8_t phase)
{
	uint8_t has_callback_function = 0;

	//			MOSI		SCK			SS
	DDRB |= _BV(PINB2) | _BV(PINB1) | _BV(PINB0);

	SPCR = (
			(has_callback_function << SPIE) |
			_BV(SPE) |
			(data_order << DORD) |
			(mode << MSTR) |
			(polarity << CPOL) |
			(phase << CPHA) |
			_BV(SPR1) |
			_BV(SPR0)
		   );
}

uint8_t spi_send_byte(uint8_t value)
{
	uint8_t tmp = SREG;
	cli();

	SPDR = value;

	while(!(SPSR & _BV(SPIF))){}

	uint8_t received = SPDR;
	
	sei();
	SREG = tmp;

	return received;
}
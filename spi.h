#ifndef SPI_H_
#define SPI_H_

#include <avr/io.h>
#include <avr/interrupt.h>

#define SPI_DATA_ORDER_LSB_FIRST 1
#define SPI_DATA_ORDER_MSB_FIRST 0
#define SPI_MODE_MASTER 1
#define SPI_MODE_SLAVE 0
#define SPI_CLOCK_POLARITY_HIGH 1
#define SPI_CLOCK_POLARITY_LOW 0
#define SPI_CLOCK_PHASE_SAMPLE_LOADING 0
#define SPI_CLOCK_PHASE_SAMPLE_TRAILING 1

static void(*_callback_function)() = 0;

void spi_init(uint8_t data_order, 
			  uint8_t mode, 
			  uint8_t clock_polarity, 
			  uint8_t clock_phase, 
			  void(*callback_function)())
{
	uint8_t has_callback_function = 0;

	_callback_function = callback_function;

	if(_callback_function)
		has_callback_function = 1;

	//			MOSI		SCK			SS
	DDRB |= _BV(DDB2) | _BV(DDB1) | _BV(DDB0);

	SPCR = (
			(has_callback_function << SPIE) |
			_BV(SPE) |
			(data_order << DORD) |
			(mode << MSTR) |
			(clock_polarity << CPOL) |
			(clock_phase << CPHA) |
			_BV(SPR1) |
			_BV(SPR0)
			);
}

// TODO: send burst
uint8_t spi_send_byte(uint8_t value)
{
	uint8_t received_byte;
	uint8_t tmp = SREG;
	cli();

	SPDR = value;

	while(!(SPSR & _BV(SPIF))){}

	received_byte = SPDR;

	SREG = tmp;
	return received_byte;
}

#endif /* SPI_H_ */
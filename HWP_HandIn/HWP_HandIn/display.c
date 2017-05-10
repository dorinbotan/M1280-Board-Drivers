#include "display.h"
#include "spi.h"
#include "mcu.h"
#include <avr/interrupt.h>
#include <math.h>

const uint16_t DISPLAY[4] = { PL3, PL0, PL1, PL2 };
const uint8_t DIGIT[10] = { 0xFC, 0x60, 0xDA, 0xF2, 0x66, 0xB6, 0xBE, 0xE0, 0xFE, 0xF6 };

uint8_t init = 0;
int8_t value_by_digits[4] = { -1, -1, -1, -1 };
uint8_t current = 0;

ISR(TIMER0_OVF_vect)
{
	uint8_t digit_value = value_by_digits[current];
	if(digit_value != -1) {
		spi_init(SPI_MODE_MASTER,
				 SPI_DATA_ORDER_LSB_FIRST,
				 SPI_CLOCK_POLARITY_LOW,
				 SPI_CLOCK_PHASE_LEADING);

		spi_send_byte(digit_value);
		// Display value
		CLEAR_BIT(PORTL, DISPLAY[current]);
		CLOCK_PIN(PORTK, PK3);
		SET_BIT(PORTL, current);
		current++;
		current %= 4;
	}
}

void _init_7_segment()
{
	DDRB |= _BV(DDB3);
	DDRK |= _BV(DDK3);
	DDRL |= _BV(DDL3) | _BV(DDL2) | _BV(DDL1) | _BV(DDL0);

	PORTB |= _BV(PB3);

	TCCR0B |= _BV(CS01) | _BV(CS00);		// 64 prescaler
	TIMSK0 = _BV(TOIE0);					// Enable timer compare interrupt (8 bit)
}

void display_7_segment(float value, uint8_t no_of_decimals)
{
	if(!init) {
		init = 1;
		_init_7_segment();
	}

	uint16_t real = value;
	uint16_t decimal = (value - real) * (pow(10, no_of_decimals) + 1);
	
	real = real * pow(10, no_of_decimals) + decimal;
	
	uint16_t n = real;
	uint8_t rLen = 0;
	while(n != 0 || rLen == 0)	{
		n /= 10;
		++rLen;
	}
	
	if(rLen > 4) {
		real /= pow(10, rLen - 4);
	}

	DDRH = 0xFF;
	PORTH = rLen;

	for(int i = 0; i < 4; i++) {
		value_by_digits[i] = DIGIT[real % 10] | (i == (4 - rLen));
		real /= 10;
	}
}
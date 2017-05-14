#include "display.h"
#include "spi.h"
#include "mcu.h"
#include <avr/interrupt.h>

const uint8_t DISPLAY[4] = { PL3, PL0, PL1, PL2 };
const uint8_t DIGIT[10] = { 0xFC, 0x60, 0xDA, 0xF2, 0x66, 0xB6, 0xBE, 0xE0, 0xFE, 0xF6 };

uint8_t init = 0;
int8_t value_by_digits[4] = { -1, -1, -1, -1 };
uint8_t current = 0;

void _trigger(uint8_t *dummy)
{
	// Display value
	CLEAR_BIT(PORTL, DISPLAY[current]);
	CLOCK_PIN(PORTK, PK3);
	SET_BIT(PORTL, current);
	current = (curent + 1) % 4;
}

ISR(TIMER0_OVF_vect)
{
	TCNT0 = 136;
	uint8_t digit_value = value_by_digits[current];
	if(digit_value != -1) {
		spi_init(SPI_MODE_MASTER,
				 SPI_DATA_ORDER_LSB_FIRST,
				 SPI_CLOCK_POLARITY_LOW,
				 SPI_CLOCK_PHASE_LEADING);

		spi_send_byte(digit_value, _trigger);
	}
}

void _init_7_segment()
{
//	DDRB |= _BV(DDB3);
	DDRK |= _BV(DDK3);
	DDRL |= _BV(DDL3) | _BV(DDL2) | _BV(DDL1) | _BV(DDL0);

	PORTB |= _BV(PB3);

	TCNT0 = 136;							// 120 ticks (240 Hz)
	TCCR0B |= _BV(CS02);					// 256 prescaler
	TIMSK0 = _BV(TOIE0);					// Enable timer compare interrupt (8 bit)
}

void display_7_segment(float value, uint8_t no_of_decimals)
{
	if(!init) {
		init = 1;
		_init_7_segment();
	}

	// Math.pow gives wrong results
	uint16_t pow_ten[] = { 1, 10, 100, 1000, 10000 };
	uint32_t real = value * pow_ten[no_of_decimals];

	for(int i = 0; i < 4; i++) {
		uint8_t digit = (real / pow_ten[i]) % 10;
		value_by_digits[i] = DIGIT[digit] | (i == no_of_decimals && no_of_decimals);
	}
}

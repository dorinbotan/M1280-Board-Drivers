#include "display.h"
#include "spi.h"
#include "mcu.h"

const uint16_t DISPLAY[4] = { PL3, PL0, PL1, PL2 };
const uint8_t DIGIT[10] = { 0xFC, 0x60, 0xDA, 0xF2, 0x66, 0xB6, 0xBE, 0xE0, 0xFE, 0xF6 };

uint8_t init = 0;
int8_t value_by_digits[4] = { -1, -1, -1, -1 };
uint8_t current = 0;

ISR(TIMER0_OVF_vect)
{
	uint8_t value_digit = value_by_digits[current];
	if(value_digit != -1) {
		spi_init(SPI_MODE_MASTER,
				 SPI_DATA_ORDER_LSB_FIRST,
				 SPI_CLOCK_POLARITY_LOW,
				 SPI_CLOCK_PHASE_SAMPLE_LEADING,
				 0);
		spi_send_byte(value_digit);
		_trigger_display();
	}
}

void _display_init()
{
	DDRB |= _BV(DDB3);
	DDRK |= _BV(DDK3);
	DDRL |= _BV(DDL3) | _BV(DDL2) | _BV(DDL1) | _BV(DDL0);

	PORTB |= _BV(PB3);
	// Timer
	TCCR0B |= _BV(CS01) | _BV(CS00);
	TIMSK0 = _BV(TOIE0);
}

void _trigger_display()
{
	CLEAR_BIT(PORTL, DISPLAY[current]);
	CLOCK_PIN(PORTK, PK3);
	SET_BIT(PORTL, current);

	current++;
	current %= 4;
}

void display_value(unsigned short value)
{
	if(!init) {
		_display_init();
		init = 1;
	}

	unsigned short n = value;
	uint8_t count = 0;
	while(n != 0 || count == 0)	{
		n /= 10;
		++count;
	}

	for(int i = 0; i < 4; i++) {
		if(i < count)
			value_by_digits[i] = DIGIT[value % 10];
		else
			value_by_digits[i] = 0;
		value /= 10;
	}
}
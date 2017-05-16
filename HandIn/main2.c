#include <avr/io.h>
#include <avr/interrupt.h>
#include "mcu.h"

const uint8_t DIGIT[10] = { 0xFC, 0x60, 0xDA, 0xF2, 0x66, 0xB6, 0xBE, 0xE0, 0xFE, 0xF6 };

uint8_t init = 0;
int8_t value_by_digits[4] = { -1, -1, -1, -1 };
uint8_t current = 0;

ISR(TIMER0_OVF_vect)
{
	TCNT0 = 136;

	uint8_t digit_value = value_by_digits[current];
	if(digit_value != -1) {
		for (uint8_t i = 0; i < 8; i++) {
			if (digit_value & 0x01)
				SET_BIT(PORTB, PB2);
			else
				CLEAR_BIT(PORTB, PB2);

			CLOCK_PIN(PORTB, PB1);
			digit_value >>= 1;
		}

		PORTL &= ~0x0F;
		CLOCK_PIN(PORTK, PK3);
		SET_BIT(PORTL, 3 - current);

		current = (current + 1) % 4;
	}
}

void init_7_segment()
{
	DDRB = _BV(PB3) | _BV(PB2) | _BV(PB1);	// MR, DS, SHCP
	DDRK = _BV(PK3);						// STCP to output
	DDRL = 0x0F;							// PL0 .. PL3
	PORTB = _BV(PB3);						// MR

	// Refreshes display
	TCNT0 = 136;							// 120 ticks (240 Hz)
	TCCR0B = _BV(CS02);						// 256 prescaler
	TIMSK0 = _BV(TOIE0);					// Enable timer compare interrupt (8 bit)
}

void display_7_segment(float value, uint8_t no_of_decimals)
{
	if(!init) {
		init = 1;
		init_7_segment();
	}

	uint16_t pow_ten[] = { 1, 10, 100, 1000 };
	uint32_t real = value * pow_ten[no_of_decimals];

	for(int i = 0; i < 4; i++) {
		uint8_t digit = (real / pow_ten[3 - i]) % 10;
		value_by_digits[i] = DIGIT[digit] | (i == 3 - no_of_decimals);
	}
}

float counter = 0;
ISR(TIMER1_OVF_vect)
{
	TCNT1 = 58336;
	display_7_segment(counter, 2);
	counter += 0.01;
}

int main(void)
{
	sei();

	display_7_segment(123.4, 1);

	// Updates shown value
	TCNT1 = 58336;					// 7200 ticks (4 Hz)
	TCCR1B |= _BV(CS12);			// 256 prescaler
	TIMSK1 |= _BV(TOIE1);			// enable overflow interrupt

	while (1);
}
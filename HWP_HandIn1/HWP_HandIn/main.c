#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>

static volatile uint8_t disp_value[4];
static volatile uint8_t digit;

const uint8_t DIGIT[10] = { 0xFC, 0x60, 0xDA, 0xF2, 0x66, 0xB6, 0xBE, 0xE0, 0xFE, 0xF6 };

ISR (TIMER1_COMPA_vect) 
{
	uint8_t value = disp_value[digit];
	for (int i = 0; i < 8; i++) {
		if (value & 1) {
			PORTB |= _BV(PB2);
		}
		else {
			PORTB &= ~_BV(PB2);
		}
		PORTB |= _BV(PB1);
		PORTB &= ~_BV(PB1);
		value >>= 1;
	}

	PORTL &= ~0x0F;

	PORTK |= _BV(PK3);
	PORTK &= ~_BV(PK3);

	PORTL = _BV(3 - digit);

	digit = (digit + 1) % 4;
}

void init_7_segment() 
{
	TIMSK1 = _BV(OCIE1A); //Turn timer 1 interrupt

	OCR1A = 7200/240; //Interrupt frequency to 240 Hz
	TCCR1B |= _BV(WGM12); //Run in CTC mode

	DDRB = _BV(PB3) | _BV(PB2) | _BV(PB1); //MR, DS, SHCP to output
	DDRK = _BV(PK3); //STCP to output
	DDRL = _BV(PL0) | _BV(PL1) | _BV(PL2) | _BV(PL3); //PL0 .. PL3 to output
	PORTB = _BV(PB3); // MR to high
}

void display_7_segment(float value, uint8_t no_of_decimals) 
{
	// convert float value to array of values which represent each digit
	uint16_t n = round(value*pow(10, no_of_decimals));
	for (int j = 3; j >=0; j--) {
		disp_value[j] = DIGIT[n % 10];
		n = div(n, 10).quot;
	}

	disp_value[3-no_of_decimals] |= 1; // place a decimal point
	TCCR1B |= _BV(CS12) | _BV(CS10); //start timer, clk / 1024
}

int main(void)
{
	sei();
	init_7_segment();
	display_7_segment(123.4, 1);

    while (1);
}


#include <avr/io.h>
#include <avr/interrupt.h>
#include "display.h"
#include "DS1337.h"

void _display_value(uint8_t *value)
{
	// From BCD
	uint8_t second = (value[0] >> 4) * 10 + (value[0] & 0x0F);
	uint8_t minute = (value[1] >> 4) * 10 + (value[1] & 0x0F);
	float toDisplay = minute + (float)second / 100;

	display_7_segment(toDisplay, 2);
}

ISR(TIMER1_OVF_vect)
{
	rtc_getTime(_display_value);
	TCNT1 = 51136;
}

void _timer_setup(uint8_t *dummy)
{
	TCNT1 = 51136;						// 14400 ticks (2 Hz)
	TCCR1B |= _BV(CS12);				// 256 prescaler
	TIMSK1 |= _BV(TOIE1);				// enable overflow interrupt (16 bit)
}

void _set_time(uint8_t *dummy)
{
	rtc_setTime(0, 0, 0, _timer_setup);
}

int main(void)r
{
	sei();

	rtc_init(_set_time);
	
    while (1);
}
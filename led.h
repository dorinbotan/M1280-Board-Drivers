#ifndef LED_H_
#define LED_H_

void init_leds()
{
	DDRH = 0xFF;
	PORTH = 0x00;
}

// led_no [1..8], state [0..1]
void set_led(uint8_t led_no, uint8_t state)
{
	if(led_no < 9 && led_no > 0)
		if(state)
			PORTH |= 1 << (8 - led_no);
		else
			PORTH &= ~(1 << (8 - led_no));
}

#endif /* LED_H_ */

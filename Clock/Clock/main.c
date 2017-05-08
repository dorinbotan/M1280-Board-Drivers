#include <avr/io.h>
#include <avr/interrupt.h>
#include "display.h"
#include "DS1337.h"
#include "twi.h"
//#include "USART/usart.h"
//#include "USART/usart0.h"

uint8_t fromBcd(uint8_t value)
{
	return (value >> 4) * 10 + (value & 0x0F);
}

uint8_t status = 0;

void a1(uint8_t *value)
{
	status = 1;
	display_value(fromBcd(value[1]) * 100 + fromBcd(value[0]));
}

void a0(uint8_t *value) { status = 1; }

int main(void)
{
	sei();

//	char buf;
//	usart0_init();
//	usart0_send_arr("\r\n", 2);

//	twi_init(100000L);
	rtc_setTime(10, 21, 0, a0);

	while(1)
	{
//		if(usart0_recv_dequeue(&buf))
//			usart0_send(buf);
		if(status)
		{
			status = 0;
			rtc_getTime(a1);
		}
		else if(status)
		{
			rtc_getTime(a1);
		}
	}
}
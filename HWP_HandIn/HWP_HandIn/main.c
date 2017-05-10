#include <avr/io.h>
#include <avr/interrupt.h>
#include "display.h"

int main(void)
{
	sei();

	display_7_segment(1.234, 0);

    while (1) 
    {
    }
}

